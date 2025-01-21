#ifndef VSCOM_H
#define VSCOM_H

#ifdef __unix__
#include "SNPortSerie.h"
#elif _WIN32 || _WIN64
#endif

class VSCOM
{
private:
    string com;
	bool ouvert,connecteVSCOM;
public:
    VSCOM();
    ~VSCOM();
    void ModifierCOM(string scom);
    bool OuvrirCOM();
    void FermerCOM();
    bool ConnexionVSCOM();
    void DeconnexionVSCOM();
    void EnvoiTrameCAN(string trameVSCOM)
    string ReceptionTrame();
};


