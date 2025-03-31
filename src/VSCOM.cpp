#include "VSCOM.h"


#ifdef __unix__
#include <unistd.h>
void Sleep(int ms) { usleep(ms*1000);}
#endif


VSCOM::VSCOM()
{    ouvert=false;connecteVSCOM=false;
}
void VSCOM::ModifierCOM(string scom)
{    
	com=scom;
}
bool VSCOM::OuvrirCOM()
{
		#ifdef __unix__
		portCom = new SNPortSerie();
		if(portCom->Ouvrir(com.c_str()) != 0)
		{
			cout << "Erreur lors de l'ouverture du port serie" << endl;
			return false;
		}
		if(!portCom->ModifierVitesse(V3000000_BAUD))
		{
			cout << "Erreur lors du param�trage de la vitesse" << endl;
			return false;
		}
		ouvert = true;
		cout << "Ouverture effectuee..." << endl;
		#elif _WIN32 || _WIN64
		portCom=new Rs232((char*)com.c_str());
		portCom->configurer(CBR_9600,8,NOPARITY,ONESTOPBIT);
		portCom->ouvrir();
		ouvert = true;
		#endif
    
/*	if(portCom->envoyer(0x02))	//envoi du caract�re 0x02 et test du port COM1
	{
		ouvert=true;
	}*/
	return ouvert;
}
void VSCOM::FermerCOM()
{    portCom->fermer();
	 delete portCom;
}
bool VSCOM::ConnexionVSCOM(char *Svitesse)    //S0 � S8 : 10k, 20k, 50k, 100k, 125k, 250k, 500k, 800k, 1Mbits/s
{    char TrameConfig[30]="CFFFFFF\r";
	strcpy(Svitesse,"\r");
	unsigned char c[10];
	if(ouvert)if (portCom->envoyer('v')!=-1)
		{       connecteVSCOM=true;
				portCom->envoyer('\r');
				Sleep(500);for(int i=0;i<7;i++) portCom->recevoir(&c[i]);
				c[7] = 0; cout << "MSG : " << c << endl;
				for(int i=0;i<strlen(TrameConfig);i++) portCom->envoyer(TrameConfig[i]);
				Sleep(500);portCom->recevoir(&c[0]);
				cout << "OCTET : " << hex << (int)c[0] << dec << endl;
				for(int i=0;i<strlen(TrameConfig);i++) portCom->envoyer(TrameConfig[i]);
				Sleep(500);portCom->recevoir(&c[0]);
				cout << "OCTET : " << hex << (int)c[0] << dec << endl;
				for(int i=0;i<strlen(TrameConfig);i++) portCom->envoyer(TrameConfig[i]);
				Sleep(500);portCom->recevoir(&c[0]);
				cout << "OCTET : " << hex << (int)c[0] << dec << endl;
				strcpy(TrameConfig,Svitesse);
				for(int i=0;i<strlen(TrameConfig);i++) portCom->envoyer(TrameConfig[i]);
				Sleep(500);portCom->recevoir(&c[0]);
				cout << "OCTET : " << hex << (int)c[0] << dec << endl;
				strcpy(TrameConfig,"Z1\r");
				for(int i=0;i<strlen(TrameConfig);i++) portCom->envoyer(TrameConfig[i]);
				Sleep(500);portCom->recevoir(&c[0]);
				cout << "OCTET : " << hex << (int)c[0] << dec << endl;
				strcpy(TrameConfig,"O\r");
				for(int i=0;i<strlen(TrameConfig);i++) portCom->envoyer(TrameConfig[i]);
				return true;
		}
		else return false;
}
void VSCOM::DeconnexionVSCOM()
{       char TrameConfig[30]="CFFFFFF\r";
	unsigned char c[10];
	if(ouvert)
		{	strcpy(TrameConfig,"C\r");
			for(int i=0;i<strlen(TrameConfig);i++) portCom->envoyer(TrameConfig[i]);
			Sleep(500);portCom->recevoir(&c[0]);
			connecteVSCOM=false;
		}
}

void VSCOM::EnvoiTrameCAN(DonneeCAN donneeCAN, bool standard)
{   string trame=ConstruitTrameVSCOM(donneeCAN, standard);
	for(int i=0;i<trame.length();i++) portCom->envoyer(trame[i]);
	portCom->envoyer('\r');
}
void VSCOM::EnvoiTrameCAN(string trameVSCOM)
{   for(int i=0;i<trameVSCOM.length();i++) portCom->envoyer(trameVSCOM[i]);
	portCom->envoyer('\r');
}
DonneeCAN VSCOM::ExtraitDonneeTrameVSCOM(string trameVSCOM, bool &standard)//t001280021245
{   DonneeCAN donneeCANExtrait;
	if(trameVSCOM.length()<5) return donneeCANExtrait;
	if(trameVSCOM[0]<'Z')        //extended
	{	donneeCANExtrait.identifiant=SNIR::ChaineHexaVersInt((char*)trameVSCOM.substr(1,8).c_str(),8);
		donneeCANExtrait.longueur=((int)trameVSCOM[9])-'0';
		donneeCANExtrait.requete=(trameVSCOM[0]=='R');
		if(!donneeCANExtrait.requete)
		{   for(int i=0;i<8;i++)
				if(i<donneeCANExtrait.longueur)
						donneeCANExtrait.donnee[i]=SNIR::ChaineHexaVersInt((char*)trameVSCOM.substr(10+2*i,2).c_str(),2);
				else	donneeCANExtrait.donnee[i]=0;
		}
		else for(int i=0;i<8;i++) donneeCANExtrait.donnee[i]=0;
		standard=false;
	}
	else                         //standard
	{   donneeCANExtrait.identifiant=SNIR::ChaineHexaVersInt((char*)("0"+trameVSCOM.substr(1,3)).c_str(),4);
		donneeCANExtrait.longueur=((int)trameVSCOM[4])-'0';
		if(donneeCANExtrait.longueur>8 || donneeCANExtrait.longueur<0) donneeCANExtrait.longueur=8;
		donneeCANExtrait.requete=(trameVSCOM[0]=='r');
		if(!donneeCANExtrait.requete)
		{	for(int i=0;i<8;i++)
				if(i<donneeCANExtrait.longueur)
						if(trameVSCOM.length()>=(5+2*i+2))
						donneeCANExtrait.donnee[i]=SNIR::ChaineHexaVersInt((char*)trameVSCOM.substr(5+2*i,2).c_str(),2);
				else	donneeCANExtrait.donnee[i]=0;
		}
		else for(int i=0;i<8;i++) donneeCANExtrait.donnee[i]=0;
		standard=true;
	}
	return donneeCANExtrait;
}
string VSCOM::ConstruitTrameVSCOM(DonneeCAN donneeCAN, bool standard)//false pour extended
{   //donneeCAN ->string ConstruitTrame(DonneeCAN donneeCAN)->I00000800R0L7:2A09021801110100
	//I00000800R0L7:2A09021801110100 -> standard : t80072A090218011101
	//								 -> extended : T0000080072A090218011101
	//I000003A1R0L3:1450000000000000 -> standard : t3A13145000
	//								 -> extended : T000003A13145000
	//I00000128R1L0:0000000000000000 -> standard : r1280
	//								 -> extended : R000001280
	//I00000128R1L3:0000000000000000 -> standard : t128300R0000       tester   r1283000000 ou r1283
	//								 -> extended : T00000128300R0000  tester   R000001283000000 ou R000001283
	MessageCANetendu mcan;
	string trame30=mcan.ConstruitTrame(donneeCAN);
	string trameVSCOM;
	if(standard && trame30[10]=='0') trameVSCOM="t";
	if(standard && trame30[10]=='1') trameVSCOM="r";
	if(!standard && trame30[10]=='0') trameVSCOM="T";
	if(!standard && trame30[10]=='1') trameVSCOM="R";
	//Extended
	if(trameVSCOM[0]<'Z')
	{	trameVSCOM=trameVSCOM+trame30.substr(1,8)+trame30[12];
		if(trameVSCOM[0]=='T') trameVSCOM=trameVSCOM+trame30.substr(14,2*(trame30[12]-'0'));
	}
	//Standard
	else
	{	trameVSCOM=trameVSCOM+trame30.substr(6,3)+trame30[12];
		if(trameVSCOM[0]=='t') trameVSCOM=trameVSCOM+trame30.substr(14,2*(trame30[12]-'0'));
	}
	return trameVSCOM;
}
//DonneeCAN VSCOM::ReceptionTrame()
string VSCOM::ReceptionTrame()
{   int Reponse;
	unsigned char TOctets[50];
	string trameVSCOM;
	int Nb=0;
	do
	{
		Reponse=portCom->recevoir(&TOctets[Nb]);
	}while((TOctets[Nb++]!='\r')&&(Reponse>=0)&&(Nb<50));
	TOctets[Nb]=0;
	if((TOctets[0]=='t'||TOctets[0]=='T'||TOctets[0]=='r'||TOctets[0]=='R') && TOctets[1]>='0' && TOctets[1]<='9')
	{	trameVSCOM=(const char *)TOctets;
		return trameVSCOM;
	}
	return "";
	//return ExtraitDonneeTrameVSCOM(trameVSCOM);
}
DonneeCAN VSCOM::ReceptionTrameFormatee()
{   int Reponse;
	MessageCANetendu messageCAN;
	DonneeCAN erreur;
	erreur.identifiant=0;
	unsigned char TOctets[50];
	bool trameStandard;
	string trameVSCOM;
	int Nb=0;
	do
	{
		Reponse=portCom->recevoir(&TOctets[Nb]);
	}while((TOctets[Nb++]!='\r')&&(Reponse>=0)&&(Nb<50));
	TOctets[Nb]=0;
	if((TOctets[0]=='t'||TOctets[0]=='T'||TOctets[0]=='r'||TOctets[0]=='R') && TOctets[1]>='0' && TOctets[1]<='9')
	{	trameVSCOM=(const char *)TOctets;
		if(trameVSCOM.length()<5) return erreur;
		//return messageCAN.ConstruitTrame(ExtraitDonneeTrameVSCOM(trameVSCOM,trameStandard));//+" "+trameVSCOM;
		//return ExtraitDonneeTrameVSCOM(trameVSCOM,trameStandard).identifiant;
		return ExtraitDonneeTrameVSCOM(trameVSCOM,trameStandard);
	}
	return erreur;
	//return ExtraitDonneeTrameVSCOM(trameVSCOM);
}

