
// RyanGate 2.0

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>
# include <assert.h>
# include "RyanGate_files.h"

//Data Sturctures

//struct that stroes all the info of the players (name, number, position, arrays of pitch percentages and batting avg)
struct player{
	char lastname[20];
	int jerseyNumber;
	char position[5];
	float total[12];//array with total of every count 
	float pitch[4][12];//array with pitch occurence
	float avg[4][12];//array with batting average of every pitch on each count
	float avg_total[4][12];//array marking how many times a hit was happens each count
};

//needed to return balls and strikes in prediction function
struct predictionStruct 
{
	int ball;
	int strike;
	char pitch[15];
};

char opponent[100];//stores the opponents name

struct player *lineup[9];//array of pointers to struct

//Functions
void remove_PLAYER(); //MM remove a player for the lineup
void add_PLAYER(); //MM add player to empty slot in roster
void list_TEAM(); //MM lists all stats of the whole team
void list_PLAYER(); //MM lists all stats of single player
void list_STATS(struct player *);// prints stats of both arrays

void newGAME (); //MM starts new game 
void view_TEAM (); //NG quick way to view the team
void create_LINEUP(); //NG user enters lastname, number, and position
struct player *create_PLAYER (int); //NG -> CL creates player to going into the array

void selectBATTER();//NG allows the user to select batter to enter info
void selectBATTER_FASTBALL (int, int, int);//NG -> SB if pitch is a fastball
void selectBATTER_CURVEBALL ();//NG -> SB if pitch is a curve
void selectBATTER_SLIDER ();//NG -> SB if pitch is a slider
void selectBATTER_CHANGEUP ();//NG -> SB if pitch is a change
int result_ATBAT (int, int);
void hit (int, int, int, int);//results in a hit. adds to avg array
void out (int, int, int, int);//results in an out. incemeants the total but not avg

void prediction(int);//NG->SB likelihood of seeing a pitch on a count (Max)
struct predictionStruct *predictionCount (int, int);//returns balls and strikes for prediction function
void suggestion(int);// NG -> SB Highest pitch/success rate (Max) weigh the success rate more 1.5x aka look for this pitch
void documentation ();




int main (void){
	int switchNumber;// used in the switch

	printf("\n\n*** WELCOME TO RYANGATE 2.0 ***\n\n");
	
	while (1){
		printf("*** MAIN MENU ***\n\n SELECT:\n 1. NEW GAME\n 2. LIST TEAM STATS\n 3. LIST PLAYER STATS\n 4. DOCUMENTATION\n 5. QUIT\n");
		scanf("%d", &switchNumber);

		switch (switchNumber){
			case 1:
				newGAME ();// function that user can enter lineup, select batter
				break;

			case 2:
				list_TEAM();
				break;

			case 3:
				list_PLAYER();
				break;	

			case 4:
				documentation();
				break;

			default:
				return 0;			
		}


	}
}


//NEW GAME FUNCTION
void newGAME(){
	int switch_ADD;
	
	printf("\n *** NEW GAME MENU ***\n");
	view_TEAM ();
	printf("\n 1: ENTER LINEUP\n 2: SELECT BATTER\n 3: BACK TO THE MAIN MENU\n");
	scanf("%d", &switch_ADD);

	switch (switch_ADD){
		case 1:
			create_LINEUP ();
			break;

		case 2:	
			selectBATTER ();
			break;

		default:
			printf("\nBACKING UP...\n");
			return;	
	}
}

//creates array of structures
void create_LINEUP (struct player *temp_Player){
	int index_LINEUP;
	
	//loop to go through lineup array to add pointer and structs
	for (index_LINEUP = 0; index_LINEUP < 9; index_LINEUP++){
			lineup[index_LINEUP] = create_PLAYER(index_LINEUP);
	}	
	
	printf("\nLINEUP SUCCESSFULLY CREATED\n\n");
}

//creates the structures and returns a pointer to the structure
struct player *create_PLAYER(int createCounter){
	struct player *temp_Player;//temp pointer to structure
	char scanned_opponent[100];// string for opponent
	char scanned_lastname[20];//string scanned in
	int scanned_jnumber;// jersey number scanned in
	char scanned_position[5];// string scanned in
	temp_Player = malloc(sizeof(struct player)); // creates a struct for in the element
	assert(temp_Player != NULL);
	int pitchINDEX;// indexes to zero out
	int countINDEX;// indexes to zero out

	//Opponent
	printf("\nENTER THE NICKNAME OF THE TEAM YOU ARE PLAYING:\n");
	scanf("%s", scanned_opponent);
	strcpy(opponent, scanned_opponent);

	//lastName
	printf("ENTER THE LAST NAME OF BATTER #%d:\n", createCounter+1);
	scanf("%s", scanned_lastname);
	strcpy(temp_Player->lastname, scanned_lastname);

	//Number
	printf("ENTER JERSEY NUMBER OF %s:\n", scanned_lastname);
	scanf("%d", &scanned_jnumber);
	temp_Player->jerseyNumber = scanned_jnumber;

	//Position
	printf("ENTER POSITION OF  %s\n", scanned_lastname);
	scanf("%s", scanned_position);
	strcpy(temp_Player->position, scanned_position);

	// zeros arrays out
	for(pitchINDEX = 0; pitchINDEX < 4; pitchINDEX++){
		for(countINDEX = 0; countINDEX < 12; countINDEX++){
			temp_Player->pitch[pitchINDEX][countINDEX] = 0.0;
			temp_Player->total[countINDEX] = 0.0;
			temp_Player->avg[pitchINDEX][countINDEX] = 0.0;
			temp_Player->avg_total[pitchINDEX][countINDEX] = 0.0;
		}
	}
	
	return temp_Player;
}

// asks for batter then prints info
void list_PLAYER (){
	int scanned_listplayer;// scanned batter to list

	printf("\nWHICH BATTER'S STATS WOULD YOU LIKE TO VIEW?\n");
	scanf("%d", &scanned_listplayer);
	if(lineup[scanned_listplayer] == NULL)
		printf("THIS BATTER DOES NOT EXIST.\n");
	else{
		printf("Batter #%d: %s\n", scanned_listplayer, lineup[scanned_listplayer-1]->lastname);
		list_STATS (lineup[scanned_listplayer-1]);
	}
	return;
}

// formats and prints the info 
void list_STATS (struct player *pointerListStats){
	int countINDEX;

	printf("COUNT 0-0\n");
	printf("  PITCH PERCENTAGES:\n    FASTBALL: %.3f%% (%.0f/%.0f)\n    CURVEBALL: %.3f%% (%.0f/%.0f)\n    SLIDER: %.3f%% (%.0f/%.0f)\n    CHANGE-UP: %.3f%% (%.0f/%.0f)\n\n", pointerListStats->pitch[0][0] / pointerListStats->total[0], pointerListStats->pitch[0][0], pointerListStats->total[0], pointerListStats->pitch[1][0] / pointerListStats->total[0], pointerListStats->pitch[1][0], pointerListStats->total[0], pointerListStats->pitch[2][0] / pointerListStats->total[0], pointerListStats->pitch[2][0], pointerListStats->total[0], pointerListStats->pitch[3][0] / pointerListStats->total[0], pointerListStats->pitch[3][0], pointerListStats->total[0]);
	printf("  BATTING AVERAGE:\n    FASTBALL: %.3f%% (%.0f/%.0f)\n    CURVEBALL: %.3f%% (%.0f/%.0f)\n    SLIDER: %.3f%% (%.0f/%.0f)\n    CHANGE-UP: %.3f%% (%.0f/%.0f)\n\n", (pointerListStats->avg[0][0] / pointerListStats->avg_total[0][0]), pointerListStats->avg[0][0], pointerListStats->avg_total[0][0], (pointerListStats->avg[1][0] / pointerListStats->avg_total[1][0]), pointerListStats->avg[1][0], pointerListStats->avg_total[1][0], (pointerListStats->avg[2][0] / pointerListStats->avg_total[2][0]), pointerListStats->avg[2][0], pointerListStats->avg_total[3][0], (pointerListStats->avg[3][0] / pointerListStats->avg_total[3][0]), pointerListStats->avg[3][0], pointerListStats->avg_total[3][0]);	
	
	printf("COUNT 0-1\n");
	printf("  PITCH PERCENTAGES:\n    FASTBALL: %.3f%% (%.0f/%.0f)\n    CURVEBALL: %.3f%% (%.0f/%.0f)\n    SLIDER: %.3f%% (%.0f/%.0f)\n    CHANGE-UP: %.3f%% (%.0f/%.0f)\n\n", pointerListStats->pitch[0][1] / pointerListStats->total[1], pointerListStats->pitch[0][1], pointerListStats->total[1], pointerListStats->pitch[1][1] / pointerListStats->total[1], pointerListStats->pitch[1][1], pointerListStats->total[1], pointerListStats->pitch[2][1] / pointerListStats->total[1], pointerListStats->pitch[2][1], pointerListStats->total[1], pointerListStats->pitch[3][1] / pointerListStats->total[1], pointerListStats->pitch[3][1], pointerListStats->total[1]);
	printf("  BATTING AVERAGE:\n    FASTBALL: %.3f%% (%.0f/%.0f)\n    CURVEBALL: %.3f%% (%.0f/%.0f)\n    SLIDER: %.3f%% (%.0f/%.0f)\n    CHANGE-UP: %.3f%% (%.0f/%.0f)\n\n", pointerListStats->avg[0][1] / pointerListStats->avg_total[0][1], pointerListStats->avg[0][1], pointerListStats->avg_total[0][1], pointerListStats->avg[1][1] / pointerListStats->avg_total[1][1], pointerListStats->avg[1][1], pointerListStats->avg_total[1][1], pointerListStats->avg[2][1] / pointerListStats->avg_total[2][1], pointerListStats->avg[2][1], pointerListStats->avg_total[3][1], pointerListStats->avg[3][1] / pointerListStats->avg_total[3][1], pointerListStats->avg[3][1], pointerListStats->avg_total[3][1]);
	
	printf("COUNT 0-2\n");
	printf("  PITCH PERCENTAGES:\n    FASTBALL: %.3f%% (%.0f/%.0f)\n    CURVEBALL: %.3f%% (%.0f/%.0f)\n    SLIDER: %.3f%% (%.0f/%.0f)\n    CHANGE-UP: %.3f%% (%.0f/%.0f)\n\n", pointerListStats->pitch[0][2] / pointerListStats->total[2], pointerListStats->pitch[0][2], pointerListStats->total[2], pointerListStats->pitch[1][2] / pointerListStats->total[2], pointerListStats->pitch[1][2], pointerListStats->total[2], pointerListStats->pitch[2][2] / pointerListStats->total[2], pointerListStats->pitch[2][2], pointerListStats->total[2], pointerListStats->pitch[3][2] / pointerListStats->total[2], pointerListStats->pitch[3][2], pointerListStats->total[2]);
	printf("  BATTING AVERAGE:\n    FASTBALL: %.3f%% (%.0f/%.0f)\n    CURVEBALL: %.3f%% (%.0f/%.0f)\n    SLIDER: %.3f%% (%.0f/%.0f)\n    CHANGE-UP: %.3f%% (%.0f/%.0f)\n\n", pointerListStats->avg[0][2] / pointerListStats->avg_total[0][2], pointerListStats->avg[0][2], pointerListStats->avg_total[0][2], pointerListStats->avg[1][2] / pointerListStats->avg_total[1][2], pointerListStats->avg[1][2], pointerListStats->avg_total[1][2], pointerListStats->avg[2][2] / pointerListStats->avg_total[2][2], pointerListStats->avg[2][2], pointerListStats->avg_total[3][2], pointerListStats->avg[3][2] / pointerListStats->avg_total[3][2], pointerListStats->avg[3][2], pointerListStats->avg_total[3][2]);
	
	printf("COUNT 1-0\n");
	printf("  PITCH PERCENTAGES:\n    FASTBALL: %.3f%% (%.0f/%.0f)\n    CURVEBALL: %.3f%% (%.0f/%.0f)\n    SLIDER: %.3f%% (%.0f/%.0f)\n    CHANGE-UP: %.3f%% (%.0f/%.0f)\n\n", pointerListStats->pitch[0][3] / pointerListStats->total[3], pointerListStats->pitch[0][3], pointerListStats->total[3], pointerListStats->pitch[1][3] / pointerListStats->total[3], pointerListStats->pitch[1][3], pointerListStats->total[3], pointerListStats->pitch[2][3] / pointerListStats->total[3], pointerListStats->pitch[2][3], pointerListStats->total[3], pointerListStats->pitch[3][3] / pointerListStats->total[3], pointerListStats->pitch[3][3], pointerListStats->total[3]);
	printf("  BATTING AVERAGE:\n    FASTBALL: %.3f%% (%.0f/%.0f)\n    CURVEBALL: %.3f%% (%.0f/%.0f)\n    SLIDER: %.3f%% (%.0f/%.0f)\n    CHANGE-UP: %.3f%% (%.0f/%.0f)\n\n", pointerListStats->avg[0][3] / pointerListStats->avg_total[0][3], pointerListStats->avg[0][3], pointerListStats->avg_total[0][3], pointerListStats->avg[1][3] / pointerListStats->avg_total[1][3], pointerListStats->avg[1][3], pointerListStats->avg_total[1][3], pointerListStats->avg[2][3] / pointerListStats->avg_total[2][3], pointerListStats->avg[2][3], pointerListStats->avg_total[3][3], pointerListStats->avg[3][3] / pointerListStats->avg_total[3][3], pointerListStats->avg[3][3], pointerListStats->avg_total[3][3]);

	printf("COUNT 1-1\n");
	printf("  PITCH PERCENTAGES:\n    FASTBALL: %.3f%% (%.0f/%.0f)\n    CURVEBALL: %.3f%% (%.0f/%.0f)\n    SLIDER: %.3f%% (%.0f/%.0f)\n    CHANGE-UP: %.3f%% (%.0f/%.0f)\n\n", pointerListStats->pitch[0][4] / pointerListStats->total[4], pointerListStats->pitch[0][4], pointerListStats->total[4], pointerListStats->pitch[1][4] / pointerListStats->total[4], pointerListStats->pitch[1][4], pointerListStats->total[4], pointerListStats->pitch[2][4] / pointerListStats->total[4], pointerListStats->pitch[2][4], pointerListStats->total[4], pointerListStats->pitch[3][4] / pointerListStats->total[4], pointerListStats->pitch[3][4], pointerListStats->total[4]);
	printf("  BATTING AVERAGE:\n    FASTBALL: %.3f%% (%.0f/%.0f)\n    CURVEBALL: %.3f%% (%.0f/%.0f)\n    SLIDER: %.3f%% (%.0f/%.0f)\n    CHANGE-UP: %.3f%% (%.0f/%.0f)\n\n", pointerListStats->avg[0][4] / pointerListStats->avg_total[0][4], pointerListStats->avg[0][4], pointerListStats->avg_total[0][4], pointerListStats->avg[1][4] / pointerListStats->avg_total[1][4], pointerListStats->avg[1][4], pointerListStats->avg_total[1][4], pointerListStats->avg[2][4] / pointerListStats->avg_total[2][4], pointerListStats->avg[2][4], pointerListStats->avg_total[3][4], pointerListStats->avg[3][4] / pointerListStats->avg_total[3][4], pointerListStats->avg[3][4], pointerListStats->avg_total[3][4]);
	
	printf("COUNT 1-2\n");
	printf("  PITCH PERCENTAGES:\n    FASTBALL: %.3f%% (%.0f/%.0f)\n    CURVEBALL: %.3f%% (%.0f/%.0f)\n    SLIDER: %.3f%% (%.0f/%.0f)\n    CHANGE-UP: %.3f%% (%.0f/%.0f)\n\n", pointerListStats->pitch[0][5] / pointerListStats->total[5], pointerListStats->pitch[0][5], pointerListStats->total[5], pointerListStats->pitch[1][5] / pointerListStats->total[5], pointerListStats->pitch[1][5], pointerListStats->total[5], pointerListStats->pitch[2][5] / pointerListStats->total[5], pointerListStats->pitch[2][5], pointerListStats->total[5], pointerListStats->pitch[3][5] / pointerListStats->total[5], pointerListStats->pitch[3][5], pointerListStats->total[5]);
	printf("  BATTING AVERAGE:\n    FASTBALL: %.3f%% (%.0f/%.0f)\n    CURVEBALL: %.3f%% (%.0f/%.0f)\n    SLIDER: %.3f%% (%.0f/%.0f)\n    CHANGE-UP: %.3f%% (%.0f/%.0f)\n\n", pointerListStats->avg[0][5] / pointerListStats->avg_total[0][5], pointerListStats->avg[0][5], pointerListStats->avg_total[0][5], pointerListStats->avg[1][5] / pointerListStats->avg_total[1][5], pointerListStats->avg[1][5], pointerListStats->avg_total[1][5], pointerListStats->avg[2][5] / pointerListStats->avg_total[2][5], pointerListStats->avg[2][5], pointerListStats->avg_total[3][5], pointerListStats->avg[3][5] / pointerListStats->avg_total[3][5], pointerListStats->avg[3][5], pointerListStats->avg_total[3][5]);
	
	printf("COUNT 2-0\n");
	printf("  PITCH PERCENTAGES:\n    FASTBALL: %.3f%% (%.0f/%.0f)\n    CURVEBALL: %.3f%% (%.0f/%.0f)\n    SLIDER: %.3f%% (%.0f/%.0f)\n    CHANGE-UP: %.3f%% (%.0f/%.0f)\n\n", pointerListStats->pitch[0][6] / pointerListStats->total[6], pointerListStats->pitch[0][6], pointerListStats->total[6], pointerListStats->pitch[1][6] / pointerListStats->total[6], pointerListStats->pitch[1][6], pointerListStats->total[6], pointerListStats->pitch[2][6] / pointerListStats->total[6], pointerListStats->pitch[2][6], pointerListStats->total[6], pointerListStats->pitch[3][6] / pointerListStats->total[6], pointerListStats->pitch[3][6], pointerListStats->total[6]);
	printf("  BATTING AVERAGE:\n    FASTBALL: %.3f%% (%.0f/%.0f)\n    CURVEBALL: %.3f%% (%.0f/%.0f)\n    SLIDER: %.3f%% (%.0f/%.0f)\n    CHANGE-UP: %.3f%% (%.0f/%.0f)\n\n", pointerListStats->avg[0][6] / pointerListStats->avg_total[0][6], pointerListStats->avg[0][6], pointerListStats->avg_total[0][6], pointerListStats->avg[1][6] / pointerListStats->avg_total[1][6], pointerListStats->avg[1][6], pointerListStats->avg_total[1][6], pointerListStats->avg[2][6] / pointerListStats->avg_total[2][6], pointerListStats->avg[2][6], pointerListStats->avg_total[3][6], pointerListStats->avg[3][6] / pointerListStats->avg_total[3][6], pointerListStats->avg[3][6], pointerListStats->avg_total[3][6]);

	printf("COUNT 2-1\n");
	printf("  PITCH PERCENTAGES:\n    FASTBALL: %.3f%% (%.0f/%.0f)\n    CURVEBALL: %.3f%% (%.0f/%.0f)\n    SLIDER: %.3f%% (%.0f/%.0f)\n    CHANGE-UP: %.3f%% (%.0f/%.0f)\n\n", pointerListStats->pitch[0][7] / pointerListStats->total[7], pointerListStats->pitch[0][7], pointerListStats->total[7], pointerListStats->pitch[1][7] / pointerListStats->total[7], pointerListStats->pitch[1][7], pointerListStats->total[7], pointerListStats->pitch[2][7] / pointerListStats->total[7], pointerListStats->pitch[2][7], pointerListStats->total[7], pointerListStats->pitch[3][7] / pointerListStats->total[7], pointerListStats->pitch[3][7], pointerListStats->total[7]);
	printf("  BATTING AVERAGE:\n    FASTBALL: %.3f%% (%.0f/%.0f)\n    CURVEBALL: %.3f%% (%.0f/%.0f)\n    SLIDER: %.3f%% (%.0f/%.0f)\n    CHANGE-UP: %.3f%% (%.0f/%.0f)\n\n", pointerListStats->avg[0][7] / pointerListStats->avg_total[0][7], pointerListStats->avg[0][7], pointerListStats->avg_total[0][7], pointerListStats->avg[1][7] / pointerListStats->avg_total[1][7], pointerListStats->avg[1][7], pointerListStats->avg_total[1][7], pointerListStats->avg[2][7] / pointerListStats->avg_total[2][7], pointerListStats->avg[2][7], pointerListStats->avg_total[3][7], pointerListStats->avg[3][7] / pointerListStats->avg_total[3][7], pointerListStats->avg[3][7], pointerListStats->avg_total[3][7]);
	
	printf("COUNT 2-2\n");
	printf("  PITCH PERCENTAGES:\n    FASTBALL: %.3f%% (%.0f/%.0f)\n    CURVEBALL: %.3f%% (%.0f/%.0f)\n    SLIDER: %.3f%% (%.0f/%.0f)\n    CHANGE-UP: %.3f%% (%.0f/%.0f)\n\n", pointerListStats->pitch[0][8] / pointerListStats->total[8], pointerListStats->pitch[0][8], pointerListStats->total[8], pointerListStats->pitch[1][8] / pointerListStats->total[8], pointerListStats->pitch[1][8], pointerListStats->total[8], pointerListStats->pitch[2][8] / pointerListStats->total[8], pointerListStats->pitch[2][8], pointerListStats->total[8], pointerListStats->pitch[3][8] / pointerListStats->total[8], pointerListStats->pitch[3][8], pointerListStats->total[8]);
	printf("  BATTING AVERAGE:\n    FASTBALL: %.3f%% (%.0f/%.0f)\n    CURVEBALL: %.3f%% (%.0f/%.0f)\n    SLIDER: %.3f%% (%.0f/%.0f)\n    CHANGE-UP: %.3f%% (%.0f/%.0f)\n\n", pointerListStats->avg[0][8] / pointerListStats->avg_total[0][8], pointerListStats->avg[0][8], pointerListStats->avg_total[0][8], pointerListStats->avg[1][8] / pointerListStats->avg_total[1][8], pointerListStats->avg[1][8], pointerListStats->avg_total[1][8], pointerListStats->avg[2][8] / pointerListStats->avg_total[2][8], pointerListStats->avg[2][8], pointerListStats->avg_total[3][8], pointerListStats->avg[3][8] / pointerListStats->avg_total[3][8], pointerListStats->avg[3][8], pointerListStats->avg_total[3][8]);
	
	printf("COUNT 3-0\n");
	printf("  PITCH PERCENTAGES:\n    FASTBALL: %.3f%% (%.0f/%.0f)\n    CURVEBALL: %.3f%% (%.0f/%.0f)\n    SLIDER: %.3f%% (%.0f/%.0f)\n    CHANGE-UP: %.3f%% (%.0f/%.0f)\n\n", pointerListStats->pitch[0][9] / pointerListStats->total[9], pointerListStats->pitch[0][9], pointerListStats->total[9], pointerListStats->pitch[1][9] / pointerListStats->total[9], pointerListStats->pitch[1][9], pointerListStats->total[9], pointerListStats->pitch[2][9] / pointerListStats->total[9], pointerListStats->pitch[2][9], pointerListStats->total[9], pointerListStats->pitch[3][9] / pointerListStats->total[9], pointerListStats->pitch[3][9], pointerListStats->total[9]);
	printf("  BATTING AVERAGE:\n    FASTBALL: %.3f%% (%.0f/%.0f)\n    CURVEBALL: %.3f%% (%.0f/%.0f)\n    SLIDER: %.3f%% (%.0f/%.0f)\n    CHANGE-UP: %.3f%% (%.0f/%.0f)\n\n", pointerListStats->avg[0][9] / pointerListStats->avg_total[0][9], pointerListStats->avg[0][9], pointerListStats->avg_total[0][9], pointerListStats->avg[1][9] / pointerListStats->avg_total[1][9], pointerListStats->avg[1][9], pointerListStats->avg_total[1][9], pointerListStats->avg[2][9] / pointerListStats->avg_total[2][9], pointerListStats->avg[2][9], pointerListStats->avg_total[3][9], pointerListStats->avg[3][9] / pointerListStats->avg_total[3][9], pointerListStats->avg[3][9], pointerListStats->avg_total[3][9]);
	
	printf("COUNT 3-1\n");
	printf("  PITCH PERCENTAGES:\n    FASTBALL: %.3f%% (%.0f/%.0f)\n    CURVEBALL: %.3f%% (%.0f/%.0f)\n    SLIDER: %.3f%% (%.0f/%.0f)\n    CHANGE-UP: %.3f%% (%.0f/%.0f)\n\n", pointerListStats->pitch[0][10] / pointerListStats->total[10], pointerListStats->pitch[0][10], pointerListStats->total[10], pointerListStats->pitch[1][10] / pointerListStats->total[10], pointerListStats->pitch[1][10], pointerListStats->total[10], pointerListStats->pitch[2][10] / pointerListStats->total[10], pointerListStats->pitch[2][10], pointerListStats->total[10], pointerListStats->pitch[3][10] / pointerListStats->total[10], pointerListStats->pitch[3][10], pointerListStats->total[10]);
	printf("  BATTING AVERAGE:\n    FASTBALL: %.3f%% (%.0f/%.0f)\n    CURVEBALL: %.3f%% (%.0f/%.0f)\n    SLIDER: %.3f%% (%.0f/%.0f)\n    CHANGE-UP: %.3f%% (%.0f/%.0f)\n\n", pointerListStats->avg[0][10] / pointerListStats->avg_total[0][10], pointerListStats->avg[0][10], pointerListStats->avg_total[0][10], pointerListStats->avg[1][10] / pointerListStats->avg_total[1][10], pointerListStats->avg[1][10], pointerListStats->avg_total[1][10], pointerListStats->avg[2][10] / pointerListStats->avg_total[2][10], pointerListStats->avg[2][10], pointerListStats->avg_total[3][10], pointerListStats->avg[3][10] / pointerListStats->avg_total[3][10], pointerListStats->avg[3][10], pointerListStats->avg_total[3][10]);
	
	printf("COUNT 3-2\n");
	printf("  PITCH PERCENTAGES:\n    FASTBALL: %.3f%% (%.0f/%.0f)\n    CURVEBALL: %.3f%% (%.0f/%.0f)\n    SLIDER: %.3f%% (%.0f/%.0f)\n    CHANGE-UP: %.3f%% (%.0f/%.0f)\n\n", pointerListStats->pitch[0][11] / pointerListStats->total[11], pointerListStats->pitch[0][11], pointerListStats->total[11], pointerListStats->pitch[1][11] / pointerListStats->total[11], pointerListStats->pitch[1][11], pointerListStats->total[11], pointerListStats->pitch[2][11] / pointerListStats->total[11], pointerListStats->pitch[2][11], pointerListStats->total[11], pointerListStats->pitch[3][11] / pointerListStats->total[11], pointerListStats->pitch[3][11], pointerListStats->total[11]);
	printf("  BATTING AVERAGE:\n    FASTBALL: %.3f%% (%.0f/%.0f)\n    CURVEBALL: %.3f%% (%.0f/%.0f)\n    SLIDER: %.3f%% (%.0f/%.0f)\n    CHANGE-UP: %.3f%% (%.0f/%.0f)\n\n", pointerListStats->avg[0][11] / pointerListStats->avg_total[0][11], pointerListStats->avg[0][11], pointerListStats->avg_total[0][11], pointerListStats->avg[1][11] / pointerListStats->avg_total[1][11], pointerListStats->avg[1][11], pointerListStats->avg_total[1][11], pointerListStats->avg[2][11] / pointerListStats->avg_total[2][11], pointerListStats->avg[2][11], pointerListStats->avg_total[3][11], pointerListStats->avg[3][11] / pointerListStats->avg_total[3][11], pointerListStats->avg[3][11], pointerListStats->avg_total[3][11]);
	
	return;
}

// prints the lineup for New Game page
void view_TEAM (){
	int view_INDEX; 

	if (lineup[0] == NULL)
			printf("Warning: The lineup is empty. Enter a lineup to continue.\n");
	else{
		printf("\nOPPONENT: %s\n", opponent);
		printf("CURRENT LINEUP\n\n");
		for( view_INDEX = 0; view_INDEX < 9; view_INDEX++){
			printf("%d. %s #%d %s\n", view_INDEX+1, lineup[view_INDEX]->position, lineup[view_INDEX]->jerseyNumber, lineup[view_INDEX]->lastname);
		}
	}	
	return;
}

//prints the full teams stats
void list_TEAM (){
	int listteam_INDEX; 

	if (lineup[0] == NULL)
		printf("\nTHE LINEUP IS EMPTY. PLEASE ENTER A LINEUP.\n");
		
	else{
		printf("\n*** CURRENT LINEUP ***\n\n");
		//traverses lineup array to print all stats
		for( listteam_INDEX = 0; listteam_INDEX < 9; listteam_INDEX++){
			printf("\n\n BATTING #%d: %s #%d %s\n\n", listteam_INDEX+1, lineup[listteam_INDEX]->position, lineup[listteam_INDEX]->jerseyNumber, lineup[listteam_INDEX]->lastname);
			list_STATS (lineup[listteam_INDEX]);
		}
	}	
	return;
}

void selectBATTER (){
	int scanned_selectbatter;// batter at bat
	int scanned_pitch;// pitch that was thrown
	int counter_strike = 0;
	int counter_balls = 0;
	int switch_selectbatter;
	int result_return;
	int flag_selectBATTER = 0;

	printf("\n*** SELECT BATTER ***\n");// header
	printf("\nENTER THE BATTER AT THE PLATE\n");
	scanf("%d", &scanned_selectbatter);
	printf("\n*** AT BAT ***\n");
	prediction (scanned_selectbatter);// prints prediction
	suggestion (scanned_selectbatter);// prints suggestion
	
	while (flag_selectBATTER != 1){
	printf("\nBatting: %s #%d %s\nCount: %d-%d\n\nENTER PITCH THROWN:\n1. FASTBALL\n2. CURVEBALL\n3. SLIDER\n4. CHANGE-UP\n5. BACK\n", lineup[scanned_selectbatter-1]->position, lineup[scanned_selectbatter-1]->jerseyNumber, lineup[scanned_selectbatter-1]->lastname, counter_balls, counter_strike);
	scanf("%d", &switch_selectbatter);

		switch (switch_selectbatter){
				case 1:
					selectBATTER_FASTBALL (scanned_selectbatter, counter_balls, counter_strike);
					result_return = result_ATBAT(counter_balls, counter_strike);
					if (result_return == 1){
						counter_balls++;
						break;
					}	
					if (result_return == 2){
						counter_strike++;
						break;
					}
					if (result_return == 3){
						hit (scanned_selectbatter, counter_balls, counter_strike, 0);
						flag_selectBATTER = 1;
						newGAME ();
					}
					if (result_return == 4){
						out (scanned_selectbatter, counter_balls, counter_strike, 0);
						flag_selectBATTER = 1;
						newGAME ();
					}	
					else
						flag_selectBATTER = 1;
						break;
				
				case 2:
					selectBATTER_CURVEBALL (scanned_selectbatter, counter_balls, counter_strike);
					result_return = result_ATBAT(counter_balls, counter_strike);
					if (result_return == 1){
						counter_balls++;
						break;
					}	
					if (result_return == 2){
						counter_strike++;
						break;
					}
					if (result_return == 3){
						hit (scanned_selectbatter, counter_balls, counter_strike, 1);
						flag_selectBATTER = 1;
						newGAME ();
					}
					if (result_return == 4){
						out (scanned_selectbatter, counter_balls, counter_strike, 1);
						flag_selectBATTER = 1;
						newGAME ();
					}	
					else
						flag_selectBATTER = 1;
						break;

				case 3:
					selectBATTER_SLIDER (scanned_selectbatter, counter_balls, counter_strike);
					result_return = result_ATBAT(counter_balls, counter_strike);
					if (result_return == 1){
						counter_balls++;
						break;
					}	
					if (result_return == 2){
						counter_strike++;
						break;
					}
					if (result_return == 3){
						hit (scanned_selectbatter, counter_balls, counter_strike, 2);
						flag_selectBATTER = 1;
						newGAME ();
					}	
					if (result_return == 4){
						out (scanned_selectbatter, counter_balls, counter_strike, 2);
						flag_selectBATTER = 1;
						newGAME ();
					}
					else
						flag_selectBATTER = 1;
						break;

				case 4:
					selectBATTER_CHANGEUP (scanned_selectbatter, counter_balls, counter_strike);
					result_return = result_ATBAT(counter_balls, counter_strike);
					if (result_return == 1){
						counter_balls++;
						break;
					}	
					if (result_return == 2){
						counter_strike++;
						break;
					}	
					if (result_return == 3){
						hit (scanned_selectbatter, counter_balls, counter_strike, 3);
						flag_selectBATTER = 1;
						newGAME ();
					}
					if (result_return == 4){
						out (scanned_selectbatter, counter_balls, counter_strike, 3);
						flag_selectBATTER = 1;
						newGAME ();
					}
					else
						flag_selectBATTER = 1;
						break;

				case 5:
					newGAME ();
					flag_selectBATTER = 1;
					break;

				default:
					return;
						
		}
	}
}

void selectBATTER_FASTBALL (int batter, int ball, int strike){
	if( ball == 0){
		((lineup[batter-1]->pitch[0][strike])++);
		((lineup[batter-1]->total[strike])++);
		return;
	}

	if (ball == 1){
		((lineup[batter-1]->pitch[0][strike + 3])++);
		((lineup[batter-1]->total[strike + 3])++);
		return;
	}

	if (ball == 2){
		((lineup[batter-1]->pitch[0][strike + 6])++);
		((lineup[batter-1]->total[strike + 6])++);
		return;
	}

	if (ball == 3){
		((lineup[batter-1]->pitch[0][strike + 9])++);
		((lineup[batter-1]->total[strike + 9])++);
		return;
	}
	
	else
		printf("Invalid count\n");
		return;
}

void selectBATTER_CURVEBALL (int batter, int ball, int strike){
	if( ball == 0){
		((lineup[batter-1]->pitch[1][strike])++);
		((lineup[batter-1]->total[strike])++);
		return;
	}

	if (ball == 1){
		((lineup[batter-1]->pitch[1][strike+3])++);
		((lineup[batter-1]->total[strike + 3])++);
		return;
	}

	if (ball == 2){
		((lineup[batter-1]->pitch[1][strike + 6])++);
		((lineup[batter-1]->total[strike + 6])++);
		return;
	}

	if (ball == 3){
		((lineup[batter-1]->pitch[1][strike + 9])++);
		((lineup[batter-1]->total[strike + 9])++);
		return;
	}
	
	else
		printf("Invalid count\n");
		return;
}

void selectBATTER_SLIDER (int batter, int ball, int strike){
	if( ball == 0){
		((lineup[batter-1]->pitch[2][strike])++);
		((lineup[batter-1]->total[strike])++);
		return;
	}

	if (ball == 1){
		((lineup[batter-1]->pitch[2][strike + 3])++);
		((lineup[batter-1]->total[strike + 3])++);
		return;
	}

	if (ball == 2){
		((lineup[batter-1]->pitch[2][strike + 6])++);
		((lineup[batter-1]->total[strike + 6])++);
		return;
	}

	if (ball == 3){
		((lineup[batter-1]->pitch[2][strike + 9])++);
		((lineup[batter-1]->total[strike + 9])++);
		return;
	}
	
	else
		printf("Invalid count\n");
		return;
}

void selectBATTER_CHANGEUP (int batter, int ball, int strike){
	if( ball == 0){
		((lineup[batter-1]->pitch[3][strike])++);
		((lineup[batter-1]->total[strike])++);
		return;
	}

	if (ball == 1){
		((lineup[batter-1]->pitch[3][strike + 3])++);
		((lineup[batter-1]->total[strike + 3])++);
		return;
	}

	if (ball == 2){
		((lineup[batter-1]->pitch[3][strike + 6])++);
		((lineup[batter-1]->total[strike + 6])++);
		return;
	}

	if (ball == 3){
		((lineup[batter-1]->pitch[3][strike + 9])++);
		((lineup[batter-1]->total[strike + 9])++);
		return;
	}
	
	else
		printf("Invalid count\n");
		return;
}

int result_ATBAT (int ball, int strike){
	int switch_result;
	int flag_result = 0;

	printf("\n*** RESULT OF AT BAT ***\n\n1. Ball\n2. Strike\n3. Hit\n4. Out\n");
	scanf("%d", &switch_result);

	switch (switch_result){
		case 1:
			flag_result = 1;
			break;

		case 2:
			flag_result = 2;
			break;

		case 3:
			flag_result = 3;
			break;

		case 4:
			flag_result = 4;
			break;

		default:
			return 0;

	}

	if(flag_result == 1)
		return 1;
	if(flag_result == 2)
		return 2;
	if(flag_result == 3)
		return 3;
	if(flag_result == 4)
		return 4;
	else
		return 0;

}

void hit (int batter, int ball, int strike, int pitch){
	if( ball == 0){
		((lineup[batter-1]->avg[pitch][strike])++);
		((lineup[batter-1]->avg_total[pitch][strike])++);
		return;
	}

	if (ball == 1){
		((lineup[batter-1]->avg[pitch][strike + 3])++);
		((lineup[batter-1]->avg_total[pitch][strike + 3])++);
		return;
	}

	if (ball == 2){
		((lineup[batter-1]->avg[pitch][strike + 6])++);
		((lineup[batter-1]->avg_total[pitch][strike + 6])++);
		return;
	}

	if (ball == 3){
		((lineup[batter-1]->avg[pitch][strike + 9])++);
		((lineup[batter-1]->avg_total[pitch][strike + 9])++);
		return;
	}
	
	else
		printf("ERROR: (Hit function) Invalid count\n");
		return;
}

void out (int batter, int ball, int strike, int pitch){
	if( ball == 0){
		((lineup[batter-1]->avg_total[pitch][strike])++);
		return;
	}

	if (ball == 1){
		((lineup[batter-1]->avg_total[pitch][strike + 3])++);
		return;
	}

	if (ball == 2){
		((lineup[batter-1]->avg_total[pitch][strike + 6])++);
		return;
	}

	if (ball == 3){
		((lineup[batter-1]->avg_total[pitch][strike + 9])++);
		return;
	}
	
	else
		printf("ERROR: (out function) Invalid count\n");
		return;

}
void prediction (int batter){
	float max;// highest percentage / pitch probability 
	int maxpitch = 0;// location for max
	int maxcount = 0;// location for max
	int countINDEX;// index
	int pitchINDEX;// index
	struct predictionStruct *count;// struct for formatting pitches and count


	if(lineup[batter-1]->total[0] == 0){
		printf("\nPrediction Error: No data for prediction.\n");
		return;
	}

	else{
		printf("PREDICTION:\n");
		max = (lineup[batter-1]->pitch[0][0] / lineup[batter-1]->total[0]);
		//find the max
		for(countINDEX = 0; countINDEX < 12; countINDEX++){
			if(lineup[batter-1]->total[countINDEX] == 0)//count has not occurred so continue on the loop
				continue;
			else{	
				for(pitchINDEX = 0; pitchINDEX < 4; pitchINDEX++){
					if((lineup[batter-1]->pitch[pitchINDEX][countINDEX] / lineup[batter-1]->total[countINDEX]) > max){
						max = (lineup[batter-1]->pitch[pitchINDEX][countINDEX] / lineup[batter-1]->total[countINDEX]);
					}
				}	
			}
		}
		
		//loop to check for duplicates
		for(countINDEX = 0; countINDEX < 12; countINDEX++){
			if(lineup[batter-1]->total[countINDEX] == 0){//count has not occurred so continue on the loop
				continue;
			}

			else{
				//loop to go through the pitches to find the max
				for(pitchINDEX = 0; pitchINDEX < 4; pitchINDEX++){
					if((lineup[batter-1]->pitch[pitchINDEX][countINDEX] / lineup[batter-1]->total[countINDEX]) == max){
						max = (lineup[batter-1]->pitch[pitchINDEX][countINDEX] / lineup[batter-1]->total[countINDEX]);
						maxcount = countINDEX;
						maxpitch = pitchINDEX;
						count = predictionCount(maxcount, maxpitch);
						
						if(max > .750)
							printf("%s is very likely to see a %d-%d %s (%.3f%%)\n", lineup[batter-1]->lastname, count->ball, count->strike, count->pitch, max);
											
						else if(max > .500 && max <= .750)
							printf("%s is likely to see a %d-%d %s (%.3f%%)\n", lineup[batter-1]->lastname, count->ball, count->strike, count->pitch, max);
												
						else if(max > .250 && max <= .500)
							printf("%s is fairly likely to see a %d-%d %s (%.3f%%)\n", lineup[batter-1]->lastname, count->ball, count->strike, count->pitch, max);
								
						else
							printf("ERROR: Invalid percentage\n");
								}	
					else
						continue;
				}	
			}
		}
	}
}



void suggestion (int batter){
	float max;// highest percentage / pitch probability 
	int maxpitch = 0;// location for max
	int maxcount = 0;// location for max
	int countINDEX;// index
	int pitchINDEX;// index
	float suggestionValue;
	struct predictionStruct *count;// struct for formatting pitches and count


	if(lineup[batter-1]->total[0] == 0){
		printf("Suggestion Error: No data for suggestion.\n");
		return;
	}

	else{
		printf("SUGGESTION:\n");
		max = (lineup[batter-1]->pitch[0][0] / lineup[batter-1]->total[0]) + ((lineup[batter-1]->avg[0][0] / lineup[batter-1]->avg_total[0][0]) * 1.25);// algo of the suggestion value

		//finds the max
		for(pitchINDEX = 0; pitchINDEX < 4; pitchINDEX++){	
			for(countINDEX = 0; countINDEX < 12; countINDEX++){
				if(lineup[batter-1]->avg_total[pitchINDEX][countINDEX] == 0)//count has not occurred so continue on the loop
					continue;
				else{	
					if((lineup[batter-1]->pitch[pitchINDEX][countINDEX] / lineup[batter-1]->total[countINDEX]) + ((lineup[batter-1]->avg[pitchINDEX][countINDEX] / lineup[batter-1]->avg_total[pitchINDEX][countINDEX]) * 1.25) > max)
						max = (lineup[batter-1]->pitch[pitchINDEX][countINDEX] / lineup[batter-1]->total[countINDEX]) + ((lineup[batter-1]->avg[pitchINDEX][countINDEX] / lineup[batter-1]->avg_total[pitchINDEX][countINDEX]) * 1.25);				
				}	
			}
		}
		
		//find any dups of the max
		for(pitchINDEX = 0; pitchINDEX < 4; pitchINDEX++){
			for(countINDEX = 0; countINDEX < 12; countINDEX++){
				if(lineup[batter-1]->avg_total[pitchINDEX][countINDEX] == 0)//count has not occurred so continue on the loop
					continue;	
				else{	
					suggestionValue = ((lineup[batter-1]->pitch[pitchINDEX][countINDEX] / lineup[batter-1]->total[countINDEX]) + ((lineup[batter-1]->avg[pitchINDEX][countINDEX] / lineup[batter-1]->avg_total[pitchINDEX][countINDEX]) * 1.25));
					if(suggestionValue == max){
						maxpitch = pitchINDEX;
						maxcount = countINDEX;
						count = predictionCount(maxcount, maxpitch); //gets me the counts and pitch
						printf("Look for a %s on %d-%d because you are batting %.3f on that pitch.\n", count->pitch, count->ball, count->strike, (lineup[batter-1]->avg[maxpitch][maxcount] / lineup[batter-1]->avg_total[maxpitch][maxcount]));	
					}
				}	
			}
		}
	}
}	

// gets the count and pitch of the maxes
struct predictionStruct *predictionCount (int maxcount, int maxpitch){	

	struct predictionStruct *temp_struct;
	temp_struct = malloc(sizeof(struct predictionStruct));

	// figuring out the count
	if(maxcount < 3){
		temp_struct->ball = 0;
		temp_struct->strike = maxcount;
	}

	else if(maxcount < 6 && maxcount >= 3){
		temp_struct->ball = 1;
		temp_struct->strike = (maxcount - 3);
	}

	else if(maxcount < 9 && maxcount >= 6){
		temp_struct->ball = 2;
		temp_struct->strike = (maxcount - 6);
	}
			
	else if(maxcount > 9){
		temp_struct->ball = 3;
		temp_struct->strike = (maxcount - 9);
	}
	else{
		printf("ERROR: Invalid count.\n");
	}	

	//figureing out which pitch it is
	if(maxpitch == 0){
		strcpy(temp_struct->pitch, "FASTBALL");
	}
	
	else if(maxpitch == 1){
		strcpy(temp_struct->pitch, "CURVEBALL");
	}
	
	else if(maxpitch == 2){
		strcpy(temp_struct->pitch, "SLIDER");
	}
	
	else if(maxpitch == 3){
		strcpy(temp_struct->pitch, "CHANGE-UP");
	}
	
	else{
		printf("ERROR: Invalid pitch.\n");
	}
	
	return temp_struct;
}

void documentation (){

	printf("*** RyanGate 2.0 ***\n\n\n 	New Game: New Game is where the user can enter a lineup and enter data.\n 		Create Lineup: This is where the user enters the name, number, and position of each the players in the lineup\n 		Select Batter: This is where the user enters data; which pitches are thrown and the result of those at-bats. This is also where the user can view suggestions and predictions the programs makes.\n\n 	List Team Stats: This is where the user can view the whole team's data, pitch percentages and batting averages on each count.\n\n 	List Player Stats: This is where the user can view the data of a select player, pitch percentages and batting averages on each count\n\n");
}


