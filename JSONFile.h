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
    JSONFile();
    std::string creerJSON();
    void DebuterJSON();
    void AjouterDonneesJSON(std::string message,int idCAN,int lenData,int Data);
    void CloreJSON();
};
#endif
