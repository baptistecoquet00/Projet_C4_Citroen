#include <iostream>
#include <stdio.h>
#include "VSCOM.h"
#include "ServeurTCP.h"


using namespace std;

int main(){
    VSCOM com;
    char trameTCP[50];
    char ReponseTCP[20] ="I00000115R0L4";

    com.ModifierCOM("/dev/ttyUSB0");
    if(com.OuvrirCOM()) cout<<"COM OUVERT"<<endl; else cout<<"ERREUR DE COM"<<endl;
    if(com.ConnexionVSCOM()) cout<<"CONNEXION AU VSCOM"<<endl; else cout<<"ERREUR DE CONNEXION AU VSCOM"<<endl;    ;
    com.EnvoiTrameCAN("I00000115R0L4"); 
    com.DeconnexionVSCOM(); 
    //com.FermerCOM();
    ServeurTCP serveur("0.0.0.0",8080);
    cout<<"Le serveur est en écoute...."<<endl;
    while (true)
    {
        serveur.Envoyer(ReponseTCP,strlen(ReponseTCP));
        if(serveur.Recevoir(trameTCP,50)==-1){
            //cout<<"SOCKET : ERREUR"<<endl;
        }
        cout<<trameTCP<<endl;
        serveur.AttendreClient();
        serveur.Envoyer(ReponseTCP,strlen(ReponseTCP));
        serveur.FermerCommunication();
    }
    

    return 0;
}
