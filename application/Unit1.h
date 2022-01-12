//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.FileCtrl.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Outline.hpp>
#include <Vcl.Samples.DirOutln.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtDlgs.hpp>
#include <Vcl.WinXCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Tabs.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.ExtCtrls.hpp>

#include <sstream>
#include <fstream>
#include <string>
#include <cstdlib>

#include "Entities.h"
#include "Error.h"
#include "RedisGraph.h"

class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TListBox *ListBox1;
	TOpenDialog *OpenDialog1;
	TButton *ZaladujDane;
	TEdit *ip_address;
	TLabel *Label2;
	TEdit *Port;
	TLabel *Label3;
	TLabel *Label1;
	TButton *Polacz;
	TButton *Odlacz;
	TComboBox *ComboBox1;
	TLabel *Label4;
	TButton *WojButton;
	TLabel *Label5;
	TLabel *Label6;
	TButton *PowButton;
	TLabel *Label7;
	TButton *GmButton;
	TEdit *GmInfoEditText;
	TButton *GmInfoBtn;
	TLabel *Label8;
	TLabel *Label9;
	TButton *WojLudnBtn;
	TGroupBox *GroupBox1;
	void __fastcall ZaladujDaneClick(TObject *Sender);
	void __fastcall PolaczClick(TObject *Sender);
	void __fastcall OdlaczClick(TObject *Sender);
	void __fastcall WojButtonClick(TObject *Sender);
	void __fastcall PowButtonClick(TObject *Sender);
	void __fastcall GmButtonClick(TObject *Sender);
	void __fastcall GmInfoBtnClick(TObject *Sender);
	void __fastcall WojLudnBtnClick(TObject *Sender);

private:
	RedisGraph* redisgraph;

public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif










