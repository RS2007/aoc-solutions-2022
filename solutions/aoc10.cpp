#include <bits/stdc++.h>
#include <climits>
#include <cwchar>
#include <fstream>
#include <string>

#define print(x) cout << x << "\n"

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

enum class Instruction_Type { NOOP, ADDX };
class Instruction {
public:
  Instruction_Type type;
  long long clock_cycles;
  long long arg;
};

class CRTMonitor {
public:
  array<array<char, 40>, 6> frameBuffer;

  CRTMonitor() {
    for (int i = 0; i < 6; ++i) {
      for (int j = 0; j < 40; ++j) {
        this->frameBuffer[i][j] = '.';
      }
    }
  }

  void drawPixel(int x, int y, char pixel) { 
    this->frameBuffer[y][x-1] = pixel; 
  }
  void display() {
    for (auto pixelArray : frameBuffer) {
      for (auto pixel : pixelArray) {
        cout << pixel;
      }
      cout << '\n';
    }
  }
};

class IMem {
public:
  vector<Instruction *> instructions;
};

class CPU {
public:
  long long pc;
  long long register_x;
  long long clock;
  bool in_between_execution;
  IMem *imem;
  long long cached_arg;
  CRTMonitor *monitor;
  CPU(IMem *imem) {
    this->pc = 0;
    this->register_x = 1;
    this->clock = 1;
    this->imem = imem;
    this->in_between_execution = false;
    this->cached_arg = 0;
    this->monitor = new CRTMonitor();
  }
  CPU(IMem *imem, CRTMonitor *monitor) {
    this->pc = 0;
    this->register_x = 1;
    this->clock = 1;
    this->imem = imem;
    this->in_between_execution = false;
    this->cached_arg = 0;
    this->monitor = monitor;
  }
  CPU(long long register_x, IMem *imem) {
    this->pc = 0;
    this->register_x = register_x;
    this->clock = 0;
    this->imem = imem;
  }
  void start(long long clk_to_stop) {
    while (1) {
      if (!this->step(clk_to_stop)) {
        return;
      }
    }
  }
  bool stepAndDraw(long long clk_to_stop) {
    Instruction *inst_to_exec = this->_fetch();
    if (inst_to_exec == nullptr)
      return false;
    if (this->in_between_execution) {
      if ((this->clock%40) >= this->register_x &&
          (this->clock%40) <= this->register_x + 2) {
        this->monitor->drawPixel(this->clock % 40, this->clock / 40, '#');
      } else {
        this->monitor->drawPixel(this->clock % 40, this->clock / 40, '.');
      }
      this->clock += 1;
      this->register_x += this->cached_arg;
      this->pc++;
      this->in_between_execution = false;
      this->cached_arg = 0;
      return true;
    }
    switch (inst_to_exec->type) {
    case Instruction_Type::NOOP: {
      if (this->clock >= clk_to_stop)
        return false;
      if ((this->clock%40) >= this->register_x  &&
          (this->clock%40) <= this->register_x + 2) {
        this->monitor->drawPixel(this->clock % 40, this->clock / 40, '#');
      } else {
        this->monitor->drawPixel(this->clock % 40, this->clock / 40, '.');
      }
      this->clock += 1;
      this->pc++;
      return true;
    }
    case Instruction_Type::ADDX: {
      if (this->clock >= clk_to_stop) {
        return false;
      }
      if ((this->clock%40) >= this->register_x &&
          (this->clock%40) <= this->register_x + 2) {
        this->monitor->drawPixel(this->clock % 40, this->clock / 40, '#');
      } else {
        this->monitor->drawPixel(this->clock % 40, this->clock / 40, '.');
      }
      this->in_between_execution = true;
      this->cached_arg = inst_to_exec->arg;
      this->clock++;
      return true;
    }
    default:
      assert("Unreachable");
    }
    return false;
    if (this->step(clk_to_stop)) {
      return true;
    }
    return false;
  }
  bool step(long long clk_to_stop) {
    Instruction *inst_to_exec = this->_fetch();
    if (inst_to_exec == nullptr)
      return false;
    if (this->in_between_execution) {
      this->clock += 1;
      this->register_x += this->cached_arg;
      this->pc++;
      this->in_between_execution = false;
      this->cached_arg = 0;
      return true;
    }
    switch (inst_to_exec->type) {
    case Instruction_Type::NOOP: {
      if (this->clock >= clk_to_stop)
        return false;
      this->clock += 1;
      this->pc++;
      return true;
    }
    case Instruction_Type::ADDX: {
      if (this->clock + 1 == clk_to_stop) {
        this->in_between_execution = true;
        this->cached_arg = inst_to_exec->arg;
        this->clock++;
        return false;
      }
      if (this->clock >= clk_to_stop) {
        return false;
      }
      this->clock += 2;
      this->register_x += inst_to_exec->arg;
      this->pc++;
      return true;
    }
    default:
      assert("Unreachable");
    }
    return false;
  }

private:
  Instruction *_fetch() {
    if (pc >= imem->instructions.size())
      return nullptr;
    return imem->instructions[pc];
  }
};

void part1() {
  ifstream myReadFile("../inputs/input10.txt");
  string myText;
  IMem instructions;
  while (getline(myReadFile, myText)) {
    vector<string> instruction_string = stringSplit(myText, ' ');
    Instruction *instruction = new Instruction();
    string instruction_debug;
    instruction_debug = myText;
    if (instruction_string[0] == "noop") {
      instruction->type = Instruction_Type::NOOP;
    } else if (instruction_string[0] == "addx") {
      instruction->type = Instruction_Type::ADDX;
      instruction->arg = atoi(instruction_string[1].c_str());
    }
    instructions.instructions.push_back(instruction);
  }
  CPU *cpu = new CPU(&instructions);
  array<int, 6> clk_stops = array<int, 6>{20, 60, 100, 140, 180, 220};
  long long signal_strength = 0;
  for (auto clk_stop : clk_stops) {
    cpu->start(clk_stop);
    cout << "Register x " << cpu->register_x << '\n';
    cout << "Clock " << cpu->clock << '\n';
    signal_strength += cpu->register_x * cpu->clock;
  }
  cout << "Signal strength: " << signal_strength << '\n';
}

void part2() {
  ifstream myReadFile("../inputs/input10.txt");
  string myText;
  IMem instructions;
  while (getline(myReadFile, myText)) {
    vector<string> instruction_string = stringSplit(myText, ' ');
    Instruction *instruction = new Instruction();
    string instruction_debug;
    instruction_debug = myText;
    if (instruction_string[0] == "noop") {
      instruction->type = Instruction_Type::NOOP;
    } else if (instruction_string[0] == "addx") {
      instruction->type = Instruction_Type::ADDX;
      instruction->arg = atoi(instruction_string[1].c_str());
    }
    instructions.instructions.push_back(instruction);
  }
  CPU *cpu = new CPU(&instructions);
  while (1) {
    if (!cpu->stepAndDraw(INT_MAX)) {
      break;
    }
  }
  cpu->monitor->display();
}

int main() {
  part1();
  part2();
  return 0;
}
