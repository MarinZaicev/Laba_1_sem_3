#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <sstream>

#include "../include/array.hpp"
#include "../include/binaryTree.hpp"
#include "../include/DList.hpp"
#include "../include/forwardList.hpp"
#include "../include/queue.hpp"
#include "../include/stack.hpp"

using namespace std;

// enum class menu{
//     PRINT,
//     HELP,
//     EXIT,

//     M_PUSH_END,
//     M_PUSH_AT,
//     M_DEL_AT,
//     M_GET_AT,
//     M_REPLACE_AT,
//     M_LENGTH,

//     F_PUSH_HEAD, L_PUSH_HEAD,
//     F_PUSH_TAIL, L_PUSH_TAIL,
//     F_PUSH_AFTER, L_PUSH_AFTER,
//     F_PUSH_BEFORE, L_PUSH_BEFORE,
//     F_DEL_HEAD, L_DEL_HEAD,
//     F_DEL_TAIL, L_DEL_TAIL,
//     F_DEL_VAL, L_DEL_VAL,
//     F_GET_VAL, L_GET_VAL,

//     Q_PUSH,
//     Q_POP,
//     Q_GET,

//     S_PUSH,
//     S_POP,
//     S_GET,

//     T_INSERT,
//     T_SEARCH,
//     T_DEL,
//     T_IS_COMPLETE,
//     T_ISEMPTY,
//     T_CLEAR,
//     VISUALIZE
// };

void command(){
    cout << "--- Доступные команды ---" << endl
         << "PRINT - Показать содержимое структуры" << endl
         << "HELP - Показать это сообщение" << endl
         << "EXIT - Выйти из программы" << endl << endl
         << "--- МАССИВ (M) ---" << endl
         << "M_PUSH_END - Добавить в конец массива" << endl
         << "M_PUSH_AT - Добавить по индексу" << endl
         << "M_DEL_AT - Удалить по индексу" << endl
         << "M_GET_AT - Получить по индексу" << endl
         << "M_REPLACE_AT - Заменить по индексу" << endl
         << "M_LENGTH - Узнать длину массива" << endl << endl
         << "--- СПИСКИ (F - односвязный, L - двусвязный) ---" << endl
         << "F_PUSH_HEAD / L_PUSH_HEAD - Добавить в начало" << endl
         << "F_PUSH_TAIL / L_PUSH_TAIL - Добавить в конец" << endl
         << "F_PUSH_AFTER / L_PUSH_AFTER - Добавить после значения" << endl
         << "F_PUSH_BEFORE / L_PUSH_BEFORE - Добавить до значения" << endl
         << "F_DEL_HEAD / L_DEL_HEAD - Удалить первый элемент" << endl
         << "F_DEL_TAIL / L_DEL_TAIL - Удалить последний элемент" << endl
         << "F_DEL_AFTER / L_DEL_AFTER - Удалить после значения" << endl
         << "F_DEL_BEFORE / L_DEL_BEFORE - Удалить до значения" << endl
         << "L_DEL_VAL - Удалить по значению" << endl
         << "L_DEL_VAL / F_GET_VAL / L_GET_VAL - Найти по значению" << endl << endl
         << "--- ОЧЕРЕДЬ (Q) ---" << endl
         << "Q_PUSH - Добавить в очередь" << endl
         << "Q_POP - Извлечь из очереди" << endl << endl
         << "--- СТЕК (S) ---" << endl
         << "S_PUSH - Добавить в стек" << endl
         << "S_POP - Извлечь из стека" << endl << endl
         << "--- ДЕРЕВО (T) ---" << endl
         << "T_INSERT - Вставить число в дерево" << endl
         << "T_SEARCH - Найти число в дереве" << endl
         << "T_IS_FULL - Проверить, является ли дерево полным" << endl
         << "T_CLEAR - Очистить дерево" << endl
         << "T_PRE_ORDER - Прямой обход" << endl
         << "T_IN_ORDER - Центрированный обход" << endl
         << "T_POST_ORDER - Обратный обход" << endl
         << "VISUALIZE - Визуализировать дерево" << endl
         << "-----------------------" << endl;
}

int main(){
    MyArray<string> arr;
    FList<string> fList;
    DList<string> dList;
    Stack<string> stack;
    queue<string> que;
    fullBinary<int> tree;

    fileToArray(arr, "array.txt");
    fileToFList(fList, "FList.txt");
    fileToDList(&dList, "DList.txt");
    fileToStack(stack, "stack.txt");
    fileToqueue(que, "queue.txt");
    fileToTree(tree, "tree.txt");

    cout << "Данные списков загружены, введите 'HELP' для просмотра списка команд" << endl;

    string input;
    while (true){
        cout << ">> ";
        cin >> input;
        
        if(input == "EXIT" || input == "BACK"){
            break;
        }
        if(input == "HELP"){
            command();
            continue;
        }
        string sData, target;
        int itr, data;

        if(input == "M_PUSH_END"){
            cout  << "Введите строку" << endl << ">> ";
            cin >> sData;

            addTail(arr, sData);
            arrayToFile(arr, "array.txt");
        }
        else if (input == "M_PUSH_AT"){
            cout << "Введите индекс" << endl << ">> ";
            cin >> itr;

            cout << "Введите строку" << endl << ">> ";
            cin >> sData;

            addToIndex(arr, itr, sData);
            arrayToFile(arr, "array.txt");
        }
        else if(input == "M_DEL_AT"){
            cout << "Введите индекс" << endl << ">> ";
            cin >> itr;

            deleteNode(arr, itr);
            arrayToFile(arr, "array.txt");
        }
        else if(input == "M_GET_AT"){
            cout << "Введите индекс" << endl << ">> ";
            cin >> itr;

            string result = indexKey(arr, itr);
            cout << "Значение по индексу " << itr << " :" << result << endl;
        }
        else if(input == "M_REPLACE_AT"){
            cout << "Введите индекс" << endl << ">> ";
            cin >> itr;

            cout << "Введите строку" << endl << ">> ";
            cin >> sData;

            swapNode(arr, itr, sData);
            arrayToFile(arr, "array.txt");
        }
        else if(input == "M_LENGTH"){
            cout << "Длина массива: " << arraySize(arr) << endl;
        }


        else if(input == "F_PUSH_HEAD"){
            cout << "Введите строку" << endl << ">> ";
            cin >> sData;

            addToFront(&fList, sData);
            FListToFile(&fList, "FList.txt");
        }
        else if(input == "F_PUSH_TAIL"){
            cout << "Введите строку" << endl << ">> ";
            cin >> sData;

            addToEnd(&fList, sData);
            FListToFile(&fList, "FList.txt");
        }
        else if(input == "F_PUSH_AFTER"){
            cout << "Введите значение, после которого добавить" << endl << ">> ";
            cin >> target;

            cout << "Введите строку" << endl << ">> ";
            cin >> sData;

            addAfter(&fList, target, sData);
            FListToFile(&fList, "FList.txt");
        }
        else if(input == "F_PUSH_BEFORE"){
            cout << "Введите значение, до которого добавить" << endl << ">> ";
            cin >> target;

            cout << "Введите строку" << endl << ">> ";
            cin >> sData;

            addBefore(&fList, target, sData);
            FListToFile(&fList, "FList.txt");
        }
        else if(input == "F_DEL_HEAD"){
            deleteHead(&fList);
            FListToFile(&fList, "FList.txt");
        }
        else if(input == "F_DEL_TAIL"){
            deleteTail(&fList);
            FListToFile(&fList, "FList.txt");
        }
        else if(input == "F_DEL_AFTER"){
            cout << "Введите значение, после которого удалить" << endl << ">> ";
            cin >> target;

            deleteAfter(&fList, target);
            FListToFile(&fList, "FList.txt");
        }
        else if(input == "F_DEL_BEFORE"){
            cout << "Введите значение, до которого удалить" << endl << ">> ";
            cin >> target;

            deleteBefore(&fList, target);
            FListToFile(&fList, "FList.txt");
        }
        else if(input == "F_DEL_VAL"){
            cout << "Введите значение для удаления" << endl << ">> ";
            cin >> target;
            
            deleteVal(&fList, target);
            FListToFile(&fList, "FList.txt");
        }
        else if(input == "F_GET_VAL"){
            cout << "Введите строку для поиска" << endl << ">> ";
            cin >> sData;

            findData(&fList, sData);
        }


        else if(input == "L_PUSH_HEAD"){
            cout << "Введите строку" << endl << ">> ";
            cin >> sData;

            addHead(&dList, sData);
            DListToFile(&dList, "DList.txt");
        }
        else if(input == "L_PUSH_TAIL"){
            cout << "Введите строку" << endl << ">> ";
            cin >> sData;

            addEnd(&dList, sData);
            DListToFile(&dList, "DList.txt");
        }
        else if(input == "L_PUSH_AFTER"){
            cout << "Введите значение, после которого добавить" << endl << ">> ";
            cin >> target;

            cout << "Введите строку" << endl << ">> ";
            cin >> sData;

            addAfter(&dList, target, sData);
            DListToFile(&dList, "DList.txt");
        }
        else if(input == "L_PUSH_BEFORE"){
            cout << "Введите значение, до которого добавить" << endl << ">> ";
            cin >> target;

            cout << "Введите строку" << endl << ">> ";
            cin >> sData;

            addBefore(&dList, target, sData);
            DListToFile(&dList, "DList.txt");
        }
        else if(input == "L_DEL_HEAD"){
            deleteHead(&dList);
            DListToFile(&dList, "DList.txt");
        }
        else if(input == "L_DEL_TAIL"){
            deleteTail(&dList);
            DListToFile(&dList, "DList.txt");
        }
        else if(input == "L_DEL_AFTER"){
            cout << "Введите значение, после которого удалить" << endl << ">> ";
            cin >> target;

            deleteAfter(&dList, target);
            DListToFile(&dList, "DList.txt");
        }
        else if(input == "L_DEL_BEFORE"){
            cout << "Введите значение, до которого удалить" << endl << ">> ";
            cin >> target;

            deleteBefore(&dList, target);
            DListToFile(&dList, "DList.txt");
        }
        else if(input == "L_DEL_VAL"){
            cout << "Введите значение, которое надo удалить" << endl << ">> ";
            cin >> target;

            DNode<string>* current = findDNode(&dList, sData);
            deleteDNode(&dList, current);
            DListToFile(&dList, "DList.txt");
        }
        else if(input == "L_GET_VAL"){
            cout << "Введите строку для поиска" << endl << ">> ";
            cin >> sData;

            findData(&dList, sData);
        }


        else if(input == "Q_PUSH"){
            cout << "Введите строку" << endl << ">> ";
            cin >> sData;

            Qadd(&que, sData);
            queueToFile(&que, "queue.txt");
        }
        else if(input == "Q_POP"){
            Qdelete(&que);
            queueToFile(&que, "queue.txt");
        }
        

        else if(input == "S_PUSH"){
            cout << "Введите строку" << endl << ">> ";
            cin >> sData;
            Sadd(&stack, sData);
            stackToFile(&stack, "stack.txt");
        }
        else if(input == "S_POP"){
            Sdelete(&stack);
            stackToFile(&stack, "stack.txt");
        }


        else if(input == "T_INSERT"){
            cout << "Введите число" << endl << ">> ";
            cin >> data;

            BTadd(tree, data);
            treeToFile(tree, "tree.txt");
        }
        else if(input == "T_SEARCH"){
            cout << "Введите число для поиска" << endl << ">> ";
            cin >> data;

            searchData(tree, data);
        }
        else if(input == "T_IS_FULL"){
            if(TreeIsFull(tree.root)){
                cout << "Дерево полное" << endl;
            }
            else{
                cout << "Дерево не полное" << endl;
            }
        }
        else if(input == "T_CLEAR"){
            clearTree(tree);
            treeToFile(tree, "tree.txt");
        }
        else if(input == "VISUALIZE"){
            treePrint(tree);
        }

        else if(input == "T_PRE_ORDER"){
            printPreOrder(tree);
        }
        else if(input == "T_IN_ORDER"){
            printInOrder(tree);
        }
        else if(input == "T_POST_ORDER"){
            printPostOrder(tree);
        }


        else if(input == "PRINT"){
            string type;
            cout << "Что хотите напечатать? " << endl
            << "FLIST" << endl
            << "LLIST" << endl
            << "QUEUE" << endl
            << "STACK" << endl
            << "ARRAY" << endl
            << ">> ";
            if(type == "FLIST"){
                print(&fList);
            }
            else if(type == "LLIST"){
                print(&dList);
            }
            else if(type == "QUEUE"){
                Qprint(&que);
            }
            else if(type == "STACK"){
                Sprint(&stack);
            }
            else if(type == "ARRAY"){
                print(arr);
            }

            clearStack(&stack);
            clearQueue(&que);
            clearFList(&fList);
            clearDList(&dList);
            clearTree(tree);
            clearArray(arr);
        }

    }

    return 0;
}





