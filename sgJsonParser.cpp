#include "sgJsonParser.h"
#include <fstream>
#include <sstream>

sgJsonParser::sgJsonParser(void)
: mSuccess(false), mErrorMsg("")
{
}


sgJsonParser::~sgJsonParser(void)
{
}

sgVar sgJsonParser::parse( const std::string &filename )
{
	mSuccess = true;
	mErrorMsg = "";

	std::ifstream in(filename.c_str());
	if(in.fail())
	{
		mSuccess = false;
		mErrorMsg = std::string("open file failed, ") + filename;
		return sgVar();
	}

	return parse(in);
}

sgVar sgJsonParser::parse( std::istream &in )
{
	mSuccess = true;
	mErrorMsg = "";

	try{
		char ch = 0 ;
		while(in>>ch)
		{
			switch(ch)
			{
			case '{':
				return parseMap(in);
			case '[':
				return parseArray(in);
			case ' ':
			case '\r':
			case '\n':
			case '\t':
				break;
			default:
				throw sgJsonException("unknown format, please start with '{' or '['");
			}
		}
	}catch(const std::exception &e)
	{
		mSuccess = false;
		mErrorMsg = e.what();
	}

	return sgVar();

}

sgVar sgJsonParser::parseMap( std::istream &in )
{
	sgVar var;
	var.makeMap();

	char ch = 0 ;
	while(in>>ch)
	{
		if(needTrim(ch))
		{
			continue;
		}
		else if(ch == '\"'
			|| ch == '\'')
		{
			// key
			sgVar key = parseString(in, ch);
			while(in>>ch)
			{
				if(needTrim(ch))
				{
					continue;
				}
				else if(ch == ':')
				{
					// value
					sgVar v = parseValue(in);
					var[key.toString()] = v;
					break;
				}
				else
				{
					throw sgJsonException(std::string("Expected ':' to start map value for key '") + key.toString() + "'" ) ;
				}
			}
		}
		else if(ch == ',')
		{
			;
		}
		else if(ch == '}')
		{
			// end of map
			return var;
		}
		else
		{
			throw sgJsonException("Expected a string key for a map value");
		}
	}

	throw sgJsonException("map no end");
}

sgVar sgJsonParser::parseArray( std::istream &in )
{
	sgVar var;
	var.makeArray(0);

	bool askForComma = false;
	char ch = 0 ;
	while(in>>ch)
	{
		if(needTrim(ch))
		{
			continue;
		}
		else if(ch == ',')
		{
			if(!askForComma)
			{
				throw sgJsonException("Expected a value in array");
			}
			else
			{
				askForComma = false;
			}
		}
		else if(ch == ']')
		{
			// end of array
			return var;
		}
		else
		{
			if(askForComma)
			{
				throw sgJsonException("Expected ',' before parsing value in array");
			}
			in.seekg(-1, std::ios_base::cur);
			var[var.size()] = parseValue(in);
			askForComma = true;
		}
	}
	
	throw sgJsonException("array no end");
}

sgVar sgJsonParser::parseValue( std::istream &in )
{
	char ch = 0 ;
	std::stringstream ss;
	while(in>>ch)
	{
		if(needTrim(ch))
		{
			continue;
		}
		else if(ch == '\"'
			|| ch == '\'')
		{
			return parseString(in, ch);
		}
		else if(ch == '{')
		{
			return parseMap(in);
		}
		else if(ch == '[')
		{
			return parseArray(in);
		}
		else if(ch == ',' || ch == ']' || ch == '}')
		{
			// end value, seek back 
			in.seekg(-1, std::ios_base::cur);

			std::string bl = ss.str();
			if(bl == "true")
			{
				return sgVar(true);
			}
			else if(bl == "false")
			{
				return sgVar(false);
			}
			else if(bl == "null")
			{
				return sgVar();
			}
			else
			{
				double num;
				ss >> num;
				return sgVar(num);
			}
		}
		else
		{
			ss << ch;
		}
	}
	throw sgJsonException("invalid value");
}

sgVar sgJsonParser::parseString( std::istream &in, char start)
{
	sgVar var;
	std::string str;
	char ch = 0;
	while(in>>ch)
	{
		if(ch == start)
		{
			// end
			var = str.c_str();
			return var;
		}
		else
		{
			str += ch;
		}
	}

	// error
	throw sgJsonException("string must end with ' or \"");
}

bool sgJsonParser::needTrim( char ch )
{
	if(ch == ' '
		|| ch == '\r'
		|| ch == '\n'
		|| ch == '\t')
		return true;
	return false;
}

