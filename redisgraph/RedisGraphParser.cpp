#include "RedisGraphParser.h"

RedisGraphParser::RedisGraphParser(const char* data, int len): process_number_of_elements(0),process_number_of_records(0),process_number_of_header_row(0),
                                                                process_number_of_attributes(0),process_level(0),process_elements_of_array(0),
                                                                process_type_of_element(0),attribute_id(-1),_type(-1),_pointer(nullptr),
                                                                _vector_attribute_pointer(nullptr),labels(nullptr),relation_types(nullptr),
                                                                properties_keys(nullptr),char_to_omit(0), stringBuffer(data,len){}



RedisGraphParser::~RedisGraphParser()
{
    std::cout<<"niszczy sie\n";
}

void RedisGraphParser::write(const char* data, int len){ stringBuffer.write(data, len); }

void RedisGraphParser::setProceduresPointers(Procedure* labels, Procedure* relation_types, Procedure* properties_keys){

    this->labels = labels;
    this->relation_types = relation_types;
    this->properties_keys = properties_keys;
}

void RedisGraphParser::resetProcedurePointers(){

    this->labels = nullptr;
    this->relation_types = nullptr;
    this->properties_keys = nullptr;

}


int RedisGraphParser::makeResultSet(ResultSet& result_set){

    std::string data;
    process_level++;
    ///Zainicjalizowanie procesu
    if(process_stack.size() == process_level-1) process_stack.push_back(1);
    ///Czytanie header row
    if(process_stack[process_level-1] == 1){
        if(takeRecordsofString(4,8,result_set.header_row,result_set.size_of_header_row)==RESULT_UNFINISHED) return RESULT_UNFINISHED;
        process_stack[process_level-1] = 2;
    }
    ///Result Rows///
    ///Pobranie ilosci rekordow i inicjalizacja kolumn
    if(process_stack[process_level-1] == 2){
        if(getNumber(0,data) != RESULT_OK) return RESULT_UNFINISHED;
        ///Sprawdzenie ilosci rekordow
        ///Jesli jest rowna zero to mozna przejsc etap Result Rows do juz etapu pobrania statystyk
        if(atoi(data.c_str()) == 0) process_stack[process_level-1] = 4;
        else{
            result_set.size_of_records = atoi(data.c_str());
            process_number_of_records = result_set.size_of_records;
            for(int i = 0; i < result_set.size_of_header_row; i++){
                result_set.addColumn();
            }
            process_stack[process_level-1] = 3;
        }

    }
    ///Pobieranie rekordow
    if(process_stack[process_level-1] == 3){
         ///rekordy
        while(process_number_of_records != 0){
            if(takeRecordsofHeaderRow(result_set,false)==RESULT_UNFINISHED) return RESULT_UNFINISHED;
            process_number_of_records--;
        }
        process_stack[process_level-1] = 4;
    }
    ///Pobranie statystyk
    if(process_stack[process_level-1] == 4){
        if(takeRecordsofString(0,0,result_set.statistics.elements, result_set.statistics.size_of_statistic)==RESULT_UNFINISHED) return RESULT_UNFINISHED;
        process_stack.pop_back();
    }
    process_level--;
    return RESULT_OK;
}

int RedisGraphParser::makeProcedureRelationType(Procedure& procedure){

    if(makeProcedureCall(31,procedure) != RESULT_OK ) return RESULT_UNFINISHED;
    return RESULT_OK;
}

int RedisGraphParser::makeProcedureLabel(Procedure& procedure){

    if(makeProcedureCall(19,procedure) != RESULT_OK ) return RESULT_UNFINISHED;
    return RESULT_OK;
}

int RedisGraphParser::makeProcedurePropertyKey(Procedure& procedure){

    if(makeProcedureCall(26,procedure) != RESULT_OK ) return RESULT_UNFINISHED;
    return RESULT_OK;
}

int RedisGraphParser::makeStatistics(Statistics& statistics){

    std::string data;
    process_level++;
    ///Zainicjalizowanie procesu
    if(process_stack.size() == process_level-1)
    {
        process_stack.push_back(1);
    }
    ///pobranie statystyk
    if(process_stack[process_level-1] == 1){
        if(takeRecordsofString(4,0,statistics.elements, statistics.size_of_statistic)==RESULT_UNFINISHED) return RESULT_UNFINISHED;
        process_stack.pop_back();
    }
    process_level--;
    return RESULT_OK;
}


int RedisGraphParser::checkResponse(){

    std::string data;
    process_level++;
    ///Zainicjalizowanie procesu
    if(process_stack.size() == process_level-1){
        process_stack.push_back(1);
        char_to_omit = 1;
    }
    ///Czytanie pierwszego znaku
   if(process_stack[process_level-1] == 1){

        if(stringBuffer.checkDataByNum(char_to_omit) != STRING_BUFFER_READ){
            process_level = 0;
            return RESULT_UNFINISHED;
        }
        char_to_omit = 0;
        data = stringBuffer.getData();
        if(data[0] == '-')
            process_stack[process_level-1] = 2;
    }
    ///Pobranie calego bledu protokolu (wystapil znak -)
    if(process_stack[process_level-1] == 2){

        if(stringBuffer.readDataByChr("\r\n",false) != STRING_BUFFER_READ)
        {
            process_level = 0;
            return RESULT_UNFINISHED;
        }
        data = stringBuffer.getData();
        process_stack.pop_back();
        process_level--;
        throw ResponseError(data);
    }

    process_level--;
    return RESULT_OK;

}

/// private method
int RedisGraphParser::makeProcedureCall(int omit,Procedure& procedure){

    std::string data;
    process_level++;
    ///Zainicjalizowanie procesu
    if(process_stack.size() == process_level-1)
    {
        process_stack.push_back(1);
    }
    ///Pobranie rekordow
    if(process_stack[process_level-1] == 1){
        if(takeRecordsofString(omit,4,procedure.elements,procedure.size_of_elements)==RESULT_UNFINISHED) return RESULT_UNFINISHED;
        process_stack[process_level-1] = 2;
    }
    ///pobranie statystyk
    if(process_stack[process_level-1] == 2){
        if(takeRecordsofString(0,0,procedure.statistics.elements, procedure.statistics.size_of_statistic)==RESULT_UNFINISHED) return RESULT_UNFINISHED;
        process_stack.pop_back();
    }
    process_level--;
    return RESULT_OK;

}

int RedisGraphParser::takeRecordsofHeaderRow(ResultSet& result_set, bool ifInitialized){

    std::string data;
    process_level++;
    ///Zainicjalizowanie procesu
    if(process_stack.size() == process_level-1) {
        process_number_of_header_row = result_set.size_of_header_row;
        process_stack.push_back(1);
    }
    ///Ominiecie
    if(process_stack[process_level-1] == 1){
        if(getNumber(0,data) != RESULT_OK) return RESULT_UNFINISHED;
        process_stack[process_level-1] = 2;
    }

    ///Pobranie wartosci, dodanie do listy i gdy ifInitializad jest true to utworzenie listy na stercie
    if(process_stack[process_level-1] == 2){

        while(process_number_of_header_row != 0){
            if(takeValueType(result_set,ifInitialized) == RESULT_UNFINISHED) return RESULT_UNFINISHED;
            process_number_of_header_row--;
        }
        process_stack.pop_back();
    }
    process_level--;
    return RESULT_OK;
}

int RedisGraphParser::takeValueType(ResultSet& result_set, bool ifInitialized){

    std::string data;
    process_level++;
    ///Zainicjalizowanie procesu
    if(process_stack.size() == process_level-1) {
        process_stack.push_back(1);
    }

    ///Pobranie typu i inicjalizacja na stercie
    if(process_stack[process_level-1] == 1){

        if(getNumber(4,data) != RESULT_OK) return RESULT_UNFINISHED;
        int type = atoi(data.c_str());
        _type = type;
        process_stack[process_level-1] = 2;
        _pointer = result_set.addValue(result_set.size_of_header_row - process_number_of_header_row, type);

    }

    ///Pobranie obiektu
    if(process_stack[process_level-1] == 2){
        int type = _type;
        switch(type){
        //Null
        case 1:
        {   // Nie trzeba zmieniaæ wartoœci wskaŸnika gdy¿ domyœlnie jest ju¿ nullptr
            if(getNumber(0,data) == RESULT_UNFINISHED) return RESULT_UNFINISHED;
            break;
        }
        //String
        case 2:
        {
            if(makeString(0,data) == RESULT_UNFINISHED) return RESULT_UNFINISHED;
            std::string* str = static_cast<std::string*>(_pointer);
            *str = data;
            break;
        }
        //Integer
        case 3:
        {
            if(getNumber(0,data) == RESULT_UNFINISHED) return RESULT_UNFINISHED;
            int* p_int = static_cast<int*>(_pointer);
            *p_int = atoi(data.c_str());
            break;

        }
        //Boolean:
        case 4:
        {
            if(makeString(0,data) == RESULT_UNFINISHED) return RESULT_UNFINISHED;
            bool* p_bool = static_cast<bool*>(_pointer);
            if(data == "true") *p_bool = true;
            else *p_bool =  false;
            break;

        }
        //Double
        case 5:
        {
            if(makeString(0,data) == RESULT_UNFINISHED) return RESULT_UNFINISHED;
            double* p_double = static_cast<double*>(_pointer);
            *p_double = std::stod(data);
            break;
        }
        //Array
        case 6:
        {
            std::vector<RedisGraphContainer::RedisValue>* v_pointer = static_cast<std::vector<RedisGraphContainer::RedisValue>*>(_pointer);
            if(getArray(v_pointer) == RESULT_UNFINISHED) return RESULT_UNFINISHED;
            break;
        }
        //Edge
        case 7:
        {
            Edge* edge = static_cast<Edge*>(_pointer);
            if(makeEdge(edge) == RESULT_UNFINISHED) return RESULT_UNFINISHED;
            break;
        }
         //Node
        case 8:
        {
            Node* node = static_cast<Node*>(_pointer);
            if(makeNode(node) == RESULT_UNFINISHED) return RESULT_UNFINISHED;
            break;
        }
        }
        process_stack.pop_back();
        _type = -1;
        _pointer = nullptr;
    }
    process_level--;
    return RESULT_OK;

}

int RedisGraphParser::makeNode(Node* node){

    std::string data;
    process_level++;
    ///Zainicjalizowanie procesu
    if(process_stack.size() == process_level-1) process_stack.push_back(1);

    ///Pobranie id wierzcholka
    if(process_stack[process_level-1] == 1){

        if(getNumber(4,data) != RESULT_OK) return RESULT_UNFINISHED;
        node->setId(atoi(data.c_str()));
        process_stack[process_level-1] = 2;
    }
    ///Pobranie label
    if(process_stack[process_level-1] == 2){
        if(getNumber(4,data) != RESULT_OK) return RESULT_UNFINISHED;
        //Pobranie z procedury labels labela na podstawie id
        node->setLabel(labels->elements[atoi(data.c_str())]);
        process_stack[process_level-1] = 3;
    }
    ///Pobranie rozmiaru atrybutow
    if(process_stack[process_level-1] == 3){
        if(getNumber(0,data) != RESULT_OK) return RESULT_UNFINISHED;
        process_number_of_attributes = atoi(data.c_str());

        process_stack[process_level-1] = 4;
    }
    ///Pobranie atrybutow
    if(process_stack[process_level-1] == 4){

        while(process_number_of_attributes != 0){
            if(takeAttributes<Node*>(node) == RESULT_UNFINISHED) return RESULT_UNFINISHED;
            process_number_of_attributes--;
        }
        process_stack.pop_back();
    }
    process_level--;
    return RESULT_OK;
}

int RedisGraphParser::makeEdge(Edge* edge){

    std::string data;
    process_level++;
    ///Zainicjalizowanie procesu
    if(process_stack.size() == process_level-1) process_stack.push_back(1);

    ///Pobranie id krawedzi
    if(process_stack[process_level-1] == 1){

        if(getNumber(4,data) != RESULT_OK) return RESULT_UNFINISHED;
        edge->setId(atoi(data.c_str()));
        process_stack[process_level-1] = 2;
    }

    ///Pobranie relation type
    if(process_stack[process_level-1] == 2){
        if(getNumber(0,data) != RESULT_OK) return RESULT_UNFINISHED;
        //Pobranie z procedury relation_types elemenetu na podstawie id
        edge->setRelationType(relation_types->elements[atoi(data.c_str())]);
        process_stack[process_level-1] = 3;
    }

    ///Pobranie id wierzcholka poczatkowego (src node)
    if(process_stack[process_level-1] == 3){
        if(getNumber(0,data) != RESULT_OK) return RESULT_UNFINISHED;
        edge->setSrcNode(atoi(data.c_str()));
        process_stack[process_level-1] = 4;
    }

    ///Pobranie id wierzcholka konczowego (dest node)
    if(process_stack[process_level-1] == 4){
        if(getNumber(0,data) != RESULT_OK) return RESULT_UNFINISHED;
        edge->setDesNode(atoi(data.c_str()));
        process_stack[process_level-1] = 5;
    }

    ///Pobranie rozmiaru atrybutow
    if(process_stack[process_level-1] == 5){
        if(getNumber(0,data) != RESULT_OK) return RESULT_UNFINISHED;
        process_number_of_attributes = atoi(data.c_str());
        process_stack[process_level-1] = 6;
    }

    ///Pobranie atrybutow
    if(process_stack[process_level-1] == 6){

        while(process_number_of_attributes != 0){
            if(takeAttributes<Edge*>(edge) == RESULT_UNFINISHED) return RESULT_UNFINISHED;
            process_number_of_attributes--;
        }
        process_stack.pop_back();
    }
    process_level--;
    return RESULT_OK;

}

template<class T>
int RedisGraphParser::takeAttributes(T pointer_entity){

    static_assert(std::is_pointer<T>::value,"Expected a a pointer");

    std::string data;
    process_level++;
    ///Zainicjalizowanie procesu
    if(process_stack.size() == process_level-1) process_stack.push_back(1);
    ///Pobranie id atrybutu
    if(process_stack[process_level-1] == 1){
        if(getNumber(4,data) != RESULT_OK) return RESULT_UNFINISHED;
         attribute_id = atoi(data.c_str());
         process_stack[process_level-1] = 2;
    }
    ///Sprawdzenie typu atrybutu
    if(process_stack[process_level-1] == 2){
        if(getNumber(0,data) != RESULT_OK) return RESULT_UNFINISHED;
         int type = atoi(data.c_str());

         switch(type){
            //Null
            case 1:
                process_stack[process_level-1] = 3;
                break;
            //String
            case 2:
                process_stack[process_level-1] = 4;
                break;
            //Integer
            case 3:
                process_stack[process_level-1] = 5;
                break;
            //Bool
            case 4:
                process_stack[process_level-1] = 6;
                break;
            //Double
            case 5:
                process_stack[process_level-1] = 7;
                break;
            //Array
            case 6:
            {
                _vector_attribute_pointer = new std::vector<RedisGraphContainer::RedisValue>();
                process_stack[process_level-1] = 8;
                break;
            }

         }
    }
    ///Pobranie nulla
    if(process_stack[process_level-1] == 3){
        if(getNumber(0,data) == RESULT_UNFINISHED) return RESULT_UNFINISHED;
        pointer_entity->addProperty(properties_keys->elements[attribute_id], nullptr, 1);
        attribute_id = -1;
        process_stack.pop_back();
    }

    ///Pobranie stringa
    if(process_stack[process_level-1] == 4){
        if(makeString(0,data) == RESULT_UNFINISHED) return RESULT_UNFINISHED;
        std::string* value = new std::string(data);
        pointer_entity->addProperty(properties_keys->elements[attribute_id],value,2);
        attribute_id = -1;
        process_stack.pop_back();
    }

    ///Pobranie integer
    if(process_stack[process_level-1] == 5){
        if(getNumber(0,data) == RESULT_UNFINISHED) return RESULT_UNFINISHED;
        int val = atoi(data.c_str());
        int* value = new int(val);
        pointer_entity->addProperty(properties_keys->elements[attribute_id],value,3);
        attribute_id = -1;
        process_stack.pop_back();
    }

    ///Pobranie boola
    if(process_stack[process_level-1] == 6){
        if(makeString(0,data) == RESULT_UNFINISHED) return RESULT_UNFINISHED;
        bool* value = new bool();
        if(data == "true") *value = true;
        else *value =  false;
        pointer_entity->addProperty(properties_keys->elements[attribute_id],value,4);
        attribute_id = -1;
        process_stack.pop_back();
    }

    ///Pobranie double
    if(process_stack[process_level-1] == 7){
        if(makeString(0,data) == RESULT_UNFINISHED) return RESULT_UNFINISHED;
        double* p_double = new double();
        *p_double = std::stod(data);
        pointer_entity->addProperty(properties_keys->elements[attribute_id],p_double,5);
        attribute_id = -1;
        process_stack.pop_back();
    }

    ///Pobranie array
    if(process_stack[process_level-1] == 8){
        if(getArray(_vector_attribute_pointer) == RESULT_UNFINISHED) return RESULT_UNFINISHED;
        pointer_entity->addProperty(properties_keys->elements[attribute_id],_vector_attribute_pointer,6);
        attribute_id = -1;
        _vector_attribute_pointer = nullptr;
        process_stack.pop_back();
    }

    process_level--;
    return RESULT_OK;

}

int RedisGraphParser::takeRecordsofString(int omit_start,int omit_string,std::vector<std::string>& lista,int& size_of_elements){

    std::string data;
    process_level++;
    if(process_stack.size() == process_level-1) process_stack.push_back(1);
    ///Pobranie rozmiaru
    if(process_stack[process_level-1] == 1){
        if(getNumber(omit_start,data) != RESULT_OK) return RESULT_UNFINISHED;
        size_of_elements = atoi(data.c_str());
        process_number_of_elements = size_of_elements;
        process_stack[process_level-1] = 2;
    }
    ///Pobranie elementow
    if(process_stack[process_level-1] == 2){
        while(process_number_of_elements != 0){
            if(makeString(omit_string,data)== RESULT_UNFINISHED)
                return RESULT_UNFINISHED;
            lista.push_back(data);
            process_number_of_elements--;
        }
        process_stack.pop_back();
    }
    process_level--;
    return RESULT_OK;
}

int RedisGraphParser::getArray(std::vector<RedisGraphContainer::RedisValue>* lista){

    std::string data;
    process_level++;
    process_elements_of_array++;
    ///Zainicjalizowanie procesu
    if(process_stack.size() == process_level-1) process_stack.push_back(1);
    ///Pobranie rozmiaru
    if(process_stack[process_level-1] == 1){
        if(getNumber(0,data) != RESULT_OK) return RESULT_UNFINISHED;
        process_elements_of_array_stack.push_back(atoi(data.c_str()));
        process_stack[process_level-1] = 2;
    }
     ///Pobranie elementow
    if(process_stack[process_level-1] == 2){
        while(process_elements_of_array_stack[process_elements_of_array-1] != 0){
            if(takeElementsOfArray(lista) == RESULT_UNFINISHED) return RESULT_UNFINISHED;
            process_elements_of_array_stack[process_elements_of_array-1]--;
        }
        process_stack.pop_back();
        process_elements_of_array_stack.pop_back();
    }
    process_level--;
    process_elements_of_array--;
    return RESULT_OK;


}

int RedisGraphParser::takeElementsOfArray(std::vector<RedisGraphContainer::RedisValue>* lista){

    std::string data;
    process_level++;
    process_type_of_element++;
    ///Zainicjalizowanie procesu
    if(process_stack.size() == process_level-1) {
        process_stack.push_back(1);
    }

    ///Pobranie typu elementu
    if(process_stack[process_level-1] == 1){
        if(getNumber(4,data) != RESULT_OK) return RESULT_UNFINISHED;
        int type = atoi(data.c_str());
        process_type_of_element_stack.push_back(type);
        ///W przypadku listy to aby nie tworzyc kolejnego stosu dla przechowywania jedynie wskaznikow vectora to tutaj utworzy sie i
        /// dalej bedzie sie do niego odwolywac przez wziecie wskaznika z ostatniego elementu listy
        if(type == 6){
            lista->push_back(RedisGraphContainer::RedisValue(type));
        }
        process_stack[process_level-1] = 2;
    }

    ///Pobranie obiektu
    if(process_stack[process_level-1] == 2){
        int type = process_type_of_element_stack[process_type_of_element-1];
        switch(type){
        //Null
        case 1:
        {
            if(getNumber(0,data) == RESULT_UNFINISHED) return RESULT_UNFINISHED;
            lista->push_back(RedisGraphContainer::RedisValue(type));
            break;
        }
        //String
        case 2:
        {
            if(makeString(0,data) == RESULT_UNFINISHED) return RESULT_UNFINISHED;
            std::string* str = new std::string(data);
            lista->push_back(RedisGraphContainer::RedisValue(type,(void*)str));
            break;
        }
        //Integer
        case 3:
        {
            if(getNumber(0,data) == RESULT_UNFINISHED) return RESULT_UNFINISHED;
            int* p_int = new int(atoi(data.c_str()));
            lista->push_back(RedisGraphContainer::RedisValue(type,(void*)p_int));
            break;

        }
        //Boolean:
        case 4:
        {
            if(makeString(0,data) == RESULT_UNFINISHED) return RESULT_UNFINISHED;
            bool* p_bool = new bool();
            if(data == "true") *p_bool = true;
            else *p_bool =  false;
            lista->push_back(RedisGraphContainer::RedisValue(type,(void*)p_bool));
            break;

        }
        //Double
        case 5:
        {
            if(makeString(0,data) == RESULT_UNFINISHED) return RESULT_UNFINISHED;
            double* p_double = new double(std::stod(data));
            lista->push_back(RedisGraphContainer::RedisValue(type,(void*)p_double));
            break;
        }
        //Array
        case 6:
        {
            void* v_p =lista->back().getPointer();
            std::vector<RedisGraphContainer::RedisValue>* v_pointer = static_cast<std::vector<RedisGraphContainer::RedisValue>*>(v_p);
            if(getArray(v_pointer) == RESULT_UNFINISHED) return RESULT_UNFINISHED;
            break;

        }
        }
        process_stack.pop_back();
        process_type_of_element_stack.pop_back();
    }
    process_level--;
    process_type_of_element--;
    return RESULT_OK;

}

int RedisGraphParser::makeString(int omit, std::string& value){

    std::string data;
    process_level++;
    ///inicjalizacja
    if(process_stack.size() == process_level-1) process_stack.push_back(1);
    ///pobranie rozmiaru stringa
    if(process_stack[process_level-1]  == 1){
        if(getNumber(omit,data) != RESULT_OK) return RESULT_UNFINISHED;
        char_to_omit = atoi(data.c_str()) + 2;
        process_stack[process_level-1] = 2;
    }
    ///pobranie stringa
    if(process_stack[process_level-1] == 2){

        int readChar = 0;
        if(stringBuffer.readDataByNum(char_to_omit,readChar) != STRING_BUFFER_READ){
            char_to_omit -= readChar;
            process_level = 0;
            return RESULT_UNFINISHED;
        }
        value = stringBuffer.getData();
        value = value.substr(0,value.length()-2);
        process_stack.pop_back();

    }
    process_level--;
    return RESULT_OK;
}

int RedisGraphParser::getNumber(int omit, std::string& value){

    process_level++;
    ///Zainicjalizowanie procesu
    if(process_stack.size() == process_level-1){
        process_stack.push_back(1);
        char_to_omit = omit;
    }
    ///ominiecie
    if(process_stack[process_level-1] == 1){

        int readChar = 0;
        if(stringBuffer.readDataByNum(char_to_omit,readChar) != STRING_BUFFER_READ){
            char_to_omit -= readChar;
            process_level = 0;
            return RESULT_UNFINISHED;
        }
        char_to_omit = 0;
        stringBuffer.flush();
        process_stack[process_level-1] = 2;
    }
    ///pobranie liczby
    if(process_stack[process_level-1] == 2){

        if(stringBuffer.readDataByChr("\r\n",false) != STRING_BUFFER_READ)
        {
            process_level = 0;
            return RESULT_UNFINISHED;
        }
        value = stringBuffer.getData();
        value = value.substr(1,value.length()-3);
        process_stack.pop_back();

    }
    process_level--;
    return RESULT_OK;
}
