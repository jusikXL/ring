#include "bi_ring.h"

#include <string>
#include <cassert>
#include <iostream>
#include <vector>
#include <cassert>
using namespace std;

// int _concatenate(const string&, const int& i1, const int& i2) {
//   return i1 + i2;
// };

pair<Ring<int, string>, vector<pair<int, string>>>
_ring_fixture() {
  // Call the fixture to initialize ring and nodes
  Ring<int, string> ring;

  vector<pair<int, string>> nodes = {
      {4, "D node"},
      {3, "C node"},
      {2, "B node"},
      {1, "A node"},
  };

  for (const auto& pair : nodes) {
    ring.push_front(pair.first, pair.second);
  }

  return make_pair(ring, nodes);
}

// void shuffle() {
//   Ring<string, int> ring_first;
//   Ring<string, int> ring_second;

//   vector<pair<string, int>> nodes_first = {
//       {"quattro", 4},
//       {"tre", 3},
//       {"due", 2},
//       {"uno", 1},
//   };

//   vector<pair<string, int>> nodes_second = {
//       {"bes", 5},
//       {"dort", 4},
//       {"uc", 3},
//       {"iki", 2},
//       {"bir", 1},
//   };

//   for (const auto& pair : nodes_first) {
//     ring_first.push_front(pair.first, pair.second);
//   }

//   for (const auto& pair : nodes_second) {
//     ring_second.push_front(pair.first, pair.second);
//   }

//   cout << ring_first << endl;
//   cout << ring_second << endl;

//   Ring<string, int> ring_shuffled = shuffle(ring_first, 1, ring_second, 2, 3);
//   cout << ring_shuffled << endl;
// }

void constructor() {
  Ring<int, int> ring;

  assert(ring.cbegin() == ring.cend());
  assert((++ring.cbegin()) == ring.cend());
  assert((--ring.cbegin()) == ring.cend());

  assert(ring.cend()->first == int{});
  assert(ring.cend()->second == int{});

  assert(ring.size() == 0);
}

void push() {
  auto [ring, nodes] = _ring_fixture();

  auto it = --(ring.cend());
  for (const auto& pair : nodes) {
    // should sequentially add keys and infos
    assert(*it == pair);

    it--;
  }

  // should change _next pointers
  // so that we can traverse the ring forward using iterators
  it = --ring.cend();
  for (auto node : nodes) {
    assert(*it == node);

    it--;
  }

  // should increase the size
  assert(ring.size() == nodes.size());

  // should return a correct iterator to the just pushed node
  auto it_pushed = ring.push_front(5, "E node");
  assert(it_pushed == ring.begin());
}

void insert() {
  Ring<string, string> ring;

  // should insert in an empty ring if begin() is provided as a position
  ring.insert(ring.begin(), "key", "info");
  assert(ring.begin()->first == "key" && ring.begin()->second == "info");

  // should increase size
  assert(ring.size() == 1);

  // should insert before successfully updating _next and _past pointers
  ring.insert(ring.begin(), "before key", "before info");
  auto it_first = ++ring.begin();

  assert(ring.begin()->first == "before key" && ring.begin()->second == "before info");
  assert(it_first->first == "key" && it_first->second == "info");

  auto it_second = --it_first;
  assert(it_second->first == "before key" && it_second->second == "before info");
}

void pop() {
  auto [ring, nodes] = _ring_fixture();

  for (unsigned int i = 0; i < nodes.size(); i++) {
    auto it_next = ring.pop_front();

    // should decrease the size
    assert(ring.size() == nodes.size() - i - 1);

    if (i != nodes.size() - 1) {
      // should return iterator to a next node after removed
      assert(*it_next == nodes[nodes.size() - i - 2]);

      // should change the _next pointer of the _sentinel to its _next
      assert(*ring.begin() == nodes[nodes.size() - i - 2]);
    } else {
      assert(*it_next == make_pair(int{}, string{}));
      assert(*ring.begin() == make_pair(int{}, string{}));
    }

    // should change the _past pointer of the next node to _sentinel
    assert(*it_next.past() == make_pair(int{}, string{}));
  }

  // should return _sentinel while trying to pop_front on the empty ring
  Ring<int, string> ring_empty;

  assert(ring_empty.pop_front() == ring_empty.begin());
}

void erase() {
  auto [ring, nodes] = _ring_fixture();

  // should remove the first node - covered in pop()
  // should decrease the size - covered in pop()
  // should return _sentinel if trying to erease _sentinel - covered in pop()

  auto it = ring.begin();
  it++; // second node

  auto it_next = ring.erase(it);

  // should return pointer to the next node after removed
  assert(*it_next == nodes[nodes.size() - 3]);

  // should change _next pointer of the previous node to its next
  assert(++ring.begin() == it_next);

  // should change _past pointer of the next node to its past
  assert(--it_next == ring.begin());
}

void clear() {
  auto [ring, nodes] = _ring_fixture();

  ring.clear();

  assert(ring.cbegin() == ring.cend());
  assert((++ring.cbegin()) == ring.cend());
  assert((--ring.cbegin()) == ring.cend());

  assert(ring.size() == 0);
}

void find() {
  auto [ring, nodes] = _ring_fixture();

  // should return iterator to the found node
  for (unsigned int i = 0; i < nodes.size(); i++) {
    auto it = ring.find(nodes[i].first);
    assert(*it == nodes[i]);
  }

  // should return iterator to the upper bound if not found (_sentinel in our case)
  auto it_not_found = ring.find(456789);
  assert(it_not_found == ring.end());
}