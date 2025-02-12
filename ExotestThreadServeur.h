#ifndef EXOTESTTRHEADSERVEUR_H
#define EXOTESTTRHEADSERVEUR_H
#include <iostream>
#include <stdio.h>
#include <pthread.h>
#include <thread>
#include <sys/time.h>

#include "VSCOM.h"
#include "ServeurTCP.h"

class ExotestThreadServeur
{
private:

    VSCOM vscom;
    ServeurTCP serveur;
    int m_nbTrames[2048];
    std::string m_tabDonneesFormatee[2048];
    DonneeCAN m_tabDonnees[2048];
    bool OK = true;
    std::string m_leCOM;
    pthread_t thread_TCP; // Pas sur que ça reste dans la classe peut-etre laisser dans le main
    pthread_create( & thread_TCP, NULL, ThreadServeur, NULL); // Pas sur que ça reste dans la classe peut-etre laisser dans le main
public:
    ExotestThreadServeur(std::string adresseIPServeur,unsigned short portServeur,std::string leCOM);
    void InitialisationDonneeZero(DonneeCAN tabDonnees,int nbTrames);
    bool OuvertureConnexionVSCOM(std::string leCOM,char vitesse[10]);
    void AffichageReceptionTrame();
    void DeconnexionFermetureVSCOM();
    static void * ThreadServeur(void * pDataBis);
    ~ExotestThreadServeur();
};
#endif

