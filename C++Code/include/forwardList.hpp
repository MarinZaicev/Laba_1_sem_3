#ifndef FLIST_HPP
#define FLIST_HPP
#include <iostream>
#include <fstream>

using namespace std;

template <typename T>
struct FNode{
    T key;
    FNode<T>* next;
};

template <typename T>
struct FList {
    FNode<T>* head = nullptr;
    int size = 0;
};

template<typename T>
FNode<T>* findFNode(FList<T>* list, T data) {
    FNode<T>* current = list->head;
    while (current != nullptr) {
        if (current->key == data){
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

template <typename T>
FNode<T>* addAfter(FList<T>* list,T target, T data){
    FNode<T>* afterFNode = findFNode(list, target);
    if (afterFNode == nullptr) return nullptr;

    FNode<T>* thisFNode = new FNode<T>;
    thisFNode->key = data;
    thisFNode->next = afterFNode->next;
    afterFNode->next = thisFNode;

    list->size++;

    return thisFNode;
}

template <typename T>
void addToFront(FList<T>* list, T data){
    FNode<T>* thisFNode = new FNode<T>;
    thisFNode->key = data;
    
    thisFNode->next = list->head;
    list->head = thisFNode; 

    list->size++;
}

template <typename T>
FNode<T>* addBefore(FList<T>* list,T target, T data){
    FNode<T>* beforeFNode = findFNode(list, target);

    if(beforeFNode == nullptr){
        return nullptr;
    }

    FNode<T>* thisFNode = new FNode<T>;
    thisFNode->key = data;

    if(beforeFNode == list->head){
        thisFNode->next = list->head;
        list->head = thisFNode;

        list->size++;

        return thisFNode;
    }

    FNode<T>* itrFNode = list->head; 

    while(itrFNode != nullptr && itrFNode->next != beforeFNode){
        itrFNode = itrFNode->next;
    }

    if(itrFNode != nullptr){
        thisFNode->next = beforeFNode;
        itrFNode->next = thisFNode;
        list->size++;
    }
    else{
        delete thisFNode;
        return nullptr;
    }

    return thisFNode;
}

template <typename T>
FNode<T>* addToEnd(FList<T>* list, T data){
    FNode<T>* thisFNode = new FNode<T>;
    thisFNode->key = data;
    thisFNode->next = nullptr;

    if(list->head == nullptr){
        list->head = thisFNode;
        list->size++;
        return thisFNode;
    }

    FNode<T>* itrFNode = list->head;
    while(itrFNode->next != nullptr){
        itrFNode = itrFNode->next;
    }

    itrFNode->next = thisFNode;
    list->size++;

    return thisFNode;
}

template <typename T>
void deleteAfter(FList<T>* list, T data){
    FNode<T>* afterFNode = findFNode(list, data);

    if (afterFNode == nullptr || afterFNode->next == nullptr){
        return;
    }

    FNode<T>* deletedFNode = afterFNode->next;
    afterFNode->next = deletedFNode->next;

    list->size--;

    delete deletedFNode;
}

template <typename T>
void deleteBefore(FList<T>* list, T data){
    FNode<T>* beforeFNode = findFNode(list, data);

    if(list->head == nullptr || beforeFNode == nullptr || list->head == beforeFNode){
        return;
    }

    if(list->head->next == beforeFNode){
        FNode<T>* deletedFNode = list->head;
        list->head = beforeFNode;
        list->size--;
        delete deletedFNode;
        return;
    }
    
    FNode<T>* current = list->head;

    while(current != nullptr && current->next != nullptr && current->next->next != beforeFNode){
        current = current->next;
    }

    if(current == nullptr || current->next == nullptr){
        return;
    }
    else{
        FNode<T>* deletedFNode = current->next;
        current->next = beforeFNode;
        list->size--;
        delete deletedFNode;
    }
}

template <typename T>
void deleteVal(FList<T>* list, T data){
    if(list->head == nullptr){
        return;
    }
    
    if(list->head->key == data){
        deleteHead(list);
        return;
    }
    
    FNode<T>* current = list->head;

    while(current->next != nullptr && current->next->key != data){
        current = current->next;
    }
    
    if(current->next != nullptr && current->next->key == data){
        FNode<T>* deletedFNode = current->next;
        current->next = deletedFNode->next;
        delete deletedFNode;
        list->size--;
    }
}

template <typename T>
void deleteHead(FList<T>* list){
    if(list->head == nullptr) return;
    
    FNode<T>* deletedFNode = list->head;
    list->head = deletedFNode->next;
    list->size--;
    delete deletedFNode;
}

template <typename T>
void deleteTail(FList<T>* list){
    if(list->head == nullptr){
        return;
    }

    if(list->head->next == nullptr){
        FNode<T>* deletedFNode = list->head;
        list->head = nullptr;
        list->size--;
        delete deletedFNode;
        return;
    }

    FNode<T>* current = list->head;
    while(current->next->next != nullptr){
        current = current->next;
    }

    FNode<T>* deletedFNode = current->next;
    current->next = nullptr;
    list->size--;
    delete deletedFNode;
}

template <typename T>
void print(FList<T>* list){
    FNode<T>* thisFNode = list->head;
    while(thisFNode != nullptr){
        cout << thisFNode->key << " ";
        thisFNode = thisFNode->next;
    }
    cout << endl;
}

template <typename T>
void clearFList(FList<T>* list){
    FNode<T>* thisFNode = list->head;
    while(thisFNode != nullptr){
        FNode<T>* nextFNode = thisFNode->next;
        delete thisFNode;
        thisFNode = nextFNode;
    }
    list->head = nullptr;
    list->size = 0;
}

template<typename T>
void findData(FList<T>* list, T data){
    FNode<T>* current = list->head;
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
void FListToFile(const FList<T>* list, const string& filename) {
    if (list->head == nullptr) {
        cout << "Предупреждение: Записывается пустой список в файл" << endl;
    }
    
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка: Не удалось создать файл " << filename << endl;
        return;
    }
    
    file << list->size << ' ';
    
    FNode<T>* current = list->head;
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
void fileToFList(FList<T>& list, const string& filename) {
    clearFList(&list);
    
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Ошибка открытия файла для чтения" << endl;
        return;
    }
    
    int FListSize;
    if (!(file >> FListSize)) {
        cout << "Ошибка чтения размера списка из файла" << endl;
        file.close();
        return;
    }
    
    for (int i = 0; i < FListSize; i++) {
        T data;
        if (file >> data) {
            addToEnd(&list, data);
        } else {
            cout << "Ошибка чтения элемента " << i << " из файла" << endl;
            break;
        }
    }
    file.close();
    
    cout << "Список загружен из файла: " << filename << endl;
}
#endif
