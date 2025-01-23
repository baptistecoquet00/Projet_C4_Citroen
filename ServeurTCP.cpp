#include "ServeurTCP.h"

#ifdef __unix__
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h> 
#endif

#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <sstream>


using namespace std;


ServeurTCP::ServeurTCP(string adresseIPServeur,unsigned short portServeur)
{
    string m_adresseIPServeur = adresseIPServeur;
    unsigned short m_portServeur = portServeur;
    DemarrerServeur(m_adresseIPServeur,m_portServeur);
}

bool ServeurTCP::DemarrerServeur(string adresseIPServeur,unsigned short portServeur){
    string m_adresseIPServeur = adresseIPServeur;
    unsigned short m_portServeur = portServeur;
    InitialisationSocket();

    struct sockaddr_in clientARecevoir;
    clientARecevoir.sin_family = AF_INET;
    clientARecevoir.sin_addr.s_addr = inet_addr(m_adresseIPServeur.c_str());
    clientARecevoir.sin_port = htons(m_portServeur);
    
    if(bind(m_SocketEcoute, (const struct sockaddr *) &clientARecevoir,sizeof(clientARecevoir))<0)
    {
        cout<<"Association a un port : ECHEC."<<endl;
        return false;
    }
    cout<<"Association a un port : OK."<<endl;

    int ServeurEcoute = listen(m_SocketEcoute, sizeof(clientARecevoir));
    if(ServeurEcoute == -1){
        cout << "Mise en ecoute : ECHEC." << endl;
        return false;
    }
    cout << "Mise en ecoute : OK." << endl;
    return true;
        // je recois puis je dois send une réponse et  je dois close la connexion avec le client
}

void ServeurTCP::InitialisationSocket(){
    m_SocketEcoute = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(m_SocketEcoute == -1){
        cout << "Creation de la socket d'ecoute : ECHEC." << endl;
    }
    cout << "Creation de la socket d'ecoute : OK." << endl;
}

bool ServeurTCP::AttendreClient(){
    if(m_SocketEcoute == -1)
	{
		return false;
	}
	struct sockaddr_in addrClient;
	socklen_t addrClientLen = sizeof(addrClient);
	m_SocketCommunication = accept(m_SocketEcoute,(struct sockaddr*)&addrClient, &addrClientLen);
	if(m_SocketCommunication <= 0)
	{
		cout << "Client accepte : ECHEC." << endl;
		return false;
	}

	cout << "Client accepte : OK." << endl;
	return true;
}

int ServeurTCP::Envoyer(char message[],int longueurDuMessage){
    int nbOctet = send(m_SocketCommunication,message,longueurDuMessage,0);
    return nbOctet;
}

int ServeurTCP::Recevoir(char reponse[], int taille){
    int OctetLus = recv(m_SocketCommunication,reponse,taille,0);
    return OctetLus;
}

void ServeurTCP::FermerCommunication(){
    close(m_SocketCommunication);
    //cout<<"Arret de la communication avec le client : OK"<<endl;
}

void ServeurTCP::EteindreServeur(){
    close(m_SocketEcoute);
    cout<<"Arret du Serveur : OK"<<endl;
}