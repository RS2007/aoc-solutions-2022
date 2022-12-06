#include <bits/stdc++.h>
#include <fstream>
#include <string>

using namespace std;

int main() {
  ifstream myReadFile("../inputs/template.txt");
  string myText;
  while (getline(myReadFile, myText)) {
    cout << myText << "\n";
  }
  return 0;
}
