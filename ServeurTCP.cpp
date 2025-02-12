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
    m_adresseIPServeur = adresseIPServeur;
    m_portServeur = portServeur;
    DemarrerServeur(m_adresseIPServeur,m_portServeur);
}

bool ServeurTCP::DemarrerServeur(string adresseIPServeur,unsigned short portServeur){
    m_adresseIPServeur = adresseIPServeur;
    m_portServeur = portServeur;
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

int ServeurTCP::Recevoir(char message[], int longueurMaxDuMessage,int timeout_us)
{
	fd_set fds;
	int n;
    struct timeval tv;
	FD_ZERO(&fds);
	FD_SET(m_SocketCommunication, &fds);
	tv.tv_sec =  timeout_us/1000000;
	tv.tv_usec = timeout_us%1000000;

	n = select(m_SocketCommunication, &fds, NULL, NULL, &tv);
    if(n==0)
    {
        //std::cout << "Timeout.." << std::endl;
        return 0;
    }
    else if(n==-1)
    {
        //std::cout << "Error.." << std::endl;
        return -1;
	}
	int nbOctets = recv(m_SocketCommunication, message, longueurMaxDuMessage, 0);
	//string str(message, nbOctets);
	//cout << "Message du client (" << nbOctets << " octets) : " << str << endl;
	return nbOctets;
}

void ServeurTCP::FermerCommunication(){
    close(m_SocketCommunication);
    //cout<<"Arret de la communication avec le client : OK"<<endl;
}

void ServeurTCP::EteindreServeur(){
    close(m_SocketEcoute);
    cout<<"Arret du Serveur : OK"<<endl;
}