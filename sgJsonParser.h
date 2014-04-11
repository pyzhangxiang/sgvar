#ifndef __SGJSONPARSER_H__
#define __SGJSONPARSER_H__

#include "sgVar.h"
#include <string>
#include <vector>
#include <iostream>
#include <exception>

class sgJsonException : public std::exception
{
private:
	std::string msg;

public:
	sgJsonException(const std::string &str) throw()
	: msg(str)
	{
	}
	
	~sgJsonException() throw()
	{

	}

	const char *what() const throw()
	{
		return msg.c_str();
	}
};

class sgJsonParser
{
private:
	std::string mErrorMsg;
	bool mSuccess;

public:
	sgJsonParser(void);
	~sgJsonParser(void);

	sgVar parse(const std::string &filename);
	sgVar parse(std::istream &in);

	const std::string &getErrorMsg(void) const{ return mErrorMsg; }
	bool fail(void) const{ return !mSuccess; }

protected:
	bool needTrim(char ch);

	sgVar parseMap(std::istream &in);
	sgVar parseArray(std::istream &in);
	sgVar parseValue(std::istream &in);
	// should start with ' or "
	sgVar parseString(std::istream &in, char start);

};

#endif // __SGJSONPARSER_H__
