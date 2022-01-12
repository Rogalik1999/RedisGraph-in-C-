#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cstring>
#include <map>
#include "SocketClient.h"
#include "Error.h"
#include "Connection.h"
#include <stdio.h>
#include <string.h>
#include <stack>
#include <fstream>
#include <iterator>
#include "Entities.h"
#include "StringBuffer.h"

#include "RedisGraphParser.h"

#include "Reply.h"

#include "RedisGraph.h"

#include <typeinfo>
#include <iomanip>
using namespace std;

class Example{

    vector<int> lista;

public:
    struct iterator
    {
        using valueType = int;
        using pointer = int*;
        using reference = int&;
        using vector_it = vector<int>::iterator;

        iterator(vector_it it): it(it) {}
        reference operator*() const {return *it;}
        pointer operator->() { return &(*it);}
        //Prefix increment
        iterator& operator++() {it++; return *this;}
        //Postfix increment
        iterator operator++(int) {iterator tmp = *this; ++(*this); return tmp;}
        friend bool operator==(const iterator& r_it, const iterator& l_it);
        friend bool operator!=(const iterator& r_it, const iterator& l_it);

    private:
        vector_it it;

    };

    Example(): lista{1,2,3,4,5} {}
    ~Example(){}

    int &operator[](int index){
        if(index < 0) return lista[0];
        else if(index > lista.size()) return lista[lista.size()-1];
        return lista[index];
    }

    iterator begin() {return iterator(lista.begin());}
    iterator end() {return iterator(lista.end());}

};

bool operator==(const Example::iterator& r_it, const Example::iterator& l_it) {return r_it.it == l_it.it;}
bool operator!=(const Example::iterator& r_it, const Example::iterator& l_it) {return r_it.it != l_it.it;}

const int socket_read_size = 10000;

int main()
{

    int liczba = 0;

    ifstream file;
    file.open("C:/Users/rafpa/Uczelnia/Praca_dyplomowa/projekt/GitHub/Praca_dyplomowa/przyklad_adm_copy.cypher");

    try{
    ResultSet* po;

    RedisGraph redis_graph("127.0.0.1",6379, socket_read_size);
    redis_graph.connect();

    std::string query = "MATCH (k:kraj)-[:ZAWIERA]->(w:woj)-[z:ZAWIERA]->(p:powiat)-[za:ZAWIERA]->(gm:gmina) WHERE gm.nazwa = '' OR '1'='1'";
    ResultSet result = redis_graph.get_return_response("prg_poland", query,"RETURN k.nazwa, w.nazwa, p, gm");
    cout<< result<<endl;
//    ResultSet result = redis_graph.get_return_response("prg_poland", "MATCH (p:kraj)-[z:ZAWIERA]->(g:woj)","RETURN p,z,g");
//    //result.print();
//
//    cout<<result<<endl;
//
//    Record rec = result[2];
//
//    Node nod = rec.getValue<Node>(0);
//
//    string naz = nod.getValueOfProperty<string>("nazwa");
//
//    cout<<naz<<" "<<&nod<<endl;

//
//    ResultSet result = redis_graph.get_return_response("social", "MERGE (p:robot {actor:'Rami Malek'})","RETURN p");
//    result.print();
//
//    Procedure procedure = redis_graph.call_labels_procedure("social");
//    procedure.print();
//
//    Statistics statistics = redis_graph.execute_query("social", "MERGE (p:robot {actor:'Rami Malek'})");
//    statistics.print();
//
//    Statistics statistics = redis_graph.execute_query("prg_poland", "MERGE (s:kraj {nazwa:'Polska'})");
//    statistics.print();

//    string line;
//
//    string query;
//
//    if(file.is_open()){
//        while(getline(file,line)){
//
//            if(line.back() == ';'){
//                ///zeby usunac srednik z stringa
//                line.pop_back();
//                query += line;
//                //cout<<query<<endl;
//                Statistics statistics = redis_graph.execute_query("prg_poland", query);
//                //statistics.print();
//                stringstream aa;
//                aa<<statistics;
//                string resu = aa.str();
//                cout<<resu;
//                query = "";
//
//            }else{
//                query += line;
//            }
//        }
//        file.close();
//    }
//    else{
//        cout<<"nie mozna otworzyc pliku\n";
//    }

    redis_graph.disconnect();
    file.close();
    }catch(const std::exception& e){
        cout<<e.what()<<endl;
        file.close();
    }

    return 0;

}
