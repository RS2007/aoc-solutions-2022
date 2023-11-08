#include <bits/stdc++.h>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <string>

using namespace std;

enum class ValueType { INT, LIST };

#define todo() assert(false && "Not implemented")

class Object {
public:
  ValueType type;
  int valueInt;
  vector<Object *> valueList;
  Object(int value) : valueInt(value), type(ValueType::INT) {}
  Object(vector<Object *> value) : valueList(value), type(ValueType::LIST) {}
};

class Parser {
public:
  string::iterator it;
  string input;

  Parser(string in) {
    this->input = in;
    this->it = this->input.begin();
  }
  void skipWhitespace() {
    while (isspace(*it)) {
      it++;
    }
  }

  Object *parseObject() {
    skipWhitespace();
    switch (*it) {
    case '[': {
      return parseList();
    }
    default: {
      if (!isdigit(*it)) {
        assert(false && "Shouldn't hit this");
      }
      return parseInteger();
    }
    }
  }

  Object *parseList() {
    it++;
    skipWhitespace();
    vector<Object *> objVector;
    while (*it != ']') {
      auto obj = parseObject();
      objVector.push_back(obj);
      if (*it == ',') {
        it++;
        skipWhitespace();
      }
    }
    if (it != input.end()) {
      it++;
    }
    return new Object(objVector);
    // parses lists and goes to the right square bracket or next comma
  }

  Object *parseInteger() {
    int num = 0;
    while (*it >= '0' && *it <= '9') {
      num = num * 10 + (*it - '0');
      it++;
    }
    auto obj = new Object(num);
    skipWhitespace();
    return obj;
    // parses integer and goes to the next comma or right square bracket
  }
};

enum class Ord { Equal, Lesser, Greater };

Ord numCompare(int a, int b) {
  if (a == b)
    return Ord::Equal;
  if (a < b)
    return Ord::Lesser;
  return Ord::Greater;
}

Ord boolToOrd(bool val) { return val ? Ord::Lesser : Ord::Greater; }

Ord compareObject(Object *first, Object *second) {
  if (first->type == ValueType::INT && second->type == ValueType::INT) {
    return numCompare(first->valueInt, second->valueInt);
  } else if (first->type == ValueType::INT && second->type == ValueType::LIST) {
    Object *firstCastToList = new Object(vector<Object *>{first});
    return compareObject(firstCastToList, second);
  } else if (first->type == ValueType::LIST && second->type == ValueType::INT) {
    Object *secondCastToList = new Object(vector<Object *>{second});
    return compareObject(first, secondCastToList);
  } else {
    if (first->valueList.size() > second->valueList.size()) {
      for (auto i = 0; i < second->valueList.size(); i++) {
        auto ord = compareObject(first->valueList[i], second->valueList[i]);
        if (ord == Ord::Equal) {
          continue;
        }
        return ord;
      }
      return Ord::Greater;
    } else {
      for (auto i = 0; i < first->valueList.size(); i++) {
        auto ord = compareObject(first->valueList[i], second->valueList[i]);
        if (ord == Ord::Equal) {
          continue;
        }
        return ord;
      }
      if (first->valueList.size() == second->valueList.size())
        return Ord::Equal;
      return Ord::Lesser;
    }
  }
}

void part1() {
  ifstream myReadFile("../inputs/input13.txt");
  string myText;
  long accum = 0;
  int index = 1;
  while (getline(myReadFile, myText)) {
    if (myText.size() == 0)
      continue;
    Parser *parser = new Parser(myText);
    Object *first = parser->parseObject();
    getline(myReadFile, myText);
    delete parser;
    parser = new Parser(myText);
    Object *second = parser->parseObject();
    switch (compareObject(first, second)) {
    case Ord::Lesser:
      accum += index;
      break;
    case Ord::Greater:
      // cout << "Greater: " << index << "\n";
      break;
    case Ord::Equal:
      assert(false && "Ord should not be equal");
    }
    delete first;
    delete second;
    delete parser;
    index++;
  }
  cout << accum << "\n";
}

bool customCmp(Object *first, Object *second) {
  switch (compareObject(first, second)) {
  case Ord::Lesser:
    return true;
  case Ord::Greater:
    return false;
  default:
    assert(false && "Shouldn't hit this");
  }
}

void part2() {
  ifstream myReadFile("../inputs/input13.txt");
  string myText;
  vector<Object *> container;
  while (getline(myReadFile, myText)) {
    if (myText.size() == 0)
      continue;
    Parser *parser = new Parser(myText);
    container.emplace_back(parser->parseObject());
    delete parser;
  }
  Object *pointer1 = new Object(vector<Object *>{new Object(2)});
  Object *pointer2 = new Object(vector<Object *>{new Object(6)});
  container.emplace_back(pointer1);
  container.emplace_back(pointer2);
  sort(container.begin(), container.end(), customCmp);
  int index1 =
      find(container.begin(), container.end(), pointer1) - container.begin();
  int index2 =
      find(container.begin(), container.end(), pointer2) - container.begin();
  delete pointer1->valueList[0];
  delete pointer2->valueList[0];
  delete pointer1;
  delete pointer2;
  cout << (index1 + 1) * (index2 + 1);
}

int main() {
  part1();
  part2();
  return 0;
}
