//
// Created by lahmer on 29/10/19.
//

#ifndef SIDAHMEDHMAR_PACKETCOUNTER_H
#define SIDAHMEDHMAR_PACKETCOUNTER_H


#include <string>
#include <unordered_map>
#include <map>
#include <boost/signals2/signal.hpp>
#include <fstream>
#include <cmath>
#include "Node.h"
#include "PacketHis.h"
#include "Interface.h"
#include "defs.h"
class GeneratedPacketCounter {
    public:
        GeneratedPacketCounter() = default;
        static std::unordered_map<std::string,Node*> nodes;
        static std::map<long long int,PacketHis*> packetHistory;
        static std::unordered_map<std::string,end_to_end> tt;
        static trans trans_link;
        static std::unordered_map<std::string,Interface*> interfaces;
        static std::vector<stat> packet_in_net;
        static std::vector<std::pair<long double,long double>> tt_his;

        static long long total_dropped;
        static long long total_received;
        static long long total_sent;


        static void onPacketReceived(double time, int packetIdentifier, int fluxIdentifier, const std::string& sourceNode, const std::string& destinationNode, const std::string& nowNode);
        static void onPacketDeque(double time, int packetIdentifier, int fluxIdentifier, const std::string& sourceNode, const std::string& destinationNode, const std::string& nowNode);
        static void onPacketGenerated(double time, int packetIdentifier, int fluxIdentifier, const std::string& sourceNode, const std::string& destinationNode, const std::string& nowNode);
        static void onPacketReachedDestination(double time, int packetIdentifier, int fluxIdentifier, const std::string& sourceNode, const std::string& destinationNode, const std::string& nowNode);
        static void onPacketDropped(double time, int packetIdentifier, int fluxIdentifier, const std::string& sourceNode, const std::string& destinationNode, const std::string& nowNode);

        static long double get_curr_end_to_end_delay(){
            long double ret = 0;
            long double c = 0;
            for(std::pair<std::string,end_to_end> t : tt){
                    ret +=t.second.X;
                    c += (double)t.second.count;
            }
            return ret/(double)c;
        }
        inline static void add_tt(const std::string&src,const std::string&dst,long double t_time){
            std::string tmp = src+dst;
            std::string tmp2 = dst+src;

            if(tt.find(tmp) == tt.end() && tt.find(tmp2) == tt.end()){
                end_to_end t;
                t.count = 1;
                t.X = t_time;
                t.V = t_time*t_time;
                tt[tmp] = t;
                return;
            }
            if(tt.find(tmp) != tt.end()){
                tt[tmp].count++;
                tt[tmp].X += t_time;
                tt[tmp].V += t_time*t_time;

                return;
            }
            if(tt.find(tmp2) != tt.end()){
                tt[tmp2].count++;
                tt[tmp2].X += t_time;
                tt[tmp2].V += t_time*t_time;

                return;
            }


        }

        inline static void init(){
            for (int i = 1; i < 27 ; ++i) {
                std::string node = "N"+std::to_string(i);
                Node* tmp = new Node(node);
                GeneratedPacketCounter::nodes[node] = tmp;
            }
            trans_link.count =0;
            trans_link.X = 0.0;
            trans_link.V = 0.0;

        }

        inline static void display(){
            std::ofstream f("prop_et_taux_perte.txt");
            for (const std::pair<const std::string , Node *>& n : nodes){
                std::cout<<"[+] "<<n.first<<std::endl;
                long long int queue_size = (n.second->queue_size != 0 )? n.second->queue_size : n.second->queue_max_size;


                std::cout<<"\t[+] Max number of paquet in Node size : "<<queue_size<<std::endl;
                std::cout<<"\t[+] Generated by Me                   : "<<n.second->create_by_me<<std::endl;
                std::cout<<"\t[+] droped by me                      : "<<n.second->dropped_by_me<<std::endl;
                std::cout<<"\t[+] passed by me                      : "<<n.second->passed_by_me<<std::endl;
                long double paquets_trait = ((long double)n.second->passed_by_me+n.second->create_by_me);
                std::cout<<"\t[+] paquets traité                    : "<<paquets_trait<<std::endl;

                long double prop = ((long double)n.second->dropped_by_me)/paquets_trait;
                long double taux = ((long double)n.second->dropped_by_me)/((long double)total_sent);
                std::cout<<"\t[+] prop de perte                     : "<<prop<<std::endl;
                std::cout<<"\t[+] taux de perte                     : "<<taux<<std::endl;

                std::cout<<"==================================================="<<std::endl;

                std::string tmp = std::to_string(prop)+","+std::to_string(taux)+"\n";
                f<<tmp;
            }

            std::cout<<"\n\n";
            for (const std::pair<const std::string,end_to_end>& t : tt){
                std::cout<<"[+] "<<t.first<<"\t : "<<t.second.X /(double)t.second.count<<std::endl;
            }


            std::cout<<"\n\n"<<std::endl;

            for (std::pair<std::string,Interface*> i : interfaces){
                std::cout<<"Interface "<<i.first<<" :\t"<<i.second->passed_by_me<<std::endl;
            }


            std::cout<<"\n\n"<<std::endl;
            long double mean = trans_link.X/(double)trans_link.count;
            long double seg = std::sqrt((trans_link.V-(trans_link.count*mean*mean))/((double)trans_link.count));
            long double left  = mean - (seg/std::sqrt((double)trans_link.count));
            long double right = mean + (seg/std::sqrt((double)trans_link.count));
            std::cout<<"Mean of link transmission : "<<mean<<std::endl;
            std::cout<<"Confident interval : "<<left<<" , "<<right<<" ]"<<std::endl;

            std::cout<<"[+] Total sent : "<<total_sent<<std::endl<<"[+] Total dropped : "<<total_dropped<<std::endl<<"[+] Total received : "<<total_received<<std::endl;
/*

            std::ofstream f1("packet_in_net.txt");
            std::ofstream f2("drop.txt");
            std::ofstream f3("end_to_end_delay.txt");

            long double t = packet_in_net[0].last_sec;
            for(unsigned long i=0;i< packet_in_net.size();i += 10){
                std::string tmp = std::to_string(packet_in_net[i].time)+" "+std::to_string(packet_in_net[i].packet_num)+"\n";
                f1 << tmp;
            }
            long double max = 0;
            bool tiw = false;
            for(unsigned long i=0;i< packet_in_net.size()-1;i ++){
                if(packet_in_net[i].garb == 1) {
                    std::string tmp =  std::to_string(packet_in_net[i].time) + " " + std::to_string(packet_in_net[i].dropped_num) + "\n";
                    f2 << tmp;

                }
            }

            for(auto & tt_hi : tt_his){
                std::string tmp = std::to_string(tt_hi.first) + " " + std::to_string(tt_hi.second) + "\n";
                f3 << tmp;
            }
            */

        }
        inline static Interface* get_interface(Node* src ,Node* dst){
            std::string tmp = src->getName()+"-"+dst->getName();
            if(interfaces.find(tmp) != interfaces.end())
                return interfaces[tmp];
            auto* i = new Interface(src,dst);
            interfaces[tmp] = i;
            return i;
        }





};


#endif //SIDAHMEDHMAR_PACKETCOUNTER_H
