/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SonarSimulatorTest.hpp
 * Author: emile
 *
 * Created on June 28, 2019, 3:06 PM
 */

#include "catch.hpp"
#include "../src/SonarSimulator.hpp"
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <ctime>
#include <chrono>
#include <exception>
#include <ratio>
#include <iostream>

using namespace std;
#ifdef _WIN32
static string binexec("..\\bin\\simulate-sonar.exe");
static string outputdir(".");
#else
static string binexec("build/bin/simulate-sonar");
static string outputdir(".");
#endif

/**
 * Execute the SonarSimulator main function
 * 
 * @param command the parameters for the execution
 */
std::stringstream system_call(const std::string& command){

     std::stringstream out;
     FILE *fp;
     char path[1035];

#ifdef _WIN32
     fp = _popen(command.c_str(), "r");
#else
     fp = popen(command.c_str(), "r");
#endif
     if (fp == NULL) {
	printf("Failed to run command\n" );
	exit(1);
     }

     while (fgets(path, sizeof(path)-1, fp) != NULL) {
	     out<<path;
     }

#ifdef _WIN32
     _pclose(fp);
#else
     pclose(fp);
#endif

     return out;
}

TEST_CASE("test with no output file")
{
    SonarSimulator *simulator;
    std:string empty = "";
    simulator = new SonarSimulator(1,1,empty);
    std::string msgError = "";
    try
    {
        simulator->run();
    }
    catch(Exception * e)
    {
        msgError = e->getMessage();
    }
    REQUIRE(msgError == "Invalid argument: Output file not found");
}

TEST_CASE("test with a negative frequence")
{
    SonarSimulator *simulator;
    std::string filename = "ayudy";
    simulator = new SonarSimulator(-1,1,filename);
    std::string msgError = "";
    try
    {
        simulator->run();
    }
    catch(Exception * e)
    {
        msgError = e->getMessage();
    }
    REQUIRE(msgError == "Out of range: Frequency cannot be negative or null");
}

TEST_CASE("test with a null frequence")
{
    SonarSimulator *simulator;
    std::string filename = "hdohef";
    simulator = new SonarSimulator(0,1,filename);
    std::string msgError = "";
    try
    {
        simulator->run();
    }
    catch(Exception * e)
    {
        msgError = e->getMessage();
    }
    REQUIRE(msgError == "Out of range: Frequency cannot be negative or null");
}

TEST_CASE("test the Sonar Simulator command with wrong parameter")
{
    string commFile = " 2>&1";
    std::stringstream ss;
    ss = system_call(std::string(binexec+commFile));
    REQUIRE(ss.str()=="Usage: simulate-sonar outputFileName\n");
}

TEST_CASE("test the checksum calculation")
{
    std::string line = "$GPGGA,151748.00,4645.5364455,N,07255.2973044,W,2,14,0.8,278.587,M,-31.492,M,5.0,0133*";
    SonarSimulator *simulator;
    std::string filename = "test";
    simulator = new SonarSimulator(1,1,filename);
    int checksum = simulator->calculChecksum(line);
    std::stringstream ss;
    ss<<std::hex<<checksum;
    REQUIRE(ss.str() == "7a");
}

TEST_CASE("test the Sonar Simulator NMEA line")
{
    SonarSimulator *simulator;
    std::string filename = "outTest";
    std::string talkerID = "SD";
    simulator = new SonarSimulator(1,1,filename,talkerID);
    std::string result;
    result = simulator->generateNMEA(3.75);
    char ID[3];
    double depthFeet;
    double depthMeter;
    double depthFathoms;
    char checksum[3];
    REQUIRE(sscanf(result.c_str(),"$%2sDBT,%lf,f,%lf,M,%lf,F*%2s\x0d\x0a",ID,&depthFeet,&depthMeter,&depthFathoms,checksum)==5);
    REQUIRE(std::string(ID)==talkerID);
    REQUIRE(abs(depthFeet-12.3)<1e-10);
    REQUIRE(abs(depthMeter-3.7)<1e-10);
    REQUIRE(abs(depthFathoms-2.0)<1e-10);
    REQUIRE(std::string(checksum)=="2f");
}

TEST_CASE("test the writing on the outputFile")
{
    SonarSimulator *simulator;
    std::string filename = "outTest";
    simulator = new SonarSimulator(1,1,filename);
    simulator->openFile();
    simulator->writeLine();
    simulator->closeFile();
    std::ifstream inFile("outTest");
    REQUIRE(inFile);
    std::string row;
    REQUIRE(std::getline(inFile,row));
    char ID[3];
    double depthFeet;
    double depthMeter;
    double depthFathoms;
    char checksum[3];
    REQUIRE(sscanf(row.c_str(),"$%2sDBT,%lf,f,%lf,M,%lf,F*%2s\x0d\x0a",ID,&depthFeet,&depthMeter,&depthFathoms,checksum)==5);
}

TEST_CASE("test the frequence")
{
    SonarSimulator *simulator;
    std::string filename = "outTest";
    simulator = new SonarSimulator(16,1,filename);
    simulator->openFile();
    double time = 0;
    timeval start;
    timeval stop;
    while(time < 1)
    {
        gettimeofday(&start,NULL);
        simulator->writeLine();
        gettimeofday(&stop,NULL);
        time = time+ (double) ((stop.tv_sec*1000000+stop.tv_usec)-(start.tv_sec*1000000+start.tv_usec))/1000000.0;
    }
    simulator->closeFile();
    std::ifstream inFile("outTest");
    REQUIRE(inFile);
    std::string row;
    int rowCount = 0;
    while(std::getline(inFile,row))
    {
        rowCount = rowCount+1;
    }
    REQUIRE(rowCount==16);
}