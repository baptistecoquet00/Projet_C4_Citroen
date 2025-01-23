#ifndef SERVEURTCP_H
#define SERVEURTCP_H
#include <string>



class ServeurTCP
{
private:
    unsigned short portServeur;
    std::string adresseIPServeur;
    int m_SocketEcoute;
    int m_SocketCommunication;
    bool DemarrerServeur(std::string addresseIPServeur,unsigned short portServeur);
    void InitialisationSocket();
public:
    ServeurTCP(std::string addresseIPServeur,unsigned short portServeur);
    bool AttendreClient();
    int Envoyer(char message[], int longueurDuMessage);
    int Recevoir(char reponse[], int taille);
    void FermerCommunication();
    void EteindreServeur();

};
#endif
