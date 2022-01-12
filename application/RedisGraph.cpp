//---------------------------------------------------------------------------

#pragma hdrstop

#include "RedisGraph.h"

#pragma package(smart_init)

RedisGraph::RedisGraph(char* host, int port,const int read_size_buf): read_size_buf(read_size_buf),write_size_buf(6000),
																		connection(port, host), redis_graph_parser("",0){}

RedisGraph::~RedisGraph(){ disconnect(); }

void RedisGraph::connect(){
	try{
		connection.connect();
    }catch(SocketError& blad){
        throw SocketError(blad);
	}
}

void RedisGraph::disconnect(){

    connection.disconnect();
}

Reply::ResultSet RedisGraph::get_return_response(std::string graph_name, std::string query, std::string return_alias){

	std::string all_query(query);
	all_query+=" ";
    all_query+=return_alias;

	Reply::Procedure labels;
	Reply::Procedure relation_types;
	Reply::Procedure property_keys;

	try{
        labels = call_labels_procedure(graph_name);
        relation_types = call_relation_types_procedure(graph_name);
        property_keys = call_property_keys_procedure(graph_name);
    }catch(SocketError& blad){
        throw SocketError(blad);
    }catch(ResponseError& blad){
        throw ResponseError(blad);
	}

    //Wyslanie
    try{
        send_query(graph_name, all_query);
    }catch(SocketError& blad){
        throw SocketError(blad);
    }

    //Odebranie
	Reply::ResultSet resultset;

    redis_graph_parser.setProceduresPointers(&labels, &relation_types, &property_keys);

    try{
        get_result(&RedisGraphParser::makeResultSet, resultset);
    }catch(SocketError& blad){
        redis_graph_parser.resetProcedurePointers();
        throw SocketError(blad);
    }catch(ResponseError& blad){
		redis_graph_parser.resetProcedurePointers();
        throw ResponseError(blad);
    }

    redis_graph_parser.resetProcedurePointers();
    return resultset;

}

Reply::Statistics RedisGraph::execute_query(std::string graph_name, std::string query){

    //Wyslanie
    try{
        send_query(graph_name, query);
    }catch(SocketError& blad){
        throw SocketError(blad);
    }

    //Odebranie
	Reply::Statistics statistics;

    try{
        get_result(&RedisGraphParser::makeStatistics, statistics);
    }catch(SocketError& blad){
        throw SocketError(blad);
    }catch(ResponseError& blad){
        throw ResponseError(blad);
    }

    return statistics;

}

Reply::Procedure RedisGraph::call_labels_procedure(std::string graph_name){

    std::string query("CALL db.labels()");

    //Wyslanie
    try{
        send_procedure(graph_name, query);
    }catch(SocketError& blad){
        throw SocketError(blad);
	}

    //Odebranie
	Reply::Procedure procedure;

    try{
        get_result(&RedisGraphParser::makeProcedureLabel, procedure);
    }catch(SocketError& blad){
        throw SocketError(blad);
    }catch(ResponseError& blad){
        throw ResponseError(blad);
    }

    return procedure;

}

Reply::Procedure RedisGraph::call_relation_types_procedure(std::string graph_name){

    std::string query("CALL db.relationshipTypes()");

    //Wyslanie
    try{
        send_procedure(graph_name, query);
    }catch(SocketError& blad){
        throw SocketError(blad);
    }

    //Odebranie
	Reply::Procedure procedure;

	try{
        get_result(&RedisGraphParser::makeProcedureRelationType, procedure);
    }catch(SocketError& blad){
        throw SocketError(blad);
    }catch(ResponseError& blad){
        throw ResponseError(blad);
    }

    return procedure;

}

Reply::Procedure RedisGraph::call_property_keys_procedure(std::string graph_name){


    std::string query("CALL db.propertyKeys()");

    //Wyslanie
    try{
        send_procedure(graph_name, query);
    }catch(SocketError& blad){
        throw SocketError(blad);
    }

    //Odebranie
    Reply::Procedure procedure;

	try{
        get_result(&RedisGraphParser::makeProcedurePropertyKey, procedure);
    }catch(SocketError& blad){
        throw SocketError(blad);
    }catch(ResponseError& blad){
        throw ResponseError(blad);
	}

	return procedure;

}

void RedisGraph::send_query(std::string& graph_name, std::string& query){

    std::vector<std::string> args;

    //Dodanie argumentow do listy
    args.push_back("GRAPH.QUERY");
    args.push_back(graph_name);
    args.push_back(query);
    args.push_back("--compact");

    std::vector<std::string> commands;
    commands = pack_command(args);

    //Wyslanie zapytania do bazy
    try{
        for(auto it = commands.begin(); it != commands.end(); it++){
            const char* command = (*it).c_str();
            connection.sendCommand(command);

		}

    }catch(SocketError& blad){
        throw SocketError(blad);
    }

}

void RedisGraph::send_procedure(std::string& graph_name, std::string& procedure){

    std::vector<std::string> args;

    //Dodanie argumentow do listy
    args.push_back("GRAPH.QUERY");
	args.push_back(graph_name);
    args.push_back(procedure);

    std::vector<std::string> commands;
	commands = pack_command(args);

    //Wyslanie zapytania do bazy
    try{

        for(auto it = commands.begin(); it != commands.end(); it++){

            const char* command = (*it).c_str();
			connection.sendCommand(command);

        }

    }catch(SocketError& blad){
        throw SocketError(blad);
    }



}

template<typename T>
void RedisGraph::get_result(int(RedisGraphParser::*fp)(T&), T& result){

    std::unique_ptr<char>server_reply(new char[read_size_buf]);
    int recv_size;

    try{
    //Sprawdzenie czy wystapil Protocol Error
        do{
			recv_size = connection.receiveResponse(server_reply.get(),read_size_buf);
            redis_graph_parser.write(server_reply.get(), recv_size);
            if(redis_graph_parser.checkResponse() == RESULT_OK){
                 break;
            }



        }while(true);

    //Parsowanie odpowiedzi do postaci result
		do{
            if((redis_graph_parser.*fp)(result) == RESULT_OK){
                break;
            }
            recv_size = connection.receiveResponse(server_reply.get(),read_size_buf);
            redis_graph_parser.write(server_reply.get(), recv_size);

        }while(true);

    }catch(SocketError& blad){
        throw SocketError(blad);
	}catch(ResponseError& blad){
        throw ResponseError(blad);
    }
}


std::vector<std::string> RedisGraph::pack_command(const std::vector<std::string>& args){

    int len_arg;
    std::string buff("");
    std::vector<std::string> output;

    buff += "*";
    buff += std::to_string(args.size());
    buff += "\r\n";

    std::stringstream data;
    for(auto arg = args.begin(); arg != args.end(); arg++){

        data << *arg;
        len_arg = (*arg).length();
        buff += "$";
		buff += std::to_string(len_arg);
        buff += "\r\n";

        if(buff.length() >= write_size_buf){
            output.push_back(std::move(buff));
        }

        if(buff.length() + len_arg > write_size_buf){
            char * helper = new char[write_size_buf + 1];
            while(data.tellg() != data.tellp() && data.tellg() > -1 ){

                int unget_chars = data.tellp() - data.tellg();

                if(unget_chars < write_size_buf) {
                    data.read(helper, unget_chars);
                    helper[unget_chars] = '\0';
                }
                else{
                    data.read(helper, write_size_buf);
                    helper[write_size_buf] = '\0';
                }

                buff += helper;

				if(buff.length() >= write_size_buf){
                    output.push_back(std::move(buff));
                }
            }

            buff += "\r\n";
            delete[] helper;

        }else{
            buff += *arg;
            buff += "\r\n";
        }

        data.seekg(0);
        data.seekp(0);

    }
    output.push_back(std::move(buff));
    return std::move(output);

}