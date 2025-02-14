#include "JSONFile.h"

JSONFile::JSONFile(std::string nomUtilisateur,std::string nomFichier){
    creerJSON(nomFichier);
    DebuterJSON(nomUtilisateur);
}

std::string JSONFile::creerJSON(std::string nomFichier){
    busCAN.open(nomFichier);
}

void JSONFile::DebuterJSON(std::string nomUtilisateur){
    // stringstream stimestamp;
    // int timestamp = (int)time(NULL);
    // stimestamp<<timestamp;

    leJSON = "{ \"nomUtilisateur\" : \"" + nomUtilisateur + "\", \" data \" : [";
}

void JSONFile::AjouterDonneesJSON(std::string message,int idCAN,int lenData,int Data){
    message.insert(0,"{\"");
    int position = 1;

    do{
        message+="idCAN\":";
        message += idCAN;
        message +="\"lenData\":";
        message +=lenData;
        message += "\"Data\":";
        message += Data;
        // if(position>=0){
        //     position = message.insert(position+1,"{\"idCAN\":");
        //     message.insert(position+2,idCAN);
        //     message.insert

        // }
        position++;
        if(position==""){
            position = -1;
        }
    }while(position !=-1);
    message +="},";
   	leJSON += message;
}

void JSONFile::CloreJSON(){
    leJSON.erase(leJSON.length()-1,1);
    leJSON += "}]}";
    busCAN<<leJSON<<std::endl;
    busCAN.close();
}