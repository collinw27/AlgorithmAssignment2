#include <iostream>
#include <fstream>
#include <map>

using namespace std;

class Cache
{
protected:
    unsigned cacheSize;

    Cache(unsigned size) : cacheSize{size} {
        if (cacheSize <= 0)
            throw runtime_error("Invalid cache size");
    }

public:

    // Simulates attempting to read a value from the cache
    virtual void simulateRead(int value) = 0;

    // Returns the total number of misses since the instance was created
    virtual unsigned getTotalMisses() = 0;
};

class FIFOCache : public Cache
{
    map<int, unsigned> cache; // Maps VALUE -> FIRST_READ_TIME
    unsigned currentTime = 0;
    unsigned totalMisses = 0;

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

    virtual unsigned getTotalMisses() override { return totalMisses; }
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
        for (int i = 0; i < m; ++i) {
            readIntFromFile(file, buffer);
            FIFO.simulateRead(buffer);
        }
        cout << "Total misses: " << FIFO.getTotalMisses() << endl;
    }
    catch(const runtime_error& e) {
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}