#include <iostream>
#include <fstream>

using namespace std;

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
        
        cout << "Success! (" << k << ", " << m << ")" << endl;
    }
    catch(const runtime_error& e) {
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}