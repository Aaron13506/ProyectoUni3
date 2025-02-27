#include "iostream"
#include "fstream"
#include "cmath"
#include "cstdlib"

using namespace std;
struct atribute {
    string name;
    int value{};
};
struct adventurer {
    string clase;
    string faction;
    string name;
    int puntuation{};
    atribute atributes[100];
    };

int countFiles(const string& directory) {
    string tempFile = "temp.txt";
    string command = "ls " + directory + "/*.in > " + tempFile;

    system(command.c_str());

    ifstream file(tempFile);
    int count = 0;
    string line;
    while (getline(file, line)) {
        count++;
    }

    file.close();
    remove(tempFile.c_str());

    return count;
}
int calculateADN (string numbers) {
 }

int main() {
     class Adventurers {
     public:
         adventurer adventurers[];

         adventurer* load(string directory) {
int adventurerQty = countFiles(directory);
             adventurer adventurers[adventurerQty];

         }

     };
 }