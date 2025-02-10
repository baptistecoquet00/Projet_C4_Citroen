#ifndef CLIENTTCP_H
#define CLIENTTCP_H
#include <string>


class ClientTCP
{

public:
    ClientTCP();
    bool SeConnecterAUnServeur(std::string adresseIPServeur, unsigned short portServeur);
    bool SeDeconnecter();
    bool Envoyer(const char * requete,int longueur);
    bool Envoyer(std::string requete);
    int Recevoir(char * reponse, int tailleMax);
    int Recevoir(std::string & reponse);
    inline std::string Erreur() const {return m_erreur;};



private :
    static int nbClientTCP;
    int m_maSocket;
    std::string m_adresseIPServeur;
    unsigned short m_portServeur;
    std::string m_erreur;

};

#endif 
