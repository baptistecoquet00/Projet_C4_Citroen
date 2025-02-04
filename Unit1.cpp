//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
TCustomWinSocket *MonClientSocket;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
	if(RadioButton1->Checked)
	{
		ClientSocket1->Address=Edit1->Text;
		ClientSocket1->Port=Edit2->Text.ToInt();
		ClientSocket1->Active=true;
	}
	else
	{
		ServerSocket1->Port=Edit2->Text.ToInt();
		ServerSocket1->Active=true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{   	if(RadioButton1->Checked)
	{
		ClientSocket1->Active=false;
	}
	else
	{
		ServerSocket1->Active=false;
	}

}
//---------------------------------------------------------------------------
void __fastcall TForm1::ClientSocket1Connect(TObject *Sender, TCustomWinSocket *Socket)

{
	RichEdit1->Lines->Add("Connecté au serveur");
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ServerSocket1ClientConnect(TObject *Sender, TCustomWinSocket *Socket)

{   MonClientSocket=Socket;
	RichEdit1->Lines->Add("Client connecté");
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ServerSocket1ClientDisconnect(TObject *Sender, TCustomWinSocket *Socket)

{
	RichEdit1->Lines->Add("Client déconnecté");
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ServerSocket1ClientRead(TObject *Sender, TCustomWinSocket *Socket)

{
	RichEdit1->Lines->Add(Socket->ReceiveText());
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ClientSocket1Read(TObject *Sender, TCustomWinSocket *Socket)

{
	RichEdit1->Lines->Add(Socket->ReceiveText());
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)
{   if(RadioButton1->Checked)
	{
		ClientSocket1->Socket->SendText(Edit3->Text);
	}
	else
	{
		MonClientSocket->SendText(Edit3->Text);
	}
}
//---------------------------------------------------------------------------
