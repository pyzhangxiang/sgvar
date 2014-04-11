#ifndef __SGVAR_H__
#define __SGVAR_H__


#include <string>
#include <map>
#include <vector>
#include <iostream>

class sgVar
{
public:
    typedef std::string string_type;
    typedef long int_type;
    typedef long long int64_type;
    typedef double float_type;
    typedef std::vector<sgVar> array_type;
    typedef std::map<string_type, sgVar> map_type;
	typedef std::pair<string_type, sgVar> map_pair_type;
	typedef std::vector<string_type> map_key_array_type;
	
	static sgVar NullVar;

private:
    enum DataType{
        T_BOOL='B', 
        T_FLOAT='F',
        T_INT='I',  
        T_INT64='L',
        T_STRING='S',
        T_MAP='M',  
        T_ARRAY='A',
        T_NULL = '0',
    };

    DataType mDataType;
    void *mData;
    /*array_type *mArray;
    map_type *mMap;*/
    
    size_t *_ref_count;
    void _release(void);
    size_t _increase_ref(void);
    size_t _decrease_ref(void);
    size_t _count_ref(void) const;

    void _initInt(int_type v);
    void _initInt64(int64_type v);
    void _initFloat(float_type v);
public:
    ~sgVar(void);

    sgVar(void);
    sgVar(const char *v);
    sgVar(bool v);
    sgVar(char v);
    sgVar(unsigned char v);
    sgVar(short v);
    sgVar(unsigned short v);
    sgVar(int v);
    sgVar(unsigned int v);
    sgVar(long v);
    sgVar(unsigned long v);
    sgVar(long long v);
    sgVar(unsigned long long v);
    sgVar(float v);
    sgVar(double v);
    sgVar(const sgVar &v);

    sgVar &operator=(const char *v);
    sgVar &operator=(bool v);
    sgVar &operator=(char v);
    sgVar &operator=(unsigned char v);
    sgVar &operator=(short v);
    sgVar &operator=(unsigned short v);
    sgVar &operator=(int v);
    sgVar &operator=(unsigned int v);
    sgVar &operator=(long v);
    sgVar &operator=(unsigned long v);
    sgVar &operator=(long long v);
    sgVar &operator=(unsigned long long v);
    sgVar &operator=(float v);
    sgVar &operator=(double v);
    sgVar &operator=(const sgVar &v);

	void makeNull(void);
	/// make type to be array, and resize it by sz
	void makeArray(size_t sz);
	void makeMap(void);

    sgVar &operator[](const string_type &key);
    sgVar &operator[](size_t index);

	const sgVar &operator[](const string_type &key) const;
	const sgVar &operator[](size_t index) const;

    string_type toString(void) const;
    bool toBool(void) const;
    int_type toInt(void) const;
    int64_type ToInt64(void) const;
    float_type toFloat(void) const; 
    string_type toJson(bool formatted = false, char quote = '\'', size_t level = 0) const;  

    bool isNull(void) const{ return mDataType == T_NULL; }
    bool isString(void) const{ return mDataType == T_STRING; }
    bool isBool(void) const{ return mDataType == T_BOOL; }
    bool isInt(void) const{ return mDataType == T_INT; }
    bool IsInt64(void) const{ return mDataType == T_INT64; }
    bool isFloat(void) const{ return mDataType == T_FLOAT; }
    bool isArray(void) const{ return mDataType == T_ARRAY; }
    bool isMap(void) const{ return mDataType == T_MAP; }
	
	/// array or map's size, otherwise return 0
	size_t size(void) const;
	void getMapKeys(map_key_array_type &arr) const;

    friend std::ostream & operator << (std::ostream & os, const sgVar &t1);

};

std::ostream & operator << (std::ostream & os, const sgVar &t1);

#endif  // __SGVAR_H__

