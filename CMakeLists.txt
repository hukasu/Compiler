cmake_minimum_required(VERSION 3.8.2)

project(Compiler)

add_library(
	CompilerLib
	STATIC
	src/nfa.cpp
)
target_include_directories(
	CompilerLib
	PUBLIC include/
)
target_compile_features(
	CompilerLib
	PUBLIC cxx_std_17
)