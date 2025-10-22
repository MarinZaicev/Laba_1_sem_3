#ifndef DLIST_HPP
#define DLIST_HPP
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

template <typename T>
struct DNode{
    T key;  
    DNode<T>* next;
    DNode<T>* prev;
};

template <typename T>
struct DList{
    DNode<T>* head = nullptr;
    DNode<T>* tail = nullptr;
    int size = 0; 
};

template <typename T>
void print(DList<T>* myList){
    DNode<T>* itr = myList->head;

    while(itr != nullptr){
        cout << itr->key << " ";
        itr = itr->next;
    }
    cout << endl;
}

template<typename T>
DNode<T>* findDNode(DList<T>* list, T value) {
    DNode<T>* current = list->head;
    while (current != nullptr) {
        if (current->key == value) return current;
        current = current->next;
    }
    return nullptr;
}

template <typename T>
void addHead(DList<T>* myList, T data){
    DNode<T>* newDNode = new DNode<T>;
    newDNode->key = data;
    newDNode->prev = nullptr;

    if(myList->head == nullptr){
        newDNode->next = nullptr;
        myList->head = newDNode;
        myList->tail = newDNode;
    }
    else{
        newDNode->next = myList->head;
        myList->head->prev = newDNode;
        myList->head = newDNode;
    }
    myList->size++; 
}

template <typename T>
void addEnd(DList<T>* myList, T data){
    DNode<T>* newDNode = new DNode<T>;
    newDNode->key = data;
    newDNode->next = nullptr;

    if(myList->head == nullptr){
        addHead(myList, data);
        return;
    }
    else{
        newDNode->prev = myList->tail;
        myList->tail->next = newDNode;
        myList->tail = newDNode; 
    }
    myList->size++; 
}

template <typename T>
DNode<T>* addAfter(DList<T>* myList,T target, T data){
    DNode<T>* afterDNode = findDNode(myList, target);

    if(afterDNode == nullptr){
        return nullptr;
    }
    
    if(afterDNode->next == nullptr){
        addEnd(myList, data);
        return myList->tail; 
    }
    else{
        DNode<T>* newDNode = new DNode<T>;
        newDNode->key = data;
        
        newDNode->next = afterDNode->next;
        newDNode->prev = afterDNode;
        afterDNode->next->prev = newDNode;
        afterDNode->next = newDNode;

        myList->size++; 
        return newDNode;
    }
}

template <typename T>
DNode<T>* addBefore(DList<T>* myList,T target, T data){
    DNode<T>* beforeDNode = findDNode(myList, target);
    if(beforeDNode == nullptr){
        return nullptr;
    }
    
    if(beforeDNode->prev == nullptr){
        addHead(myList, data);
        return myList->head; 
    }
    else{
        DNode<T>* newDNode = new DNode<T>;
        newDNode->key = data;
        
        newDNode->next = beforeDNode;
        newDNode->prev = beforeDNode->prev;
        beforeDNode->prev->next = newDNode;
        beforeDNode->prev = newDNode;

        myList->size++;  
        return newDNode;
    }
}

template <typename T>
void deleteHead(DList<T>* myList){
    if (myList->head == nullptr) return;
    
    DNode<T>* deletedDNode = myList->head;
    myList->head = deletedDNode->next;
    
    if (myList->head != nullptr) {
        myList->head->prev = nullptr;
    } else {
        myList->tail = nullptr;  
    }

    delete deletedDNode;
    myList->size--;  
}

template <typename T>
void deleteTail(DList<T>* myList){
    if(myList->head == nullptr){
        return;
    }
    else if(myList->head == myList->tail){
        DNode<T>* deletedDNode = myList->head;
        myList->head = nullptr;
        myList->tail = nullptr;
        delete deletedDNode;
    }
    else{
        DNode<T>* deletedDNode = myList->tail;
        myList->tail = deletedDNode->prev;
        myList->tail->next = nullptr;
        delete deletedDNode;
    }
    myList->size--; 
}

template <typename T>
void deleteAfter(DList<T>* myList, T data){
    DNode<T>* afterDNode = findDNode(myList, data);
    if(myList->head == nullptr || afterDNode == nullptr || afterDNode->next == nullptr){
        return;
    }
    
    DNode<T>* deletedDNode = afterDNode->next;
    afterDNode->next = deletedDNode->next;

    if(deletedDNode->next == nullptr){
        myList->tail = afterDNode; 
    }
    else{
        deletedDNode->next->prev = afterDNode;
    }

    delete deletedDNode;
    myList->size--; 
}

template <typename T>
void deleteBefore(DList<T>* myList, T data){
    DNode<T>* beforeDNode = findDNode(myList, data);
    if(myList->head == nullptr || beforeDNode == nullptr || beforeDNode->prev == nullptr){
        return;
    }

    DNode<T>* deletedDNode = beforeDNode->prev;
    beforeDNode->prev = deletedDNode->prev;

    if(deletedDNode->prev != nullptr){
        deletedDNode->prev->next = beforeDNode;
    }
    else{
        myList->head = beforeDNode;
        beforeDNode->prev = nullptr;
    }

    delete deletedDNode;
    myList->size--;  
}

template <typename T>
void deleteDNode(DList<T>* myList, DNode<T>* thisDNode){
    if(myList == nullptr || myList->head == nullptr || thisDNode == nullptr){
        return;
    }
    if(thisDNode == myList->tail){
        deleteTail(myList);
    }
    else if(thisDNode == myList->head){
        deleteHead(myList);
    }
    else{
        thisDNode->prev->next = thisDNode->next;
        thisDNode->next->prev = thisDNode->prev;
        delete thisDNode;
        myList->size--;
    }
}

template <typename T>
void clearDList(DList<T>* myList){
    DNode<T>* itr = myList->head;
    while(itr != nullptr){
        DNode<T>* deletedDNode = itr;
        itr = itr->next;
        delete deletedDNode;
    }

    myList->head = nullptr;
    myList->tail = nullptr;
    myList->size = 0; 
}

template<typename T>
void findData(DList<T>* list, T data){
    DNode<T>* current = list->head;
    while(current != nullptr){
        if(current->key == data){
            cout << "Значение " << data << " найдено" << endl;
            return;
        }
        current = current->next;
    }
    cout << "Значение " << data << " не найдено" << endl;
}

template<typename T>
void DListToFile(const DList<T>* myList, const string& filename) {
    if (myList->head == nullptr) {
        cout << "Предупреждение: Записывается пустой список в файл" << endl;
    }
    
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка: Не удалось создать файл " << filename << endl;
        return;
    }
    
    file << myList->size << ' ';
    
    DNode<T>* current = myList->head;
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
void fileToDList(DList<T>* myList, const string& filename) {
    clearDList(myList);
    
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Ошибка открытия файла для чтения" << endl;
        return;
    }
    
    int listSize;
    if (!(file >> listSize)) {
        cout << "Ошибка чтения размера списка" << endl;
        file.close();
        return;
    }
    
    for (int i = 0; i < listSize; i++) {
        T data;
        if (file >> data) {
            addEnd(myList, data);
        } else {
            cout << "Ошибка чтения элемента " << i << " из файла" << endl;
            break;
        }
    }
    file.close();
    
    cout << "Список загружен из файла: " << filename << endl;
}
#endif
