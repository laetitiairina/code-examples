//
//  main.c
//  Task 2
//
//  Created by Laetitia Britschgi on 15.04.18.
//  Copyright © 2018 Laetitia Britschgi. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

struct node {
    int key;
    struct node* next;
};

struct stack {
    struct node* head;
};

struct queue {
    struct stack* stack1;
    struct stack* stack2;
};

struct stack* initStack() {
    struct stack* stackA = malloc(sizeof(struct stack));
    stackA->head = malloc(sizeof(struct node));
    stackA->head = NULL;
    return stackA;
}

struct queue* initQueue() {
    struct queue* q = malloc(sizeof(struct queue));
    q->stack1 = initStack();
    q->stack2 = initStack();
    return q;
}

// some functions for using stacks
int isEmpty(struct stack* stackA) {
    if(stackA->head == NULL) {
        return -1;
    }
    else {
        return 1;
    }
}

void push(struct stack* stackA, int key) {
    //    in case the stack is empty
    if(stackA->head == NULL) {
        stackA->head = malloc(sizeof(struct node));
        stackA->head->key = key;
        stackA->head->next = NULL;
    }
    //    in case the stack isn't empty
    else {
        struct node *q = stackA->head;
        stackA->head = malloc(sizeof(struct node));
        stackA->head->key = key;
        stackA->head->next = q;
    }
}

int pop(struct stack* stackA) {
    if(isEmpty(stackA) == -1) {
        return -1;
    }
    else {
        struct node *q = stackA->head->next;
        int key = stackA->head->key;
        free(stackA->head);
        stackA->head = q;
        return key;
    }
}

void printStack(struct stack* stackA) {
    struct node *temp = stackA->head;
    printf("[ ");
    while (temp != NULL) {
        printf("%d ", temp->key);
        temp = temp->next;
    }
    printf("]\n");
}

// a)
void enQueue(struct queue *q, int key) {
//    for enqueueing all the values have to be in stack1
    while(q->stack2->head != NULL) {
        push(q->stack1, pop(q->stack2));
    }
    push(q->stack1, key);
}

// b)
int deQueue(struct queue *q) {
//    for dequeueing all the values have to be in stack2
    while(q->stack1->head != NULL) {
        push(q->stack2, pop(q->stack1));
    }
    return pop(q->stack2);
}

// c)
void printQueue(struct queue *q) {
//    to get in right order (in stack2)
    while(q->stack1->head != NULL) {
        push(q->stack2, pop(q->stack1));
    }
    printStack(q->stack2);
}

int main(int argc, const char * argv[]) {
    struct queue* q = initQueue();
    enQueue(q, 2);
    enQueue(q, 8);
    enQueue(q, 11);
    enQueue(q, 15);
    printf("%d\n", deQueue(q));
    printf("%d\n", deQueue(q));
    enQueue(q, 1);
    enQueue(q, 5);
    printQueue(q);
    return 0;
}
