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

/* 
 * File:   simulate-sonar.hpp
 * Author: emile
 *
 * Created on June 26, 2019, 2:41 PM
 */

class SonarSimulator
{
    public:
        
        SonarSimulator(double frequenceHertz, double pattern, std::string filename, std::string talkerID = "SD"):
        frequence(frequenceHertz),
        pattern(pattern),
        filename(filename),
        talkerID(talkerID)
        {}
        
        ~SonarSimulator()
        {}
        
        void setFrequence(double frequenceHertz)
        {
            frequence = frequenceHertz;
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
            if (frequence > 0)
            {
                while(1)
                {
                    usleep(1000000/frequence); 
                    double depth = 3.75;
                    out << generateNMEA(depth);
                }
            }
            else
            {
                out.close();
                exit(1);
            }
        }
        
        std::string generateNMEA(double depth)
        {
            std::stringstream nmea;
            double ftDepth = depth*3.28084;
            double fmDepth = ftDepth/6;
            nmea << std::setprecision(1) << std::fixed;
            nmea << "$" << talkerID << "DBT," << ftDepth << ",f," << depth << ",M," << fmDepth << ",F" << "*";
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
        
        std::string talkerID;
        
};

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