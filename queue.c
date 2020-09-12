#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "harness.h"
list_ele_t *merge(list_ele_t *, list_ele_t *);
list_ele_t *mergeSortList(list_ele_t *);
/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (!q)
        return NULL;
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q)
        return;
    while (q->head) {
        list_ele_t *ptr = q->head;
        free(q->head->value);
        q->head = q->head->next;
        free(ptr);
    }
    free(q);
    q->size = 0;
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (!q)
        return false;
    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (!newh)
        return false;
    newh->value = malloc(sizeof(char) * strlen(s) + 1);
    if (!newh->value) {
        free(newh);
        return false;
    }
    strncpy(newh->value, s, strlen(s) + 1);
    newh->next = q->head;
    q->head = newh;
    if (!q->size)
        q->tail = newh;
    q->size++;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q)
        return false;
    list_ele_t *newt;
    newt = malloc(sizeof(list_ele_t));
    if (!newt)
        return false;
    newt->value = malloc(sizeof(char) * strlen(s) + 1);
    if (!newt->value) {
        free(newt);
        return false;
    }
    strncpy(newt->value, s, strlen(s) + 1);
    newt->next = NULL;
    if (!q->size) {
        q->head = newt;
    } else {
        q->tail->next = newt;
    }
    q->tail = newt;
    q->size++;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q)
        return false;
    if (!q->size)
        return false;
    if (sp && q->head->value) {
        memset(sp, '\0', bufsize);
        strncpy(sp, q->head->value, bufsize - 1);
    } else
        return false;
    list_ele_t *ptr = q->head;
    q->head = q->head->next;
    free(ptr->value);
    free(ptr);
    q->size--;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    return q ? q->size : 0;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q)
        return;
    if (q->size < 2) {
        printf("elements in queue lower than 2\n");
        return;
    }

    list_ele_t *ptr = q->head->next->next;
    q->tail->next = q->head;
    while (q->head->next != q->tail) {
        q->head->next->next = q->tail->next;
        q->tail->next = q->head->next;
        q->head->next = ptr;
        ptr = ptr->next;
    }
    q->tail = q->head;
    q->head = q->head->next;
    q->tail->next = NULL;
}


/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */



void q_sort(queue_t *q)
{
    if (!q)
        return;
    if (q->size < 3)
        return;
    q->head = mergeSortList(q->head);
    q->tail = q->head;
    while (q->tail->next)
        q->tail = q->tail->next;
}

list_ele_t *merge(list_ele_t *L1, list_ele_t *L2)
{
    // merge with recursive
    if (!L2)
        return L1;
    if (!L1)
        return L2;
    if (strcmp(L1->value, L2->value) < 0) {
        // printf("L1=%s	L2=%s\n", L1->value, L2->value);
        L1->next = merge(L1->next, L2);
        return L1;
    } else {
        L2->next = merge(L1, L2->next);
        return L2;
    }
}

list_ele_t *mergeSortList(list_ele_t *head)
{
    if (!head || !head->next)
        return head;
    // tmp pointer for original q->head
    list_ele_t *ptr1 = head->next;
    list_ele_t *ptr2 = head;
    while (ptr1 && ptr1->next) {
        ptr2 = ptr2->next;
        ptr1 = ptr1->next->next;
    }
    ptr1 = ptr2->next;
    ptr2->next = NULL;
    // sort start
    list_ele_t *L1 = mergeSortList(head);
    list_ele_t *L2 = mergeSortList(ptr1);
    return merge(L1, L2);
}
