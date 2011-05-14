/*
SList, emulando las SList de la Glib
*/

#include <stdlib.h>
#include <assert.h>
#include "slist.h"

SList* slist_alloc(void) {
    SList *result = NULL;
    result = calloc(1, sizeof(SList));
    return result;
}

void slist_free_1(SList *list) {
    if(list != NULL) {
        free(list);
    }
}

void slist_free(SList *list) {
    SList *iter, *next;

    while(iter != NULL) {
        next = slist_next(iter);
        free(iter);
        iter = next;
    }
}

void slist_free_full(SList *list, DestroyDataFunc(free_func)) {
    SList *iter, *next;

    while(iter != NULL) {
        next = slist_next(iter);
        free_func(iter->data);
        free(iter);
        iter = next;
    }
}

SList* slist_append(SList *list, void* data) {
    SList *result, *toAdd;

    toAdd = slist_alloc();
    result = list;
    if(toAdd != NULL) {
        toAdd->data = data;
        toAdd->next = NULL;

        /* Check case of empy list and non-empy list */
        if(list == NULL) {
            result = toAdd;
        } else {
            SList *iter = list;
            result = list;
            while(slist_next(iter) != NULL) {
                iter = slist_next(iter);
            }
            /* Got on iter the last element*/
            iter->next = toAdd;
        }
    }
    return result;
}

SList* slist_prepend(SList *list, void* data) {
    SList *result, *toAdd;

    toAdd = slist_alloc();
    result = list;
    if(toAdd != NULL) {
        toAdd->data = data;
        toAdd->next = list;
        result = toAdd;
    }
    return result;
}

SList* slist_insert(SList *list, void* data, int position) {
    SList *result;

    if(position == 0) {
        result = slist_prepend(list, data);
    } else {
        SList *toAdd;
        result = list;
        toAdd = slist_alloc();
        if(toAdd != NULL) {
            int currPos = 0, previous = position - 1;
            SList *next = list;
            toAdd->data = data;
            assert(position >= 0);
            /* If currPos != previous the assignation next = ... would not
            take effect */
            while(currPos != previous && (next = slist_next(next)) != NULL) {
                currPos++;
            }
            /* Why I ended the while statement? */
            if(currPos == previous) {
                toAdd->next = slist_next(slist_next(next));
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

SList* slist_insert_sorted (SList *list, void* data, CompareFunc(func)) {
    /* TODO: Complete*/
    return NULL;
}

SList* slist_insert_sorted_with_data (SList *list, void* data,
CompareDataFunc(func), void* user_data) {
    /* TODO: Complete*/
    return NULL;
}

SList* slist_insert_before (SList *slist, SList *sibling, void* data) {
    /* TODO: Complete*/
    return NULL;
}

SList* slist_concat (SList *list1, SList *list2) {
    SList *result;

    result = list1;
    if(list1 == NULL) {
        result = list2;
    } else {
        SList *last1;
        last1 = slist_last(list1);
        last1->next = list2;
    }
    return result;
}

/*
-----------------------
*/

SList* slist_reverse (SList *list) {
    SList *result, *iter;

    result = NULL;
    iter = list;
    while(iter != NULL) {
        slist_prepend(result, iter->data);
        iter = slist_next(iter);
    }
    return result;
}

SList* slist_copy (SList *list) {
    SList *result;

    result = NULL;
    if(list != NULL) {
        /* I'm actually gonna create a new list*/
        result = slist_alloc();
        if(result != NULL) {
            SList *iter1, *iter2;
            result->data = list->data;
            iter1 = result;
            iter2 = slist_next(list);
            while(iter2 != NULL) {
                SList *cpy;
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

SList* slist_nth(SList *list, int n) {
    SList *result;
    int currPos = 0;

    result = list;
    while(currPos != n && ((result = slist_next(result)) != NULL) && currPos++);

    return result;
}

SList* slist_find (SList *list, const void* data) {
    SList *result;

    while(result != NULL && result->data != data) {
        result = slist_next(result);
    }
    return result;
}

SList* slist_find_custom (SList *list, const void* data, CompareFunc(func)) {
    SList *result;

    while(result != NULL && func(result->data, data) != 0) {
        result = slist_next(result);
    }
    return result;
}

int slist_position (SList *list, SList *llink) {
    SList *iter;
    int result = 0;

    iter = list;
    while(iter != llink && iter != NULL && result++);
    if(iter != llink) {
        result = -1;
    }
    return result;
}

int slist_index (SList *list, const void* data) {
    SList *iter;
    int result = 0;

    iter = list;
    while(iter->data != data && iter != NULL && result++);
    if(iter == NULL) {
        result = -1;
    }
    return result;
}

SList* slist_last (SList *list) {
    SList *result;

    result = list;
    /* Check case of empy list and non-empy list */
    if(list != NULL) {
        SList *iter = list;
        while(slist_next(iter) != NULL) {
            iter = slist_next(iter);
        }
        /* Got on iter the last element*/
        result = iter;
    }
    return result;
}

int slist_length (SList *list) {
    SList *iter;
    int result = 0;

    iter = list;
    while(iter != NULL && result++ && (iter = slist_next(iter)));
    return result;
}

void slist_foreach (SList *list, Func(func), void* user_data) {
    SList *iter;

    iter = list;
    while(iter != NULL) {
        func(iter->data);
        iter = slist_next(iter);
    }
}

SList* slist_sort (SList *list, CompareFunc(compare_func)) {
    /*TODO: Implement a sorting algorithm*/
    return NULL;
}

SList* slist_sort_with_data (SList *list, CompareDataFunc(compare_func),
void* user_data) {
    /*TODO: Implement a sorting algorithm*/
    return NULL;
}

void* slist_nth_data (SList *list, int n) {
    SList *link;
    void* result;

    result = NULL;
    link = slist_nth(list, n);
    if(link==NULL) {
        /* KABOOM! */
        assert(0);
    } else {
        result = link->data;
    }
    return result;
}
