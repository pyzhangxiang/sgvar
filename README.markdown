sgVar 
=======
#### a json parser/generator and var lib for c++ ####

### Build ###
Copy sgVar.h/cpp and sgJsonParser.h/cpp to your own project.

### Usage ###
Most usage are shown in the demo.cpp. make and execute it to see the result.

##### Read json from a file #####

```cpp
sgJsonParser parser;
sgVar doc = parser.parse("demo.json");
if(parser.fail())
{
    std::cout << parser.getErrorMsg() << "\n";
    return 1;
}
```

##### serialize to json #####

```cpp
doc.toJson()
```

Then you can save the json string to a file.


##### Use the sgVar #####

```cpp
sgVar val;                  // create a null var
val[0] = 11;                // make val be an array
val[2] = "hello sgVar";     // val[1] will be a null var

sgVar &vMap = val[3];       // create a null var
vMap["pi"] = 3.14;          // make vMap be a map


/****** get value from var ******/

bool isFloat = vMap["pi"].isFloat();    // check a var is float

double pi = vMap["pi"].toFloat();    // get the float value of a var
int i0 = val[0].toInt();             // get the int value of a var
std::string str = val[2].toString(); // get the string value of a var
```




