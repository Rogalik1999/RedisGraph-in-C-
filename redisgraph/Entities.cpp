#include "Entities.h"

///*/////////////////
///Klasa Properties
///*//////////////////
void Properties::addProperty(std::string key, void* value, int valueType){
    properties.insert(std::pair<std::string,RedisGraphContainer::RedisValue>(key,RedisGraphContainer::RedisValue(valueType, value)));
}

template<typename T>
T Properties::getValueOfProperty(std::string key){
    try{
        return (properties.at(key)).getValue<T>();
    }catch(const BadCast& e){
        throw e;
    }catch(const std::out_of_range& ex){
        throw OutOfRangeProperties("Klucz nie znajduje sie w zbiorze wlasciwosci");
    }
}

///*/////////////////
///Klasa Node
///*//////////////////

Node::Node(int id, std::string label, std::string alias): id(id), label(label), alias(alias){

    if(this->id < -1) this->id = -1;
}
Node::Node(): id(-1), label(""), alias(""){}
Node::~Node(){}

void Node::setId(int id){

    if(id < 0) this->id = -1;
    else this->id = id;
}

int Node::getId() { return this->id;}

void Node::setLabel(std::string label){

   this->label = label;
}

std::string Node::getLabel() { return this->label;}

void Node::setAlias(std::string alias){
    this->alias = alias;
}

std::string Node::getAlias() { return this->alias;}

///*/////////////////////
///Klasa EDGE
///*//////////////////

Edge::Edge(int id, int src_node, int des_node, std::string relation_type, std::string alias): id(id),relation_type(relation_type),
                                                                    src_node(src_node), des_node(des_node), alias(alias){

    if(this->id < -1) this->id = -1;
    if(this->src_node < -1) this->src_node = -1;
    if(this->src_node < -1) this->des_node = -1;
}
Edge::Edge(): id(-1), relation_type(""), src_node(-1), des_node(-1), alias(""){}
Edge::~Edge(){}

void Edge::setId(int id){

    if(id < 0) this->id = -1;
    else this->id = id;
}

int Edge::getId() { return this->id;}

void Edge::setRelationType(std::string relation_type){

   this->relation_type = relation_type;
}

std::string Edge::getRelationType() { return this->relation_type;}

void Edge::setSrcNode(int src_node){

    if(src_node < 0) this->src_node = -1;
    else this->src_node = src_node;
}

int Edge::getSrcNode() { return this->src_node;}

void Edge::setDesNode(int des_node){

    if(des_node < 0) this->des_node = -1;
    else this->des_node = des_node;
}

int Edge::getDesNode() { return this->des_node;}

void Edge::setAlias(std::string alias){
    this->alias = alias;
}

std::string Edge::getAlias() { return this->alias;}


std::ostream& operator<< (std::ostream& os, const Properties& properties){

    if(properties.properties.size() == 0 ) return os;
    os << "{";
    auto it = properties.properties.cbegin();
    for(int i = 0 ; i < properties.properties.size() - 1; i++){
        os << it->first << ":" << it->second << ", ";
        it++;
    }
    os << it->first << ":" << it->second;
    os << "}";
    return os;
}

std::ostream& operator<< (std::ostream& os, const Node& node){

    os << "(" << node.alias << ":" << node.label;
    os << (Properties&)(node);
    os << ")";

    return os;
}

std::ostream& operator<<(std::ostream& os, const Edge& edge){

    os << "[" << edge.alias << ":" << edge.relation_type;
    os << (Properties&)(edge);
    os << "]";

    return os;
}


template std::string Properties::getValueOfProperty<std::string>(std::string key);
template int Properties::getValueOfProperty<int>(std::string key);
template bool Properties::getValueOfProperty<bool>(std::string key);
template double Properties::getValueOfProperty<double>(std::string key);
template std::vector<RedisGraphContainer::RedisValue> Properties::getValueOfProperty<std::vector<RedisGraphContainer::RedisValue>>(std::string key);
