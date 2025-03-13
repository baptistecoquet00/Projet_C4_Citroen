#include "JSONFile.h"

JSONFile::JSONFile(std::string nomUtilisateur,std::string nomFichier){
    creerJSON(nomFichier);
    DebuterJSON(nomUtilisateur);
}

void JSONFile::creerJSON(std::string nomFichier){
    try{
        std::cout << "dd";
        busCAN.open(nomFichier);
        //busCAN<<"test"; busCAN.close();
        
    }catch(std::runtime_error& e){
     std::cout <<"erreur : impossible d'ouvrir le fichier" << e.what() << std::endl;

    }
}

void JSONFile::DebuterJSON(std::string nomUtilisateur){
    leJSON = "{\"users\" : \"" + nomUtilisateur + "\", \"data\" : [";
}

// void JSONFile::AjouterDonneesJSON(std::string message,int idCAN,int lenData,int Data){
//     message.insert(0,"{");
//     int position = 1;
//     std::string lenDataStr = std::to_string(lenData);
//     std::string idCANStr = std::to_string(idCAN);
//     std::string positionStr = std::to_string(position);
//     std::string DataStr = std::to_string(Data);

//     do{
        
//         std::string strToInsert = "\"idCAN\":"; 

//         std::size_t lenToInsert = strToInsert.length();

//         if(position>=0){
//             //position = message.insert(position+1,"\"idCAN\":");
//             message.insert(position + 1, strToInsert);
//             position += lenToInsert + 1;
//             message.insert(position+2,idCANStr);
//             message.insert(position+1,",");
//             message.insert(position+1,"\"lenData\":");
//             message.insert(position+1,lenDataStr);
//             message.insert(position+1,",");
//             message.insert(position+1,"\"Data\":");
//             message.insert(position+1,DataStr);

//         }
//         position++;
//         if(positionStr==""){
//             position = -1;
//         }
//     }while(position !=-1);
//     message +="},";
//    	leJSON += message;
// }

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

std::string JSONFile::getJSON(){
    return leJSON;
}