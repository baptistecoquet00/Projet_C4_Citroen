#ifndef CONFIG_YAML_H
#define CONFIG_YAML_H

#include <sstream>
#include <iostream>
#include <string>
#include <fstream>
#include <map>

typedef unsigned int u32;

class Config_yaml
{
private:
    /* data */
    std::string nomFichierYAML;
    std::ifstream YAMLFichier;
    std::string SectionCourante;
    std::map<std::string, std::map<std::string, std::string>> configurationDonnee;
    void AnalyserLigne(std::string ligne);
public:
    Config_yaml(/*const std::string& nomFichierYAML*/);
    //bool OuvrirFichierConfigYaml();
    bool LireFichierYaml();
    void FermerConfigYaml();
    int getInt(const std::string& section, const std::string& clef);
    std::string getString(const std::string& section, const std::string& clef);
    ~Config_yaml();
};



#endif