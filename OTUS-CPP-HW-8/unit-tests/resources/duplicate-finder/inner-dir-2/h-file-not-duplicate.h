#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main () {
    // Create a text file
    ofstream MyWriteFile("filename.txt");

    // Write to the file
    MyWriteFile << "Files can be tricky, but it is fun enough!";

    // Close the file
    MyWriteFile.close();

    // Create a text string, which is used to output the text file
    string myText;
}
#include <iostream>
using namespace std;

class MyClass {
public:    // Public access specifier
    int x;   // Public attribute
private:   // Private access specifier
    int y;   // Private attribute
};

int main() {
    MyClass myObj;
    myObj.x = 25;  // Allowed (x is public)
    myObj.y = 50;  // Not allowed (y is private)
    return 0;
}
