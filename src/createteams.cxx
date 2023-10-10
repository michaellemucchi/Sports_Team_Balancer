#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "json.hpp"
#include "priorityqueue.h"

using namespace std;

int main(int argc, char** argv) {
  nlohmann::json jsonObject1, jsonObject2, jsonObject3;
  vector<int> storage;
  ifstream file;
  file.open(argv[1]);
  if (file.is_open()) {
    file >> jsonObject1;
  }
  file.close();
  int numPlayers = jsonObject1["metadata"]["numPlayers"];
  PriorityQueue Heap(numPlayers * numPlayers);
  int playerOne;
  int playerTwo;
  size_t playerCount = 0;
  for (auto itr = jsonObject1["teamStats"].begin(); itr != jsonObject1["teamStats"].end(); itr++) {
    double winPercentage = itr.value()["winPercentage"];
    double difference = fabs(50.000 - winPercentage);
    playerOne = itr.value()["playerOne"];
    playerTwo = itr.value()["playerTwo"];
    KeyValuePair temp = make_pair(difference, make_pair(playerOne, playerTwo));
    Heap.insert(temp);
    playerCount++;
  }
  for (size_t i = 1; i <= playerCount; i++) {
    KeyValuePair temp = Heap.removeMin();
    Value players = temp.second;
    int counter = 0;
    //cout << temp.first << " : " << players.first << ", " << players.second << endl;
    //cout << "current players:" << endl << "player one: " << players.first << endl << "player two: " << players.second << endl << endl;
    if (i == 1) {
      jsonObject2["teams"].push_back(players);
      storage.push_back(players.first);
      //cout << "entering in " << players.first << endl;
      storage.push_back(players.second);
      //cout << "entering in " << players.second << endl;
    }
    else {
      if (find(storage.begin(), storage.end(), players.first) != storage.end()) {
        counter++;
      }
      if (find(storage.begin(), storage.end(), players.second) != storage.end()) {
        counter++;
      }
      if (counter == 0) {
        jsonObject2["teams"].push_back(players);
        storage.push_back(players.first);
        storage.push_back(players.second);
      }
    }
  }

  cout << jsonObject2.dump(2) << endl;
}
