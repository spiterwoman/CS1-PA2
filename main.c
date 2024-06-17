/* COP 3502C Assignment 2
This program is written by: Chrystel Geno */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LINES 12

//structs
typedef struct customer {
    char name[16];
    int numOfSmoothies;
    int lineNumber;
    int timeEnteringLine;
} customer;

typedef struct node {
    customer* customer;
    struct node* nextNode;
} node;

typedef struct Queue {
    node* front;
    node* rear;
} Queue;

//function prototypes
customer* createCustomer(char customerName[], int numSmoothies, int lineNum, int lineTime);
Queue* init();
void Enqueue(Queue* queue, customer* customer);
customer* Dequeue(Queue* queue);
customer* Peek(Queue* queue);
int isEmpty(Queue* queue);
void freeQueue(Queue* queue);

//main
int main(void) {
    int testCases, numOfCustomers, time, lineNumber, numSmoothies;
    char name[16];
    scanf("%d", &testCases);

    char **output = (char **)malloc(testCases * 500000 * sizeof(char *));
    int outputIndex = 0;

    for (int i = 0; i < testCases; i++) {
        scanf("%d", &numOfCustomers);

        //initialize array of queues for each line
        Queue* lines[LINES];
        for (int j = 0; j < LINES; j++) {
            lines[j] = init();
        }

        //reads customer + details and adds them to their respective line
        for (int k = 0; k < numOfCustomers; k++) {
            scanf("%d %d %s %d", &time, &lineNumber, name, &numSmoothies);
            customer* newCustomer = createCustomer(name, numSmoothies, lineNumber, time);
            Enqueue(lines[lineNumber - 1], newCustomer);
        }

        int currentTime = 0;
        while (1) {
            customer* nextCustomer = NULL;
            int minSmoothies = 101, selectedLine = -1;

            for (int l = 0; l < LINES; l++) {
                if (!isEmpty(lines[l])) {
                    customer* frontCustomer = Peek(lines[l]);
                    if (frontCustomer->timeEnteringLine <= currentTime) {
                        if ((frontCustomer->numOfSmoothies < minSmoothies) ||
                            (frontCustomer->numOfSmoothies == minSmoothies && l < selectedLine)) {
                            minSmoothies = frontCustomer->numOfSmoothies;
                            nextCustomer = frontCustomer;
                            selectedLine = l;
                        }
                    }
                }
            }
            if (nextCustomer == NULL) {
                int earliestTime = 1000000000;
                for (int m = 0; m < LINES; m++) {
                    if (!isEmpty(lines[m])) {
                        customer* frontCustomer = Peek(lines[m]);
                        if (frontCustomer->timeEnteringLine < earliestTime) {
                            earliestTime = frontCustomer->timeEnteringLine;
                            selectedLine = m;
                        }
                    }
                }
                if (earliestTime == 1000000000) {
                    break;
                }
                currentTime = earliestTime;
                continue;
            }
            Dequeue(lines[selectedLine]);
            int checkoutTime = currentTime + 30 + (5 * nextCustomer->numOfSmoothies);
            char* result = (char*)malloc(100 * sizeof(char));
            sprintf(result, "At time %d, %s left the counter from line %d.\n", checkoutTime, nextCustomer->name, nextCustomer->lineNumber);
            output[outputIndex++] = result;
            currentTime = checkoutTime;
            free(nextCustomer);
        }

        //frees all queues
        for (int j = 0; j < LINES; j++) {
            freeQueue(lines[j]);
        }
    }

    //print and free output strings
    for (int i = 0; i < outputIndex; i++) {
        printf("%s", output[i]);
        free(output[i]);
    }
    free(output);

    return 0;
}

//creates a customer and initializes it values
customer* createCustomer(char customerName[], int numSmoothies, int lineNum, int lineTime) {
    customer* newCustomer = (customer*)malloc(sizeof(customer));
    strcpy(newCustomer->name, customerName);
    newCustomer->numOfSmoothies = numSmoothies;
    newCustomer->lineNumber = lineNum;
    newCustomer->timeEnteringLine = lineTime;
    return newCustomer;
}

//makes front and rear of queue as NULL / initializes a new queue
Queue* init() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->front = NULL;
    queue->rear = NULL;
    return queue;
}

//adds a node at the end of the queue
void Enqueue(Queue* queue, customer* customer) {
    node* newNode = (node*)malloc(sizeof(node));
    newNode->customer = customer;
    newNode->nextNode = NULL;
    if (queue->rear == NULL) {
        queue->front = newNode;
        queue->rear = newNode;
    } else {
        queue->rear->nextNode = newNode;
        queue->rear = newNode;
    }
}

//deletes a node from the front of the queue
customer* Dequeue(Queue* queue) {
    if (isEmpty(queue)) {
        return NULL;
    }
    node* temp = queue->front;
    customer* customer = temp->customer;
    queue->front = queue->front->nextNode;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    free(temp);
    return customer;
}

//returns customer to the front of the queue w/o removing
customer* Peek(Queue* queue) {
    if (isEmpty(queue)) {
        return NULL;
    }
    return queue->front->customer;
}

//checks if the queue is empty
int isEmpty(Queue* queue) {
    return queue->front == NULL;
}

//frees all nodes / customers in the queue
void freeQueue(Queue* queue) {
    while (!isEmpty(queue)) {
        customer* customer = Dequeue(queue);
        free(customer);
    }
    free(queue);
}
