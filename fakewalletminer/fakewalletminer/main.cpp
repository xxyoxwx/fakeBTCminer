#include <iostream>
#include <windows.h>
#include <random>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <string>

#define _CRT_SECURE_NO_WARNINGS

using namespace std;

const int MIN_HASH_LENGTH = 26;
const int MAX_HASH_LENGTH = 35;
const double MIN_BTC_AMOUNT = 0.0001;
const double MAX_BTC_AMOUNT = 1.0;

string real, rl;
auto timenow = std::chrono::system_clock::now();
std::time_t now_c = std::chrono::system_clock::to_time_t(timenow);
HANDLE iostreamHandle = GetStdHandle(STD_OUTPUT_HANDLE);
random_device rd;
mt19937 gen(rd());
uniform_int_distribution<> int_dist(0, 1);
uniform_int_distribution<> char_dist(0, 25);
uniform_int_distribution<> length_dist(MIN_HASH_LENGTH, MAX_HASH_LENGTH);
uniform_real_distribution<> btc_dist(MIN_BTC_AMOUNT, MAX_BTC_AMOUNT);

string generateRandomHash() {
    string hash;
    int length = length_dist(gen);
    hash.reserve(length);
    for (int i = 0; i < length; i++) {
        char character = 'a' + char_dist(gen);
        if (int_dist(gen) == 1) {
            character = toupper(character);
        }
        hash += character;
    }

    if (length < 35) {
        hash += string(35 - length, ' ');
    }
    return hash;
}

bool login() {
    real = "admin";
    rl = "test";
    string username, password;
    cout << "Please enter your username: ";
    getline(cin, username);
    cout << "Please enter your password: ";
    getline(cin, password);
    return username == real && password == rl;
}

bool isValidBTCAddress(string address) {
    if (address.length() > 25 && address.length() <= 35)
    {
        //TODO Add validation logic here
        return true;
    }
    else return false;
}

string requestBTCAddress() {
    string address;
    while (true) {
        cout << "Please enter your BTC address: ";
        getline(cin, address);
        if (isValidBTCAddress(address)) {
            cout << "BTC address accepted." << endl;
            Sleep(1000);
            Sleep(1000);
            Sleep(1000);
            return address;
        }
        else {
            cout << "Invalid BTC address. Please try again." << endl;
        }
    }
}

void displayVerificationMessage(string address) {
    HANDLE iostreamHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(iostreamHandle, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    cout << "[+] Successfully verified" << endl;
    Sleep(700);
    cout << "Welcome " << real << endl;
    Sleep(700);
    std::cout << "You logged in at: " << std::ctime(&now_c) << std::endl;
    cout << "Your BTC address: " << address << endl;
}

void displayRandomHash() {
    HANDLE iostreamHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(iostreamHandle, FOREGROUND_RED | FOREGROUND_INTENSITY);
    cout << "[-] ";
    cout << generateRandomHash() << " ";
    cout << "0.0000000 BTC" << endl;
    Sleep(10);
}

void runVerificationProcess() {
    string address = requestBTCAddress();
    // Add verification logic here
    bool isVerified = true;
    displayVerificationMessage(address);
}

int main() {
    gen.seed(time(nullptr));

    while (!login()) {
        cout << "Invalid login credentials. Please try again." << endl;
    }

    bool isVerified = false;
    int redTimer = 0;
    int greenTimer = 0;
    const int RED_TIME = 1000; // 10 seconds
    const int GREEN_TIME = 1000; // 1 second
    while (true) {
        int randomNumber = int_dist(gen) * 999 + 1;

        if (isVerified) {
            SetConsoleTextAttribute(iostreamHandle, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            cout << "[+] Successfully verified" << endl;
            Sleep(700);
            cout << "Welcome " << real << endl;
            Sleep(700);
            std::cout << "You logged in at: " << std::ctime(&now_c) << std::endl;
            requestBTCAddress();
            isVerified = false;
        }
        else if (redTimer > 0) {
            SetConsoleTextAttribute(iostreamHandle, FOREGROUND_RED | FOREGROUND_INTENSITY);
            cout << "[-] ";
            cout << generateRandomHash() << " ";
            cout << "0.0000000 BTC" << endl;
            Sleep(10);
            redTimer--;
        }
        else {
            if (randomNumber == 1) {
                SetConsoleTextAttribute(iostreamHandle, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                cout << "[-] ";
                cout << generateRandomHash() << " ";
                cout << "0.000" << fixed << setprecision(4) << btc_dist(gen) << " BTC" << endl;
                greenTimer = GREEN_TIME;
            }
            else if (greenTimer > 0) {
                SetConsoleTextAttribute(iostreamHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                cout << "[+] ";
                cout << generateRandomHash() << " ";
                cout << "0.000" << fixed << setprecision(4) << btc_dist(gen) << " BTC" << endl;
                greenTimer--;
            }
            else {
                redTimer = RED_TIME;
                isVerified = true;
            }
            Sleep(10);
        }
    }

    return 0;
}