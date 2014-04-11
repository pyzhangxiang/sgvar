#include "sgJsonParser.h"
#include <fstream>
#include <sstream>

namespace internal {

    //! Computes integer powers of 10 in double (10.0^n).
    /*! This function uses lookup table for fast and accurate results.
	    \param n positive/negative exponent. Must <= 308.
	    \return 10.0^n
    */
    inline double Pow10(int n) {
	    static const double e[] = { // 1e-308...1e308: 617 * 8 bytes = 4936 bytes
		    1e-308,1e-307,1e-306,1e-305,1e-304,1e-303,1e-302,1e-301,1e-300,
		    1e-299,1e-298,1e-297,1e-296,1e-295,1e-294,1e-293,1e-292,1e-291,1e-290,1e-289,1e-288,1e-287,1e-286,1e-285,1e-284,1e-283,1e-282,1e-281,1e-280,
		    1e-279,1e-278,1e-277,1e-276,1e-275,1e-274,1e-273,1e-272,1e-271,1e-270,1e-269,1e-268,1e-267,1e-266,1e-265,1e-264,1e-263,1e-262,1e-261,1e-260,
		    1e-259,1e-258,1e-257,1e-256,1e-255,1e-254,1e-253,1e-252,1e-251,1e-250,1e-249,1e-248,1e-247,1e-246,1e-245,1e-244,1e-243,1e-242,1e-241,1e-240,
		    1e-239,1e-238,1e-237,1e-236,1e-235,1e-234,1e-233,1e-232,1e-231,1e-230,1e-229,1e-228,1e-227,1e-226,1e-225,1e-224,1e-223,1e-222,1e-221,1e-220,
		    1e-219,1e-218,1e-217,1e-216,1e-215,1e-214,1e-213,1e-212,1e-211,1e-210,1e-209,1e-208,1e-207,1e-206,1e-205,1e-204,1e-203,1e-202,1e-201,1e-200,
		    1e-199,1e-198,1e-197,1e-196,1e-195,1e-194,1e-193,1e-192,1e-191,1e-190,1e-189,1e-188,1e-187,1e-186,1e-185,1e-184,1e-183,1e-182,1e-181,1e-180,
		    1e-179,1e-178,1e-177,1e-176,1e-175,1e-174,1e-173,1e-172,1e-171,1e-170,1e-169,1e-168,1e-167,1e-166,1e-165,1e-164,1e-163,1e-162,1e-161,1e-160,
		    1e-159,1e-158,1e-157,1e-156,1e-155,1e-154,1e-153,1e-152,1e-151,1e-150,1e-149,1e-148,1e-147,1e-146,1e-145,1e-144,1e-143,1e-142,1e-141,1e-140,
		    1e-139,1e-138,1e-137,1e-136,1e-135,1e-134,1e-133,1e-132,1e-131,1e-130,1e-129,1e-128,1e-127,1e-126,1e-125,1e-124,1e-123,1e-122,1e-121,1e-120,
		    1e-119,1e-118,1e-117,1e-116,1e-115,1e-114,1e-113,1e-112,1e-111,1e-110,1e-109,1e-108,1e-107,1e-106,1e-105,1e-104,1e-103,1e-102,1e-101,1e-100,
		    1e-99, 1e-98, 1e-97, 1e-96, 1e-95, 1e-94, 1e-93, 1e-92, 1e-91, 1e-90, 1e-89, 1e-88, 1e-87, 1e-86, 1e-85, 1e-84, 1e-83, 1e-82, 1e-81, 1e-80, 
		    1e-79, 1e-78, 1e-77, 1e-76, 1e-75, 1e-74, 1e-73, 1e-72, 1e-71, 1e-70, 1e-69, 1e-68, 1e-67, 1e-66, 1e-65, 1e-64, 1e-63, 1e-62, 1e-61, 1e-60, 
		    1e-59, 1e-58, 1e-57, 1e-56, 1e-55, 1e-54, 1e-53, 1e-52, 1e-51, 1e-50, 1e-49, 1e-48, 1e-47, 1e-46, 1e-45, 1e-44, 1e-43, 1e-42, 1e-41, 1e-40, 
		    1e-39, 1e-38, 1e-37, 1e-36, 1e-35, 1e-34, 1e-33, 1e-32, 1e-31, 1e-30, 1e-29, 1e-28, 1e-27, 1e-26, 1e-25, 1e-24, 1e-23, 1e-22, 1e-21, 1e-20, 
		    1e-19, 1e-18, 1e-17, 1e-16, 1e-15, 1e-14, 1e-13, 1e-12, 1e-11, 1e-10, 1e-9,  1e-8,  1e-7,  1e-6,  1e-5,  1e-4,  1e-3,  1e-2,  1e-1,  1e+0,  
		    1e+1,  1e+2,  1e+3,  1e+4,  1e+5,  1e+6,  1e+7,  1e+8,  1e+9,  1e+10, 1e+11, 1e+12, 1e+13, 1e+14, 1e+15, 1e+16, 1e+17, 1e+18, 1e+19, 1e+20, 
		    1e+21, 1e+22, 1e+23, 1e+24, 1e+25, 1e+26, 1e+27, 1e+28, 1e+29, 1e+30, 1e+31, 1e+32, 1e+33, 1e+34, 1e+35, 1e+36, 1e+37, 1e+38, 1e+39, 1e+40,
		    1e+41, 1e+42, 1e+43, 1e+44, 1e+45, 1e+46, 1e+47, 1e+48, 1e+49, 1e+50, 1e+51, 1e+52, 1e+53, 1e+54, 1e+55, 1e+56, 1e+57, 1e+58, 1e+59, 1e+60,
		    1e+61, 1e+62, 1e+63, 1e+64, 1e+65, 1e+66, 1e+67, 1e+68, 1e+69, 1e+70, 1e+71, 1e+72, 1e+73, 1e+74, 1e+75, 1e+76, 1e+77, 1e+78, 1e+79, 1e+80,
		    1e+81, 1e+82, 1e+83, 1e+84, 1e+85, 1e+86, 1e+87, 1e+88, 1e+89, 1e+90, 1e+91, 1e+92, 1e+93, 1e+94, 1e+95, 1e+96, 1e+97, 1e+98, 1e+99, 1e+100,
		    1e+101,1e+102,1e+103,1e+104,1e+105,1e+106,1e+107,1e+108,1e+109,1e+110,1e+111,1e+112,1e+113,1e+114,1e+115,1e+116,1e+117,1e+118,1e+119,1e+120,
		    1e+121,1e+122,1e+123,1e+124,1e+125,1e+126,1e+127,1e+128,1e+129,1e+130,1e+131,1e+132,1e+133,1e+134,1e+135,1e+136,1e+137,1e+138,1e+139,1e+140,
		    1e+141,1e+142,1e+143,1e+144,1e+145,1e+146,1e+147,1e+148,1e+149,1e+150,1e+151,1e+152,1e+153,1e+154,1e+155,1e+156,1e+157,1e+158,1e+159,1e+160,
		    1e+161,1e+162,1e+163,1e+164,1e+165,1e+166,1e+167,1e+168,1e+169,1e+170,1e+171,1e+172,1e+173,1e+174,1e+175,1e+176,1e+177,1e+178,1e+179,1e+180,
		    1e+181,1e+182,1e+183,1e+184,1e+185,1e+186,1e+187,1e+188,1e+189,1e+190,1e+191,1e+192,1e+193,1e+194,1e+195,1e+196,1e+197,1e+198,1e+199,1e+200,
		    1e+201,1e+202,1e+203,1e+204,1e+205,1e+206,1e+207,1e+208,1e+209,1e+210,1e+211,1e+212,1e+213,1e+214,1e+215,1e+216,1e+217,1e+218,1e+219,1e+220,
		    1e+221,1e+222,1e+223,1e+224,1e+225,1e+226,1e+227,1e+228,1e+229,1e+230,1e+231,1e+232,1e+233,1e+234,1e+235,1e+236,1e+237,1e+238,1e+239,1e+240,
		    1e+241,1e+242,1e+243,1e+244,1e+245,1e+246,1e+247,1e+248,1e+249,1e+250,1e+251,1e+252,1e+253,1e+254,1e+255,1e+256,1e+257,1e+258,1e+259,1e+260,
		    1e+261,1e+262,1e+263,1e+264,1e+265,1e+266,1e+267,1e+268,1e+269,1e+270,1e+271,1e+272,1e+273,1e+274,1e+275,1e+276,1e+277,1e+278,1e+279,1e+280,
		    1e+281,1e+282,1e+283,1e+284,1e+285,1e+286,1e+287,1e+288,1e+289,1e+290,1e+291,1e+292,1e+293,1e+294,1e+295,1e+296,1e+297,1e+298,1e+299,1e+300,
		    1e+301,1e+302,1e+303,1e+304,1e+305,1e+306,1e+307,1e+308
	    };
	    SGVAR_ASSERT(n <= 308);
	    return n < -308 ? 0.0 : e[n + 308];
    }

} // namespace internal

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

    // get length of file:
    in.seekg (0, in.end);
    int length = in.tellg();
    in.seekg (0, in.beg);

    // allocate memory:
    char * buffer = new char[length];

    // read data as a block:
    in.read (buffer,length);
    in.close();

    std::stringstream ss;
    ss << buffer;
    delete []buffer;

	sgVar ret = parse(ss);

    

    return ret;
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
			in.seekg(-1, in.cur);
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
			in.seekg(-1, in.cur);
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
 //   str.reserve(256);
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
//			str += ch;
            str.push_back(ch);
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

void SkipWhitespace(sgJsonStringStream &stream) {
    sgJsonStringStream s = stream;	// Use a local copy for optimization
    char ch = s.Peek();
    while (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t')
    {
        s.Take();
        ch = s.Peek();
    }
    stream = s;
}

sgVar sgJsonParser::parse( const char *str )
{
    sgJsonStringStream stream(str);

    mSuccess = true;
    mErrorMsg = "";

    SkipWhitespace(stream);

    try
    {
        char ch = stream.Peek();
        if(ch == '\0')
        {
            throw sgJsonException("Text only contains white space(s)", stream.Tell());
        }
        else
        {
            switch(ch)
            {
            case '{':
                return parseObject(stream);
            case '[':
                return parseArray(stream);
            default:
                throw sgJsonException("Unknown format, please start with '{' or '['", stream.Tell());
            }
            
            SkipWhitespace(stream);
            
            if (stream.Peek() != '\0')
            {
                throw sgJsonException("Nothing should follow the root object or array.", stream.Tell());
            }
        }
    }
    catch(const std::exception &e)
    {
        mSuccess = false;
        mErrorMsg = e.what();
    }

    return sgVar::NullVar;
    

}

sgVar sgJsonParser::parseObject( sgJsonStringStream &in )
{
    SGVAR_ASSERT(in.Peek() == '{');
    in.Take();  // skip '{'
    SkipWhitespace(in);

    sgVar var;
    var.makeMap();

    char ch = in.Peek();
    if (ch == '}')
    {
        // empty object
        in.Take();
        return var;
    }

    while(true)
    {
        ch = in.Peek();

        if(ch != '\"' && ch != '\'')
        {
            throw sgJsonException("Name of an object member must be a string.", in.Tell());
            break;
        }

        sgVar key = parseString(in);
        SkipWhitespace(in);

        ch = in.Take();
        if(ch != ':')
        {
            throw sgJsonException("There must be a colon after the name of object member.", in.Tell());
            break;
        }

        SkipWhitespace(in);

        sgVar v = parseValue(in);
        var[key.toString()] = v;
        
        SkipWhitespace(in);

        ch = in.Take();
        switch(ch)
        {
        case ',':
            {
                SkipWhitespace(in);
                break;
            }
        case '}':
            {
                // end of object
                return var;
            }
        default:
            {
                throw sgJsonException("Must be a comma or '}' after an object member.", in.Tell());
            }
        }
    }

    throw sgJsonException("The object does not end with '}'.", in.Tell());
    return sgVar::NullVar;
}

sgVar sgJsonParser::parseArray(sgJsonStringStream &in)
{
    SGVAR_ASSERT(in.Peek() == '[');
    in.Take();  // skip '['
    SkipWhitespace(in);

    sgVar var;
    var.makeArray(0);

    char ch = in.Peek();
    if (ch == ']')
    {
        // empty Array
        in.Take();
        return var;
    }

    size_t elementCount = 0;
    while(true)
    {
        var[elementCount] = parseValue(in);
        ++elementCount;

        SkipWhitespace(in);

        ch = in.Take();
        switch(ch)
        {
        case ',':
            {
                SkipWhitespace(in);
                break;
            }
        case ']':
            {
                // end of array
                return var;
            }
        default:
            {
                throw sgJsonException("Must be a comma or ']' after an array element.", in.Tell());
            }
        }
    }

    throw sgJsonException("The array does not end with ']'.", in.Tell());
    return sgVar::NullVar;
}

sgVar sgJsonParser::parseValue(sgJsonStringStream &in)
{
    char ch = in.Peek();
    switch(ch)
    {
    case '\"':
    case '\'':
        return parseString(in);
    case '{':
        return parseObject(in);
    case '[':
        return parseArray(in);
    case 'n':
        return parseNull(in);
    case 't':
        return parseTrue(in);
    case 'f':
        return parseFalse(in);
    default:
        return parseNumber(in);
    }

}

sgVar sgJsonParser::parseString(sgJsonStringStream &in)
{
    char start = in.Peek();
    SGVAR_ASSERT(start == '\"' || start == '\'');
    in.Take();  // skip

    sgVar var;
    std::string str;
    char ch = 0;
    while(true)
    {
        ch = in.Peek();
        if(ch == '\0')
        {
            break;            
        }
        else if(ch == start)
        {
            in.Take();
            // end of the string
            var = str.c_str();
            return var;
        }
        else
        {
            in.Take();
            str.push_back(ch);
        }
        
    }

    // error
    throw sgJsonException("String must end with ' or \"", in.Tell());
    return sgVar::NullVar;
}

sgVar sgJsonParser::parseNull( sgJsonStringStream &in )
{
    SGVAR_ASSERT(in.Peek() == 'n');
    in.Take();

    if (in.Take() == 'u' && in.Take() == 'l' && in.Take() == 'l')
    {
        return sgVar::NullVar;
    }

    throw sgJsonException("Invalid value.", in.Tell() - 1);
}

sgVar sgJsonParser::parseTrue( sgJsonStringStream &in )
{
    SGVAR_ASSERT(in.Peek() == 't');
    in.Take();

    if (in.Take() == 'r' && in.Take() == 'u' && in.Take() == 'e')
    {
        return sgVar(true);
    }

    throw sgJsonException("Invalid value.", in.Tell() - 1);
}

sgVar sgJsonParser::parseFalse( sgJsonStringStream &in )
{
    SGVAR_ASSERT(in.Peek() == 'f');
    in.Take();

    if (in.Take() == 'a' && in.Take() == 'l' && in.Take() == 's' && in.Take() == 'e')
    {
        return sgVar(false);
    }

    throw sgJsonException("Invalid value.", in.Tell() - 1);
}

sgVar sgJsonParser::parseNumber( sgJsonStringStream &in )
{
    sgJsonStringStream s = in; // Local copy for optimization

    
    // Parse minus
    bool minus = false;
    if (s.Peek() == '-') {
        minus = true;
        s.Take();
    }

    // Parse int: zero / ( digit1-9 *DIGIT )
    unsigned i;
    bool try64bit = false;
    if (s.Peek() == '0') {
        i = 0;
        s.Take();
    }
    else if (s.Peek() >= '1' && s.Peek() <= '9') {
        i = s.Take() - '0';

        if (minus)
            while (s.Peek() >= '0' && s.Peek() <= '9') {
                if (i >= 214748364) { // 2^31 = 2147483648
                    if (i != 214748364 || s.Peek() > '8') {
                        try64bit = true;
                        break;
                    }
                }
                i = i * 10 + (s.Take() - '0');
            }
        else
            while (s.Peek() >= '0' && s.Peek() <= '9') {
                if (i >= 429496729) { // 2^32 - 1 = 4294967295
                    if (i != 429496729 || s.Peek() > '5') {
                        try64bit = true;
                        break;
                    }
                }
                i = i * 10 + (s.Take() - '0');
            }
    }
    else {
        throw sgJsonException("Expect a value here.", in.Tell());
    }

    // Parse 64bit int
    unsigned long long i64 = 0;
    bool useDouble = false;
    if (try64bit) {
        i64 = i;
        if (minus) 
            while (s.Peek() >= '0' && s.Peek() <= '9') {					
                if (i64 >= 922337203685477580uLL) // 2^63 = 9223372036854775808
                    if (i64 != 922337203685477580uLL || s.Peek() > '8') {
                        useDouble = true;
                        break;
                    }
                    i64 = i64 * 10 + (s.Take() - '0');
            }
        else
            while (s.Peek() >= '0' && s.Peek() <= '9') {					
                if (i64 >= 1844674407370955161uLL) // 2^64 - 1 = 18446744073709551615
                    if (i64 != 1844674407370955161uLL || s.Peek() > '5') {
                        useDouble = true;
                        break;
                    }
                    i64 = i64 * 10 + (s.Take() - '0');
            }
    }

    // Force double for big integer
    double d = 0.0;
    if (useDouble) {
        d = (double)i64;
        while (s.Peek() >= '0' && s.Peek() <= '9') {
            if (d >= 1E307) {
                throw sgJsonException("Number too big to store in double", in.Tell());
            }
            d = d * 10 + (s.Take() - '0');
        }
    }

    // Parse frac = decimal-point 1*DIGIT
    int expFrac = 0;
    if (s.Peek() == '.') {
        if (!useDouble) {
            d = try64bit ? (double)i64 : (double)i;
            useDouble = true;
        }
        s.Take();

        if (s.Peek() >= '0' && s.Peek() <= '9') {
            d = d * 10 + (s.Take() - '0');
            --expFrac;
        }
        else {
            throw sgJsonException("At least one digit in fraction part", in.Tell());
        }

        while (s.Peek() >= '0' && s.Peek() <= '9') {
            if (expFrac > -16) {
                d = d * 10 + (s.Peek() - '0');
                --expFrac;
            }
            s.Take();
        }
    }

    // Parse exp = e [ minus / plus ] 1*DIGIT
    int exp = 0;
    if (s.Peek() == 'e' || s.Peek() == 'E') {
        if (!useDouble) {
            d = try64bit ? (double)i64 : (double)i;
            useDouble = true;
        }
        s.Take();

        bool expMinus = false;
        if (s.Peek() == '+')
            s.Take();
        else if (s.Peek() == '-') {
            s.Take();
            expMinus = true;
        }

        if (s.Peek() >= '0' && s.Peek() <= '9') {
            exp = s.Take() - '0';
            while (s.Peek() >= '0' && s.Peek() <= '9') {
                exp = exp * 10 + (s.Take() - '0');
                if (exp > 308) {
                    throw sgJsonException("Number too big to store in double", in.Tell());
                }
            }
        }
        else {
            throw sgJsonException("At least one digit in exponent", s.Tell());
        }

        if (expMinus)
            exp = -exp;
    }

    // Finish parsing, call event according to the type of number.
    sgVar val;
    if (useDouble) {
        d *= internal::Pow10(exp + expFrac);
        val = d;
    }
    else {
        if (try64bit) {
            if (minus)
                val = (-(long long)i64);
            else
                val = (i64);
        }
        else {
            if (minus)
                val = (-(int)i);
            else
                val = (i);
        }
    }

    in = s; // restore stream

    return val;
}