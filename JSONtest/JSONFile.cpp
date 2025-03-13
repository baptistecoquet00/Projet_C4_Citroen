#include "JSONFile.h"

JSONFile::JSONFile(std::string nomUtilisateur,std::string nomFichier){
    creerJSON(nomFichier);
    DebuterJSON(nomUtilisateur);
}

void JSONFile::creerJSON(std::string nomFichier){
    try{
        std::cout << "dd";
        busCAN.open(nomFichier,std::ios::app);
        //busCAN<<"test"; busCAN.close();
        
    }catch(std::runtime_error& e){
     std::cout <<"erreur : impossible d'ouvrir le fichier" << e.what() << std::endl;

    }
}

void JSONFile::DebuterJSON(std::string nomUtilisateur){
    leJSON = "{ \"nomUtilisateur\" : \"" + nomUtilisateur + "\", \" data \" : [";
}

void JSONFile::AjouterDonneesJSON(std::string message, int idCAN, int lenData, int Data) {
    message = "{";
    std::string lenDataStr = std::to_string(lenData);
    std::string idCANStr = std::to_string(idCAN);
    std::string DataStr = std::to_string(Data);

    message += "\"idCAN\":" + idCANStr + ",";
    message += "\"lenData\":" + lenDataStr + ",";
    message += "\"Data\":" + DataStr ;
    message+= "}";
    if (leJSON.back() != '[') {
        leJSON += ",";
    }
    leJSON += message;
}



// void JSONFile::AjouterDonneesJSON(std::string message, int idCAN, int lenData, int Data) {
//     message.insert(0, "{");
//     int position = 1;
//     std::string lenDataStr = std::to_string(lenData);
//     std::string idCANStr = std::to_string(idCAN);
//     std::string DataStr = std::to_string(Data);

//     do {
//         message.insert(position, "\"idCAN\":" + idCANStr + ",");
//         position += idCANStr.length() + 8; // 8 est la longueur de la chaîne "\"idCAN\":"

//         message.insert(position, "\"lenData\":" + lenDataStr + ",");
//         position += lenDataStr.length() + 10; // 10 est la longueur de la chaîne "\"lenData\":"

//         message.insert(position, "\"Data\":" + DataStr);
//         position += DataStr.length() + 7; // 7 est la longueur de la chaîne "\"Data\":"

//         // Vérification de fin de fichier JSON
//         if (position >= message.size() - 1) {
//             position = -1;
//         }
//     } while (position != -1);

//     message += "},";
//     std::cout<<"Le fichier JSON : "<<message;
//     leJSON += message;
// }



void JSONFile::CloreJSON(){
    leJSON.erase(leJSON.length()-1,1);
    leJSON += "}]}";
    busCAN<<leJSON;
    busCAN.close();
}