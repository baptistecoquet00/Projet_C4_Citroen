#include "MessageCAN.h"

#include <string.h>
// cette classe transforme "I00000800R0L7:2A09021801110100"
// en id=0x800, donnee[0]=0x2A, donnee[1]=0x09, longueur=7, requete=false
// informe sur la direction, et la signification de la trame
// et réciproquement
/*
typedef struct{
    unsigned char donnee[8];
    unsigned char identifiant[4];
    int longueur;
    bool requete; //=true pour une requete
} DonneeCAN;
*/
//	static unsigned int ChaineHexaVersInt(char ChaineHexa[20],int NbCar);
MessageCANetendu::MessageCANetendu()
{   messagerieChargee=false;
}
DonneeCAN MessageCANetendu::ExtraitDonnee(string trame)
{   char tab[31];
	strcpy(tab, trame.c_str());
	donneeCAN.longueur=SNIR::ChaineHexaVersInt(tab+12,1);
	if(tab[10]=='0')
		donneeCAN.requete=false;
	else
		donneeCAN.requete=true;
	if(!donneeCAN.requete)
	{	for(int i=0;i<8;i++)
			if(i<donneeCAN.longueur) donneeCAN.donnee[i]=SNIR::ChaineHexaVersInt(tab+14+i*2,2);
			else donneeCAN.donnee[i]=0;
	}
	else for(int i=0;i<8;i++) donneeCAN.donnee[i]=0;
	donneeCAN.identifiant=SNIR::ChaineHexaVersInt(tab+1,8);
	return donneeCAN;
}
//j'en suis ici

string MessageCANetendu::ConstruitTrame(unsigned char donnee[8],unsigned int identifiant,int longueur,bool requete)
{   string TrameCree = "I";
	TrameCree+=SNIR::HexToString((int)identifiant);
	TrameCree+='R';
	if(requete==false)	TrameCree+='0';
	else	TrameCree+='1';
	TrameCree+='L';
	TrameCree+=SNIR::UnsignedDecToString(longueur);
	TrameCree+=':';
	for(int i=0;i<8;i++)	TrameCree+=SNIR::HexToString((char)donnee[i]);
	return TrameCree;
}
string MessageCANetendu::ConstruitTrame(DonneeCAN donneeCAN)
{   string TrameCree="I";
	TrameCree+=SNIR::HexToString((int)donneeCAN.identifiant);
	TrameCree+='R';
	if(donneeCAN.requete==false)	TrameCree+='0';
	else	TrameCree+='1';
	TrameCree+='L';
	TrameCree+=SNIR::UnsignedDecToString(donneeCAN.longueur);
	TrameCree+=':';
	for(int i=0;i<8;i++)		TrameCree+=SNIR::HexToString((char)donneeCAN.donnee[i]);
	return TrameCree;
}
string MessageCANetendu::significationTrame(string trame/*I00000100*/,char nomFichier[100])
{   //I00000100R0L1:0000000000000000	> Mode Stop
	ifstream fichierMessagerie;
	trame=trame.substr(0,9);//choix de l'ID seulement
	char Trame[40];
	char Commentaire[1000];
	string s;
	int i=0;
	if(!messagerieChargee)
	{   fichierMessagerie.open(nomFichier);
		if(fichierMessagerie.is_open())
		{	do
			{	fichierMessagerie.get(Trame,40,'\t');
				fichierMessagerie.get(Commentaire,1000,'\n');
				fichierMessagerie.get();
				contenuMessagerie[i][0]=Trame;
				s=Commentaire;
				if(s.length()>1) contenuMessagerie[i][1]=s.substr(1,1);
				if(s.length()>3) contenuMessagerie[i][2]=s.substr(3,s.length());
				i++;
			}while(!fichierMessagerie.eof());
			nbMessages=i-1;
			fichierMessagerie.close();
			if(nbMessages==0) return "Erreur messagerie";
			messagerieChargee=true;
		}
		else return "Erreur fichier";
	}
	for(int t=0;t<nbMessages;t++)
	{   if(contenuMessagerie[i][0].find(trame)!=-1)
		{	return contenuMessagerie[i][2];
		}
	}
	return "Non trouvé";
}

bool MessageCANetendu::directionTrame(string trame,char nomFichier[100])
{	ifstream fichierMessagerie;
	fichierMessagerie.open("Trames.txt");
	char Identifiants[40];
	char Contenu[1000];
	char cTrame[31];
	strcpy(cTrame, trame.c_str());
	if(fichierMessagerie.is_open()){
		do{
			fichierMessagerie.get(Identifiants,40,'\t');
			fichierMessagerie.get(Contenu,1000,'\n');
			fichierMessagerie.get();

			if(strcmp (cTrame, Identifiants) == 0){
				//cout << Contenu[1] << endl;
				if(Contenu[1] == '>'){
					fichierMessagerie.close();
					return true;
				}
				else{
					fichierMessagerie.close();
					return false;
				}
			}
		}while(!fichierMessagerie.eof());
	fichierMessagerie.close();
	}
	//return true;
}

DonneeCAN MessageCANetendu::ExtraitDonneeTrameVSCOM(string trVSCOM)//t001280021245
{   DonneeCAN donneeCANExtrait;
    char Extrait[31];
    strcpy(Extrait, trVSCOM.c_str());
    /*for(int i=0; i<8; i++)
    {*/     if(Extrait[0]=='t' && Extrait[6]!='r')//standard avec requete=0
        {
            donneeCANExtrait.identifiant=SNIR::ChaineHexaVersInt(Extrait + 1, 3);
            donneeCANExtrait.longueur=SNIR::ChaineHexaVersInt(Extrait + 4, 1);
            donneeCANExtrait.requete=0;
            for(int i=0; i<8; i++)
            {
                donneeCANExtrait.donnee[i] = SNIR::ChaineHexaVersInt(Extrait + 5 + i * 2, 2);
            }
        }
        else if(Extrait[0]=='T' && Extrait[11]!='R')//extended avec requete=0
        {
            donneeCANExtrait.identifiant=SNIR::ChaineHexaVersInt(Extrait + 1, 8);
            donneeCANExtrait.longueur=SNIR::ChaineHexaVersInt(Extrait + 9, 1);
            donneeCANExtrait.requete=0;
            for(int i=0; i<8; i++)
            {
                donneeCANExtrait.donnee[i] = SNIR::ChaineHexaVersInt(Extrait + 10 + i * 2, 2);
            }
        }
        else if(Extrait[0]=='r' && Extrait[4]=='0')//standard avec requete=1 et longueur=0
        {
            donneeCANExtrait.identifiant=SNIR::ChaineHexaVersInt(Extrait + 1, 3);
            donneeCANExtrait.longueur=0;
            donneeCANExtrait.requete=1;
            for(int i=0; i<8; i++)
            {
                donneeCANExtrait.donnee[i]=0;
            }
        }
        else if(Extrait[0]=='R' && Extrait[9]=='0')//extended avec requete=1 et longueur=0
        {
            donneeCANExtrait.identifiant=SNIR::ChaineHexaVersInt(Extrait + 1, 8);
            donneeCANExtrait.longueur=0;
            donneeCANExtrait.requete=1;
             for(int i=0; i<8; i++)
            {
                donneeCANExtrait.donnee[i]=0;
            }
        }
        /*else if(Extrait[0]=='t' && Extrait[6]=='R')//standard avec requete=1 et longueur!=0
        {
            donneeCANExtrait.identifiant=SNIR::ChaineHexaVersInt(Extrait + 1, 5);
            donneeCANExtrait.longueur=SNIR::ChaineHexaVersInt(Extrait + 4, 1);
            char chars[] = "R";
            for(int i=0; i<8; i++)
            {
                for (unsigned int n = 0; n < strlen(chars); ++n)
                {
                    std::string str(Extrait);
                    str.erase (std::remove(str.begin(), str.end(), chars[n]), str.end());
                    strcpy(Extrait, str.c_str());
                    donneeCANExtrait.donnee[i] = SNIR::ChaineHexaVersInt(Extrait + 5 + i * 2, 2);
                }
            }
            donneeCANExtrait.requete=1;

        }*/
        /*else if(Extrait[0]=='T' && Extrait[11]=='R')//extended avec requete=1 et longueur!=0
        {
            donneeCANExtrait.identifiant=SNIR::ChaineHexaVersInt(Extrait + 1, 8);
            donneeCANExtrait.longueur=SNIR::ChaineHexaVersInt(Extrait + 9, 1);
            donneeCANExtrait.requete=1;
            for(int i=0; i<8; i++)
            {
                char chars[] = "R";
                for (unsigned int n = 0; n < strlen(chars); ++n)
                {
                    std::string str(Extrait);
                    str.erase (std::remove(str.begin(), str.end(), chars[n]), str.end());
                    strcpy(Extrait, str.c_str());
                    donneeCANExtrait.donnee[i] = SNIR::ChaineHexaVersInt(Extrait + 10 + i * 2, 2);
                }

            }
        }*/
    return donneeCANExtrait;
}

string MessageCANetendu::ConstruitTrameVSCOM(DonneeCAN donneeCAN, bool standard)//false pour extended
{   //donneeCAN ->string ConstruitTrame(DonneeCAN donneeCAN)->I00000800R0L7:2A09021801110100
    //I00000800R0L7:2A09021801110100 -> standard : t80072A090218011101
    //								 -> extended : T0000080072A090218011101
    //I000003A1R0L3:1450000000000000 -> standard : t3A13145000
    //								 -> extended : T000003A13145000
    //I00000128R1L0:0000000000000000 -> standard : r1280
    //								 -> extended : R000001280
    //I00000128R1L3:0000000000000000 -> standard : t128300R0000
    //								 -> extended : T00000128300R0000
    MessageCANetendu mcan;
    string trame30=mcan.ConstruitTrame(donneeCAN);
    string trameVSCOM;
    if((standard && trame30[10]=='0') || (standard && trame30[10]=='1' && trame30[12]!='0')) trameVSCOM="t";
    if(standard && trame30[10]=='1' && trame30[12]=='0') trameVSCOM="r";
    if((!standard && trame30[10]=='0')||(!standard && trame30[10]=='1' && trame30[12]!='0')) trameVSCOM="T";
    if(!standard && trame30[10]=='1' && trame30[12]=='0') trameVSCOM="R";
    //Extended
    if(trameVSCOM[0]<'Z')
    {	trameVSCOM=trameVSCOM+trame30.substr(1,8)+trame30[12]+trame30.substr(14,2*(trame30[12]-'0'));
    //Cas d'une requête avec longueur non nulle
        if(trame30[10]=='1'&&trame30[12]!='0')trameVSCOM.insert(11,"R");
    }

    //Standard
    else
    {	trameVSCOM=trameVSCOM+trame30.substr(6,3)+trame30[12]+trame30.substr(14,2*(trame30[12]-'0'));
    //Cas d'une requête avec longueur non nulle
        if(trame30[10]=='1'&&trame30[12]!='0')trameVSCOM.insert(6,"R");
    }
    return trameVSCOM;
}