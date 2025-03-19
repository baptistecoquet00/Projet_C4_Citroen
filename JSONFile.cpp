#include "JSONFile.h"

JSONFile::JSONFile(std::string nomUtilisateur,std::string nomFichier){
    creerJSON(nomFichier);
    DebuterJSON(nomUtilisateur);
}

void JSONFile::creerJSON(std::string nomFichier){
    try{
        busCAN.open(nomFichier);
        //busCAN<<"test"; busCAN.close();
        
    }catch(std::runtime_error& e){
     std::cout <<"erreur : impossible d'ouvrir le fichier" << e.what() << std::endl;

    }
}

void JSONFile::DebuterJSON(std::string nomUtilisateur){
    leJSON = "{\"users\" : \"" + nomUtilisateur + "\", \"data\" : [";
}

void JSONFile::AjouterDonneesJSON(std::string idCAN, std::string lenData, std::string Data) {
    std::string message = "{";
   

    message += "\"idCAN\":" + idCAN + ",";
    message += "\"lenData\":" + lenData + ",";
    message += "\"Data\":" + Data;
    message += "}";

    if (leJSON.back() != '[') {
        leJSON += ",";
    }
    leJSON += message;
}

void JSONFile::AjouterDonneesJSON(unsigned int idCAN, int lenData, unsigned char * Data) {

    std::stringstream ss;
    ss << "{\"idCAN\": \"0x" << std::hex << idCAN << "\",";
    ss << "\"lenData\":" << std::dec << lenData << ",";
    ss << "\"Data\": \"0x";
    for(int i=0; i<lenData;i++)
    {
        ss << std::hex << (unsigned int)Data[i] ;
    }
    ss << "\"}";
    if (leJSON.back() != '[') {
        leJSON += ",";
    }
    leJSON += ss.str();

}

void JSONFile::CloreJSON(){
    leJSON.erase(leJSON.length()-1,1);
    leJSON += "}]}";
    busCAN<<leJSON;
    busCAN.close();
}

std::string JSONFile::getJSON(){
    return leJSON;
}