//
// Created by slahmer on 10/30/19.
//

#ifndef SIDAHMEDHMAR_INTERFACE_H
#define SIDAHMEDHMAR_INTERFACE_H


#include "Node.h"

class Interface {
        Node* currNode;
        Node *nextNode;

public:
    long long int passed_by_me;
    long long int received;
    long long int dropped;
    Interface() = default;
   inline Interface(Node*s,Node*d){
        currNode = s;
       nextNode = d;
       passed_by_me = 0;
       received = 0;
       dropped = 0;
    }

    inline Node *getCurrNode() const {
        return currNode;
    }

    inline Node *getNextNode() const {
        return nextNode;
    }
};


#endif //SIDAHMEDHMAR_INTERFACE_H
