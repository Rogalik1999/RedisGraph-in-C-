//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner), redisgraph(nullptr)
{
}

//---------------------------------------------------------------------------
void __fastcall TForm1::ZaladujDaneClick(TObject *Sender)
{
	if(this->redisgraph == nullptr){

		this->ListBox1->Items->Clear();
		this->ListBox1->Items->Add("Nalezy polaczyc sie z baza");
	}
	else{
		this->OpenDialog1->Execute();
		std::ifstream file;
		try{
			if(this->OpenDialog1->FileName == "") throw Exception("Nie wybrano pliku");
			AnsiString path(this->OpenDialog1->FileName);
			file.open(path.c_str());

			std::string line;
			std::string query;
			if(file.is_open()){
				while(getline(file,line)){

					if(line.back() == ';'){
						///zeby usunac srednik
					   line.pop_back();
					   query += line;
					   Reply::Statistics statistics= this->redisgraph->execute_query("prg_poland", query);
					   query = "";
					   std::stringstream stats;
					   stats << statistics;
					   std::string _line;
					   while(getline(stats,_line)){

							this->ListBox1->Items->Add(_line.c_str());

					   }

					   this->ListBox1->Items->Add("");


					}else{
						query += line;
					}
				}

			}else{
				throw Exception("Nie mozna otworzyc pliku");
			}

		}catch(Error& blad){
			this->ListBox1->Items->Add(blad.what());

			this->redisgraph->disconnect();
			delete this->redisgraph;
			this->redisgraph = nullptr;
		}catch(const std::exception& e){

		   this->ListBox1->Items->Add(e.what());
		}
		this->OpenDialog1->FileName = "";
		file.close();
	}

}


void __fastcall TForm1::PolaczClick(TObject *Sender)
{
	if(this->redisgraph != nullptr){

	this->ListBox1->Items->Clear();
		this->ListBox1->Items->Add("Juz jestes polaczony z baza");
	}
	else{
		AnsiString ip(this->ip_address->Text);
		AnsiString port(this->Port->Text);
		try{
             this->redisgraph = new RedisGraph(ip.c_str(), atoi(port.c_str()), 10000);
		this->redisgraph->connect();


this->ListBox1->Items->Clear();
this->ListBox1->Items->Add("Polaczono sie z baza");
		}catch(const std::exception& e){

		this->ListBox1->Items->Clear();
		this->ListBox1->Items->Add("Nie udalo sie polaczyc z baza");
			this->redisgraph->disconnect();
			delete this->redisgraph;
			this->redisgraph = nullptr;
        }

	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::OdlaczClick(TObject *Sender)
{
if(this->redisgraph == nullptr){

		this->ListBox1->Items->Clear();
		this->ListBox1->Items->Add("Juz odlaczono sie od bazy");
	}
	else{
			this->redisgraph->disconnect();
			delete this->redisgraph;
			this->redisgraph = nullptr;
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::WojButtonClick(TObject *Sender)
{
	if(this->redisgraph == nullptr){

		this->ListBox1->Items->Clear();
		this->ListBox1->Items->Add("Nie jestes podlaczony do bazy");
	}
	else{
		try{

			Reply::ResultSet result = this->redisgraph->get_return_response("prg_poland","MATCH (w:woj)","RETURN w.nazwa");
			this->ComboBox1->Items->Clear();
			for(auto it = result.begin(); it != result.end(); it++){

				std::string nazwa_woj = (*it).getValue<std::string>(0);

				this->ComboBox1->Items->Add(nazwa_woj.c_str());

			}



		}catch(Error& blad){
			this->ListBox1->Items->Add(blad.what());

			this->redisgraph->disconnect();
			delete this->redisgraph;
			this->redisgraph = nullptr;
		}catch(BadCast& bad){

		   this->ListBox1->Items->Add(bad.what());
		}catch(OutOfRangeProperties& out){

		   this->ListBox1->Items->Add(out.what());
		}catch(const std::exception& e){

		   this->ListBox1->Items->Add(e.what());
		}


	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::PowButtonClick(TObject *Sender)
{
	if(this->redisgraph == nullptr){

		this->ListBox1->Items->Clear();
		this->ListBox1->Items->Add("Nie jestes podlaczony do bazy");
	}
	else{
		try{


			int i = this->ComboBox1->ItemIndex;
			AnsiString nazwa(this->ComboBox1->Text);
			std::string query = "MATCH (w:woj)-[z:ZAWIERA]->(p:powiat) WHERE w.nazwa = ";
			query.append("'");
			query.append(nazwa.c_str());
			query.append("'");

			//this->ListBox1->Items->Add(query.c_str());
			Reply::ResultSet result = this->redisgraph->get_return_response("prg_poland",query.c_str(),"RETURN p.nazwa");
			this->ListBox1->Items->Clear();
			for(auto it = result.begin(); it != result.end(); it++){

				std::string nazwa_pow = (*it).getValue<std::string>(0);
				this->ListBox1->Items->Add(nazwa_pow.c_str());
			}



		}catch(Error& blad){
			this->ListBox1->Items->Add(blad.what());

			this->redisgraph->disconnect();
			delete this->redisgraph;
			this->redisgraph = nullptr;
		}catch(BadCast& bad){

		   this->ListBox1->Items->Add(bad.what());
		}catch(OutOfRangeProperties& out){

		   this->ListBox1->Items->Add(out.what());
		}catch(const std::exception& e){

		   this->ListBox1->Items->Add(e.what());
		}


	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::GmButtonClick(TObject *Sender)
{
	if(this->redisgraph == nullptr){

	this->ListBox1->Items->Clear();
	this->ListBox1->Items->Add("Nie jestes podlaczony do bazy");
	}
	else{
		try{


			int i = this->ComboBox1->ItemIndex;
			AnsiString nazwa(this->ComboBox1->Text);
			std::string query = "MATCH (w:woj)-[z:ZAWIERA]->(p:powiat)-[za:ZAWIERA]->(gm:gmina) WHERE w.nazwa = ";
			query.append("'");
			query.append(nazwa.c_str());
			query.append("'");

			//this->ListBox1->Items->Add(query.c_str());
			Reply::ResultSet result = this->redisgraph->get_return_response("prg_poland",query.c_str(),"RETURN gm.nazwa");
			this->ListBox1->Items->Clear();
			for(auto it = result.begin(); it != result.end(); it++){

				std::string nazwa_pow = (*it).getValue<std::string>(0);
				this->ListBox1->Items->Add(nazwa_pow.c_str());
			}



		}catch(Error& blad){
			this->ListBox1->Items->Add(blad.what());

			this->redisgraph->disconnect();
			delete this->redisgraph;
			this->redisgraph = nullptr;
		}catch(BadCast& bad){

		   this->ListBox1->Items->Add(bad.what());
		}catch(OutOfRangeProperties& out){

		   this->ListBox1->Items->Add(out.what());
		}catch(const std::exception& e){

		   this->ListBox1->Items->Add(e.what());
		}


	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::GmInfoBtnClick(TObject *Sender)
{
	if(this->redisgraph == nullptr){

	this->ListBox1->Items->Clear();
	this->ListBox1->Items->Add("Nie jestes podlaczony do bazy");
	}
	else{
		try{

			AnsiString nazwa_gm(this->GmInfoEditText->Text);
			std::string query =  "MATCH (k:kraj)-[:ZAWIERA]->(w:woj)-[z:ZAWIERA]->(p:powiat)-[za:ZAWIERA]->(gm:gmina) WHERE gm.nazwa = ";
			query.append("'");
			query.append(nazwa_gm.c_str());
			query.append("'");

			Reply::ResultSet result = this->redisgraph->get_return_response("prg_poland",query.c_str(),"RETURN k.nazwa, w.nazwa, p, gm");
			this->ListBox1->Items->Clear();

			if(result.size_of_records == 0){
				this->ListBox1->Items->Add("Nie znalazlo sie danej gminy w bazie");
			}else{
				for(auto it = result.begin(); it != result.end(); it++){

					std::string nazwa_kraju = (*it).getValue<std::string>(0);
					std::string nazwa_woj = (*it).getValue<std::string>(1);

					Node powiat = (*it).getValue<Node>(2);
					Node gmina = (*it).getValue<Node>(3);

					std::string info = "Gmina ";

					info.append(nazwa_gm.c_str());
					info.append(" znajduje w kraju ");
					info.append(nazwa_kraju);
					info.append(" w wojewodztwie o nazwie ");
					info.append(nazwa_woj);

					this->ListBox1->Items->Add(info.c_str());

					std::string pow_nazwa = powiat.getValueOfProperty<std::string>("nazwa");
					int pow_ludnosc = powiat.getValueOfProperty<int>("ludnosc");

					info = "";
					info.append("Jej powiat to ");
					info.append(pow_nazwa);
					info.append(" ktory ma ");
					info.append(std::to_string(pow_ludnosc));
					info.append(" ludnosci");

					this->ListBox1->Items->Add(info.c_str());

					std::string gm_kod = gmina.getValueOfProperty<std::string>("kod");
					std::string gm_typ = gmina.getValueOfProperty<std::string>("typ");

					info = "";
					info.append("Gmina ma kod ");
					info.append(gm_kod);
					info.append(" i jest typem ");
					info.append(gm_typ);

					this->ListBox1->Items->Add(info.c_str());

                    this->ListBox1->Items->Add("");

				}
			}

		}catch(Error& blad){
			this->ListBox1->Items->Add(blad.what());

			this->redisgraph->disconnect();
			delete this->redisgraph;
			this->redisgraph = nullptr;
		}catch(BadCast& bad){

		   this->ListBox1->Items->Add(bad.what());
		}catch(OutOfRangeProperties& out){

		   this->ListBox1->Items->Add(out.what());
		}catch(const std::exception& e){

		   this->ListBox1->Items->Add(e.what());
		}


	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::WojLudnBtnClick(TObject *Sender)
{
	if(this->redisgraph == nullptr){

		this->ListBox1->Items->Clear();
		this->ListBox1->Items->Add("Nie jestes podlaczony do bazy");
	}
	else{
		try{

			AnsiString nazwa(this->ComboBox1->Text);
			std::string query = "MATCH (w:woj)-[z:ZAWIERA]->(p:powiat) WHERE w.nazwa = ";
			query.append("'");
			query.append(nazwa.c_str());
			query.append("'");

			Reply::ResultSet result = this->redisgraph->get_return_response("prg_poland",query.c_str(),"RETURN sum(p.ludnosc)");

            this->ListBox1->Items->Clear();
			for(auto it = result.begin(); it != result.end(); it++){

				double ludnosc_woj = (*it).getValue<double>(0);      // Przy sum to zwraca sie wtedy double a nie int

				std::string info = "";
				info.append("Wojewodztwo ");
				info.append(nazwa.c_str());
				info.append(" zawiera ");
				info.append(std::to_string((int)ludnosc_woj));
				info.append(" ludnosci");

				this->ListBox1->Items->Add(info.c_str());
			}


		}catch(Error& blad){
			this->ListBox1->Items->Add(blad.what());

			this->redisgraph->disconnect();
			delete this->redisgraph;
			this->redisgraph = nullptr;
		}catch(BadCast& bad){

		   this->ListBox1->Items->Add(bad.what());
		}catch(OutOfRangeProperties& out){

		   this->ListBox1->Items->Add(out.what());
		}catch(const std::exception& e){

		   this->ListBox1->Items->Add(e.what());
		}


	}
}
//---------------------------------------------------------------------------

