#ifndef SDPPROJECT82186_SKIPLIST_H
#define SDPPROJECT82186_SKIPLIST_H

#include <iostream>
using std::size_t;

template<typename G>
struct Node{
    G data;
    Node<G> *next;
    Node<G> *skip;
    Node(G data) : data(data), next(nullptr), skip(nullptr){};
};

template <typename T>
class SkipList{
private:
    Node<T> *front;
    size_t size;
    void clear(Node<T>* front);
    void copy(const SkipList<T> &other);
public:
    SkipList();
    ~SkipList();
    SkipList(const SkipList<T>& other);
    SkipList<T>& operator=(const SkipList<T>& other);
    void insert(T elem);
    void forward(T from, T to);
    Node<T>* find(T value) const;
    bool check(Node<T>* currentPos, Node<T>* end) const;
};

#endif
