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

static bool checkIndexInRange(int index,int range_top,int range_bottom){
    if(index <= range_top && index >= range_bottom){
            return true;
    }
    return false;
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

        if(checkIndexInRange(index, range_top,range_bottom)){
            (*current)->returns--;
            return RLE_LIST_SUCCESS;
        }
        current = (*current)->next;
        range_bottom += (*current)->returns;
    }

    return RLE_LIST_ERROR;
}


char RLEListGet(RLEList list, int index, RLEListResult *result){
    if(!list ){
        (*result) = RLE_LIST_NULL_ARGUMENT;
        return 0;
    }
    if(index < 0 || index > list->size){
        (*result) = RLE_LIST_INDEX_OUT_OF_BOUNDS;
        return 0;
    }
    int range_bottom = 0;
    int range_top = 0;
    Node* current = list->first;
    while(current){
        range_top = range_bottom + (*current)->returns;

        if(checkIndexInRange(index, range_top,range_bottom)){
            (*result) = RLE_LIST_SUCCESS;
            return (*current)->w;
        }
        current = (*current)->next;
        range_bottom += (*current)->returns;
    }
    return 0;
}


char* RLEListExportToString(RLEList list, RLEListResult* result){
    if(!list ){
        (*result) = RLE_LIST_NULL_ARGUMENT;
        return NULL;
    }
    int size = list->size;
    char* res = (char*)malloc(sizeof(*res)*size);
    if(!res){
        (*result) = RLE_LIST_OUT_OF_MEMORY;
        return NULL;
    }
    int index=0;
    Node* current = list->first;
    while (current)
    {
        for(int j = 0; j < (*current)->returns; j++){
            res[index] = (*current)->w;
            index++;
        }
        current = (*current)->next;
    }
    (*result) = RLE_LIST_SUCCESS;
    return res;
}


RLEListResult RLEListMap(RLEList list, MapFunction map_function){
    if(!list || !map_function){
        return RLE_LIST_NULL_ARGUMENT;
    }
    Node* current = list->first;
    while(current){
        (*current)->w = map_function((*current)->w);
        current = (*current)->next;
    }
    return RLE_LIST_SUCCESS;
}
