//
// Created by Hjnodm on 2022/11/26.
//
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <string>
#include <vector>
#include <fstream>
#include <direct.h>
#include <iostream>
#include <ctime>
#include <cstring>
#include <ctime>
#ifndef SUDOKU_DPLL__GLOBAL_H_
#define SUDOKU_DPLL__GLOBAL_H_

enum Cat {
  satisfied,
  unsatisfied,
  normal, //when the assignment has not been found till now
  completed //when the algo has completed
};

static int sudoku2state(int number_in_sudoku) {
  int hundreds, tens, ones;
  hundreds = number_in_sudoku / 100;
  tens = number_in_sudoku % 100 / 10;
  ones = number_in_sudoku % 10;
  return (hundreds - 1) * 81 + (tens - 1) * 9 + ones;
}

#endif //SUDOKU_DPLL__GLOBAL_H_
