#include <ctime>
#include <iostream>
#include <chrono>
#include <thread>
#include <cctype>
#include "Miscellaneous.h"
using namespace std;

bool oldEnough();
int insert();
int makeBet(int balance);
char getRandomCharacter();
void writeGrid(char grid[3][3]);
int countWinningRows(char grid[3][3]);
int winMultiplier(int wins);
int noMoney();
bool playAgain();

int main() {
	//gets if the player is old enough
	if (!oldEnough()) {
		cout << "You're too young to play." << endl;
		return 0;
	}
	int balance, bet, winningRows, roundWin, winMult;
	char grid[3][3];

	clearScreen();
	balance = insert();
	while (true) {
		cout << "balance: " << balance << " kr" << endl << endl;
		bet = makeBet(balance);
		balance -= bet;
		clearScreen();
		cout << "You're betting " << bet << " kr" << endl;

		//fills the grid array with random symbols
		srand(time(0));
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++)
				grid[i][j] = getRandomCharacter();
		}

		//calculats and announces the won amount (if any)
		writeGrid(grid);
		winningRows = countWinningRows(grid);
		winMult = winMultiplier(winningRows);
		roundWin = bet * winMult;
		if (roundWin > 0) cout << "You won " << roundWin << " kr!" << endl;
		
		balance += roundWin;
		cout << "Current balance: " << balance << " kr" << endl;

		//gets what the player wants to do when money is out
		if (balance <= 0) {
			int quit = noMoney();
			if (quit == 1)
				break;
			else if (quit == 2)
				balance += insert();
		}

		//gets if the user wants to continue playing
		if (!playAgain())
			break;
		else
			clearScreen();
	}
	//thanks the player for playing and displays final balance
	cout << endl << "Final balance: " << balance << " kr" << endl;
	cout << "Thanks for playing!" << endl;
}

//gets the players age and checks if they're old enough
bool oldEnough() {
	int age;
	cout << "How old are you? ";
	cin >> age;
	preventCrash();
	if (age >= 18)
		return true;
	else return false;
}

//allows the player to insert more money to their balance
int insert() {
	int insertMoney;
	while (true) {
		cout << "How much money do you want to put in (100, 300, 500)? ";
		cin >> insertMoney;
		preventCrash();
		if (insertMoney == 100 || insertMoney == 300 || insertMoney == 500)
			return insertMoney;
		else
			cout << "Invalid alternativ, try again" << endl;
	}
}

//handles the betting per round
int makeBet(int balance) {
	int roundBet;
	while (true) {
		cout << "How much do you want to bet? ";
		cin >> roundBet;
		preventCrash();
		if (roundBet <= balance)
			return roundBet;
		else
			cout << "You don't have enough money, try again" << endl;
	}
}


//gets random character
//the '% 3' return a value of 0, 1, or 2 which is the indices of the symbols array
char getRandomCharacter() {
	char symbols[3] = { 'A', 'O', 'X' };	
	return symbols[rand() % sizeof(symbols)];
}

//temporary char getRandomCharacter for testing purposes
//char getRandomCharacter() { return 'A'; }

//handles writing out the grid
void writeGrid(char grid[3][3]) {
	char symbols[3] = { 'A', 'O', 'X' };
	
	cout << endl;
	for (int i = 0; i < 3; i++) {
		cout << "-------------" << endl;
		srand(time(0));
		for (int j = 0; j < 20; j++) {
			//writes out random row
			cout << "| " << symbols[rand() % 3] << " | " << symbols[rand() % 3] << " | " << symbols[rand() % 3] << " |";
			this_thread::sleep_for(chrono::milliseconds(50));
			cout << "\x1b[2K\r"; //deletes previous line
		}
		//writes out actual line
		for (int c = 0; c < 3; c++) {
			cout << "| ";
			cout << grid[i][c] << " ";
		}
		cout << "|" << endl;
	}
	cout << "-------------" << endl << endl;
}

int countWinningRows(char grid[3][3]) {
	int totalWins = 0;

	//checks rows from top to bottom
	bool threeInARow = true;
	for (int i = 0; i < 3; i++) { // goes down
		threeInARow = true;
		for (int j = 0; j < 3 - 1; j++) { // goes right
			if (grid[i][j] != grid[i][j + 1])
				threeInARow = false;
		}
		if (threeInARow)
			totalWins++;
	}

	// for 3
	//checks columns from left to right
	for (int i = 0; i < 3; i++) { // goes right
		threeInARow = true;
		for (int j = 0; j < 3 - 1; j++) { // goes down
			if (grid[j][i] != grid[j + 1][i])
				threeInARow = false;
		}
		if (threeInARow)
			totalWins++;
	}

	// for \-diagonal
	//checks diagonal from top-left (0,0) to bottom-right (2,2)
	threeInARow = true;
	for (int i = 0; i < 3 - 1; i++) {
		if (grid[i][i] != grid[i + 1][i + 1])
			threeInARow = false;
	}
	if (threeInARow)
		totalWins++;

	// for /-diagonal
	//checks diagonal from bottom-left (0,2) to top-right (2,0)
	threeInARow = true;
	for (int i = 1; i < 3; i++) {
		if (grid[0][3 - 1] != grid[i][3 - 1 - i])
			threeInARow = false;
	}
	if (threeInARow)
		totalWins++;

	//outputs the amount of winning rows of there are any
	if (totalWins > 0) cout << "You got " << totalWins << " winning rows!" << endl;
	return totalWins;
}

//returns the winMultiplaier based amount of winning rows
int winMultiplier(int wins) {
	int winMult;
	switch (wins) {
	case 1:
		winMult = 2;
		break;
	case 2:
		winMult = 3;
		break;
	case 3:
		winMult = 4;
		break;
	case 4:
		winMult = 5;
		break;
	case 5:
		winMult = 7;
		break;
	case 6: //own
		winMult = 8;
		break;
	case 8:
		winMult = 10;
		break;
	default:
		cout << "You didn't win this time." << endl;
		return 0;
	}
	cout << "You got a win-multiplier of " << winMult << "!" << endl;
	return winMult;
}

//gets what the player wants to do when there is no money
int noMoney() {
	string noMoneyChoice;
	while (true) {
		cout << endl << "You're out of money. What do you want to do?" << endl << "Type '1' or 'Quit' to quit" << endl << "Type '2' or 'More' to add more money to your balance" << endl;
		cin >> noMoneyChoice;
		if (stringToLower(noMoneyChoice) == "1" || stringToLower(noMoneyChoice) == "quit") return 1;
		else if (stringToLower(noMoneyChoice) == "2" || stringToLower(noMoneyChoice) == "more") return 2;
		else cout << "Invalid choice, try another alternativ" << endl;
	}
}

//handles if the player wants to play again
bool playAgain() {
	string playMore;
	while (true) {
		cout << "Do you want to play again? (Y/N) ";
		cin >> playMore;
		preventCrash();
		
		if (stringToLower(playMore) == "y")
			return true;
		else if (stringToLower(playMore) == "n")
			return false;
		else
			cout << "Invalid choice, try another alternative." << endl;
	}
}