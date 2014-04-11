#include "sgVar.h"
#include <sstream>
#include <cassert>
//#include <exception>

sgVar sgVar::NullVar;

void sgVar::_release( void )
{
    if(mData)
    {
        switch(mDataType)
        {
        case T_BOOL:
            {
                delete (bool*)mData;
                break;
            }
        case T_FLOAT:
            {
                delete (float_type*)mData;
                break;
            }
        case T_INT:
            {
                delete (int_type*)mData;
                break;
            }
        case T_INT64:
            {
                delete (int64_type*)mData;
                break;
            }
        case T_STRING:
            {
                delete (char*)mData;
                break;
            }

        case T_ARRAY:
            {
                delete (array_type*)mData;
                break;
            }

        case T_MAP:
            {
                delete (map_type*)mData;
                break;
            }
		default:
            {
			    break;
            }
        }
    }
    mData = 0;
    mDataType = T_NULL;

    if(_ref_count)
    {
        delete _ref_count;
        _ref_count = 0;
    }

    /*if(mArray)
    {
        delete mArray;
        mArray = 0;
    }

    if(mMap)
    {
        delete mMap;
        mMap = 0;
    }*/
    
}

size_t sgVar::_increase_ref( void )
{
    if(!_ref_count)
    {
        _ref_count = new size_t;
        *_ref_count = 1;
    }
    else
    {
        ++(*_ref_count);
    }
    return *_ref_count;
}

size_t sgVar::_decrease_ref( void )
{
    if(!_ref_count)
        return 0;
    --(*_ref_count);
    size_t ret = *_ref_count;
    if(ret == 0)
    {
        _release();
    }
    return ret;
}

size_t sgVar::_count_ref( void ) const
{
    if(!_ref_count)
        return 0;
    return *_ref_count;
}

namespace internal
{
    void *data_cpy(const char *v)
    {
        if(!v)
            return 0;
        size_t length = strlen(v) + 1;
        char *ret = new char[length];
        strncpy(ret, v, length);
        return ret;
    }

    template<typename T>
    void *data_cpy(T v)
    {
        T *ret = new T;
        memcpy(ret, &v, sizeof(T));
        return ret;
    }


    template<typename T>
    sgVar::string_type tostring(const T v)
    {
        std::stringstream ss;
        ss << v;
        return sgVar::string_type(ss.str().c_str());
    }

}

void sgVar::_initInt( int_type v )
{
    mData = internal::data_cpy(v);
    if(mData)
        mDataType = T_INT;
}

void sgVar::_initInt64( int64_type v )
{
    mData = internal::data_cpy(v);
    if(mData)
        mDataType = T_INT64;
}

void sgVar::_initFloat(float_type v)
{
    mData = internal::data_cpy(v);
    if(mData)
        mDataType = T_FLOAT;
}

// destructor and constructor
sgVar::~sgVar( void )
{
    _decrease_ref();
}

sgVar::sgVar( void )
: mDataType(T_NULL), mData(0)
//, mArray(0), mMap(0)
, _ref_count(0)
{
    _increase_ref();
}


sgVar::sgVar( const char *v )
: mDataType(T_NULL), mData(0)
//, mArray(0), mMap(0)
, _ref_count(0)
{
    _increase_ref();

    mData = internal::data_cpy(v);
    if(mData)
        mDataType = T_STRING;
}

sgVar::sgVar( bool v )
: mDataType(T_NULL), mData(0)
//, mArray(0), mMap(0)
, _ref_count(0)
{
    _increase_ref();

    mData = internal::data_cpy(v);
    if(mData)
        mDataType = T_BOOL;
}

sgVar::sgVar( char v )
: mDataType(T_NULL), mData(0)
//, mArray(0), mMap(0)
, _ref_count(0)
{
    _increase_ref();
    _initInt(v);
}

sgVar::sgVar( unsigned char v )
: mDataType(T_NULL), mData(0)
//, mArray(0), mMap(0)
, _ref_count(0)
{
    _increase_ref();
    _initInt(v);
}

sgVar::sgVar( short v )
: mDataType(T_NULL), mData(0)
//, mArray(0), mMap(0)
, _ref_count(0)
{
    _increase_ref();
    _initInt(v);
}

sgVar::sgVar( unsigned short v )
: mDataType(T_NULL), mData(0)
//, mArray(0), mMap(0)
, _ref_count(0)
{
    _increase_ref();
    _initInt(v);
}

sgVar::sgVar( int v )
: mDataType(T_NULL), mData(0)
//, mArray(0), mMap(0)
, _ref_count(0)
{
    _increase_ref();
    _initInt(v);
}

sgVar::sgVar( unsigned int v )
: mDataType(T_NULL), mData(0)
//, mArray(0), mMap(0)
, _ref_count(0)
{
    _increase_ref();
    _initInt(v);
}

sgVar::sgVar( long v )
: mDataType(T_NULL), mData(0)
//, mArray(0), mMap(0)
, _ref_count(0)
{
    _increase_ref();
    _initInt(v);
}

sgVar::sgVar( unsigned long v )
: mDataType(T_NULL), mData(0)
//, mArray(0), mMap(0)
, _ref_count(0)
{
    _increase_ref();
    _initInt(v);
}

sgVar::sgVar( long long v )
: mDataType(T_NULL), mData(0)
//, mArray(0), mMap(0)
, _ref_count(0)
{
    _increase_ref();
    _initInt64(v);
}

sgVar::sgVar( unsigned long long v )
: mDataType(T_NULL), mData(0)
//, mArray(0), mMap(0)
, _ref_count(0)
{
    _increase_ref();
    _initInt64(v);
}

sgVar::sgVar( float v )
: mDataType(T_NULL), mData(0)
//, mArray(0), mMap(0)
, _ref_count(0)
{
    _increase_ref();
    _initFloat(v);
}

sgVar::sgVar( double v )
: mDataType(T_NULL), mData(0)
//, mArray(0), mMap(0)
, _ref_count(0)
{
    _increase_ref();
    _initFloat(v);
}

sgVar::sgVar( const sgVar &v )
: mDataType(T_NULL), mData(0)
//, mArray(0), mMap(0)
, _ref_count(v._ref_count)
{
    _increase_ref();

    mDataType = v.mDataType;
    mData = v.mData;
    /*mArray = v.mArray;
    mMap = v.mMap;*/
}



// assignment operators
sgVar & sgVar::operator=( const char *v )
{
    if(_decrease_ref() > 0)
    {
        _ref_count = 0;
    }
    _increase_ref();

    mData = internal::data_cpy(v);
    if(mData)
        mDataType = T_STRING;

    // delete array and map immediately?
    /*if(mArray)
    {
        delete mArray;
        mArray = 0;
    }
    if(mMap)
    {
        delete mMap;
        mMap = 0;
    }*/

    return *this;
}

sgVar & sgVar::operator=( bool v )
{
    if(_decrease_ref() > 0)
    {
        _ref_count = 0;
    }
    _increase_ref();

    mData = internal::data_cpy(v);
    if(mData)
        mDataType = T_BOOL;

    return *this;
}

sgVar & sgVar::operator=( char v )
{
    if(_decrease_ref() > 0)
    {
        _ref_count = 0;
    }
    _increase_ref();
    _initInt(v);

    return *this;
}

sgVar & sgVar::operator=( unsigned char v )
{
    if(_decrease_ref() > 0)
    {
        _ref_count = 0;
    }
    _increase_ref();
    _initInt(v);

    return *this;
}

sgVar & sgVar::operator=( short v )
{
    if(_decrease_ref() > 0)
    {
        _ref_count = 0;
    }
    _increase_ref();
    _initInt(v);

    return *this;
}

sgVar & sgVar::operator=( unsigned short v )
{
    if(_decrease_ref() > 0)
    {
        _ref_count = 0;
    }
    _increase_ref();
    _initInt(v);

    return *this;
}

sgVar & sgVar::operator=( int v )
{
    if(_decrease_ref() > 0)
    {
        _ref_count = 0;
    }
    _increase_ref();
    _initInt(v);

    return *this;
}

sgVar & sgVar::operator=( unsigned int v )
{
    if(_decrease_ref() > 0)
    {
        _ref_count = 0;
    }
    _increase_ref();
    _initInt(v);

    return *this;
}

sgVar & sgVar::operator=( long v )
{
    if(_decrease_ref() > 0)
    {
        _ref_count = 0;
    }
    _increase_ref();
    _initInt(v);

    return *this;
}

sgVar & sgVar::operator=( unsigned long v )
{
    if(_decrease_ref() > 0)
    {
        _ref_count = 0;
    }
    _increase_ref();
    _initInt(v);

    return *this;
}

sgVar & sgVar::operator=( long long v )
{
    if(_decrease_ref() > 0)
    {
        _ref_count = 0;
    }
    _increase_ref();
    _initInt64(v);

    return *this;
}

sgVar & sgVar::operator=( unsigned long long v )
{
    if(_decrease_ref() > 0)
    {
        _ref_count = 0;
    }
    _increase_ref();
    _initInt64(v);

    return *this;
}


sgVar & sgVar::operator=( float v )
{
    if(_decrease_ref() > 0)
    {
        _ref_count = 0;
    }
    _increase_ref();
    
    _initFloat(v);

    return *this;
}

sgVar & sgVar::operator=( double v )
{
    if(_decrease_ref() > 0)
    {
        _ref_count = 0;
    }
    _increase_ref();

    _initFloat(v);

    return *this;
}

sgVar & sgVar::operator=( const sgVar &v )
{
    if(this == &v)
        return *this;

    _decrease_ref();

    _ref_count = v._ref_count;
    _increase_ref();

    mDataType = v.mDataType;
    mData = v.mData;
    /*mArray = v.mArray;
    mMap = v.mMap;*/

    return *this;
}

void sgVar::makeNull( void )
{
	if(_decrease_ref() > 0)
	{
		_ref_count = 0;
	}
	_increase_ref();
	mDataType = T_NULL;
}

void sgVar::makeArray( size_t sz )
{
	if(_decrease_ref() > 0)
	{
		_ref_count = 0;
	}
	_increase_ref();
	array_type *ptr = new array_type;
	ptr->resize(sz);
	mData = ptr;
	mDataType = T_ARRAY;
}

void sgVar::makeMap( void )
{
	if(_decrease_ref() > 0)
	{
		_ref_count = 0;
	}
	_increase_ref();
	map_type *ptr = new map_type;
	mData = ptr;
	mDataType = T_MAP;
}


sgVar & sgVar::operator[]( const string_type &key )
{
    if(mDataType != T_MAP && mDataType != T_NULL)
    {
		assert(0);
    }

    mDataType = T_MAP;

    map_type *mMap = (map_type*)mData;
    if(!mMap)
    {
        mMap = new map_type;
        mData = mMap;
    }

    string_type str_key = key;
    map_type::iterator it = mMap->find(str_key);
    if(it != mMap->end())
    {
        return it->second;
    }

    // add a null var
    sgVar var;
    std::pair<map_type::iterator, bool> res = mMap->insert(map_pair_type(str_key, var));
    return res.first->second;

}

sgVar & sgVar::operator[]( size_t index )
{
    if(mDataType != T_ARRAY && mDataType != T_NULL)
    {
		assert(0);
    }

    mDataType = T_ARRAY;

    array_type *mArray = (array_type*)mData;
    if(!mArray)
    {
        mArray = new array_type;
        mData = mArray;
    }
    if(index >= mArray->size())
    {
        // add null vars
        for(size_t i=mArray->size(); i<=index; ++i)
        {
            mArray->push_back(sgVar());
        }
    }
    
    return (*mArray)[index];
    
}


const sgVar & sgVar::operator[]( const string_type &key ) const
{
    if(mDataType != T_MAP || !mData)
    {
		assert(0);
    }

	const map_type *mMap = (const map_type*)mData;
    string_type str_key = key;
    map_type::const_iterator it = mMap->find(str_key);
    if(it != mMap->end())
    {
        return it->second;
    }
	else
	{
		return NullVar;
	}

    

}

const sgVar & sgVar::operator[]( size_t index ) const
{
    if(mDataType != T_ARRAY || !mData)
    {
		assert(0);
    }

    const array_type *mArray = (const array_type*)mData;
    if(index >= mArray->size())
    {
        return NullVar;
    }
    
    return (*mArray)[index];
    
}

void sgVar::push_back(const sgVar &val)
{
    if(mDataType != T_ARRAY && mDataType != T_NULL)
    {
        assert(0);
    }

    mDataType = T_ARRAY;

    array_type *mArray = (array_type*)mData;
    if(!mArray)
    {
        mArray = new array_type;
        mData = mArray;
    }
    mArray->push_back(val);
}

sgVar::string_type sgVar::toString( void ) const
{
    if(mDataType == T_NULL || !mData)
        return "null";
    else if(mDataType == T_BOOL)
        return (*(bool*)mData) ? "true" : "false";
    else if(mDataType == T_MAP || mDataType == T_ARRAY)
        return toJson();
    else if(!mData)
        return "invalid";
    
    switch(mDataType)
    {
    case T_BOOL:
        {
            return internal::tostring(*((bool*)mData));
        }
    case T_FLOAT:
        {
            return internal::tostring(*((double*)mData));
        }
    case T_INT:
        {
            return internal::tostring(*((int_type*)mData));
            break;
        }
    case T_INT64:
        {
            return internal::tostring(*((int64_type*)mData));
            break;
        }
    case T_STRING:
        {
            return (char*)mData;
        }
	default:
		break;
    }

    return "invalid";


}

bool sgVar::toBool( void ) const
{
	if(mDataType == T_BOOL)
		 return *((bool*)mData);
	else if(mDataType == T_INT)
		return *((int_type*)mData) != 0;
    else if(mDataType == T_INT64)
        return *((int64_type*)mData) != 0;
	else if(mDataType == T_FLOAT)
		return *((float_type*)mData) != 0;
	else
		return false;

}

#define _sgVar_FE (2.3e-16)
#define _sgVar_REAL_FLOAT(f) (((f) > 0) ? ((f) + _sgVar_FE) : ((f) - _sgVar_FE))

sgVar::int_type sgVar::toInt( void ) const
{
    if(mDataType == T_INT)
	    return *((int_type*)mData);
    else if(mDataType == T_INT64)
        return (int_type)(*((int64_type*)mData));
	else if(mDataType == T_FLOAT)
		return (int_type)_sgVar_REAL_FLOAT((*(float_type*)mData));
	else if(mDataType == T_BOOL)
		return (int_type)(*(bool*)mData);
	else
		return 0;
}

sgVar::int64_type sgVar::ToInt64( void ) const
{
    if(mDataType == T_INT64)
        return *((int64_type*)mData);
    else if(mDataType == T_INT)
        return (int64_type)(*((int_type*)mData));
    else if(mDataType == T_FLOAT)
        return (int64_type)_sgVar_REAL_FLOAT((*(float_type*)mData));
    else if(mDataType == T_BOOL)
        return (int64_type)(*(bool*)mData);
    else
        return 0;
}	

sgVar::float_type sgVar::toFloat( void ) const
{
	if(mDataType == T_FLOAT)
		return (*((float_type*)mData));
	else if(mDataType == T_INT)
		return (float_type)(*(int_type*)mData);
    else if(mDataType == T_INT64)
        return (float_type)(*((int64_type*)mData));
	else if(mDataType == T_BOOL)
		return (float_type)(*(bool*)mData);
	else
		return 0.0;
}

sgVar::string_type sgVar::toJson( bool formatted/* = false*/, char quote/* = '\''*/, size_t level/* = 0*/ ) const
{
	string_type space_unit = "    ";
	string_type space_1, space_2;
	for(size_t i=0; i<level; ++i)
	{
		space_1 += space_unit;
	}
	space_2 = space_1 + space_unit;

    std::stringstream ss;
    if(mDataType == T_MAP)
    {
        map_type *mMap = (map_type*)mData;
        if(!mMap)
            return "invalid";
		
		if(formatted)
			ss << "\n" << space_1;
        ss << "{ ";
		if(formatted)
			ss << "\n";

        size_t sz = mMap->size();
        if(sz > 0)
        {
            map_type::const_iterator it = mMap->begin();
            for(sz=sz-1; sz>0 ; ++it, --sz)
            {
                const sgVar &var = it->second;
                string_type str_v = var.toJson(formatted, quote, level+1);
                if(var.isString())
                {
                    str_v = string_type() + quote + str_v + quote;
                }
				if(formatted)
					ss << space_2;
                ss << quote << it->first.c_str() << quote << ":" << str_v.c_str() << ", ";
				if(formatted)
					ss << "\n";
            }

            // the last element
            const sgVar &var = mMap->rbegin()->second;
            string_type str_v = var.toJson(formatted, quote, level+1);
            if(var.isString())
            {
                str_v = string_type() + quote + str_v + quote;
            }
			if(formatted)
				ss << space_2;
            ss <<  quote << it->first.c_str() << quote << ":" << str_v.c_str() << " ";
			if(formatted)
				ss << "\n";
        }
        
		if(formatted)
			ss << space_1;
        ss << "}";

        return string_type(ss.str().c_str());
    }
    else if(mDataType == T_ARRAY)
    {
        array_type *mArray = (array_type*)mData;
        if(!mArray)
            return "invalid";

		if(formatted)
			ss << "\n" << space_1;
        ss << "[ ";
		//if(formatted)
//			ss << "\n";

        size_t sz = mArray->size();
        if(sz > 0)
        {
            for(size_t i=0; i<sz-1; ++i)
            {
                const sgVar &var = (*mArray)[i];
                string_type str_v = var.toJson(formatted, quote, level+1);
                if(var.isString())
                {
                    str_v = string_type() + quote + str_v + quote;
                }
				if(formatted)
				{
					if(!var.isMap())
						ss << "\n";
					ss << space_2;
				}
                ss << str_v.c_str() << ", ";
				if(formatted)
					ss << "\n";
            }

            // the last element
            const sgVar &var = (*mArray)[sz-1];
            string_type str_v = var.toJson(formatted, quote, level+1);
            if(var.isString())
            {
                str_v = string_type() + quote + str_v + quote;
            }
			if(formatted)
			{
				if(!var.isMap())
					ss << "\n";
				ss << space_2;
			}
            ss << str_v.c_str() << " ";
			if(formatted)
				ss << "\n";
        }

		if(formatted)
			ss << space_1;
        ss << "]";

        return ss.str().c_str();
    }
    else
    {
        return toString();
    }
}

size_t sgVar::size( void ) const
{
	if(!mData)
		return 0;

	if(mDataType == T_ARRAY)
	{
		return ((array_type*)mData)->size();
	}
	else if(mDataType == T_MAP)
	{
		return ((map_type*)mData)->size();
	}
	else
	{
		return 0;
	}
}

void sgVar::getMapKeys( map_key_array_type &arr ) const
{
	arr.clear();
	if(mDataType != T_MAP)
		return ;

	const map_type *mapdata = (const map_type*)mData;
	arr.reserve(mapdata->size());
	map_type::const_iterator it = mapdata->begin();
	for(; it!=mapdata->end(); ++it)
	{
		arr.push_back(it->first);
	}
}

std::ostream & operator << (std::ostream & os, const sgVar &t1){
    os << "sgVar(" << t1.mData << ")";
    return os;
}
