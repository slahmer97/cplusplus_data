//
// Created by lahmer on 29/10/19.
//

#ifndef SIDAHMEDHMAR_PARSER_H
#define SIDAHMEDHMAR_PARSER_H

#include "csvstream.h"
#include "Node.h"
#include "GeneratedPacketCounter.h"
#include <boost/signals2.hpp>
#include <boost/bind.hpp>
#include <iostream>

class Parser {
public:

    static void init(const std::string &filePath){
        csvstream parser(filePath, ',');
        std::vector<std::pair<std::string, std::string>> row;
        while (parser >> row) {
            //std::cout << "row:" << "\n";
            //for (unsigned int i=0; i < row.size(); ++i) {
              //  const std::string &column_name = row[i].first;
               // const std::string &datum = row[i].second;
               // std::cout <<column_name<<"\t"<< datum << "\n";
            //}
            double time = std::stod(row[0].second);
            int code = std::stoi(row[1].second);;
            int packetIdentifier = std::stoi(row[2].second);;
            int fluxIdentifier = std::stoi(row[3].second);;
            std::string sourceNode = row[4].second;
            std::string destinationNode = row[5].second;
            std::string nowNode = row[6].second;
            // A new packet was generated
            Node* curr = GeneratedPacketCounter::nodes[nowNode];
            curr->queue_max_size = std::max(curr->queue_max_size,curr->curr_queue_size);


            switch (code){
                case 0:
                    onPacketGenerated(time, packetIdentifier, fluxIdentifier, sourceNode, destinationNode, nowNode);
                    break;
                case 1:
                    onPacketReceived(time, packetIdentifier, fluxIdentifier, sourceNode, destinationNode, nowNode);
                    break;
                case 2:
                    onPacketDeque(time, packetIdentifier, fluxIdentifier, sourceNode, destinationNode, nowNode);
                    break;
                case 3:
                    onPacketReachedDestination(time, packetIdentifier, fluxIdentifier, sourceNode, destinationNode, nowNode);
                    break;
                case 4:
                    onPacketDropped(time, packetIdentifier, fluxIdentifier, sourceNode, destinationNode, nowNode);
                    break;
                default:
                    std::cerr<<"Code err"<<std::endl;
                    exit(1);
            }


        }

    }

public:
    static boost::signals2::signal<void (double, int, int, std::string, std::string, std::string)> onPacketGenerated;

    //TODO :: Define The necessary variables
    static boost::signals2::signal<void (double, int, int, std::string, std::string, std::string)> onPacketReceived;
    static boost::signals2::signal<void (double, int, int, std::string, std::string, std::string)> onPacketDeque;
    static boost::signals2::signal<void (double, int, int, std::string, std::string, std::string)> onPacketReachedDestination;
    static boost::signals2::signal<void (double, int, int, std::string, std::string, std::string)> onPacketDropped;




};

boost::signals2::signal<void (double, int, int, std::string, std::string, std::string)> Parser::onPacketGenerated;
boost::signals2::signal<void (double, int, int, std::string, std::string, std::string)> Parser::onPacketReceived;
boost::signals2::signal<void (double, int, int, std::string, std::string, std::string)> Parser::onPacketDeque;
boost::signals2::signal<void (double, int, int, std::string, std::string, std::string)> Parser::onPacketReachedDestination;
boost::signals2::signal<void (double, int, int, std::string, std::string, std::string)> Parser::onPacketDropped;
#endif //SIDAHMEDHMAR_PARSER_H
