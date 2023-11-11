#include <algorithm>
#include <bits/stdc++.h>
#include <fstream>
#include <string>

using namespace std;


class Obstacle {

public:
  vector<pair<int, int>> structure;

  Obstacle(vector<pair<int, int>> structure) : structure(structure) {}
};

bool inBounds(int i, int boundLow, int boundHigh) {
  return i >= boundLow && i <= boundHigh;
}

int maxX = 0, minX = INT_MAX, maxY = 0, minY = 0;

Obstacle *tokenized(string input) {
  vector<pair<int, int>> structure;
  for (auto it = input.begin(); it < input.end();) {
    while (isspace(*it)) {
      it++;
      ;
    }
    int num1 = 0;
    while (*it >= '0' && *it <= '9') {
      num1 = num1 * 10 + (*it - '0');
      it++;
    }
    assert(*it == ',');
    it++;
    int num2 = 0;
    while (*it >= '0' && *it <= '9') {
      num2 = num2 * 10 + (*it - '0');
      it++;
    }
    maxX = max(maxX, num1);
    maxY = max(maxY, num2);
    minX = min(minX, num1);
    minY = min(minY, num2);
    structure.push_back(pair<int, int>{num1, num2});
    if (it == input.end()) {
      break;
    }
    it++;
    while (isspace(*it)) {
      it++;
    }
    assert(*it == '-');
    it++;
    assert(*it == '>');
    it++;
    while (isspace(*it)) {
      it++;
    }
  }
  return new Obstacle(structure);
}

void part1() {
  ifstream myReadFile("../inputs/input14.txt");
  string myText;
  vector<Obstacle *> obstacles;
  deque<deque<char>> grid;
  while (getline(myReadFile, myText)) {
#ifdef DEBUG
    cout << myText << "\n";
#endif
    Obstacle *tokens = tokenized(myText);
#ifdef DEBUG
    cout << "Length: " << tokens->structure.size() << "\n ";
#endif
    obstacles.push_back(tokens);
  }
  for (int i = minY; i <= maxY; ++i) {
    deque<char> row;
    for (int j = minX; j <= maxX; ++j) {
      row.push_back('.');
    }
    grid.push_back(row);
  }
#ifdef DEBUG
  cout << "minX: " << minX << " maxX: " << maxX << " minY: " << minY
       << " maxY: " << maxY << "\n";
#endif

  for (auto obstacle : obstacles) {
    for (int i = 0; i < obstacle->structure.size() - 1; ++i) {
      auto firstPoint = obstacle->structure[i];
      auto secondPoint = obstacle->structure[i + 1];
      if (firstPoint.first - secondPoint.first != 0) {
        for (int j = min(firstPoint.first, secondPoint.first);
             j <= max(firstPoint.first, secondPoint.first); j++) {
          grid[firstPoint.second][j - minX] = '#';
        }
      } else if (firstPoint.second - secondPoint.second != 0) {
        for (int j = min(firstPoint.second, secondPoint.second);
             j <= max(firstPoint.second, secondPoint.second); j++) {
          grid[j][firstPoint.first - minX] = '#';
        }
      } else {
        assert(0 && "Something went wrong");
      }
    }
  }

#ifdef Viz
  for (auto gridRow : grid) {
    for (auto gridElem : gridRow) {
      cout << gridElem << " ";
    }
    cout << "\n";
  }
#endif

  pair<int, int> sandStart = pair<int, int>{500, 0};
  pair<int, int> currentSandPosition = sandStart;
  int count = 0;
  while (true) {
    int xIndex = currentSandPosition.first - minX;
    int yIndex = currentSandPosition.second;
    pair<int, int> potentialCoordinates = currentSandPosition;

    std::array<bool, 3> encounteredBlock = {false, false, false};
    if (inBounds(yIndex + 1, minY, maxY) &&
        inBounds(minX + xIndex, minX, maxX)) {
      if (grid.at(yIndex + 1).at(xIndex) == '.') {
        potentialCoordinates = pair<int, int>{minX + xIndex, yIndex + 1};
        goto boundCheck;
      } else {
        encounteredBlock[0] = true;
      }
    }

    if (inBounds(yIndex + 1, minY, maxY) &&
        inBounds(minX + xIndex - 1, minX, maxX)) {
      if (grid.at(yIndex + 1).at(xIndex - 1) == '.') {
        potentialCoordinates = pair<int, int>{minX + xIndex - 1, yIndex + 1};
        goto boundCheck;
      } else {
        encounteredBlock[1] = true;
      }
    }

    if (inBounds(yIndex + 1, minY, maxY) &&
        inBounds(minX + xIndex + 1, minX, maxX)) {
      if (grid.at(yIndex + 1).at(xIndex + 1) == '.') {
        potentialCoordinates = pair<int, int>{minX + xIndex + 1, yIndex + 1};
        goto boundCheck;
      } else {
        encounteredBlock[2] = true;
      }
    }

    if (encounteredBlock[0] && encounteredBlock[1] && encounteredBlock[2]) {
      count += 1;
      grid[yIndex][xIndex] = 'o';
#ifdef VIZ
      cout << "CURRENT STATE: \n";
      for (auto gridRow : grid) {
        for (auto gridElem : gridRow) {
          cout << gridElem << " ";
        }
        cout << "\n";
      }
#endif
      currentSandPosition = sandStart;
      continue;
    } else {
      break;
    }

  boundCheck:
    if (!inBounds((potentialCoordinates).first, minX, maxX) ||
        !inBounds((potentialCoordinates).second, minY, maxY)) {
      break;
    };
    currentSandPosition = potentialCoordinates;
  }
  cout << "Part 1: " << count << "\n";
}

bool checkIfWithinPart2Bounds(pair<int, int> coordinates) {
  if (coordinates.second == maxY + 1) {
    return true;
  }
  return inBounds((coordinates).first, minX, maxX) &&
         inBounds((coordinates).second, minY, maxY);
}

deque<deque<char>> appendExtraX(deque<deque<char>> grid, int xIndex) {
  if (xIndex < 0) {
    int runner = xIndex;
    for (int i = 0; i < grid.size(); i++) {
      auto gridRow = &grid[i];
      while (runner != 0) {
        if (i == maxY) {
          gridRow->push_front('#');
        } else {
          gridRow->push_front('.');
        }
        runner++;
      }
      runner = xIndex;
    }
    minX = minX + xIndex;
    return grid;
  }

  if (xIndex > maxX - minX) {
    int originalDiff = xIndex - maxX + minX;
    int runner = originalDiff;
    for (int i = 0; i < grid.size(); ++i) {
      auto gridRow = &grid[i];
      while (runner != 0) {
        if (i == maxY) {
          gridRow->push_back('#');
        } else {
          gridRow->push_back('.');
        }
        runner--;
      }
      runner = originalDiff;
    }
    maxX = xIndex + minX;
    return grid;
  }
  return grid;
}

void part2(){
  ifstream myReadFile("../inputs/input14.txt");
  string myText;
  vector<Obstacle *> obstacles;
  while (getline(myReadFile, myText)) {
    Obstacle *tokens = tokenized(myText);
    obstacles.push_back(tokens);
  }
  deque<deque<char>> grid;

  for (int i = minY; i <= maxY; ++i) {
    deque<char> row;
    for (int j = minX; j <= maxX; ++j) {
      row.push_back('.');
    }
    grid.push_back(row);
  }
  grid[0][500 - minX] = '+';
  grid.push_back(deque<char>(grid[0].size(), '.'));
  grid.push_back(deque<char>(grid[0].size(), '#'));
  maxY = maxY + 2;

  for (auto obstacle : obstacles) {
    for (int i = 0; i < obstacle->structure.size() - 1; ++i) {
      auto firstPoint = obstacle->structure[i];
      auto secondPoint = obstacle->structure[i + 1];
      if (firstPoint.first - secondPoint.first != 0) {
        for (int j = min(firstPoint.first, secondPoint.first);
             j <= max(firstPoint.first, secondPoint.first); j++) {
          grid[firstPoint.second][j - minX] = '#';
        }
      } else if (firstPoint.second - secondPoint.second != 0) {
        for (int j = min(firstPoint.second, secondPoint.second);
             j <= max(firstPoint.second, secondPoint.second); j++) {
          grid[j][firstPoint.first - minX] = '#';
        }
      } else {
        assert(0 && "Something went wrong");
      }
    }
  }

  pair<int, int> sandStart = pair<int, int>{500, 0};
  pair<int, int> currentSandPosition = sandStart;
  int count = 0;
  while (true) {
    int xIndex = currentSandPosition.first - minX;
    int yIndex = currentSandPosition.second;
    if (yIndex == maxY + 1) {
      break;
    }
    pair<int, int> potentialCoordinates = currentSandPosition;

    std::array<bool, 3> encounteredBlock = {false, false, false};
    if (inBounds(yIndex + 1, minY, maxY)) {
      grid = appendExtraX(grid, xIndex);
      xIndex = currentSandPosition.first - minX;
      if (grid.at(yIndex + 1).at(xIndex) == '.') {
        potentialCoordinates = pair<int, int>{minX + xIndex, yIndex + 1};
        goto boundCheck;
      } else {
        encounteredBlock[0] = true;
      }
    }

    if (inBounds(yIndex + 1, minY, maxY)) {
      grid = appendExtraX(grid, xIndex - 1);
      xIndex = currentSandPosition.first - minX;
      if (grid.at(yIndex + 1).at(xIndex - 1) == '.') {
        potentialCoordinates = pair<int, int>{minX + xIndex - 1, yIndex + 1};
        goto boundCheck;
      } else {
        encounteredBlock[1] = true;
      }
    }

    if (inBounds(yIndex + 1, minY, maxY)) {
      grid = appendExtraX(grid, xIndex + 1);
      xIndex = currentSandPosition.first - minX;
      if (grid.at(yIndex + 1).at(xIndex + 1) == '.') {
        potentialCoordinates = pair<int, int>{minX + xIndex + 1, yIndex + 1};
        goto boundCheck;
      } else {
        encounteredBlock[2] = true;
      }
    }

    if (encounteredBlock[0] && encounteredBlock[1] && encounteredBlock[2]) {
      count += 1;
      if (grid[yIndex][xIndex] == '+') {
        break;
      }
      grid[yIndex][xIndex] = 'o';
#ifdef VIZ
      cout << "CURRENT STATE: \n";
      for (auto gridRow : grid) {
        for (auto gridElem : gridRow) {
          cout << gridElem << " ";
        }
        cout << "\n";
      }
#endif
      currentSandPosition = sandStart;
      continue;
    } else {
      break;
    }

  boundCheck:
    if (!inBounds((potentialCoordinates).first, minX, maxX) ||
        !inBounds((potentialCoordinates).second, minY, maxY)) {
      break;
    };
    currentSandPosition = potentialCoordinates;
  }
  cout << "Part 2: " << count << "\n";
}

int main() {
  part1();
  part2();
  // PART2: Takes infinite time, should optimize

  // parse, while parsing keep track of min and max that would be the edges
  // construct a 2d vector based on that
  // state machine:
  //  if you can move down, then move down
  //  else check diagonal left, move there
  //  if not check diagonal right, move there
  //  if none, stay there update the position in the 2d vector
  //  in case of overflow in the vector, break and return the sand units
  //  that have fallen
  return 0;
}
