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
    int offset;
public:
	sgJsonException(const std::string &str) throw()
	: msg(str), offset(0)
	{
	}

    sgJsonException(const std::string &str, int off) throw()
        : msg(str), offset(off)
    {
    }
	
	~sgJsonException() throw()
	{

	}

	const char *what() const throw()
	{
        static char str[256] = "";
        sprintf(str, "[Error] %s, with offset: %d", msg.c_str(), offset);
		return str;
	}
};

struct sgJsonStringStream {

    sgJsonStringStream(const char *src) : src_(src), head_(src) {}

    char Peek() const { return *src_; }
    char Take() { return *src_++; }
    int Tell() const { return src_ - head_; }

    const char* src_;		//!< Current read position.
    const char* head_;	//!< Original head of the string.
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


    sgVar parse(const char *str);

	const std::string &getErrorMsg(void) const{ return mErrorMsg; }
	bool fail(void) const{ return !mSuccess; }

protected:
	bool needTrim(char ch);

	sgVar parseMap(std::istream &in);
	sgVar parseArray(std::istream &in);
	sgVar parseValue(std::istream &in);
	// should start with ' or "
	sgVar parseString(std::istream &in, char start);


    //////////////////////////////////////////////////////////////////////////
    sgVar parseObject(sgJsonStringStream &in);
    sgVar parseArray(sgJsonStringStream &in);
    sgVar parseValue(sgJsonStringStream &in);
    sgVar parseString(sgJsonStringStream &in);

    sgVar parseNull(sgJsonStringStream &in);
    sgVar parseTrue(sgJsonStringStream &in);
    sgVar parseFalse(sgJsonStringStream &in);
    sgVar parseNumber(sgJsonStringStream &in);

};

#endif // __SGJSONPARSER_H__
