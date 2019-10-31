//
// Created by slahmer on 10/30/19.
//

#ifndef SIDAHMEDHMAR_PACKETHIS_H
#define SIDAHMEDHMAR_PACKETHIS_H


#include "Node.h"

class PacketHis {
        long long int pid;


public:
    double dep;
    double arriv;
    double generated_at;
    double arrived_at;
    Node* last_node;
    Node* curr_node;
public:
    PacketHis(long long pid){
        this->pid = pid;
    }

};


#endif //SIDAHMEDHMAR_PACKETHIS_H
