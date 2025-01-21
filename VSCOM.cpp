#include "VSCOM.h"
#include <string>

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
		portCom=new Rs232(com.c_str());
		portCom->configurer(CBR_9600,8,NOPARITY,ONESTOPBIT);
		portCom->ouvrir();
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
bool VSCOM::ConnexionVSCOM()    //S0 � S8 : 10k, 20k, 50k, 100k, 125k, 250k, 500k, 800k, 1Mbits/s
{    char TrameConfig[30]="CFFFFFF\r";
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
				strcpy(TrameConfig,"S5\r");
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

void VSCOM::EnvoiTrameCAN(string trameVSCOM)
{   for(int i=0;i<trameVSCOM.length();i++) portCom->envoyer(trameVSCOM[i]);
	portCom->envoyer('\r');
}

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

