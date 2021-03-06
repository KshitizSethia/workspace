/*
 * Helpers.h
 *
 *  Created on: Feb 8, 2015
 *      Author: Kshitiz
 */
#ifndef HELPERS_H_
#define HELPERS_H_

#include <map>
#include <array>
#include <vector>
#include <string>
#include <exception>
#include <sstream>

using namespace std;

class Instruction {
public:
	char type;
	unsigned data;
	Instruction(char type, int data)
		{ this->type = type; this->data = data; }
};

class Module {
public:
	unsigned offset;
	vector<string> useList;
	unsigned size;
};

enum ExceptionType{
	NoMoreModulesToRead,
	SyntaxError//Rule 1
};

enum SyntaxErrorType{
	NUM_EXPECTED=0, // Number expect
	SYM_EXPECTED, // Symbol Expected
	ADDR_EXPECTED, // Addressing Expected
	SYM_TOLONG, // Symbol Name is to long
	TO_MANY_DEF_IN_MODULE, // > 16
	TO_MANY_USE_IN_MODULE, // > 16
	TO_MANY_INSTR, // total num_instr exceeds memory size (512)
};

static string SyntaxErrorArray[] = {
"NUM_EXPECTED", // Number expect
"SYM_EXPECTED", // Symbol Expected
"ADDR_EXPECTED", // Addressing Expected
"SYM_TOLONG", // Symbol Name is to long
"TO_MANY_DEF_IN_MODULE", // > 16
"TO_MANY_USE_IN_MODULE", // > 16
"TO_MANY_INSTR", // total num_instr exceeds memory size (512)
};

class LinkerException: public exception
{
public:
	ExceptionType type;
	string description;
	LinkerException(ExceptionType type)
	{ this->type = type; }

	LinkerException(ExceptionType type, string desc)
	{
		this->type = type;
		this->description = desc;
	}

	~LinkerException() throw()
	{}
};

class StringStore
{public:
	static const string DuplicateDef;
	static const string SymbolNotDefined;
	static const string DefinedSymbolNotUsed;
	static const string SymbolOffsetOutsideModule;
	static const string ExternalAddressOutOfUseList;
	static const string UseListSymbolNotUsed;
	static const string RelativeAddressExceedsModule;
	static const string ImmediateAddressTooLarge;
	static const string IllegalOpcode;
	static const string AbsoluteAddrExceedsMachineSize;
};
const string StringStore::DuplicateDef = "Error: This variable is multiple times defined; first value used";
const string StringStore::SymbolNotDefined = "Error: %s is not defined; zero used";
const string StringStore::DefinedSymbolNotUsed = "Warning: Module %d: %s was defined but never used";
const string StringStore::SymbolOffsetOutsideModule = "Warning: Module %d: %s to big %d (max=%d) assume zero relative";
const string StringStore::ExternalAddressOutOfUseList = "Error: External address exceeds length of uselist; treated as immediate";
const string StringStore::UseListSymbolNotUsed = "Warning: Module %d: %s appeared in the uselist but was not actually used";
const string StringStore::RelativeAddressExceedsModule = "Error: Relative address exceeds module size; zero used";
const string StringStore::ImmediateAddressTooLarge = "Error: Illegal immediate value; treated as 9999";
const string StringStore::IllegalOpcode = "Error: Illegal opcode; treated as 9999";
const string StringStore::AbsoluteAddrExceedsMachineSize = "Error: Absolute address exceeds machine size; zero used";

#endif /*HELPERS_H_*/
