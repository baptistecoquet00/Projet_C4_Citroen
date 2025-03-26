#ifndef SERVEURTCP_H
#define SERVEURTCP_H
#include <string>



class ServeurTCP
{
private:

    unsigned short m_portServeur;
    //! Port Serveur
    /*! Attribut permettant de mettre le numéro du port du Serveur sur 16 Bits*/
    std::string m_adresseIPServeur;
    //! Adresse IP Serveur
    /*!Chaine de caractère permettant d'écrire l'adresse ip sur laquelle écoute le serveur */
    int m_SocketEcoute;
    //! Socket Ecoute
    /*!entier permettant de savoir sur qu'elle socket écoutera le serveur*/
    int m_SocketCommunication;
    //! Socket Communication
    /*!entier permettant  de savoir sur qu'elle socket de communication le serveur communiquera*/

    bool DemarrerServeur(std::string addresseIPServeur,unsigned short portServeur);
    /*! 
        @brief Méthode privée permettant de Demarrer un Serveur TCP.
        @param adresseIPServeur l'adresse Ip du Serveur à joindre.
        @param portServeur le numéro du port du serveur à joindre.
        @return cette méthode renvoie true si l'association du port et la mise en écoute est OK.
    */
    void InitialisationSocket();
    /*!
        @brief Méthode privée permettant d'initialiser la socket de communication.
    */

public:
    
    ServeurTCP(std::string addresseIPServeur,unsigned short portServeur);
    /*!
        @brief Constructeur qui permet de lancer le serveur.
    */
    std::string getAddrIPServeur();
    /*!
        @brief Méthode permettant d'accéder à la valeur de l'adresse du Serveur
        @return renvoie l'adresse Ip du Serveur
    */
    unsigned short getPortServeur();
    /*!
        @brief Méthode permettant d'accéder à la valeur du Port du Serveur
        @return renvoie le port du Serveur
    */
    bool AttendreClient();
    /*!
        @brief Méthode permettant d'attendre la réponse d'un client à la fois.
        @return cette méthode renvoie un true si nous recevons bien le client.
    */
    int Envoyer(char message[], int longueurDuMessage);
     /*!
        @brief Méthode qui permet d'envoyer un message au client.
        @param message le messsage à envoyer.
        @param longueurDuMessage la taille du message à envoyer.
        @return Cette méthode renvoie le nombre d'octets reçus. En cas d'échec, cette méthode renvoie 0 ou une valeur négative.
    */
    int Recevoir(char reponse[], int taille);
    /*!
        @brief Méthode qui permet de recevoir le message du client.
        @param reponse la réponse du client.
        @param taille la taille de la reponse.
        @return Cette méthode renvoie le nombre d'octets reçus. En cas d'échec, cette méthode renvoie 0 ou une valeur négative.
    */
    int Recevoir(char message[], int longueurMaxDuMessage,int timeout_us);
    /*!
        @brief Méthode qui permet de recevoir le message du client.
        @param message la réponse du client.
        @param longueurMaxDuMessage permet de définir la longueur max du message à recevoir.
        @param timeout_us permet de mettre un time out en micro-seconde. 
        @return Cette méthode renvoie le nombre d'octets reçus. En cas d'échec, cette méthode renvoie 0 ou une valeur négative.
    */  
    bool ClientEstConnecte();
    /*!
        @brief Méthode permettant si le client est toujours connecté au Serveur.
        @return retourne True si le client est toujours connecté et sinon False dans le cas contraire et ferme la communication.
    */
    void FermerCommunication();
    /*!
        @brief Méthode permettant de fermer la communication avec le client.
    */
    void EteindreServeur();
    /*!
        @brief Méthode permettant d'eteindre le serveur.
    */

};
#endif
