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
    /* data */
    VSCOM vscom;
    int m_nbTrames[2048];
    std::string m_tabDonneesFormatee[2048];
    DonneeCAN m_tabDonnees[2048];
    bool OK = true;
    std::string m_leCOM;
public:
    ExotestThreadServeur(std::string leCOM);
    void InitialisationDonneeZero(DonneeCAN tabDonnees,int nbTrames);
    bool OuvertureConnexionVSCOM(std::string leCOM,char vitesse[10]);
    void AffichageReceptionTrame();
    void DeconnexionFermetureVSCOM();
    ~ExotestThreadServeur();
};
#endif

