#ifndef REDISGRAPHPARSER_H
#define REDISGRAPHPARSER_H

#define RESULT_OK 1
#define RESULT_UNFINISHED 0

#include "StringBuffer.h"
#include "Reply.h"
#include "Error.h"
#include "RedisGraphContainer.h"

#include <vector>
#include <string> // std::stod()

class RedisGraphParser{

private:

    int process_number_of_elements; /// liczba elementow ktora zostala do przetworzenia

    ///Do result set
    int process_number_of_records;
    int process_number_of_header_row;
    int process_number_of_attributes;

    ///etapy przetwarzania procesow
    int process_level;
    std::vector<int> process_stack;

    ///Stos z procesami przetwarzania elementow listy
    int process_elements_of_array;
    std::vector<int> process_elements_of_array_stack;

    ///Stos z przetwarzanymi typami elementow w liscie
    int process_type_of_element;
    std::vector<int> process_type_of_element_stack;

    ///nazwa atrybutu
    int attribute_id;

    ///typ wartosci i pusty wskaznik
    int _type;
    void* _pointer;

    ///Dodatkowy wskaznik dla listy w atrybutach
    std::vector<RedisGraphContainer::RedisValue>* _vector_attribute_pointer;

    ///Wskaznik do Procedur Labels, RelationType, PropertiesKeys
    Procedure* labels;
    Procedure* relation_types;
    Procedure* properties_keys;

    ///ilosc znakow do pobrania
    int char_to_omit;

    ///STRING BUFFER
    StringBuffer stringBuffer;

public:

    RedisGraphParser(const char* data, int len);
    ~RedisGraphParser();
    ///zapisanie danych
    void write(const char* data, int len);
    ///Ustawianie wskaznikow do procedur
    void setProceduresPointers(Procedure* labels, Procedure* relation_types, Procedure* properties_keys);
    ///Ustawienie wszystkim wskaznikom procedur nullptr
    void resetProcedurePointers();

    ///Tworzenie ResultSet
    ///Trzeba jako parametry podac procedury z Label, RelationType, PropertyKey poniewaz jest to oparte na fladze --compact ktora korzysta z tych
    int makeResultSet(ResultSet& result_set);
    ///Tworzenie procedury z lista RelationTypes
    int makeProcedureRelationType(Procedure& procedure);
    ///Tworzenie procedury z lista Labels
    int makeProcedureLabel(Procedure& procedure);
    ///Tworzenie procedury z lista PropertyKeys
    int makeProcedurePropertyKey(Procedure& procedure);
    ///Tworzenie statystyk
    int makeStatistics(Statistics& statistics);

    ///Sprawdzenie odpowiedzi od serwera czy wystapil error
    int checkResponse();

private:
    ///przetworzenie procedury
    int makeProcedureCall(int omit,Procedure& procedure);
    ///Pobieranie resultow z Result set (pyta sie czy ma inicjalizowac liste obiektow)
    int takeRecordsofHeaderRow(ResultSet& result_set, bool ifInitialized);
    ///Sprawdzenie rodzaju elementu (czy to Node lub Edge) i inicjalizacja listy z tymi elementami jesli ifInitialized jest true
    int takeValueType(ResultSet& result_set, bool ifInitialized);
    /// Pobranie wierzcholka
    int makeNode(Node* node);
    ///Pobranie krawedzi
    int makeEdge(Edge* edge);
    ///Pobranie atrubutow
    template<class T>
    int takeAttributes(T pointer_entity);
    ///Pobieranie rekordow ze stringami
    int takeRecordsofString(int omit_start,int omit_string,std::vector<std::string>& lista, int& size_of_elements);
    ///Pobieranie listy z elementami
    int getArray(std::vector<RedisGraphContainer::RedisValue>* lista);
    ///Pobranie elementu z listy
    int takeElementsOfArray(std::vector<RedisGraphContainer::RedisValue>* lista);
    ///Pozyskanie stringa
    int makeString(int omit, std::string& value);
    ///Pozyskanie liczby w postaci stringa
    int getNumber(int omit, std::string& value);

};

#endif // REDISGRAPHPARSER_H
