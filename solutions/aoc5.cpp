#include <bits/stdc++.h>
#include <stack>

using namespace std;

vector<string> stringSplit(string a, char delimiter) {
  string accum = "";
  vector<string> result;
  for (int i = 0; i < a.size(); ++i) {
    if (a[i] == delimiter) {
      result.push_back(accum);
      accum = "";
    } else {
      accum += a[i];
    }
  }
  if (accum.size() != 0) {
    result.push_back(accum);
  }
  return result;
}

vector<vector<string>> transpose(vector<vector<string>> &A) {
  int rows = A.size();
  if (rows == 0)
    return {{}};
  int cols = A[0].size();
  vector<vector<string>> transpose(cols, vector<string>(rows));
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      transpose[j][i] = A[i][j];
    }
  }
  return transpose;
}

bool is_whitespace(string &s) {
  for (string::const_iterator it = s.begin(); it != s.end(); ++it) {
    if (!isspace(*it)) {
      return false;
    }
  }
  return true;
}

void part1() {
  ifstream myReadFileConfig("../inputs/input5.config.txt");
  ifstream myReadFileMoves("../inputs/input5.moves.txt");
  string myText;
  string gridString;
  vector<vector<string>> grid;

  while (getline(myReadFileConfig, myText)) {
    vector<string> layer;
    for (int i = 0; i < myText.size(); i += 4) {
      layer.push_back(myText.substr(i, 4));
    }
    grid.push_back(layer);
  }

  vector<vector<int>> moves;
  while (getline(myReadFileMoves, myText)) {
    vector<string> splitted = stringSplit(myText, ' ');
    vector<int> move;
    int start = atoi(splitted[3].c_str()) - 1,
        end = atoi(splitted[5].c_str()) - 1, number = atoi(splitted[1].c_str());
    move.push_back(start);
    move.push_back(end);
    move.push_back(number);
    moves.push_back(move);
  }

  vector<stack<string>> stackVec;
  for (auto x : transpose(grid)) {
    stack<string> st;
    reverse(x.begin(), x.end());
    for (auto y : x) {
      if (!is_whitespace(y)) {
        st.push(y);
      }
    }
    stackVec.push_back(st);
  }
  for (int j = 0; j < moves.size(); ++j) {
    int moveFrom = moves[j][0], moveTo = moves[j][1], moveNumber = moves[j][2];
    while (moveNumber) {
      string topElement = stackVec[moveFrom].top();
      stackVec[moveFrom].pop();
      stackVec[moveTo].push(topElement);
      moveNumber--;
    }
  }
  for (auto x : stackVec) {
    cout << x.top() << "\n";
  }
}

void part2() {

  ifstream myReadFileConfig("../inputs/input5.config.txt");
  ifstream myReadFileMoves("../inputs/input5.moves.txt");
  string myText;
  string gridString;
  vector<vector<string>> grid;

  while (getline(myReadFileConfig, myText)) {
    vector<string> layer;
    for (int i = 0; i < myText.size(); i += 4) {
      layer.push_back(myText.substr(i, 4));
    }
    grid.push_back(layer);
  }

  vector<vector<int>> moves;
  while (getline(myReadFileMoves, myText)) {
    vector<string> splitted = stringSplit(myText, ' ');
    vector<int> move;
    int start = atoi(splitted[3].c_str()) - 1,
        end = atoi(splitted[5].c_str()) - 1, number = atoi(splitted[1].c_str());
    move.push_back(start);
    move.push_back(end);
    move.push_back(number);
    moves.push_back(move);
  }

  vector<stack<string>> stackVec;
  for (auto x : transpose(grid)) {
    stack<string> st;
    reverse(x.begin(), x.end());
    for (auto y : x) {
      if (!is_whitespace(y)) {
        st.push(y);
      }
    }
    stackVec.push_back(st);
  }

  for (int j = 0; j < moves.size(); ++j) {
    int moveFrom = moves[j][0], moveTo = moves[j][1], moveNumber = moves[j][2];
    vector<string> accum;
    while (moveNumber) {
      string topElement = stackVec[moveFrom].top();
      accum.insert(accum.begin(), topElement);
      stackVec[moveFrom].pop();
      moveNumber--;
    }
    for (auto x : accum) {
      stackVec[moveTo].push(x);
    }
  }

  for (auto x : stackVec) {
    cout << x.top() << "\n";
  }
}

int main() {
  cout << "PART 1: ";
  part1();
  cout << "PART 2: ";
  part2();
  return 0;
}
