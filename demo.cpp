#include "sgJsonParser.h"
#include "sgVar.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

int main(int argc, char *argv[])
{
    std::string json = "{ 'hello' : 'world', 'bT' : true , 'bF' : false, 'n': null, 'i':123, 'pi': 3.1416, 'arr':[1, 'abcd', 3, 4] } ";
    std::cout << "\n=>Original Json: \n" << json << "\n";

    std::stringstream ss;
    ss << json;

    // parse json from string
    sgJsonParser parser;
    sgVar val = parser.parse(ss);
    if(parser.fail())
    {
        std::cout << parser.getErrorMsg() << "\n";
        return 1;
    }

    // serialize to json
    std::ofstream out("demo.json");
    out << val.toJson();
    out.flush();
    out.close();

    // load from json file
    std::ifstream in("demo.json");
    if(in.fail())
    {
        std::cout << "open demo.json failed.\n";
        return 1;
    }
    sgVar doc = parser.parse(in);
    in.close();
    std::cout << "\n=============\n=>demo.json loaded: \n" << doc.toJson(true) << "\n=============\n";

    // get the value
    std::cout << "\n=============\n=>get doc[\"pi\"] and doc[\"arr\"]\n";
    sgVar vPI = doc["pi"];
    std::cout << "pi: " << vPI.toFloat() << "\n";

    sgVar vArr = doc["arr"];
    std::cout << "arr: " << vArr.toString() << "\n";

    // set the value
    std::cout << "\n=>set arr[0] = 2.01, arr[3] = 27\n";
    vArr[0] = 2.01;
    vArr[3] = 27;
    std::cout << "arr: " << vArr.toString() << "\n";

    // create a map
    sgVar mapVal;
    mapVal["key1"] = 1;
    mapVal["key2"] = "string val 2";
    // create an array as an item of the mapVal
    sgVar &vArr2 = mapVal["key3"];
    vArr2[2] = 2.0;
    vArr2[5] = "this is val 5 of arr2";
    std::cout << "\n=============\n=>new created map var: \n" << mapVal.toJson(true) << "\n=============\n";

}
