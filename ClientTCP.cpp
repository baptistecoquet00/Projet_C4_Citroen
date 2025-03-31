#include "ClientTCP.h"

#ifdef _WIN32 || _WIN64
#include <winsock2.h>
#include <sys/types.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>  // close()
#endif
#include <stdio.h>
#include <string.h>

using namespace std;



int ClientTCP::nbClientTCP = 0;



ClientTCP::ClientTCP()
{
    if(nbClientTCP == 0)
    {
        //cout << "Bonjour" << endl;
#ifdef _WIN32 || _WIN64
        WSADATA wsaData ;
        int iResult;

        //cout << "Bonjour 2 " << endl;

        iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
        if(iResult != 0)
        {
            cout << "Initialisation de la DLL Winsock : ERROR." << endl;
        }
        //cout << "Initialisation de la DLL Winsock : OK." << endl;
#endif

    }

    nbClientTCP++;
}


bool ClientTCP::SeConnecterAUnServeur(string adresseIPServeur, unsigned short portServeur)
{
    // Création de la socket TCP
    m_maSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(m_maSocket == -1)
    {
        cout << "Creation de la socket : :ERREUR." << endl;
        return false;
    }

    m_adresseIPServeur = adresseIPServeur;
    m_portServeur = portServeur;

    // Connexion au serveur
    struct sockaddr_in serveurAJoindre;
    serveurAJoindre.sin_family = AF_INET;
    serveurAJoindre.sin_addr.s_addr = inet_addr(m_adresseIPServeur.c_str());
    serveurAJoindre.sin_port = htons(m_portServeur);
    int resultat = connect(m_maSocket, (const struct sockaddr *) &serveurAJoindre, sizeof(serveurAJoindre));
    if(resultat != 0)
    {
        cout << "Connexion au serveur : ERREUR." << endl;
        return false;
    }

    return true;
}

bool ClientTCP::SeDeconnecter()
{
#ifdef _WIN32 || _WIN64
    closesocket(m_maSocket);
#else
    close(m_maSocket);
#endif
    return true;
}

bool ClientTCP::Envoyer(const char * requete,int longueur)
{
	int resultat = send(m_maSocket, requete, longueur, 0);
    if(resultat == -1)
    {
        cout << "Envoi du message : ERREUR." << endl;
        return false;
    }
    return true;
}
bool ClientTCP::Envoyer(string requete)
{
	return Envoyer(requete.c_str(),requete.length());
}
int ClientTCP::Recevoir(char * reponse, int tailleMax, int timeout_us) {
    if (timeout_us)
    {
        fd_set fds;
        int n;
        struct timeval tv;
        FD_ZERO(&fds);
        FD_SET(m_maSocket, &fds);
        tv.tv_sec = timeout_us / 1000000;
        tv.tv_usec = timeout_us % 1000000;

        n = select(m_maSocket, &fds, NULL, NULL, &tv);
        if (n == 0) {
            //std::cout << "Timeout.." << std::endl;
            return -2;
        } else if (n == -1) {
            //std::cout << "Error.." << std::endl;
            return -1;
        }
    }
	int nbOctetsLus = recv(m_maSocket, reponse, tailleMax, 0);
	reponse[nbOctetsLus]=0;
	return nbOctetsLus;
}
int ClientTCP::Recevoir(string & reponse)
{   char creponse[1500];
	bool retour=Recevoir(creponse, 1500,0);
	reponse=creponse;
	return retour;
}
