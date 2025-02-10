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
    /* 
        @brief Méthode privée permettant de Demarrer un Serveur TCP.
        @param adresseIPServeur l'adresse Ip du Serveur à joindre.
        @param le numéro du port du serveur à joindre.
        @return cette méthode renvoie true si l'association du port et la mise en écoute est OK.
    */
    void InitialisationSocket();
    /*
        @brief Méthode privée permettant d'initialiser la socket de communication.
    */
public:
    
    ServeurTCP(std::string addresseIPServeur,unsigned short portServeur);
    /*
        @brief Constructeur qui permet de lancer le serveur.
    */
    bool AttendreClient();
    /*
        @brief Méthode permettant d'attendre la réponse d'un client à la fois.
        @return cette méthode renvoie un true si nous recevons bien le client.
    */
    int Envoyer(char message[], int longueurDuMessage);
     /*
        @brief Méthode qui permet d'envoyer un message au client.
        @param message le messsage à envoyer.
        @param longueurDuMessage la taille du message à envoyer.
        @return Cette méthode renvoie le nombre d'octets reçus. En cas d'échec, cette méthode renvoie 0 ou une valeur négative.
    */
    int Recevoir(char reponse[], int taille);
    /*
        @brief Méthode qui permet de recevoir le message du client.
        @param reponse la réponse du client.
        @param taille la taille de la reponse.
        @return Cette méthode renvoie le nombre d'octets reçus. En cas d'échec, cette méthode renvoie 0 ou une valeur négative.
    */
    int Recevoir(char message[], int longueurMaxDuMessage,int timeout_us);
    /*
        @brief Méthode qui permet de recevoir le message du client.
        @param reponse la réponse du client.
        @param longueurMaxDuMessage permet de définir la longueur max du message à recevoir.
        @param timeout_us permet de mettre un time out en micro-seconde. 
        @return Cette méthode renvoie le nombre d'octets reçus. En cas d'échec, cette méthode renvoie 0 ou une valeur négative.
    */  
    void FermerCommunication();
    /*
        @brief Méthode permettant de fermer la communication avec le client.
    */
    void EteindreServeur();
    /*
        @brief Méthode permettant d'eteindre le serveur.
    */

};
#endif
