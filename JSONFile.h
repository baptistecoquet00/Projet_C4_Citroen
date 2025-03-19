#ifndef JSONFILE_H
#define JSONFILE_H
#include <sstream>
#include <iostream>
#include <string>
#include <fstream>


class JSONFile
{
private:
    std::string leJSON;
    std::ofstream busCAN;
public:
    JSONFile(std::string nomUtilisateur,std::string nomFichier);
    void creerJSON(std::string nomFichier);
    void DebuterJSON(std::string nomUtilisateur);
    void AjouterDonneesJSON(std::string idCAN, std::string lenData, std::string Data);
    void AjouterDonneesJSON(unsigned int idCAN, int lenData, unsigned char * Data);
    void CloreJSON();
    std::string getJSON();
};
#endif
