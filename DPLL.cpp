//
// Created by Hjnodm on 2022/11/26.
//


using namespace std;

class Formula {
 public:
  //a vector that stores the value assigned,-1 unsatisfied, 0 false 1 true
  vector<int> literals;
  //store the number of occurrences of each literal
  vector<int> literals_frequency;
  //store the difference in number of occurrences with
  // positive and negative polarity of each literal
  vector<int> literal_polarity;
  // vector to store the clauses
  // for each clauses, if the variable n is of positive polarity, then 2n is
  // stored if the variable n is of negative polarity, then 2n+1 is stored here,
  // n is assumed to be zero indexed

  vector<vector<int>> clauses;//???
  Formula() {}

  Formula(const Formula &f) {
    literals = f.literals;
    clauses = f.clauses;
    literals_frequency = f.literals_frequency;
    literal_polarity = f.literal_polarity;
  }
};

class SATSolverDPLL {
 private:
  Formula formula;
  int literal_count;
  int clause_count;
  int unit_propagation(Formula &);
  int apply_transform(Formula &, int);
  static void show_result1(Formula &, int);
  void show_result2(Formula &, int);
  int DPLL(Formula);
 public:
  int type;
  void init1();
  void init2();
  void solve();
  int ans_sudoku[9][9] = {-1};
  void show_sudoku();
};

void SATSolverDPLL::init1() {
  char commend;
  type = 1;
  string s;//dummy string aka the cnf
  printf("DPLL SAT Solver\n");

  while (true) {
    scanf("%c", &commend);
    if (commend == 'c') {
      getline(cin, s);
    } else if (commend == 'p') {
      cin >> s;
      break;
    }
  }
  cin >> literal_count;
  cout << "literal_count: " << literal_count << endl;
  cin >> clause_count;
  cout << "clause_count: " << clause_count << endl;

  //set all the vector to be appropriate size and the initial values
  formula.literals.clear();
  formula.literals.resize(literal_count, -1);

  formula.clauses.clear();
  formula.clauses.resize(clause_count);

  formula.literals_frequency.clear();
  formula.literals_frequency.resize(literal_count, 0);
  formula.literal_polarity.clear();
  formula.literal_polarity.resize(literal_count, 0);

  int literal;
  for (int i = 0; i < clause_count; ++i) {
    while (true) {
      cin >> literal;
      if (literal > 0) {
        formula.clauses[i].push_back(2 * (literal - 1));
        formula.literals_frequency[literal - 1]++;
        formula.literal_polarity[literal - 1]++;
      } else if (literal < 0) {
        formula.clauses[i].push_back(2 * ((-1) * literal - 1) + 1);
        formula.literals_frequency[-1 - literal]++;
        formula.literal_polarity[-1 - literal]--;
      } else {
        break;
      }
    }
  }
  cout << "literals_frequency :";
  for (int i = 0; i < literal_count; ++i) {
    cout << formula.literals_frequency[i] << " ";
  }
  cout << endl;
}

void SATSolverDPLL::init2() {
  type = 2;
  string s;//dummy string
  printf("DPLL SAT Solver\n");
  string path = "..\\sudoku.cnf";
  ifstream cnf_file(path);
  if (!cnf_file) {
    cout << "File can not open.";
    exit(-1);
  }
  char ch;
  char buf[100];
  cnf_file >> ch;
  while (ch != 'p') {
    cnf_file.getline(buf, 100);
    cnf_file >> ch;
  }//until we read zhe letter p
  string cnf;
  cnf_file >> cnf;
  cnf_file >> literal_count;
  cout << "literal_count: " << literal_count << endl;
  cnf_file >> clause_count;
  cout << "clause_count: " << clause_count << endl;

  //set all the vector to be appropriate size and the initial values
  formula.literals.clear();
  formula.literals.resize(literal_count, -1);

  formula.clauses.clear();
  formula.clauses.resize(clause_count);

  formula.literals_frequency.clear();
  formula.literals_frequency.resize(literal_count, 0);
  formula.literal_polarity.clear();
  formula.literal_polarity.resize(literal_count, 0);

  for (int i = 0; i < clause_count; ++i) {
    while (true) {
      int literal;
      cnf_file >> literal;
      if (literal > 0) {
        literal = sudoku2state(literal);
        formula.clauses[i].push_back(2 * (literal - 1));
        formula.literals_frequency[literal - 1]++;
        formula.literal_polarity[literal - 1]++;
      } else if (literal < 0) {
        literal = sudoku2state(literal * -1) * -1;
        formula.clauses[i].push_back(2 * ((-1) * literal - 1) + 1);
        formula.literals_frequency[-1 - literal]++;
        formula.literal_polarity[-1 - literal]--;
      } else {
        break;
      }
    }
  }
  cout<<"literals_frequency : ";
  for (int i = 0; i < literal_count; ++i) {
    cout << formula.literals_frequency[i] << " ";
  }
  cout << endl;
}

int SATSolverDPLL::unit_propagation(Formula &f) {
  bool unit_clause_found = false;
  if (f.clauses.size() == 0) {
    return Cat::satisfied;
  }
  do {
    unit_clause_found = false;
    for (int i = 0; i < f.clauses.size(); ++i) {
      if (f.clauses[i].size() == 1) {
        unit_clause_found = true;
        f.literals[f.clauses[i][0] / 2] = f.clauses[i][0] % 2;
        //0 - if true, 1 - if false, set the literal
        f.literals_frequency[f.clauses[i][0] / 2] = -1;
        int result = apply_transform(f, f.clauses[i][0] / 2);
        if (result == Cat::satisfied or result == Cat::unsatisfied) {
          return result;
        }
        break;
      } else if (f.clauses[i].size() == 0) {
        return Cat::unsatisfied;
      }
    }
  } while (unit_clause_found);
  return Cat::normal;
}

int SATSolverDPLL::apply_transform(Formula &f, int literal_to_apply) {
  int value_to_apply = f.literals[literal_to_apply];
  // iterate over the clauses in f
  for (int i = 0; i < f.clauses.size(); ++i) {
    for (int j = 0; j < f.clauses[i].size(); ++j) {
      if ((2 * literal_to_apply + value_to_apply) == f.clauses[i][j]) {
        f.clauses.erase(f.clauses.begin() + i);
        i--;
        if (f.clauses.size() == 0) {
          return Cat::satisfied;
        }
        break;
      } else if (f.clauses[i][j] / 2 == literal_to_apply) {
        f.clauses[i].erase(f.clauses[i].begin() + j);
        j--;
        if (f.clauses[i].size() == 0) {
          return Cat::unsatisfied;
        }
        break;
      }
    }
  }
  return Cat::normal;
}

int SATSolverDPLL::DPLL(Formula f) {
  int result = unit_propagation(f);
  if (result == Cat::satisfied) {
    if (type == 1) {
      show_result1(f, result);
    } else if (type == 2) {
      show_result2(f, result);
    }
    return Cat::completed;
  } else if (result == Cat::unsatisfied) {
    return Cat::normal;
  }
  int i = distance(f.literals_frequency.begin(), max_element(f.literals_frequency.begin(), f.literals_frequency.end()));
  // find the most frequent literal
  for (int j = 0; j < 2; ++j) {
    Formula new_f = f;
    if (new_f.literal_polarity[i] > 0) {
      new_f.literals[i] = j;
    } else {
      new_f.literals[i] = (j + 1) % 2;
    }
    new_f.literals_frequency[i] = -1;
    // reset the frequency to -1 to ignore in the future
    int transform_result = apply_transform(new_f, i);
    if (transform_result == Cat::satisfied) {
      if (type == 1) {
        show_result1(new_f, transform_result);
      } else if (type == 2) {
        show_result2(new_f, transform_result);
      }
      return Cat::completed;
    } else if (transform_result == Cat::unsatisfied) {
      continue;
    }
    int dpll_result = DPLL(new_f);
    if (dpll_result == Cat::completed) {
      return dpll_result;
    }
  }
  return Cat::normal;
}

void SATSolverDPLL::show_result1(Formula &f, int result) {
  if (result == Cat::satisfied) {
    cout << " SAT " << endl;
    for (int i = 0; i < f.literals.size(); ++i) {
      if (i != 0) {
        cout << "  ";
      }
      if (f.literals[i] != -1) {
        cout << pow(-1, f.literals[i]) * (i + 1);
      } else {
        cout << i + 1 << "*";
      }
    }
    cout << " 0 " << endl;
  } else {
    cout << " UN SAT " << endl;
  }
}

void SATSolverDPLL::show_result2(Formula &f, int result) {
  if (result == Cat::satisfied) {
    cout << "  SAT  " << endl;
    for (int i = 0; i < f.literals.size(); ++i) {
      if (f.literals[i] != -1) {
        int ans = pow(-1, f.literals[i]) * (i + 1);
        if (ans > 0) {
          int hundreds = ans / 81;
          if (hundreds * 81 != ans) hundreds += 1;
          ans -= (hundreds - 1) * 81;
          int tens = ans / 9;
          int ones = ans % 9;
          if (ones == 0) {
            ones = 9;
            tens--;
          }
          ans_sudoku[hundreds - 1][tens] = ones;
        }
      } else {
        cout << i + 1 << "*";
      }
    }
  } else {
    cout << " UN SAT " << endl;
  }
}

void SATSolverDPLL::solve() {
  int result = DPLL(formula);
  if (result == Cat::normal) {
    show_result1(formula, Cat::unsatisfied);
  }
}
void SATSolverDPLL::show_sudoku() {
  for (int i = 0; i < 9; ++i) {
    for (int j = 0; j < 9; ++j) {
      cout << ans_sudoku[i][j] << " ";
    }
    cout << endl;
  }
}


