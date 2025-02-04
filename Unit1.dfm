object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Client et Serveur'
  ClientHeight = 208
  ClientWidth = 290
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 96
    Top = 13
    Width = 10
    Height = 13
    Caption = 'IP'
  end
  object Label2: TLabel
    Left = 96
    Top = 40
    Width = 20
    Height = 13
    Caption = 'Port'
  end
  object RadioButton1: TRadioButton
    Left = 8
    Top = 8
    Width = 57
    Height = 17
    Caption = 'Client'
    TabOrder = 0
  end
  object RadioButton2: TRadioButton
    Left = 8
    Top = 31
    Width = 57
    Height = 17
    Caption = 'Serveur'
    Checked = True
    TabOrder = 1
    TabStop = True
  end
  object Edit1: TEdit
    Left = 112
    Top = 10
    Width = 89
    Height = 21
    TabOrder = 2
    Text = '172.20.100.2'
  end
  object Button1: TButton
    Left = 207
    Top = 8
    Width = 75
    Height = 25
    Caption = 'Connexion'
    TabOrder = 3
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 207
    Top = 61
    Width = 75
    Height = 25
    Caption = 'Envoi'
    TabOrder = 4
    OnClick = Button2Click
  end
  object Edit2: TEdit
    Left = 160
    Top = 37
    Width = 41
    Height = 21
    TabOrder = 5
    Text = '17777'
  end
  object Edit3: TEdit
    Left = 8
    Top = 63
    Width = 193
    Height = 21
    TabOrder = 6
    Text = 'votre message...'
  end
  object RichEdit1: TRichEdit
    Left = 8
    Top = 92
    Width = 274
    Height = 109
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    ScrollBars = ssVertical
    TabOrder = 7
    Zoom = 100
  end
  object ClientSocket1: TClientSocket
    Active = False
    ClientType = ctNonBlocking
    Port = 0
    OnConnect = ClientSocket1Connect
    OnRead = ClientSocket1Read
    Left = 72
    Top = 64
  end
  object ServerSocket1: TServerSocket
    Active = False
    Port = 0
    ServerType = stNonBlocking
    OnClientConnect = ServerSocket1ClientConnect
    OnClientDisconnect = ServerSocket1ClientDisconnect
    OnClientRead = ServerSocket1ClientRead
    Left = 144
    Top = 64
  end
end
