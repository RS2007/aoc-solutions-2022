#include <bits/stdc++.h>
#include <fstream>
#include <string>

using namespace std;

void part1() {
    ifstream myReadFile("../inputs/input6.txt");
    string myText;
    while (getline(myReadFile, myText)) {
        for (int i = 0; i < myText.size() - 4; i++) {
            string microString = myText.substr(i, 4);
            unordered_map<char, bool> hashMap;
            bool flag = true;
            for (auto x: microString) {
                if (hashMap.find(x) != hashMap.end()) {
                    flag = false;
                    break;
                } else {
                    hashMap[x] = true;
                }
            }
            if (flag) {
                cout << i + 4 << "\n";
                break;
            }

        }
    }
}

void part2() {
    ifstream myReadFile("../inputs/input6.txt");
    string myText;
    while (getline(myReadFile, myText)) {
        for (int i = 0; i < myText.size() - 14; i++) {
            string microString = myText.substr(i, 14);
            unordered_map<char, bool> hashMap;
            bool flag = true;
            for (auto x: microString) {
                if (hashMap.find(x) != hashMap.end()) {
                    flag = false;
                    break;
                } else {
                    hashMap[x] = true;
                }
            }
            if (flag) {
                cout << i + 14 << "\n";
                break;
            }

        }
    }
}

int main() {
    cout << "PART 1: ";
    part1();
    cout << "PART 2: ";
    part2();
    return 0;
}
