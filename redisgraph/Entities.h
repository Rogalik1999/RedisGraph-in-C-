#ifndef ENTITIES_H
#define ENTITIES_H

#include <map>
#include <vector>
#include <utility>
#include <exception>
#include <iostream>

#include "RedisGraphContainer.h"
#include "Reply.h"
#include "Error.h"

class Properties{

public:
    void addProperty(std::string key, void* value, int valueType);

    template<typename T>
    T getValueOfProperty(std::string key);

    friend std::ostream& operator<< (std::ostream& os, const Properties& properties);

private:
    std::map<std::string, RedisGraphContainer::RedisValue> properties;
};

class Node: public Properties{

    int id;
    std::string label;

    std::string alias;

public:

    Node(int id, std::string label, std::string alias);
    ///Musi byc konstruktor bez niczego aby moc zainicjalizowac go dynamicznie
    Node();
    ~Node();

    ///Id
    void setId(int id);
    int getId();
    ///Label
    void setLabel(std::string label);
    std::string getLabel();
    ///Alias
    void setAlias(std::string alias);
    std::string getAlias();

    friend std::ostream& operator<< (std::ostream& os, const Node& node);

};


class Edge: public Properties{

    int id;
    std::string relation_type;
    int src_node;
    int des_node;

    std::string alias;

public:
    Edge(int id, int src_node, int des_node, std::string relation_type, std::string alias);
     ///Musi byc konstruktor bez niczego aby moc zainicjalizowac go dynamicznie
    Edge();
    ~Edge();

    ///Id
    void setId(int id);
    int getId();
    ///RelationType
    void setRelationType(std::string relation_type);
    std::string getRelationType();
    ///Source Node id
    void setSrcNode(int src_node);
    int getSrcNode();
    ///Destination Node id
    void setDesNode(int des_node);
    int getDesNode();
    ///Alias
    void setAlias(std::string alias);
    std::string getAlias();

    friend std::ostream& operator<<(std::ostream& os, const Edge& edge);

};



#endif // ENTITIES_H
