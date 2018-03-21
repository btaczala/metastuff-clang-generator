
add_executable(metastuff-clang-generator
    main.cpp
)

target_compile_definitions(metastuff-clang-generator PRIVATE __STDC_LIMIT_MACROS __STDC_CONSTANT_MACROS)
target_link_libraries(metastuff-clang-generator fmt)
target_link_libraries(metastuff-clang-generator
    clangFrontend clangSerialization clangDriver clangParse clangRewriteFrontend clangStaticAnalyzerFrontend clangSema
    clangAnalysis clangEdit clangAST clangLex clangBasic clangTooling
    clangRewrite clangASTMatchers clangToolingCore)

target_link_libraries(metastuff-clang-generator 
    LLVMX86AsmParser
    LLVMX86Desc # MC, Support, X86AsmPrinter, X86Info
    LLVMX86AsmPrinter # MC, Support, X86Utils
    LLVMX86Info # MC, Support, Target
    LLVMX86Utils # Core, Support
    LLVMipo
    LLVMScalarOpts
    LLVMInstCombine
    LLVMTransformUtils
    LLVMAnalysis
    LLVMTarget
    LLVMOption # Support
    LLVMMCParser # MC, Support
    LLVMMC # Object, Support
    LLVMObject # BitReader, Core, Support
    LLVMBitReader # Core, Support
    LLVMCore # Support
    LLVMSupport
)

install(TARGETS metastuff-clang-generator
    RUNTIME DESTINATION bin)
