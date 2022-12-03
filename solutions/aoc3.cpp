#include <algorithm>
#include <bits/stdc++.h>
#include <cstdlib>
#include <unordered_map>

using namespace std;

void part1() {
  string myText;
  ifstream MyReadFile("../inputs/input3.txt");
  int sum = 0;
  while (getline(MyReadFile, myText)) {
    string firstHalf = myText.substr(0, myText.length() / 2);
    string secondHalf = myText.substr(myText.length() / 2);
    for (auto x : firstHalf) {
      int flag = 0;
      for (auto y : secondHalf) {
        if (x == y) {
          if (int(x) >= 65 && int(x) <= 90) {
            sum += int(x) - 38;
            flag = 1;
            break;
          } else if (int(x) >= 97 && int(x) <= 122) {
            sum += int(x) - 96;
            flag = 1;
            break;
          }
        }
      }
      if (flag != 0) {
        break;
      }
    }
  }
  cout << "sum = " << sum << "\n";
}

void part2() {
  string line1, line2, line3;
  ifstream MyReadFile("../inputs/input3.txt");
  int sum = 0;
  while (getline(MyReadFile, line1) && getline(MyReadFile, line2) &&
         getline(MyReadFile, line3)) {
    for (auto x : line1) {
      int flag = 0;
      for (auto y : line2) {
        if (x == y) {
          for (auto z : line3) {
            if (y == z) {
              if (int(x) >= 65 && int(x) <= 90) {
                sum += int(x) - 38;
                flag = 1;
                break;
              } else if (int(x) >= 97 && int(x) <= 122) {
                sum += int(x) - 96;
                flag = 1;
                break;
              }
            }
          }
        }
        if (flag != 0) {
          break;
        }
      }
      if (flag != 0) {
        break;
      }
    }
  }
  cout << sum << "\n";
}

int main() {
  cout << "PART 1: ";
  part1();
  cout << "PART 2: ";
  part2();
  return 0;
}
