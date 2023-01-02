#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
typedef struct {
	int position;
	int length;
	int end;
} sol_t;
typedef struct {
	char name[20];
	int position;
}player_t;

void CreateSnakes(sol_t snakes[], int size);
void printboard();
void PrintSnakes(sol_t snakes[]);
void PrintLadders(sol_t ladders[]);
void CreateLadders(sol_t ladders[], sol_t snakes[], int size);
void BuildPlayerList(player_t *players, int count);
void ShowPlayerList(player_t *players, int count);
void TheGame(player_t *players, sol_t snakes[], sol_t ladders[],int playerCount, int  playerIndex);
int IsDuplicate(sol_t object, sol_t list[], int size);
int RollDice();
int CheckVictory(int position);
int CheckSnakes(int position, sol_t snakes[]);
int CheckLadders(int position, sol_t ladders[]);
void ShowRules();
int GetPlayerCount();
void SaveAndExitGame(int playerCount, int currentPlayer, player_t *players, sol_t snakes[], sol_t ladders[]);
int LoadGame(int fileChoice, int *playerCount, int *currentPlayer, player_t *players, sol_t snakes[], sol_t ladders[]);
int CheckFiles();
void removeFile(char fileName[]);
void MenuRemoveFile();
void CreateNewReferenceFile(int fileCount, char files[20][20], char nFile[20]);
int WhatFile();
#define MAX 7

//=======================================================================================


void main() {

	player_t *players;
	int playerCount,loaded,fileChoice;
	int currentPlayer=0;
	time_t theTime;
	int menuChoice;
	sol_t snakes[MAX];
	sol_t ladders[MAX];
	srand((unsigned) time(&theTime));
	do {
		printf("===========================================\n");
		printf("Welcome To Snakes and Ladders 3000!\n");
		printf("===========================================\n");
		printf("%-14s New Game\n", "1)");
		printf("%-14s Load Game\n", "2)");
		printf("%-14s Game Rules\n", "3)");
		printf("%-14s Delete File\n", "4)");
		printf("%-14s Exit Game\n", "0)");
		printf("===========================================\n");
		scanf("%d", &menuChoice);
		switch (menuChoice) {
		case 1:
			do {
				printf("How many players? Minimum - 2 Maximum - 6\n");
				scanf("%d", &playerCount);
			} while (playerCount > 6 || playerCount < 2);
			players = (player_t *)malloc(playerCount*sizeof(player_t));
			BuildPlayerList(players,playerCount);
			ShowPlayerList(players, playerCount);
			CreateSnakes(snakes, 7);
			CreateLadders(ladders, snakes, 7);
			printboard();
			PrintSnakes(snakes);
			PrintLadders(ladders);
			TheGame(players, snakes, ladders,playerCount,currentPlayer);
			break;
		case 2:
			fileChoice = WhatFile();
			if (fileChoice == -1) {
				break;
			}
			printf("loading...\n");
			playerCount = GetPlayerCount(fileChoice);
			players = (player_t *)malloc(playerCount*sizeof(player_t));
			loaded=LoadGame(fileChoice, &playerCount, &currentPlayer, players, snakes, ladders);
			if (loaded > 0) {
                printboard();
				PrintSnakes(snakes);
				PrintLadders(ladders);
				printf("\nGame Loaded\n");
				TheGame(players, snakes, ladders, playerCount, currentPlayer);
			}
			else {
				printf("No games to load! Maybe Try a new game...\n");
			}
			break;
		case 3:
			ShowRules();
			break;

		case 4:
			printf("\nDelete a Save File\n");
			MenuRemoveFile();
			break;
		case 0:
			printf("\nThanks for Playing! Press RETURN to Exit!\n");
			break;
		default:
			printf("Invalid Input :(\n");
			break;
		}
	} while (menuChoice != 0);

	getch();


}
//=======================================================================================
void ShowRules() {
	printf("\n===========================================\n");
	printf("Welcome To Snakes and Ladders 3000!\n");
	printf("===========================================\n");
	printf("Welcome to Snakes and Ladders 3000!\nIt is a lot like regular Snakes And\nLadders but has the number 3000 after it.\nThere is no good reason for this.\n");
	printf("Players: 2 - 6\n");
	printf("===============Game Rules==================\n");
	printf("All players start at position 1 on the board.\n");
	printf("First to get to space 100 wins the game!\n");
	printf("Roll The Dice and move along the board.");
	printf("\nRoll a 4? Move 4 spaces.\n");
	printf("However, if you land on a snake:");
	printf("\nyou will slide down the board and have to work your way up again.\n");
	printf("If you land on a ladder:");
	printf("\nThe player may climb the ladder and take a nice shortcut to get to the top.\n");
	printf("Snakes and Ladders is a game of luck.\nSo... Good Luck!\n");
	printf("===========================================\n");
}

void CreateSnakes(sol_t snakes[], int size) {
	int i;
	for (i = 0;i < size;i++) {
		do {
			snakes[i].position = (rand() % 89) + 11;
			snakes[i].length = ((rand() % (snakes[i].position - 10)) + 10);
			snakes[i].end = snakes[i].position - snakes[i].length;
		} while (IsDuplicate(snakes[i],snakes,i)> 0);

	}
}
void CreateLadders(sol_t ladders[], sol_t snakes[], int size) {
	int maxPossibleLength;
	int i = 0;
	for (i = 0;i < size;i++) {
		do {
			ladders[i].position = (rand() % 89) + 2;

			maxPossibleLength = 99 - ladders[i].position;
			if (maxPossibleLength < 10) {
				ladders[i].length = 9;
			}
			else if (maxPossibleLength == 10) {
				ladders[i].length = 10;
			}
			else {
				ladders[i].length = ((rand() % (maxPossibleLength - 9) + 10));
			}
			ladders[i].end = ladders[i].position + ladders[i].length;
		} while ((IsDuplicate(ladders[i], ladders, i) > 0) || (IsDuplicate(ladders[i], snakes, MAX) > 1));


	}
}
void printboard()
{
      printf("----------------------------------------------------\n");
      printf("| 100 | 99 | 98 | 97 | 96 | 95 | 94 | 93 | 92 | 91 |\n");
      printf("----------------------------------------------------\n");
      printf("|  81 | 82 | 83 | 84 | 85 | 86 | 87 | 88 | 89 | 90 |\n");
      printf("----------------------------------------------------\n");
      printf("|  80 | 79 | 78 | 77 | 76 | 75 | 74 | 73 | 72 | 71 |\n");
      printf("----------------------------------------------------\n");
      printf("|  61 | 62 | 63 | 64 | 65 | 66 | 67 | 68 | 69 | 70 |\n");
      printf("----------------------------------------------------\n");
      printf("|  60 | 59 | 58 | 57 | 56 | 55 | 54 | 53 | 52 | 51 |\n");
      printf("----------------------------------------------------\n");
      printf("|  41 | 42 | 43 | 44 | 45 | 46 | 47 | 48 | 49 | 50 |\n");
      printf("----------------------------------------------------\n");
      printf("|  40 | 39 | 38 | 37 | 36 | 35 | 34 | 33 | 32 | 31 |\n");
      printf("----------------------------------------------------\n");
      printf("|  21 | 22 | 23 | 24 | 25 | 26 | 27 | 28 | 29 | 30 |\n");
      printf("----------------------------------------------------\n");
      printf("|  20 | 19 | 18 | 17 | 16 | 15 | 14 | 13 | 12 | 11 |\n");
      printf("----------------------------------------------------\n");
      printf("|  1  |  2 |  3 |  4 |  5 |  6 |  7 |  8 |  9 | 10 |\n");
      printf("----------------------------------------------------\n");

}
void PrintSnakes(sol_t snakes[]) {

	int i;
	printf("\nSnakes\n");

	for (i = 0;i < 7;i++) {
		printf("Snake Positioned at square: %d Tail Located at: %d\n", snakes[i].position, snakes[i].end);
	}
}

void PrintLadders(sol_t ladders[]) {

	int i;
	printf("\nLadders\n");

	for (i = 0;i < 7;i++) {
		printf("Ladder Positioned at square: %d Reaches Square: %d\n", ladders[i].position, ladders[i].end);
	}
}


int IsDuplicate(sol_t object, sol_t list[], int size) {
	int i;
	int duplicate = -1;
	for (i = 0;i < size;i++) {
		if (object.position == list[i].position || object.end == list[i].position) {
			duplicate = 1;
			return duplicate;
		}

	}
	return duplicate;


}

void BuildPlayerList(player_t *players, int count) {
	int i;
	for (i = 0;i < count;i++) {
		printf("\nEnter Player %d's Name: ", i + 1);
		scanf("%s", (players+ i)->name);
		(players + i)->position = 1;
	}

}
void ShowPlayerList(player_t *players, int count) {
	int i;
	for (i = 0;i < count;i++) {
		printf("Player %d: %s Position: %d\n", i+1, (players+i)->name, (players+i)->position);
	}

}

int RollDice() {
	int roll = (rand() % 6) + 1;
	printf("You rolled a %d\n", roll);
	return roll;
}

void TheGame(player_t *players, sol_t snakes[], sol_t ladders[], int playerCount,int playerIndex) {
	int userChoice;
	int victorYN = 0;
	int diceRoll;
	char *save;
	int currentPlayer = playerIndex;
	do {
		printf("Current Player: %s Position: %d\n\n", (players + currentPlayer)->name, (players + currentPlayer)->position);
		printf("%-14s Roll Dice\n", "1)");
		printf("%-14s Save and Exit\n", "0)");
		scanf("%d", &userChoice);
		switch (userChoice) {
		case 1:
			diceRoll = RollDice();
			(players + currentPlayer)->position += diceRoll;
			printf("%s is now at position : %d\n\n", (players + currentPlayer)->name, (players + currentPlayer)->position);


			(players + currentPlayer)->position = CheckLadders((players + currentPlayer)->position, ladders);

			victorYN = CheckVictory((players + currentPlayer)->position); //pass an integer

			if (victorYN > 0) {
				printf("\n%s has reached position 100! Victory Achieved!\n", (players + currentPlayer)->name);
				return;
			}
			(players + currentPlayer)->position = CheckSnakes((players + currentPlayer)->position, snakes);
			if (currentPlayer == (playerCount - 1)) {
				currentPlayer = 0;
				printf("\n===========================================\n");
				printf("Snakes and Ladders 3000!\nCurrent Positions\n");
				printf("===========================================\n");
				ShowPlayerList(players, playerCount);
				printboard();
				PrintSnakes(snakes);
				PrintLadders(ladders);
			}
			else {
				currentPlayer++;
			}
			break;
		case 0:
			SaveAndExitGame(playerCount, currentPlayer, players, snakes, ladders);
			break;
		default:
			printf("\nInavlid Input - Try again!\n");
		}

	} while (userChoice != 0);








}

int CheckVictory(int playerPos) {
	int victoryYN;
	if (playerPos >= 100) {
		victoryYN = 1;
	}
	else {
		victoryYN = 0;
	}
	return victoryYN;
}

int CheckSnakes(int position, sol_t snakes[]) {

	int pos = position;
	int i;
	for (i = 0;i < MAX;i++) {
		if (position == snakes[i].position) {
			printf("You landed on a Snake at Position: %d and slid down the board...\nNew Position %d\n",snakes[i].position,snakes[i].end);
			pos = snakes[i].end;

		}
		else {
			//printf("Test code : No Snake %d\n", i);
		}
	}
	return pos;
}

int CheckLadders(int position, sol_t ladders[]) {

	int pos = position;
	int i;
	for (i = 0;i < MAX;i++) {
		if (position == ladders[i].position) {
			printf("You landed on a Ladder at Position: %d and climbed up the board...\nNew Position %d\n", ladders[i].position, ladders[i].end);
			pos = ladders[i].end;

		}
		else {
			//printf("Test code : No Ladder %d\n", i);
		}
	}
	return pos;

}


void SaveAndExitGame(int playerCount, int currentPlayer, player_t *players, sol_t snakes[], sol_t ladders[]) {
	char fileSave[20] = "";
	int overWriteQ,overA;
	char files[20][20];
	FILE *reference = fopen("reference.txt", "r");
	int fileCount,i;

	char end[] = ".txt";

	FILE *file;

	if (reference != NULL) {
		fscanf(reference, "%d", &fileCount);
		for (i = 0;i < fileCount;i++) {
			fscanf(reference, "%s", files[i]);
		}

	}
	else {
		fileCount = 0;
	}




	printf("\n===========================================\n");
	printf("Saving Snakes and Ladders 3000!\n");
	printf("===========================================\n");
	printf("Would you Like to Save Before Quitting?\n");
	printf("%10s New Save File\n", "1)");
	printf("%10s Overwrite Existing File\n", "2)");
	printf("%10s Quit Without Saving\n", "0)");
	scanf("%d", &overWriteQ);
	switch (overWriteQ) {
	case 1:
		printf("Name The file: (one word only, please)");
		scanf("%s", fileSave);
		strcat(fileSave, end);
		break;
	case 2:
		if (fileCount > 0) {
			for (i = 0;i < fileCount;i++) {
				printf("%d) %s\n", (i + 1), files[i]);
			}
			do {
				printf("Choose a File to OverWrite: (based off the number assigned):\n");
				scanf("%d", &overA);
				overA--;
			} while (overA<0 || overA>fileCount);
			strcpy(fileSave, files[overA]);
			printf("You have chosen to overwrite %s\n", fileSave);
		}
		else {
			printf("Error, no files to overwrite!\n");
			return;
		}
		break;
		case 0:
			printf("quitting without saving\n");
			return;
		break;
		default:
			printf("Invalid Choice - Exiting without saving \n");
			break;
	}
	if(reference!=NULL)
	fclose(reference);

	CreateNewReferenceFile((fileCount), files, fileSave);
	file = fopen(fileSave, "w");

	fprintf(file, "%d\n", playerCount);
	fprintf(file, "%d\n", currentPlayer);
	for (i = 0;i < playerCount;i++) {

		fprintf(file, "%s %d\n", (players + i)->name, (players + i)->position);
	}
	for (i = 0;i < MAX;i++) {

		fprintf(file, "%d %d %d\n", (snakes + i)->position, (snakes + i)->length, (snakes + i)->end);
	}

	for (i = 0;i < MAX;i++) {

		fprintf(file, "%d %d %d\n", (ladders + i)->position, (ladders + i)->length, (ladders + i)->end);
	}
	fprintf(file,"EOF");
	fclose(file);


}
int LoadGame(int fileChoice, int *playerCount, int *currentPlayer, player_t *players, sol_t snakes[], sol_t ladders[]) {
	FILE *reference = fopen("reference.txt", "r");
	char fileName[20];
	FILE *file;

	int i;

	int fileCount;
	char files[20][20];
	if (reference != NULL) {
	fscanf(reference, "%d", &fileCount);
	for (i = 0;i < fileCount;i++) {
		fscanf(reference, "%s", files[i]);

	}
	fclose(reference);
	}
	else {
		printf("\nNo Files Currently Stored\n");
		return -1;
	}



	if (fileCount > 0) {

		strcpy(fileName, files[fileChoice]);
		printf("You have chosen to open %s\n", fileName);

		file = fopen(fileName, "r");
		printf("File Opened\n");


		if (file != NULL) {

			fscanf(file, "%d", playerCount);

			fscanf(file, "%d", currentPlayer);

			for (i = 0; i < *playerCount;i++) {

				fscanf(file, "%s", (players + i)->name);
				fscanf(file, "%d", &(players + i)->position);

			}
			ShowPlayerList(players, *playerCount);

			for (i = 0;i < MAX;i++) {

				fscanf(file, "%d %d %d", &(snakes + i)->position, &(snakes + i)->length, &(snakes + i)->end);

			}
			for (i = 0;i < MAX;i++) {

				fscanf(file, "%d %d %d", &(ladders + i)->position, &(ladders + i)->length, &(ladders + i)->end);

			}
		}
		fclose(file);
	}


	return 1;

}


int WhatFile() {


	char fileName[20];

	int i, count, userChoice;
	int fileChoice;
	int fileCount;
	char files[20][20];
	FILE *file;
	FILE *reference = fopen("reference.txt", "r");


	if (reference != NULL) {
		fscanf(reference, "%d", &fileCount);
		for (i = 0;i < fileCount;i++) {
			fscanf(reference, "%s", files[i]);
		}
		fclose(reference);
	}
	else {
		printf("\nNo Files Currently Stored\n");
		return -1;
	}


	for (i = 0;i <fileCount;i++) {
		printf("%d) %s\n", (i + 1), files[i]);
	}
	if (fileCount > 0) {
		do {
			printf("Choose a File to Open: (based off the number assigned):\n");
			scanf("%d", &fileChoice);
			fileChoice--;
		} while (fileChoice<0 || fileChoice>fileCount);
		strcpy(fileName, files[fileChoice]);



		file = fopen(fileName, "r");
		if (file != NULL) {
			printf("Accessing File...\n");

			fclose(file);
			return fileChoice;


		}
		else {
			printf("Error opening File");
			return -1;
		}
	}

}
int GetPlayerCount(int fileChoice) {

	char fileName[20];

	int i, count, userChoice;
	int fileCount;
	char files[20][20];
	FILE *file;
	FILE *reference = fopen("reference.txt", "r");



	if (reference != NULL) {
		fscanf(reference, "%d", &fileCount);
		for (i = 0;i < fileCount;i++) {
			fscanf(reference, "%s", files[i]);
		}
		fclose(reference);
	}
	else {
		printf("\nNo Files Currently Stored\n");
		return -1;
	}



		strcpy(fileName, files[fileChoice]);
		file = fopen(fileName, "r");
		if (file != NULL) {

			fscanf(file, "%d", &count);
			fclose(file);
			return count;

			fclose(file);
		}


}




int CheckFiles() {

	FILE *file;
	int fileCount=0;
	file = fopen("reference.txt", "r");
	if (file != NULL) {
		fscanf(file, "%d", &fileCount);
		fclose(file);
	}
	return fileCount;
}

void CreateNewReferenceFile(int fileCount, char files[20][20],char nFile[20]) {

	FILE *ref;
	int i;
	int sameQ,sameA;
	sameA = 0;


	removeFile("reference.txt");




	for (i = 0;i < fileCount; i++) {

		sameQ = strcmp(files[i], nFile);
		if (sameQ == 0) {
			sameA = 1;
		}
	}

	ref = fopen("reference.txt", "w");
	if (sameA!=1) {

		fprintf(ref, "%d", (fileCount+1));
	}
	else {
		fprintf(ref, "%d", fileCount);
	}



	for (i = 0;i < (fileCount);i++) {

		fprintf(ref, "\n%s",files[i]);
	}

	if (sameA != 1) {
		fprintf(ref, "\n%s", nFile);

	}

	fclose(ref);
	printf("\nReference file has been updated to reflect changes\n");
}

void removeFile(char fileName[]) {
	int i;
	i=remove(fileName);
	if (i == 0) {
		printf("\n%s File Deleted\n",fileName);
	}
	else {
		printf("\ncannot find  %s\n", fileName);
	}

}

void MenuRemoveFile() {
	char fileName[20];

	int i;
	int fileChoice;
	int fileCount;
	char files[20][20];

	FILE *reference = fopen("reference.txt", "r");



	if (reference != NULL) {
		fscanf(reference, "%d", &fileCount);
		for (i = 0;i < fileCount;i++) {
			fscanf(reference, "%s", files[i]);
		}
		fclose(reference);
	}
	else {
		printf("\nNo Files Currently Stored\n");
		return;
	}


	for (i = 0;i <fileCount;i++) {
		printf("%d) %s\n", (i + 1), files[i]);
	}
	if (fileCount > 0) {
		do {
			printf("Choose a File to Delete: (based off the number assigned):\n");
			scanf("%d", &fileChoice);
			fileChoice--;
		} while (fileChoice<0 || fileChoice>fileCount);
		strcpy(fileName, files[fileChoice]);
		printf("You have chosen to delete %s\n", fileName);
		removeFile(fileName);

	}


	removeFile("reference.txt");
	if ((fileCount - 1) > 0) {

		reference = fopen("reference.txt", "w");
		fprintf(reference, "%d", (fileCount - 1));
		for (i = 0;i < fileCount;i++) {
			if (strcmp(fileName, files[i]) == 0) {
				//do nothing
				//printf("No printy %s", fileName);
				//test code
			}
			else {
				//printf("\n%s printy yes! \n", files[i]);
				//test code to ensure working as intended

				fprintf(reference, "\n%s", files[i]);
			}
		}

		fclose(reference);
	}

}

