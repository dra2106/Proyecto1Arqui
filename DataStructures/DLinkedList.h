/*
 *
 * Nombre de Archivo: DLinkedList.h
 * 
 * Descripci�n General:
 *
 * Clase hija de List que implementa una lista doblemente enlazada. Esta funciona
 * con un nodo que tiene un puntero al siguiente nodo y otro al nodo anterior.
 * De manera que podemos recorrer la lista en ambas direcciones.
 *
 * Autor: Mauricio Avil�s 
 *
 */

#pragma once

#include <iostream>
#include <stdexcept>
#include "List.h"
#include "DNode.h"

using std::runtime_error;
using std::cout;
using std::endl;

template <typename E>
class DLinkedList : public List<E> {
private:
	DNode<E>* head;
	DNode<E>* current;
	DNode<E>* tail;
	int size;

public:
	DLinkedList() {
		current = head = new DNode<E>(nullptr, nullptr);
		head->next = tail = new DNode<E>(nullptr, head);
		size = 0;
	}

	~DLinkedList() {
		clear();
		delete head;
		delete tail;
	}

	// Constructor de copia
	DLinkedList(const DLinkedList<E>& other) {
		current = head = new DNode<E>(nullptr, nullptr);
		head->next = tail = new DNode<E>(nullptr, head);
		size = 0;

		DNode<E>* temp = other.head->next;
		while (temp != other.tail) {
			append(temp->element); // Reutiliza tu método append
			temp = temp->next;
		}
		current = head; // Ubica el current al inicio
	}

	// Operador de asignación
	DLinkedList<E>& operator=(const DLinkedList<E>& other) {
		if (this == &other) return *this; // Verificación de autoasignación

		clear(); // Borra el contenido actual

		DNode<E>* temp = other.head->next;
		while (temp != other.tail) {
			append(temp->element);
			temp = temp->next;
		}
		current = head;
		return *this;
	}

	void insert(E element) {
		current->next = current->next->previous 
			= new DNode<E>(element, current->next, current);
		size++;
	}

	void append(E element) {
		tail->previous = tail->previous->next 
			= new DNode<E>(element, tail, tail->previous);
		size++;
	}

	E remove() {
		if (size == 0)
			throw runtime_error("List is empty.");
		if (current->next == tail)
			throw runtime_error("No current element.");
		E result = current->next->element;
		current->next = current->next->next;
		delete current->next->previous;
		current->next->previous = current;
		size--;
		return result;
	}

	E getElement() {
		if (size == 0)
			throw runtime_error("List is empty.");
		if (current->next == tail)
			throw runtime_error("No current element.");
		return current->next->element;
	}

	bool isEmpty(){
		return size == 0;
	}

	void clear() {
		while (head->next != tail) {
			current = head->next;
			head->next = head->next->next;
			delete current;
		}
		tail->previous = head;
		current = head;
		size = 0;
	}

	void goToStart() {
		current = head;
	}

	void goToEnd() {
		current = tail->previous;
	}

	void goToPos1(int pos) {
		if (pos < 0 || pos > size)
			throw runtime_error("Index out of range");
		current = head;
		for (int i = 0; i < pos; i++) {
			current = current->next;
		}
	}

	void goToPos(int pos) {
		if (pos < 0 || pos > size)
			throw runtime_error("Index out of range");
		if (pos < size / 2) {
			current = head;
			for (int i = 0; i < pos; i++)
				current = current->next;
		} else {
			current = tail->previous;
			for (int i = size; i > pos; i--)
				current = current->previous;
		}
	}

	void next() {
		if (current->next != tail)
			current = current->next;
	}

	void previous() {
		if (current != head)
			current = current->previous;
	}

	bool atStart() {
		return current == head;
	}

	bool atEnd() {
		return current->next == tail;
	}

	int getPos() {
		int pos = 0;
		DNode<E>* temp = head;
		while (temp != current) {
			pos++;
			temp = temp->next;
		}
		return pos;
	}

	int getSize() {
		return size;
	}

	void print() {
		cout << "[ ";
		DNode<E>* temp = head->next;
		while (temp != tail) {
			if (temp == current->next)
				cout << "*";
			cout << temp->element << " ";
			temp = temp->next;
		}
		if (current->next == tail)
			cout << "*";
		cout << "]" << endl;
	}

};

