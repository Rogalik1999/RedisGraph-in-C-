#ifndef REDISGRAPH_H
#define REDISGRAPH_H

#include <string>
#include <memory>
#include <sstream>
#include <vector>

#include "Connection.h"
#include "RedisGraphParser.h"
#include "Reply.h"

class RedisGraph
{
    ///Rozmiar bufera przy odbieraniu danych z serwera
    const int read_size_buf;
    ///Rozmiar bufera przy wysylaniu danych do serwera
    const int write_size_buf;

    Connection connection;
    RedisGraphParser redis_graph_parser;

public:
    RedisGraph(char* host, int port,const int read_size_buf);
    ~RedisGraph();

    void connect();
    void disconnect();
    ///Wykonanie zapytania ktore zawiera klauzule RETURN
    ResultSet get_return_response(std::string graph_name, std::string query, std::string return_alias);
    ///Wykonanie zapytania ktore nie ma klauzuli RETURN
    Statistics execute_query(std::string graph_name, std::string query);
    ///Uzyskanie listy z nazwami labelow
    Procedure call_labels_procedure(std::string graph_name);
    ///Uzyskanie listy z nazwami relacji miedzy wierzcholkami
    Procedure call_relation_types_procedure(std::string graph_name);
    ///Uzyskanie listy z nazwami kluczy atrybutow
    Procedure call_property_keys_procedure(std::string graph_name);

private:
    void send_query(std::string& graph_name, std::string& query);
    void send_procedure(std::string& graph_name, std::string& procedure);
    template<typename T>
    void get_result(int(RedisGraphParser::*fp)(T&), T& result);

    ///Funkcja ktora opakowuje komende Redisa w RESP oraz dzieli je na paczki wg rozmiaru bufera
    std::vector<std::string> pack_command(const std::vector<std::string>& args);
};

#endif // REDISGRAPH_H
