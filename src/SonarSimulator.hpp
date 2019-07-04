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
#include <math.h>
#include "Exception.hpp"

/*! 
 * \brief SonarSimulator class
 * \author emile
 * \date Created on June 28, 2019, 3:25 PM
 */

class SonarSimulator
{
    public:
        
        /**
         * Create a Sonar Simulator
         * 
         * @param frequenceHertz the Sonar Hertz frequence
         * @param pattern the Sonar depth pattern
         * @param filename the outputFileName
         * @param talkerID the Sonar talkerID
         */
        SonarSimulator(double frequenceHertz, double pattern, std::string & filename, std::string talkerID = "SD"):
        frequenceHertz(frequenceHertz),
        depthPattern(pattern),
        outputFileName(filename),
        talkerID(talkerID)
        {}
        
        /**
         * Destroy the SonarSimulator
         */
        ~SonarSimulator()
        {}
        
        /**
         * Set the Sonar Hertz frequence
         * 
         * @param frequenceHertz the new frequence
         */
        void setFrequence(double frequenceHertz)
        {
            frequenceHertz = frequenceHertz;
        }
        
        /**
         * Set the Sonar depth pattern
         * 
         * @param pattern the new pattern
         */
        void setDepthPattern(double pattern)
        {
            depthPattern = pattern;
        }
        
        /**
         * Set the OutputFileName
         * 
         * @param fileName the new filename
         */
        void setOutputFilename(std::string & fileName)
        {
            outputFileName = fileName;
        }
        
        /**
         * Open the OutputFile
         */
        void openFile()
        {
            outputFile.open(outputFileName);
            if(!outputFile)
            {
                throw new Exception("Invalid argument: Output file not found");
            }
        }
        
        /**
         * run the Sonar Simulator
         */
        void run()
        {
            if (frequenceHertz > 0)
            {
	        outputFile.open(outputFileName);
		
		if(outputFile){
			while(1)
			{
			    writeLine();
			}
		}
		else{
                        throw new Exception("Invalid argument: Output file not found");
		}
            }
            else
            {
                throw new Exception ("Out of range: Frequency cannot be negative or null");
            }
        }
        
        /**
         * write the NMEA line
         */
        void writeLine()
        {
            usleep(1000000/frequenceHertz); 
            double depth = 3.75;
            outputFile << generateNMEA(depth);
        }
        
        /**
         * generate the NMEA line
         * 
         * @param depth the depth detect by the Sonar(meter)
         */
        std::string generateNMEA(double depth)
        {
            std::stringstream nmea;
            double ftDepth = trunc(depth*3.28084*10)/10;
            double fmDepth = trunc(ftDepth/6*10)/10;
            depth = trunc(depth*10)/10;
            nmea << std::setprecision(1) << std::fixed;
            nmea << "$" << talkerID << "DBT," << ftDepth << ",f," << depth << ",M," << fmDepth << ",F" << "*";
            int checksum = calculChecksum(nmea.str());
            nmea << std::hex << checksum << "\x0d\x0a";
            return nmea.str();
        }
        
        /**
         * close the OutputFile
         */
        void closeFile()
        {
            if (outputFile)
            {
                outputFile.close();   
            }
        }
        
        /**
         * Calculation of the checksum 
         * 
         * @param nmea first part of nmea line
         */
        int calculChecksum(std::string nmea)
        {
            int checksum = 0;
            for (int i = 1; i < (int)nmea.length()-1; i++)
            {
                checksum ^= nmea.c_str()[i];
            }
            return checksum;
        }
        
    private:
        
        /**The Sonar Hertz frequence*/
        double frequenceHertz;
        
        /**The Sonar depth pattern*/
        double depthPattern;
        
        /**The OutputFileName*/
        std::string outputFileName;
        
        /**The Sonar OutputFile*/
        std::ofstream outputFile;
        
        /**The Sonar talkerID*/
        std::string talkerID;
        
};

