#include "ExotestThreadServeur.h"

ExotestThreadServeur::ExotestThreadServeur(std::string adresseIPServeur,unsigned short portServeur,std::string leCOM){
    ServeurTCP serveur(adresseIPServeur,portServeur);
}

void ExotestThreadServeur::InitialisationDonneeZero(DonneeCAN tabDonnees[2048],int nbTrames[2048]){
    m_nbTrames = nbTrames;
    m_tabDonnees = tabDonnees;
    
    for(int i=0;i<2048;i++) {
        tabDonnees[i].identifiant=0;
        nbTrames[i]=0;
    }
}

bool ExotestThreadServeur::OuvertureConnexionVSCOM(std::string leCOM,char vitesse[10]){
    m_leCOM = leCOM;
    vscom.ModifierCOM(m_leCOM);
	if(vscom.OuvrirCOM()) cout<<"Ouverture COM OK"<<endl;
	else {
        cout<<"Ouverture COM KO"<<endl;
        OK=false;
        return OK;
    }
	char vitesse[10]="S4";
	if(vscom.ConnexionVSCOM(vitesse)) cout<<"Connexion OK"<<endl;
	else {
        cout<<"Connexion KO"<<endl; 
        OK=false;
        return OK;
    }
    return OK;
}

void ExotestThreadServeur::AffichageReceptionTrame(){
    while(OK)
	{   DonneeCAN d=vscom.ReceptionTrameFormatee();
		if(d.identifiant)
		{	//cout<<d.identifiant<<"["<<d.longueur<<"]"<<endl;
			tabDonnees[d.identifiant].identifiant=d.identifiant;
			for(int j=0;j<8;j++) {
                tabDonnees[d.identifiant].donnee[j]=d.donnee[j];
            }
			
            tabDonnees[d.identifiant].longueur=d.longueur;
			tabDonnees[d.identifiant].requete=d.requete;
			nbTrames[d.identifiant]++;
			char trameFormatee[1500]="",donnee[1500]="";
			sprintf(donnee,"%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X",d.donnee[0],d.donnee[1],d.donnee[2],d.donnee[3],d.donnee[4],d.donnee[5],d.donnee[6],d.donnee[7]);
			sprintf(trameFormatee,"%.3X [ %d octets ] : %s",d.identifiant,d.longueur,donnee);
			tabDonneesFormatee[d.identifiant]=trameFormatee;
			
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
		}
	}
}

void ExotestThreadServeur::DeconnexionFermetureVSCOM(){
    vscom.DeconnexionVSCOM();
    std::cout<<"Deconnexion du VSCOM : OK"<<endl;
	vscom.FermerCOM();
    std::cout<<"Fermeture du VSCOM : OK"<<endl;
}

static void * ExotestThreadServeur::ThreadServeur(void * pDataBis)
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