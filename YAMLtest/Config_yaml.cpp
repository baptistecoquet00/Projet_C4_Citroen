#include "Config_yaml.h"

void Config_yaml::AnalyserLigne(std::string ligne) {
    // Supprimer les commentaires
    size_t commentPos = ligne.find('#');
    if (commentPos != std::string::npos) {
        ligne = ligne.substr(0, commentPos);
    }

    // Ignorer les lignes vides ou sans ':' (après suppression des commentaires)
    ligne.erase(0, ligne.find_first_not_of(" \t")); // Trim gauche
    ligne.erase(ligne.find_last_not_of(" \t") + 1); // Trim droit
    if (ligne.empty() || ligne.find(':') == std::string::npos) {
        return;
    }

    // Gestion des sections (ex: "config_server:")
    if (ligne.back() == ':') {
        SectionCourante = ligne.substr(0, ligne.size() - 1);
        SectionCourante.erase(0, SectionCourante.find_first_not_of(" \t")); // Trim gauche
        SectionCourante.erase(SectionCourante.find_last_not_of(" \t") + 1); // Trim droit
        return; // Une section n'a pas de valeur associée
    }

    // Extraction clé/valeur
    size_t colonPos = ligne.find(':');
    std::string clef = ligne.substr(0, colonPos);
    clef.erase(0, clef.find_first_not_of(" \t")); // Trim gauche
    clef.erase(clef.find_last_not_of(" \t") + 1); // Trim droit

    std::string valeur = ligne.substr(colonPos + 1);
    valeur.erase(0, valeur.find_first_not_of(" \t")); // Trim gauche
    valeur.erase(valeur.find_last_not_of(" \t") + 1); // Trim droit

    // Supprimer les guillemets simples si présents
    if (valeur.size() >= 2 && valeur.front() == '\'' && valeur.back() == '\'') {
        valeur = valeur.substr(1, valeur.size() - 2);
    }

    // Validation finale
    if (!SectionCourante.empty() && !clef.empty()) {
        configurationDonnee[SectionCourante][clef] = valeur;
    }
}

Config_yaml::Config_yaml(/*const std::string& nomFichierYAML*/){}

/*
bool Config_yaml::LireFichierYaml()
{   
    YAMLFichier.open("Config_Serveur.yml");//"Config_Serveur.yml"
    if(!YAMLFichier.is_open())
    {
        return false;
    }
    return true;
    std::string message;
    std::string ligne;
    while (std::getline(YAMLFichier, ligne)) {
        AnalyserLigne(ligne);
    }

    return true;
}
*/
std::vector<std::string> Config_yaml::decomposeChemin(const std::string& chemin) {
    std::vector<std::string> sections;
    std::stringstream ss(chemin);
    std::string item;
    while (std::getline(ss, item, '.')) {
        sections.push_back(item);
    }
    return sections;
}

bool Config_yaml::LireFichierYaml() {
    if (YAMLFichier.is_open()) {
        return true;  // Déjà ouvert
    }

    YAMLFichier.open("Config_Serveur.yml");
    if (!YAMLFichier.is_open()) {
        std::cerr << "Erreur: Impossible d'ouvrir le fichier YAML.\n";
        return false;
    }

    std::string ligne;
    while (std::getline(YAMLFichier, ligne)) {
        AnalyserLigne(ligne);
    }

    return true;
}


void Config_yaml::FermerConfigYaml()
{
    if (YAMLFichier.is_open()) {
        YAMLFichier.close();
    }
}

// std::string Config_yaml::getString(const std::string& section, const std::string& clef)
// {
//     auto sectionIt = configurationDonnee.find(section);
//     if(sectionIt == configurationDonnee.end())
//     {
//         throw std::runtime_error("Section non trouvee: " + section);
//     }

//     auto clefIt = sectionIt->second.find(clef);
//     if(clefIt == sectionIt->second.end())
//     {
//         throw std::runtime_error("Clef non-trouvee dans la section " + section + ": " + clef);
//     }
//     return clefIt->second;
// }

std::string Config_yaml::getString(const std::string& chemin) {
    // 1. Découper le chemin (ex: "section.sous_section.clé" → ["section", "sous_section", "clé"])
    auto sections = decomposeChemin(chemin);
    if (sections.empty()) {
        throw std::runtime_error("Chemin vide");
    }

    // 2. Accéder à la section racine
    auto sectionIt = configurationDonnee.find(sections[0]);
    if (sectionIt == configurationDonnee.end()) {
        throw std::runtime_error("Section non trouvée: " + sections[0]);
    }

    // 3. Naviguer récursivement dans les sous-sections
    const std::map<std::string, std::string>* currentMap = &sectionIt->second;
    std::string valeur;

    for (size_t i = 1; i < sections.size(); ++i) {
        auto entryIt = currentMap->find(sections[i]);
        if (entryIt == currentMap->end()) {
            throw std::runtime_error("Clé/sous-section non trouvée: " + sections[i]);
        }

        if (i == sections.size() - 1) {
            // Dernière partie du chemin → retourne la valeur
            valeur = entryIt->second;
        } else {
            // Sous-section → vérifie si c'est une map imbriquée
            throw std::runtime_error("Structure YAML non supportée (sous-sections imbriquées)");
        }
    }

    return valeur;
}
int Config_yaml::getInt(const std::string& chemin) {
    std::string valeur = getString(chemin);
    try {
        return std::stoi(valeur);
    } catch (...) {
        throw std::runtime_error("Valeur invalide pour l'entier dans la section: " + chemin);
    }
}

Config_yaml::~Config_yaml()
{
    FermerConfigYaml();
}
