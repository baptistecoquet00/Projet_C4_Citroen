#include "DonneesCAN.h"

DonneesCANClass::DonneesCANClass(std::string nomUtilisateur,std::string nomFichierJSON,std::string addrServeur,int portServeur)
{	nomUtilisateur_pv = nomUtilisateur;
	nomFichierJSON_pv = nomFichierJSON;
	addrServeur_pv = addrServeur;
	portServeur_pv = portServeur;
	JSONFile json(nomUtilisateur_pv,nomFichierJSON_pv);
	ConnexionServeurRest(addrServeur, portServeur);
}

const std::string DonneesCANClass::currentDateTime() 
{   time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = * localtime( & now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", & tstruct);
    return buf;
}

void DonneesCANClass::SauvegarderDonneesCANClass(std::string nomFichierLog)
{	nomFichierLog_pv = nomFichierLog;
	ofstream fichierDonneeCAN;
	fichierDonneeCAN.open(nomFichier);
	fichierDonneeCAN<<currentDateTime()<<"\n";
	for(int i=0;i<2048;i++)
	{	if(tabDonnees[i].identifiant)
		{	fichierDonneeCAN<<tabDonneesFormatee[i]<<"\t"<<nbTrames[i]<<"\n";	
		}
	}
	fichierDonneeCAN.close();
}

void DonneesCANClass::ChargerDonneesCAN(std::string nomFichierLog)
{	nomFichierLog_pv = nomFichierLog;
	ifstream fichierDonneeCAN;
	fichierDonneeCAN.open(nomFichier);
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

void DonneesCANClass::affichageDonneeCAN()
{	cout<<endl<<endl<<"=============================================="<<endl;
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
}

bool DonneesCANClass::ConnexionServeurRest(std::string addrServeur,int portServeur)
{	if(client.SeConnecterAUnServeur(addrServeur,portServeur))
	{
		cout<<"Connexion REST : OK"<<endl;
		return true;
	}
	else
	{
		cout<<"Connexion REST : NOK"<<endl;
		return false;
	}
}

void DonneesCANClass::EnvoyerDonneesAuServeurREST()
{   std::string addrEtPortServeur;
    addrEtPortServeur = addrServeur_pv;
    addrEtPortServeur +=":";
    addrEtPortServeur +=std::to_string(portServeur_pv);
	string jsonstr = json.getJSON();
	int LengthJSON;
	for(int i = 0;i<jsonstr.length();i++) LengthJSON =i;
	string LengthJSONstr = std::to_string(LengthJSON+1);
	string requete = "POST /api/trame HTTP/1.1\r\nContent-Type: application/json\r\nHost: "+addrEtPortServeur+"\r\nContent-Length: "+(LengthJSONstr)+"\r\nConnection: keep-alive\r\n\r\n"+jsonstr+"";
	cout<<"Requete HTTP : "<<requete<<endl;
	client.Envoyer(requete);
}