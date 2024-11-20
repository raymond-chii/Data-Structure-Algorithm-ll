#include "hash.h"

hashTable::hashTable(int size){
    capacity = getPrime(size);
    filled = 0;
    data.resize(capacity);
}

unsigned int hashTable::getPrime(int size){
    static const std::array<unsigned int, 12> primes = {1913, 7247, 11717, 52631, 220579, 
        504181, 721291, 826549, 999983, 3000073, 6002131, 9003487};
    for (unsigned int prime : primes){
        if (prime >= size){
            return prime;
        }
    }
    return primes.back();
}

int hashTable::hash(const std::string &key){
    uint32_t hashVal = 0;
    for (char c : key) {
        hashVal = 37 * hashVal + static_cast<unsigned char>(c);
    }
    return hashVal % capacity;
}

int hashTable::findPos(const std::string &key){
    unsigned int pos = hash(key);

    while (true){

        if(!data[pos].isOccupied){ 
            return -1;
        }
        else if (data[pos].key == key && !data[pos].isDeleted){ 
            return pos;
        }

        pos++;
        pos %= capacity; 
    }

}

int hashTable::insert(const std::string &key, void *pv) {
    if (filled > capacity * 0.5) {  // Check rehash at 50% capacity
        if (!rehash()) {
            return 2;  // Rehash failed
        }
    }

    int pos = findPos(key);
    if (pos != -1 && data[pos].isOccupied && !data[pos].isDeleted && data[pos].key == key) {
        return 1;
    }

    // Insert at the found position or the next available slot
    if (pos == -1) {
        pos = hash(key);
        while (data[pos].isOccupied) {
            pos = (pos + 1) % capacity;
        }
    }

    data[pos].key = key;
    data[pos].isOccupied = true;
    data[pos].isDeleted = false;
    data[pos].pv = pv;
    filled++;

    return 0;  // Success
}

bool hashTable::contains(const std::string &key){
    int pos = findPos(key);
    return (pos != -1 && data[pos].key == key);
}

bool hashTable::rehash() {
    unsigned int new_size_min = capacity * 2; // Double the current capacity
    unsigned int new_prime = getPrime(new_size_min);

    std::vector<hashItem> old_data = data;
    
    // Reset the hash table
    data.clear();
    filled = 0;
    capacity = new_prime;
    data.resize(capacity);

    for (hashItem item: old_data) {
        if (item.isOccupied && !item.isDeleted) {
            insert(item.key, item.pv);
        }
    }

    // Check if rehash was successful
    if (filled < (capacity * 0.6)) {
        return true;
    } else {
        return false;
    }
}

void *hashTable::getPointer(const std::string &key, bool *b) {
    unsigned int pos = findPos(key);
    
    if (pos == -1){
        *b = false;
        return nullptr;
    }
    else{ 
        *b = true;
        return data[pos].pv;
    }

}

int hashTable::setPointer(const std::string &key, void *pv) {
    unsigned int pos = findPos(key);
    
    if (pos == -1) {
        return 1;  // Key not found
    }
    else {
        data[pos].pv = pv;
        return 0;  // Success
    }
    
}

bool hashTable::remove(const std::string &key) {
    unsigned int pos = findPos(key);
    
    if (pos == -1) {
        return false;  // Key not found
    } 
    else {
        data[pos].isDeleted = true;
        return true;
    }

}
