#include "YamlConfigParser.h"
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

string YamlConfigParser::trim(const string& str) {
    const auto first = str.find_first_not_of(" \t");
    if (string::npos == first) {
        return str;
    }
    const auto last = str.find_last_not_of(" \t");
    return str.substr(first, (last - first + 1));
}

pair<string, string> YamlConfigParser::splitKeyValue(const string& line) {
    const size_t colonPos = line.find(':');
    if (colonPos == string::npos) {
        return {"", ""};
    }
    
    string key = trim(line.substr(0, colonPos));
    string value = trim(line.substr(colonPos + 1));
    
    if (!value.empty() && value.front() == '\'' && value.back() == '\'') {
        value = value.substr(1, value.size() - 2);
    }
    
    return {key, value};
}

YamlConfigParser::YamlConfigParser(const string& filename) {
    loadFromFile(filename);
}

bool YamlConfigParser::loadFromFile(const string& filename) {
    ifstream file(filename);
    configData.clear();

    if (!file.is_open()) {
        cerr << "Erreur: Impossible d'ouvrir le fichier " << filename << endl;
        return false;
    }

    string line;
    string currentSection;

    while (getline(file, line)) {
        line = trim(line);
        
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        if (line.back() == ':') {
            currentSection = line.substr(0, line.size() - 1);
            currentSection = trim(currentSection);
            configData[currentSection] = {};
        } 
        else {
            auto kvPair = splitKeyValue(line);
            const string& key = kvPair.first;
            const string& value = kvPair.second;
            if (!key.empty() && !currentSection.empty()) {
                configData[currentSection][key] = value;
            }
        }
    }

    file.close();
    return true;
}

const map<string, map<string, string>>& YamlConfigParser::getData() const {
    return configData;
}



string YamlConfigParser::getValue(const string& section, const string& key, 
    const string& defaultValue) const {
    try {
        // Pour une structure à 2 niveaux : section -> key
        const auto& node = configData[section][key];
        if(node) return node.as<string>();

        // Pour une structure imbriquée : section -> subsection -> key
        size_t dot_pos = key.find('.');
        if(dot_pos != string::npos) {
            string subsection = key.substr(0, dot_pos);
            string subkey = key.substr(dot_pos+1);
            const auto& nested_node = configData[section][subsection][subkey];
        if(nested_node) return nested_node.as<string>();
        }
    } catch (...) {}
    return defaultValue;
}

int YamlConfigParser::getIntValue(const string& section, const string& key, int defaultValue) const {
    try {
        return configData[section][key].as<int>();
    } catch (...) {
    try {
            // Gestion des clés imbriquées
        size_t dot_pos = key.find('.');
        if(dot_pos != string::npos) {
            string subsection = key.substr(0, dot_pos);
            string subkey = key.substr(dot_pos+1);
            return configData[section][subsection][subkey].as<int>();
        }
    } catch (...) {}
    return defaultValue;
    }
}

void YamlConfigParser::printConfig() const {
    for (const auto& sectionEntry : configData) {
        cout << "[" << sectionEntry.first << "]\n";
        for (const auto& keyValueEntry : sectionEntry.second) {
            cout << "  " << keyValueEntry.first << ": " << keyValueEntry.second << "\n";
        }
    }
}


// string YamlConfigParser::getValue(const string& section, const string& key, 
//                                 const string& defaultValue) const {
//     if (configData.count(section) && configData.at(section).count(key)) {
//         return configData.at(section).at(key);
//     }
//     return defaultValue;
// }

// int YamlConfigParser::getIntValue(const string& section, const string& key, int defaultValue) const {
//     try {
//         return stoi(getValue(section, key, to_string(defaultValue)));
//     } catch (...) {
//         return defaultValue;
//     }
// }