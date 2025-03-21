#ifndef DONNEESCANCLASS_H
#define DONNEESCANCLASS_H
#include <string>
#include "JSONFile.h"
#include "ClientTCP.h"
#include "MessageCAN.h"
#include "SNIRconversions.h"

class DonneesCANClass
{
private:
    std::string nomUtilisateur_pv;
    std::string nomFichierJSON_pv;
    std::string nomFichierLog_pv;
    ClientTCP client;
    std::string addrServeur_pv;
    int portServeur_pv;
    bool ConnexionServeurRest(std::string addrServeur,int portServeur);
public:
    std::string tabDonneesFormatee[2048];
    DonneeCAN tabDonnees[2048];
    DonneesCANClass(std::string nomUtilisateur,std::string nomFichierJSON,std::string addrServeur,int portServeur);
    const std::string currentDateTime();
    void SauvegarderDonneesCAN(std::string nomFichierLog);
    void ChargerDonneesCAN(std::string nomFichierLog);
    void affichageDonneeCAN();
    void EnvoyerDonneesAuServeurREST();
};


#endif