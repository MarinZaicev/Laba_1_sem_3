#ifndef STACK_HPP
#define STACK_HPP
#include <iostream>
#include <fstream>

using namespace std;

template<typename T>
struct Node{
    T key;
    Node<T>* next;
};

template<typename T>
struct Stack{
    Node<T>* head = nullptr;
    int size = 0;
};

template <typename T>
Node<T>* createNode(T key){
    Node<T>* newNode = new Node<T>;

    newNode -> key = key;
    newNode -> next = nullptr;

    return newNode;
}

template <typename T>
Node<T>* Sadd(Stack<T>* myStack, T data){
    Node<T>* newNode = createNode(data);

    newNode -> next = myStack -> head;
    myStack -> head = newNode;

    myStack -> size ++;

    return newNode;
}

template <typename T>
void Sdelete(Stack<T>* myStack){
    if (myStack -> head == nullptr){
        cout << "в этом стеке нечего удалять." << endl;\
        return;
    }

    Node<T>* headNode = myStack -> head;

    myStack -> head = myStack -> head -> next;

    myStack->size --;

    delete headNode;

}

template <typename T>
void Sprint(Stack<T>* myStack){
    if (myStack -> head == nullptr){
        cout << "стек пуст" << endl;
    }
    Node<T>* current = myStack -> head;
    while(current != nullptr){
        cout << current -> key << " ";
        current = current -> next;
    }
    cout << endl;
}

template <typename T>
void clearStack(Stack<T>* myStack){
    Node<T>* current = myStack -> head;
    while(current != nullptr){
        Node<T>* delNode = current;
        current = current -> next;
    }

    myStack -> size = 0;
}

template<typename T>
void stackToFile(const Stack<T>* myStack, const string& filename) {
    if (myStack -> head == nullptr) {
        cout << "Предупреждение: Записывается пустой стек в файл" << endl;
    }
    
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка: Не удалось создать файл " << filename << endl;
        return;
    }
    
    file << myStack -> size << ' ';
    
    Node<T>* current = myStack -> head;
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
void fileToStack(Stack<T>& myStack, const string& filename) {
    clearStack(&myStack);
    
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Ошибка открытия файла для чтения" << endl;
        return;
    }
    
    int stackSize;
    file >> stackSize;  
    
    Stack<T> tempStack;
    
    for (int i = 0; i < stackSize; i++) {
        T data;
        if (file >> data) {
            Sadd(&tempStack, data);
        }
    }
    file.close();

    Node<T>* current = tempStack.head;
    while (current != nullptr) {
        Sadd(&myStack, current->key);
        current = current->next;
    }
    
    cout << "Стек загружен из файла: " << filename << endl;
}
#endif
