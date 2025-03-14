#include <iostream>
#include <stdio.h>
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
typedef char _TCHAR;
#define _tmain main

using namespace std;


VSCOM vscom;
ServeurTCP serveur("0.0.0.0",2085);
ClientTCP client;
int nbTrames[2048];
string tabDonneesFormatee[2048];
DonneeCAN tabDonnees[2048];     //11 bits


void * ThreadServeur(void * pDataBis)
{	char message[1501];
	bool connecte = true;
	while(connecte)
	{	//message du client
		
		int nbOctets=serveur.Recevoir(message,1500);
		
		if(nbOctets)          //si "0" : envoi de toutes les trames //CODER ENVOITRAMECAN(DONNEE)
		{	message[nbOctets]=0;
			
			//cout<<"Message du client recue : "<<message<<endl;

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
				for(int rd=0;rd<8;rd++){donneeAEnvoyer.donnee[rd]=r_donnee[rd];} 
				vscom.EnvoiTrameCAN(donneeAEnvoyer,true);
				
			}
			else
			{   if(id)
				{
					serveur.Envoyer((char*)tabDonneesFormatee[id].c_str(),tabDonneesFormatee[id].length());
				}
				// else if(id==0){
				// 	for(int i=0;i<2048;i++)
				// 	{	
				// 		if(tabDonnees[i].identifiant)
				// 		{	serveur.Envoyer((char*)(tabDonneesFormatee[i]+"\n").c_str(),tabDonneesFormatee[i].length()+1);
							
				// 		}
				// 	}
				// }
				else
				{
					for(int i=0;i<2048;i++)
					{	
						if(tabDonnees[i].identifiant)
						{	serveur.Envoyer((char*)(tabDonneesFormatee[i]+"\n").c_str(),tabDonneesFormatee[i].length()+1);
						}
					}
				}
				//cout<<"Message du client : "<<tabDonneesFormatee<<endl;
			}	
			//Sleep(2000);
		}
		else // Si nbOctets vaut 0, envoyer toutes les trames
        {
            for (int i = 0; i < 2048; i++)
            {
                if (tabDonnees[i].identifiant)
                {
                    serveur.Envoyer((char*)(tabDonneesFormatee[i] + "\n").c_str(), tabDonneesFormatee[i].length() + 1);
                }
            }
        }
	}
}

int main(){
    
    string leCOM="/dev/ttyUSB0";
	string msg;

	for(int i=0;i<2048;i++) {tabDonnees[i].identifiant=0;nbTrames[i]=0;}
	bool OK=true;
	vscom.ModifierCOM(leCOM);
	if(vscom.OuvrirCOM()) cout<<"Ouverture COM OK"<<endl;
	else cout<<"Ouverture COM KO"<<endl;
	char vitesse[10]="S4";
	if(vscom.ConnexionVSCOM(vitesse)) cout<<"Connexion OK"<<endl;
	else {cout<<"Connexion KO"<<endl; OK=false;}
	
	cout<<"Connexion au Serveur......"<<endl;
	if(client.SeConnecterAUnServeur("172.18.110.111",3000)){
		cout<<"connecté : OK"<<endl;
	}else{
		cout<<"Connecté : NOK"<<endl;
	}
	cout<<"En attente du client..."<<endl;
	serveur.AttendreClient();
	
	pthread_t thread_TCP;
	
	pthread_create( & thread_TCP, NULL, ThreadServeur, NULL);

	while(OK)
	{   	
		DonneeCAN d=vscom.ReceptionTrameFormatee();
		if(d.identifiant)
		{
			tabDonnees[d.identifiant].identifiant=d.identifiant;
			for(int j=0;j<8;j++) tabDonnees[d.identifiant].donnee[j]=d.donnee[j];
			tabDonnees[d.identifiant].longueur=d.longueur;
			tabDonnees[d.identifiant].requete=d.requete;
			nbTrames[d.identifiant]++;
			char trameFormatee[1500]="",donnee[1500]="";
			sprintf(donnee,"%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X",d.donnee[0],d.donnee[1],d.donnee[2],d.donnee[3],d.donnee[4],d.donnee[5],d.donnee[6],d.donnee[7]);
			sprintf(trameFormatee,"%.3X [ %d octets ] : %s",d.identifiant,d.longueur,donnee);
			tabDonneesFormatee[d.identifiant]=trameFormatee;
			cout<<endl<<endl<<"=============================================="<<endl;
			//"0B6 [ 8 octets ] : 5600160041547ED0"
			JSONFile json("TestUser","CAN.json");
			for(int i=0;i<2048;i++)
			{	
				if(tabDonnees[i].identifiant)
				{	printf("%.3X ",tabDonnees[i].identifiant);
					cout<<"\t"<<tabDonnees[i].longueur<<"\t";
					
					for(int j=0;j<8;j++)
					{ 
						printf("%.2X ",tabDonnees[i].donnee[j]);
						
					}
					cout<<" nb : "<<nbTrames[i];
					cout<<endl;	
					json.AjouterDonneesJSON(tabDonnees[i].identifiant, tabDonnees[i].longueur, tabDonnees[i].donnee);		
				}
				
				
			}
			json.CloreJSON();
			cout<<"=============================================="<<endl<<endl<<endl;
			string jsonstr = json.getJSON();
			int LengthJSON;
			for(int i = 0;i<jsonstr.length();i++) LengthJSON =i;
			string LengthJSONstr = std::to_string(LengthJSON+1);
			string requete = "POST /api/trame HTTP/1.1\r\nContent-Type: application/json\r\nHost: 172.18.110.111:3000\r\nContent-Length: "+(LengthJSONstr)+"\r\nConnection: keep-alive\r\n\r\n"+jsonstr+"";
			cout<<"Requete HTTP : "<<requete<<endl;
			client.Envoyer(requete);
			// serveur.FermerCommunication();
			// vscom.DeconnexionVSCOM();
			// vscom.FermerCOM();
			//client.SeDeconnecter();	
			
			
			if(!serveur.ClientEstConnecte())
			{
				cout<<"Le client n'est plus connecté."<<endl;
				cout << "FERMETURE SOCKET" << endl;
				serveur.FermerCommunication();
				cout<< "DECONNEXION VSCOM"<<endl;
				vscom.DeconnexionVSCOM();
				cout<< "FERMETURE COM"<<endl;
				vscom.FermerCOM();
				OK = false;
				break;
			}
			
		}
		

	// 	if(serveur.ClientEstConnecte()){
	// 		cout << "FERMETURE SOCKET" << endl;
	// 		serveur.FermerCommunication();
	// 		cout<< "DECONNEXION VSCOM"<<endl;
	// 		vscom.DeconnexionVSCOM();
	// 		cout<< "FERMETURE COM"<<endl;
	// 		vscom.FermerCOM();
	// 		OK = false;
	// 		break;
	// 	}
	// 	else{
	// 		cout << "rien";
	// 		break;
	// 	}
	 }
    return 0;
}


