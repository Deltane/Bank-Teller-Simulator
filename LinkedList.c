#include <stdlib.h>
#include "linkedlist.h"

LinkedList* createLinkedList() 
{
    /* Create empty linked list */

    LinkedList* list = (LinkedList*) malloc(sizeof(LinkedList));

    list->pHead = NULL;
    list->pTail = NULL;
    list->length = 0;

    return list;
}

LinkedListNode* getLinkedListNode(LinkedList* list, int n) {
    /* Gets the nth element of a linked list */
    int i = 0;
    LinkedListNode* pointer = NULL;

    if (n >= 0 && n < list->length) {
        pointer = list->pHead;
        while (i < n && pointer != list->pTail) {
            pointer = pointer->pNext;
            i++;
        }
    }
    return pointer;
}

void insertStart(LinkedList* list, void* entry) {
    /* Inserts a node at the start of a linked list */
    LinkedListNode* oldHead = list->pHead;
    list->pHead = (LinkedListNode*) malloc(sizeof(LinkedListNode));

    list->pHead->pData = entry;
    list->pHead->pNext = oldHead;

    list->length += 1;

    if (list->length == 1) {
        list->pTail = list->pHead;
    }
}

void* removeStart(LinkedList* list) {
    /* Frees the first node and returns its data */
    LinkedListNode* start = list->pHead;
    void* data = start->pData;

    list->pHead = start->pNext;
    start->pNext = NULL; 

    list->length -= 1;
    free(start);

    return data;
}

void insertLast(LinkedList* list, void* entry) {
    /* Inserts a node at the end of a linked list */
    if (list->length == 0) {
        insertStart(list, entry);
    } else {
        LinkedListNode* newTail = (LinkedListNode*)malloc(sizeof(LinkedListNode));
        list->pTail->pNext = newTail;
        list->pTail = newTail;

        newTail->pData = entry;
        newTail->pNext = NULL;

        list->length += 1;
    } 

}

void* removeLast(LinkedList* list) {
    /* Frees the last node and returns its data */
    if (list->length == 0) {
        return NULL;
    } else if (list->length == 1) {
        return removeStart(list);
    } else {
        void* data = list->pTail->pData;
        LinkedListNode* pointer = list->pHead;

        while (pointer->pNext->pNext != NULL) {
            pointer = pointer->pNext;
        }

        free(list->pTail);

        list->pTail = pointer;
        list->pTail->pNext = NULL;
        list->length -= 1;
        
        return data;
    }
}

void printLinkedList(LinkedList* list, listFunc func) {
    /* Uses a callback function to print the data of each node */
    LinkedListNode* pointer = list->pHead;
    while (pointer != NULL) {
        (*func)(pointer->pData);
        pointer = pointer->pNext;
    }
    
}

void freeLinkedList(LinkedList* list, listFunc func) {
    /* Uses a callback function to free the data of each node and the list */
    LinkedListNode* pointer = list->pHead;
    LinkedListNode* temp = NULL;
    while(pointer != NULL) {
        (*func)(pointer->pData);
        temp = pointer;
        pointer = pointer->pNext;
        free(temp);
        list->length -= 1;
        list->pHead = pointer;
    }

    free(list);
}
