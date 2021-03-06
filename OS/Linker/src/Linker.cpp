//============================================================================
// Name        : Linker.cpp
// Author      : Kshitiz Sethia
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <istream>
#include <cstdio>

#include "FirstPass.h"
#include "SecondPass.h"
#include "Helpers.h"
#include "CountingStreamBuffer.h"

using namespace std;

int main(int argc, char * argv[]) {

	//open required file
	ifstream inputFile(argv[1]);
	CountingStreamBuffer cBuffer(inputFile.rdbuf());
	istream inputStream(&cBuffer);

	if(inputFile.is_open())
	{
		//Objects to store the deserialized input
		map<string, int> masterSymbolTable;
		vector<Module> modules;
		//Temp store for instructions
		string tempFile = "temp";
		ofstream tempOut(tempFile);

		//read and parse input
		FirstPass passOne(inputStream, cBuffer, tempOut, masterSymbolTable, modules);
		passOne.DeserializeInput();

		//second pass
		inputFile.close();
		tempOut.close();
		ifstream tempIn(tempFile);

		SecondPass passTwo(
				tempIn
				, passOne.masterSymbolTable
				, passOne.multipleDefs
				, passOne.modules);
		passTwo.PrintToConsole();
		tempIn.close();
		remove(tempFile.c_str());
	}
	else
	{cout<< "Expected argument after options";}

	return 0;
}
