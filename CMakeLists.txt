cmake_minimum_required(VERSION 3.0.0)
project(PlanetoidScript VERSION 0.1.0)

add_executable(PlanetoidScript src/PlanetoidScript.cpp src/Token.cpp src/Lexer.cpp src/Position.cpp src/Error.cpp src/Parser.cpp src/TokenNode.cpp src/Interpreter.cpp src/Value.cpp src/SymbolTable.cpp)