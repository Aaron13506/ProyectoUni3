#include "iostream"
#include "fstream"
#include "cmath"
#include "cstdlib"

using namespace std;

struct atribute {
    char name[120]{};
    int value{};
};
struct adventurer {
    char clase[120]{};
    char faction[120]{};
    char name[120]{};
    int puntuation{};
    atribute atributes[120];

    void printInfo() const {
        cout << "Clase: " << clase << endl;
        cout << "Faction: " << faction << endl;
        cout << "Name: " << name << endl;
        cout << "Puntuation: " << puntuation << endl;
        cout << "Atributes: " << endl;
        for (const auto& atr : atributes) {
            if (atr.name[0] != '\0') {
                cout << "  " << atr.name << ": " << atr.value << endl;
            }
        }
    }
};

char tempFile[] = "temp.txt";

void normalizePath(char*& path) {
    for (int i = 0;true; i++) {
        if (path[i] == '/') {
            path[i] = '\\';
        }
        else if (path[i] == '\0') {
            if (path[i - 1] == ' ') {
                path[i - 1] = '\0';
                continue;
            }
            if (path[i - 1] != '\\') {
                path[i] = '\\';
                path[i + 1] = '\0';
            }
            break;
        }
    }
}
int countFiles(const char* directory) {
    char command[256];
    snprintf(command, sizeof(command), "dir /b %s*.in > %s", directory, tempFile);
    system(command);
    ifstream file(tempFile);
    int count = 0;
    char line[256];
    while (file.getline(line, sizeof(line))) {
        count++;
    }

    return count;
}
void fillSquareArray(char* lineNumbers, int** arrayN, int* j) {
    int count = 0;
    for (int i = 0; lineNumbers[i]; i++) {
        if (lineNumbers[i] != ' ') {
            lineNumbers[count++] = lineNumbers[i];
        }
    }
    lineNumbers[count] = '\0';
    for (int i = 0; lineNumbers[i] != '\0'; i++) {
        arrayN[*j][i] = lineNumbers[i] - '0';
    }
    *j += 1;
}
int sizeSquareArray(char* lineNumbers) {
        int size {0};
        for (int i = 0; lineNumbers[i] != '\0';i++){if (lineNumbers[i] != ' ') size++;};
        return size;
}
int calculateADN (int** arrNumbers, int arrSize) {
    int centralCrossSum = 0, edgeMult = 1, arrMid = static_cast<int>(floor(arrSize / 2));
    for (int i = 0; i < arrSize; i++) {
        for (int j = 0; j < arrSize; j++) {
            if (i == j || i + j == arrSize - 1 || i == arrMid || j == arrMid) {
                centralCrossSum += arrNumbers[i][j];
            }
            else {
                edgeMult *= arrNumbers[i][j];
            }
        }
    }
    return edgeMult/4 - centralCrossSum;
 }
int loadData (adventurer* adv,char* data, int currentLine, bool* readingADN,bool* isPuntuation, int* j, int**& arrN, int& size) {
    for (int i = 0;true; i++) {
        if (data[i] == '\0') {
            if (data[i - 1] == ' ') {
                data[i - 1] = '\0';
                continue;
            }
            break;
        }
    }

    cout << "currentLine: " << currentLine << endl;
    switch (currentLine) {
        case 0:
            for (int i = 0; data[i] != '\0'; i++) {
                if (data[i] == ':') {
                    for (int y = i + 1; data[y] != '\0'; y++) {
                        adv->clase[y - (i + 1)] = data[y];
                    }
                }
            }
        break;
        case 1:
            for (int i = 0; data[i] != '\0';i++){
                if (data[i] == ':') {
                    for (int y = i+1; data[y] != '\0';y++) {
                        adv->faction[y - (i+1)] = data[y];
                    }
                }
            }
        break;
        case 2:
            for (int i = 0; data[i] != '\0';i++){
                if (data[i] == ':') {
                    for (int y = i+1; data[y] != '\0';y++) {
                        adv->name[y - (i+1)] = data[y];
                    }
                }};
        break;
        default:
            if (data[0] == 'A' && data[1] == 'D' && data[2] == 'N') {
                *readingADN = true;
            }
        if (data[0] == 'A' && data[1] == 'T' && data[2] == 'R' && data[3] == 'I' && data[4] == 'B') {break;}
            if (!*readingADN) {
                int i = currentLine - 4;
                    for (int j = 0; data[j] != '\0';j++) {
                        if (data[j] != ':' && !*isPuntuation) {
                            adv->atributes[i].name[j] = data[j];
                        }
                        else if (data[j] == ':') {
                            *isPuntuation = true;
                        }
                        else {
                            char temp[100] = {};
                            int atributeValue = 0;
                            for (int y = j; data[y] != '\0';y++) {
                                cout << "data[y]: " << data[y] << endl;
                                temp[y - j] = data[y];
                            }
                            cout << "temp: " << temp << endl;
                            atributeValue = atoi(temp);
                            cout << "atributeValue: " << atributeValue << endl;
                            adv->atributes[i].value = atributeValue;
                            *isPuntuation = false;
                        }
                    };
                }

            else {
                int size = sizeSquareArray(data);
                arrN = new int*[size];
                for (int i = 0; i < size; i++) {
                    arrN[i] = new int[size];
                }
                fillSquareArray(data, arrN, j);
                return size;
            }
    }
}
void loadAdventurer(adventurer* adv, char* directory) {
    char path[256];
    snprintf(path, sizeof(path), "%s%s",directory, tempFile);
    ifstream file(path);
    char line[256];
    while (file.getline(line, sizeof(line))) {
        snprintf(path, sizeof(path), "%s%s",directory, line);
        ifstream fileIn(path);
        char lineIn[256];
        int index = 0;
        int actuaLine = 0;
        cout <<  "path: " << path << endl;
        cout << "line: " << line << endl;
        bool isPuntuation = false;
        bool readingADN = false;
        int j = 0;
        int size = 0;
        int** arrN = nullptr;
        while (fileIn.getline(lineIn, sizeof(lineIn))) {
            loadData(&adv[index], lineIn, actuaLine, &readingADN, &isPuntuation, &j, arrN, size);
            actuaLine++;
        }
        index++;
        fileIn.close();
       int ADN = calculateADN(arrN, size);
        adv[index].puntuation = ADN;
    }
    file.close();
}

class Adventurers {
public:
    adventurer** adventurers;
    int advQty;

    void load(char*& directory) {
        normalizePath(directory);
        advQty = countFiles(directory);
        adventurers = new adventurer *[advQty];
        for (int i = 0; i < advQty; i++) {
            adventurers[i] = new adventurer;
        }
        loadAdventurer(*adventurers, directory);
    }

};
int main() {
    Adventurers adventurers{};
    char directory[200];
    cout << "Ingrese la ruta del directorio: ";
    cin >> directory;
    char* dirPtr = directory;
    adventurers.load(dirPtr);
adventurers.adventurers[0]->printInfo();
    return 0;
 }