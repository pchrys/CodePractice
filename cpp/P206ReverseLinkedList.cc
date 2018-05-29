#include <stdlib.h>
#include <stdio.h>
#include <string.h>


// /**
//  * Definition for singly-linked list.
//  * struct ListNode {
//  *     int val;
//  *     struct ListNode *next;
//  * };
//  */
// struct ListNode* reverseList(struct ListNode* head) {

// }


// Reverse a singly linked list.

// Example:

// Input: 1->2->3->4->5->NULL
// Output: 5->4->3->2->1->NULL

// Follow up:

// A linked list can be reversed either iteratively or recursively. Could you implement both?


/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */

struct ListNode
{
    int val;
    struct ListNode* next;
};

struct ListNode* reverseList(struct ListNode* head)
{
    if (head == NULL)
    {
        return NULL;
    }

    struct ListNode dummy;
    dummy.next = head;

    struct ListNode *p, *q, *r;
    p = &dummy;
    q = p->next;
    r = NULL;

    while (q)
    {
        r = q->next;
        q->next = p;
        p = q;
        q = r;
    }

    head->next = NULL;

    return p;
}

struct ListNode* reverseListRecursive(struct ListNode* head)
{
    if (head == NULL)
    {
        return NULL;
    }

    struct ListNode* p = head->next;
    struct ListNode* q = NULL;

    head->next = NULL;
    q = reverseListRecursive(p);

    if (p)
    {
        p->next = head;
    }

    if (q == NULL)
    {
        q = head;
    }

    return q;
}

struct ListNode* Add(struct ListNode* head, int v)
{

    struct ListNode dummy;
    dummy.next = head;

    struct ListNode *p, *q;
    p = &dummy;
    q = p->next;
    while(q){
        p= q;
        q = q->next;
    }

    p->next  = (ListNode*)malloc(sizeof(ListNode));
    p->next->val = v;
    p->next->next = NULL;

    return dummy.next;
}

struct ListNode* Remove(struct ListNode* head, int v)
{
    struct ListNode dummy;
    dummy.next = head;

    struct ListNode *p, *q;
    p = &dummy;
    q = p->next;
    while(q && q->val != v){
        p= q;
        q = q->next;
    }

    if(q){
        p->next = q->next;
        free(q);
    }

    return dummy.next;
}

void printList(struct ListNode* head){

    struct ListNode dummy;
    dummy.next = head;
    struct ListNode *p, *q;
    p = &dummy;
    q = p->next;
    while(q){
        printf("%d-->", q->val);
        p =q ;
        q = q->next;
    }

    printf("NULL \n");
}


int main()
{
    ListNode * head = NULL;

    head = Add(head, 1);

    head = Add(head, 2);
    head = Add(head, 3);
    head = Add(head, 4);
    head = Add(head, 5);

    printList(head);

    //head = Remove(head, 3);
    //head = Remove(head, 1);


    head = reverseListRecursive(head);
    //head = reverseList(head);
    printList(head);

    return 0;

}
