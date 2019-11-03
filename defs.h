//
// Created by slahmer on 10/30/19.
//

#ifndef SIDAHMEDHMAR_DEFS_H
#define SIDAHMEDHMAR_DEFS_H


typedef struct stat_s{
    long double time;
    long long int packet_num;
    long long int dropped_num;

    int garb;
    long double last_sec;
}stat;


typedef struct{
    int long long count;
    long double X;
    long double V;
} end_to_end;


typedef struct{
    int long long count;
    long double X;
    long double V;
} trans;



#endif //SIDAHMEDHMAR_DEFS_H
