//
// Created by slahmer on 10/30/19.
//

#ifndef SIDAHMEDHMAR_NODE_H
#define SIDAHMEDHMAR_NODE_H

#include "iostream"
#include "defs.h"

class Node {
private:
        std::string name;
public:
        long long int queue_max_size;
        long long int curr_queue_size;
        long long int queue_size;
        long long int create_by_me;
        long long int dropped_by_me;
        long long int passed_by_me;
        long long int for_me;
        bool is_done;

        std::vector<stat> stat_r;
public:
        inline Node(const std::string& n){
            name = n;
            queue_size = 0;
            queue_max_size = 0;
            curr_queue_size = 0;
            create_by_me = 0;
            dropped_by_me = 0;
            passed_by_me = 0;
            for_me = 0;
            is_done = false;
        }

        inline const std::string &getName() const {
            return name;
        }


};


#endif //SIDAHMEDHMAR_NODE_H
