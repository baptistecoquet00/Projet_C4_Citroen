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
    message.insert(0,"{");
    int position = 1;
    std::string lenDataStr = std::to_string(lenData);
    std::string idCANStr = std::to_string(idCAN);
    std::string positionStr = std::to_string(position);
    std::string DataStr = std::to_string(Data);

    do{
        // message+="idCAN\":";
        // message += idCAN;
        // message +="\"lenData\":";
        // message +=lenData;
        // message += "\"Data\":";
        // message += Data;
        std::string strToInsert = "\"idCAN\":"; 

        std::size_t lenToInsert = strToInsert.length();

        if(position>=0){
            //position = message.insert(position+1,"\"idCAN\":");
            message.insert(position + 1, strToInsert);
            position += lenToInsert + 1;
            message.insert(position+2,idCANStr);
            message.insert(position+1,",");
            message.insert(position+1,"\"lenData\":");
            message.insert(position+1,lenDataStr);
            message.insert(position+1,",");
            message.insert(position+1,"\"Data\":");
            message.insert(position+1,DataStr);

        }
        position++;
        if(positionStr==""){
            position = -1;
        }
    }while(position !=-1);
    message +="},";
   	leJSON += message;
}

void JSONFile::CloreJSON(){
    leJSON.erase(leJSON.length()-1,1);
    leJSON += "]}";
    busCAN<<leJSON<<std::endl;
    busCAN.close();
}