#ifndef IRSERVEURTCPMONOCLIENT_H
#define IRSERVEURTCPMONOCLIENT_H

class IRServeurTcpMonoClient
{
private:
	int socketEcoute;
	unsigned short portEcoute;
	int socketDeCommunication; // la socket permettant de communiquer avec le 'mono' client connecté

public:
	IRServeurTcpMonoClient();
	~IRServeurTcpMonoClient();
	void Initialisation();
	bool MettreEnEcouteSurLePort(unsigned short portEcoute);
	bool AttendreUnNouveauClient();
	int EnvoyerUnMessage(char message[], int longueurDuMessage);
	int RecevoirUnMessage(char message[], int longueurMaxDuMessage);
	int RecevoirUnMessage(char message[], int longueurMaxDuMessage,int timeout_us);
	void FermerLaCommunicationAvecLeClient();
	void ArreterLeServeur();


};

#endif // IRSERVEURTCPMONOCLIENT_H
