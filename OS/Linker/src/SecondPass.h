/*
 * SecondPansss.h
 *
 *  Created on: Feb 8, 2015
 *      Author: Kshitiz
 */

#ifndef SECONDPASS_H_
#define SECONDPASS_H_

#include <cstdio>
#include <fstream>
#include <map>
#include <vector>
#include <string>

using namespace std;

class SecondPass{
public:

	map<string, int> masterSymbolTable;
	map<string, bool> multipleDefs;
	vector<Module> modules;
	ifstream& tempIn;

	SecondPass(
			ifstream& temp
			, map<string, int> masterSymbolTable
			, map<string, bool> multipleDefs
			, vector<Module>& modules): tempIn(temp)
	{
		this->masterSymbolTable = masterSymbolTable;
		this->multipleDefs = multipleDefs;
		this->modules = modules;
	}

	void PrintToConsole()
	{
		//Rule 4
		map<string, bool> definedSymbolUsed;
		for (map<string, int>::iterator iter = masterSymbolTable.begin();
						iter != masterSymbolTable.end(); ++iter)
		{ definedSymbolUsed[iter->first] = false; }

		PrintSymbolTable();
		cout << endl;

		PrintMemoryMap(definedSymbolUsed);
		cout<<endl;
		//Rule 4
		WarnForUnusedDefinedSymbols(definedSymbolUsed);
	}

private:
	void HandleRelative(Instruction& ins, const Module& module,
			unsigned instructionNum) {
		{
			ins.data += module.offset;
			//Rule 11
			bool illegalOpcode = false;
			if (ins.data >= 10000) {
				ins.data = 9999;
				illegalOpcode = true;
			}

			//Rule 9
			unsigned operand = ins.data % 1000;
			bool relativeAddrExceedsModule = false;
			if (!illegalOpcode
					&& operand > module.size+module.offset) {
				relativeAddrExceedsModule = true;
				ins.data -= operand-module.offset;
			}

			printf("%03d: %04d", instructionNum, ins.data);
			if (illegalOpcode)
				cout << " " << StringStore::IllegalOpcode;
			else if (relativeAddrExceedsModule)
				cout << " " << StringStore::RelativeAddressExceedsModule;
		}
		cout << endl;
	}

	void HandleImmediate(Instruction& ins, unsigned instructionNum) {
		//Rule 10
		bool illegalImmediateAddress = false;
		if (ins.data >= 10000) {
			ins.data = 9999;
			illegalImmediateAddress = true;
		}
		printf("%03d: %04d", instructionNum, ins.data);
		if (illegalImmediateAddress)
			cout << " " << StringStore::ImmediateAddressTooLarge;

		cout << endl;
	}

	void HandleAbsolute(Instruction& ins, unsigned instructionNum) {
		//Rule 11
		bool illegalOpcode = false;
		if (ins.data >= 10000) {
			ins.data = 9999;
			illegalOpcode = true;
		}
		//Rule 8
		bool absoluteAddrExceedsMachineSize = false;
		unsigned opcode = ins.data % 1000;
		if (opcode > 512) {
			ins.data = ins.data - opcode;
			absoluteAddrExceedsMachineSize = true;
		}
		printf("%03d: %04d", instructionNum, ins.data);
		if (illegalOpcode)
			cout << " " << StringStore::IllegalOpcode;
		else if (absoluteAddrExceedsMachineSize)
			cout << " " << StringStore::AbsoluteAddrExceedsMachineSize;

		cout << endl;
	}

	void HandleExternal(
			unsigned instructionData
			, int instructionNum
			, vector<string>& useList
			, vector<bool>& useListSymbolUsed
			, map<string, bool>& definedSymbolUsed)
	{
		//Rule 11:
		bool illegalOpcode = false;
		if (instructionData >= 10000) {
			instructionData = 9999;
			illegalOpcode = true;
		}

		unsigned operand = instructionData % 1000;
		if(illegalOpcode)
		{
			printf("%03d: %04d", instructionNum, instructionData);
			cout << " " << StringStore::IllegalOpcode << endl;
		}
		//Rule 6:
		else if (!illegalOpcode && operand > useList.size() - 1) {
			printf("%03d: %04d", instructionNum, instructionData);
			cout << " " << StringStore::ExternalAddressOutOfUseList << endl;
		} else {
			instructionData = instructionData - operand;
			string symbolName = useList[operand];
			useListSymbolUsed[operand] = true;
			//Rule 3:
			bool symbolFound = false;
			if (masterSymbolTable.find(symbolName) != masterSymbolTable.end()) {
				instructionData += masterSymbolTable[symbolName];
				symbolFound = true;
				definedSymbolUsed[symbolName] = true;
			}
			printf("%03d: %04d", instructionNum, instructionData);
			if (!symbolFound) {
				cout << " ";
				printf(StringStore::SymbolNotDefined.c_str(),
						symbolName.c_str());
			}
			cout << endl;
		}

	}

	void WarnForUnusedDefinedSymbols(map<string, bool>& definedSymbolUsed)
	{
		//Rule 4
		for (map<string, int>::iterator iter = masterSymbolTable.begin();
				iter != masterSymbolTable.end(); ++iter) {
			if (!definedSymbolUsed[iter->first]) {
				unsigned symbolModule;
				unsigned symbolAddr = masterSymbolTable[iter->first];
				for (unsigned moduleNum = 0; moduleNum < modules.size();
						moduleNum++) {
					Module module = modules[moduleNum];
					if (symbolAddr >= module.offset
							&& symbolAddr
									< module.offset
											+ module.size) {
						symbolModule = moduleNum + 1;
						break;
					}
				}
				printf(StringStore::DefinedSymbolNotUsed.c_str(), symbolModule,
						iter->first.c_str());
				cout << endl;
			}
		}
	}

	void WarnForUnusedUseListSymbols(const Module& module,
			vector<bool>& useListSymbolUsed, unsigned moduleNum) {
		//Rule 7:
		for (unsigned index = 0; index < module.useList.size(); index++) {
			if (!useListSymbolUsed[index]) {
				//char buffer[100];
				printf(StringStore::UseListSymbolNotUsed.c_str(), moduleNum + 1,
						module.useList[index].c_str());
				cout << endl;
				//warnings.push_back(string(buffer));
			}
		}
	}

	void PrintSymbolTable() {
		//vector<string> warnings;
		cout << "Symbol Table" << endl;
		for (map<string, int>::iterator iter = masterSymbolTable.begin();
				iter != masterSymbolTable.end(); ++iter) {
			cout << iter->first << "=" << iter->second;
			//Rule 2
			if (multipleDefs[iter->first])
				cout << " " << StringStore::DuplicateDef;

			cout << endl;
		}
	}

	void PrintMemoryMap(map<string, bool>& definedSymbolUsed)
	{
		cout << "Memory Map" << endl;
		unsigned instructionNum = 0;
		for (unsigned moduleNum = 0; moduleNum < modules.size(); moduleNum++) {
			Module module = modules[moduleNum];
			//Rule 7
			vector<bool> useListSymbolUsed(module.useList.size(), false);

			for (unsigned instr = 0; instr < module.size; instr++)
			{
				char type; int data;
				tempIn>>type>>data;
				Instruction ins(type, data);
				switch (ins.type) {
				case 'R':
					HandleRelative(ins, module, instructionNum);
					break;
				case 'I':
					HandleImmediate(ins, instructionNum);
					break;
				case 'A':
					HandleAbsolute(ins, instructionNum);
					break;
				case 'E':
					HandleExternal(ins.data, instructionNum
							, module.useList, useListSymbolUsed
							, definedSymbolUsed);
					break;
				}
				instructionNum++;
			}
			//Rule 7:
			WarnForUnusedUseListSymbols(module, useListSymbolUsed, moduleNum);
		}
	}
};

#endif /* SECONDPASS_H_ */
