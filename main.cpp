#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

class FileStorage {
private:
    std::string getFilename(const std::string& index) {
        // Create a safe filename from the index
        std::string filename = "db_" + index + ".txt";
        // Replace any potentially problematic characters
        for (char& c : filename) {
            if (c == '/' || c == '\\' || c == ':' || c == '*' || c == '?' || 
                c == '"' || c == '<' || c == '>' || c == '|' || c == ' ') {
                c = '_';
            }
        }
        return filename;
    }
    
public:
    void insert(const std::string& index, int value) {
        std::string filename = getFilename(index);
        std::vector<int> values;
        
        // Read existing values
        std::ifstream infile(filename);
        if (infile.is_open()) {
            int val;
            while (infile >> val) {
                if (val != value) { // Avoid duplicates
                    values.push_back(val);
                }
            }
            infile.close();
        }
        
        // Add new value
        values.push_back(value);
        
        // Sort values
        std::sort(values.begin(), values.end());
        
        // Write back to file
        std::ofstream outfile(filename);
        for (int val : values) {
            outfile << val << " ";
        }
        outfile.close();
    }
    
    void deleteEntry(const std::string& index, int value) {
        std::string filename = getFilename(index);
        std::vector<int> values;
        
        // Read existing values
        std::ifstream infile(filename);
        if (infile.is_open()) {
            int val;
            while (infile >> val) {
                if (val != value) {
                    values.push_back(val);
                }
            }
            infile.close();
        }
        
        // Write back to file
        std::ofstream outfile(filename);
        for (int val : values) {
            outfile << val << " ";
        }
        outfile.close();
        
        // If file is empty, remove it
        if (values.empty()) {
            fs::remove(filename);
        }
    }
    
    std::vector<int> find(const std::string& index) {
        std::string filename = getFilename(index);
        std::vector<int> values;
        
        // Read values from file
        std::ifstream infile(filename);
        if (infile.is_open()) {
            int val;
            while (infile >> val) {
                values.push_back(val);
            }
            infile.close();
        }
        
        // Sort values (though they should already be sorted)
        std::sort(values.begin(), values.end());
        
        return values;
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    FileStorage storage;
    int n;
    std::cin >> n;
    std::cin.ignore(); // Consume newline after n
    
    for (int i = 0; i < n; i++) {
        std::string line;
        std::getline(std::cin, line);
        std::istringstream iss(line);
        
        std::string command;
        iss >> command;
        
        if (command == "insert") {
            std::string index;
            int value;
            iss >> index >> value;
            storage.insert(index, value);
        } else if (command == "delete") {
            std::string index;
            int value;
            iss >> index >> value;
            storage.deleteEntry(index, value);
        } else if (command == "find") {
            std::string index;
            iss >> index;
            std::vector<int> values = storage.find(index);
            
            if (values.empty()) {
                std::cout << "null" << std::endl;
            } else {
                for (size_t j = 0; j < values.size(); j++) {
                    if (j > 0) std::cout << " ";
                    std::cout << values[j];
                }
                std::cout << std::endl;
            }
        }
    }
    
    return 0;
}