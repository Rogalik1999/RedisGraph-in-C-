#include "RedisGraphContainer.h"
///import Entities musi sie znajdowac w pliku cpp, z powodu kolizji importowania dwoch header files przez siebie
#include "Entities.h"

namespace RedisGraphContainer{


RedisValue::RedisValue(int type, void* p_value): type(type), p_value(p_value){}

RedisValue::RedisValue(int type): type(type), p_value(nullptr)
{

    switch(type){
        case VALUE_NULL:
            break;
        case VALUE_STRING:
        {
            std::string* p_str = new std::string();
            p_value = (void*)p_str;
            break;
        }
        case VALUE_INTEGER:
        {
            int* p_int = new int();
            p_value = (void*)p_int;
            break;
        }
        case VALUE_BOOLEAN:
        {
            bool* p_bool = new bool();
            p_value = (void*)p_bool;
            break;
        }
        case VALUE_DOUBLE:
        {
            double* p_double = new double();
            p_value = (void*)p_double;
            break;
        }
        case VALUE_ARRAY:
        {
            std::vector<RedisValue>* p_array = new std::vector<RedisValue>();
            p_value = (void*)p_array;
            break;
        }
        case VALUE_EDGE:
        {
            Edge* p_edge = new Edge();
            p_value = (void*)p_edge;
            break;
        }
        case VALUE_NODE:
        {
            Node* p_node = new Node();
            p_value = (void*)p_node;
            break;
        }
        default:
            break;

    }

}

RedisValue::~RedisValue()
{
        ///Usuwanie istniejej wartosci
        auto old_value = getPointerType(this->type, this->p_value);
        old_value->deleteValueOnHeap();
}

RedisValue::RedisValue(const RedisValue& other): type(other.type), p_value(nullptr)
{
    ///Kopiowanie wartosci
    auto other_value = getPointerType(other.type, other.p_value);
    this->p_value = other_value->copyValueToHeap();

}

RedisValue::RedisValue(RedisValue&& other) noexcept : type(-1), p_value(nullptr)
{
    type = other.type;
    p_value = other.p_value;

    other.p_value = nullptr;
    other.type = -1;

}

RedisValue& RedisValue::operator=(const RedisValue& other)
{

    if(this != &other){

        if(this->type != other.type) {this->type = other.type;}

        ///Usuwanie istniejej wartosci
        auto old_value = getPointerType(this->type, this->p_value);
        old_value->deleteValueOnHeap();

        ///Kopiowanie wartosci
        auto other_value = getPointerType(other.type, other.p_value);
        this->p_value = other_value->copyValueToHeap();

    }

    return *this;
}

RedisValue& RedisValue::operator=(RedisValue&& other) noexcept
{

    if(this != &other){

        ///Usuwanie istniejej wartosci
        auto old_value = getPointerType(this->type, this->p_value);
        old_value->deleteValueOnHeap();

        type = other.type;
        p_value = other.p_value;

        other.p_value = nullptr;
        other.type = -1;

    }

    return *this;

}

template<typename T>
T RedisValue::getValue(){

    auto value = getPointerType(this->type, this->p_value);

    if(!value->is_same_type(typeid(T))){
        throw BadCast();
    }

    return (*(reinterpret_cast<T*>(p_value)));
}

std::shared_ptr<BasePointer> getPointerType(int type, void* pointer){

     switch(type){
        case VALUE_NULL:
        {
            std::shared_ptr<BasePointer> sp = std::make_shared<BasePointer>(nullptr);
            return sp;
        }
        case VALUE_STRING:
        {
            std::shared_ptr<BasePointer> sp = std::make_shared<RedisValuePointer<std::string>>(pointer);
            return sp;
        }
        case VALUE_INTEGER:
        {
            std::shared_ptr<BasePointer> sp = std::make_shared<RedisValuePointer<int>>(pointer);
            return sp;
        }
        case VALUE_BOOLEAN:
        {
            std::shared_ptr<BasePointer> sp = std::make_shared<RedisValuePointer<bool>>(pointer);
            return sp;
        }
        case VALUE_DOUBLE:
        {
            std::shared_ptr<BasePointer> sp = std::make_shared<RedisValuePointer<double>>(pointer);
            return sp;
        }
        case VALUE_ARRAY:
        {
            std::shared_ptr<BasePointer> sp = std::make_shared<RedisValuePointer<std::vector<RedisValue>>>(pointer);
            return sp;
        }
        case VALUE_EDGE:
        {
            std::shared_ptr<BasePointer> sp = std::make_shared<RedisValuePointer<Edge>>(pointer);
            return sp;
        }
        case VALUE_NODE:
        {
            std::shared_ptr<BasePointer> sp = std::make_shared<RedisValuePointer<Node>>(pointer);
            return sp;
        }
        default:
            break;

    }

    return std::make_shared<BasePointer>(nullptr);

}

std::ostream& operator<< (std::ostream& os, const BasePointer& bp){

    bp.print(os);
    return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<RedisValue>& v){

    os << "[";
    if(v.size() == 0){ os << "]"; return os; }
    /// length() - 1 poniewaz chcemy bez ostatniego
    for(int i = 0; i < v.size()-1; i++){
        os << v[i] << ",";
    }
    os << v[v.size()-1];
    os << "]";
    return os;
}

std::ostream& operator<< (std::ostream& os, const RedisValue& rhs){

    auto pointer = getPointerType(rhs.type, rhs.p_value);

    if (rhs.type == VALUE_STRING){
        os << "\"" << *pointer << "\"";
    }else{
        os << *pointer;
    }
    return os;
}

///Nalezy podac definicje roznych typow gdyz linker nie potrafi i pojawia sie blad ze undefined reference
template std::string RedisValue::getValue<std::string>();
template int RedisValue::getValue<int>();
template bool RedisValue::getValue<bool>();
template double RedisValue::getValue<double>();
template std::vector<RedisValue> RedisValue::getValue<std::vector<RedisValue>>();
template Edge RedisValue::getValue<Edge>();
template Node RedisValue::getValue<Node>();

}



