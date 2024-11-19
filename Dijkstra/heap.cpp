#include "heap.h"

heap::heap(int capacity){
    currentPos = 1;
    this -> capacity = capacity;
    hashTable mapping(capacity*2);
    data.resize(capacity+1);
};

int heap::insert(const std::string &id, int key, void *pv){

    if (currentPos > capacity){
        return 1;
    } 
    else if (mapping.contains(id)){
        return 2;
    } 
    else {

        data[currentPos].id = id;
        data[currentPos].key = key;
        data[currentPos].pData = pv;

        mapping.insert(id, &data[currentPos]);
        percolateUp(currentPos);
        currentPos++;

        return 0; 
    }
    
};

int heap::deleteMin(std::string *pId, int *pKey, void *ppData){
    if(currentPos <= 1){
        return 1;
    }
    else{
        if(pId){
            *pId = data[1].id;
        }
        if(pKey){
            *pKey = data[1].key;
        }
        if(ppData){
            *(static_cast<void **> (ppData)) = data[1].pData;
        }

        mapping.remove(data[1].id);
        currentPos--;
        data[1] = data[currentPos];
        mapping.setPointer(data[currentPos].id, &data[1]);
        percolateDown(1);

        return 0;
    }
};

int heap::setKey(const std::string &id, int key){
    
    bool b = false;
    node *pn = static_cast<node *> (mapping.getPointer(id, &b));

    if (!b){
        return 1;
    }
    else{

        int oldKey = (*pn).key;
        (*pn).key = key;
        if(oldKey > key){
            percolateUp(getPos(pn));
        } 
        else if(oldKey < key){
            percolateDown(getPos(pn));
        }

        return 0;
    }

};

int heap::remove(const std::string &id, int *pKey, void *ppData){

    bool b = false;
    node *pn = static_cast<node *> (mapping.getPointer(id, &b));

    if(!b){
        return 1;
    }
    else{
        int deletedKey = (*pn).key;

        if(pKey){
            *pKey = (*pn).key;
        }
        if(ppData){
            *(static_cast<void **> (ppData)) = (*pn).pData;
        }

        mapping.remove(id);
        currentPos--;
        *pn = data[currentPos];
        mapping.setPointer(data[currentPos].id, pn);

        if(data[currentPos].key > deletedKey){
            percolateUp(getPos(pn));
        } 
        else if(data[currentPos].key < deletedKey){
            percolateDown(getPos(pn));
        }

        return 0;
    }
};

void heap::percolateDown(int pos) {
    node temp = data[pos];
    int size = currentPos - 1;
    
    for (int left = pos * 2; left <= size; pos = left, left = pos * 2) {
        int right = left + 1;
        int child = (left == size || data[left].key < data[right].key) ? left : right;
        
        if (temp.key <= data[child].key) break;
        
        data[pos] = data[child];
        mapping.setPointer(data[pos].id, &data[pos]);
    }
    
    data[pos] = temp;
    mapping.setPointer(data[pos].id, &data[pos]);
}

void heap::percolateUp(int currentPos) {

    node temp = data[currentPos];
    
    while (currentPos > 1 && data[currentPos / 2].key > temp.key) {
        
        data[currentPos] = data[currentPos / 2];
        mapping.setPointer(data[currentPos].id, &data[currentPos]);
        currentPos = currentPos / 2;
    }

    data[currentPos] = temp;
    mapping.setPointer(data[currentPos].id, &data[currentPos]);
}

int heap::getPos(node *pn){
    return pn - &data[0];
}
