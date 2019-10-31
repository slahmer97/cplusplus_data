//
// Created by slahmer on 10/30/19.
//

#ifndef SIDAHMEDHMAR_DEFS_H
#define SIDAHMEDHMAR_DEFS_H


typedef struct stat_s{
    long double time;
    long long int packet_num;
    long long int dropped_num;


    long double last_sec;
}stat;
#endif //SIDAHMEDHMAR_DEFS_H
