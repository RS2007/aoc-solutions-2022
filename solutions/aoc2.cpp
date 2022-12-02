#include <algorithm>
#include <bits/stdc++.h>
#include <cstdlib>
#include <unordered_map>

using namespace std;

vector<string> simple_tokenizer(string s) {
  stringstream ss(s);
  string word;
  vector<string> returnvector;
  while (ss >> word) {
    returnvector.push_back(word);
  }
  return returnvector;
}

void part1() {
  string myText;
  ifstream MyReadFile("../inputs/input2.txt");
  unordered_map<string, string> antagonistPairMap = {
      {"Y", "C"},
      {"Z", "A"},
      {"X", "B"},
  };
  unordered_map<string, string> similarPairMap = {
      {"X", "A"},
      {"Y", "B"},
      {"Z", "C"},
  };
  unordered_map<string, int> pointsMap = {
      {"X", 1},
      {"Y", 2},
      {"Z", 3},
  };
  int score = 0;
  while (getline(MyReadFile, myText)) {
    vector<string> arr = simple_tokenizer(myText);
    if (similarPairMap[arr[1]] == arr[0]) {
      score += 3 + pointsMap[arr[1]];
    } else if (antagonistPairMap[arr[1]] == arr[0]) {
      score += pointsMap[arr[1]];
    } else {
      score += 6 + pointsMap[arr[1]];
    }
  }
  cout << score << "\n";
}

void part2() {
  string myText;
  ifstream MyReadFile("../inputs/input2.txt");
  unordered_map<string, string> loosePairMap = {
      {"A", "Z"},
      {"B", "X"},
      {"C", "Y"},
  };
  unordered_map<string, string> winPairMap = {
      {"A", "Y"},
      {"B", "Z"},
      {"C", "X"},
  };
  unordered_map<string, string> drawPairMap = {
      {"A", "X"},
      {"B", "Y"},
      {"C", "Z"},
  };
  unordered_map<string, int> eventPointMap = {
      {"X", 0},
      {"Y", 3},
      {"Z", 6},
  };
  unordered_map<string, int> pointsMap = {
      {"X", 1},
      {"Y", 2},
      {"Z", 3},
  };
  int score = 0;
  while (getline(MyReadFile, myText)) {
    vector<string> arr = simple_tokenizer(myText);
    if (arr[1] == "X") {
      score += pointsMap[loosePairMap[arr[0]]];
    } else if (arr[1] == "Y") {
      score += pointsMap[drawPairMap[arr[0]]] + 3;
    } else if (arr[1] == "Z") {
      score += pointsMap[winPairMap[arr[0]]] + 6;
    }
  }
  cout << score << "\n";
}

int main() {
  cout << "Part 1: ";
  part1();
  cout << "Part 2: ";
  part2();
  return 0;
}
