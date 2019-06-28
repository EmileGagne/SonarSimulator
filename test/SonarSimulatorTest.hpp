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

TEST_CASE("test ...")
{
    SonarSimulator *s;
}