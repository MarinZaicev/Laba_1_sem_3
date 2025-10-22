#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

using namespace std;

template<typename T>
struct ANode {
    T key;
};

template<typename T>
struct MyArray {
    ANode<T>* data;
    int size;
    int capacity;
    
    MyArray() : data(nullptr), size(0), capacity(0) {}
    
    MyArray(int initial) : size(0), capacity(initial) {
        if (initial > 0) {
            data = new ANode<T>[initial];
        } else {
            data = nullptr;
        }
    }
    
    ~MyArray() {
        if (data != nullptr) {
            delete[] data;
        }
    }
    
    MyArray(const MyArray&) = delete;
    MyArray& operator=(const MyArray&) = delete;
};

template<typename T>
void addTail(MyArray<T>& array, T key) {
    if (array.size >= array.capacity) {
        int newCapacity;
        
        if (array.capacity == 0) {
            newCapacity = 1;
        } else {
            newCapacity = array.capacity * 2;
        }
        
        ANode<T>* newData = new ANode<T>[newCapacity];
        
        for (int i = 0; i < array.size; i++) {
            newData[i] = array.data[i];
        }
        
        if (array.data != nullptr) {
            delete[] array.data;
        }
        
        array.data = newData;
        array.capacity = newCapacity;
    }
    
    array.data[array.size].key = key;
    array.size++;
}

template<typename T>
void addToIndex(MyArray<T>& array, int index, T key) {
    if (index > array.size || index < 0) {
        throw invalid_argument("Array index out of bounds");
    }
    
    if (array.size >= array.capacity) {
        int newCapacity;
        
        if (array.capacity == 0) {
            newCapacity = 1;
        } else {
            newCapacity = array.capacity * 2;
        }
        
        ANode<T>* newData = new ANode<T>[newCapacity];
        
        for (int i = 0; i < array.size; i++) {
            newData[i] = array.data[i];
        }
        
        if (array.data != nullptr) {
            delete[] array.data;
        }
        
        array.data = newData;
        array.capacity = newCapacity;
    }
    
    for (int i = array.size; i > index; i--) {
        array.data[i] = array.data[i - 1];
    }
    
    array.data[index].key = key;
    array.size++;
}

template<typename T>
T indexKey(MyArray<T>& array, int index) {
    if (index >= array.size || index < 0) {
        throw invalid_argument("Array index out of bounds");
    }
    return array.data[index].key;
}

template<typename T>
void deleteNode(MyArray<T>& array, int index) {
    if (index >= array.size || index < 0) {
        throw invalid_argument("Array index out of bounds");
    }
    
    for (int i = index; i < array.size - 1; i++) {
        array.data[i] = array.data[i + 1];
    }
    array.size--;
}

template<typename T>
void swapNode(MyArray<T>& array, int index, T swapkey) {
    if (index >= array.size || index < 0) {
        throw invalid_argument("Array index out of bounds");
    }
    array.data[index].key = swapkey;
}

template<typename T>
void print(const MyArray<T>& array) {
    for (int i = 0; i < array.size; i++) {
        cout << array.data[i].key << " ";
    }
    cout << endl;
}

template<typename T>
int arraySize(const MyArray<T>& array) {
    return array.size;
}

template<typename T>
void clearArray(MyArray<T>& array) {
    if (array.data != nullptr) {
        delete[] array.data;
    }
    array.data = nullptr;
    array.size = 0;
    array.capacity = 0;
}

template<typename T>
void arrayToFile(const MyArray<T>& array, const string& filename) {
    if (array.size == 0) {
        cout << "Предупреждение: Записывается пустой массив в файл" << endl;
    }
    
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка: Не удалось создать файл " << filename << endl;
        return;
    }
    
    file << array.size << ' ';
    
    for (int i = 0; i < array.size; i++) {
        file << array.data[i].key << ' ';
    }
    
    file.close();
    cout << "Успешно записано " << array.size << " элементов массива в файл " << filename << endl;
}

template<typename T>
void fileToArray(MyArray<T>& array, const string& filename) {
    clearArray(array);
    
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Ошибка открытия файла для чтения" << endl;
        return;
    }
    
    int arraySize;
    file >> arraySize;
    
    if (arraySize > 0) {
        // Создаем массив нужного размера
        array.data = new ANode<T>[arraySize];
        array.capacity = arraySize;
        
        for (int i = 0; i < arraySize; i++) {
            T value;
            if (file >> value) {
                array.data[i].key = value;
                array.size++;
            }
        }
    }
    
    file.close();
    cout << "Массив загружен из файла: " << filename << " (размер: " << array.size << ")" << endl;
}

#endif
