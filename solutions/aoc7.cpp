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

class Node {
public:
  vector<Node *> children;
  Node *parent;
  bool isRoot;
  bool isDir;
  int size;
  bool sizeKnown;
  string name;
  bool visited;

  Node(bool isDir, bool isRoot, string name) {
    this->sizeKnown = false;
    this->isRoot = isRoot;
    this->isDir = isDir;
    this->name = name;
    this->sizeKnown = false;
    this->size = 0;
  }

  Node(bool isDir, bool isRoot, string name, int size, Node *parent) {
    this->sizeKnown = false;
    this->isRoot = isRoot;
    this->isDir = isDir;
    this->name = name;
    this->size = size;
    this->sizeKnown = true;
    this->parent = parent;
  }

  Node(bool isDir, bool isRoot, string name, Node *parent) {
    this->sizeKnown = false;
    this->isRoot = isRoot;
    this->isDir = isDir;
    this->name = name;
    this->sizeKnown = true;
    this->parent = parent;
    this->size = 0;
  }

  Node *findChild(string name) {
    for (auto x : children) {
      if (x->name == name) {
        return x;
      }
    }
    cout << "Error";
    exit(-1);
    return new Node(false, false, "Erorr");
  }

  void pushChildren(Node *child) { children.push_back(child); }
};

int populateTreeSizes(Node *root) {
  if (!root->isDir) {
    assert(root->sizeKnown && root->size != 0);
    return root->size;
  } else {
    for (auto x : root->children) {
      root->size += populateTreeSizes(x);
    }
    root->sizeKnown = true;
    return root->size;
  }
}

void dfs(Node *root, int *minSizeTillNow, int mustBeGreaterThan) {
  if (!root->isDir) {
    return;
  } else {
    if ((mustBeGreaterThan + root->size > 30000000) &&
        (root->size < (*minSizeTillNow))) {
      *minSizeTillNow = root->size;
    }
    for (auto x : root->children) {
      dfs(x, minSizeTillNow, mustBeGreaterThan);
    }
  }
}

int returnSizeOfSmallestDirCanBeDeleted(Node *root) {
  int minSizeTillNow = 1e9;
  int totalSpaceUsed = root->size;
  dfs(root, &minSizeTillNow, 70000000 - totalSpaceUsed);
  return minSizeTillNow;
}

int accum = 0;

void numDirectoriesWithSumLesserOrEqualTo100k(Node *root) {
  if (root->children.size() == 0) {
    return;
  } else {
    if ((root->size <= 100000)) {
      accum += root->size;
    }
    for (auto x : root->children) {
      numDirectoriesWithSumLesserOrEqualTo100k(x);
    }
  }
}

void part1() {
  ifstream myReadFile("../inputs/input7.txt");
  string myText;
  Node *root = new Node(true, true, "/");
  Node *currParent = root;
  while (getline(myReadFile, myText)) {
    vector<string> tokens = stringSplit(myText, ' ');

    if (tokens[1] == "cd") {
      if (tokens[2] == "/") {
        continue;
      } else if (tokens[2] == "..") {
        currParent = currParent->parent;
        continue;
      } else {
        currParent = currParent->findChild(tokens[2]);
      }
    } else if (tokens[1] == "ls") {
      continue;
    } else {
      if (tokens[0] == "dir") {
        Node *node = new Node(true, false, tokens[1], currParent);
        currParent->pushChildren(node);
      } else {
        Node *node = new Node(false, false, tokens[1], atoi(tokens[0].c_str()),
                              currParent);
        currParent->pushChildren(node);
      }
    }
  }
  populateTreeSizes(root);
  numDirectoriesWithSumLesserOrEqualTo100k(root);
  cout << accum << "\n";
}

void part2() {
  ifstream myReadFile("../inputs/input7.txt");
  string myText;
  Node *root = new Node(true, true, "/");
  Node *currParent = root;
  while (getline(myReadFile, myText)) {
    vector<string> tokens = stringSplit(myText, ' ');

    if (tokens[1] == "cd") {
      if (tokens[2] == "/") {
        continue;
      } else if (tokens[2] == "..") {
        currParent = currParent->parent;
        continue;
      } else {
        currParent = currParent->findChild(tokens[2]);
      }
    } else if (tokens[1] == "ls") {
      continue;
    } else {
      if (tokens[0] == "dir") {
        Node *node = new Node(true, false, tokens[1], currParent);
        currParent->pushChildren(node);
      } else {
        Node *node = new Node(false, false, tokens[1], atoi(tokens[0].c_str()),
                              currParent);
        currParent->pushChildren(node);
      }
    }
  }
  populateTreeSizes(root);
  cout << returnSizeOfSmallestDirCanBeDeleted(root) << "\n";
}

int main() {
  cout << "PART 1: ";
  part1();
  cout << "PART 2: ";
  part2();
  return 0;
}
