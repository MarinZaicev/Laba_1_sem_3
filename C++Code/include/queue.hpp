#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <iostream>
#include <fstream>
#include <string>

using namespace std;
template <typename T>
struct QNode{
    T key;
    QNode<T>* next;
};
template <typename T>
struct queue{
    QNode<T>* head = nullptr;
    QNode<T>* tail = nullptr;
    int size = 0;
};

template <typename T>
QNode<T>* Qadd(queue<T>* myQueue, T data){
    QNode<T>* newNode = new QNode<T>;
    newNode -> key = data;
    newNode -> next = nullptr;
    

    if(myQueue -> head == nullptr && myQueue -> tail == nullptr){
        myQueue -> tail = newNode;
        myQueue -> head = newNode;
    }
    else{
        myQueue -> tail -> next = newNode;
        myQueue -> tail = newNode;
    }

    myQueue->size ++;

    return newNode;
}

template <typename T>
void Qprint(queue<T>* myQueue){
    QNode<T>* ptr = myQueue -> head;
    while(ptr != nullptr){
        cout << &ptr -> key << " ";
        ptr = ptr -> next;
    }
    cout << endl;
}

template <typename T>
void Qdelete(queue<T>* myQueue){
    if (myQueue->head == nullptr) {
        return;
    }
    
    QNode<T>* deletedNode = myQueue->head;
    myQueue->head = deletedNode->next;
    
    if (myQueue->head == nullptr) {
        myQueue->tail = nullptr;
    }
    
    delete deletedNode;
    myQueue->size--;
}

template <typename T>
void clearQueue(queue<T>* myQueue){
    QNode<T>* ptr = myQueue->head;
    while(ptr != nullptr){
        QNode<T>* deletedNode = ptr;
        ptr = ptr->next;
        delete deletedNode;
    }
    
    // Нужно обнулить head и tail!
    myQueue->head = nullptr;
    myQueue->tail = nullptr;
    myQueue->size = 0;
}
template<typename T>
bool qEmpty(const queue<T>& myQueue){
    if (myQueue.head == nullptr){
        return true;
    }
    return false;
}

template<typename T>
T getHead(const queue<T>& myQueue){
    return myQueue.head -> key;
}

template<typename T>  
T getTail(const queue<T>& myQueue){
    return myQueue.tail -> key;
}

template<typename T>
void queueToFile(const queue<T>* myqueue, const string& filename) {
    if (myqueue -> head == nullptr) {
        cout << "Предупреждение: Записывается пустая очередь в файл" << endl;
    }
    
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка: Не удалось создать файл " << filename << endl;
        return;
    }
    
    file << myqueue -> size << ' ';
    
    QNode<T>* current = myqueue -> head;
    int count = 0;
    while (current != nullptr) {
        file << current->key << ' ';
        current = current->next;
        count++;
    }
    
    file.close();
    cout << "Успешно записано " << count << " элементов в файл " << filename << endl;
}

template<typename T>
void fileToqueue(queue<T>& myqueue, const string& filename) {
    clearQueue(&myqueue);
    
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Ошибка открытия файла для чтения" << endl;
        return;
    }
    
    int queueSize;
    file >> queueSize;  
    
    queue<T> tempqueue;
    
    for (int i = 0; i < queueSize; i++) {
        T data;
        if (file >> data) {
            Qadd(&tempqueue, data);
        }
    }
    file.close();

    QNode<T>* current = tempqueue.head;
    while (current != nullptr) {
        Qadd(&myqueue, current->key);
        current = current->next;
    }
    
    cout << "Очередь загружена из файла: " << filename << endl;

}

#endif
