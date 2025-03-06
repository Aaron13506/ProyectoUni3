#include "iostream"
#include "fstream"
#include "cmath"
#include "cstdlib"

using namespace std;

char tempFile[] = "temp.txt";

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
        cout << "Puntaje: " << puntuation << endl;
        cout << "Clase: " << clase << endl;
        cout << "Faccion: " << faction << endl;
        cout << "Nombre: " << name << endl;
        cout << "ATRIB" << endl;
        for (const auto &atr: atributes) {
            if (atr.name[0] != '\0') {
                cout << "  " << atr.name << ":" << atr.value << endl;
            }
        }
    }
};

void removeSpaces(char *str) {
    int count = 0;
    for (int i = 0; str[i]; i++) {
        if (str[i] != ' ') {
            str[count++] = str[i];
        }
    }
    str[count] = '\0';
}

void normalizePath(char *&path) {
    for (int i = 0; path[i] != '\0'; i++) {
        if (path[i] == '/') {
            path[i] = '\\';
        }
        if (path[i + 1] == '\0' && path[i] == ' ') {
            path[i] = '\\';
        } else if (path[i] != '\\' && path[i + 1] == '\0') {
            path[i + 1] = '\\';
            path[i + 2] = '\0';
        }
    }
}

int countFiles(const char *directory) {
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

void fillSquareArray(char *lineNumbers, int **arrayN, int *j) {
    for (int i = 0; lineNumbers[i] != '\0'; i++) {
        arrayN[*j][i] = lineNumbers[i] - '0';
    }

    *j += 1;
}

int sizeSquareArray(char *lineNumbers) {
    int size{0};
    for (int i = 0; lineNumbers[i] != '\0'; i++) { if (lineNumbers[i] != ' ') size++; };
    return size;
}

int calculateADN(int **arrNumbers, int arrSize) {
    int centralCrossSum = 0, edgeMult = 1, arrMid = static_cast<int>(floor(arrSize / 2));
    for (int i = 0; i < arrSize; i++) {
        for (int j = 0; j < arrSize; j++) {
            if (i == j || i + j == arrSize - 1 || i == arrMid || j == arrMid) {
                centralCrossSum += arrNumbers[i][j];
            } else {
                edgeMult *= arrNumbers[i][j];
            }
        }
    }
    return edgeMult / 4 - centralCrossSum;
}

void loadData(adventurer *adv, char *data, int currentLine, bool *readingADN, bool *isPuntuation) {
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
            for (int i = 0; data[i] != '\0'; i++) {
                if (data[i] == ':') {
                    for (int y = i + 1; data[y] != '\0'; y++) {
                        adv->faction[y - (i + 1)] = data[y];
                    }
                }
            }
            break;
        case 2:
            for (int i = 0; data[i] != '\0'; i++) {
                if (data[i] == ':') {
                    for (int y = i + 1; data[y] != '\0'; y++) {
                        adv->name[y - (i + 1)] = data[y];
                    }
                }
            };
            break;
        case 3:
            break;
        default:
            if (data[0] == 'A' && data[1] == 'D' && data[2] == 'N') {
                *readingADN = true;
            }
            if (!*readingADN) {
                int i = currentLine - 4;
                for (int j = 0; data[j] != '\0'; j++) {
                    if (data[j] != ':' && !*isPuntuation) {
                        adv->atributes[i].name[j] = data[j];
                    } else if (data[j] == ':') {
                        *isPuntuation = true;
                    } else {
                        char temp[100] = {};
                        int atributeValue = 0;
                        for (int y = j; data[y] != '\0'; y++) {
                            temp[y - j] = data[y];
                        }
                        atributeValue = atoi(temp);
                        adv->atributes[i].value = atributeValue;
                        *isPuntuation = false;
                    }
                };
            }
    }
}

void loadAdventurer(adventurer *adv, const char *directory, const char *filename) {
    char path[256];
    snprintf(path, sizeof(path), "%s%s", directory, filename);
    ifstream fileIn(path);
    char lineIn[256];
    int actuaLine = 0;
    bool isPuntuation = false;
    bool readingADN = false;
    bool arrayInit = false;
    int j = 0;
    int **arrN = nullptr;
    int size = 0;
    while (fileIn.getline(lineIn, sizeof(lineIn))) {
        char *linePtr = lineIn;
        if (!readingADN) {
            loadData(adv, linePtr, actuaLine, &readingADN, &isPuntuation);
        } else {
            if (!arrayInit) {
                size = sizeSquareArray(linePtr);
                arrN = new int *[size];
                for (int i = 0; i < size; i++) {
                    arrN[i] = new int[size];
                }
                arrayInit = true;
            }
            removeSpaces(linePtr);
            fillSquareArray(linePtr, arrN, &j);
        }
        actuaLine++;
    }
    if (arrN != nullptr) {
        int ADN = calculateADN(arrN, size);
        adv->puntuation = ADN;
    }
    fileIn.close();
}

bool compareString(char *str1, char *str2) {
    for (int i = 0; str1[i] != '\0'; i++) {
        if (str1[i] != str2[i]) {
            return false;
        }
    }
    return true;
}

bool compareSimilarityString(char *findOption, char *comparer, int findSize) {
    int count = 0;
    int j = 0;
    for (int i = 0; findOption[i] != '\0'; i++) {
        if (comparer[j] == '\0') {
            return false;
        }
        cout << findOption[i] << "[" << i << "]" << "-" << comparer[j] << "[" << j << "]" << endl;
        if (findOption[i] == comparer[j]) {
            count++;
        } else {
            i = -1;
            count = 0;
        }
        if (count == findSize) {
            return true;
        }
        j++;
        cout << i << endl;
    }
    return false;
}

class Adventurers {
public:
    adventurer **adventurers{};
    adventurer **adventurersCopy{};
    int advQty{};
    char finalOption[8] = "INICIAR";

    void load(char *directory) {
        normalizePath(directory);
        char *dirPtr = directory;
        advQty = countFiles(dirPtr);
        adventurers = new adventurer *[advQty];
        for (int i = 0; i < advQty; i++) {
            adventurers[i] = new adventurer;
        }
        ifstream file(tempFile);
        char line[256];
        int index = 0;
        while (file.getline(line, sizeof(line))) {
            loadAdventurer(adventurers[index], directory, line);
            index++;
        }
        file.close();
        remove(tempFile);
    }

    void find(char *&findOption) {
        int copyQty = advQty;
        int length = 0;
        int index = 0;
        char advOption[20];
        char findParam[120];

        adventurersCopy = new adventurer *[advQty];
        for (int i = 0; i < advQty; i++) {
            adventurersCopy[i] = adventurers[i];
        }
        while (!compareString(findOption, finalOption)) {
            while (findOption[index] != ':') {
                advOption[index] = findOption[index];
                index++;
            }

            int j = index + 1;
            index = 0;
            while (findOption[j] != '\0') {
                findParam[index] = findOption[j];
                j++;
                index++;
                length++;
            }
            char *advOptionPtr = advOption;
            char *findOptionPtr = findParam;
            char option1[] = "clase";
            char option2[] = "faccion";
            char option3[] = "nombre";
            if (!compareString(advOptionPtr, option1)) {
                if (!compareString(advOptionPtr, option2)) {
                    if (compareString(advOptionPtr, option3)) {
                        for (int h = 0; h < copyQty; h++) {
                            if (!compareSimilarityString(findOptionPtr, adventurersCopy[h]->name, length)) {
                                delete adventurersCopy[h];
                                adventurersCopy[h] = nullptr;
                                copyQty--;
                            }
                        }
                    }
                } else {
                    cout << "Faccion" << endl;
                    for (int h = 0; h < copyQty; h++) {
                        if (!compareSimilarityString(findOptionPtr, adventurersCopy[h]->faction, length)) {
                            delete adventurersCopy[h];
                            adventurersCopy[h] = nullptr;
                            copyQty--;
                        }
                    }
                }
            } else {
                cout << "Clase" << endl;
                for (int h = 0; h < copyQty; h++) {
                    if (!compareSimilarityString(findOptionPtr, adventurersCopy[h]->clase, length)) {
                        delete adventurersCopy[h];
                        adventurersCopy[h] = nullptr;
                        copyQty--;
                    }
                }
            }

            int newIndex = 0;
            for (int h = 0; h < advQty; h++) {
                if (adventurersCopy[h] != nullptr) {
                    adventurersCopy[newIndex++] = adventurersCopy[h];
                }
            }
            cin >> findOption;
        }

        for (int h = 0; h < copyQty; h++) {
            cout << "#" << h + 1 << endl;
            adventurersCopy[h]->printInfo();
        }
    }

    void sort() {
        atribute temp[110];
    }

    void select(char *&selectOption) {
        atribute temp;
        int index = 0;
        int value = 0;
        char option = '\0';
        bool isPuntuation = false;

        while (!compareString(selectOption, finalOption)) {
            while (selectOption[index] != '\0') {
                if (isPuntuation) {
                    value = value * 10 + (selectOption[index] - '0');
                    index++;
                    continue;
                }
                if (selectOption[index] == '<' || selectOption[index] == '>' || selectOption[index] == '=' ||
                    selectOption[index] == '#') {
                    option = selectOption[index];
                    isPuntuation = true;
                    index++;
                    continue;
                }
                temp.name[index] = selectOption[index];
                index++;
            }
            temp.value = value;

            for (int i = 0; i < advQty; i++) {
                bool haveAtribute = false;
                for (auto &[name, value]: adventurers[i]->atributes) {
                    if (name[0] != '\0') {
                        if (compareString(name, temp.name)) {
                            haveAtribute = true;
                        }
                    }
                }
                if (!haveAtribute) {
                    delete adventurers[i];
                    adventurers[i] = nullptr;
                    advQty--;
                }
            }
            int newIndex = 0;
            for (int h = 0; h < advQty; h++) {
                if (adventurers[h] != nullptr) {
                    adventurers[newIndex++] = adventurers[h];
                }
            }
            advQty = newIndex;
            if (option != '\0') {
                switch (option) {
                    case '<':
                        for (int i = 0; i < advQty; i++) {
                            for (auto &[name, value]: adventurers[i]->atributes) {
                                if (name[0] != '\0' && compareString(name, temp.name)) {
                                    if (value < temp.value) {
                                        delete adventurers[i];
                                        adventurers[i] = nullptr;
                                        advQty--;
                                    }
                                }
                            }
                        }
                        break;
                    case '>':
                        for (int i = 0; i < advQty; i++) {
                            for (auto &[name, value]: adventurers[i]->atributes) {
                                if (name[0] != '\0' && compareString(name, temp.name)) {
                                    if (value > temp.value) {
                                        delete adventurers[i];
                                        adventurers[i] = nullptr;
                                        advQty--;
                                    }
                                }
                            }
                        }
                        break;
                    case '=':
                        for (int i = 0; i < advQty; i++) {
                            for (auto &[name, value]: adventurers[i]->atributes) {
                                if (name[0] != '\0' && compareString(name, temp.name)) {
                                    if (value != temp.value) {
                                        delete adventurers[i];
                                        adventurers[i] = nullptr;
                                        advQty--;
                                    }
                                }
                            }
                        }
                        break;
                    case '#':
                        for (int i = 0; i < advQty; i++) {
                            for (auto &[name, value]: adventurers[i]->atributes) {
                                if (name[0] != '\0' && compareString(name, temp.name)) {
                                    if (value == temp.value) {
                                        delete adventurers[i];
                                        adventurers[i] = nullptr;
                                        advQty--;
                                    }
                                }
                            }
                        }
                        break;
                }
            }
            newIndex = 0;
            for (int h = 0; h < advQty; h++) {
                if (adventurers[h] != nullptr) {
                    adventurers[newIndex++] = adventurers[h];
                }
            }
            cin >> selectOption;
        }
    }
};

int main() {
    Adventurers adventurers{};
    char directory[200];
    char findOption[200];
    char selectOption[200];
    cout << "Ingrese la ruta del directorio: ";
    cin >> directory;
    adventurers.load(directory);
    for (int i = 0; i < adventurers.advQty; i++) {
        adventurers.adventurers[i]->printInfo();
    }

    // cout << "Ingrese el nombre del aventurero a buscar: ";
    // cin >> findOption;
    // removeSpaces(findOption);
    // char *findPtr = findOption;
    // adventurers.find(findPtr);

    cout << "Ingrese el dato del aventurero a seleccionar: ";
    cin >> selectOption;
    removeSpaces(selectOption);
    cout << "SelectOption: ";
    cout << selectOption << endl;
    char *selectPtr = selectOption;
    adventurers.select(selectPtr);
    for (int i = 0; i < adventurers.advQty; i++) {
        cout << "Aventurero #" << i + 1 << endl;
        adventurers.adventurers[i]->printInfo();
    }
    return 0;
}
