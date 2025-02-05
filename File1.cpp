#pragma hdrstop
#pragma argsused

#ifdef _WIN32
#include <tchar.h>
#else
  typedef char _TCHAR;
  #define _tmain main
#endif
#include <stdio.h>
#include <conio.h>
#include "VSCOM.h"
#include "IRServeurTcpMonoclient.h"
 int _tmain(int argc, _TCHAR* argv[]) 
{   VSCOM vscom;
	string leCOM;
	IRServeurTcpMonoClient serveur;
	serveur.Initialisation();
	serveur.MettreEnEcouteSurLePort(2025);
	DonneeCAN tabDonnees[2048];     //11 bits
	int nbTrames[2048];
	string tabDonneesFormatee[2048];
	for(int i=0;i<2048;i++) {tabDonnees[i].identifiant=0;nbTrames[i]=0;}
	bool OK=true;
	cout<<"COM? [ex COM5] ";cin>>leCOM;
	vscom.ModifierCOM(leCOM);
	if(vscom.OuvrirCOM()) cout<<"Ouverture COM OK"<<endl;
	else cout<<"Ouverture COM KO"<<endl;
	if(vscom.ConnexionVSCOM("S4")) cout<<"Connexion OK"<<endl;
	else {cout<<"Connexion KO"<<endl; OK=false;}
	char message[1501];
	cout<<"En attente du client..."<<endl;
	serveur.AttendreUnNouveauClient();
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
			clrscr();
			cout<<endl<<endl<<"=============================================="<<endl;
			//"0B6 [ 8 octets ] : 5600160041547ED0"
			for(int i=0;i<2048;i++)
			{	if(tabDonnees[i].identifiant)
				{	printf("%.3X ",tabDonnees[i].identifiant);
					cout<<"\t"<<tabDonnees[i].longueur<<"\t";
					for(int j=0;j<8;j++)
					{   printf("%.2X ",tabDonnees[i].donnee[j]);
					}
					cout<<" nb : "<<nbTrames[i];
					cout<<endl;
				}
			}
			cout<<"=============================================="<<endl<<endl<<endl;
			//cin.get();
		}
		//message du client
		int nbOctets=serveur.RecevoirUnMessage(message,1500,50000);
		if(nbOctets)          //si "0" : envoi de toutes les trames //CODER ENVOITRAMECAN(DONNEE)
		{	message[nbOctets]=0;
			clrscr();
			cout<<"Message du client : "<<message<<endl;
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
					serveur.EnvoyerUnMessage((char*)tabDonneesFormatee[id].c_str(),tabDonneesFormatee[id].length());
				else
					for(int i=0;i<2048;i++)
					{	if(tabDonnees[i].identifiant)
						{	serveur.EnvoyerUnMessage((char*)(tabDonneesFormatee[i]+"\n").c_str(),tabDonneesFormatee[i].length()+1);
						}
					}
			}
			Sleep(2000);
		}
	}
	serveur.FermerLaCommunicationAvecLeClient();
	vscom.DeconnexionVSCOM();
	vscom.FermerCOM();
	cin.get();
	cin.get();
	return 0;
}
