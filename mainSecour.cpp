#include <iostream>
#include <stdio.h>
#include <pthread.h>
#include <thread>
#include <sys/time.h>
#include "VSCOM.h"
#include "ServeurTCP.h"
typedef char _TCHAR;
#define _tmain main

using namespace std;
VSCOM vscom;
ServeurTCP serveur("0.0.0.0",2025);
int nbTrames[2048];
string tabDonneesFormatee[2048];
DonneeCAN tabDonnees[2048];     //11 bits
void * ThreadServeur(void * pDataBis)
{	char message[1501];
	while(true)
	{	//message du client
		int nbOctets=serveur.Recevoir(message,1500);
		
		if(nbOctets)          //si "0" : envoi de toutes les trames //CODER ENVOITRAMECAN(DONNEE)
		{	message[nbOctets]=0;
			cout<<message<<endl;
			//clrscr();
			
			int id,r_long;unsigned char r_donnee[8]={3,4,5,6,7,8,9,10};
			char r_donnees[30];
			sscanf(message,"%X",&id);
			//"115 [ 4 octets ] : 0800000000000000"
			if(strlen(message)>3 && message[4]=='[' && message[15]==']')
			{	sscanf(message,"%X [ %d octets ] : %s",&id,&r_long,r_donnees);
				//cout<<"="<<id<<"="<<r_long<<"="; //cout<<r_donnees<<endl;
				for(int rd=0;rd<8;rd++) r_donnee[rd]=SNIR::ChaineHexaVersInt(r_donnees+2*rd,2);
				//for(int rd=0;rd<8;rd++)cout<<(int)r_donnee[rd]<<" ";cout<<endl;
				DonneeCAN donneeAEnvoyer;
				donneeAEnvoyer.identifiant=id;
				donneeAEnvoyer.longueur=r_long;
				donneeAEnvoyer.requete=false;
				for(int rd=0;rd<8;rd++) donneeAEnvoyer.donnee[rd]=r_donnee[rd];
				vscom.EnvoiTrameCAN(donneeAEnvoyer,true);
				
			}
			else
			{   if(id)
					serveur.Envoyer((char*)tabDonneesFormatee[id].c_str(),tabDonneesFormatee[id].length());
				else
					for(int i=0;i<2048;i++)
					{	if(tabDonnees[i].identifiant)
						{	serveur.Envoyer((char*)(tabDonneesFormatee[i]+"\n").c_str(),tabDonneesFormatee[i].length()+1);
						}
					}
				cout<<"Message du client : "<<tabDonneesFormatee<<endl;
			}
			//Sleep(2000);
		}
	}	
}

int main(){
    
    string leCOM="/dev/ttyUSB0";
    
	

	for(int i=0;i<2048;i++) {tabDonnees[i].identifiant=0;nbTrames[i]=0;}
	bool OK=true;
	vscom.ModifierCOM(leCOM);
	if(vscom.OuvrirCOM()) cout<<"Ouverture COM OK"<<endl;
	else cout<<"Ouverture COM KO"<<endl;
	char vitesse[10]="S4";
	if(vscom.ConnexionVSCOM(vitesse)) cout<<"Connexion OK"<<endl;
	else {cout<<"Connexion KO"<<endl; OK=false;}
	
	cout<<"En attente du client..."<<endl;
	serveur.AttendreClient();
	pthread_t thread_TCP;
	pthread_create( & thread_TCP, NULL, ThreadServeur, NULL);

	while(OK)
	{   DonneeCAN d=vscom.ReceptionTrameFormatee();
		if(d.identifiant)
		{	//cout<<d.identifiant<<"["<<d.longueur<<"]"<<endl;
			tabDonnees[d.identifiant].identifiant=d.identifiant;
			for(int j=0;j<8;j++) tabDonnees[d.identifiant].donnee[j]=d.donnee[j];
			tabDonnees[d.identifiant].longueur=d.longueur;
			tabDonnees[d.identifiant].requete=d.requete;
			nbTrames[d.identifiant]++;
			char trameFormatee[1500]="",donnee[1500]="";
			sprintf(donnee,"%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X",d.donnee[0],d.donnee[1],d.donnee[2],d.donnee[3],d.donnee[4],d.donnee[5],d.donnee[6],d.donnee[7]);
			sprintf(trameFormatee,"%.3X [ %d octets ] : %s",d.identifiant,d.longueur,donnee);
			tabDonneesFormatee[d.identifiant]=trameFormatee;
			//clrscr();
			cout<<endl<<endl<<"=============================================="<<endl;
			//"0B6 [ 8 octets ] : 5600160041547ED0"
			for(int i=0;i<2048;i++)
			{	if(tabDonnees[i].identifiant)
				{	printf("%.3X ",tabDonnees[i].identifiant);
					cout<<"\t"<<tabDonnees[i].longueur<<"\t";
					for(int j=0;j<8;j++)
					{ printf("%.2X ",tabDonnees[i].donnee[j]);
					}
					cout<<" nb : "<<nbTrames[i];
					cout<<endl;
				}
			}
			cout<<"=============================================="<<endl<<endl<<endl;
			//cin.get();
		}
	}
	serveur.FermerCommunication();
	vscom.DeconnexionVSCOM();
	vscom.FermerCOM();



	//cout<<"Reception des octets du client : "<<nbOctets<<endl;
    // VSCOM com;
	// char trameTCP[50];
    // char ReponseTCP[20] ="I00000115R0L4";
    // std::string repTCP="I00000115R0L4";

    // com.ModifierCOM("/dev/ttyUSB0");
    // if(com.OuvrirCOM()) cout<<"COM OUVERT"<<endl; else cout<<"ERREUR DE COM"<<endl;
    // if(com.ConnexionVSCOM("S5")) cout<<"CONNEXION AU VSCOM"<<endl; else cout<<"ERREUR DE CONNEXION AU VSCOM"<<endl;    ;
    // com.EnvoiTrameCAN(ReponseTCP); 
    // cout<<"ENVOI DU MESSAGE : "<<ReponseTCP<<endl; 
    // com.DeconnexionVSCOM(); 
    
    // com.FermerCOM();

    // ServeurTCP serveur("0.0.0.0",8080);
    // cout<<"Le serveur est en ecoute...."<<endl;
    // while (true)
    // {
    //     int reception = serveur.Recevoir(trameTCP,strlen(ReponseTCP),1000);
    //     cout<<"Reception du client : "<<reception<<endl;
    //     serveur.AttendreClient();
    //     serveur.Envoyer(ReponseTCP,strlen(ReponseTCP));
    // }
    // serveur.FermerCommunication();

    return 0;
}


