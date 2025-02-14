#include "JSONfile.h"

JSONFile::JSONFile(std::string nomUtilisateur){
    DebuterJSON(nomUtilisateur);
}

void JSONFile::DebuterJSON(std::string nomUtilisateur){
    // stringstream stimestamp;
    // int timestamp = (int)time(NULL);
    // stimestamp<<timestamp;

    leJSON = "{ \"nomUtilisateur\" : \"" + nomUtilisateur + "\", \" data \" : [";
}

void AjouterDonneesJSON(std::string message){}

void CloreJSON(){}