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

/* 
 * File:   SonarSimulator.hpp
 * Author: emile
 *
 * Created on June 28, 2019, 3:25 PM
 */

class SonarSimulator
{
    public:
        
        SonarSimulator(double frequenceHertz, double pattern, std::string & filename, std::string talkerID = "SD"):
        frequenceHertz(frequenceHertz),
        depthPattern(pattern),
        outputFileName(filename),
        talkerID(talkerID)
        {}
        
        ~SonarSimulator()
        {}
        
        void setFrequence(double frequenceHertz)
        {
            frequenceHertz = frequenceHertz;
        }
        
        void setDepthPattern(double pattern)
        {
            depthPattern = pattern;
        }
        
        void setOutputFilename(std::string & fileName)
        {
            outputFileName = fileName;
        }
        
        void run()
        {
            if (frequenceHertz > 0)
            {
	        outputFile.open(outputFileName);
		
		if(outputFile){
			while(1)
			{
			    usleep(1000000/frequenceHertz); 
			    double depth = 3.75;
			    outputFile << generateNMEA(depth);
			}
		}
		else{
			throw new std::invalid_argument("Output file not found");
		}
            }
            else
            {
		throw new std::out_of_range("Frequency cannot be negative");
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
            outputFile.close();
        }
        
    private:
        
        double frequenceHertz;
        
        double depthPattern;
        
        std::string outputFileName;
        
        std::ofstream outputFile;
        
        std::string talkerID;
        
};

