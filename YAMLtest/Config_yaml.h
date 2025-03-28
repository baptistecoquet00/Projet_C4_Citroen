#ifndef CONFIG_YAML_H
#define CONFIG_YAML_H

#include <sstream>
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <stdexcept>
#include <vector>

typedef unsigned int u32;

class Config_yaml
{
private:
    /* data */
    std::ifstream YAMLFichier;
    std::string SectionCourante;
    std::map<std::string, std::map<std::string, std::string>> configurationDonnee;
    void AnalyserLigne(std::string ligne);
public:
    Config_yaml(/*const std::string& nomFichierYAML*/);
    //bool OuvrirFichierConfigYaml();
    std::vector<std::string> decomposeChemin(const std::string& chemin);
    bool LireFichierYaml();
    void FermerConfigYaml();
    int getInt(const std::string& chemin);
    std::string getString(const std::string& chemin);
    ~Config_yaml();
};



#endif