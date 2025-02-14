#ifndef JSONFILE_H
#define JSONFILE_H
#include <sstream>
#include <iostream>
#include <string>


class JSONFile
{
private:
    std::string leJSON;
public:
    JSONFile(std::string nomUtilisateur);
    std::string creerJSON(std::string nomUtilisateur);
    void DebuterJSON(std::string nomUtilisateur);
    void AjouterDonneesJSON(string message);
    void CloreJSON();
    ~JSONFile();
};

