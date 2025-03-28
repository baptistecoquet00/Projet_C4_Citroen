#include "Config_yaml.h"

void Config_yaml::AnalyserLigne(std::string ligne)
{
    ligne.erase(0,ligne.find_first_not_of(" \t"));
    ligne.erase(ligne.find_last_not_of(" \t")+1);

    if(ligne.back() == ':')
    {
        SectionCourante = ligne.substr(0,ligne.size()-1);
        SectionCourante.erase(0, SectionCourante.find_first_not_of(" \t"));
        SectionCourante.erase(SectionCourante.find_last_not_of(" \t")+1);
    }
    u32 pointPosition = ligne.find(':');
    std::string clef = ligne.substr(0,pointPosition);
    clef.erase(0,clef.find_first_not_of(" \t"));
    clef.erase(clef.find_last_not_of(" \t")+1);

    std::string valeur = ligne.substr(pointPosition+1);
    valeur.erase(0,valeur.find_first_not_of(" \t"));
    valeur.erase(valeur.find_last_not_of(" \t")+1);

    if(valeur.size()>=2 && valeur.front()=='\'' && valeur.back()  =='\'')
    {
        valeur = valeur.substr(1,valeur.size()-2);
    }

    if(!SectionCourante.empty() && !clef.empty())
    {
        configurationDonnee[SectionCourante][clef] = valeur;
    }
}

Config_yaml::Config_yaml(/*const std::string& nomFichierYAML*/){
   
}


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

void Config_yaml::FermerConfigYaml()
{
    if (YAMLFichier.is_open()) {
        YAMLFichier.close();
    }
}

std::string Config_yaml::getString(const std::string& section, const std::string& clef)
{
    auto sectionIt = configurationDonnee.find(section);
    if(sectionIt == configurationDonnee.end())
    {
        std::cout<<"Section pas trouvée: "<<section<<"\n";
    }

    auto clefIt = sectionIt->second.find(clef);
    if(clefIt == sectionIt->second.end())
    {
        std::cout<<"Clef non-trouvee dans section :"<<section<<":"<<clef<<"\n";
    }
    return clefIt->second;
}

int Config_yaml::getInt(const std::string& section, const std::string& clef)
{
    std::string valeur = getString(section, clef);
    try {
        return std::stoi(valeur);
    }catch (...) {
        std::cout<<"Valeur d'entier non-valide pour la clef"<<clef<<" dans la section "<<section<<"\n";
    }
}

Config_yaml::~Config_yaml()
{
    FermerConfigYaml();
}
