/** pagequeue.c
 * ===========================================================
 * Name: Jack Barnett, April 20 2026
 * Section: CS483 / M3
 * Project: PEX3 - Page Replacement Simulator
 * Purpose: Implementation of the PageQueue ADT — a doubly-linked
 *          list for LRU page replacement.
 *          Head = LRU (eviction end), Tail = MRU end.
 * =========================================================== */
#include <stdio.h>
#include <stdlib.h>

#include "pagequeue.h"

/**
 * @brief Create and initialize a page queue with a given capacity
 */
PageQueue *pqInit(unsigned int maxSize) {
    // TODO: malloc a PageQueue, set head and tail to NULL,
    //       size to 0, maxSize to maxSize, and return the pointer
    
    //malloc a pq
    PageQueue* pq=malloc(sizeof(PageQueue));
    //set size to 0
    pq->size=0;
    //set head and tail to null
    pq->head=NULL;
    pq->tail=NULL;
    //set maxsize
    pq->maxSize = maxSize;

    //return the pointer
    return pq;
}

/**
 * @brief Access a page in the queue (simulates a memory reference)
 */
long pqAccess(PageQueue *pq, unsigned long pageNum) {
    // TODO: Search the queue for pageNum (suggest searching tail->head
    //       so you naturally count depth from the MRU end).

    PqNode* currNode = pq->tail; //start search from tail 
    int d = 0;
    int miss = 1; //var indicating page not found (1 if true)

    //testing
    //printf("%ld",currNode->pageNum);

    while (currNode != NULL ){

        //current segfault
            if (currNode->pageNum == pageNum){
                miss = 0;
                break;
            }
            else{
                miss = 1;
                currNode = currNode->prev;
                d++;
            }
        
    }   

    // MISS path (page not found):
    //   - Allocate a new node for pageNum and insert it at the tail.
    //   - If size now exceeds maxSize, evict the head node (free it).
    //   - Return -1.
    if (miss == 1){
        
        //Allocate new node for pageNum at the tail
            //create the new node (malloc)
            PqNode* newNode = malloc(sizeof(PqNode));

            if (pq->tail !=NULL){
                //point new nodes previous to current tail
                newNode->prev=pq->tail;
                //point the current tails next at the new node
                pq->tail->next=newNode;

                //point the lists tail to the new node
                pq->tail=newNode;
                //make the new nodes next NULL (because its the end)    
                newNode->next=NULL;
                //add data to new node
                newNode->pageNum=pageNum;
            }
            else{
                //point the lists tail to the new node
                pq->tail=newNode;
                //point the list head at the new node (because it is the only node in the list rn)
                pq->head=newNode;
                //make the new nodes nexts NULL (because its the only node)    
                newNode->next=NULL;
                newNode->prev=NULL;
                //add data to new node
                newNode->pageNum=pageNum;
            }

        
        //If position/size exceeds maxSize
        if (d>pq->maxSize){
            //evict head node
                //create temp node ptr that points to current list head
                PqNode* tmpNode = pq->head;
                //set list head equal to current heads next
                pq->head=pq->head->next;
                //set new heads prev to NULL
                pq->head->prev=NULL;
                //free the old heads node using the temp ptr
                free(tmpNode);
                }
        
        return -1;
    }
    // HIT path (page found at depth d):
    //   - Remove the node from its current position and re-insert
    //     it at the tail (most recently used).
    //   - Return d.
    else{
        
        if(currNode != pq->tail){

            //create temp node ptr that points to current node
            PqNode* tmpNode = currNode;

            //point the node in front of curr's prev to the node behind curr
            
            //SEGFAULT
            currNode->next->prev = currNode->prev;
            
            //point the node behind curr's next to the node in front of curr
            currNode->prev->next = currNode->next;
            //now the list should skip over where the hit node (curr) used to be
            
            //set the new tail to the current node
            pq->tail->next = currNode;

            currNode->prev = pq->tail;

            pq->tail = currNode;

            pq->tail->next = NULL;

            //delete the old nodes position
            free(tmpNode);
        }

        //return d
        return d;
    }

}

/**
 * @brief Free all nodes in the queue and reset it to empty
 */
void pqFree(PageQueue *pq) {
    // TODO: Walk from head to tail, free each node, then free
    //       the PageQueue struct itself.


}

/**
 * @brief Print queue contents to stderr for debugging
 */
void pqPrint(PageQueue *pq) {
    // TODO (optional): Print each page number from head to tail,
    //                  marking which is head and which is tail.
    //                  Useful for desk-checking small traces.
    PqNode* currentNode = pq->head;
    printf("HEAD:\n");
    while (currentNode != NULL){
        //print the information/data at the current node pointer
        printf("%ld\n",currentNode->pageNum);
        //direct to the next node
        currentNode=currentNode->next;
    }
    printf("TAIL^\n");
}
