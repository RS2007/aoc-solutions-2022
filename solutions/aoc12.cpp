#include <bits/stdc++.h>
#include <fstream>
#include <string>

#define TEST 0
#define PART2

using namespace std;

bool inBounds(int i,int boundLow,int boundHigh){
  return i >= boundLow && i < boundHigh;
}


int dijikistra(vector<vector<char>> graph,int startNodeI,int startNodeJ,int endNodeI,int endNodeJ){
  vector<vector<bool>> seen;
  for(auto graphRow: graph){
    vector<bool> seenRow;
    for(auto graphElem: graphRow){
      seenRow.push_back(false);
    }
    seen.push_back(seenRow);
  }
  seen[startNodeI][startNodeJ] = true;
  int currentI = startNodeI,currentJ = startNodeJ;
  queue<pair<int,int>> bfsQueue;
  vector<pair<int,int>> directions= {{-1,0},{0,-1},{1,0},{0,1}};
  for(auto direction: directions){
    if(inBounds(currentI+direction.first,0,graph.size()) && inBounds(currentJ+direction.second,0,graph[0].size()) && !seen[currentI+direction.first][currentJ+direction.second] ){
      #ifdef PART1
      if((currentI == startNodeI && currentJ == startNodeJ) || (graph[currentI+direction.first][currentJ+direction.second] <= (char)((int)graph[currentI][currentJ]+1))){
        bfsQueue.push(pair<int,int>({currentI+direction.first,currentJ+direction.second}));
        seen[currentI+direction.first][currentJ+direction.second] = true;
      }
      #endif
      #ifdef PART2
      if(graph[currentI+direction.first][currentJ+direction.second] <= (char)((int)graph[currentI][currentJ]+1)){
        bfsQueue.push(pair<int,int>({currentI+direction.first,currentJ+direction.second}));
        seen[currentI+direction.first][currentJ+direction.second] = true;
      }
      #endif
    }
  }
  int level = 0;
  while(!bfsQueue.empty()){
    level+=1;
    size_t current_q_len = bfsQueue.size();
    for(int i = 0; i < current_q_len;i++){
      pair<int,int> currentNode = bfsQueue.front();
      for(auto direction: directions){
        if(inBounds(currentNode.first+direction.first,0,graph.size()) && inBounds(currentNode.second+direction.second,0,graph[0].size()) && !seen[currentNode.first+direction.first][currentNode.second+direction.second]){
          if(graph[currentNode.first+direction.first][currentNode.second+direction.second] <= (char)((int)graph[currentNode.first][currentNode.second]+1)){
            if(graph[currentNode.first+direction.first][currentNode.second+direction.second] != 'E' || (graph[currentNode.first][currentNode.second] == 'z')){
              bfsQueue.push(pair<int,int>({currentNode.first+direction.first,currentNode.second+direction.second}));
              seen[currentNode.first+direction.first][currentNode.second+direction.second] = true;
            }
          }
        }
      }
      bfsQueue.pop();
      if(currentNode.first == endNodeI && currentNode.second == endNodeJ){
        return level;
      }
    }
  }
  return INT_MAX;
}

pair<int,int> find_index_in_graph(vector<vector<char>> graph,char target){
  int i = 0,j=0;
  for(auto charVector: graph){
    j=0;
    for(auto character: charVector){
      if(character == target){
        goto firstLoopEnd;
      }
      j+=1;
    }
    i+=1;
  }

  firstLoopEnd:
  return pair<int,int>{i,j};
}

vector<pair<int,int>> find_indexes_in_graph(vector<vector<char>> graph,char target){
  int i = 0,j=0;
  vector<pair<int,int>> ans;
  for(auto charVector: graph){
    j=0;
    for(auto character: charVector){
      if(character == target){
        ans.push_back(pair<int,int>{i,j});
      }
      j+=1;
    }
    i+=1;
  }

  return ans;
}

int main() {
  #if TEST
  ifstream myReadFile("../inputs/input12.dev.txt");
  #else
  ifstream myReadFile("../inputs/input12.txt");
  #endif
  string myText;
  vector<vector<char>> graph;
  while (getline(myReadFile, myText)) {
    vector<char> charVector;
    for(auto it = myText.begin(); it < myText.end();it++){
      charVector.push_back(*it);
    }
    graph.push_back(charVector);
  }
  pair<int,int> startIndexes = find_index_in_graph(graph,'S');
  pair<int,int> endIndexes = find_index_in_graph(graph,'E');
  vector<pair<int,int>> allIndexesWitha = find_indexes_in_graph(graph,'a');
  allIndexesWitha.push_back(startIndexes);
  #if TEST
  cout <<"Start I: " << startIndexes.first << " Start J: " << startIndexes.second << "\n";
  cout <<"End I: " << endIndexes.first << " End J: " << endIndexes.second << "\n";
  #endif
  int total_shortest_distance = INT_MAX;
  #ifdef PART1
    int shortest_distance = dijikistra(graph,startIndexes.first,startIndexes.second,endIndexes.first,endIndexes.second);
    cout << "shortest distance: " << shortest_distance << "\n";
  #endif
  #ifdef PART2
  for(auto indexWithA: allIndexesWitha){
    int shortest_distance = dijikistra(graph,indexWithA.first,indexWithA.second,endIndexes.first,endIndexes.second);
    #if TEST
      cout << "shortest distance: " << shortest_distance << "\n";
    #endif
    total_shortest_distance = min(shortest_distance,total_shortest_distance);
  }
  cout << total_shortest_distance << "\n";
  #endif
  return 0;
}
