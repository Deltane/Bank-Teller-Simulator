#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct LinkedListNode {
    void* pData;
    struct LinkedListNode* pNext;
} LinkedListNode;

typedef struct LinkedList {
    LinkedListNode* pHead;
    LinkedListNode* pTail; 
    int length;
} LinkedList;

typedef void (*listFunc)(void* data);

LinkedList* createLinkedList();
LinkedListNode* getLinkedListNode(LinkedList* list, int n);
void insertStart(LinkedList* list, void* entry);
void* removeStart(LinkedList* list);
void insertLast(LinkedList* list, void* entry);
void* removeLast(LinkedList* list);
void printLinkedList(LinkedList* list, listFunc func);
void freeLinkedList(LinkedList* list, listFunc func);

#endif
