#include "YamlConfigParser.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

YamlConfigParser::YamlConfigParser(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    std::string currentSection;
    int prevIndent = 0;

    while (std::getline(file, line)) {
        int currentIndent = countIndentation(line);
        line = trim(line);

        if (line.empty() || line[0] == '#') continue;

        // Section principale (ex: "config_serveur:")
        if (currentIndent == 0 && line.back() == ':') {
            currentSection = line.substr(0, line.size() - 1);
            prevIndent = currentIndent;
            continue;
        }

        // Sous-section (ex: "ServeurRest:")
        if (currentIndent > prevIndent && line.back() == ':') {
            // Pour simplifier, on stocke en clé composite
            std::string subsection = line.substr(0, line.size() - 1);
            prevIndent = currentIndent;
            continue;
        }

        // Paire clé-valeur
        size_t colonPos = line.find(':');
        if (colonPos != std::string::npos) {
            std::string key = trim(line.substr(0, colonPos));
            std::string value = trim(line.substr(colonPos + 1));

            // Nettoyer les guillemets
            if (!value.empty() && value.front() == '\'' && value.back() == '\'') {
                value = value.substr(1, value.size() - 2);
            }

            if (!currentSection.empty()) {
                configData[currentSection][key] = value;
            }
        }
    }
}

// Méthodes utilitaires
std::string YamlConfigParser::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t");
    return str.substr(first, (last - first + 1));
}

int YamlConfigParser::countIndentation(const std::string& line) {
    size_t i = 0;
    while (i < line.size() && (line[i] == ' ' || line[i] == '\t')) ++i;
    return i;
}

void YamlConfigParser::parseLine(std::string line, std::string& currentSection, int& currentIndent) {
    // Nettoyage des caractères de fin de ligne Windows (\r)
    line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
    
    // Comptage de l'indentation
    int newIndent = 0;
    while (newIndent < line.size() && (line[newIndent] == ' ' || line[newIndent] == '\t')) {
        newIndent++;
    }
    
    std::string trimmedLine = line.substr(newIndent);
    
    if (trimmedLine.empty() || trimmedLine[0] == '#') {
        return;
    }

    // Gestion des sections
    if (trimmedLine.back() == ':') {
        std::string sectionName = trimmedLine.substr(0, trimmedLine.size() - 1);
        if (newIndent == 0) {
            currentSection = sectionName;
        } else if (newIndent > currentIndent) {
            currentSection += "." + sectionName;
        } else {
            currentSection = sectionName; // Remonte dans la hiérarchie
        }
        currentIndent = newIndent;
        return;
    }

    // Extraction clé-valeur
    size_t colonPos = trimmedLine.find(':');
    if (colonPos != std::string::npos) {
        std::string key = trimmedLine.substr(0, colonPos);
        std::string value = trimmedLine.substr(colonPos + 1);
        
        // Nettoyage
        key = trim(key);
        value = trim(value);
        
        // Suppression des guillemets
        if (!value.empty() && (value[0] == '\'' || value[0] == '"')) {
            value = value.substr(1, value.size() - 2);
        }

        if (!currentSection.empty()) {
            configData[currentSection][key] = value;
        }
    }
}

// std::vector<std::string> YamlConfigParser::split(const std::string& s, char delimiter) {
//     std::vector<std::string> tokens;
//     std::string token;
//     std::istringstream tokenStream(s);
//     while (std::getline(tokenStream, token, delimiter)) {
//         tokens.push_back(token);
//     }
//     return tokens;
// }

// std::string YamlConfigParser::getValue(const std::string& section, const std::string& key, 
//     const std::string& defaultValue) const {
// // Recherche directe
//     auto sectionIt = configData.find(section);
//     if (sectionIt != configData.end()) {
//         auto keyIt = sectionIt->second.find(key);
//     if (keyIt != sectionIt->second.end()) {
//         return keyIt->second;
//         }
//     }

// // Recherche dans les sous-sections
//     size_t dotPos = section.find_last_of('.');
//     if (dotPos != std::string::npos) {
//         std::string mainSection = section.substr(0, dotPos);
//         std::string subKey = section.substr(dotPos + 1) + "." + key;

//         sectionIt = configData.find(mainSection);
//     if (sectionIt != configData.end()) {
//         auto keyIt = sectionIt->second.find(subKey);
//     if (keyIt != sectionIt->second.end()) {
//             return keyIt->second;
//             }
//         }
//     }

//     return defaultValue;
// }

std::string YamlConfigParser::getValue(const std::string& section, 
                                     const std::string& key,
                                     const std::string& defaultValue) const {
    // Essai direct
    if (configData.count(section)) {
        const auto& sectionMap = configData.at(section);
        if (sectionMap.count(key)) {
            return sectionMap.at(key);
        }
    }
    
    // Essai avec sous-section
    std::string compositeKey = section + "." + key;
    for (const auto& sectionPair : configData) {
        const auto& sectionMap = sectionPair.second;
        auto it = sectionMap.find(compositeKey);
        if (it != sectionMap.end()) {
            return it->second;
        }
    }
    
    return defaultValue; // Retourne la valeur par défaut si rien trouvé
}

int YamlConfigParser::getIntValue(const std::string& section, 
                                const std::string& key,
                                int defaultValue) const {
    std::string value = getValue(section, key, ""); // Utilise la version à 3 args
    if (!value.empty()) {
        try {
            return std::stoi(value);
        } catch (...) {
            return defaultValue;
        }
    }
    return defaultValue;
}

void YamlConfigParser::printConfig() const {
    for (const auto& section : configData) {
        std::cout << "[" << section.first << "]\n";
        for (const auto& keyValue : section.second) {
            std::cout << keyValue.first << ": " << keyValue.second << "\n";
        }
        std::cout << "\n";
    }
}