# metastuff-clang-generator
Clang based generator for [MetaStuff](https://github.com/eliasdaler/MetaStuff)

# Prerequisites
[clang](https://clang.llvm.org/) & [llvm](https://llvm.org/) is required to be installed as generating is based on [libTooling](https://clang.llvm.org/docs/LibTooling.html) from LLVM/Clang

# Building
For building you need to have llvm & clang installed then standard cmake stuff: 
$ git clone https://github.com/w0land/metastuff-clang-generator.git
$ cd metastuff-clang-generator
$ mkdir build && cd build
$ cmake ..
$ cmake --build . --config Release
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

# Detailed usage 
```
$ metastuff-clang-generator --help

USAGE: metastuff-clang-generator [options] <source0> [... <sourceN>]

OPTIONS:

Generic Options:

  -help                      - Display available options (-help-hidden for more)
  -help-list                 - Display list of available options (-help-list-hidden for more)
  -version                   - Display the version of this program

metastuff-generator options:

  -extra-arg=<string>        - Additional argument to append to the compiler command line
  -extra-arg-before=<string> - Additional argument to prepend to the compiler command line
  -p=<string>                - Build path
  -template-file=<filename>  - path to template file

-p <build-path> is used to read a compile command database.

	For example, it can be a CMake build directory in which a file named
	compile_commands.json exists (use -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
	CMake option to get this output). When no build path is specified,
	a search for compile_commands.json will be attempted through all
	parent paths of the first input file . See:
	http://clang.llvm.org/docs/HowToSetupToolingForLLVM.html for an
	example of setting up Clang Tooling on a source tree.

<source0> ... specify the paths of source files. These paths are
	looked up in the compile command database. If the path of a file is
	absolute, it needs to point into CMake's source tree. If the path is
	relative, the current working directory needs to be in the CMake
	source tree and the file must be in a subdirectory of the current
	working directory. "./" prefixes in the relative files will be
	automatically removed, but the rest of a relative path must be a
	suffix of a path in the compile command database.


More help text...
```
