/*
* Copyright 2019 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
*/

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <iomanip>
#include <csignal>
#include <exception>
#include "SonarSimulator.hpp"

/* 
 * File:   simulate-sonar.cpp
 * Author: emile
 *
 * Created on June 26, 2019, 2:41 PM
 */

void printUsage()
{
    std::cerr << "Usage: simulate-sonar outputFileName" << std::endl;
	exit(1);
}

SonarSimulator *simulator;

void closeProgram(int signum)
{
    simulator->closeFile();
    exit(1);
}

int main(int argc,char **argv)
{
    signal(SIGINT,closeProgram);
    if (argc == 2)
    {    
        std::string filename = argv[1];
        simulator = new SonarSimulator(0.5,10,filename);
        simulator->run();
    }
    else
    {
        printUsage();
    }
}
