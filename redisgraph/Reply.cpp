#include "Reply.h"


template<typename T>
T Record::getValue(int i){

    if(i < 0) i = 0;
    else if(i > size_of_record - 1) i = size_of_record - 1;

    try{
        return record[i].getValue<T>();
    }catch(const BadCast& e){
        throw e;
    }

}

Record ResultSet::operator[](int index) const {

    if(index < 0) index = 0;
    else if(index > size_of_records - 1) index = size_of_records - 1;

    RedisArray record;
    for(int i = 0; i < size_of_header_row; i++) { record.push_back(columns[i][index]);}
    return Record(std::move(record), size_of_header_row);

}

void ResultSet::addHeader(std::string header) { header_row.push_back(header); }

void ResultSet::addColumn() { columns.push_back(RedisArray()); }

void ResultSet::addValue(int column_position, int valueType, void* p_value){

    columns[column_position].push_back(RedisGraphContainer::RedisValue(valueType, p_value));

}

void* ResultSet::addValue(int column_position, int valueType){

    columns[column_position].push_back(RedisGraphContainer::RedisValue(valueType));
    return columns[column_position].back().getPointer();

}


std::ostream& operator<< (std::ostream& os, const Statistics& rhs){

    os << "Statistics:\n";
    for(auto it = rhs.elements.begin(); it != rhs.elements.end(); it++){
        os << *it << "\n";
    }
    os << "\n";

    return os;

}

std::ostream& operator<< (std::ostream& os, const Procedure& rhs){

    os << "Elements: ";
    for(auto it = rhs.elements.begin(); it != rhs.elements.end(); it++){
        os << *it << " ";
    }
    os << "\n";
    os << rhs.statistics;

    return os;
}

std::ostream& operator<< (std::ostream& os, const Record& rhs){

    os << "Record: ";
    if(rhs.record.size() == 0) return os;
    auto it = rhs.record.cbegin();
    for(int i = 0; i < rhs.record.size() - 1; i++){
        os << *it << " | ";
        it++;
    }
    os << *it;
    return os;
}

std::ostream& operator<< (std::ostream& os, const ResultSet& rhs){

    os << "ResultSet:\n";
    os << "HeaderRow: ";
    for(auto it = rhs.header_row.begin(); it != rhs.header_row.end(); it++){
        os << *it << " ";
    }
    os << "\n";
    os << "ResultRow:\n";
    for(auto it = rhs.begin(); it != rhs.end(); it++){
        os << *it << "\n";
    }
    os << rhs.statistics;

    return os;

}

template std::string Record::getValue<std::string>(int i);
template int Record::getValue<int>(int i);
template bool Record::getValue<bool>(int i);
template double Record::getValue<double>(int i);
template RedisArray Record::getValue<RedisArray>(int i);
template Edge Record::getValue<Edge>(int i);
template Node Record::getValue<Node>(int i);
