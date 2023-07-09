#include <bits/stdc++.h>
#include <cassert>
#include <fstream>
#include <string>

using namespace std;

class Node {
public:
  bool isHead;
  bool isTail;
  int id;
  int x;
  int y;
  Node(int id) {
    this->id = id;
    this->isHead = (id == 0);
    this->x = 0;
    this->y = 0;
  }
  void change_position(int delta_x, int delta_y) {
    this->x += delta_x;
    this->y += delta_y;
  }
};

class Snake {
public:
  int size;
  vector<Node *> nodes;
  Snake(int size) {
    for (int i = 0; i < size; ++i) {
      Node *new_node = new Node(i);
      this->nodes.push_back(new_node);
    }
    this->nodes[size - 1]->isTail = true;
    this->size = size;
  }
  void moveSnake(int deltaX, int deltaY, int mag, int running_id) {
    while (mag > 0) {
      if (running_id >= size - 1) {
        mag--;
        running_id = 0;
      }
      nodes[running_id]->change_position(deltaX, deltaY);
      if (abs(nodes[running_id]->x - nodes[running_id + 1]->x) > 1 ||
          abs(nodes[running_id]->y - nodes[running_id + 1]->y) > 1) {
        if (nodes[running_id + 1]->x == nodes[running_id]->x) {
          moveSnake(0, 1, mag, running_id + 1);
        } else if (nodes[running_id + 1]->y - nodes[running_id]->y == 1) {
          moveSnake(1, 0, mag, running_id + 1);
        } else {
          if (abs(deltaX)) {
            moveSnake(deltaX, nodes[running_id]->y - nodes[running_id + 1]->y,
                      mag, running_id + 1);
          } else if (abs(deltaY)) {
            moveSnake(nodes[running_id]->x - nodes[running_id + 1]->x, deltaY,
                      mag, running_id + 1);
          }
        }
      }
      mag--;
    }
  }
  void moveHead(string dir, int mag, map<pair<int, int>, bool> &visited) {
    for (int i = 0; i < mag; ++i) {
      if (dir == "R")
        nodes[0]->x += 1;
      else if (dir == "U")
        nodes[0]->y += 1;
      else if (dir == "D")
        nodes[0]->y -= 1;
      else if (dir == "L")
        nodes[0]->x -= 1;
      for (int j = 1; j < nodes.size(); ++j) {
        moveNodeSingleStep(j, visited);
      }
    }
  }

  void moveNodeSingleStep(int i, map<pair<int, int>, bool> &visited) {
    pair<int, int> diff_v(nodes[i - 1]->x - nodes[i]->x,
                          nodes[i - 1]->y - nodes[i]->y);
    int diff_x = 0;
    int diff_y = 0;
    if (diff_v == pair<int, int>(0, 0)) {
      diff_x = 0;
      diff_y = 0;
    } else if (diff_v == pair<int, int>(1, 0) ||
               diff_v == pair<int, int>(1, 1) ||
               diff_v == pair<int, int>(-1, 1) ||
               diff_v == pair<int, int>(-1, 0) ||
               diff_v == pair<int, int>(-1, -1) ||
               diff_v == pair<int, int>(0, -1) ||
               diff_v == pair<int, int>(1, -1) ||
               diff_v == pair<int, int>(0, 1)) {
      diff_x = 0;
      diff_y = 0;
    } else if (diff_v == pair<int, int>(2, 0)) {
      diff_x = 1;
      diff_y = 0;
    } else if (diff_v == pair<int, int>(2, 1) ||
               diff_v == pair<int, int>(2, 2) ||
               diff_v == pair<int, int>(1, 2)) {
      diff_x = 1;
      diff_y = 1;
    } else if (diff_v == pair<int, int>(0, 2)) {
      diff_x = 0;
      diff_y = 1;
    } else if (diff_v == pair<int, int>(-1, 2) ||
               diff_v == pair<int, int>(-2, 2) ||
               diff_v == pair<int, int>(-2, 1)) {
      diff_x = -1;
      diff_y = 1;
    } else if (diff_v == pair<int, int>(-2, 0)) {
      diff_x = -1;
      diff_y = 0;
    } else if (diff_v == pair<int, int>(-1, -2) ||
               diff_v == pair<int, int>(-2, -2) ||
               diff_v == pair<int, int>(-2, -1)) {
      diff_x = -1;
      diff_y = -1;
    } else if (diff_v == pair<int, int>(0, -2)) {
      diff_x = 0;
      diff_y = -1;
    } else if (diff_v == pair<int, int>(1, -2) ||
               diff_v == pair<int, int>(2, -2) ||
               diff_v == pair<int, int>(2, -1)) {
      diff_x = 1;
      diff_y = -1;
    }
    nodes[i]->x += diff_x;
    nodes[i]->y += diff_y;
    if (i == 9) {
      visited[pair<int, int>{nodes[i]->x, nodes[i]->y}] = true;
    }
  }
};

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

unordered_map<string, pair<int, int>> moves = {
    {"R", pair<int, int>{1, 0}},
    {"U", pair<int, int>{0, 1}},
    {"L", pair<int, int>{-1, 0}},
    {"D", pair<int, int>{0, -1}},
};

map<pair<int, int>, bool> visited;

void part1() {
  visited[pair<int, int>{0, 0}] = true;
  ifstream myReadFile("../inputs/input9.txt");
  string myText;
  pair<int, int> currentHPos = {0, 0};
  pair<int, int> currentTPos = {0, 0};
  while (getline(myReadFile, myText)) {
    vector<string> dirMag = stringSplit(myText, ' ');
    pair<int, int> dirChangePair = moves[dirMag[0]];
    int magnitude = atoi(dirMag[1].c_str());
    while (magnitude) {
      int deltaX = dirChangePair.first;
      int deltaY = dirChangePair.second;
      currentHPos.first += deltaX;
      currentHPos.second += deltaY;
      // bool shouldWeMoveTheTail
      if (abs(currentTPos.first - currentHPos.first) > 1 ||
          abs(currentTPos.second - currentHPos.second) > 1) {
        if (currentTPos.first == currentHPos.first) {
          currentTPos.second += deltaY;
        } else if (currentTPos.second == currentHPos.second) {
          currentTPos.first += deltaX;
        } else {
          if (abs(deltaX)) {
            currentTPos.second = currentHPos.second;
            currentTPos.first += deltaX;
          } else if (abs(deltaY)) {
            currentTPos.first = currentHPos.first;
            currentTPos.second += deltaY;
          }
        }
        if (visited.find(pair<int, int>{currentTPos.first,
                                        currentTPos.second}) == visited.end()) {
          visited[pair<int, int>{currentTPos.first, currentTPos.second}] = true;
        }
      }
      magnitude--;
    }
  }
  cout << visited.size() << "\n";
}

void part2() {
  visited[pair<int, int>{0, 0}] = true;
  ifstream myReadFile("../inputs/input9.txt");
  string myText;
  Snake *snake = new Snake(10);
  while (getline(myReadFile, myText)) {
    vector<string> move = stringSplit(myText, ' ');
    string dir = move[0];
    int mag = atoi(move[1].c_str());
    snake->moveHead(dir, mag, visited);
  }
  cout << visited.size() << "\n";
}

int main() {
  part2();
  return 0;
}
