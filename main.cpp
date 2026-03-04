#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <set>
#include <unordered_map>
#include <stdexcept>

using namespace std;

class Cache
{
protected:
    unsigned cacheSize;
    unsigned totalMisses = 0;

    Cache(unsigned size) : cacheSize{size} {
        if (cacheSize <= 0)
            throw runtime_error("Invalid cache size");
    }

public:

    // Simulates attempting to read a value from the cache
    virtual void simulateRead(int value) = 0;

    // Returns the total number of misses since the instance was created
    unsigned getTotalMisses() { return totalMisses; }
};

class FIFOCache : public Cache
{
    map<int, unsigned> cache; // Maps VALUE -> FIRST_READ_TIME
    unsigned currentTime = 0;

public:

    FIFOCache(unsigned size) : Cache{size}, cache{} {}

    virtual void simulateRead(int value) override {

        // Track a miss if cache doesn't contain value
        if (cache.find(value) == cache.end()) {
            ++totalMisses;
            cache.emplace(value, currentTime);
        }
        ++currentTime;

        // Evict oldest member if necessary
        if (cache.size() > cacheSize) {
            auto it = cache.begin();
            auto oldest = it;
            while (it != cache.end())
            {
                if (it->second < oldest->second)
                    oldest = it;
                ++it;
            }
            cache.erase(oldest);
        }
    }
};

class LRUCache : public Cache
{
    map<int, unsigned> cache; // Maps VALUE -> RECENT_READ_TIME
    unsigned currentTime = 0;

public:

    LRUCache(unsigned size) : Cache{size}, cache{} {}

    virtual void simulateRead(int value) override {

        // Track a miss if cache doesn't contain value
        if (cache.find(value) == cache.end()) {
            ++totalMisses;
            cache.emplace(value, currentTime);
        }
        cache[value] = currentTime; // Flag value as recently used
        ++currentTime;

        // Evict least-recent member if necessary
        if (cache.size() > cacheSize) {
            auto it = cache.begin();
            auto oldest = it;
            while (it != cache.end())
            {
                if (it->second < oldest->second)
                    oldest = it;
                ++it;
            }
            cache.erase(oldest);
        }
    }
};

class OPTFFCache
{
    unsigned cacheSize;
    unsigned totalMisses = 0;

    // value -> next use index (based on most recent request of that value)
    unordered_map<int, int> cache;

    // ordered by (nextUseIndex, value); farthest in future is at the end
    set<pair<int, int>> order;

public:

    OPTFFCache(unsigned size) : cacheSize{size}, cache{}, order{} {
        if (cacheSize <= 0) {
            throw runtime_error("Invalid cache size");
        }
    }

    // For request at position i:
    //   value = requests[i]
    //   nextUse = index of next occurrence of value after i, or INF if none
    void simulateRead(int value, int nextUse) {

        // HIT: update its next-use record
        auto it = cache.find(value);
        if (it != cache.end()) {
            order.erase({it->second, value});
            it->second = nextUse;
            order.insert({nextUse, value});
            return;
        }

        // MISS
        ++totalMisses;

        // Evict farthest in future if full
        if (cache.size() >= cacheSize) {
            auto victim = prev(order.end());
            int victimValue = victim->second;
            order.erase(victim);
            cache.erase(victimValue);
        }

        // Insert new value
        cache.emplace(value, nextUse);
        order.insert({nextUse, value});
    }

    unsigned getTotalMisses() { return totalMisses; }
};

void readIntFromFile(std::ifstream& file, int& var) {
    if (file.eof())
        throw runtime_error("EOF reached");
    file >> var;
}

int main(int argc, char* argv[]) {
    try {

        // Filepath should be argument 1
        if (argc != 2)
            throw runtime_error("Filepath must be only argument.");

        // Open file
        ifstream file{argv[1]};
        if (!file.is_open())
            throw runtime_error("Cannot open file.");

        // Read input (assuming valid file)
        int k, m, buffer;
        readIntFromFile(file, k);
        readIntFromFile(file, m);

        // Simulate cache
        FIFOCache FIFO(k);
        LRUCache LRU(k);
        for (int i = 0; i < m; ++i) {
            readIntFromFile(file, buffer);
            FIFO.simulateRead(buffer);
            LRU.simulateRead(buffer);
        }
        cout << "Total misses (FIFO): " << FIFO.getTotalMisses() << endl;
        cout << "Total misses (LRU): " << LRU.getTotalMisses() << endl;
    }
    catch(const runtime_error& e) {
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}