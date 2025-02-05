#include "IRServeurTcpMonoclient.h"
//#include <arpa/inet.h>
//#include <unistd.h>
#include <iostream>
#include <winsock2.h>
#include <iostream>
#include <string>
#include <conio.h>
#include <windows.h>

using namespace std;

IRServeurTcpMonoClient::IRServeurTcpMonoClient()
{
}

IRServeurTcpMonoClient::~IRServeurTcpMonoClient()
{
	FermerLaCommunicationAvecLeClient();
	ArreterLeServeur();
}

void IRServeurTcpMonoClient::Initialisation()
{
	#ifdef _WIN32 || _WIN64
	WSADATA wsaData ;
	int iResult;
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if(iResult != 0)
	{
		cout << "Echec lors de l'initialisation de la DLL Winsock" << endl;
	}
	#endif
	socketEcoute = socket ( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if ( socketEcoute == -1 )
	{
		cout << "Creation de la socket d'ecoute : ECHEC." << endl;
		return ;
	}
	cout << "Creation de la socket d'ecoute : OK." << endl;

}
bool IRServeurTcpMonoClient::MettreEnEcouteSurLePort(unsigned short portEcoute)
{
	if(socketEcoute == -1)
	{
		return false;
	}
	portEcoute = portEcoute;

	struct sockaddr_in addrMonServeur;
	addrMonServeur.sin_family = AF_INET;
	// Ecoute sur le port 4222
	addrMonServeur.sin_port = htons(portEcoute);
	// Ecoute sur toutes les adresses IP de ma machine
	addrMonServeur.sin_addr.s_addr = INADDR_ANY;
	if ( bind ( socketEcoute, ( struct sockaddr * ) &addrMonServeur, sizeof ( addrMonServeur )  ) < 0 )
	{
		cout << "Association a un port : ECHEC." << endl;
		return false;
	}

	cout << "Association a un port : OK." << endl;

	// Mise en ecoute du serveur
	if (listen(socketEcoute,1) == -1) {
		cout << "Mise en ecoute : ECHEC." << endl;
		return false;
	}
	cout << "Mise en ecoute : OK." << endl;
	return true;
}

bool IRServeurTcpMonoClient::AttendreUnNouveauClient()
{
	if(socketEcoute == -1)
	{
		return false;
	}
	struct sockaddr_in addrClient;
	int addrClientLen = sizeof(addrClient);
	socketDeCommunication = accept(socketEcoute,(struct sockaddr*)&addrClient, (int *) &addrClientLen);
	if(socketDeCommunication <= 0)
	{
		cout << "Client accepte : ECHEC." << endl;
		return false;
	}

	cout << "Client accepte : OK." << endl;
	return true;
}

int IRServeurTcpMonoClient::RecevoirUnMessage(char message[], int longueurMaxDuMessage)
{
	int nbOctets = recv(socketDeCommunication, message, longueurMaxDuMessage, 0);
	//string str(message, nbOctets);
	//cout << "Message du client (" << nbOctets << " octets) : " << str << endl;
	return nbOctets;
}
int IRServeurTcpMonoClient::RecevoirUnMessage(char message[], int longueurMaxDuMessage,int timeout_us)
{
	fd_set fds;
	int n;
    struct timeval tv;
	FD_ZERO(&fds);
	FD_SET(socketDeCommunication, &fds);
	tv.tv_sec =  timeout_us/1000000;
	tv.tv_usec = timeout_us%1000000;

	n = select(socketDeCommunication, &fds, NULL, NULL, &tv);
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
	int nbOctets = recv(socketDeCommunication, message, longueurMaxDuMessage, 0);
	//string str(message, nbOctets);
	//cout << "Message du client (" << nbOctets << " octets) : " << str << endl;
	return nbOctets;
}
int IRServeurTcpMonoClient::EnvoyerUnMessage(char message[], int longueurDuMessage)
{
	int nbOctets = send(socketDeCommunication, message, longueurDuMessage, 0);
	return nbOctets;

}

void IRServeurTcpMonoClient::FermerLaCommunicationAvecLeClient()
{
	// Fermeture de la connexion avec le client
	closesocket(socketDeCommunication);
	cout << "Fermeture de la communication avec le client : OK." << endl;
}

void IRServeurTcpMonoClient::ArreterLeServeur()
{
	// Fermeture de la socket d'ecoute
	closesocket(socketEcoute);
	cout << "Arret du serveur : OK." << endl;

}

