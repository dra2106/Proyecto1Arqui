/*
 *
 * Nombre de Archivo: DNode.h
 *
 * Descripci�n General:
 *
 * Nodo que nos ayuda a implementar Doubly Linked List y otras estructuras.
 *
 * Autor: Mauricio Avil�s
 *
 */

#pragma once

template <typename E>
class DNode {
public:
	E element;
	DNode<E>* next;
	DNode<E>* previous;

	DNode(E element, DNode<E>* next = nullptr, DNode<E>* previous = nullptr) {
		this->element = element;
		this->next = next;
		this->previous = previous;
	}

	DNode(DNode<E>* next = nullptr, DNode<E>* previous = nullptr) {
		this->next = next;
		this->previous = previous;
	}

};
