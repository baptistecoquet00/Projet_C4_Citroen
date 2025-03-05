#include <iostream>
#include <tchar.h>
#include "ClientTCP.h"
#include <fstream>
#include <sstream>
#include <string.h>
#include <windows.h>
#include <string>

using namespace std;
int main(){

	int choix=3;
	cout<<"Pour choisir un id tapez 1 , pour envoyer une trame tapez 2 et pour avoir la signification tapez 3"<<endl;
	cin>>choix;

	ClientTCP client;
	char identifiant[10];
	char reponse[50];
	char json[5000];

	cin.get();
	switch (choix) {
		case 1:
			client.SeConnecterAUnServeur("172.20.21.9",2025);
			cout<<"Ecrire l'identifiant souhaite :"<<endl;
			cin>>identifiant;

			//Envoie fde l'identifiant
			client.Envoyer(identifiant,strlen(identifiant));

			//Réception de la réponse
			client.Recevoir(reponse,50);
			cout<<reponse<<endl;
			cin.get();
		break;

		case 2:
			client.SeConnecterAUnServeur("172.20.21.9",2025);
			//Envoyer message serveur direct
			while (true)
			{
				char donnees[30];
				cout<<"Ecire la trame a envoyer : (ex 115 [ 4 octets ] : 0800000000000000)"<<endl;
				cin.getline(donnees,1000);
				client.Envoyer(donnees);
				cin.get();
			}

		break;
		case 3:
			cout<<"choisi un identifiant";
			cin>>identifiant;
			client.SeConnecterAUnServeur("172.18.110.111",3000);
			Sleep(2000);
			string requete="GET /api/significations/"+std::string(identifiant)+" HTTP/1.1\r\nHost: 172.18.110.111:3000\r\nConnection: keep-alive\r\n\r\n";
			cout<<requete<<endl;
			client.Envoyer(requete);
			client.Recevoir(json,5000);
			cout<<json;
			cin.get();
			client.SeDeconnecter();
			ofstream fichier("data.json");
			if (!fichier) {
			cout << "Erreur: impossible d'ouvrir le fichier" << endl;
			system("pause");
			return 1;
			}
			fichier<<json;
			fichier.close();

		break;
	}
	cin.get();
	return 0;
}






	///////////////////////////////////////////Choisir un identifiant  et communiquer avec le serv/////////////////////////////////
//  client.SeConnecterAUnServeur("172.20.21.9",2025);
//	cout<<"Ecrire l'identifiant souhaite :"<<endl;
//	cin>>identifiant;
//
//	//Envoie fde l'identifiant
//	client.Envoyer(identifiant,strlen(identifiant));
//
//	//Réception de la réponse
//	client.Recevoir(reponse,50);
//	cout<<reponse<<endl;
//
//	//Envoyer message serveur direct
//	while (true)
//	{
//		char donnees[30];
//		cout<<"Ecire la trame a envoyer : (ex 115 [ 4 octets ] : 0800000000000000)"<<endl;
//		cin.getline(donnees,1000);
//		client.Envoyer(donnees);
//		cin.get();
//	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////Ouverture JSON////////////////////////////////////////////////////////////////
//	ifstream fichier;
//	fichier.open("data.txt");
//	if (!fichier) {
//		cout << "Erreur: impossible d'ouvrir le fichier" << endl;
//		system("pause");
//		return 1;
//	}
//
//	// Lit et affiche le contenu du fichier
//	string ligne;
//	cout << "Contenu du fichier JSON:" << endl;
//	while (getline(fichier, ligne)) {
//		cout << ligne << endl;
//	}
//	fichier.close();
//	system("pause");
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////Requet GET + insertion json//////////////////////////////////////////////////////////////////////	cout<<"choisi un identifiant";
//	cin>>identifiant;
//	client.SeConnecterAUnServeur("172.18.110.111",3000);
//	Sleep(2000);
//	string requete="GET /api/significations/"+std::string(identifiant)+" HTTP/1.1\r\nHost: 172.18.110.111:3000\r\nConnection: keep-alive\r\n\r\n";
//	cout<<requete<<endl;
//	client.Envoyer(requete);
//	client.Recevoir(json,5000);
//	cout<<json;
//	cin.get();
//	client.SeDeconnecter();
//	ofstream fichier("data.json");
//	if (!fichier) {
//		cout << "Erreur: impossible d'ouvrir le fichier" << endl;
//		system("pause");
//		return 1;
//	}
//	fichier<<json;
//	fichier.close();
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





