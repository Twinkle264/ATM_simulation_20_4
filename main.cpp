#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

void sumATM(const int *moneyInATM, int *countsBanknotes, const int *banknotes, int &sumMoneyInATM) {
    sumMoneyInATM = 0;
    for (int i = 0; i < 1000; i++) {
        for (int j = 0; j < 5; ++j) {
            if (moneyInATM[i] == banknotes[j]) {
                countsBanknotes[j]++;
            }
        }
        sumMoneyInATM += moneyInATM[i];
    }
}

bool cash(int sum, int *moneyInATM, const int *banknotes, const int *countsBanknotes) {
    int counts[5] = {0};
    int tmp = sum;
    for (int i = 0; i < 5; i++) {
        while (tmp >= banknotes[i] && (countsBanknotes[i] != 0 && counts[i] < countsBanknotes[i])) {
            tmp -= banknotes[i];
            counts[i]++;
        }
    }

    int sumBank = 0;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < counts[i]; ++j) {
            sumBank += banknotes[i];
        }
    }

    if (sumBank != sum) return false;

    for (int i = 0; i < 1000; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (moneyInATM[i] == banknotes[j] && counts[j] != 0) {
                moneyInATM[i] = 0;
                sum -= moneyInATM[i];
                counts[j]--;
            } else if (moneyInATM[i] == 0) continue;
            if (sum <= 0) break;
        }
    }
    return true;
}

bool is_correctSum(int sum, int sumMoneyInATM) {
    if (sum > sumMoneyInATM) {
        cout << "ERROR: ATM cannot dispense more than " << sumMoneyInATM << endl;
        return false;
    } else if (sum < 100 || sum % 100 != 0) {
        cout << "ERROR: ATM cannot dispense " << sum << endl;
        cout << "The smallest denomination - 100" << endl;
        return false;
    }
    return true;
}

bool readFromFile(int *moneyInATM, int &sumMoneyInATM) {
    ifstream inFile("./files/bankomat.bin", ios::binary);
    if (!inFile.is_open()) {
        cerr << "FILE READ ERROR!";
        return false;
    }

    for (int i = 0; i < 1000; ++i) {
        inFile >> moneyInATM[i];
    }
    inFile.close();
    return true;
}

bool writeInFile(int *moneyInATM) {
    ofstream outFile("./files/bankomat.bin", ios::binary);
    if (!outFile.is_open()) {
        cerr << "FILE WRITE ERROR!";
        return false;
    }

    for (int i = 0; i < 1000; ++i) {
        outFile << moneyInATM[i] << endl;
    }
    outFile.close();

    return true;
}

void addMoney(int *moneyInATM, const int banknotes[]) {
    for (int i = 0; i < 1000; ++i) {
        if (moneyInATM[i] == 0) {
            moneyInATM[i] = banknotes[rand() % 5];
        }
    }
}

int main() {
    srand(std::time(nullptr));
    int moneyInATM[1000] = {0};
    int sumMoneyInATM = 0;
    int sum = 0;
    int banknotes[5] = {5000, 1000, 500, 200, 100};
    int countsBanknotes[5] = {0};

    readFromFile(moneyInATM, sumMoneyInATM);
    sumATM(moneyInATM, countsBanknotes, banknotes, sumMoneyInATM);

    cout << "Sum in ATM: " << sumMoneyInATM;
    cout << endl;

    char command = ' ';
    do {
        cout << R"(Enter "+" to top up an ATM or "-" to withdraw cash from an ATM: )";
        cin >> command;
    } while (command != '+' && command != '-');

    if (command == '+') {
        cout << "\tATM replenishment" << endl;
        addMoney(moneyInATM, banknotes);
        writeInFile(moneyInATM);
        sumATM(moneyInATM, countsBanknotes, banknotes, sumMoneyInATM);
        cout << "in ATM: " << sumMoneyInATM;
        cout << endl;
        return 0;
    } else if (command == '-') {
        cout << endl;
        cout << "\tCash withdrawal" << endl;
        cout << "Enter a sum for to withdraw cash: ";
        cin >> sum;
        if (!is_correctSum(sum, sumMoneyInATM)) {
            return 1;
        } else {
            if (cash(sum, moneyInATM, banknotes, countsBanknotes)) {
                cout << "Cash: " << sum << endl;
                sumATM(moneyInATM, countsBanknotes, banknotes, sumMoneyInATM);
                cout << "in ATM: " << sumMoneyInATM;
                cout << endl;
            } else {
                cout << "The ATM cannot dispense this amount due to the lack of necessary banknotes.";
            }
        }

    }
    writeInFile(moneyInATM);

    return 0;
}
