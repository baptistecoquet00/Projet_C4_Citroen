#ifndef VSCOM_H
#define VSCOM_H

#include "MessageCAN.h"

#ifdef __unix__
#include "SNPortSerie.h"
#elif _WIN32 || _WIN64
#include "PortComDI.h"
#endif

class VSCOM {
	private:

		string com;
		bool ouvert,connecteVSCOM;
	public:
		#ifdef __unix__
		SNPortSerie * portCom;
		#elif _WIN32 || _WIN64
		Rs232 *portCom;
		#endif
		VSCOM();
		void ModifierCOM(string scom);
		bool OuvrirCOM();  //false si erreur
		void FermerCOM();
		bool ConnexionVSCOM(char *Svitesse);
		void DeconnexionVSCOM();
		bool ConnecteVSCOM(){return connecteVSCOM;};
		bool Ouvert(){ return ouvert;};
		void EnvoiTrameCAN(DonneeCAN donneeCAN, bool standard);
		void EnvoiTrameCAN(string trameVSCOM);
		DonneeCAN ExtraitDonneeTrameVSCOM(string trameVSCOM, bool &standard);//t001280021245
		string ConstruitTrameVSCOM(DonneeCAN donneeCAN, bool standard);//false pour extended
		//DonneeCAN ReceptionTrame();
		DonneeCAN ReceptionTrameFormatee();
		string ReceptionTrame();
};
#endif
