#ifndef REPLY_H
#define REPLY_H

#include <vector>
#include <string>
#include <iostream>

#include "Error.h"
#include "Entities.h"
#include "RedisGraphContainer.h"

using RedisArray = std::vector<RedisGraphContainer::RedisValue>;

///Struktura dla statystyk
struct Statistics{

    int size_of_statistic = 0;
    std::vector<std::string> elements {};

};

///Struktura dla Procedury
struct Procedure{

    int size_of_elements = 0;
    std::vector<std::string> elements = {};

    Statistics statistics;

};

///Struktura dla pojedynczego rekordu zwroconego z zapytania
struct Record{

    int size_of_record = 0;
    RedisArray record = {};
    Record(RedisArray&& record, int size): size_of_record(size),record(std::forward<RedisArray>(record)){}

    template<typename T>
    T getValue(int i);

};

struct ResultSet{

    ///Header row
    int size_of_header_row = 0 ;
    std::vector<std::string> header_row = {};
    ///ResultRow
    int size_of_records = 0;
    ///Statistic
    Statistics statistics;

    struct iterator{

        iterator(const ResultSet* result_set, int index): index(index),result_set(result_set){};

        Record operator*() const { return (*result_set)[this->index];}
        //Prefix increment
        iterator& operator++() {index++; return *this;}
        //Postfix increment
        iterator operator++(int) {iterator tmp = *this; ++(*this); return tmp;}
        friend bool operator==(const iterator& l_it, const iterator& r_it){ return l_it.index == r_it.index; }
        friend bool operator!=(const iterator& l_it, const iterator& r_it){ return r_it.index != l_it.index; }

    private:
        int index;
        const ResultSet* result_set;
    };

    ///Zwrocenie rekordu
    Record operator[] (int index) const;
    ///Dodanie headera
    void addHeader(std::string header);
    ///Stworzenie nowej kolumny
    void addColumn();
    ///Dodanie wartosci do kolumny
    void addValue(int column_position, int valueType, void* p_value);
    ///Dodanie wartosci do kolumny wraz z zwroceniem wskaznika
    void* addValue(int column_position, int valueType);

    iterator begin() const {return iterator(this, 0);}
    iterator end() const {return iterator(this, size_of_records);}

private:
    ///Result row
    std::vector<RedisArray> columns = {};

};

std::ostream& operator<< (std::ostream& os, const Statistics& rhs);
std::ostream& operator<< (std::ostream& os, const Procedure& rhs);
std::ostream& operator<< (std::ostream& os, const Record& rhs);
std::ostream& operator<< (std::ostream& os, const ResultSet& rhs);

#endif // REPLY_H
