#ifndef MESSAGECANETENDU_H
#define MESSAGECANETENDU_H
#include <string>
#include "SNIRconversions.h"
#include <fstream>
#include <iostream>

using namespace std;

typedef struct{
    unsigned char donnee[8];
    unsigned int identifiant;
    int longueur;
    bool requete; //=true pour une requete
} DonneeCAN;

class MessageCANetendu
{
private:
    string message;
	DonneeCAN donneeCAN;
	string contenuMessagerie[500][3];   //trame, sens (>ou<), commentaire
	//I00000100R0L1:0000000000000000	> Mode Stop
	int nbMessages;
	bool messagerieChargee;
public:
    MessageCANetendu();
    DonneeCAN ExtraitDonnee(string trame);
    string ConstruitTrame(unsigned char donnee[8],unsigned int identifiant,int longueur,bool requete);
	string ConstruitTrame(DonneeCAN donneeCAN);
	string significationTrame(string trame/*I00000100*/,char nomFichier[100]); // renvoi le commentaire trouvé dans le fichier messagerie.txt
	bool directionTrame(string trame/*I00000100*/,char nomFichier[100]); //=true en sortie
	DonneeCAN ExtraitDonneeTrameVSCOM(string trVSCOM);//t001280021245
	void EnvoiTrameCAN(DonneeCAN donneeCAN, bool standard);
    string ConstruitTrameVSCOM(DonneeCAN donneeCAN, bool standard);//false pour extended
};
#endif // MESSAGECANETENDU_H
