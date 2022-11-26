//
// Created by Hjnodm on 2022/11/25.
//
#include "Global.h"
using namespace std;

void ShowProblem(int a[][9]) {
  for (int i = 0; i < 9; ++i) {
    for (int j = 0; j < 9; ++j) {
      if (a[i][j] != 0) {
        printf("%d ", a[i][j]);
      } else {
        printf("_ ");
      }
    }
    printf("\n");
  }
}
void GenerateFirstLine(int a[]) {
  srand((unsigned) time(NULL));//设置随机数种子为 真随机 数 in English  set it for true random number
  for (int i = 0; i < 9; ++i) {
    a[i] = rand() % 9 + 1;
    int j = 0;
    while (j < i) {
      if (a[i] == a[j]) {
        a[i] = rand() % 9 + 1; //生成随机的不同的9个数字 in English generate nine random number
        j = 0;
      } else {
        j++;
      }
    }
  }
}

int CompleteSudoku(int a[][9], int x, int y) {
  if (x < 9 and y < 9) {
    int check[10];//用来表示没使用过的 即合法的 数字。0 合法 1 非法 in English: 0 is legal , 1 is illegal
    memset(check, 0, sizeof(check));
    for (int i = 0; i < y; ++i) {
      check[a[x][i]] = 1; //在第 x 行出现的数字 非法
    }
    for (int i = 0; i < x; ++i) {
      check[a[i][y]] = 1;// 在第 y 列出现的数字 非法
    }
    //在 x,y处出现的值，不能在其3×3 的小方格内出现
    for (int i = x / 3 * 3; i < x / 3 * 3 + 3; ++i) {
      for (int j = y / 3 * 3; j < y / 3 * 3 + 3; ++j) {
        if (i == x or j == y) {
          continue;
        }
        check[a[i][j]] = 1;
      }
    }
    int fl = 0;
    for (int i = 1; i <= 9 and fl == 0; ++i) {
      //从 check 中合法的值开始一个个选取
      if (check[i] == 0) {
        fl = 1;
        a[x][y] = i;
        if (y == 8 and x != 8) {
          //所选方格的赋值到了最后一列 就向下一行的第一个移动
          if (CompleteSudoku(a, x + 1, 0) == 0) {
            return 0;
          } else {
            fl = 0;
          }
        } else if (y != 8) {
          //即 如果所选方格没在最后一列时，向后移动即可
          if (CompleteSudoku(a, x, y + 1) == 0) {
            return 0;
          } else {
            fl = 0;
          }
        }
      }
    }
    if (fl == 0) {
      a[x][y] = 0;
      return 1;
    }
  }
  return 0;
}

void CreateSudoku(int a[][9]) {
  GenerateFirstLine(a[0]);
  CompleteSudoku(a, 1, 0);
}

void CreateProle(int a[][9], int b[][9], int empty_number) {
  // a 为原来完整的数独，b 为我们需要的数独问题 in English : a is the sudoku we want to generate b is the sudoku problem
  srand((unsigned) time(NULL));//设置随机数种子为 真随机 数
  for (int i = 0; i < 9; ++i) {
    for (int j = 0; j < 9; ++j) {
      b[i][j] = a[i][j];
    }
  }
  for (int i = 0; i < empty_number; ++i) {
    int x = rand() % 9;
    int y = rand() % 9;

    if (b[x][y] != 0) {
      b[x][y] = 0;
    } else {
      i--;
    }
  }
}

void Sudoku2CNF(string path, int a[][9], int empty_number) {
  ofstream cnf_file;
  cnf_file.open(path, ios::out);//定义输入文件 in English: define the input file
  if (!cnf_file) {
    printf("can't open!\n");
  }
  cnf_file << "p" << " " << "cnf" << " " << 729 << " " << 8829 + 81 - empty_number << endl;
  //根据已有的数字进行子句的添加 in English: according to the sudoku problem add the number we already have.
  for (int i = 0; i < 9; ++i) {
    for (int j = 0; j < 9; ++j) {
      if (a[i][j] != 0) {
        cnf_file << (i + 1) * 100 + (j + 1) * 10 + a[i][j] << " " << 0 << endl;
      }
    }
  }
  //单个方块内只能出现1-9中的其中一个 in English: Cells must be filled in with a number between 1 and 9.
  for (int i = 1; i <= 9; ++i) {
    for (int j = 1; j <= 9; ++j) {
      for (int z = 1; z <= 9; ++z) {
        cnf_file << i * 100 + j * 10 + z << " ";
      }
      cnf_file << 0 << endl;
    }
  }
  //行不能出现重复的规则 in English: Rows can not have duplicate numbers.
  for (int z = 1; z <= 9; ++z) {
    for (int j = 1; j <= 9; ++j) {
      for (int i = 1; i <= 8; ++i) {
        for (int k = i + 1; k <= 9; ++k) {
          cnf_file << 0 - (i * 100 + j * 10 + z) << " " << 0 - (k * 100 + j * 10 + z) << " " << 0 << endl;
        }
      }
    }
  }
  //列不能出现重复的规则  in English: Columns can not have duplicate numbers.
  for (int z = 1; z <= 9; ++z) {
    for (int i = 1; i <= 9; ++i) {
      for (int j = 1; j <= 8; ++j) {
        for (int k = j + 1; k <= 9; ++k) {
          cnf_file << 0 - (i * 100 + j * 10 + z) << " " << 0 - (i * 100 + k * 10 + z) << " " << 0 << endl;
        }
      }
    }
  }
  //每个 3 ×3的小方格内不能有重复的数字  in English: Subsquares can not have duplicate numbers.
  for (int z = 1; z <= 9; ++z) {
    for (int i = 0; i <= 2; ++i) {
      for (int j = 0; j <= 2; ++j) {
        for (int x = 1; x <= 3; ++x) {
          for (int y = 1; y <= 3; ++y) {
            for (int k = 1; k <= 3; ++k) {
              for (int l = 1; l <= 3; ++l) {
                if (x == k or y == l) {
                  continue;
                } else {
                  cnf_file << 0 - (k * 100 + l * 10 + z) << " " << 0 - (x * 100 + y * 10 + z) << " " << 0 << endl;
                }
              }
            }
          }
        }
      }
    }
  }
  cnf_file.close();

}
void CreateSudokuProblem(int empty_number) {
  int sudoku[9][9] = {0};// the sudoku we want to generate
  int sudoku_backup[9][9] = {0};// the sudoku problem we want to generate
  CreateSudoku(sudoku);
  CreateProle(sudoku, sudoku_backup, empty_number);
  ShowProblem(sudoku_backup);
  string path = "..\\sudoku.cnf";
  Sudoku2CNF(path, sudoku_backup, empty_number);
}
