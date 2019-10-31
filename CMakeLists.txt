cmake_minimum_required(VERSION 3.10)
project(sidahmedhmar)

set(CMAKE_CXX_STANDARD 11)


INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIRS} )

add_executable(sidahmedhmar main.cpp csvstream.h Parser.h GeneratedPacketCounter.cpp GeneratedPacketCounter.h Node.h Interface.h PacketHis.h defs.h)

target_link_libraries(sidahmedhmar ${Boost_LIBRARIES})
