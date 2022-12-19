#include <ctime>
#include <iostream>
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
	char grid[3][3]{};
	
	clearScreen();
	balance = insert();
	while (true) {
		//prints out the players current balance
		cout << "balance: " << balance << " kr" << endl << endl;
		bet = makeBet(balance);
		balance -= bet;
		clearScreen();
		cout << "You're betting " << bet << " kr" << endl;

		//fills the grid array with random symbols
		srand((signed)time(0));
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++)
				grid[i][j] = getRandomCharacter();
		}

		//calculates and announces the won amount (if any)
		writeGrid(grid);
		winningRows = countWinningRows(grid);
		winMult = winMultiplier(winningRows);
		roundWin = bet * winMult;
		if (roundWin > 0) cout << "You won " << roundWin << " kr!" << endl;
		
		balance += roundWin;
		cout << endl << "Current balance: " << balance << " kr" << endl;

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
	bool betZero = false;
	while (true) {
		cout << "How much do you want to bet? ";
		cin >> roundBet;
		preventCrash();
		if (roundBet > balance)
			cout << "You don't have enough money, try again" << endl;
		else if (roundBet <= 0 && !betZero) {
			cout << "Did you mean to bet 0 kr?" << endl << "Type '0' again to confirm" << endl;
			betZero = true;
		}
		else
			return roundBet;
	}
}

//gets random character
//the '% sizeof(symbols)' returns a value between 0 and the length of the array (in this case 3) - 1, which matches it's indices
char getRandomCharacter() {
	char symbols[3] = { 'A', 'O', 'X' };	
	return symbols[rand() % sizeof(symbols)];
}

//function for writing out the grid
//write all random for ~40 times then each line after 20 loops
//version for writing entire grid (3x3) and then the real grid 1 row at a time from top to bottom.
void writeGrid(char grid[3][3]) {
	char symbols[3] = { 'A', 'O', 'X' };
	int deleteRows = 7, writeRandomRows = 0, actualRow = 0, randomRows = 80;

	for (int i = 0; i < randomRows; i++) {
		//writes out the random grid. The random characters don't change the actual grid so they just get
		//overwritten by the actual grid when their time comes
		cout << "-------------" << endl;
		for (int j = writeRandomRows; j < 3; j++) {
			for (int k = 0; k < 3; k++)
				cout << "| " << symbols[rand() % 3] << " ";
			cout << "|" << endl;
			cout << "-------------" << endl;
		}

		//delay for extra suspension and so the player sees what happens
		millieDelay(50);

		//clears the grid every turn
		clearConsoleLines(deleteRows);

		//writes out the actual lines after a certain amount of turns have passed
		//first after half, second after 3/4, and last row on last turn
		if (i == 39 || i == 59 || i == 79) {
			cout << "-------------" << endl;
			for (int j = 0; j < 3; j++)
				cout << "| " << grid[actualRow][j] << " ";
			cout << "|" << endl;
			//changes values as to not overwrite the actual grid lines
			actualRow++;
			writeRandomRows++;
			deleteRows -= 2;
		}
	}
	//for writing out the last line of the grid
	cout << "-------------" << endl;
}

//function for calculating the amount of winning rows
int countWinningRows(char grid[3][3]) {
	int totalWins = 0;

	//for rows
	//checks rows from top to bottom
	bool threeInARow = true;
	for (int i = 0; i < 3; i++) { // goes down
		threeInARow = true;
		for (int j = 0; j < 2; j++) { // goes right
			if (grid[i][j] != grid[i][j + 1])
				threeInARow = false;
		}
		if (threeInARow)
			totalWins++;
	}

	//for columns
	//checks columns from left to right
	for (int i = 0; i < 3; i++) { // goes right
		threeInARow = true;
		for (int j = 0; j < 2; j++) { // goes down
			if (grid[j][i] != grid[j + 1][i])
				threeInARow = false;
		}
		if (threeInARow)
			totalWins++;
	}

	// for \-diagonal
	//checks diagonal from top-left (0,0) to bottom-right (2,2)
	threeInARow = true;
	for (int i = 0; i < 2; i++) {
		if (grid[i][i] != grid[i + 1][i + 1])
			threeInARow = false;
	}
	if (threeInARow)
		totalWins++;

	// for /-diagonal
	//checks diagonal from bottom-left (0,2) to top-right (2,0)
	threeInARow = true;
	for (int i = 1; i < 3; i++) {
		if (grid[0][2] != grid[i][2 - i])
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
	//index 0 = 0 wins, index 1 = 1 win, index 2 = 2 wins, etc 
	int winMultipliers[9] = { 0,2,3,4,5,7,8,0,10 };
	int won = winMultipliers[wins];
	if (won > 1) cout << "You got a win-multiplier of " << won << "!" << endl;
	else cout << "You didn't win this time." << endl;
	return won;
}

//gets what the player wants to do when there is no money
int noMoney() {
	string noMoneyChoice;
	while (true) {
		cout << endl << "You're out of money. What do you want to do?" << endl 
			<< "Type '1' or 'Quit' to quit" << endl 
			<< "Type '2' or 'More' to add more money to your balance" << endl;
		cin >> noMoneyChoice;
		preventCrash();
		if (noMoneyChoice == "1" || stringToLower(noMoneyChoice) == "quit") return 1;
		else if (noMoneyChoice == "2" || stringToLower(noMoneyChoice) == "more") return 2;
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
		
		if (stringToLower(playMore) == "y" || stringToLower(playMore) == "yes")
			return true;
		else if (stringToLower(playMore) == "n" || stringToLower(playMore) == "no")
			return false;
		else
			cout << "Invalid choice, try another alternative." << endl;
	}
}