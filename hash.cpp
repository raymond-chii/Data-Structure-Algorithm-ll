#include "hash.h"

hashTable::hashTable(int size){
    capacity = getPrime(size);
    filled = 0;
    data.resize(capacity);
}

unsigned int hashTable::getPrime(int size){
    static const std::array<unsigned int, 12> primes = {1949, 7877, 11717, 52631, 220579, 
        500069, 700319, 800647, 1000003, 3000073, 6002131, 9003487};
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
    unsigned int start = pos;

    while (data[pos].isOccupied)
    {
        if (data[pos].key == key){
            return pos;
        }
        
        pos = (pos + 1) % capacity;

        if (pos == start){
            break;
        }
    }
    
    return -1;

}

int hashTable::insert(const std::string &key, void *pv) {
    if (filled > capacity * 0.5) {  // Check rehash at 50% capacity
        if (!rehash()) {
            return 2;  // Rehash failed
        }
    }

    int pos = findPos(key);
    if (pos != -1 && data[pos].isOccupied && data[pos].key == key) {
        return 1;  // Key already exists
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

    std::vector<hashItem> old_data = std::move(data); // Move old data to temporary vector
    
    // Reset the hash table
    data.clear();
    filled = 0;
    capacity = new_prime;
    data.resize(capacity);

    // Reinsert all occupied items from old data
    for (const hashItem &item : old_data) {
        if (item.isOccupied) {
            int result = insert(item.key, item.pv);
            if (result != 0) {
                // If insertion fails, rollback to old state
                data = std::move(old_data);
                return false;
            }
        }
    }

    // Check if rehash was successful
    if (filled < (capacity * 0.6)) {
        return true;
    } else {
        return false;
    }
}
