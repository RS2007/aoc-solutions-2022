#include <bits/stdc++.h>
#include <cstdlib>
#include <fstream>
#include <string>

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

void part1() {
  ifstream myReadFile("../inputs/input4.txt");
  string myText;
  int count = 0;
  while (getline(myReadFile, myText)) {
    if (myText.size() == 0) {
      continue;
    }
    vector<string> a = stringSplit(myText, ',');
    int firstStart, secondStart, firstEnd, secondEnd;

    vector<string> firstElf = stringSplit(a[0], '-');
    vector<string> secondElf = stringSplit(a[1], '-');

    firstStart = atoi(firstElf[0].c_str());
    firstEnd = atoi(firstElf[1].c_str());

    secondStart = atoi(secondElf[0].c_str());
    secondEnd = atoi(secondElf[1].c_str());
    if ((firstStart <= secondStart && firstEnd >= secondEnd) ||
        (secondStart <= firstStart) && (secondEnd >= firstEnd)) {
      count++;
    }
  }

  cout << count << "\n";
}

void part2() {
  ifstream myReadFile("../inputs/input4.txt");
  string myText;
  int count = 0;
  while (getline(myReadFile, myText)) {
    if (myText.size() == 0) {
      continue;
    }
    vector<string> a = stringSplit(myText, ',');
    int firstStart, secondStart, firstEnd, secondEnd;

    vector<string> firstElf = stringSplit(a[0], '-');
    vector<string> secondElf = stringSplit(a[1], '-');

    firstStart = atoi(firstElf[0].c_str());
    firstEnd = atoi(firstElf[1].c_str());

    secondStart = atoi(secondElf[0].c_str());
    secondEnd = atoi(secondElf[1].c_str());
    if (!((firstStart > secondEnd) || (firstEnd < secondStart))) {
      count++;
    }
  }

  cout << count << "\n";
}

int main() {
  cout << "PART 1: ";
  part1();
  cout << "PART 2: ";
  part2();
  return 0;
}
