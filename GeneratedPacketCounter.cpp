//
// Created by lahmer on 29/10/19.
//

#include <iostream>
#include <deque>
#include "GeneratedPacketCounter.h"
std::unordered_map<std::string,Node*> GeneratedPacketCounter::nodes;
std::map<long long int,PacketHis*> GeneratedPacketCounter::packetHistory;
std::unordered_map<std::string,std::pair<long,long double>> GeneratedPacketCounter::tt;
std::pair<long,long double> GeneratedPacketCounter::trans_link = std::make_pair(0,0.0);
std::unordered_map<std::string,Interface*> GeneratedPacketCounter::interfaces;
std::vector<stat> GeneratedPacketCounter::packet_in_net;


long long  GeneratedPacketCounter::total_dropped = 0;
long long  GeneratedPacketCounter::total_received = 0;
long long  GeneratedPacketCounter::total_sent = 0;

void GeneratedPacketCounter::onPacketGenerated(double time, int pid, int fid, const std::string &src, const std::string &dst, const std::string &pos) {
    //0 départ de la source
    total_sent++;
    Node* curr = nodes[pos];
    curr->create_by_me++;
    curr->curr_queue_size++;

    auto *packet = new PacketHis(pid);
    packet->curr_node = curr;
    packet->last_node = curr;
    packetHistory[pid] = packet;

    packet->generated_at = time;
    packet->dep = time;


    unsigned long index = packet_in_net.size()-1;

    stat t;
    t.time = time;
    t.packet_num = 0;
    t.dropped_num = 0;
    t.last_sec = 1;

    if(!packet_in_net.empty() && packet_in_net[index].time == time)
        packet_in_net[index].packet_num++;
    else if(packet_in_net.empty())
        packet_in_net.emplace_back(t);

    else{
        t.packet_num = packet_in_net[index].packet_num+1;

        if(time < packet_in_net[index].last_sec ){
            t.dropped_num = packet_in_net[index].dropped_num;
            t.last_sec = packet_in_net[index].last_sec;
        }
        else{
            t.dropped_num = 0;
            t.last_sec = packet_in_net[index].last_sec+1;
        }
        t.last_sec = packet_in_net[index].last_sec;
        packet_in_net.emplace_back(t);

    }






}




void GeneratedPacketCounter::onPacketReceived(double time, int pid, int fid, const std::string &src, const std::string &dst, const std::string &pos){
   //1 arrivée dans un nœud intermédiaire
    Node* curr = nodes[pos];
    Node* prev;
    //ON queue
    curr->curr_queue_size++;

    PacketHis* packet = packetHistory[pid];
    prev = packet->last_node;

    Interface* in = get_interface(prev,curr);
    in->passed_by_me++;


    packet->arriv = time;
    trans_link.first ++;
    trans_link.second += time-packet->dep;
    packet->curr_node = nodes[pos];

}

void GeneratedPacketCounter::onPacketDeque(double time, int pid, int fid, const std::string &src, const std::string &dst, const std::string &pos) {
    //2 départ d’une file d’attente
    Node* curr = nodes[pos];
    curr->passed_by_me++;


    PacketHis* packet = packetHistory[pid];
    packet->curr_node->curr_queue_size--;
    packet->dep = time;

}



void GeneratedPacketCounter::onPacketReachedDestination(double time, int pid, int fid, const std::string &src, const std::string &dst, const std::string &pos){
   //3 arrivée à destination
    total_received++;
    Node* curr = nodes[pos];
    curr->for_me++;
    curr->curr_queue_size--;


    PacketHis* packet = packetHistory[pid];

    trans_link.first++;
    trans_link.second += time-packet->dep;

    long double sent_time = time - packet->generated_at;
    add_tt(src,dst,sent_time);
    delete(packet);

    unsigned long index = packet_in_net.size()-1;
    if(packet_in_net[index].time == time){
        packet_in_net[index].packet_num--;
    }
    else{

        stat t;
        t.time = time;
        if(time <=  packet_in_net[index].last_sec){
            t.dropped_num = packet_in_net[index].dropped_num;
            t.last_sec = packet_in_net[index].last_sec;
        }
        else{
            t.dropped_num = 1;
            t.last_sec = packet_in_net[index].last_sec+1;
        }

        t.packet_num = packet_in_net[index].packet_num-1;
        packet_in_net.emplace_back(t);
    }


}

void GeneratedPacketCounter::onPacketDropped(double time, int pid, int fid, const std::string &src, const std::string &dst, const std::string &pos) {
    //4 destruction d’un paquet (placement dans une file pleine)
    total_dropped++;
    Node* curr = nodes[pos];
    curr->dropped_by_me++;

    curr->curr_queue_size--;

    if(curr->dropped_by_me == 1 &&  !curr->is_done){
        curr->queue_size = curr->curr_queue_size;
        curr->is_done = true;
    }
    PacketHis* packet = packetHistory[pid];

    delete(packet);

    //delete packet history

    unsigned long index = packet_in_net.size()-1;


    if(packet_in_net[index].time == time){
        packet_in_net[index].packet_num--;
        packet_in_net[index].dropped_num++;
    }
    else{
        stat t;
        t.time = time;

        if(time <=  packet_in_net[index].last_sec){
            t.dropped_num = packet_in_net[index].dropped_num;
            t.last_sec = packet_in_net[index].last_sec;
        }
        else{
            t.dropped_num = 1;
            t.last_sec = packet_in_net[index].last_sec+1;
        }

        t.packet_num = packet_in_net[index].packet_num-1;
        packet_in_net.emplace_back(t);
    }

}
