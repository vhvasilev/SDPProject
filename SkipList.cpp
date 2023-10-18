#include "SkipList.h"

template <typename T>
SkipList<T>::SkipList(){
    this->front= nullptr;
    size = 0;
}

template <typename T>
void SkipList<T>::copy(const SkipList<T> &other) {
    Node<T> *currentFromOther = other.front;
    Node<T> *current = this->front;
    while (currentFromOther != nullptr) {
        current = new Node<T>(currentFromOther->data);
        current = current->next;
        currentFromOther = currentFromOther->next;
    }

    currentFromOther = other.front;
    current = this->front;
    while (currentFromOther != nullptr) {

        // Count the number of nodes that are skipped from currentFromOther to currentFromOther->skip
        int count = 0;
        Node<T> *tempDestination = currentFromOther->skip;
        Node<T> *tempSource = currentFromOther;
        while (tempSource != tempDestination) {
            count++;
            tempSource = tempSource->next;
        }

        // Skip the same number of nodes from current to current->skip and save it in tempToReachInThis
        Node<T> *tempToReachInThis = current;
        for (int i = 0; i < count; ++i) {
            tempToReachInThis = tempToReachInThis->next;
        }
        // Set the skip of current to tempToReachInThis
        current->skip = tempToReachInThis;

        // Move to the next node in both lists
        current = current->next;
        currentFromOther = currentFromOther->next;
    }
}

template <typename T>
SkipList<T>::SkipList(const SkipList<T> &other) {
    copy(other);
}

template <typename T>
SkipList<T>& SkipList<T>::operator=(const SkipList<T> &other) {
    if(this != &other) {
        this->clear();
        this->copy(other);
    }
    return *this;
}

template <typename T>
void SkipList<T>::clear(Node<T>* front)
{
    Node<T>* iterator = front;
    while(iterator)
    {
        Node<T>* del = iterator;
        iterator = iterator->next;
        delete del;
    }
    size = 0;
}

template <typename T>
SkipList<T>::~SkipList()
{
    clear(front);
}

template <typename T>
void SkipList<T>::insert(T elem){
    if(!front)
    {
        front = new Node<T>(elem);
        size++;
        return;
    }
    Node<T>* iterator = front;
    while(iterator->next)
    {
        iterator = iterator->next;
    }
    iterator->next = new Node<T>(elem);
    size++;
}

template <typename T>
Node<T>* SkipList<T>::find(const T value) const
{
    Node<T>* iterator = front;
    while(iterator)
    {
        if(iterator->data == value)
        {
            return iterator;
        }
        iterator = iterator->next;
    }
    return nullptr;
}

template <typename T>
void SkipList<T>::forward(T from, T to)
{
    Node<T>* first = find(from);
    Node<T>* second = find(to);
    first->skip = second;
}

template <typename T>
bool SkipList<T>::check(Node<T>* currentPos, Node<T>* end) const
{
    Node<T>* iterator = currentPos;
    while (iterator)
    {
        if(iterator->data == end->data)
        {
            return true;
        }
        iterator = iterator->next;
    }
    return false;
}