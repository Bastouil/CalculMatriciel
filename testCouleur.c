#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <math.h>

void gotoligcol( int lig, int col )
{
	// ressources
	COORD mycoord;
	
	mycoord.X = col;
	mycoord.Y = lig;
	SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), mycoord );
}

void main(void)
{
    char c;
    int i;
  
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  
	for(i = 0; i < 255; ++i)
	{
	  	SetConsoleTextAttribute(hConsole, i);
		printf("%3d: XX ", i);
		
		if ((i % 16) == 15){
            SetConsoleTextAttribute(hConsole, 15);
		    printf(" "); printf("\n");
        }
	}
    SetConsoleTextAttribute(hConsole, 15);
    printf("\n");printf("\n");printf("\n");

    printf("Appuiez sur une touche pour continuer ...");
    c = getch();
}