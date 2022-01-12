//---------------------------------------------------------------------------

#ifndef RedisGraphContainerH
#define RedisGraphContainerH
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <typeinfo>

#include "Error.h"

#define VALUE_UNKNOWN 0

#define VALUE_NULL 1
#define VALUE_STRING 2
#define VALUE_INTEGER 3
#define VALUE_BOOLEAN 4
#define VALUE_DOUBLE 5
#define VALUE_ARRAY 6
#define VALUE_EDGE 7
#define VALUE_NODE 8
#define VALUE_PATH 9
#define VALUE_MAP 10
#define VALUE_POINT 11

class Node;
class Edge;

namespace RedisGraphContainer{

class BasePointer{

    void* p;
public:
    BasePointer(void* p): p(p){}
    virtual ~BasePointer() = default;
    virtual void deleteValueOnHeap(){}
    virtual void* copyValueToHeap(){ return nullptr;}

    virtual bool is_same_type(const std::type_info& b) {return typeid(void*)==b;}

    virtual void print(std::ostream& os) const{ os << "nullptr"; }
    friend std::ostream& operator<< (std::ostream& os, const BasePointer& bp);
};

template<typename T>
class RedisValuePointer: public BasePointer{

    T* pointer;
public:
    RedisValuePointer(void* poi): BasePointer(poi)
    {
        pointer = static_cast<T*>(poi);
    }

    virtual ~RedisValuePointer() override = default;
    virtual void deleteValueOnHeap() override{ delete pointer; }
    virtual void* copyValueToHeap() override{

        T* new_pointer = new T(*pointer);
        void* p_value = (void*)new_pointer;
        return p_value;
    }

    virtual bool is_same_type(const std::type_info& b) override{
        return typeid(T)==b;
    }

    virtual void print(std::ostream& os) const {
        os << *pointer;
    }

};

class RedisValue{

    int type;
    void* p_value;

public:
    ///Konstruktor w przypadku juz wczesniejszej inicjalizacji i w celu jedynie przechowania
    RedisValue(int type, void* p_value);
    ///Konstruktor w przypadku inicjalizacji konkretnej wartosci na stercie
    RedisValue(int type);
    ~RedisValue();

    ///Copy and move constructor
    RedisValue(const RedisValue& other);
    RedisValue(RedisValue&& other) noexcept;

    ///Copy and move assignment operator
    RedisValue& operator=(const RedisValue& other);
    RedisValue& operator=(RedisValue&& other) noexcept;

    void* getPointer(){ return p_value; }
    int getType() { return type; }

    template<typename T>
    T getValue();

    friend std::ostream& operator<< (std::ostream& os, const RedisValue& rhs);

};


///Funkcja do zwracania wskaznika konkretnego typu wskaznika (e.g int*; double*)
std::shared_ptr<BasePointer> getPointerType(int type, void* pointer);

}

#endif
