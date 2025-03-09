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
    atribute atributes[120];

    void printInfo() {
        cout << "Puntaje: " << atributes[0].value << endl;
        cout << "Clase: " << clase << endl;
        cout << "Faccion: " << faction << endl;
        cout << "Nombre: " << name << endl;
        cout << "ATRIB" << endl;
        for (const auto &[name, value]: atributes) {
            if (name[0] != '\0' && !(name[0] == 'A' && name[1] == 'D' && name[2] == 'N')) {
                cout << "  " << name << ":" << value << endl;
            }
        }
    }
};

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void removeSpaces(char *str) {
    int count = 0;
    for (int i = 0; str[i]; i++) {
        if (str[i] != ' ') {
            str[count++] = str[i];
        }
    }
    str[count] = '\0';
}

void takePath(char *str) {
    int count = 0;
    for (int i = 6; str[i]; i++) {
        if (str[i] != '\"' && str[i] != '\'') {
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
                int i = currentLine - 3;
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
        adv->atributes[0].name[0] = 'A';
        adv->atributes[0].name[1] = 'D';
        adv->atributes[0].name[2] = 'N';
        adv->atributes[0].name[3] = '\0';
        adv->atributes[0].value = ADN;
    }
    fileIn.close();
}

char toLower(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + ('a' - 'A');
    }
    return c;
}

bool compareString(char *str1, char *str2) {
    for (int i = 0; str1[i] != '\0'; i++) {
        if (toLower(str1[i]) != toLower(str2[i])) {
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
        if (toLower(findOption[i]) == toLower(comparer[j])) {
            count++;
            if (count == findSize) {
                return true;
            }
        } else {
            count = 0;
            i = -1;
            j++;
            continue;
        }
        j++;
    }
    return false;
}

class Guild {
public:
    adventurer **adventurers{};
    adventurer **adventurersCopy{};
    int advQty{};
    int opsQty = 0;
    int callCount = 0;
    char finalOption[8] = "INICIAR";
    bool adventurersLoaded = false;

    void load(char *directory) {
        if (adventurersLoaded) {
            for (int i = 0; i < advQty; i++) {
                delete adventurers[i];
            }
            delete[] adventurers;
        }
        cout << directory << endl;
        takePath(directory);
        cout << directory << endl;
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
        adventurersLoaded = true;
    }

    void find(char *findOption) {
        opsQty++;
        int copyQty = advQty;
        int length = 0;
        int index = 0;
        char advOption[20];
        char findParam[120];
        int elementsRemoved = 0;

        adventurersCopy = new adventurer *[advQty];
        for (int i = 0; i < advQty; i++) {
            adventurersCopy[i] = new adventurer(*adventurers[i]);
        }
        while (true) {
            while (findOption[index] != '\'') {
                advOption[index] = findOption[index];
                index++;
            }

            int j = index + 1;
            index = 0;
            while (findOption[j] != '\0') {
                if (findOption[j] == '\'') {
                    j++;
                    continue;
                }
                findParam[index] = findOption[j];
                j++;
                index++;
            }
            length = index;
            index = 0;
            char *advOptionPtr = advOption;
            char *findOptionPtr = findParam;
            char option1[] = "clase";
            char option2[] = "faccion";
            char option3[] = "nombre";
            int optionNumber = 0;
            if (compareString(advOptionPtr, option1)) {
                optionNumber = 1;
            } else if (compareString(advOptionPtr, option2)) {
                optionNumber = 2;
            } else if (compareString(advOptionPtr, option3)) {
                optionNumber = 3;
            }

            switch (optionNumber) {
                case 1:
                    for (int h = 0; h < copyQty; h++) {
                        if (!compareSimilarityString(findOptionPtr, adventurersCopy[h]->clase, length)) {
                            delete adventurersCopy[h];
                            adventurersCopy[h] = nullptr;
                            elementsRemoved++;
                        }
                    }
                    break;
                case 2:
                    for (int h = 0; h < copyQty; h++) {
                        if (!compareSimilarityString(findOptionPtr, adventurersCopy[h]->faction, length)) {
                            delete adventurersCopy[h];
                            adventurersCopy[h] = nullptr;
                            elementsRemoved++;
                        }
                    }
                    break;
                case 3:
                    for (int h = 0; h < copyQty; h++) {
                        if (!compareSimilarityString(findOptionPtr, adventurersCopy[h]->name, length)) {
                            delete adventurersCopy[h];
                            adventurersCopy[h] = nullptr;
                            elementsRemoved++;
                        }
                    }
                    break;
                default:
                    break;
            }

            int newIndex = 0;
            for (int h = 0; h < copyQty; h++) {
                if (adventurersCopy[h] != nullptr) {
                    adventurersCopy[newIndex++] = adventurersCopy[h];
                }
            }
            copyQty -= elementsRemoved;
            cin >> findOption;
            removeSpaces(findOption);
            if (compareString(findOption, finalOption)) {
                break;
            }
        }

        for (int h = 0; h < copyQty; h++) {
            cout << "#" << h + 1 << endl;
            adventurersCopy[h]->printInfo();
        }
        for (int i = 0; i < copyQty; i++) {
            delete adventurersCopy[i];
        }
        delete[] adventurersCopy;
    };

    void sort(char *sortOption) {
        opsQty++;
        char atr[105][120];
        char symbol[105];
        int index = 0;
        int atrIndex = 0;
        int atributeIndex = 0;
        int elementsRemoved = 0;
        adventurersCopy = new adventurer *[advQty];
        for (int i = 0; i < advQty; i++) {
            adventurersCopy[i] = new adventurer(*adventurers[i]);
        }
        for (int i = 0; i < advQty; i++) {
            delete adventurers[i];
        }
        delete[] adventurers;

        while (true) {
            bool isSymbol = false;
            while (sortOption[index] != '\0') {
                if (sortOption[index] == '<' || sortOption[index] == '>') {
                    symbol[atributeIndex] = sortOption[index];
                    isSymbol = true;
                } else if (!isSymbol) {
                    atr[atributeIndex][atrIndex] = sortOption[index];
                }
                index++;
            }
            atr[atributeIndex][atrIndex] = '\0';
            atributeIndex++;
            index = 0;
            atrIndex = 0;
            cin >> sortOption;
            removeSpaces(sortOption);
            if (compareString(sortOption, finalOption)) {
                break;
            }
        }
        for (int i = 0; i < advQty; i++) {
            bool haveAttribute = false;
            for (int j = 0; j < atributeIndex; j++) {
                for (int k = 0; k < 100; k++) {
                    if (compareString(atr[j], adventurersCopy[i]->atributes[k].name)) {
                        haveAttribute = true;
                        break;
                    }
                }
                if (haveAttribute) {
                    break;
                }
            }
            if (!haveAttribute) {
                delete adventurersCopy[i];
                adventurersCopy[i] = nullptr;
                elementsRemoved++;
            }
        }
        int newIndex = 0;
        for (int i = 0; i < advQty; i++) {
            if (adventurersCopy[i] != nullptr) {
                adventurersCopy[newIndex++] = adventurersCopy[i];
            }
        }

        advQty = newIndex;

        adventurers = new adventurer *[advQty];
        for (int i = 0; i < advQty; i++) {
            adventurers[i] = new adventurer(*adventurersCopy[i]);
        }
        for (int i = 0; i < advQty; i++) {
            delete adventurersCopy[i];
        }
        delete[] adventurersCopy;

        for (int i = 0; i < atributeIndex; i++) {
            if (!(symbol[i] == '<' || symbol[i] == '>')) {
                continue;
            }
            for (int j = 0; j < advQty; j++) {
                for (int k = 0; k < advQty - 1; k++) {
                    int atributeIndex1 = -1, atributeIndex2 = -1;
                    for (int l = 0; l < 100; l++) {
                        if (compareString(atr[i], adventurers[k]->atributes[l].name)) {
                            atributeIndex1 = l;
                        }
                        if (compareString(atr[i], adventurers[k + 1]->atributes[l].name)) {
                            atributeIndex2 = l;
                        }
                    }
                    if (atributeIndex1 == -1 || atributeIndex2 == -1) {
                        continue;
                    }
                    if ((symbol[i] == '<' && adventurers[k]->atributes[atributeIndex1].value > adventurers[k + 1]->
                         atributes[atributeIndex2].value) ||
                        (symbol[i] == '>' && adventurers[k]->atributes[atributeIndex1].value < adventurers[k + 1]->
                         atributes[atributeIndex2].value)) {
                        adventurer *temp = adventurers[k];
                        adventurers[k] = adventurers[k + 1];
                        adventurers[k + 1] = temp;
                    }
                }
            }
        }
    }

    void select(char *selectOption) {
        atribute temp;
        int index = 0;
        int value = 0;
        int copyQty = advQty;
        char option = '\0';
        bool isPuntuation = false;
        int elementsRemoved = 0;
        adventurersCopy = new adventurer *[advQty];
        for (int i = 0; i < advQty; i++) {
            adventurersCopy[i] = new adventurer(*adventurers[i]);
        }
        for (int i = 0; i < advQty; i++) {
            delete adventurers[i];
        }
        delete[] adventurers;
        while (true) {
            while (selectOption[index] != '\0') {
                if (isPuntuation) {
                    value = value * 10 + (selectOption[index] - '0');
                } else if (selectOption[index] == '<' || selectOption[index] == '>' || selectOption[index] == '=' ||
                           selectOption[index] == '#') {
                    option = selectOption[index];
                    isPuntuation = true;
                } else {
                    temp.name[index] = selectOption[index];
                }
                index++;
            }
            temp.name[index] = '\0'; // Asegurarse de que el nombre termine con '\0'
            temp.value = value;
            index = 0;
            value = 0;
            isPuntuation = false;

            for (int i = 0; i < copyQty; i++) {
                bool haveAtribute = false;
                for (auto &[name, value]: adventurersCopy[i]->atributes) {
                    if (name[0] != '\0' && compareString(name, temp.name)) {
                        haveAtribute = true;
                        break;
                    }
                }
                if (!haveAtribute) {
                    delete adventurersCopy[i];
                    adventurersCopy[i] = nullptr;
                    copyQty--;
                }
            }
            int newIndex = 0;
            for (int h = 0; h < advQty; h++) {
                if (adventurersCopy[h] != nullptr) {
                    adventurersCopy[newIndex++] = adventurersCopy[h];
                }
            }

            if (option != '\0') {
                switch (option) {
                    case '<':
                        for (int i = 0; i < copyQty; i++) {
                            for (auto &[name, value]: adventurersCopy[i]->atributes) {
                                if (name[0] != '\0' && compareString(name, temp.name) && value < temp.value) {
                                    delete adventurersCopy[i];
                                    adventurersCopy[i] = nullptr;
                                    elementsRemoved++;
                                    break;
                                }
                            }
                        }
                        break;
                    case '>':
                        for (int i = 0; i < copyQty; i++) {
                            for (auto &[name, value]: adventurersCopy[i]->atributes) {
                                if (name[0] != '\0' && compareString(name, temp.name) && value > temp.value) {
                                    delete adventurersCopy[i];
                                    adventurersCopy[i] = nullptr;
                                    elementsRemoved++;
                                    break;
                                }
                            }
                        }
                        break;
                    case '=':
                        for (int i = 0; i < copyQty; i++) {
                            for (auto &[name, value]: adventurersCopy[i]->atributes) {
                                if (name[0] != '\0' && compareString(name, temp.name) && value != temp.value) {
                                    delete adventurersCopy[i];
                                    adventurersCopy[i] = nullptr;
                                    elementsRemoved++;
                                    break;
                                }
                            }
                        }
                        break;
                    case '#':
                        for (int i = 0; i < copyQty; i++) {
                            for (auto &[name, value]: adventurersCopy[i]->atributes) {
                                if (name[0] != '\0' && compareString(name, temp.name) && value == temp.value) {
                                    delete adventurersCopy[i];
                                    adventurersCopy[i] = nullptr;
                                    elementsRemoved++;
                                    break;
                                }
                            }
                        }
                        break;
                    default: break;
                }
            }
            newIndex = 0;
            for (int h = 0; h < advQty; h++) {
                if (adventurersCopy[h] != nullptr) {
                    adventurersCopy[newIndex++] = adventurersCopy[h];
                }
            }
            copyQty -= elementsRemoved;
            cin >> selectOption;
            removeSpaces(selectOption);
            if (compareString(selectOption, finalOption)) {
                break;
            }
        }

        advQty = copyQty;
        adventurers = new adventurer *[copyQty];
        for (int i = 0; i < advQty; i++) {
            adventurers[i] = new adventurer(*adventurersCopy[i]);
        }
        for (int i = 0; i < advQty; i++) {
            delete adventurersCopy[i];
        }
        delete[] adventurersCopy;
        opsQty++;
        cout << "Operacion realizada" << endl;
        for (int i = 0; i < advQty; i++) {
            adventurers[i]->printInfo();
        }
    }

    void print() {
        cout << "operaciones realizadas: " << opsQty << endl;
        callCount++;
        char fileName[50];
        snprintf(fileName, sizeof(fileName), "Operaciones%d.out", callCount);
        ofstream fileOut(fileName);
        fileOut << "operaciones realizadas: " << opsQty << endl;
        fileOut << "Aventureros encontrados: " << advQty << endl;
        fileOut << "Lista de aventureros:" << endl;
        for (int i = 0; i < advQty; i++) {
            fileOut << "#" << i + 1 << endl;
            fileOut << "Puntaje: " << adventurers[i]->atributes[0].value << endl;
            fileOut << "Clase: " << adventurers[i]->clase << endl;
            fileOut << "Faccion: " << adventurers[i]->faction << endl;
            fileOut << "Nombre: " << adventurers[i]->name << endl;
            fileOut << "Atributos" << endl;
            for (const auto &[name, value]: adventurers[i]->atributes) {
                if (name[0] != '\0' && !(name[0] == 'A' && name[1] == 'D' && name[2] == 'N')) {
                    fileOut << name << ":" << value << endl;
                }
            }
        }
        fileOut.close();
    }
};

int main() {
    Guild guild{};
    char directory[200];
    char findOption[200];
    char selectOption[200];
    char sortOption[200];
    char lOption[200];
    char load[] = "CARGAR";
    char find[] = "BUSCAR";
    char sort[] = "ORDENAR";
    char select[] = "SELECCIONAR";
    char print[] = "IMPRIMIR";
    int actualOption = 0;

    while (true) {
        char option[120];
        cin.getline(option, 120);
        removeSpaces(option);
        if (compareSimilarityString(option, load, 6)) {
            actualOption = 1;
        }
        if (compareString(option, find)) {
            actualOption = 2;
        } else if (compareString(option, sort)) {
            actualOption = 3;
        } else if (compareString(option, select)) {
            actualOption = 4;
        } else if (compareString(option, print)) {
            actualOption = 5;
        }
        cout << "actualOption: " << actualOption << endl;

        switch (actualOption) {
            case 1:
                guild.load(directory);
                for (int i = 0; i < guild.advQty; i++) {
                    guild.adventurers[i]->printInfo();
                }
                break;
            case 2:
                cout << "Ingrese la opción de búsqueda: ";
                cin.getline(findOption, 200);
                removeSpaces(findOption);
                guild.find(findOption);
                clearInputBuffer(); // Limpiar el buffer de entrada
                break;
            case 3:
                cout << "Ingrese la opción de ordenamiento: ";
                cin.getline(sortOption, 200);
                removeSpaces(sortOption);
                guild.sort(sortOption);
                clearInputBuffer(); // Limpiar el buffer de entrada
                break;
            case 4:
                cout << "Ingrese la opción de selección: ";
                cin.getline(selectOption, 200);
                removeSpaces(selectOption);
                guild.select(selectOption);
                clearInputBuffer(); // Limpiar el buffer de entrada
                break;
            case 5:
                cout << "IMPRIMIR" << endl;
                guild.print();
                break;
            default:
                break;
        }

        for (char &i: option) {
            i = '\0';
        }
        actualOption = 0;
    }
}

