#include <iostream>
#include "Parser.h"
#include "GeneratedPacketCounter.h"

int main() {
    std::unique_ptr<GeneratedPacketCounter> generatedPacketCounter(new GeneratedPacketCounter());

    Parser::onPacketGenerated.connect([&generatedPacketCounter](double time, int packetIdentifier, int fluxIdentifier, const std::string& sourceNode, const std::string& destinationNode, const std::string& nowNode){
        generatedPacketCounter->onPacketGenerated(time, packetIdentifier, fluxIdentifier, sourceNode, destinationNode, nowNode);
    });
    Parser::onPacketDeque.connect([&generatedPacketCounter](double time, int packetIdentifier, int fluxIdentifier, const std::string& sourceNode, const std::string& destinationNode, const std::string& nowNode){
        generatedPacketCounter->onPacketDeque(time, packetIdentifier, fluxIdentifier, sourceNode, destinationNode, nowNode);
    });
    Parser::onPacketReceived.connect([&generatedPacketCounter](double time, int packetIdentifier, int fluxIdentifier, const std::string& sourceNode, const std::string& destinationNode, const std::string& nowNode){
        generatedPacketCounter->onPacketReceived(time, packetIdentifier, fluxIdentifier, sourceNode, destinationNode, nowNode);
    });
    Parser::onPacketReachedDestination.connect([&generatedPacketCounter](double time, int packetIdentifier, int fluxIdentifier, const std::string& sourceNode, const std::string& destinationNode, const std::string& nowNode){
        generatedPacketCounter->onPacketReachedDestination(time, packetIdentifier, fluxIdentifier, sourceNode, destinationNode, nowNode);
    });
    Parser::onPacketDropped.connect([&generatedPacketCounter](double time, int packetIdentifier, int fluxIdentifier, const std::string& sourceNode, const std::string& destinationNode, const std::string& nowNode){
        generatedPacketCounter->onPacketDropped(time, packetIdentifier, fluxIdentifier, sourceNode, destinationNode, nowNode);
    });

    GeneratedPacketCounter::init();

    Parser::init("/home/slahmer/CLionProjects/sidahmedhmar/cmake-build-debug/trace2650_preprocessed.csv");

    GeneratedPacketCounter::display();
    return 0;
}