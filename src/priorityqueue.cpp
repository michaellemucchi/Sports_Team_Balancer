#include <iostream>
#include "json.hpp"

#include "priorityqueue.h"

PriorityQueue::PriorityQueue(std::size_t max_size) :
	nodes_(max_size + 1, KeyValuePair()),
	max_size_(max_size),
	size_(0) {
}

void PriorityQueue::insert(Key k) {
	insert(std::make_pair(k, std::make_pair(0, 0)));
}

void PriorityQueue::insert(KeyValuePair kv) {
	size_++;
	int i = size_;
	nodes_[i] = kv;
	heapifyUp(size_);
}

KeyValuePair PriorityQueue::min() {
	return nodes_[1];
}

KeyValuePair PriorityQueue::removeMin() {
	KeyValuePair temp = nodes_[1];
	nodes_[1] = nodes_[size_];
	size_--;
	heapifyDown(1);
	return temp;
}

bool PriorityQueue::isEmpty() const {
	if (size_ == 0) {
		return true;
	}
	else {
		return false;
	}
}

size_t PriorityQueue::size() const {
	return size_;
}

nlohmann::json PriorityQueue::JSON() const {
	nlohmann::json result;
  for (size_t i = 1; i <= size_; i++) {
      nlohmann::json node;
      KeyValuePair kv = nodes_[i];
      node["key"] = kv.first;
      node["value"] = kv.second;
      if (i != ROOT) {
          node["parent"] = std::to_string(i / 2);
      }
      if (2 * i <= size_) {
          node["leftChild"] = std::to_string(2 * i);
      }
      if (2 * i + 1 <= size_) {
          node["rightChild"] = std::to_string(2 * i + 1);
      }
      result[std::to_string(i)] = node;
  }
  result["metadata"]["max_size"] = max_size_;
  result["metadata"]["size"] = size_;
	return result;
}

void PriorityQueue::heapifyUp(size_t i) {
	if ((i > 1) && (nodes_[i] < nodes_[i/2])) {
		swap(nodes_[i], nodes_[i/2]);
		heapifyUp(i / 2);
	}
}

void PriorityQueue::heapifyDown(size_t i) {
	size_t smallest = i;

	if ((2 * i <= size_) && (nodes_[2 * i] < nodes_[i])) {
		smallest = 2 * i;
	}
	if ((2 * i + 1 <= size_) && (nodes_[2 * i + 1] < nodes_[smallest])) {
		smallest = 2 * i + 1;
	}
	if (smallest != i) {
		KeyValuePair temp = nodes_[i];
		nodes_[i] = nodes_[smallest];
		nodes_[smallest] = temp;
		heapifyDown(smallest);
	}
}

void PriorityQueue::removeNode(size_t i) {
	nodes_[i] = nodes_[size_];
	size_--;
	heapifyDown(i);
	heapifyUp(i);
}

Key PriorityQueue::getKey(size_t i) {
	return nodes_[i].first;
}
