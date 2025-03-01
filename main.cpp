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
    char faction [120]{};
    char name[120]{};
    int puntuation{};
    atribute atributes[120];
    };

char tempFile[] = "temp.txt";
int arrN[size][size];

void normalizePath(char* path) {
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
    char command[200];
    snprintf(command, sizeof(command), "dir /b %s *.in > %s", directory, tempFile);
    system(command);
    ifstream file(tempFile);
    int count = 0;
    char line[256];
    while (file.getline(line, sizeof(line))) {
        count++;
    }


    return count;
}
int fillSquareArray(char* lineNumbers) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            arrN[i][j] = lineNumbers[i * size + j] - '0';
        }
    }
}
int sizeSquareArray(char* lineNumbers) {
        int size {0};
        for (int i = 0; lineNumbers[i] != '\0';i++){if (lineNumbers[i] != ' ') size++;};
        return size;
}
int calculateADN (int** arrNumbers) {
    int arrSize = sizeof(arrNumbers) / sizeof(arrNumbers[0]);
    int centralCrossSum = 0, edgeMult = 1, arrMid = static_cast<int>(floor(arrSize / 2)), initialBorder = 0, finalBorder = arrSize - 1;
    for (int i = 0; i < arrSize; i++) {
        for (int j = 0; j < arrSize; j++) {
            if (i == j || i + j == arrSize - 1) {
                centralCrossSum += arrNumbers[i][j];
            }
            else if (i == arrMid || j == arrMid) {
                centralCrossSum += arrNumbers[i][j];
            }
            else {
                edgeMult *= arrNumbers[i][j];
            }
        }
    }
    return edgeMult/4 - centralCrossSum;
 }
void loadData (adventurer* adv,char* data, int line) {
    bool readingADN = false;
    for (int i = 0;true; i++) {
        if (data[i] == '\0') {
            if (data[i - 1] == ' ') {
                data[i - 1] = '\0';
                continue;
            }
            break;
        }
    }
switch (line) {
    case 0:
        for (int i = 0; data[i] != '\0';i++){
            if (data[i] == ':') {
            for (int y = i+1; data[y] != '\0';i++) {
                adv->clase[y - i+1] = data[y];
            }
        }};
    break;
    case 1:
        for (int i = 0; data[i] != '\0';i++){
            if (data[i] == ':') {
            for (int y = i+1; data[y] != '\0';i++) {
                adv->faction[y - i+1] = data[y];
            }
        }};
        break;
    case 2:
        for (int i = 0; data[i] != '\0';i++){
            if (data[i] == ':') {
            for (int y = i+1; data[y] != '\0';i++) {
                adv->name[y - i+1] = data[y];
            }
        }};
        break;
    default:

        if (data != "ATRIB" || data !="ADN" || !readingADN) {
            bool isPuntuation = false;
            for (int i = 0; data[i] != '\0';i++) {
                if (data[i] != ':' && !isPuntuation) {
                    adv->atributes[i].name[i] = data[i];
                }
                else if (data[i] == ':') {
                    isPuntuation = true;
                }
                else {
                    char temp[110] = {};
                    int atributeValue = 0;
                    for (int y = i; data[y] != '\0';y++) {
                        temp[y - i] = data[y];
                    }
                    atributeValue = atoi(temp);
                    adv->atributes[i].value = atributeValue;

                }
            };
        }
    else if (data == "ADN") {
        readingADN = true;
    }
    else {
        int size = sizeSquareArray(data);
        fillSquareArray(data);
        adv->puntuation = calculateADN(arrN);
    }
}
adventurer* loadAdventurer(adventurer* adv, const char* directory) {
    char path[200];
    snprintf(path, sizeof(path), "%s%s", directory, tempFile);
    ifstream file(path);
    char line[256];
    while (file.getline(line, sizeof(line))) {
        snprintf(path, sizeof(path), "%s%s", directory, line);
        ifstream fileIn(path);
        char lineIn[256];
        int index = 0;
        int actuaLine = 0;
        while (fileIn.getline(lineIn, sizeof(lineIn))) {
            loadData(&adv[index], lineIn, actuaLine);
            fileIn.close();
            actuaLine++;
        }
        index++;
    }
}
}
class Adventurers {
public:
    adventurer** adventurers;

    adventurer* load(char* directory) {
        normalizePath(directory);
        int advQty = countFiles(directory);
        adventurers = new adventurer*[advQty];

    }
};
int main() {
 }