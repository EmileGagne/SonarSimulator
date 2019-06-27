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
#include <csignal>

/* 
 * File:   simulate-sonar.hpp
 * Author: emile
 *
 * Created on June 26, 2019, 2:41 PM
 */

class SonarSimulator
{
    public:
        
        SonarSimulator()
        {
            frequence = 0;
            pattern = 0;
            filename = "";
        }
        
        SonarSimulator(double frequenceSecond, double patternFix, std::string fileName)
        {
            frequence = frequenceSecond;
            pattern = patternFix;
            filename = fileName;
        }
        
        ~SonarSimulator()
        {}
        
        void setFrequence(double frequenceSecond)
        {
            frequence = frequenceSecond;
        }
        
        void setPattern(double patternFix)
        {
            pattern = patternFix;
        }
        
        void setFilename(std::string fileName)
        {
            filename = fileName;
        }
        
        void run()
        {
            out = std::ofstream(filename);
            while(1)
            {
                sleep(frequence);
                double depth = 3.75;
                std::cout << generateNMEA(depth);
                out << generateNMEA(depth);
            }
        }
        
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
        
        void closeFile()
        {
            out.close();
        }
        
    private:
        
        double frequence;
        
        double pattern;
        
        std::string filename;
        
        std::ofstream out;
        
};

void printUsage()
{
    std::cerr << "Usage: simulate-sonar outputFileName" << std::endl;
	exit(1);
}

SonarSimulator simul;

void closeProgram(int signum)
{
    simul.closeFile();
    exit(1);
}

int main(int argc,char **argv)
{
    if (argc >= 2)
    {
        std::string filename = argv[1];
        simul.setFrequence(3);
        simul.setPattern(10);
        simul.setFilename(filename);
        signal(SIGINT,closeProgram);
        simul.run();
    }
    else
    {
        printUsage();
    }
}