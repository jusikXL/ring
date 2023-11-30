#ifndef RING_HPP
#define RING_HPP

#include <iostream>
using namespace std;

template <typename Key, typename Info>
class Ring
{
private:
  /////////////////////////////// NODE //////////////////////////////
  class Node
  {

  public:
    Key key;
    Info info;
    Node(const Key &k, const Info &i, Node *p = nullptr, Node *n = nullptr)
        : key(k), info(i), _past(p ? p : this), _next(n ? n : this) {}

  private:
    Node *_past;
    Node *_next;
    friend class Ring<Key, Info>;
  };
  /////////////////////////////// NODE //////////////////////////////

  Node *_sentinel;
  unsigned int _size;

  bool is_sentinel(Node *node) const { return node == _sentinel; }

  Node *_push(Node *before, Node *after, const Key &key, const Info &info)
  {
    Node *new_node = new Node(key, info, before, after);

    before->_next = new_node;
    after->_past = new_node;

    _size++;

    return new_node;
  }

  Node *_pop(Node *node)
  {
    if (is_sentinel(node))
      return _sentinel;

    Node *to_delete = node;
    Node *next_node = node->_next;

    node->_next->_past = node->_past;
    node->_past->_next = node->_next;

    delete to_delete;
    _size--;

    return next_node; // returns _sentinel if the last node is deleted
  }

public:
  /////////////////////////////// ITERATORS //////////////////////////////
  template <typename Derived>
  class IteratorBase
  {
  protected:
    Node *_curr;
    friend class Ring<Key, Info>;
    IteratorBase(Node *node) : _curr(node) {}

  public:
    Derived &operator++()
    {
      _curr = _curr->_next;
      return static_cast<Derived &>(*this);
    }

    Derived operator++(int)
    {
      Derived temp = static_cast<Derived &>(*this);
      ++(*this);
      return temp;
    }

    Derived &operator--()
    {
      _curr = _curr->_past;
      return static_cast<Derived &>(*this);
    }

    Derived operator--(int)
    {
      Derived temp = static_cast<Derived &>(*this);
      --(*this);
      return temp;
    }

    bool operator==(const Derived &other) const
    {
      return _curr == other._curr;
    }

    bool operator!=(const Derived &other) const
    {
      return _curr != other._curr;
    }
  };

  class Iterator : public IteratorBase<Iterator>
  {
  public:
    using IteratorBase<Iterator>::IteratorBase;

    Key &key() { return this->_curr->key; }
    Info &info() { return this->_curr->info; }
  };

  class ConstIterator : public IteratorBase<ConstIterator>
  {
  public:
    using IteratorBase<ConstIterator>::IteratorBase;

    const Key &key() const { return this->_curr->key; }
    const Info &info() const { return this->_curr->info; }
  };
  /////////////////////////////// ITERATORS //////////////////////////////

  Ring() : _sentinel(new Node(Key{}, Info{})), _size(0) {}

  ~Ring()
  {
    clear();
    delete _sentinel;
  }

  Ring &operator=(const Ring &src)
  {
    if (this != &src)
    {
      clear();

      ConstIterator it_last = --(src.cend());
      ConstIterator it_sentinel = --(src.cbegin());

      for (ConstIterator it = it_last; it != it_sentinel; it--)
      {
        push_front(it.key(), it.info());
      }
    }

    return *this;
  }

  Ring(const Ring &src) : Ring() { *this = src; }

  Iterator begin() { return Iterator(_sentinel->_next); }
  ConstIterator cbegin() const { return ConstIterator(_sentinel->_next); }

  Iterator end() { return Iterator(_sentinel); }
  ConstIterator cend() const { return ConstIterator(_sentinel); }

  Iterator push_front(const Key &key, const Info &info)
  {
    return Iterator(_push(_sentinel, _sentinel->_next, key, info));
  }

  Iterator pop_front()
  {
    return Iterator(_pop(_sentinel->_next));
  }

  Iterator erase(Iterator position)
  {
    return Iterator(_pop(position._curr));
  }

  Iterator insert(Iterator position, const Key &key, const Info &info)
  {
    return Iterator(_push(position._curr->_past, position._curr, key, info));
  }

  void clear()
  {
    while (_size)
    {
      _pop(_sentinel->_next);
    }
  }
};

template <typename Key, typename Info>
ostream &operator<<(ostream &os, const Ring<Key, Info> &ring)
{
  for (typename Ring<Key, Info>::ConstIterator it = ring.cbegin(); it != ring.cend(); ++it)
  {
    os << "Key: " << it.key() << ", Info: " << it.info() << endl;
  }
  return os;
}

#endif // RING_HPP