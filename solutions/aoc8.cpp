#include <algorithm>
#include <bits/stdc++.h>
#include <fstream>
#include <string>
#include <unordered_map>

using namespace std;

vector<vector<int>> grid;

bool isEdge(int i, int j)
{
  return (i == 0 || i == grid.size() - 1 || j == 0 || j == grid[0].size() - 1);
}

bool isGreaterThanForwardPropogateMaxInCol(int i, int j)
{
  int maxTillNow = grid[i][j];
  for (int k = i + 1; k < grid.size(); ++k)
  {
    if (maxTillNow <= grid[k][j])
    {
      return false;
    }
  }
  return true;
}

int forwardPropScoreCol(int i, int j)
{
  if (i == grid.size() - 1)
  {
    return 0;
  }
  int current = grid[i][j];
  int score = 0;
  for (int k = i + 1; k < grid.size(); ++k)
  {
    if ((current <= grid[k][j]))
    {
      score++;
      return score;
    }
    else
    {
      score++;
    }
  }
  return score;
}

bool isGreaterThanForwardPropogateMaxInRow(int i, int j)
{
  int maxTillNow = grid[i][j];
  for (int k = j + 1; k < grid[0].size(); ++k)
  {
    if (maxTillNow <= grid[i][k])
    {
      return false;
    }
  }
  return true;
}

int forwardPropScoreRow(int i, int j)
{
  if (j == grid[0].size() - 1)
  {
    return 0;
  }
  int current = grid[i][j];
  int score = 0;
  for (int k = j + 1; k < grid[0].size(); ++k)
  {
    if ((current <= grid[i][k]))
    {
      score++;
      return score;
    }
    else
    {
      score++;
    }
  }
  return score;
}

bool isGreaterThanBackwardPropogateMaxInRow(int i, int j)
{
  int maxTillNow = grid[i][j];
  for (int k = 0; k < j; ++k)
  {
    if (maxTillNow <= grid[i][k])
    {
      return false;
    }
  }
  return true;
}

int backwardPropScoreRow(int i, int j)
{
  if (j == 0)
  {
    return 0;
  }
  int current = grid[i][j];
  int score = 0;
  for (int k = j - 1; k >= 0; --k)
  {
    if ((current < grid[i][k]))
    {
      return score;
    }
    else if (current == grid[i][k])
    {
      score++;
      return score;
    }
    else
    {
      score++;
    }
  }
  return score;
}

bool isGreaterThanBackwardPropogateMaxInCol(int i, int j)
{
  int maxTillNow = grid[i][j];
  for (int k = 0; k < i; ++k)
  {
    if (maxTillNow <= grid[k][j])
    {
      return false;
    }
  }
  return true;
}

int backwardPropScoreCol(int i, int j)
{
  if (i == 0)
  {
    return 0;
  }
  int current = grid[i][j];
  int score = 0;
  for (int k = i - 1; k >= 0; --k)
  {
    if ((current < grid[k][j]))
    {
      return score;
    }
    else if (current == grid[k][j])
    {
      score++;
      return score;
    }
    else
    {
      score++;
    }
  }
  return score;
}

void part1()
{
  int visible = 0;
  for (int i = 0; i < grid.size(); ++i)
  {
    for (int j = 0; j < grid[0].size(); ++j)
    {
      if (isEdge(i, j))
      {
        visible++;
      }
      else
      {
        if (isGreaterThanForwardPropogateMaxInRow(i, j) ||
            isGreaterThanForwardPropogateMaxInCol(i, j) ||
            isGreaterThanBackwardPropogateMaxInCol(i, j) ||
            isGreaterThanBackwardPropogateMaxInRow(i, j))
        {
          visible++;
        }
      }
    }
  }
  cout << visible << "\n";
}

void part2()
{
  int score = -1e9;
  for (int i = 0; i < grid.size(); ++i)
  {
    for (int j = 0; j < grid[0].size(); ++j)
    {
      score =
          max(score, (forwardPropScoreCol(i, j) * forwardPropScoreRow(i, j) *
                      backwardPropScoreCol(i, j) * backwardPropScoreRow(i, j)));
    }
  }
  cout << score << "\n";
}

int main()
{
  ifstream myReadFile("../inputs/input8.txt");

  string myText;
  while (getline(myReadFile, myText))
  {
    vector<int> layer;
    for (auto x : myText)
    {
      layer.push_back(atoi(&x));
    }
    grid.push_back(layer);
  }

  cout << "PART 1: ";
  part1();
  cout << "PART 2: ";
  part2();

  return 0;
}
