#ifndef BYNARYTREE_HPP
#define BYNARYTREE_HPP
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "../include/queue.hpp"

template<typename T>
struct TNode{
    T key;
    TNode* left;
    TNode* right;
    
    TNode() : key(T()), left(nullptr), right(nullptr){}
    TNode(T value, TNode* leftptr, TNode* rightptr) : key(value), left(leftptr), right(rightptr){}
};

template<typename T>
struct fullBinary{
    TNode<T>* root;
    int size = 0;
    
    fullBinary() : root(nullptr){}
    
    void deleteTree(TNode<T>* thisNode) {
        if (thisNode != nullptr) {
            deleteTree(thisNode->left);
            deleteTree(thisNode->right);
            delete thisNode;
        }
    }
    
    ~fullBinary(){
        deleteTree(root);
        size = 0;
    }
};

using namespace std;

template<typename T>
bool preordeSearch(const TNode<T>* root, T key){  // прямой обход для поиска элемента
    if (root != nullptr) {
        if (root->key == key) {
            return true;
        }
    
        if (preordeSearch(root->left, key)) {
            return true;
        }
        if (preordeSearch(root->right, key)) {
            return true;
        }
    }
    return false;
}

template<typename T>
void searchData(const fullBinary<T>& tree, T key){   // поиск элемента
    if(preordeSearch(tree.root, key)){
        cout << "Элемент " << key << " найден" << endl;
        return;
    }
    cout << "Элемент " << key << " не найден" << endl;
    return;
}

template<typename T>
void BTadd(fullBinary<T>& tree, T key){
    
    TNode<T>* thisNode = new TNode<T>{key, nullptr, nullptr};
    if (tree.root == nullptr) {
        tree.root = thisNode;
        tree.size++;
        cout << "Добавлен корень: " << key << endl;
        return;
    }

    queue<TNode<T>*> q;
    Qadd(&q, tree.root);

    while (!qEmpty(q)) {
        TNode<T>* current = q.head -> key;
        Qdelete(&q);

        if (current->left == nullptr) {
            current->left = thisNode;
            tree.size++;
            cout << "Добавлен левый потомок " << key << " к узлу " << current->key << endl;
            return;
        }
        else if (current->right == nullptr) {
            current->right = thisNode;
            tree.size++;
            cout << "Добавлен правый потомок " << key << " к узлу " << current->key << endl;
            return;
        }
        else {
            // Добавляем оба потомка в очередь для дальнейшего обхода
            Qadd(&q, current->left);
            Qadd(&q, current->right);
        }
    }
}

template<typename T>
void printTree(const TNode<T>* node, const string& prefix, bool isLeft) {
    if (node == nullptr) {
        return;
    }
    

    if (isLeft) {  // правое поддерево
        printTree(node->right, prefix + "│   ", false);
    } else {
        printTree(node->right, prefix + "    ", false);
    }
    
    
    cout << prefix; // текущий узел
    if (isLeft) {
        cout << "└── ";
    } else {
        cout << "┌── ";
    }
    cout << node->key << endl;
    
    
    if (isLeft) { // левое поддерево
        printTree(node->left, prefix + "    ", true);
    } else {
        printTree(node->left, prefix + "│   ", true);
    }
}

template<typename T>
void treePrint(const fullBinary<T>& tree) {
    if (tree.root == nullptr) {
        cout << "Дерево пустое." << endl;
        return;
    }
    cout << "Визуализация дерева:" << endl;
    printTree(tree.root, "", false);
    cout << endl;
}

template<typename T>
bool TreeIsFull(TNode<T>* current){
    if ((current->left == nullptr) ^ (current->right == nullptr) == 1){
        return false;
    } 
    else if (!(current->left == nullptr && current->right == nullptr))
    {
        return TreeIsFull(current->left) && TreeIsFull(current->right);
    }
    return true;
}

// обходы дерева в глубину
template<typename T>
void preOrder(const TNode<T>* root){  // прямой обход
    if (root != nullptr){
        cout << root->key << ' ';
        preOrder(root->left);
        preOrder(root->right);
    }
}

template<typename T>
void inOrder(const TNode<T>* root){  // центрированный обход
    if (root != nullptr){
        inOrder(root->left);
        cout << root->key << ' ';
        inOrder(root->right);
    }
}

template<typename T>
void postOrder(const TNode<T>* root){  // обратный обход
    if (root != nullptr){
        postOrder(root->left);
        postOrder(root->right);
        cout << root->key << ' ';
    }
}

template<typename T>
void printPreOrder(fullBinary<T>& tree){
    cout << "Прямой обход: ";
    preOrder(tree.root);
    cout << endl;
}

template<typename T>
void printInOrder(fullBinary<T>& tree){
    cout << "Центрированный обход: ";
    inOrder(tree.root);
    cout << endl;
}

template<typename T>
void printPostOrder(fullBinary<T>& tree){
    cout << "Обратный обход: ";
    postOrder(tree.root);
    cout << endl;
}

template<typename T>
void printBFS(const fullBinary<T>& tree){    // вывод через обход в ширину (BFS)
    if (tree.root == nullptr){
        cout << "Дерево пустое" << endl;
        return;
    }
    
    cout << "Обход в ширину: ";
    queue<TNode<T>*> q;
    Qadd(&q, tree.root);
    
    while (!qEmpty(q)) {
        TNode<T>* current = q.head -> key;
        Qdelete(&q);
        
        cout << current->key << ' ';
        if (current->left != nullptr){
            Qadd(&q, current->left);
        }
        if (current->right != nullptr){
            Qadd(&q, current->right);
        }
    }
    cout << endl;
}

template<typename T>
void clearTree(fullBinary<T>& tree) {
    tree.deleteTree(tree.root);
    tree.root = nullptr;
    tree.size = 0;
    cout << "Дерево очищено" << endl;
}

template<typename T>
void treeToFile(const fullBinary<T>& tree, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка: Не удалось создать файл " << filename << endl;
        return;
    }
    
    // Записываем размер дерева
    file << tree.size << "\n";
    
    if (tree.root == nullptr) {
        file.close();
        cout << "Успешно записано пустое дерево в файл " << filename << endl;
        return;
    }
    
    queue<TNode<T>*> q;
    Qadd(&q, tree.root);
    
    int count = 0;
    
    while (!qEmpty(q)) {
        TNode<T>* current = getHead(q);
        Qdelete(&q);
        
        // Записываем элемент
        file << current->key;
        count++;
        
        // Добавляем пробел после каждого элемента, кроме последнего
        if (!qEmpty(q)) {
            file << " ";
        }
        
        // Добавляем потомков в очередь
        if (current->left != nullptr) {
            Qadd(&q, current->left);
        }
        if (current->right != nullptr) {
            Qadd(&q, current->right);
        }
    }
    
    file << "\n"; // Завершаем строку
    file.close();
    cout << "Успешно записано " << count << " элементов дерева в файл " << filename << endl;
}

// Вспомогательная функция для восстановления дерева из файла
template<typename T>
void rebuildTree(fullBinary<T>& tree, const vector<T>& data) {
    if (data.empty()) {
        return;
    }
    
    clearTree(tree);
    
    tree.root = new TNode<T>{data[0], nullptr, nullptr};
    tree.size = 1;
    
    queue<TNode<T>*> q;
    Qadd(&q, tree.root);
    
    int index = 1;
    int n = data.size();
    
    while (!qEmpty(q) && index < n) {
        TNode<T>* current = q.head -> key;
        Qdelete(&q);
        
        if (index < n) { // левый потомок
            current->left = new TNode<T>{data[index], nullptr, nullptr};
            Qadd(&q, current->left);
            tree.size++;
            index++;
        }
        
        if (index < n) { // правый потомок
            current->right = new TNode<T>{data[index], nullptr, nullptr};
            Qadd(&q, current->right);
            tree.size++;
            index++;
        }
    }
}

template<typename T>
void fileToTree(fullBinary<T>& tree, const string& filename) {
    clearTree(tree);
    
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Ошибка открытия файла для чтения: " << filename << endl;
        return;
    }
    
    int treeSize;
    if (!(file >> treeSize)) {
        cout << "Ошибка чтения размера дерева из файла" << endl;
        file.close();
        return;
    }
    
    cout << "Размер дерева из файла: " << treeSize << endl;
    
    if (treeSize == 0) {
        file.close();
        cout << "Загружено пустое дерево из файла: " << filename << endl;
        return;
    }
    
    // Пропускаем перевод строки после размера
    file.ignore(1, '\n');
    
    vector<T> data;
    T value;
    
    // Читаем все элементы из второй строки
    string line;
    if (getline(file, line)) {
        cout << "Прочитана строка данных: " << line << endl;
        istringstream iss(line);
        while (iss >> value) {
            data.push_back(value);
            cout << "Добавлен элемент: " << value << endl;
        }
    }
    
    file.close();
    
    if (data.size() != treeSize) {
        cout << "Предупреждение: Прочитано " << data.size() << " элементов, но ожидалось " << treeSize << endl;
        if (data.size() == 0) {
            cout << "Не удалось прочитать ни одного элемента данных!" << endl;
            return;
        }
    }
    
    rebuildTree(tree, data);
    cout << "Дерево загружено из файла: " << filename << " (размер: " << tree.size << ")" << endl;
}
#endif
