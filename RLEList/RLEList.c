#include "RLEList.h"

/* declaring a node in list */
typedef struct node_t* Node;

/* node implementation */
struct node_t {
    char w;
    int returns;
    Node* next;
    Node* prev;
};

struct RLEList_t{
    int size;
    Node* first;
    Node* last;
};

//implement the functions here

RLEList RLEListCreate() {
    //generate new empty list
    RLEList newList = (RLEList)malloc(sizeof(*newList));
    if(!newList){
        return NULL;
    }
    newList->first = NULL;
    newList->last = NULL;
    return newList;
}

void RLEListDestroy(RLEList list){
    if(!list){
        return;
    }
    Node* first = list->first;
    Node* last = list->last;
    while(first != last){
        Node* tmp = (*first)->next;
        (*first)->next = NULL;
        free(*first);
        first = tmp;
    }
    free(*first);
    free(list);
}

RLEListResult RLEListAppend(RLEList list, char value){
    if(!list || !value){
        return RLE_LIST_NULL_ARGUMENT;
    }
    Node* last = list->last;
    if((*last)->w == value){
        (*last)->returns++;
        list->size++;
        return RLE_LIST_SUCCESS;
    }
    Node* newLast = (Node*)malloc(sizeof(*newLast));
    if(!newLast){
        return RLE_LIST_OUT_OF_MEMORY;
    }
    (*newLast)->w = value;
    (*newLast)->returns = 1;
    list->size++;
    (*newLast)->next = NULL;
    (*last)->next = newLast;
    (*newLast)->prev = last;
    return RLE_LIST_SUCCESS;

}


int RLEListSize(RLEList list){
    if(!list){
        return -1;
    }
    return list->size;
}


RLEListResult RLEListRemove(RLEList list, int index){
    if(!list){
        return RLE_LIST_NULL_ARGUMENT;
    }
    if(index < 0 || index > list->size){
        return RLE_LIST_INDEX_OUT_OF_BOUNDS;
    }
    int range_bottom = 0;
    int range_top = 0;
    Node* current = list->first;
    while(current){
        range_top = range_bottom + (*current)->returns;

        if(index < range_top && index < range_top){
            (*current)->returns--;
            return RLE_LIST_SUCCESS;
        } else if(index == range_top){
            (*(*current)->next)->returns--;
            return RLE_LIST_SUCCESS;
        } else if (index == range_bottom){
            (*(*current)->next)->returns--;
            return RLE_LIST_SUCCESS;
        }
        current = (*current)->next;
        range_bottom += (*current)->returns;
    }

    return RLE_LIST_ERROR;
}
