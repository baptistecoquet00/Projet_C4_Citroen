#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <thread>
#include <unistd.h>
#include <sys/time.h>
#include <mutex>
#include <chrono>

#include "VSCOM.h"
#include "ServeurTCP.h"
#include "JSONFile.h"
#include "ClientTCP.h"
#include "YamlConfigParser.h"
typedef char _TCHAR;
#define _tmain main

using namespace std;

/*Prévoir fichier texte :
4	//(1)CAN (2)SERVEURTCP (3)CLIENTREST (4)COMPLET
/dev/ttyUSB0	//Port USB
2085	//Port serveur Raspberry
172.20.21.73	//IP serveur REST
4000	//Port serveur REST
*/
string testUnitaire;
//"CAN", "SERVEURTCP", "CLIENTREST", "COMPLET"
YamlConfigParser parser("config_serveur.yml");
VSCOM vscom;
ServeurTCP serveur("0.0.0.0",2085);
bool EnvoiTrameCAN=false;
ClientTCP client;
int nbTrames[2048];
string tabDonneesFormatee[2048];
DonneeCAN tabDonnees[2048];
int PortServeurTCP,PortServeurRest;
string AddrServeurRest;
string AddrEtPortServeurRest;     //11 bits
DonneeCAN donneeAEnvoyer;
string messageCANAEnvoyer;

const std::string currentDateTime() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = * localtime( & now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", & tstruct);
    return buf;
}

void SauvegarderDonneesCAN()
{	ofstream fichierDonneeCAN;
	fichierDonneeCAN.open("DonneeCAN.log");
	fichierDonneeCAN<<currentDateTime()<<"\n";
	for(int i=0;i<2048;i++)
	{	if(tabDonnees[i].identifiant)
		{	fichierDonneeCAN<<tabDonneesFormatee[i]<<"\t"<<nbTrames[i]<<"\n";	
		}
	}
	fichierDonneeCAN.close();
}
void ChargerDonneesCAN()
{	ifstream fichierDonneeCAN;
	fichierDonneeCAN.open("DonneeCAN.log");
	char ligne[201],donnee[100],trameFormatee[200];
	fichierDonneeCAN.getline(ligne,200);
	int id,lg,nb;
	while(!fichierDonneeCAN.eof())
	{	fichierDonneeCAN.getline(ligne,200);
		sscanf(ligne,"%X [ %d octets ] : %s\t%d",&id,&lg,donnee,&nb);
		nbTrames[id]=nb;
		sprintf(trameFormatee,"%.3X [ %d octets ] : %s",id,lg,donnee);
		tabDonneesFormatee[id]=trameFormatee;
		tabDonnees[id].requete=false;//non géré
		tabDonnees[id].longueur=lg;
		tabDonnees[id].identifiant=id;
		tabDonnees[id].donnee[0]=SNIR::ChaineHexaVersInt(donnee,2);
		tabDonnees[id].donnee[1]=SNIR::ChaineHexaVersInt(donnee+2,2);
		tabDonnees[id].donnee[2]=SNIR::ChaineHexaVersInt(donnee+4,2);
		tabDonnees[id].donnee[3]=SNIR::ChaineHexaVersInt(donnee+6,2);
		tabDonnees[id].donnee[4]=SNIR::ChaineHexaVersInt(donnee+8,2);
		tabDonnees[id].donnee[5]=SNIR::ChaineHexaVersInt(donnee+10,2);
		tabDonnees[id].donnee[6]=SNIR::ChaineHexaVersInt(donnee+12,2);
		tabDonnees[id].donnee[7]=SNIR::ChaineHexaVersInt(donnee+14,2);
	}
	fichierDonneeCAN.close();
}
void * ThreadVSCOM(void * pDataBis)
{	while(true)
	if(!EnvoiTrameCAN)
	{	DonneeCAN d=vscom.ReceptionTrameFormatee();
		if(d.identifiant)
		{	tabDonnees[d.identifiant].identifiant=d.identifiant;
			for(int j=0;j<8;j++) tabDonnees[d.identifiant].donnee[j]=d.donnee[j];
			tabDonnees[d.identifiant].longueur=d.longueur;
			tabDonnees[d.identifiant].requete=d.requete;
			nbTrames[d.identifiant]++;
			char trameFormatee[1500]="",donnee[1500]="";
			sprintf(donnee,"%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X",d.donnee[0],d.donnee[1],d.donnee[2],d.donnee[3],d.donnee[4],d.donnee[5],d.donnee[6],d.donnee[7]);
			sprintf(trameFormatee,"%.3X [ %d octets ] : %s",d.identifiant,d.longueur,donnee);
			tabDonneesFormatee[d.identifiant]=trameFormatee;
			//cout<<"Lecture bus CAN ID : "<<d.identifiant<<endl;
		}
	}
	else
	{	vscom.EnvoiTrameCAN(donneeAEnvoyer,true);
		cout<<"Envoi de la trame CAN : "<<messageCANAEnvoyer<<endl;
		usleep(500000);
		EnvoiTrameCAN=false;	
	}
}
void * ThreadServeur(void * pDataBis)
{	char message[1501];
	bool connecte = true;
	while(connecte)
	{	//message du client
		int nbOctets=serveur.Recevoir(message,1501);
		if(nbOctets)          //si "0" : envoi de toutes les trames //CODER ENVOITRAMECAN(DONNEE)
		{	message[nbOctets]=0;
			//cout<<"Message du client recue : "<<message<<endl;
			int id,r_long;unsigned char r_donnee[8]={3,4,5,6,7,8,9,10};
			char r_donnees[30];
			sscanf(message,"%X",&id);
			//"115 [ 4 octets ] : 0800000000000000"
			if(strlen(message)>3 && message[4]=='[' && message[15]==']')
			{	cout<<"Message du client (trame complete) : "<<message<<endl;
				sscanf(message,"%X [ %d octets ] : %s",&id,&r_long,r_donnees);
				//cout<<"="<<id<<"="<<r_long<<"="; //cout<<r_donnees<<endl;
				for(int rd=0;rd<8;rd++) r_donnee[rd]=SNIR::ChaineHexaVersInt(r_donnees+2*rd,2);
				//for(int rd=0;rd<8;rd++)cout<<(int)r_donnee[rd]<<" ";cout<<endl;
				donneeAEnvoyer.identifiant=id;
				donneeAEnvoyer.longueur=r_long;
				donneeAEnvoyer.requete=false;
				messageCANAEnvoyer=message;
				for(int rd=0;rd<8;rd++){donneeAEnvoyer.donnee[rd]=r_donnee[rd];} 
				if(testUnitaire=="COMPLET")
				{	EnvoiTrameCAN=true;
				}
			}
			else
			{   if(id)
				{	serveur.Envoyer((char*)tabDonneesFormatee[id].c_str(),tabDonneesFormatee[id].length());
				}
				else
				{	for(int i=0;i<2048;i++)
					{	if(tabDonnees[i].identifiant)
						{	serveur.Envoyer((char*)(tabDonneesFormatee[i]+"\n").c_str(),tabDonneesFormatee[i].length()+1);
						}
					}
				}
			cout<<"Message du client (identifiant) : "<<message<<endl;
			}	
		}
	}
}

string AffichageDonneeCAN_CreationJSON()
{	JSONFile json("TestUser","CAN.json");
	cout<<endl<<endl<<"=============================================="<<endl;
	//"0B6 [ 8 octets ] : 5600160041547ED0"
	for(int i=0;i<2048;i++)
	{	if(tabDonnees[i].identifiant)
		{	printf("%.3X ",tabDonnees[i].identifiant);
			cout<<"\t"<<tabDonnees[i].longueur<<"\t";
			for(int j=0;j<8;j++)
			{	printf("%.2X ",tabDonnees[i].donnee[j]);
			}
			cout<<" nb : "<<nbTrames[i];
			cout<<endl;	
			json.AjouterDonneesJSON(tabDonnees[i].identifiant, tabDonnees[i].longueur, tabDonnees[i].donnee);		
		}
	}
	json.CloreJSON();
	cout<<"=============================================="<<endl<<endl<<endl;
	return json.getJSON();
}
int EnvoyerDonneesAuServeurREST(string addrEtPortServeur, string jsonstr)
{	int LengthJSON;
	for(int i = 0;i<jsonstr.length();i++) LengthJSON =i;
	string LengthJSONstr = std::to_string(LengthJSON+1);
	string requete = "POST /api/trame HTTP/1.1\r\nContent-Type: application/json\r\nHost: "+addrEtPortServeur+"\r\nContent-Length: "+(LengthJSONstr)+"\r\nConnection: keep-alive\r\n\r\n"+jsonstr+"\n";
	cout<<"Requete HTTP : "<<requete<<endl;
	client.Envoyer(requete);
}

void Parametrage_Serveur(){
	//cout<<"Veuillez choisir le test Unitaire entre \"CAN\", \"SERVEURTCP\", \"CLIENTREST\", \"COMPLET\" : ";
	testUnitaire = parser.getValue("config_serveur", "test_unitaire");;
	//testUnitaire ="COMPLET";
	cout<<"Le test Unitaire est : "<<testUnitaire<<"\n";
	if(testUnitaire=="SERVEURTCP" || testUnitaire=="COMPLET"){
	//cout<<"Veuillez choisir le port du Seveur TCP : ";
	//cin>>PortServeurTCP;
	PortServeurTCP =parser.getIntValue("config_serveur", "serveurTCP_port");;
	cout<<"Le port du serveur TCP est : "<<PortServeurTCP<<"\n";
	}
	if(testUnitaire=="CLIENTREST" || testUnitaire=="COMPLET"){
		cout<<"Veuillez choisir indiquer l'adresse du Serveur Rest : ";
		//cin>>AddrServeurRest;
		
		AddrServeurRest=parser.getValue("config_serveur", "addrServeurRest");
		PortServeurRest=parser.getIntValue("config_serveur", "PortServeurRest");
		cout<<"L'adresse du Serveur Rest : "<<AddrServeurRest<<"\n";
		//cout<<"Veuillez indiqué le port du Serveur Rest : ";
		//cin>>PortServeurRest;
		cout<<"Le port du serveur Rest est : "<<PortServeurRest<<"\n";
		AddrEtPortServeurRest = AddrServeurRest;
		AddrEtPortServeurRest +=":";
		AddrEtPortServeurRest+=std::to_string(PortServeurRest);
		cout<<"Adresse et Port du Serveur Rest : "<<AddrEtPortServeurRest<<endl;
		
	}

	// parser.printConfig();

    // std::cout << "\nPort TCP: " << parser.getIntValue("config_serveur", "serveurTCP_port") << "\n";
    // std::cout << "Adresse Serveur REST: " << parser.getValue("config_serveur", "ServeurRest.addrServeurRest") << "\n";
    // std::cout << "Port Serveur REST: " << parser.getIntValue("config_serveur", "ServeurRest.PortServeurRest") << "\n";
}
//AddrServeurRest="172.20.21.73";
		//AddrServeurRest="172.20.21.26";
int main(){
	bool OK=true;
	//AJOUTER 
	//cin "CAN", "SERVEURTCP", "CLIENTREST", "COMPLET"
	//cout port ServeurTCP 2085;
	//cin IP serveur REST 172.18.110.111
	//cout port serveur REST 3000
	Parametrage_Serveur();
	if(testUnitaire=="CAN" || testUnitaire=="COMPLET")
	{	string leCOM="/dev/ttyUSB0";
		for(int i=0;i<2048;i++) {tabDonnees[i].identifiant=0;nbTrames[i]=0;}
		vscom.ModifierCOM(leCOM);
		if(vscom.OuvrirCOM()) cout<<"Ouverture COM OK"<<endl;
		else cout<<"Ouverture COM KO"<<endl;
		char vitesse[10]="S4";
		if(vscom.ConnexionVSCOM(vitesse)) 
		{	cout<<"Connexion OK"<<endl;
			pthread_t thread_VSCOM;
			pthread_create( & thread_VSCOM, NULL, ThreadVSCOM, NULL);
		}
		else {cout<<"Connexion KO"<<endl; OK=false;}
	}	
	if(testUnitaire=="CLIENTREST" || testUnitaire=="COMPLET")
	{	cout<<"Connexion au Serveur REST......"<<endl;
		if(client.SeConnecterAUnServeur(AddrServeurRest,PortServeurRest)){
		//if(client.SeConnecterAUnServeur("172.18.110.111",3000)){
			cout<<"Connexion REST : OK"<<endl;
		}else{
			cout<<"Connexion REST : NOK"<<endl;
		 }
	}
	if(testUnitaire=="SERVEURTCP" || testUnitaire=="COMPLET")
	{	cout<<"En attente du client..."<<endl;
		serveur.AttendreClient();
		pthread_t thread_TCP;
		pthread_create( & thread_TCP, NULL, ThreadServeur, NULL);
	}
	if(testUnitaire=="SERVEURTCP" || testUnitaire=="CLIENTREST")
	{	ChargerDonneesCAN();
	}
	while(OK)
	{   	string leJSON=AffichageDonneeCAN_CreationJSON();
		if(testUnitaire=="CAN" || testUnitaire=="COMPLET")
		{	SauvegarderDonneesCAN();
		}
		usleep(2000000);
		if(testUnitaire=="CLIENTREST" || testUnitaire=="COMPLET")
		{	//EnvoyerDonneesAuServeurREST("172.18.110.111:3000",leJSON);			
			EnvoyerDonneesAuServeurREST(AddrEtPortServeurRest,leJSON);			
		}
	}
	//serveur.FermerCommunication();
	//vscom.DeconnexionVSCOM();
	//vscom.FermerCOM();
	return 0;
}


