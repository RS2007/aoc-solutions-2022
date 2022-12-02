#include <algorithm>
#include <bits/stdc++.h>
#include <cstdlib>

using namespace std;

void part1() {
  string myText;
  ifstream MyReadFile("../inputs/input1.txt");
  int maxTillNow = -1;
  int count = 0;
  int index = 1;
  int maxIndex = -1;
  while (getline(MyReadFile, myText)) {
    if (myText.empty()) {
      if (count > maxTillNow) {
        maxTillNow = count;
        maxIndex = index;
      }
      maxTillNow = max(maxTillNow, count);
      count = 0;
      index++;
    } else {
      count += atoi(myText.c_str());
    }
  }
  cout << maxTillNow << "\n";
  MyReadFile.close();
}

void part2() {
  string myText;
  ifstream MyReadFile("../inputs/input1.txt");
  vector<int> calorieCount;
  int count = 0;
  while (getline(MyReadFile, myText)) {
    if (myText.empty()) {
      calorieCount.push_back(count);
      count = 0;
    } else {
      count += atoi(myText.c_str());
    }
  }
  sort(calorieCount.begin(), calorieCount.end(), greater<int>());
  int sum = 0;
  for (int i = 0; i < 3; ++i) {
    sum += calorieCount[i];
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
