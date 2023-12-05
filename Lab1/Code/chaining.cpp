#include "readfile.h" 

struct cDataItem {
   int data;     
   int key;
   cDataItem * nextDataItem;
};


//Each bucket contains number of records
struct cBucket {
	cDataItem * ptrDataItem;
    int count;
}; 

int ChainningHashCode(int key ) {
    return key % MBUCKETS;
}

int ChainningInsertItem(int fd , DataItem item){

}

int ChainningSearchItem(int fd , DataItem * item , int * count){

}

int ChainningDeleteItem(int key){

}
