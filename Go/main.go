package main

import (
	"bufio"
	"fmt"
	"io"
	"os"
	"strconv"
	"strings"
)

// --- Общие утилиты для ввода ---

// Используем один сканер для всего стандартного ввода
var globalScanner = bufio.NewScanner(os.Stdin)

// readString считывает строку от пользователя
func readString(prompt string) string {
	fmt.Println(prompt)
	fmt.Print(">> ")
	globalScanner.Scan()
	return globalScanner.Text()
}

// readInt считывает целое число от пользователя
func readInt(prompt string) int {
	fmt.Println(prompt)
	fmt.Print(">> ")
	globalScanner.Scan()
	val, err := strconv.Atoi(globalScanner.Text())
	if err != nil {
		fmt.Println("Ошибка: Введите число")
		return -1 // Возвращаем -1 как признак ошибки
	}
	return val
}

// --- ARRAY (array.go) ---
// В Go MyArray - это просто обертка вокруг среза,
// который автоматически управляет емкостью.

type MyArray struct {
	data []string
}

func NewEmptyMyArray() *MyArray {
	return &MyArray{data: make([]string, 0)}
}

func (arr *MyArray) addTail(key string) {
	arr.data = append(arr.data, key)
}

func (arr *MyArray) addToIndex(index int, key string) error {
	if index < 0 || index > len(arr.data) {
		return fmt.Errorf("Array index out of bounds")
	}
	// Вставляем элемент
	arr.data = append(arr.data, "") // Расширяем срез
	copy(arr.data[index+1:], arr.data[index:]) // Сдвигаем элементы
	arr.data[index] = key
	return nil
}

func (arr *MyArray) indexKey(index int) (string, error) {
	if index < 0 || index >= len(arr.data) {
		return "", fmt.Errorf("Array index out of bounds")
	}
	return arr.data[index], nil
}

func (arr *MyArray) deleteNode(index int) error {
	if index < 0 || index >= len(arr.data) {
		return fmt.Errorf("Array index out of bounds")
	}
	// Удаляем элемент
	arr.data = append(arr.data[:index], arr.data[index+1:]...)
	return nil
}

func (arr *MyArray) swapNode(index int, swapkey string) error {
	if index < 0 || index >= len(arr.data) {
		return fmt.Errorf("Array index out of bounds")
	}
	arr.data[index] = swapkey
	return nil
}

func (arr *MyArray) print() {
	for _, key := range arr.data {
		fmt.Print(key, " ")
	}
	fmt.Println()
}

func (arr *MyArray) arraySize() int {
	return len(arr.data)
}

func (arr *MyArray) clearArray() {
	arr.data = make([]string, 0)
}

func (arr *MyArray) arrayToFile(filename string) {
	if len(arr.data) == 0 {
		fmt.Println("Предупреждение: Записывается пустой массив в файл")
	}
	file, err := os.Create(filename)
	if err != nil {
		fmt.Fprintf(os.Stderr, "Ошибка: Не удалось создать файл %s\n", filename)
		return
	}
	defer file.Close()

	fmt.Fprintf(file, "%d ", len(arr.data))
	for _, key := range arr.data {
		// Используем strconv.Quote для корректной записи строк (даже с пробелами)
		// Но для совместимости с C++ версией, просто пишем как есть
		fmt.Fprintf(file, "%s ", key)
	}
	fmt.Printf("Успешно записано %d элементов массива в файл %s\n", len(arr.data), filename)
}

func (arr *MyArray) fileToArray(filename string) {
	arr.clearArray()
	file, err := os.Open(filename)
	if err != nil {
		fmt.Println("Ошибка открытия файла для чтения")
		return
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)
	scanner.Split(bufio.ScanWords)

	var arraySize int
	if scanner.Scan() {
		arraySize, _ = strconv.Atoi(scanner.Text())
	}

	if arraySize > 0 {
		arr.data = make([]string, 0, arraySize)
		for i := 0; i < arraySize && scanner.Scan(); i++ {
			arr.data = append(arr.data, scanner.Text())
		}
	}
	fmt.Printf("Массив загружен из файла: %s (размер: %d)\n", filename, len(arr.data))
}

// --- FORWARD LIST (flist.go) ---

type FNode struct {
	key  string
	next *FNode
}

type FList struct {
	head *FNode
	size int
}

func NewFList() *FList {
	return &FList{head: nil, size: 0}
}

func (list *FList) findFNode(data string) *FNode {
	current := list.head
	for current != nil {
		if current.key == data {
			return current
		}
		current = current.next
	}
	return nil
}

func (list *FList) addAfter(target, data string) *FNode {
	afterNode := list.findFNode(target)
	if afterNode == nil {
		return nil
	}
	newNode := &FNode{key: data, next: afterNode.next}
	afterNode.next = newNode
	list.size++
	return newNode
}

func (list *FList) addToFront(data string) {
	newNode := &FNode{key: data, next: list.head}
	list.head = newNode
	list.size++
}

func (list *FList) addBefore(target, data string) *FNode {
	beforeNode := list.findFNode(target)
	if beforeNode == nil {
		return nil
	}
	newNode := &FNode{key: data}
	if beforeNode == list.head {
		newNode.next = list.head
		list.head = newNode
		list.size++
		return newNode
	}

	current := list.head
	for current != nil && current.next != beforeNode {
		current = current.next
	}

	if current != nil {
		newNode.next = beforeNode
		current.next = newNode
		list.size++
	} else {
		return nil
	}
	return newNode
}

func (list *FList) addToEnd(data string) *FNode {
	newNode := &FNode{key: data, next: nil}
	if list.head == nil {
		list.head = newNode
		list.size++
		return newNode
	}
	current := list.head
	for current.next != nil {
		current = current.next
	}
	current.next = newNode
	list.size++
	return newNode
}

func (list *FList) deleteAfter(data string) {
	afterNode := list.findFNode(data)
	if afterNode == nil || afterNode.next == nil {
		return
	}
	afterNode.next = afterNode.next.next
	list.size--
}

func (list *FList) deleteHead() {
	if list.head == nil {
		return
	}
	list.head = list.head.next
	list.size--
}

func (list *FList) deleteBefore(data string) {
	beforeNode := list.findFNode(data)
	if list.head == nil || beforeNode == nil || list.head == beforeNode {
		return
	}
	if list.head.next == beforeNode {
		list.deleteHead()
		return
	}
	current := list.head
	for current != nil && current.next != nil && current.next.next != beforeNode {
		current = current.next
	}
	if current != nil && current.next != nil {
		current.next = beforeNode
		list.size--
	}
}

func (list *FList) deleteVal(data string) {
	if list.head == nil {
		return
	}
	if list.head.key == data {
		list.deleteHead()
		return
	}
	current := list.head
	for current.next != nil && current.next.key != data {
		current = current.next
	}
	if current.next != nil && current.next.key == data {
		current.next = current.next.next
		list.size--
	}
}

func (list *FList) deleteTail() {
	if list.head == nil {
		return
	}
	if list.head.next == nil {
		list.head = nil
		list.size--
		return
	}
	current := list.head
	for current.next.next != nil {
		current = current.next
	}
	current.next = nil
	list.size--
}

func (list *FList) print() {
	current := list.head
	for current != nil {
		fmt.Print(current.key, " ")
		current = current.next
	}
	fmt.Println()
}

func (list *FList) clearFList() {
	list.head = nil
	list.size = 0
}

func (list *FList) findData(data string) {
	if list.findFNode(data) != nil {
		fmt.Printf("Значение %s найдено\n", data)
	} else {
		fmt.Printf("Значение %s не найдено\n", data)
	}
}

func (list *FList) FListToFile(filename string) {
	if list.head == nil {
		fmt.Println("Предупреждение: Записывается пустой список в файл")
	}
	file, err := os.Create(filename)
	if err != nil {
		fmt.Fprintf(os.Stderr, "Ошибка: Не удалось создать файл %s\n", filename)
		return
	}
	defer file.Close()

	fmt.Fprintf(file, "%d ", list.size)
	current := list.head
	count := 0
	for current != nil {
		fmt.Fprintf(file, "%s ", current.key)
		current = current.next
		count++
	}
	fmt.Printf("Успешно записано %d элементов в файл %s\n", count, filename)
}

func (list *FList) fileToFList(filename string) {
	list.clearFList()
	file, err := os.Open(filename)
	if err != nil {
		fmt.Println("Ошибка открытия файла для чтения")
		return
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)
	scanner.Split(bufio.ScanWords)

	var listSize int
	if scanner.Scan() {
		listSize, _ = strconv.Atoi(scanner.Text())
	}

	for i := 0; i < listSize && scanner.Scan(); i++ {
		list.addToEnd(scanner.Text())
	}
	fmt.Printf("Список загружен из файла: %s\n", filename)
}

// --- DOUBLY LINKED LIST (dlist.go) ---

type DNode struct {
	key  string
	next *DNode
	prev *DNode
}

type DList struct {
	head *DNode
	tail *DNode
	size int
}

func NewDList() *DList {
	return &DList{head: nil, tail: nil, size: 0}
}

func (list *DList) print() {
	itr := list.head
	for itr != nil {
		fmt.Print(itr.key, " ")
		itr = itr.next
	}
	fmt.Println()
}

func (list *DList) findDNode(value string) *DNode {
	current := list.head
	for current != nil {
		if current.key == value {
			return current
		}
		current = current.next
	}
	return nil
}

func (list *DList) addHead(data string) {
	newNode := &DNode{key: data, prev: nil}
	if list.head == nil {
		newNode.next = nil
		list.head = newNode
		list.tail = newNode
	} else {
		newNode.next = list.head
		list.head.prev = newNode
		list.head = newNode
	}
	list.size++
}

func (list *DList) addEnd(data string) {
	if list.head == nil {
		list.addHead(data)
		return
	}
	newNode := &DNode{key: data, next: nil, prev: list.tail}
	list.tail.next = newNode
	list.tail = newNode
	list.size++
}

func (list *DList) addAfter(target, data string) *DNode {
	afterNode := list.findDNode(target)
	if afterNode == nil {
		return nil
	}
	if afterNode.next == nil {
		list.addEnd(data)
		return list.tail
	}
	newNode := &DNode{key: data}
	newNode.next = afterNode.next
	newNode.prev = afterNode
	afterNode.next.prev = newNode
	afterNode.next = newNode
	list.size++
	return newNode
}

func (list *DList) addBefore(target, data string) *DNode {
	beforeNode := list.findDNode(target)
	if beforeNode == nil {
		return nil
	}
	if beforeNode.prev == nil {
		list.addHead(data)
		return list.head
	}
	newNode := &DNode{key: data}
	newNode.next = beforeNode
	newNode.prev = beforeNode.prev
	beforeNode.prev.next = newNode
	beforeNode.prev = newNode
	list.size++
	return newNode
}

func (list *DList) deleteHead() {
	if list.head == nil {
		return
	}
	deletedNode := list.head
	list.head = deletedNode.next
	if list.head != nil {
		list.head.prev = nil
	} else {
		list.tail = nil
	}
	list.size--
}

func (list *DList) deleteTail() {
	if list.head == nil {
		return
	}
	if list.head == list.tail {
		list.head = nil
		list.tail = nil
	} else {
		deletedNode := list.tail
		list.tail = deletedNode.prev
		list.tail.next = nil
	}
	list.size--
}

func (list *DList) deleteAfter(data string) {
	afterNode := list.findDNode(data)
	if list.head == nil || afterNode == nil || afterNode.next == nil {
		return
	}
	deletedNode := afterNode.next
	afterNode.next = deletedNode.next
	if deletedNode.next == nil {
		list.tail = afterNode
	} else {
		deletedNode.next.prev = afterNode
	}
	list.size--
}

func (list *DList) deleteBefore(data string) {
	beforeNode := list.findDNode(data)
	if list.head == nil || beforeNode == nil || beforeNode.prev == nil {
		return
	}
	deletedNode := beforeNode.prev
	beforeNode.prev = deletedNode.prev
	if deletedNode.prev != nil {
		deletedNode.prev.next = beforeNode
	} else {
		list.head = beforeNode
		beforeNode.prev = nil
	}
	list.size--
}

func (list *DList) deleteDNode(thisNode *DNode) {
	if list == nil || list.head == nil || thisNode == nil {
		return
	}
	if thisNode == list.tail {
		list.deleteTail()
	} else if thisNode == list.head {
		list.deleteHead()
	} else {
		thisNode.prev.next = thisNode.next
		thisNode.next.prev = thisNode.prev
		list.size--
	}
}

func (list *DList) clearDList() {
	list.head = nil
	list.tail = nil
	list.size = 0
}

func (list *DList) findData(data string) {
	if list.findDNode(data) != nil {
		fmt.Printf("Значение %s найдено\n", data)
	} else {
		fmt.Printf("Значение %s не найдено\n", data)
	}
}

func (list *DList) DListToFile(filename string) {
	if list.head == nil {
		fmt.Println("Предупреждение: Записывается пустой список в файл")
	}
	file, err := os.Create(filename)
	if err != nil {
		fmt.Fprintf(os.Stderr, "Ошибка: Не удалось создать файл %s\n", filename)
		return
	}
	defer file.Close()

	fmt.Fprintf(file, "%d ", list.size)
	current := list.head
	count := 0
	for current != nil {
		fmt.Fprintf(file, "%s ", current.key)
		current = current.next
		count++
	}
	fmt.Printf("Успешно записано %d элементов в файл %s\n", count, filename)
}

func (list *DList) fileToDList(filename string) {
	list.clearDList()
	file, err := os.Open(filename)
	if err != nil {
		fmt.Println("Ошибка открытия файла для чтения")
		return
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)
	scanner.Split(bufio.ScanWords)

	var listSize int
	if scanner.Scan() {
		listSize, _ = strconv.Atoi(scanner.Text())
	}

	for i := 0; i < listSize && scanner.Scan(); i++ {
		list.addEnd(scanner.Text())
	}
	fmt.Printf("Список загружен из файла: %s\n", filename)
}

// --- QUEUE (queue.go) ---
// Реализуем универсальную (generic) очередь, так как она нужна
// и для string (в main) и для *TNode (в tree)

type QNode[T any] struct {
	key  T
	next *QNode[T]
}

type Queue[T any] struct {
	head *QNode[T]
	tail *QNode[T]
	size int
}

func NewQueue[T any]() *Queue[T] {
	return &Queue[T]{head: nil, tail: nil, size: 0}
}

func (q *Queue[T]) Qadd(data T) *QNode[T] {
	newNode := &QNode[T]{key: data, next: nil}
	if q.head == nil && q.tail == nil {
		q.tail = newNode
		q.head = newNode
	} else {
		q.tail.next = newNode
		q.tail = newNode
	}
	q.size++
	return newNode
}

// Qprint для string. Универсальная печать сложнее.
func (q *Queue[string]) Qprint() {
	ptr := q.head
	for ptr != nil {
		fmt.Print(ptr.key, " ") // Исправлена ошибка: печатаем значение, а не адрес
		ptr = ptr.next
	}
	fmt.Println()
}

func (q *Queue[T]) Qdelete() {
	if q.head == nil {
		return
	}
	q.head = q.head.next
	if q.head == nil {
		q.tail = nil
	}
	q.size--
}

func (q *Queue[T]) clearQueue() {
	q.head = nil
	q.tail = nil
	q.size = 0
}

func (q *Queue[T]) qEmpty() bool {
	return q.head == nil
}

func (q *Queue[T]) getHead() T {
	if q.head == nil {
		var zero T
		return zero // C++ тут бы упал
	}
	return q.head.key
}

func (q *Queue[T]) getTail() T {
	if q.tail == nil {
		var zero T
		return zero
	}
	return q.tail.key
}

// --- QUEUE I/O (queue_io.go) ---
// Функции для работы с файлами для Queue[string]

func queueToFile(q *Queue[string], filename string) {
	if q.head == nil {
		fmt.Println("Предупреждение: Записывается пустая очередь в файл")
	}
	file, err := os.Create(filename)
	if err != nil {
		fmt.Fprintf(os.Stderr, "Ошибка: Не удалось создать файл %s\n", filename)
		return
	}
	defer file.Close()

	fmt.Fprintf(file, "%d ", q.size)
	current := q.head
	count := 0
	for current != nil {
		fmt.Fprintf(file, "%s ", current.key)
		current = current.next
		count++
	}
	fmt.Printf("Успешно записано %d элементов в файл %s\n", count, filename)
}

func fileToQueue(q *Queue[string], filename string) {
	q.clearQueue()
	file, err := os.Open(filename)
	if err != nil {
		fmt.Println("Ошибка открытия файла для чтения")
		return
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)
	scanner.Split(bufio.ScanWords)

	var queueSize int
	if scanner.Scan() {
		queueSize, _ = strconv.Atoi(scanner.Text())
	}

	for i := 0; i < queueSize && scanner.Scan(); i++ {
		q.Qadd(scanner.Text())
	}
	fmt.Printf("Очередь загружена из файла: %s\n", filename)
}

// --- STACK (stack.go) ---

type SNode struct {
	key  string
	next *SNode
}

type Stack struct {
	head *SNode
	size int
}

func NewStack() *Stack {
	return &Stack{head: nil, size: 0}
}

func (s *Stack) Sadd(data string) *SNode {
	newNode := &SNode{key: data, next: s.head}
	s.head = newNode
	s.size++
	return newNode
}

func (s *Stack) Sdelete() {
	if s.head == nil {
		fmt.Println("в этом стеке нечего удалять.")
		return
	}
	s.head = s.head.next
	s.size--
}

func (s *Stack) Sprint() {
	if s.head == nil {
		fmt.Println("стек пуст")
	}
	current := s.head
	for current != nil {
		fmt.Print(current.key, " ")
		current = current.next
	}
	fmt.Println()
}

func (s *Stack) clearStack() {
	s.head = nil
	s.size = 0
}

func (s *Stack) stackToFile(filename string) {
	if s.head == nil {
		fmt.Println("Предупреждение: Записывается пустой стек в файл")
	}
	file, err := os.Create(filename)
	if err != nil {
		fmt.Fprintf(os.Stderr, "Ошибка: Не удалось создать файл %s\n", filename)
		return
	}
	defer file.Close()

	fmt.Fprintf(file, "%d ", s.size)
	current := s.head
	count := 0
	for current != nil {
		fmt.Fprintf(file, "%s ", current.key)
		current = current.next
		count++
	}
	fmt.Printf("Успешно записано %d элементов в файл %s\n", count, filename)
}

func (s *Stack) fileToStack(filename string) {
	s.clearStack()
	file, err := os.Open(filename)
	if err != nil {
		fmt.Println("Ошибка открытия файла для чтения")
		return
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)
	scanner.Split(bufio.ScanWords)

	var stackSize int
	if scanner.Scan() {
		stackSize, _ = strconv.Atoi(scanner.Text())
	}

	// Считываем во временный стек, чтобы сохранить порядок
	tempStack := NewStack()
	for i := 0; i < stackSize && scanner.Scan(); i++ {
		tempStack.Sadd(scanner.Text())
	}

	// Перекладываем из временного стека в основной (восстанавливая порядок)
	current := tempStack.head
	for current != nil {
		s.Sadd(current.key)
		current = current.next
	}
	fmt.Printf("Стек загружен из файла: %s\n", filename)
}

// --- BINARY TREE (tree.go) ---
// Используется для int, как в C++ main

type TNode struct {
	key   int
	left  *TNode
	right *TNode
}

type FullBinary struct {
	root *TNode
	size int
}

func NewFullBinary() *FullBinary {
	return &FullBinary{root: nil, size: 0}
}

// Внутренняя рекурсивная функция
func (tree *FullBinary) deleteTreeRecursive(node *TNode) {
	if node != nil {
		tree.deleteTreeRecursive(node.left)
		tree.deleteTreeRecursive(node.right)
		// В Go просто обнуляем ссылки, GC сделает остальное
	}
}

// Очищает дерево
func (tree *FullBinary) clearTree() {
	tree.deleteTreeRecursive(tree.root)
	tree.root = nil
	tree.size = 0
}

// Очищает и печатает сообщение
func (tree *FullBinary) ClearTreeAndPrint() {
	tree.clearTree()
	fmt.Println("Дерево очищено")
}

func (tree *FullBinary) preordeSearch(root *TNode, key int) bool {
	if root != nil {
		if root.key == key {
			return true
		}
		if tree.preordeSearch(root.left, key) {
			return true
		}
		if tree.preordeSearch(root.right, key) {
			return true
		}
	}
	return false
}

func (tree *FullBinary) searchData(key int) {
	if tree.preordeSearch(tree.root, key) {
		fmt.Printf("Элемент %d найден\n", key)
	} else {
		fmt.Printf("Элемент %d не найден\n", key)
	}
}

func (tree *FullBinary) BTadd(key int) {
	newNode := &TNode{key: key, left: nil, right: nil}
	if tree.root == nil {
		tree.root = newNode
		tree.size++
		fmt.Printf("Добавлен корень: %d\n", key)
		return
	}

	// Используем нашу универсальную очередь для *TNode
	q := NewQueue[*TNode]()
	q.Qadd(tree.root)

	for !q.qEmpty() {
		current := q.getHead()
		q.Qdelete()

		if current.left == nil {
			current.left = newNode
			tree.size++
			fmt.Printf("Добавлен левый потомок %d к узлу %d\n", key, current.key)
			return
		} else if current.right == nil {
			current.right = newNode
			tree.size++
			fmt.Printf("Добавлен правый потомок %d к узлу %d\n", key, current.key)
			return
		} else {
			q.Qadd(current.left)
			q.Qadd(current.right)
		}
	}
}

func (tree *FullBinary) printTreeRecursive(node *TNode, prefix string, isLeft bool) {
	if node == nil {
		return
	}

	var newPrefixRight string
	if isLeft {
		newPrefixRight = prefix + "│   "
	} else {
		newPrefixRight = prefix + "    "
	}
	tree.printTreeRecursive(node.right, newPrefixRight, false)

	fmt.Print(prefix)
	if isLeft {
		fmt.Print("└── ")
	} else {
		fmt.Print("┌── ")
	}
	fmt.Println(node.key)

	var newPrefixLeft string
	if isLeft {
		newPrefixLeft = prefix + "    "
	} else {
		newPrefixLeft = prefix + "│   "
	}
	tree.printTreeRecursive(node.left, newPrefixLeft, true)
}

func (tree *FullBinary) treePrint() {
	if tree.root == nil {
		fmt.Println("Дерево пустое.")
		return
	}
	fmt.Println("Визуализация дерева:")
	tree.printTreeRecursive(tree.root, "", false)
	fmt.Println()
}

func (tree *FullBinary) TreeIsFull(current *TNode) bool {
	if current == nil {
		return true // Пустое дерево (или узел) считается полным
	}
	// XOR
	if (current.left == nil) != (current.right == nil) {
		return false
	}
	if current.left != nil && current.right != nil {
		return tree.TreeIsFull(current.left) && tree.TreeIsFull(current.right)
	}
	// Оба nil
	return true
}

// --- Обходы ---
func (tree *FullBinary) preOrder(root *TNode) {
	if root != nil {
		fmt.Print(root.key, " ")
		tree.preOrder(root.left)
		tree.preOrder(root.right)
	}
}

func (tree *FullBinary) printPreOrder() {
	fmt.Print("Прямой обход: ")
	tree.preOrder(tree.root)
	fmt.Println()
}

func (tree *FullBinary) inOrder(root *TNode) {
	if root != nil {
		tree.inOrder(root.left)
		fmt.Print(root.key, " ")
		tree.inOrder(root.right)
	}
}

func (tree *FullBinary) printInOrder() {
	fmt.Print("Центрированный обход: ")
	tree.inOrder(tree.root)
	fmt.Println()
}

func (tree *FullBinary) postOrder(root *TNode) {
	if root != nil {
		tree.postOrder(root.left)
		tree.postOrder(root.right)
		fmt.Print(root.key, " ")
	}
}

func (tree *FullBinary) printPostOrder() {
	fmt.Print("Обратный обход: ")
	tree.postOrder(tree.root)
	fmt.Println()
}

func (tree *FullBinary) printBFS() {
	if tree.root == nil {
		fmt.Println("Дерево пустое")
		return
	}
	fmt.Print("Обход в ширину: ")
	q := NewQueue[*TNode]()
	q.Qadd(tree.root)

	for !q.qEmpty() {
		current := q.getHead()
		q.Qdelete()
		fmt.Print(current.key, " ")
		if current.left != nil {
			q.Qadd(current.left)
		}
		if current.right != nil {
			q.Qadd(current.right)
		}
	}
	fmt.Println()
}

// --- Файлы дерева ---

func (tree *FullBinary) treeToFile(filename string) {
	file, err := os.Create(filename)
	if err != nil {
		fmt.Fprintf(os.Stderr, "Ошибка: Не удалось создать файл %s\n", filename)
		return
	}
	defer file.Close()

	fmt.Fprintf(file, "%d\n", tree.size)
	if tree.root == nil {
		fmt.Printf("Успешно записано пустое дерево в файл %s\n", filename)
		return
	}

	q := NewQueue[*TNode]()
	q.Qadd(tree.root)

	count := 0
	for !q.qEmpty() {
		current := q.getHead()
		q.Qdelete()

		fmt.Fprintf(file, "%d", current.key)
		count++

		if current.left != nil {
			q.Qadd(current.left)
		}
		if current.right != nil {
			q.Qadd(current.right)
		}

		if !q.qEmpty() {
			fmt.Fprint(file, " ")
		}
	}
	fmt.Fprintln(file, "")
	fmt.Printf("Успешно записано %d элементов дерева в файл %s\n", count, filename)
}

func (tree *FullBinary) rebuildTree(data []int) {
	if len(data) == 0 {
		return
	}
	tree.clearTree()

	tree.root = &TNode{key: data[0], left: nil, right: nil}
	tree.size = 1

	q := NewQueue[*TNode]()
	q.Qadd(tree.root)

	index := 1
	n := len(data)

	for !q.qEmpty() && index < n {
		current := q.getHead()
		q.Qdelete()

		if index < n { // левый потомок
			current.left = &TNode{key: data[index], left: nil, right: nil}
			q.Qadd(current.left)
			tree.size++
			index++
		}

		if index < n { // правый потомок
			current.right = &TNode{key: data[index], left: nil, right: nil}
			q.Qadd(current.right)
			tree.size++
			index++
		}
	}
}

func (tree *FullBinary) fileToTree(filename string) {
	tree.clearTree()
	file, err := os.Open(filename)
	if err != nil {
		fmt.Println("Ошибка открытия файла для чтения:", filename)
		return
	}
	defer file.Close()

	// Используем bufio.Reader для чтения строки
	reader := bufio.NewReader(file)

	// Читаем первую строку (размер)
	line, err := reader.ReadString('\n')
	if err != nil {
		fmt.Println("Ошибка чтения размера дерева из файла")
		return
	}

	var treeSize int
	_, err = fmt.Sscanf(strings.TrimSpace(line), "%d", &treeSize)
	if err != nil {
		fmt.Println("Ошибка парсинга размера дерева:", err)
		return
	}

	fmt.Println("Размер дерева из файла:", treeSize)
	if treeSize == 0 {
		fmt.Printf("Загружено пустое дерево из файла: %s\n", filename)
		return
	}

	// Читаем вторую строку (данные)
	line, err = reader.ReadString('\n')
	if err != nil && err != io.EOF {
		fmt.Println("Ошибка чтения данных дерева:", err)
		return
	}
	fmt.Println("Прочитана строка данных:", line)

	data := make([]int, 0, treeSize)
	// strings.Fields обрабатывает пробелы
	for _, sVal := range strings.Fields(line) {
		val, err := strconv.Atoi(sVal)
		if err == nil {
			data = append(data, val)
			fmt.Println("Добавлен элемент:", val)
		}
	}

	if len(data) != treeSize {
		fmt.Printf("Предупреждение: Прочитано %d элементов, но ожидалось %d\n", len(data), treeSize)
		if len(data) == 0 {
			fmt.Println("Не удалось прочитать ни одного элемента данных!")
			return
		}
	}

	tree.rebuildTree(data)
	fmt.Printf("Дерево загружено из файла: %s (размер: %d)\n", filename, tree.size)
}

// --- MAIN (main.go) ---

func command() {
	fmt.Println("--- Доступные команды ---")
	fmt.Println("PRINT - Показать содержимое структуры")
	fmt.Println("HELP - Показать это сообщение")
	fmt.Println("EXIT - Выйти из программы")
	fmt.Println()
	fmt.Println("--- МАССИВ (M) ---")
	fmt.Println("M_PUSH_END - Добавить в конец массива")
	fmt.Println("M_PUSH_AT - Добавить по индексу")
	fmt.Println("M_DEL_AT - Удалить по индексу")
	fmt.Println("M_GET_AT - Получить по индексу")
	fmt.Println("M_REPLACE_AT - Заменить по индексу")
	fmt.Println("M_LENGTH - Узнать длину массива")
	fmt.Println()
	fmt.Println("--- СПИСКИ (F - односвязный, L - двусвязный) ---")
	fmt.Println("F_PUSH_HEAD / L_PUSH_HEAD - Добавить в начало")
	fmt.Println("F_PUSH_TAIL / L_PUSH_TAIL - Добавить в конец")
	fmt.Println("F_PUSH_AFTER / L_PUSH_AFTER - Добавить после значения")
	fmt.Println("F_PUSH_BEFORE / L_PUSH_BEFORE - Добавить до значения")
	fmt.Println("F_DEL_HEAD / L_DEL_HEAD - Удалить первый элемент")
	fmt.Println("F_DEL_TAIL / L_DEL_TAIL - Удалить последний элемент")
	fmt.Println("F_DEL_AFTER / L_DEL_AFTER - Удалить после значения")
	fmt.Println("F_DEL_BEFORE / L_DEL_BEFORE - Удалить до значения")
	fmt.Println("L_DEL_VAL - Удалить по значению")
	fmt.Println("F_DEL_VAL / F_GET_VAL / L_GET_VAL - Найти по значению")
	fmt.Println()
	fmt.Println("--- ОЧЕРЕДЬ (Q) ---")
	fmt.Println("Q_PUSH - Добавить в очередь")
	fmt.Println("Q_POP - Извлечь из очереди")
	fmt.Println()
	fmt.Println("--- СТЕК (S) ---")
	fmt.Println("S_PUSH - Добавить в стек")
	fmt.Println("S_POP - Извлечь из стека")
	fmt.Println()
	fmt.Println("--- ДЕРЕВО (T) ---")
	fmt.Println("T_INSERT - Вставить число в дерево")
	fmt.Println("T_SEARCH - Найти число в дереве")
	fmt.Println("T_IS_FULL - Проверить, является ли дерево полным")
	fmt.Println("T_CLEAR - Очистить дерево")
	fmt.Println("T_PRE_ORDER - Прямой обход")
	fmt.Println("T_IN_ORDER - Центрированный обход")
	fmt.Println("T_POST_ORDER - Обратный обход")
	fmt.Println("VISUALIZE - Визуализировать дерево")
	fmt.Println("-----------------------")
}

func main() {
	arr := NewEmptyMyArray()
	fList := NewFList()
	dList := NewDList()
	stack := NewStack()
	que := NewQueue[string]() // Очередь строк
	tree := NewFullBinary()     // Дерево чисел

	// Загрузка данных из файлов
	arr.fileToArray("array.txt")
	fList.fileToFList("FList.txt")
	dList.fileToDList("DList.txt")
	stack.fileToStack("stack.txt")
	fileToQueue(que, "queue.txt") // Используем специальную I/O функцию
	tree.fileToTree("tree.txt")

	fmt.Println("Данные списков загружены, введите 'HELP' для просмотра списка команд")

	for {
		fmt.Print(">> ")
		if !globalScanner.Scan() {
			break // EOF
		}
		input := globalScanner.Text()

		if input == "EXIT" || input == "BACK" {
			break
		}
		if input == "HELP" {
			command()
			continue
		}

		// Переменные для данных
		var sData, target string
		var itr, data int
		var err error

		switch input {
		// --- МАССИВ ---
		case "M_PUSH_END":
			sData = readString("Введите строку")
			arr.addTail(sData)
			arr.arrayToFile("array.txt")
		case "M_PUSH_AT":
			itr = readInt("Введите индекс")
			sData = readString("Введите строку")
			err = arr.addToIndex(itr, sData)
			if err != nil {
				fmt.Println(err)
			} else {
				arr.arrayToFile("array.txt")
			}
		case "M_DEL_AT":
			itr = readInt("Введите индекс")
			err = arr.deleteNode(itr)
			if err != nil {
				fmt.Println(err)
			} else {
				arr.arrayToFile("array.txt")
			}
		case "M_GET_AT":
			itr = readInt("Введите индекс")
			sData, err = arr.indexKey(itr)
			if err != nil {
				fmt.Println(err)
			} else {
				fmt.Printf("Значение по индексу %d :%s\n", itr, sData)
			}
		case "M_REPLACE_AT":
			itr = readInt("Введите индекс")
			sData = readString("Введите строку")
			err = arr.swapNode(itr, sData)
			if err != nil {
				fmt.Println(err)
			} else {
				arr.arrayToFile("array.txt")
			}
		case "M_LENGTH":
			fmt.Printf("Длина массива: %d\n", arr.arraySize())

		// --- ОДНОСВЯЗНЫЙ СПИСОК ---
		case "F_PUSH_HEAD":
			sData = readString("Введите строку")
			fList.addToFront(sData)
			fList.FListToFile("FList.txt")
		case "F_PUSH_TAIL":
			sData = readString("Введите строку")
			fList.addToEnd(sData)
			fList.FListToFile("FList.txt")
		case "F_PUSH_AFTER":
			target = readString("Введите значение, после которого добавить")
			sData = readString("Введите строку")
			fList.addAfter(target, sData)
			fList.FListToFile("FList.txt")
		case "F_PUSH_BEFORE":
			target = readString("Введите значение, до которого добавить")
			sData = readString("Введите строку")
			fList.addBefore(target, sData)
			fList.FListToFile("FList.txt")
		case "F_DEL_HEAD":
			fList.deleteHead()
			fList.FListToFile("FList.txt")
		case "F_DEL_TAIL":
			fList.deleteTail()
			fList.FListToFile("FList.txt")
		case "F_DEL_AFTER":
			target = readString("Введите значение, после которого удалить")
			fList.deleteAfter(target)
			fList.FListToFile("FList.txt")
		case "F_DEL_BEFORE":
			target = readString("Введите значение, до которого удалить")
			fList.deleteBefore(target)
			fList.FListToFile("FList.txt")
		case "F_DEL_VAL":
			target = readString("Введите значение для удаления")
			fList.deleteVal(target)
			fList.FListToFile("FList.txt")
		case "F_GET_VAL":
			sData = readString("Введите строку для поиска")
			fList.findData(sData)

		// --- ДВУСВЯЗНЫЙ СПИСОК ---
		case "L_PUSH_HEAD":
			sData = readString("Введите строку")
			dList.addHead(sData)
			dList.DListToFile("DList.txt")
		case "L_PUSH_TAIL":
			sData = readString("Введите строку")
			dList.addEnd(sData)
			dList.DListToFile("DList.txt")
		case "L_PUSH_AFTER":
			target = readString("Введите значение, после которого добавить")
			sData = readString("Введите строку")
			dList.addAfter(target, sData)
			dList.DListToFile("DList.txt")
		case "L_PUSH_BEFORE":
			target = readString("Введите значение, до которого добавить")
			sData = readString("Введите строку")
			dList.addBefore(target, sData)
			dList.DListToFile("DList.txt")
		case "L_DEL_HEAD":
			dList.deleteHead()
			dList.DListToFile("DList.txt")
		case "L_DEL_TAIL":
			dList.deleteTail()
			dList.DListToFile("DList.txt")
		case "L_DEL_AFTER":
			target = readString("Введите значение, после которого удалить")
			dList.deleteAfter(target)
			dList.DListToFile("DList.txt")
		case "L_DEL_BEFORE":
			target = readString("Введите значение, до которого удалить")
			dList.deleteBefore(target)
			dList.DListToFile("DList.txt")
		case "L_DEL_VAL":
			// Исправлена ошибка из C++ (использовалась sData вместо target)
			target = readString("Введите значение, которое надo удалить")
			current := dList.findDNode(target)
			dList.deleteDNode(current)
			dList.DListToFile("DList.txt")
		case "L_GET_VAL":
			sData = readString("Введите строку для поиска")
			dList.findData(sData)

		// --- ОЧЕРЕДЬ ---
		case "Q_PUSH":
			sData = readString("Введите строку")
			que.Qadd(sData)
			queueToFile(que, "queue.txt")
		case "Q_POP":
			que.Qdelete()
			queueToFile(que, "queue.txt")

		// --- СТЕК ---
		case "S_PUSH":
			sData = readString("Введите строку")
			stack.Sadd(sData)
			stack.stackToFile("stack.txt")
		case "S_POP":
			stack.Sdelete()
			stack.stackToFile("stack.txt")

		// --- ДЕРЕВО ---
		case "T_INSERT":
			data = readInt("Введите число")
			if data != -1 {
				tree.BTadd(data)
				tree.treeToFile("tree.txt")
			}
		case "T_SEARCH":
			data = readInt("Введите число для поиска")
			if data != -1 {
				tree.searchData(data)
			}
		case "T_IS_FULL":
			if tree.root == nil {
				fmt.Println("Дерево пустое")
			} else if tree.TreeIsFull(tree.root) {
				fmt.Println("Дерево полное")
			} else {
				fmt.Println("Дерево не полное")
			}
		case "T_CLEAR":
			tree.ClearTreeAndPrint()
			tree.treeToFile("tree.txt")
		case "VISUALIZE":
			tree.treePrint()
		case "T_PRE_ORDER":
			tree.printPreOrder()
		case "T_IN_ORDER":
			tree.printInOrder()
		case "T_POST_ORDER":
			tree.printPostOrder()

		// --- ПЕЧАТЬ ---
		case "PRINT":
			// Исправлена ошибка: C++ не считывал ввод
			fmt.Println("Что хотите напечатать?")
			fmt.Println("FLIST, LLIST, QUEUE, STACK, ARRAY")
			fmt.Print(">> ")
			if !globalScanner.Scan() {
				break
			}
			typePrint := globalScanner.Text()

			switch typePrint {
			case "FLIST":
				fList.print()
			case "LLIST":
				dList.print()
			case "QUEUE":
				que.Qprint()
			case "STACK":
				stack.Sprint()
			case "ARRAY":
				arr.print()
			default:
				fmt.Println("Неизвестный тип структуры")
			}
			// Исправлена ошибка: C++ удалял все данные после печати

		default:
			fmt.Println("Неизвестная команда. Введите 'HELP'.")
		}
	}

	if err := globalScanner.Err(); err != nil {
		fmt.Fprintln(os.Stderr, "Ошибка чтения:", err)
	}
}
