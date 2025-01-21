#ifndef VSCOM_H
#define VSCOM_H
#include "SNPortSerie.h"
#include <iostream>
class VSCOM
{
private:
    string com;
    bool ouvert,connecteVSCOM;
public:
    SNPortSerie * portCom;
    VSCOM();
    void ModifierCOM(string scom);
    bool OuvrirCOM();
    void FermerCOM();
    bool ConnexionVSCOM();
    void DeconnexionVSCOM();
    void EnvoiTrameCAN(string trameVSCOM);
    string ReceptionTrame();
};
#endif

