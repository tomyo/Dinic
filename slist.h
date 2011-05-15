#ifndef SLIST_H
#define SLIST_H

/*
Grupo x (?) Discreta 2011, como no nos dejan usar la GLib, he aqui el reinvento
del automovil, siempre con la misma interfaz de usuario (ie. pedales volante,
etc) pero implementado de manera distinta.
*/

#include <stdbool.h>

typedef struct _SList SList;
#define CompareFunc(x) int (*x) (const void *, const void *)
#define CompareDataFunc(x) CompareFunc(x)
#define DestroyDataFunc(x) void (*x) (void *)
#define Func(x) void (*x) (void *)

struct _SList
{
    void* data;
    SList *next;
};


/* Glib GSList interface */

/*
Allocates space for one SList element. It is called by the slist_append(),
slist_prepend(), slist_insert() and slist_insert_sorted()
functions and so is rarely used on its own.
*/
SList* slist_alloc(void);

/*
Frees one SList element. It is usually used after slist_remove_link().
*/
void slist_free_1(SList *list);

/*
Frees all of the memory used by a SList.
*/
void slist_free(SList *list);

/*
Convenience method, which frees all the memory used by a SList, and calls
the specified destroy function on every element's data.
*/
void slist_free_full(SList *list, DestroyDataFunc(free_func));

/*
Adds a new element on to the end of the list.
NOTE 1- The return value may be the new start of the list, so
make sure you store the new value.
NOTE 2- O(slist_append) = O(n)
*/
SList* slist_append(SList *list, void* data);

/*
Adds a new element on to the start of the list.
NOTE 1- The return value may be the new start of the list, so
make sure you store the new value.
NOTE 2- O(slist_prepend) = O(1)
*/
SList* slist_prepend(SList *list, void* data);

/*
Inserts a new element into the list at the given position.
If the index is out of range, an assertion will raise.
*/
SList* slist_insert(SList *list, void* data, int position);

/*
Inserts a new element into the list, using the given comparison function to
determine its position.
*/
SList* slist_insert_sorted (SList *list, void* data, CompareFunc(func));

/*
Inserts a new element into the list, using the given comparison function to
determine its position.
*/
SList* slist_insert_sorted_with_data (SList *list, void* data,
CompareDataFunc(func), void* user_data);

/*
Inserts a node before sibling containing data.
*/
SList* slist_insert_before (SList *slist, SList *sibling, void* data);

/*
Adds the second SList onto the end of the first SList. Note that the
elements of the second SList are not copied. They are used directly.
*/
SList* slist_concat (SList *list1, SList *list2);

/*
Removes an element from a SList. If two elements contain the same data, only
the first is removed. If none of the elements contain the data, the SList is
unchanged.
*/
SList* slist_remove (SList *list, const void* data);

/*
Removes all list nodes with data equal to data. Returns the new head of
the list. Contrast with slist_remove() which removes only the first node
matching the given data.
*/
SList* slist_remove_all (SList *list, const void* data);

/*
Removes an element from a SList, without freeing the element. The removed
element's next link is set to NULL, so that it becomes a self-contained list
with one element.
*/
SList* slist_remove_link (SList *list, SList *link_);

/*
Removes the node link_ from the list and frees it. Compare this to
slist_remove_link() which removes the node without freeing it.
*/
SList* slist_delete_link (SList *list, SList *link_);

/*
Reverses a SList.
Returns a new list.
NOTE 1- O(slist_reverse) = O(n)
*/
SList* slist_reverse (SList *list);

/*
Copies a SList.
NOTE 1- Note that this is a "shallow" copy. If the list elements consist of
pointers to data, the pointers are copied but the actual data isn't.
*/
SList* slist_copy (SList *list);

/*
In python
result = list[n:]
It doesn't resturn a NEW list.
If the index is out of range it returns an empty list
*/
SList* slist_nth (SList *list, int n);

/*
Finds the element in a SList which contains the given data.
*/
SList* slist_find (SList *list, const void* data);

/*
Finds an element in a SList, using a supplied function to find the desired
element. It iterates over the list, calling the given function which should
return 0 when the desired element is found. The function takes two
const void* arguments, the SList element's data as the first argument
and the given user data.
*/
SList* slist_find_custom (SList *list, const void* data, CompareFunc(func));

/*
Gets the position of the given element in the SList (starting from 0).
*/
int slist_position (SList *list, SList *llink);

/*
Gets the position of the element containing the given data (starting from 0).
*/
int slist_index (SList *list, const void* data);

/*
Gets the last element in a SList.
Note 1 - This function iterates over the whole list.
*/
SList* slist_last (SList *list);

/*
Gets the number of elements in a SList.
*/
int slist_length (SList *list);

/*
Calls a function for each element of a SList.
*/
void slist_foreach (SList *list, Func(func), void* user_data);

/*
Sorts a SList using the given comparison function.
compare_func :
the comparison function used to sort the SList. This function is passed the
data from 2 elements of the SList and should return 0 if they are equal, a
negative value if the first element comes before the second, or a positive
value if the first element comes after the second.
*/
SList* slist_sort (SList *list, CompareFunc(compare_func));

/*
Like slist_sort(), but the sort function accepts a user data argument.
*/
SList* slist_sort_with_data (SList *list, CompareDataFunc(compare_func),
void* user_data);

/*
Gets the data of the element at the given position.
*/
void* slist_nth_data (SList *list, int n);

/*
 convenience macro to get the next element in a SList.
*/
#define slist_next(slist) ((slist) ? (((SList *)(slist))->next) : NULL)
#endif
