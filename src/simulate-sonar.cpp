/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <iomanip>


/* 
 * File:   simulate-sonar.hpp
 * Author: emile
 *
 * Created on June 26, 2019, 2:41 PM
 */

class simulateSonar
{
    public:
        simulateSonar(double pFrequence, double pPattern, std::string pFilename)
        {
            frequence = pFrequence;
            pattern = pPattern;
            filename = pFilename;
        }
        
        ~simulateSonar()
        {}
        
        void makeSignal()
        {
            std::stringstream nmeaList;
            std::ofstream out;
            while(1)
            {
                sleep(frequence);
                out = std::ofstream(filename);
                double depth = 3.75;
                nmeaList << generateNMEA(depth);
                std::cout << generateNMEA(depth);
                out << nmeaList.str();
                out.close();
            }
        }
        
    private:
        
        double frequence;
        
        double pattern;
        
        std::string filename;
        
        std::string generateNMEA(double depth)
        {
            std::stringstream nmea;
            double ftDepth = depth*3.28084;
            double fmDepth = ftDepth/6;
            nmea << std::setprecision(1) << std::fixed;
            nmea << "$SDDBT," << ftDepth << ",f," << depth << ",M," << fmDepth << ",F" << "*";
            int checksum = 0;
            for (int i = 1; i < (int)nmea.str().length()-1; i++)
            {
                checksum ^= nmea.str().c_str()[i];
            }
            nmea << std::hex << checksum << "\x0d\x0a";
            return nmea.str();
        }
};

void printUsage()
{
    std::cerr << "Usage: simulate-sonar outputFileName" << std::endl;
	exit(1);
}

int main(int argc,char **argv)
{
    if (argc >= 2)
    {
        std::string filename = argv[1];
        simulateSonar simul(3,10,filename);
        simul.makeSignal();
    }
    else
    {
        printUsage();
    }
}