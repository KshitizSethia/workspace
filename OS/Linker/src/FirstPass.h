/*
 * FirstPass.cpp
 *
 *  Created on: Feb 8, 2015
 *      Author: Kshitiz
 */
#ifndef FIRSTPASS_H_
#define FIRSTPASS_H_

#include <fstream>
#include <map>
#include <string>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <istream>
#include <typeinfo>

#include "Helpers.h"
#include "CountingStreamBuffer.h"

using namespace std;

class FirstPass{
public:

	FirstPass(istream &ipStream
			, CountingStreamBuffer &cBuf
			, ofstream &temp
			, map<string, int> &masterSymTable
			, vector<Module> &mods)
			: inputStream(ipStream), cBuffer(cBuf), tempOutput(temp)
	{
		masterSymbolTable = masterSymTable;
		modules = mods;
	}

	void DeserializeInput() {
		int moduleOffset = 0;

		try {
			while (true) {
				Module module = parseModule(moduleOffset);
				moduleOffset += module.size;
				modules.push_back(module);
			}
		} catch (LinkerException& e) {
			if (e.type != NoMoreModulesToRead)
				{ cout<<e.description; exit(EXIT_SUCCESS);}

		}

		//Rule 5
		for(unsigned warningNum = 0; warningNum<warnings.size(); warningNum++)
		{
			cout<<warnings[warningNum]<<endl;
		}
		warnings.clear();
	}

	istream& inputStream;
	CountingStreamBuffer& cBuffer;
	map<string, int> masterSymbolTable;
	vector<Module> modules;
	map<string, bool> multipleDefs;
	vector<string> warnings;
	ofstream& tempOutput;

private:
	Module parseModule(int moduleOffset)
	{
		Module module;
		map<string, int> moduleDefs = parseDefs(moduleOffset);
		module.useList = parseUseList();
		module.size = parseProgram(moduleOffset);
		module.offset = moduleOffset;

		//Rule 5
		int moduleSize = module.size;
		for(map<string, int>::iterator iter = moduleDefs.begin();
				iter != moduleDefs.end(); ++iter)
		{
			string symbolName = string(iter->first);
			if(moduleDefs[symbolName] > moduleSize)
			{
				int moduleNum = modules.size()+1;
				char buffer[100];
				sprintf(buffer, StringStore::SymbolOffsetOutsideModule.c_str()
						, moduleNum, symbolName.c_str(), moduleDefs[symbolName], moduleSize-1);
				masterSymbolTable[symbolName] = 0;
				warnings.push_back(string(buffer));
			}
		}

		return module;
	}

	map<string, int> parseDefs
		(int moduleOffset)
	{
		unsigned inputSize;
		int numDefs;

		if(!tryFill(numDefs, inputSize) ||!inputStream)
			{
			if(inputStream.eof())//No more modules present
				throw LinkerException(NoMoreModulesToRead);
			else//invalid syntax of a module
				throw LinkerException(SyntaxError,
						GetSyntaxErrorDetails(NUM_EXPECTED, inputSize));
			}
		if(numDefs>16) throw LinkerException(SyntaxError
				, GetSyntaxErrorDetails(TO_MANY_DEF_IN_MODULE, inputSize));

		map<string, int> moduleDefs;

		for(int def=0;def<numDefs;def++)
		{
			string varName;
			int varAddress;

			inputStream >> varName;
			if(!inputStream) throw LinkerException(SyntaxError,
					GetSyntaxErrorDetails(SYM_EXPECTED, varName.size()));
			VerifyVarName(varName);

			//inputStream >> varAddress;
			if(!tryFill(varAddress, inputSize) || !inputStream)
				throw LinkerException(SyntaxError,
					GetSyntaxErrorDetails(NUM_EXPECTED, inputSize));

			if(masterSymbolTable.find(varName) == masterSymbolTable.end())
			{
				masterSymbolTable[varName] = varAddress+moduleOffset;
				multipleDefs[varName] = false;
				moduleDefs[varName] = varAddress;
			}
			else
			{ multipleDefs[varName] = true; }
		}

		return moduleDefs;
	}

	vector<string> parseUseList()
	{
		unsigned inputSize;
		int numUses;
		//inputStream >> numUses;
		if(!tryFill(numUses, inputSize) || !inputStream) throw LinkerException(SyntaxError
				, GetSyntaxErrorDetails(NUM_EXPECTED, inputSize));
		if(numUses>16) throw LinkerException(SyntaxError
				, GetSyntaxErrorDetails(TO_MANY_USE_IN_MODULE, inputSize));

		vector<string> useList(numUses);

		for(int index=0; index<numUses;index++)
		{
			string buffer;
			inputStream>> buffer;
			if(!inputStream) throw LinkerException(SyntaxError,
					GetSyntaxErrorDetails(SYM_EXPECTED, buffer.size()));
			useList[index] = buffer;
			VerifyVarName(buffer);
		}

		return useList;
	}

	int parseProgram(int moduleOffset)
	{
		unsigned inputSize;
		int numInstructions;
		//inputStream >> numInstructions;
		if(!tryFill(numInstructions, inputSize) || !inputStream)
			throw LinkerException(SyntaxError,
				GetSyntaxErrorDetails(NUM_EXPECTED, inputSize));
		if(numInstructions+moduleOffset>512) throw LinkerException(SyntaxError
						, GetSyntaxErrorDetails(TO_MANY_INSTR, inputSize));


		for(int index=0;index<numInstructions;index++)
		{
			char type;
			int data;
			//inputStream >> type;
			if(!tryFill(type, inputSize) || !inputStream)
				throw LinkerException(SyntaxError
					, GetSyntaxErrorDetails(ADDR_EXPECTED, inputSize));

			//inputStream >> data;
			if(!tryFill(data, inputSize) || !inputStream)
				throw LinkerException(SyntaxError
					, GetSyntaxErrorDetails(NUM_EXPECTED, inputSize));

			tempOutput<<type<<" "<<data<<endl;
		}

		return numInstructions;
	}

	string GetSyntaxErrorDetails(int errorNum, unsigned lastReadSize)
	{
		ostringstream str;
		unsigned column = cBuffer.column();
		if(column!=0)
		{
			str <<"Parse Error line "<<cBuffer.lineNumber()<<" offset "
					<<column-lastReadSize+1
				<<": "<<SyntaxErrorArray[errorNum];
		}
		else
		{
			str <<"Parse Error line "<<cBuffer.prevLineNumber()<<" offset "
					<<cBuffer.prev_column()
					<<": "<<SyntaxErrorArray[errorNum];
		}
		return str.str();
	}

	void VerifyVarName(string varName)
	{
		char firstChar = varName.at(0);
		if(not((firstChar>64 && firstChar<91) || (firstChar>96 && firstChar<123)))
			throw LinkerException(SyntaxError, GetSyntaxErrorDetails(SYM_EXPECTED, varName.size()));
		if(varName.size()>16)
			throw LinkerException(SyntaxError, GetSyntaxErrorDetails(SYM_TOLONG, varName.size()));
	}

	template<typename T>
	bool tryFill(T& result, unsigned& inputSize)
	{
		string holder;
		inputStream >> holder;
		//if(!inputStream) { inputSize = holder.size(); return false; }
		stringstream sstream(holder);

		//When filling character check if we got a string instead
		if(typeid(T) == typeid(char) && holder.size()>1)
		{ inputSize = holder.size(); return false; }

		sstream>>result;
		if(!sstream) { inputSize = holder.size(); return false; }

		inputSize = holder.size();
		return true;
	}
};

#endif /* FIRSTPASS_H_ */
