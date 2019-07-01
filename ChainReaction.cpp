//HEADER FILES
#include <fstream.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dos.h>
#include <ctype.h>

//FUNCTION FOR GENERATING AND DISPLAYING GRID ON WHICH GAME IS PLAYED

void grid()
{
	clrscr();
	int absg, ordg;	//USED AS INDICES OF ARRAY
	char grid[13][25]; //ARRAY OF 13x25 IS USED TO CREATE THE GRID OF 4x6
	for (absg = 0; absg < 13; absg++)
	{
		for (ordg = 0; ordg < 25; ordg++)
		{
			grid[absg][ordg] = ' ';
		}
	}
	for (absg = 0; absg < 13; absg = absg + 3)
	{
		for (ordg = 0; ordg < 25; ordg++)
		{
			grid[absg][ordg] = '_';
		}
	}
	for (ordg = 0; ordg < 25; ordg = ordg + 4)
	{
		for (absg = 0; absg < 13; absg++)
		{
			grid[absg][ordg] = '|';
		}
	}
	for (ordg = 0, absg = 0; ordg < 25; ordg++)
	{
		if (ordg % 4 == 0)
		{
			grid[absg][ordg] = ' ';
		}
	}
	for (absg = 0; absg < 13; absg++) //DISPLAY GRID
	{
		for (ordg = 0; ordg < 25; ordg++)
		{
			textcolor(WHITE);
			gotoxy(ordg + 29, absg + 6);
			putch(grid[absg][ordg]);
		}
		cout << endl;
	}
}

//GLOBAL VARIABLES

fstream f1;				 //FOR HANDLING FILE
int color1, color2, sop; //CONTROL VARIABLES FOR HOLDING COLOR AND SOUND SETTING SAVED IN THE SETTING.DAT FILE

//CLASS PLAYER USED TO STORE DETAILS ABOUT SPOTS OCCUPIED BY ORBS, POSITION OF CURSOR, TURN AND WINNING PLAYER WHILE PLAYING

class player
{
public:
	int player1[6][4], //store orbs of player 1
		player2[6][4], //store orbs of player 2
		win1,		   //stores true if player 1 has won
		win2,		   //stores true if player 2 has won
		rowp,		   //stores row in which cursor is
		colp,		   //stores column in which curson is
		turn;		   //stores player's turn
} P;

//CLASS SETTINGS TO HOLD COLOR AND SOUND SETTINGS

class settings
{
public:
	int color1, //color chosen by player 1
		color2, //color chosen by player 2
		sop;	//sound on/off
} S;

//CLASS RECORDS TO HOLD STATISTICS OF THE GAME

class records
{
public:
	int victory1,  //total number of matches won by player 1
		victory2,  //total number of matches won by player 2
		totalplay, //total number of matches played
		draw;	  //number of matches tied
} R;

//FUNCTION COLOR

void color()
{
	f1.open("setting.dat", ios::in | ios::binary);
	while (!f1.eof())
	{
		if (!f1.eof())
		{
			f1.read((char *)&S, sizeof(S));
			color1 = S.color1; //Assign color from settings saved in file
			color2 = S.color2;
		}
	}
	f1.close();
}

//FUNCTION VOLUME

void volume()
{
	f1.open("setting.dat", ios::in | ios::binary);
	while (!f1.eof())
	{
		if (!f1.eof())
		{
			f1.read((char *)&S, sizeof(S));
			sop = S.sop; //Assign volume from settings saved in file
		}
	}
	f1.close();
}

//FUNCTION DISPCOLORCH

void dispcolorch() //Display marker on the setting screen corresponding to color settings saved in file
{
	color();
	textcolor(YELLOW);
	switch (color1)
	{
	case 12:
		gotoxy(18, 14);
		putch(42);
		gotoxy(18, 15);
		cout << " ";
		break;
	case 13:
		gotoxy(18, 15);
		putch(42);
		gotoxy(18, 14);
		cout << " ";
		break;
	}
	switch (color2)
	{
	case 11:
		gotoxy(51, 14);
		putch(42);
		gotoxy(51, 15);
		cout << " ";
		break;
	case 10:
		gotoxy(51, 15);
		putch(42);
		gotoxy(51, 14);
		cout << " ";
		break;
	}
	textcolor(WHITE);
}

//FUNCTION DISPVOLCH

void dispvolch() //Display marker on the setting screen corresponding to volume settings saved in file
{
	volume();
	textcolor(YELLOW);
	switch (sop)
	{
	case 1:
		gotoxy(32, 17);
		putch(42);
		gotoxy(51, 17);
		cout << " ";
		break;
	case 0:
		gotoxy(51, 17);
		putch(42);
		gotoxy(32, 17);
		cout << " ";
		break;
	}
	textcolor(WHITE);
}

//FUNCTION FOR INCREASING, CHECKING AND REACTING FOR PLAYER 1

player inccherea1(player P)
{
	int rowchk1 = 0, //used as index for player 1's array
		colchk1 = 0, //used as index for player 1's array
		rowchk2 = 0, //used as index for player 2's array
		colchk2 = 0, //used as index for player 2's array
		absgt = 0,   //x coordinate of screen where orb is to be displayed
		ordgt = 0,   //y coordinate of screen where orb is to be displayed
		row = 0,
		col = 0;
	int ctr = 1;						//if true then incrementing and reacting is required
	if (P.player2[P.rowp][P.colp] == 0) //slot is vacant
	{
		P.player1[P.rowp][P.colp] = P.player1[P.rowp][P.colp] + 1;
		if (sop == 1)
		{
			sound(375);
			delay(80);
			nosound();
		}
	}
	if (P.player1[P.rowp][P.colp] < 4) //display orb
	{
		textcolor(color1);
		putch(P.player1[P.rowp][P.colp]);
	}
	if (P.player1[P.rowp][P.colp] < 2)
	{
		ctr = 0;
	}
	while (ctr != 0)
	{
		for (rowchk1 = 0; rowchk1 < 6; rowchk1++) //checking where reaction is required
		{
			for (colchk1 = 0; colchk1 < 4; colchk1++)
			{
				if (((colchk1 == 0) || (colchk1 == 3)) && ((rowchk1 == 0) || (rowchk1 == 5)))
				{ /*Checking codition for corners*/
					if (P.player1[rowchk1][colchk1] == 2)
					{
						ctr++;
						break;
					}
					else
					{
						ctr = 0;
					}
				}
				if ((((colchk1 == 1) || (colchk1 == 2)) && ((rowchk1 == 0) || (rowchk1 == 5))) || (((colchk1 == 0) || (colchk1 == 3)) && ((rowchk1 == 1) || (rowchk1 == 2) || (rowchk1 == 3) || (rowchk1 == 4))))
				{ /*Checking codition for edges*/
					if (P.player1[rowchk1][colchk1] == 3)
					{
						ctr++;
						break;
					}
					else
					{
						ctr = 0;
					}
				}
				if (P.player1[rowchk1][colchk1] >= 4)
				{ /*Checking codition for interior*/
					ctr++;
					break;
				}
				if (P.player1[rowchk1][colchk1] < 4)
				{
					ctr = 0;
				}
			}
			if (ctr != 0)
			{
				break;
			}
		}
		if (ctr == 0)
		{
			break;
		}
		if (sop == 1)
		{
			delay(250);
			sound(250);
			delay(80);
			nosound();
		}
		absgt = rowchk1 + 31 + (rowchk1 * 3); //calculating coordinates of the screen corresponding to the index of the grid
		ordgt = 8 + (colchk1 * 3);
		rowchk2 = rowchk1;
		colchk2 = colchk1;
		//reaction starts here
		P.player1[rowchk1][colchk1] = 0;
		gotoxy(absgt, ordgt);
		cout << ' ';
		if (colchk1 - 1 >= 0 && P.player2[rowchk2][colchk2 - 1] == 0) //if player2 does not have an orb to the left of the orb to be exploded
		{
			P.player1[rowchk1][colchk1 - 1] = P.player1[rowchk1][colchk1 - 1] + 1;
			gotoxy(absgt, ordgt - 3);
			textcolor(color1);
			putch(P.player1[rowchk1][colchk1 - 1]);
		}
		if (colchk1 - 1 >= 0 && P.player2[rowchk2][colchk2 - 1] != 0) //if player2 has an orb to the left of the orb to be exploded
		{
			P.player1[rowchk1][colchk1 - 1] = P.player2[rowchk2][colchk2 - 1];
			P.player2[rowchk2][colchk2 - 1] = 0;
			P.player1[rowchk1][colchk1 - 1] = P.player1[rowchk1][colchk1 - 1] + 1;
			gotoxy(absgt, ordgt - 3);
			textcolor(color1);
			putch(P.player1[rowchk1][colchk1 - 1]);
		}
		if (colchk1 + 1 <= 3 && P.player2[rowchk2][colchk2 + 1] == 0) //if player2 does not have an orb to the right of the orb to be exploded
		{
			P.player1[rowchk1][colchk1 + 1] = P.player1[rowchk1][colchk1 + 1] + 1;
			gotoxy(absgt, ordgt + 3);
			textcolor(color1);
			putch(P.player1[rowchk1][colchk1 + 1]);
		}
		if (colchk1 + 1 <= 3 && P.player2[rowchk2][colchk2 + 1] != 0) //if player2 has an orb to the right of the orb to be exploded
		{
			P.player1[rowchk1][colchk1 + 1] = P.player2[rowchk2][colchk2 + 1];
			P.player2[rowchk2][colchk2 + 1] = 0;
			P.player1[rowchk1][colchk1 + 1] = P.player1[rowchk1][colchk1 + 1] + 1;
			gotoxy(absgt, ordgt + 3);
			textcolor(color1);
			putch(P.player1[rowchk1][colchk1 + 1]);
		}
		if (rowchk1 + 1 <= 5 && P.player2[rowchk2 + 1][colchk2] == 0) //if player2 does not have an orb below the orb to be exploded
		{
			P.player1[rowchk1 + 1][colchk1] = P.player1[rowchk1 + 1][colchk1] + 1;
			gotoxy(absgt + 4, ordgt);
			textcolor(color1);
			putch(P.player1[rowchk1 + 1][colchk1]);
		}
		if (rowchk1 + 1 <= 5 && P.player2[rowchk2 + 1][colchk2] != 0) //if player2 has orb below the orb to be exploded
		{
			P.player1[rowchk1 + 1][colchk1] = P.player2[rowchk2 + 1][colchk2];
			P.player2[rowchk2 + 1][colchk2] = 0;
			P.player1[rowchk1 + 1][colchk1] = P.player1[rowchk1 + 1][colchk1] + 1;
			gotoxy(absgt + 4, ordgt);
			textcolor(color1);
			putch(P.player1[rowchk1 + 1][colchk1]);
		}
		if (rowchk1 - 1 >= 0 && P.player2[rowchk2 - 1][colchk2] == 0) //if player2 does not have an orb above the orb to be exploded
		{
			P.player1[rowchk1 - 1][colchk1] = P.player1[rowchk1 - 1][colchk1] + 1;
			gotoxy(absgt - 4, ordgt);
			textcolor(color1);
			putch(P.player1[rowchk1 - 1][colchk1]);
		}
		if (rowchk1 - 1 >= 0 && P.player2[rowchk2 - 1][colchk2] != 0) //if player2 does has an orb above the orb to be exploded
		{
			P.player1[rowchk1 - 1][colchk1] = P.player2[rowchk2 - 1][colchk2];
			P.player2[rowchk2 - 1][colchk2] = 0;
			P.player1[rowchk1 - 1][colchk1] = P.player1[rowchk1 - 1][colchk1] + 1;
			gotoxy(absgt - 4, ordgt);
			textcolor(color1);
			putch(P.player1[rowchk1 - 1][colchk1]);
			//reaction ends here
		}
		P.win1 = 0;
		if (P.turn > 2)
		{
			for (row = 0; row < 6; row++)
			{
				for (col = 0; col < 4; col++)
				{
					if (P.player2[row][col] == 0)
					{
						P.win1++; //Checking if player1 has won//
					}
				}
			}
			if (P.win1 == 24)
				break;
		}
	}
	return P;
}

//FUNCTION FOR INCREASING, CHECKING AND REACTING FOR PLAYER 2
//THIS FUNCTION IS SIMILAR TO inccherea1 EXCEPT IT IS USED FOR PLAYER 2 SO DIFFERENT GRID IS USED

player inccherea2(player P)
{
	int rowchk1 = 0, colchk1 = 0, rowchk2 = 0, colchk2 = 0, absgt = 0, ordgt = 0, row = 0, col = 0;
	int ctr = 1;
	f1.close();
	if (P.player1[P.rowp][P.colp] == 0)
	{
		P.player2[P.rowp][P.colp] = P.player2[P.rowp][P.colp] + 1;
		if (sop == 1)
		{
			sound(375);
			delay(80);
			nosound();
		}
	}
	if (P.player2[P.rowp][P.colp] < 4)
	{
		textcolor(color2);
		putch(P.player2[P.rowp][P.colp]);
	}
	if (P.player2[P.rowp][P.colp] < 2)
	{
		ctr = 0;
	}
	while (ctr != 0)
	{
		for (rowchk2 = 0; rowchk2 < 6; rowchk2++)
		{
			for (colchk2 = 0; colchk2 < 4; colchk2++)
			{
				if (((colchk2 == 0) || (colchk2 == 3)) && ((rowchk2 == 0) || (rowchk2 == 5)))
				{ /*Checking codition for corners*/
					if (P.player2[rowchk2][colchk2] == 2)
					{
						ctr++;
						break;
					}
					else
					{
						ctr = 0;
					}
				}
				if ((((colchk2 == 1) || (colchk2 == 2)) && ((rowchk2 == 0) || (rowchk2 == 5))) || (((colchk2 == 0) || (colchk2 == 3)) && ((rowchk2 == 1) || (rowchk2 == 2) || (rowchk2 == 3) || (rowchk2 == 4))))
				{ /*Checking codition for edges*/
					if (P.player2[rowchk2][colchk2] == 3)
					{
						ctr++;
						break;
					}
					else
					{
						ctr = 0;
					}
				}
				if (P.player2[rowchk2][colchk2] >= 4)
				{
					ctr++;
					break;
				}
				if (P.player2[rowchk2][colchk2] < 4)
				{
					ctr = 0;
				}
			}
			if (ctr != 0)
			{
				break;
			}
		}
		if (ctr == 0)
		{
			break;
		}
		if (sop == 1)
		{
			delay(250);
			sound(250);
			delay(80);
			nosound();
		}
		absgt = rowchk2 + 31 + (rowchk2 * 3); //Calculating coordinates of grid//
		ordgt = 8 + (colchk2 * 3);
		rowchk1 = rowchk2;
		colchk1 = colchk2;
		P.player2[rowchk2][colchk2] = 0;
		gotoxy(absgt, ordgt);
		cout << ' ';
		if (colchk2 - 1 >= 0 && P.player1[rowchk1][colchk1 - 1] == 0)
		{
			P.player2[rowchk2][colchk2 - 1] = P.player2[rowchk2][colchk2 - 1] + 1;
			gotoxy(absgt, ordgt - 3);
			textcolor(color2);
			putch(P.player2[rowchk2][colchk2 - 1]);
		}
		if (colchk2 - 1 >= 0 && P.player1[rowchk1][colchk1 - 1] != 0)
		{
			P.player2[rowchk2][colchk2 - 1] = P.player1[rowchk1][colchk1 - 1];
			P.player1[rowchk1][colchk1 - 1] = 0;
			P.player2[rowchk2][colchk2 - 1] = P.player2[rowchk2][colchk2 - 1] + 1;
			gotoxy(absgt, ordgt - 3);
			textcolor(color2);
			putch(P.player2[rowchk2][colchk2 - 1]);
		}
		if (colchk2 + 1 <= 3 && P.player1[rowchk1][colchk1 + 1] == 0)
		{
			P.player2[rowchk2][colchk2 + 1] = P.player2[rowchk2][colchk2 + 1] + 1;
			gotoxy(absgt, ordgt + 3);
			textcolor(color2);
			putch(P.player2[rowchk2][colchk2 + 1]);
		}
		if (colchk2 + 1 <= 3 && P.player1[rowchk1][colchk1 + 1] != 0)
		{
			P.player2[rowchk2][colchk2 + 1] = P.player1[rowchk1][colchk1 + 1];
			P.player1[rowchk1][colchk1 + 1] = 0;
			P.player2[rowchk2][colchk2 + 1] = P.player2[rowchk2][colchk1 + 1] + 1;
			gotoxy(absgt, ordgt + 3);
			textcolor(color2);
			putch(P.player2[rowchk2][colchk2 + 1]);
		}
		if (rowchk2 + 1 <= 5 && P.player1[rowchk1 + 1][colchk1] == 0)
		{
			P.player2[rowchk2 + 1][colchk2] = P.player2[rowchk2 + 1][colchk2] + 1;
			gotoxy(absgt + 4, ordgt);
			textcolor(color2);
			putch(P.player2[rowchk2 + 1][colchk2]);
		}
		if (rowchk2 + 1 <= 5 && P.player1[rowchk1 + 1][colchk1] != 0)
		{
			P.player2[rowchk2 + 1][colchk2] = P.player1[rowchk1 + 1][colchk1];
			P.player1[rowchk1 + 1][colchk1] = 0;
			P.player2[rowchk2 + 1][colchk2] = P.player2[rowchk2 + 1][colchk2] + 1;
			gotoxy(absgt + 4, ordgt);
			textcolor(color2);
			putch(P.player2[rowchk2 + 1][colchk2]);
		}
		if (rowchk2 - 1 >= 0 && P.player1[rowchk1 - 1][colchk1] == 0)
		{
			P.player2[rowchk2 - 1][colchk2] = P.player2[rowchk2 - 1][colchk2] + 1;
			gotoxy(absgt - 4, ordgt);
			textcolor(color2);
			putch(P.player2[rowchk2 - 1][colchk2]);
		}
		if (rowchk2 - 1 >= 0 && P.player1[rowchk1 - 1][colchk1] != 0)
		{
			P.player2[rowchk2 - 1][colchk2] = P.player1[rowchk1 - 1][colchk1];
			P.player1[rowchk1 - 1][colchk1] = 0;
			P.player2[rowchk2 - 1][colchk2] = P.player2[rowchk2 - 1][colchk2] + 1;
			gotoxy(absgt - 4, ordgt);
			textcolor(color2);
			putch(P.player2[rowchk2 - 1][colchk2]);
		}
		P.win2 = 0;
		if (P.turn > 2)
		{
			for (row = 0; row < 6; row++)
			{
				for (col = 0; col < 4; col++)
				{
					if (P.player1[row][col] == 0)
					{
						P.win2++; //Checking if player2 has won//
					}
				}
			}
			if (P.win2 == 24)
				break;
		}
	}
	return P;
}

//FUCTION FOR MOVING CUSOR

void movent()
{
	char val = 'x';
	int abs = 31, //x coordinate of the cursor on screen
		ord = 8,  // y coodinate of the cursor on screen
		i = 0, j = 0;
	P.win1 = 0, P.win2 = 0,
	P.rowp = 0,		//index of the grid in which cursor is present
		P.colp = 0; //index of the grid in which cursor is present
	randomize();
	P.turn = random(2); //Assigning turn randomly//
	for (i = 0; i < 6; i++)
	{
		for (j = 0; j < 4; j++) /////////INITIALISING ARRAYS///////
		{
			P.player1[i][j] = 0;
		}
	}
	for (i = 0; i < 6; i++)
	{
		for (j = 0; j < 4; j++)
		{
			P.player2[i][j] = 0;
		}
	}
	gotoxy(12, 20);
	putch(1);
	cprintf(" ---> ONE ORB");
	gotoxy(34, 20);
	putch(2);
	cprintf(" ---> TWO ORBS");
	gotoxy(55, 20);
	putch(3);
	cprintf(" ---> THREE ORBS");
	textcolor(YELLOW);
	gotoxy(34, 5);
	cprintf("CHAIN REACTION");
	gotoxy(abs, ord);
	while (P.win1 < 24 && P.win2 < 24 && val != 27) //till neither has won nor pressed escape
	{
		while (val != 27 && P.turn % 2 != 0) //player 1's turn
		{
			textcolor(color1);
			for (int xax = 14; xax <= 19; xax++) //Displaying player1's turn//
			{
				for (int yax = 12; yax < 15; yax++)
				{
					gotoxy(xax, yax);
					putch(254);
				}
			}
			gotoxy(12, 9);
			cprintf("PLAYER 1'S");
			gotoxy(15, 10);
			cprintf("TURN");
			gotoxy(abs, ord);
			val = getch();
			val = tolower(val);
			if (val == 13 && P.player2[P.rowp][P.colp] == 0) //if player places an orb wthout moving the cursor
			{
				P = inccherea1(P);
				P.turn++;
			}
			while (val == 'a' || val == 's' || val == 'w' || val == 'd' && P.turn % 2 != 0) //till it is player 1's turn and a valid cursor movement key has been pressed
			{
				if (val == 'a')
				{
					abs = abs - 4;
					P.rowp = P.rowp - 1;										 //moving to left column
					if ((abs <= 51) && (abs >= 31) && (ord <= 17) && (ord >= 8)) //checking if cursor is in the bounds
					{
						gotoxy(abs, ord);
						val = getch(); //as cannot move the cursor accept movement key again
						val = tolower(val);
						if (val == 13 && P.player2[P.rowp][P.colp] == 0) //player 1 places the orb
						{
							P = inccherea1(P);
							P.turn++;
						}
						if (val == 13 && P.player2[P.rowp][P.colp] != 0) //player 1 tries to place the orb but cannot has player 2 already has an orb in this place
						{
							val = getch(); //as cannot place the orb accept key again
							val = tolower(val);
						}
					}
					else //if cursor is not in the bounds the restore previous values
					{
						abs = abs + 4;
						P.rowp = P.rowp + 1;
						val = getch(); //as cannot move the cursor accept movement key again
						val = tolower(val);
					}
				}
				if (val == 's')
				{
					ord = ord + 3;
					P.colp = P.colp + 1;
					if ((abs <= 51) && (abs >= 31) && (ord <= 17) && (ord >= 8))
					{
						gotoxy(abs, ord);
						val = getch();
						val = tolower(val);
						if (val == 13 && P.player2[P.rowp][P.colp] == 0)
						{
							P = inccherea1(P);
							P.turn++;
						}
						if (val == 13 && P.player2[P.rowp][P.colp] != 0)
						{
							val = getch();
							val = tolower(val);
						}
					}
					else
					{
						ord = ord - 3;
						P.colp = P.colp - 1;
						val = getch();
						val = tolower(val);
					}
				}
				if (val == 'w')
				{
					ord = ord - 3;
					P.colp = P.colp - 1;
					if ((abs <= 51) && (abs >= 31) && (ord <= 17) && (ord >= 8))
					{
						gotoxy(abs, ord);
						val = getch();
						val = tolower(val);
						if (val == 13 && P.player2[P.rowp][P.colp] == 0)
						{
							P = inccherea1(P);
							P.turn++;
						}
						if (val == 13 && P.player2[P.rowp][P.colp] != 0)
						{
							val = getch();
							val = tolower(val);
						}
					}
					else
					{
						ord = ord + 3;
						P.colp = P.colp + 1;
						val = getch();
						val = tolower(val);
					}
				}
				if (val == 'd')
				{
					abs = abs + 4;
					P.rowp = P.rowp + 1;
					if ((abs <= 51) && (abs >= 31) && (ord <= 17) && (ord >= 8))
					{
						gotoxy(abs, ord);
						val = getch();
						val = tolower(val);
						if (val == 13 && P.player2[P.rowp][P.colp] == 0)
						{
							P = inccherea1(P);
							P.turn++;
						}
						if (val == 13 && P.player2[P.rowp][P.colp] != 0)
						{
							val = getch();
							val = tolower(val);
						}
					}
					else
					{
						abs = abs - 4;
						P.rowp = P.rowp - 1;
						val = getch();
						val = tolower(val);
					}
				}
				if (val == 27)
				{
					break;
				}
			}
		}
		if (val == 27 || P.win1 == 24)
		{
			break;
		}
		else
		{
			val = 'x';
		}
		while (val != 27 && P.turn % 2 == 0) //player 2's turn
		{
			textcolor(color2);
			for (int xax = 14; xax <= 19; xax++) //Displaying player 2's turn//
			{
				for (int yax = 12; yax < 15; yax++)
				{
					gotoxy(xax, yax);
					putch(254);
				}
			}
			gotoxy(12, 9);
			cprintf("PLAYER 2'S");
			gotoxy(15, 10);
			cprintf("TURN");
			gotoxy(abs, ord);
			val = getch();
			val = tolower(val);
			if (val == 13 && P.player1[P.rowp][P.colp] == 0) //if player places an orb wthout moving the cursor
			{
				P = inccherea2(P);
				P.turn++;
			}
			while (val == 'j' || val == 'k' || val == 'i' || val == 'l' && P.turn % 2 == 0) //till it is player 2's turn and a valid cursor movement key has been pressed
			{
				if (val == 'j')
				{
					abs = abs - 4;
					P.rowp = P.rowp - 1;
					if ((abs <= 51) && (abs >= 31) && (ord <= 17) && (ord >= 8))
					{
						gotoxy(abs, ord);
						val = getch();
						val = tolower(val);
						if (val == 13 && P.player1[P.rowp][P.colp] == 0)
						{
							P = inccherea2(P);
							P.turn++;
						}
						if (val == 13 && P.player1[P.rowp][P.colp] != 0)
						{
							val = getch();
							val = tolower(val);
						}
					}
					else
					{
						abs = abs + 4;
						P.rowp = P.rowp + 1;
						val = getch();
						val = tolower(val);
					}
				}
				if (val == 'k')
				{
					ord = ord + 3;
					P.colp = P.colp + 1;
					if ((abs <= 51) && (abs >= 31) && (ord <= 17) && (ord >= 8))
					{
						gotoxy(abs, ord);
						val = getch();
						val = tolower(val);
						if (val == 13 && P.player1[P.rowp][P.colp] == 0)
						{
							P = inccherea2(P);
							P.turn++;
						}
						if (val == 13 && P.player1[P.rowp][P.colp] != 0)
						{
							val = getch();
							val = tolower(val);
						}
					}
					else
					{
						ord = ord - 3;
						P.colp = P.colp - 1;
						val = getch();
						val = tolower(val);
					}
				}
				if (val == 'i')
				{
					ord = ord - 3;
					P.colp = P.colp - 1;
					if ((abs <= 51) && (abs >= 31) && (ord <= 17) && (ord >= 8))
					{
						gotoxy(abs, ord);
						val = getch();
						val = tolower(val);
						if (val == 13 && P.player1[P.rowp][P.colp] == 0)
						{
							P = inccherea2(P);
							P.turn++;
						}
						if (val == 13 && P.player1[P.rowp][P.colp] != 0)
						{
							val = getch();
							val = tolower(val);
						}
					}
					else
					{
						ord = ord + 3;
						P.colp = P.colp + 1;
						val = getch();
						val = tolower(val);
					}
				}
				if (val == 'l')
				{
					abs = abs + 4;
					P.rowp = P.rowp + 1;
					if ((abs <= 51) && (abs >= 31) && (ord <= 17) && (ord >= 8))
					{
						gotoxy(abs, ord);
						val = getch();
						val = tolower(val);
						if (val == 13 && P.player1[P.rowp][P.colp] == 0)
						{
							P = inccherea2(P);
							P.turn++;
						}
						if (val == 13 && P.player1[P.rowp][P.colp] != 0)
						{
							val = getch();
							val = tolower(val);
						}
					}
					else
					{
						abs = abs - 4;
						P.rowp = P.rowp - 1;
						val = getch();
						val = tolower(val);
					}
				}
				if (val == 27)
				{
					break;
				}
			}
		}
		if (val == 27 || P.win2 == 24)
		{
			break;
		}
		else
		{
			val = 'x';
		}
	}
	if (P.turn > 2) //if turns is greater than 2 and neither player win the register that game is tied
	{
		if (P.win1 != 24 && P.win2 != 24)
		{
			f1.open("records.dat", ios::out | ios::binary);
			R.draw++;	  //incrementing number of draws//
			R.totalplay++; //incrementing number of games played//
			f1.write((char *)&R, sizeof(R));
			f1.close();
		}
	}
	if (P.win1 == 24 || P.win2 == 24) //checking if any player has won//
	{
		gotoxy(3, 3);
		if (P.win1 == 24)
		{
			_setcursortype(_NOCURSOR);
			textcolor(color2);
			gotoxy(13, 11);
			cprintf("PLAYER 2");
			gotoxy(13, 12);
			cprintf("HAS LOST");
			gotoxy(11, 13);
			cprintf("ALL THE ORBS");
			gotoxy(11, 9);
			cout << "            ";
			gotoxy(11, 10);
			cout << "            ";
			gotoxy(11, 14);
			cout << "            ";
			gotoxy(11, 15);
			cout << "            ";
			textcolor(color1 + BLINK);
			gotoxy(61, 12);
			cprintf("PLAYER 1");
			gotoxy(61, 13);
			cprintf("WINS !!!");
			f1.open("records.dat", ios::out | ios::binary);
			R.victory1++;  //incrementing number of player1's wins//
			R.totalplay++; //incrementing number of games played//
			f1.write((char *)&R, sizeof(R));
			f1.close();
		}
		if (P.win2 == 24)
		{
			_setcursortype(_NOCURSOR);
			textcolor(color1);
			gotoxy(13, 11);
			cprintf("PLAYER 1");
			gotoxy(13, 12);
			cprintf("HAS LOST");
			gotoxy(11, 13);
			cprintf("ALL THE ORBS");
			gotoxy(11, 9);
			cout << "            ";
			gotoxy(11, 10);
			cout << "            ";
			gotoxy(11, 14);
			cout << "            ";
			gotoxy(11, 15);
			cout << "            ";
			textcolor(color2 + BLINK);
			gotoxy(61, 12);
			cprintf("PLAYER 2");
			gotoxy(61, 13);
			cprintf("WINS !!!");
			f1.open("records.dat", ios::out | ios::binary);
			R.victory2++;  //incrementing number of player2's wins//
			R.totalplay++; //incrementing number of games played//
			f1.write((char *)&R, sizeof(R));
			f1.close();
		}
		getch();
	}
}

//FUNCTION FOR GENERATING AND DISPLAYING BORDER

void border()
{
	int p = 1, q = 1, lim = 80, x, color = 14;
	while (p <= 3 && q <= 3)
	{
		textcolor(color);
		for (x = p; x < lim; x++)
		{
			gotoxy(x, q);
			cprintf("#");
		}
		p++, q++, lim--, color = color - 2;
	}
	q = 24, p = 1, color = 14, lim = 80;
	while (p <= 3 && q >= 22)
	{
		textcolor(color);
		for (x = p; x < lim; x++)
		{
			gotoxy(x, q);
			cprintf("#");
		}
		p++, q--, lim--, color = color - 2;
	}
	q = 1, p = 1, color = 14, lim = 25;
	while (p <= 3 && q <= 3)
	{
		textcolor(color);
		for (x = q; x < lim; x++)
		{
			gotoxy(p, x);
			cprintf("#");
		}
		p++, q++, lim--, color = color - 2;
	}
	q = 1, p = 80, color = 14, lim = 25;
	while (p >= 78 && q <= 3)
	{
		textcolor(color);
		for (x = q; x < lim; x++)
		{
			gotoxy(p, x);
			cprintf("#");
		}
		p--, q++, lim--, color = color - 2;
	}
}

//FUNCTION FOR DISPLAYING GAME NAME

void gamename()
{
	textcolor(WHITE);
	gotoxy(27, 7);
	cprintf(" #### #   #   #   ##### #   #");
	gotoxy(27, 8);
	cprintf("#     #   #  # #    #   ##  #");
	gotoxy(27, 9);
	cprintf("#     ##### #####   #   # # #");
	gotoxy(27, 10);
	cprintf("#     #   # #   #   #   #  ##");
	gotoxy(27, 11);
	cprintf(" #### #   # #   # ##### #   #");

	gotoxy(16, 14);
	cprintf(" ####   #####   #    #### ##### #####  ###  #   #");
	gotoxy(16, 15);
	cprintf(" #   #  #      # #  #       #     #   #   # ##  #");
	gotoxy(16, 16);
	cprintf(" ####   ###   ##### #       #     #   #   # # # #");
	gotoxy(16, 17);
	cprintf(" #   #  #     #   # #       #     #   #   # #  ##");
	gotoxy(16, 18);
	cprintf(" #    # ##### #   #  ####   #   #####  ###  #   #");
	gotoxy(28, 20);
	cout << "Press any key to continue.";
	getch();
}

//FUNCTION FOR DISPLAYING MAIN MENU

void mainmenu()
{
	clrscr();
	border();
	textcolor(WHITE);
	gotoxy(15, 5);
	cprintf("#   #   #   ##### #   #    #   # ##### #   # #   #");
	gotoxy(15, 6);
	cprintf("## ##  # #    #   ##  #    ## ## #     ##  # #   #");
	gotoxy(15, 7);
	cprintf("# # # #####   #   # # #    # # # ###   # # # #   #");
	gotoxy(15, 8);
	cprintf("#   # #   #   #   #  ##    #   # #     #  ## #   #");
	gotoxy(15, 9);
	cprintf("#   # #   # ##### #   #    #   # ##### #   #  ### ");
	textcolor(5);
	textcolor(YELLOW);
	gotoxy(15, 12);
	cprintf("1.PLAY GAME");
	gotoxy(15, 14);
	cprintf("2.OBJECTIVE");
	gotoxy(15, 16);
	cprintf("3.CONTROLS");
	gotoxy(51, 12);
	cprintf("4.RECORDS");
	gotoxy(51, 14);
	cprintf("5.SETTINGS");
	gotoxy(51, 16);
	cprintf("6.QUIT");
	gotoxy(20, 20);
	cprintf("Toggle menu using 'a', 's', 'w' and 'd'.");
}

//FUNCTION FOR DISPLAYING CONTROLS

void controls()
{
	clrscr();
	border();
	textcolor(YELLOW);
	gotoxy(17, 5);
	cprintf(" ####  ###  #   # ##### ####    ###  #      ####");
	gotoxy(17, 6);
	cprintf("#     #   # ##  #   #   #   #  #   # #     #    ");
	gotoxy(17, 7);
	cprintf("#     #   # # # #   #   ####   #   # #      ### ");
	gotoxy(17, 8);
	cprintf("#     #   # #  ##   #   #   #  #   # #         #");
	gotoxy(17, 9);
	cprintf(" ####  ###  #   #   #   #    #  ###  ##### #### ");
	color();
	textcolor(color1);
	gotoxy(20, 11);
	cprintf("PLAYER 1");
	gotoxy(17, 12);
	cprintf("To move cursor:");
	gotoxy(17, 13);
	cprintf("w ---> UP");
	gotoxy(17, 14);
	cprintf("s ---> DOWN");
	gotoxy(17, 15);
	cprintf("a ---> LEFT");
	gotoxy(17, 16);
	cprintf("d ---> RIGHT");
	textcolor(color2);
	gotoxy(53, 11);
	cprintf("PLAYER 2");
	gotoxy(50, 12);
	cprintf("To move cursor:");
	gotoxy(50, 13);
	cprintf("i ---> UP");
	gotoxy(50, 14);
	cprintf("k ---> DOWN");
	gotoxy(50, 15);
	cprintf("j ---> LEFT");
	gotoxy(50, 16);
	cprintf("l ---> RIGHT");
	textcolor(WHITE);
	gotoxy(26, 18);
	cprintf("Press Enter to insert an orb.");
	gotoxy(17, 19);
	cprintf("Press Escape to return to menu during game play.");
	textcolor(WHITE);
	gotoxy(25, 20);
	cprintf("Press any key to exit controls.");
	getch();
}

//FUNCTION FOR DISPLAYING OBJECTIVE

void objective()
{
	clrscr();
	border();
	textcolor(YELLOW);
	gotoxy(14, 5);
	cprintf(" ###  ####  ##### #####  #### ##### ##### #   # #####");
	gotoxy(14, 6);
	cprintf("#   # #   #   #   #     #       #     #   #   # #    ");
	gotoxy(14, 7);
	cprintf("#   # ####    #   ###   #       #     #   #   # ###  ");
	gotoxy(14, 8);
	cprintf("#   # #   # # #   #     #       #     #    # #  #    ");
	gotoxy(14, 9);
	cprintf(" ###  ####   #    #####  ####   #   #####   #   #####");
	textcolor(WHITE);
	gotoxy(8, 11);
	cprintf("The objective of Chain Reaction is to take control of the board by");
	gotoxy(8, 12);
	cprintf("eliminating your opponents orbs.");
	gotoxy(8, 13);
	cprintf("Players takes it in turns to place their orbs in a cell.");
	gotoxy(8, 14);
	cprintf("Once the cell has reached critical mass, the orbs explode into the");
	gotoxy(8, 15);
	cprintf("surrounding cells adding an extra orb and claiming the cell of the");
	gotoxy(8, 16);
	cprintf("player.A player may only place their orb in a blank cell or a cell");
	gotoxy(8, 17);
	cprintf("that contains orbs of their own colour.");
	gotoxy(8, 18);
	cprintf("As soon as a player looses all orbs, player is out of the game.");
	gotoxy(25, 20);
	cout << "Press any key to exit objective.";
	getch();
}

//FUNCTION FOR DISPLAYING ENDING

void ending()
{
	clrscr();
	_setcursortype(_NOCURSOR);
	border();
	textcolor(11);
	gotoxy(12, 7);
	cprintf(" #### ####   #####   #   ##### ##### ###       ####  #   #");
	gotoxy(12, 8);
	cprintf("#     #   #  #      # #    #   #     #  #      #   #  # # ");
	gotoxy(12, 9);
	cprintf("#     ####   ###   #####   #   ###   #   #     ####    #  ");
	gotoxy(12, 10);
	cprintf("#     #   #  #     #   #   #   #     #  #      #   #   #  ");
	gotoxy(12, 11);
	cprintf(" #### #    # ##### #   #   #   ##### ###       ####    #  ");
	gotoxy(12, 14);
	cprintf("####    ###  #   #   #   #   #     ###    #   ####    ### ");
	gotoxy(12, 15);
	cprintf("#   #  #   # #   #  # #  ##  #    #      # #  #   #  #    ");
	gotoxy(12, 16);
	cprintf("####   #   # ##### ##### # # #    #  ## ##### ####   #  ##");
	gotoxy(12, 17);
	cprintf("#   #  #   # #   # #   # #  ##    #   # #   # #   #  #   #");
	gotoxy(12, 18);
	cprintf("#    #  ###  #   # #   # #   #     ###  #   # #    #  ### ");
	getch();
}

//FUNCTION FOR DISPLAYING & CHANGING SETTINGS

void settings()
{
	clrscr();
	_setcursortype(_NOCURSOR);
	border();
	textcolor(YELLOW);
	gotoxy(17, 5);
	cprintf(" #### ##### ##### ##### ##### #   #  ###   ####");
	gotoxy(17, 6);
	cprintf("#     #       #     #     #   ##  # #     #    ");
	gotoxy(17, 7);
	cprintf(" ###  ###     #     #     #   # # # #  ##  ### ");
	gotoxy(17, 8);
	cprintf("    # #       #     #     #   #  ## #   #     #");
	gotoxy(17, 9);
	cprintf("####  #####   #     #   ##### #   #  ###  #### ");
	textcolor(WHITE);
	gotoxy(34, 11);
	cprintf("SELECT COLOUR");
	gotoxy(20, 12);
	cprintf("PLAYER 1");
	textcolor(12);
	gotoxy(20, 14);
	cprintf("RED");
	textcolor(13);
	gotoxy(20, 15);
	cprintf("PINK");
	textcolor(WHITE);
	gotoxy(53, 12);
	cprintf("PLAYER 2");
	textcolor(11);
	gotoxy(53, 14);
	cprintf("AQUA");
	textcolor(10);
	gotoxy(53, 15);
	cprintf("GREEN");
	textcolor(WHITE);
	gotoxy(20, 17);
	cprintf("SOUND:");
	gotoxy(34, 17);
	cprintf("ON");
	gotoxy(53, 17);
	cprintf("OFF");
	textcolor(YELLOW);
	gotoxy(25, 19);
	cprintf("Press escape to exit settings.");
	gotoxy(17, 20);
	cprintf("Press tab to toggle b/w sound & color settings.");
	dispcolorch();
	dispvolch();
	textcolor(WHITE);
	int i = 27, j = 14, flag = 0;
	gotoxy(i, j);
	putch(42);
	char pointer = 'x';
	while (pointer != 27) //For moving cursor about settings//
	{
		while (flag % 2 == 0) //For moving cursor about color settings//
		{
			pointer = getch();
			pointer = tolower(pointer);
			if (pointer == 13)
			{
				switch (wherey()) //For changing color settings//
				{
				case 14:
					if (wherex() == 28)
					{
						f1.open("setting.dat", ios::out | ios::binary);
						S.color1 = 12; //player1(red)//
						f1.write((char *)&S, sizeof(S));
						f1.close();
					}
					if (wherex() == 61)
					{
						f1.open("setting.dat", ios::out | ios::binary);
						S.color2 = 11; //player2(aqua)//
						f1.write((char *)&S, sizeof(S));
						f1.close();
					}
					break;
				case 15:
					if (wherex() == 28)
					{
						f1.open("setting.dat", ios::out | ios::binary);
						S.color1 = 13; //player1(pink)//
						f1.write((char *)&S, sizeof(S));
						f1.close();
					}
					if (wherex() == 61)
					{
						f1.open("setting.dat", ios::out | ios::binary);
						S.color2 = 10; //player2(green)//
						f1.write((char *)&S, sizeof(S));
						f1.close();
					}
					break;
				}
				dispcolorch();
			}
			if (pointer == 'w' && j > 14)
			{
				gotoxy(i, j);
				cout << " ";
				j = j - 1;
				gotoxy(i, j);
				putch(42);
			}
			if (pointer == 's' && j < 15)
			{
				gotoxy(i, j);
				cout << " ";
				j = j + 1;
				gotoxy(i, j);
				putch(42);
			}
			if (pointer == 'a' && i > 27)
			{
				gotoxy(i, j);
				cout << " ";
				i = i - 33;
				gotoxy(i, j);
				putch(42);
			}
			if (pointer == 'd' && i < 60)
			{
				gotoxy(i, j);
				cout << " ";
				i = i + 33;
				gotoxy(i, j);
				putch(42);
			}
			if (pointer == 9) //for shifting cursor to sound settings//
			{
				flag++;
				gotoxy(i, j);
				cout << " ";
				break;
			}
			if (pointer == 27) //for exiting settings//
				break;
			else
				pointer = 'x';
		}
		if (pointer == 9)
		{
			i = 40, j = 17;
			gotoxy(i, j);
			putch(42);
		}
		if (pointer == 27)
			break;
		while (flag % 2 != 0) //For moving cursor about sound settings//
		{
			pointer = getch();
			pointer = tolower(pointer);
			if (pointer == 13)
			{
				switch (wherex()) //For changing sound settings//
				{
				case 41:
					f1.open("setting.dat", ios::out | ios::binary);
					S.sop = 1; //on//
					f1.write((char *)&S, sizeof(S));
					f1.close();
					break;
				case 61:
					f1.open("setting.dat", ios::out | ios::binary);
					S.sop = 0; //off//
					f1.write((char *)&S, sizeof(S));
					f1.close();
					break;
				}
				dispvolch();
			}
			if (pointer == 'a' && i > 41)
			{
				gotoxy(i, j);
				cout << " ";
				i = i - 20;
				gotoxy(i, j);
				putch(42);
			}
			if (pointer == 'd' && i < 61)
			{
				gotoxy(i, j);
				cout << " ";
				i = i + 20;
				gotoxy(i, j);
				putch(42);
			}
			if (pointer == 9) //for shifting cursor to color settings//
			{
				gotoxy(i, j);
				cout << " ";
				flag++;
				break;
			}
			if (pointer == 27) //for exiting settings//
				break;
			else
				pointer = 'x';
		}
		if (pointer == 9)
		{
			i = 27, j = 14;
			gotoxy(27, 14);
			putch(42);
		}
	}
}

//FUNCTION FOR DISPLAYING & RESETTING RECDORDS

void records()
{
	int winperc1, winperc2;
	char reset = 127, choice;
	while (reset == 127)
	{
		clrscr();
		_setcursortype(_NOCURSOR);
		border();
		textcolor(YELLOW);
		gotoxy(19, 5);
		cprintf("####   #####  ####  ###  ####   ###    ####");
		gotoxy(19, 6);
		cprintf("#   #  #     #     #   # #   #  #  #  #    ");
		gotoxy(19, 7);
		cprintf("####   ###   #     #   # ####   #   #  ### ");
		gotoxy(19, 8);
		cprintf("#   #  #     #     #   # #   #  #  #      #");
		gotoxy(19, 9);
		cprintf("#    # #####  ####  ###  #    # ###   #### ");
		color();
		textcolor(color1);
		gotoxy(21, 11);
		cprintf("PLAYER 1");
		textcolor(color2);
		gotoxy(52, 11);
		cprintf("PLAYER 2");
		textcolor(WHITE);
		gotoxy(21, 13);
		cprintf("WINS:");
		gotoxy(21, 14);
		cprintf("WIN %:");
		gotoxy(52, 13);
		cprintf("WINS:");
		gotoxy(52, 14);
		cprintf("WIN %:");
		gotoxy(21, 16);
		cprintf("DRAWS:");
		gotoxy(21, 17);
		cprintf("TOTAL GAMES PLAYED:");
		gotoxy(25, 19);
		cprintf("Press any key to exit records.");
		gotoxy(21, 20);
		cprintf("Press ctrl+backspace to reset records.");
		f1.open("records.dat", ios::in | ios::binary);
		while (!f1.eof()) //for displaying records//
		{
			f1.read((char *)&R, sizeof(R));
			if (!f1.eof())
			{
				gotoxy(26, 13);
				cout << R.victory1;
				gotoxy(57, 13);
				cout << R.victory2;
				if (R.victory1 + R.victory2 != 0)
				{
					gotoxy(27, 14);
					winperc1 = (R.victory1 * 100) / (R.victory1 + R.victory2);
					cout << winperc1 << " %";
					gotoxy(58, 14);
					winperc2 = (R.victory2 * 100) / (R.victory1 + R.victory2);
					cout << winperc2 << " %";
				}
				else
				{
					gotoxy(27, 14);
					cout << "0 %";
					gotoxy(58, 14);
					cout << "0 %";
				}
				gotoxy(27, 16);
				cout << R.draw;
				gotoxy(40, 17);
				cout << R.totalplay;
			}
		}
		f1.close();
		reset = getch();
		if (reset == 127) //for resetting records//
		{
			while (1)
			{
				clrscr();
				border();
				textcolor(WHITE);
				gotoxy(22, 18);
				cprintf("Press any key to return to records.");
				gotoxy(19, 11);
				_setcursortype(_NORMALCURSOR);
				cprintf("Do you really want to reset records(Y/N):");
				choice = getche();
				choice = toupper(choice);
				if (choice == 'Y')
				{
					f1.open("records.dat", ios::out | ios::binary);
					R.victory1 = 0;
					R.victory2 = 0;
					R.draw = 0;
					R.totalplay = 0;
					f1.write((char *)&R, sizeof(R));
					f1.close();
					_setcursortype(_NOCURSOR);
					gotoxy(32, 13);
					delay(500);
					cprintf("RESET COMPLETE");
					getch();
					textcolor(YELLOW);
					break;
				}
				if (choice == 'N')
				{
					_setcursortype(_NOCURSOR);
					gotoxy(32, 13);
					delay(500);
					cprintf("RESET CANCELLED");
					getch();
					textcolor(YELLOW);
					break;
				}
				else
					break;
			}
		}
	}
}

//FUNCTION FOR ACCEPTING PASSWORD

void pass()
{
	_setcursortype(_NORMALCURSOR);
	char pass[20];
	char acc[3] = {82, 32, 71};
	int z, tries = 3, llim = 42;
	gotoxy(27, 12);
	textcolor(WHITE);
	cprintf("Enter password:");
	while (tries > 0)
	{
		gotoxy(63, 5);
		cprintf("TRIES LEFT:");
		cout << tries;
		llim = 42;
		gotoxy(llim, 12);
		for (z = 0; z < 20; z++)
		{
			pass[z] = '0';
		}
		for (z = 0; z < 20;)
		{
			pass[z] = getch();
			if (pass[z] == 13)
			{
				break;
			}
			/*for working of backspace*/ if (pass[z] == 8 && llim > 42)
			{
				pass[z] = '\0';
				pass[z - 1] = '\0';
				llim--;
				gotoxy(llim, 12);
				cout << pass[z - 1];
				cout << "\b";
				z--;
			}
			else if (pass[z] != 8 && llim >= 42 && llim < 61)
			{
				gotoxy(llim, 12);
				llim++;
				cprintf("*");
				z++;
			}
		}
		pass[z] = '\0';
		if (strcmp(pass, acc) == 0) //comparing password//
		{
			clrscr();
			border();
			gotoxy(34, 11);
			_setcursortype(_NOCURSOR);
			cout << "Access granted.";
			gotoxy(28, 12);
			cout << "Press any key to continue.";
			getch();
			break;
		}
		else if (strcmp(pass, acc) != 0 || z == 19)
		{
			clrscr();
			border();
			textcolor(RED);
			gotoxy(34, 10);
			cprintf("Wrong password.");
			textcolor(WHITE);
			gotoxy(26, 12);
			cprintf("Please Re-enter:");
			tries--;
		}
	}
	if (tries == 0)
	{
		ending();
		exit(0);
	}
}

//MAIN FUNCTION

void main()
{
	char cursor = 'x';
	int x = 28, y = 12, length;
	f1.open("setting.dat", ios::in | ios::out | ios::binary);
	S.color1 = 12;
	S.color2 = 10; //initialising file setting.dat if it is empty//
	S.sop = 1;
	f1.seekg(0, ios::end);
	length = f1.tellg();
	f1.seekg(0, ios::beg);
	if (length == 0)
	{
		if (!f1.eof())
		{
			f1.write((char *)&S, sizeof(S));
		}
	}
	f1.close();
	f1.open("records.dat", ios::in | ios::out | ios::binary);
	R.victory1 = 0;
	R.victory2 = 0; //initialising file records.dat if it is empty//
	R.totalplay = 0;
	R.draw = 0;
	f1.seekg(0, ios::end);
	length = f1.tellg();
	f1.seekg(0, ios::beg);
	if (length == 0)
	{
		if (!f1.eof())
		{
			f1.write((char *)&R, sizeof(R));
		}
	}
	f1.close();
	clrscr();
	textbackground(BLACK);
	_setcursortype(_NOCURSOR);
	border();
	gamename();
	clrscr();
	border();
	pass();
	mainmenu();
	while (1) //for moving cursor about main menu//
	{
		if (wherey() == 16 && wherex() == 62)
			break;
		mainmenu();
		gotoxy(x, y);
		_setcursortype(_NOCURSOR);
		putch(42);
		cursor = 'x';
		while (cursor != 13)
		{
			cursor = getch();
			cursor = tolower(cursor);
			if (cursor == 13)
				break;
			if (cursor == 'w' && y > 12)
			{
				gotoxy(x, y);
				cout << " ";
				y = y - 2;
				gotoxy(x, y);
				putch(42);
			}
			if (cursor == 's' && y < 16)
			{
				gotoxy(x, y);
				cout << " ";
				y = y + 2;
				gotoxy(x, y);
				putch(42);
			}
			if (cursor == 'a' && x > 28)
			{
				gotoxy(x, y);
				cout << " ";
				x = x - 34;
				gotoxy(x, y);
				putch(42);
			}
			if (cursor == 'd' && x < 62)
			{
				gotoxy(x, y);
				cout << " ";
				x = x + 34;
				gotoxy(x, y);
				putch(42);
			}
			else
				cursor = 'x';
		}
		switch (wherey())
		{
		case 12:
			if (wherex() == 29) //play game//
			{
				grid();
				color();
				volume();
				_setcursortype(_NORMALCURSOR);
				border();
				textcolor(WHITE);
				movent();
			}
			if (wherex() == 63) //records//
			{
				records();
			}
			break;
		case 14:
			if (wherex() == 29) //obectives//
			{
				_setcursortype(_NOCURSOR);
				objective();
			}
			if (wherex() == 63) //settings//
			{
				settings();
			}
			break;
		case 16:
			if (wherex() == 29) //controls//
			{
				_setcursortype(_NOCURSOR);
				controls();
			}
			if (wherex() == 63) //exit//
			{
				ending();
				exit(0);
			}
			break;
		}
	}
}

//END