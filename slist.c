/*
 * SList, Simple linked list with the same API as GLib lists
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "slist.h"

SList *slist_alloc(void) {
    SList *result = NULL;
    result = calloc(1, sizeof(*result));
    return result;
}

void slist_free_1(SList *self) {
    if(self != NULL) {
        free(self);
    }
}

void slist_free(SList *self) {
    SList *iter = self;
    SList *next = NULL;

    while(iter != NULL) {
        next = slist_next(iter);
        free(iter);
        iter = next;
    }
}

void slist_free_full(SList *self, DestroyDataFunc(free_func)) {
    SList *iter = self;
    SList *next = NULL;

    while(iter != NULL) {
        next = slist_next(iter);
        free_func(iter->data);
        free(iter);
        iter = next;
    }
}

SList *slist_append(SList *self, void *data) {
    SList *result = NULL;
    SList *toAdd = NULL;

    toAdd = slist_alloc();
    result = self;
    if(toAdd != NULL) {
        toAdd->data = data;
        toAdd->next = NULL;

        /* Check case of empy list and non-empy list */
        if(self == NULL) {
            result = toAdd;
        } else {
            SList *iter = self;
            result = self;
            while(slist_next(iter) != NULL) {
                iter = slist_next(iter);
            }
            /* Got on iter the last element*/
            iter->next = toAdd;
        }
    }

    return result;
}

SList *slist_prepend(SList *self, void *data) {
    SList *result = self;
    SList *toAdd = NULL;

    toAdd = slist_alloc();
    result = self;
    if(toAdd != NULL) {
        toAdd->data = data;
        toAdd->next = self;
        result = toAdd;
    }
    return result;
}

SList *slist_insert(SList *self, void *data, int position) {
    SList *result = NULL;

    /* Precondition */
    assert(position >= 0);
    assert(position <= slist_length(self));

    if(position == 0) {
        result = slist_prepend(self, data);
    } else {
        SList *toAdd;
        result = self;
        toAdd = slist_alloc();
        if(toAdd != NULL) {
            int currPos = 0, previous = position - 1;
            SList *next = self;
            toAdd->data = data;
            assert(position >= 0);
            while(currPos != previous && (next != NULL)) {
                next = slist_next(next);
                currPos++;
            }
            /* Check why did the while ended */
            if(currPos == previous && next != NULL) {
                toAdd->next = slist_next(next);
                next->next = toAdd;
            } else {
                /* The index was out of range */
                slist_free_1(toAdd);
                /* TODO: Check if this is right */
                assert(0);
            }
        }
    }

    return result;
}

SList *slist_insert_sorted(SList *self, void *data, CompareFunc(func)) {

    /* TODO: Complete*/
    return NULL;
}

SList *slist_insert_sorted_with_data(SList *self, void *data,
                                     CompareDataFunc(func), void *user_data) {

    /* TODO: Complete*/
    return NULL;
}

SList *slist_insert_before(SList *self, SList *sibling, void *data) {
    /* TODO: Complete*/
    return NULL;
}

SList *slist_concat(SList *self, SList *concat) {
    SList *result = self;

    if(self == NULL) {
        result = concat;
    } else {
        SList *last1 = NULL;
        last1 = slist_last(self);
        last1->next = concat;
    }
    return result;
}

/*
-----------------------
*/

SList *slist_reverse(const SList *self) {
    SList *result = NULL;
    SList *iter = self;

    while(iter != NULL) {
        slist_prepend(result, iter->data);
        iter = slist_next(iter);
    }

    /* Postcondition */
    /* TODO: Check if this is true */
    /* assert(slist_length(self) == slist_length(result)); */

    return result;
}

SList *slist_copy(SList *self) {
    SList *result = NULL;

    if(self != NULL) {
        /* Actually create a new list */
        result = slist_alloc();

        if(result != NULL) {
            SList *iter1 = NULL, *iter2 = NULL;

            result->data = self->data;
            iter1 = result;
            iter2 = slist_next(self);
            while(iter2 != NULL) {
                SList *cpy = NULL;

                cpy = slist_alloc();
                if(cpy == NULL) {
                    /* Out of memmory */
                    /* TODO: Is it right? */
                    /* Kaboom! */
                    assert(0);
                }

                iter1->next = cpy;
                iter1 = slist_next(iter1);
                iter2 = slist_next(iter2);
            }
            iter1->next = NULL;
        } else {
            /* KABOOM! */
            assert(0);
        }
    }
    return result;
}

SList *slist_nth(SList *self, int n) {
    SList *result;
    int currPos = 0;

    result = self;
    while((currPos != n) && (slist_next(result) != NULL)) {
        result = slist_next(result);
        currPos++;
    }

    return result;
}

SList *slist_find(SList *self, const void *data) {
    SList *result;

    result = self;
    while(result != NULL && (result->data != data)) {
        result = slist_next(result);
    }
    return result;
}

SList *slist_find_custom(SList *self, const void *data, CompareFunc(func)) {
    SList *result = NULL;

    while(result != NULL && func(result->data, data) != 0) {
        result = slist_next(result);
    }
    return result;
}

int slist_position(SList *self, SList *llink) {
    SList *iter;
    int result = 0;

    iter = self;
    while((iter != llink) && (iter != NULL)) {
        result++;
    }

    if(iter != llink) {
        result = -1;
    }
    return result;
}

int slist_index(SList *self, const void *data) {
    int result = 0;
    SList *iter = self;

    while((iter != NULL) && (iter->data != data)) {
        result++;
    }
    if(iter == NULL) {
        result = -1;
    }

    return result;
}

SList *slist_last(SList *self) {
    SList *result = self;

    /* Check case of empy list and non-empy list */
    if(self != NULL) {
        SList *iter = self;
        while(slist_next(iter) != NULL) {
            iter = slist_next(iter);
        }
        /* Got on iter the last element*/
        result = iter;
    }

    return result;
}

int slist_length(SList *self) {
    SList *iter = self;
    int result = 0;

    while(iter != NULL) {
        result++;
        iter = slist_next(iter);
    }

    return result;
}

void slist_foreach(SList *self, UserFunc(func), void *user_data) {
    SList *iter = self;

    while(iter != NULL) {
        func(iter->data, user_data);
        iter = slist_next(iter);
    }
}

SList *slist_sort(SList *self, CompareFunc(compare_func)) {
    /*TODO: Implement a sorting algorithm*/
    return NULL;
}

SList *slist_sort_with_data(SList *self, CompareDataFunc(compare_func),
                            void *user_data) {
    /*TODO: Implement a sorting algorithm*/
    return NULL;
}

void *slist_nth_data(SList *self, int n) {
    SList *link = NULL;
    void *result = NULL;

    /* Precondition */
    assert(n >= 0);
    assert(n <= slist_length(self));

    link = slist_nth(self, n);

    if(link == NULL) {
        /* KABOOM! */
        assert(0);
    } else {
        result = link->data;
    }
    return result;
}

SList *slist_remove(SList *self, const void *data) {
    SList *current = NULL, *previous = NULL, *head = NULL;

    current = self;
    previous = current;

    while(current != NULL && (current->data != data)) {
        previous = current;
        current = slist_next(current);
    }
    if (current == NULL) {
        /* Data is not in the list */
        head = self;
    }
    else {
        if (previous == current) {
            /* Data is on the head of the list */
            head = current->next;
        }
        else {
            previous->next = current->next;
            head = self;
        }
        slist_free_1(current);
    }

    return head;
}
