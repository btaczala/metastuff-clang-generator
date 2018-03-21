# metastuff-clang-generator
Clang based generator for [MetaStuff](https://github.com/eliasdaler/MetaStuff)

# Building
For building you need to have llvm & clang installed then standard cmake stuff: 
```
$ git clone https://github.com/w0land/metastuff-clang-generator.git
$ cd metastuff-clang-generator
$ mkdir build
$ cmake .. 
```

# Usage
To properly use this on actual headers you need a bit of fine tunning: you need to figure out where default includes are for clang and pass them to metastuff-clang-generator
 command line. See example: 
 ```
 $ cd build/
 $ ./src/metastuff-generator ~/main.cpp -- -I/usr/lib/clang/5.0.1/include -std=c++14
 ```
 
 where main.cpp: 
 ```cpp
 #include <string>

namespace asd {
struct SimpleType {
    std::string type;
};
struct SimpleType2 {
    std::string type;
    SimpleType bbb;
};
}

int main(int argc, char *argv[])
{
    std::string asd;
    return 0;
}
```

The generated code will be printed to stdout. 
For given example the generated code is: 
```cpp

#ifndef TEST
#define TEST

#include <Meta.h>

namespace meta {
inline auto registerMembers<asd::SimpleType>() {
    return members(
        member("type", &asd::SimpleType::type)
    )};

inline auto registerMembers<asd::SimpleType2>() {
    return members(
        member("type", &asd::SimpleType2::type),
        member("bbb", &asd::SimpleType2::bbb)
    )};


}
#endif TEST

```
