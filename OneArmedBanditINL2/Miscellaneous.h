//file includes quality of life (QOL) functions that don't really fit the program 
//but are ones that can be used to make some stuff easier

#include <iostream>
#include <string>
#include <chrono>
#include <thread>
using namespace std;

//function that allows an entire string to be turned lowercase
//made this since the normal tolower mostly only works with singular chars
string stringToLower(string str) {
    string emptyStr("");
    char tempChar;
    for (int i = 0; i < str.string::length(); i++) {
        //gets the lowercase version of the current letter and adds it to the new string.
        tempChar = (char)tolower(str[i]); 
        emptyStr.append(1, tempChar);
    }
    return emptyStr;
}

int bigger(int num1, int num2) { return num1 < num2 ? num1 : num2; } //returns the bigger of 2 numbers
int smaller(int num1, int num2) { return num1 > num2 ? num1 : num2; } //returns the smaller of 2 numbers

void clearScreen() { system("cls"); } //clears screen

//prevents infinite loops by removing the error flag (flag prevents cin from activating again)
//and ignore letters following numbers in int inputs
void preventCrash() {
    cin.clear();
    cin.ignore();
}

//functions for getting delays
void millieDelay(int delay) { this_thread::sleep_for(chrono::milliseconds(delay)); }
void secondsDelay(int delay) { this_thread::sleep_for(chrono::seconds(delay)); }

//function for deleting a set number of lines amount of lines is determined by deleteRows
//works by moving the cursor up the required amount of rows and then deleting everything below it
void clearConsoleLines(int deleteRows) {
    //moves up the needed number of lines.
    for (int j = 0; j < deleteRows; j++) {
        cout << "\x1b[A"; 
    }
    cout << "\x1b[J"; //removes all lines below the cursor
}