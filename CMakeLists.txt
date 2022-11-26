cmake_minimum_required(VERSION 3.23)
project(sudoku_DPLL)

set(CMAKE_CXX_STANDARD 11)


add_executable(sudoku_DPLL main.cpp Global.h Sudoku.cpp)

#add_executable(DPLL DPLL.cpp)