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
    //virgule flotante :
    // int j = 20;
    // float i = 12444.797898;
    // printf("|%*.2f|",j ,i);

    //numéros des caractères :
    char c;

    while(c != 'a'){
        c = getch();

        if(c != -32){
            printf("\nCaractere simple : %c\nnumeros : %d\n", c, c);
        }
        else{
            c = getch();
            printf("\nNumeros du caractere double : -32 -> %d", c);
        }
    }
}