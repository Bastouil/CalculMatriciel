// Bastian GARCON 11/12/2022
// Programme faisant des calcules de matrices

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <math.h>
#include <conio.h>

// Déclaration des constantes ///////////////////////////////////////////////////////////////////////////////////////////////////
// défination de deux constantes représentant le vrai ou faux
#define bool short int
#define false 0
#define true 1
// taille des noms de matrices
#define BASIC_STRING_SIZE 20
// Couleurs utilisées pour le changement de couleur du texte et du fond sur le terminal
#define BLACK 0
#define DARK_BLUE 1
#define DARK_GREEN 2
#define LIGHT_BLUE 3
#define RED 4
#define PURPLE 5
#define ORANGE 6
#define LIGHT_GREY 7
#define DARK_GREY 8
#define BLUE 9
#define LIGHT_GREEN 10
#define CYAN 11
#define PICK 12
#define LIGHT_PURPLE 13
#define BEIGE 14
#define WHITE 15
// taille des différents menus
#define SIZE_MAIN_MENU 4

// Déclaration des structures ///////////////////////////////////////////////////////////////////////////////////////////////////

// Structure des maillons de matrice --------------------------------------------------------------------------------------------
typedef struct unit
{
    // contient la donnée du maillon
    float data;
    // indice de la ligne
    int M;
    // indice de la colonne
    int N;
    // pointeurs sur le maillon de "gauche"
    struct unit *pLeft;
    // pointeurs sur le maillon de "droite"
    struct unit *pRight;
    // pointeurs sur le maillon du "dessus"
    struct unit *pUp;
    // pointeurs sur le maillon du "dessous"
    struct unit *pDown;
} Tunit;
typedef Tunit *PTunit;

// Structure des poiteurs sur matrice -------------------------------------------------------------------------------------------
typedef struct matrix
{
    // contient le nom de la matrice afin qu'elles puissent être différenciées plus facilement par l'utilisateur
    char name[BASIC_STRING_SIZE];
    // variable contenant la valeur max absolue de toute la matrice afin que l'affichage ce fasse correctement
    float dataMaxAbs;
    // variable qui précise si la matrive contient que des entiers ou non
    bool intPresence;
    // pointeur sur le maillon à l'indice (0, 0) de la matrice 
    PTunit pUnit00;
    // pointeur sur le maillon se trouvant à la dernière ligne et à la dernière colonne de la matrice
    PTunit pUnitEnd;
    // pointeur sur la matrice suivante
    struct matrix *pNext;
    // pointeur sur la matrice précédente
    struct matrix *pPrevious;
} Tmatrix;
typedef Tmatrix *PTmatrix;

// Déclaration des variables globales ///////////////////////////////////////////////////////////////////////////////////////////
HANDLE hConsole;

PTmatrix pBENstartMatrix, pBENendMatrix;
PTunit pBENstartUnit, pBENendUnit;

int numberOfMatrix = 0;
PTmatrix pStratMatrix, pEndMatrix;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                          FONCTIONS                                                          //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Gestion de donnée ////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Alloue ou recycle de la mémoire pour un pointeur de matrice ------------------------------------------------------------------
 */
PTmatrix Allocate_memory_matrix()
{
    PTmatrix pNewMatrix;
    // si la liste poubelle n'est pas vide => prendre dans la poubelle
    if (pBENstartMatrix->pNext != pBENendMatrix)
    {
        pNewMatrix = pBENstartMatrix->pNext;
        pBENstartMatrix->pNext = pNewMatrix->pNext;
        pNewMatrix->pNext->pPrevious = pBENstartMatrix;
    }
    // sinon la liste poubelle est vide => allocation de mémoire
    else
    {
        pNewMatrix = (PTmatrix)malloc(sizeof(*pNewMatrix));
    }
    // par securite on met tout les pointeurs de la structure a NULL
    pNewMatrix->pUnit00 = NULL;
    pNewMatrix->pUnitEnd = NULL;
    pNewMatrix->pPrevious = NULL;
    pNewMatrix->pNext = NULL;
    return (pNewMatrix);
}

/* Alloue ou recycle de la mémoire pour un pointeur de maillon ------------------------------------------------------------------
 */
PTunit Allocate_memory_unit()
{
    PTunit pNewUnit;
    // si la liste poubelle n'est pas vide => prendre dans la poubelle
    if (pBENstartUnit->pRight != pBENendUnit)
    {
        pNewUnit = pBENstartUnit->pRight;
        pBENstartUnit->pRight = pNewUnit->pRight;
        pNewUnit->pRight->pLeft = pBENstartUnit;
    }
    // sinon la liste poubelle est vide => allocation de mémoire
    else
    {
        pNewUnit = (PTunit)malloc(sizeof(*pNewUnit));
    }
    // par securite on met tout les pointeurs de la structure a NULL
    pNewUnit->pLeft = NULL;
    pNewUnit->pRight = NULL;
    pNewUnit->pUp = NULL;
    pNewUnit->pDown = NULL;
    // on met par défaut la donnée à 0
    pNewUnit->data = 0;
    return (pNewUnit);
}

/* Renvoie le maillon se trouvant à la ligne "unitLine" et à la colonne "unitColumn" dans la matrice donnée ------------------------------
 */
PTunit Unit_at(int unitLine, int unitColumn, PTmatrix pMatrix)
{
    // Necessaire ?
    if (unitLine > pMatrix->pUnitEnd->M || unitColumn > pMatrix->pUnitEnd->N)
    {
        return (NULL);
    }

    PTunit pUnit = pMatrix->pUnit00;
    while (pUnit->M != unitLine)
    {
        pUnit = pUnit->pDown;
    }
    while (pUnit->N != unitColumn)
    {
        pUnit = pUnit->pRight;
    }

    return (pUnit);
}

/* Insert une NOUVELLE matrice avant le bidon de fin de liste avec "unitLine" lignes et "unitColumn" colonnes ----------------------------
 */
PTmatrix Add_matrix(int unitLine, int unitColumn)
{
    int i;
    PTmatrix pMatrix = Allocate_memory_matrix();

    pMatrix->pNext = pEndMatrix;
    pMatrix->pPrevious = pEndMatrix->pPrevious;
    pEndMatrix->pPrevious->pNext = pMatrix;
    pEndMatrix->pPrevious = pMatrix;

    pMatrix->pUnit00 = Allocate_memory_unit();
    pMatrix->pUnit00->M = 0;
    pMatrix->pUnit00->N = 0;

    pMatrix->pUnitEnd = pMatrix->pUnit00;
    pMatrix->dataMaxAbs = 0;

    for (i = 0; i < (unitLine - 1); i++)
    {
        Add_row(pMatrix);
    }
    for (i = 0; i < (unitColumn - 1); i++)
    {
        Add_column(pMatrix);
    }

    numberOfMatrix++;
    return (pMatrix);
}

/* Insert une nouvelle ligne dans une matrice -----------------------------------------------------------------------------------
 */
void Add_row(PTmatrix pMatrix)
{
    PTunit pUnit = pMatrix->pUnitEnd;
    PTunit pNewUnit = Allocate_memory_unit();

    // initialisation des liens au dessus de "pNewUnit"
    pUnit->pDown = pNewUnit;
    pNewUnit->pUp = pUnit;
    // initialisation des liens a droite de "pNewUnit"
    pNewUnit->pRight = NULL;
    // initialisation des liens en dessous de "pNewUnit"
    pNewUnit->pDown = NULL;

    pNewUnit->N = pUnit->N;
    pNewUnit->M = pUnit->M + 1;

    pMatrix->pUnitEnd = pNewUnit;

    while (pUnit->N != 0)
    {
        pUnit = pUnit->pLeft;
        pNewUnit = Allocate_memory_unit();
        // initialisation des liens au dessus de "pNewUnit"
        pUnit->pDown = pNewUnit;
        pNewUnit->pUp = pUnit;
        // initialisation des liens a droite de "pNewUnit"
        pNewUnit->pRight = pUnit->pRight->pDown;
        pNewUnit->pRight->pLeft = pNewUnit;
        // initialisation des liens en dessous de "pNewUnit"
        pNewUnit->pDown = NULL;

        pNewUnit->N = pUnit->N;
        pNewUnit->M = pUnit->M + 1;
    }
    pNewUnit->pLeft = NULL;
}

/* Insert une nouvelle colonne dans une matrice ---------------------------------------------------------------------------------
Ou est le pUnit->pUp ?*/
void Add_column(PTmatrix pMatrix)
{
    PTunit pUnit = pMatrix->pUnitEnd;
    PTunit pNewUnit = Allocate_memory_unit();
    // initialisation des liens a gauche de "pNewUnit"
    pUnit->pRight = pNewUnit;
    pNewUnit->pLeft = pUnit;
    // initialisation des liens en dessous de "pNewUnit"
    pNewUnit->pDown = NULL;
    // initialisation des liens a droite de "pNewUnit"
    pNewUnit->pRight = NULL;
    // on met sur quelle ligne et quelle colonne se trouve "pNewUnit"
    pNewUnit->M = pUnit->M;
    pNewUnit->N = pUnit->N + 1;

    pMatrix->pUnitEnd = pNewUnit;

    while (pUnit->M != 0)
    {
        pUnit = pUnit->pUp;
        pNewUnit = Allocate_memory_unit();
        // initialisation des liens gauche de "pNewUnit"
        pUnit->pRight = pNewUnit;
        pNewUnit->pLeft = pUnit;
        // initialisation des liens au dessus de "pNewUnit"
        pNewUnit->pDown = pUnit->pDown->pRight;
        pNewUnit->pDown->pUp = pNewUnit;
        // initialisation des liens a droite de "pNewUnit"
        pNewUnit->pRight = NULL;
        // on met sur quelle ligne et quelle colonne se trouve "pNewUnit"
        pNewUnit->M = pUnit->M;
        pNewUnit->N = pUnit->N + 1;
    }
    pNewUnit->pUp = NULL;
}

/* Met à jour la valeur max absolue de la matrice en la comparant à la valeur donnée en paramètres ------------------------------
 */
void Compare_dataMax_to(float data, PTmatrix pMatrix)
{
    float dataAbs;

    dataAbs = abs(data);
    if (dataAbs > pMatrix->dataMaxAbs)
    {
        pMatrix->dataMaxAbs = dataAbs;
    }
}

/* On remplit la matrice --------------------------------------------------------------------------------------------------------
Les coordonnées doivent être à jour */
void Fill_matrix(PTmatrix pMatrix)
{
    int i = 0;
    PTunit pUnit;
    // on parcour les lignes de la matrice
    while (i <= pMatrix->pUnitEnd->M)
    {
        pUnit = Unit_at(i, 0, pMatrix);
        printf("\nEntrez le terme a la ligne %d et a la colonne 0 : ", i);

        scanf("%f", &pUnit->data);
        Compare_dataMax_to(pUnit->data, pMatrix);
        // on parcour les colonnes de la matrice
        while (pUnit->N != pMatrix->pUnitEnd->N)
        {
            pUnit = pUnit->pRight;
            printf("Entrez le terme a la ligne %d et a la colonne %d : ", i, pUnit->N);

            scanf("%f", &pUnit->data);
            Compare_dataMax_to(pUnit->data, pMatrix);
        }
        i = i + 1;
    }
}

/* Renvoie l'espace que prend un entier sur l'ecran du terminal -----------------------------------------------------------------
 */
int Space_of(int number)
{
    int numberAbs = abs(number);
    if (numberAbs < 10)
    {
        return (1);
    }

    return (log10(numberAbs) + 1);
}

/* Insert un maillon dans la poubelle -------------------------------------------------------------------------------------------
/!\ les pointeurs de maillon pointant sur "pUnit" reste sur "pUnit" /!\ */
void Remove_unit(PTunit pUnit)
{
    // liens côté maillon
    pUnit->pUp = NULL;
    pUnit->pDown = NULL;
    pUnit->pRight = pBENstartUnit->pRight;
    pUnit->pLeft = pBENstartUnit;
    // liens côté poubelle
    pBENstartUnit->pRight = pUnit;
    pUnit->pRight->pLeft = pUnit;
}

/* Met à la poubelle une ligne donnée de la matrice -----------------------------------------------------------------------------
Les coordonnées doivent être à jour
Après l'opération, les coordonnées ne sont pas mises à jour */
void Remove_row(int unitLine, PTmatrix pMatrix)
{
    if (unitLine > pMatrix->pUnitEnd->M)
    {
        return;
    }

    // "pUnitDelete" et "pNextUnitDelete" pointeront les maillons de la ligne a supprimée
    PTunit pUnitDelete, pNextUnitDelete;
    PTunit pUnit;
    pUnitDelete = Unit_at(unitLine, 0, pMatrix);

    // on commence par changer les pointeurs pointants sur la ligne à supprimée
    // on s'occupe d'abord de la ligne au dessus de "unitLine"
    if (unitLine != 0)
    {
        pUnit = pUnitDelete->pUp;
        pUnit->pDown = pUnit->pDown->pDown;
        while (pUnit->N != pMatrix->pUnitEnd->N)
        {
            pUnit = pUnit->pRight;
            pUnit->pDown = pUnit->pDown->pDown;
        }
    }
    else
    {
        pMatrix->pUnit00 = pMatrix->pUnit00->pDown;
    }
    // ensuite on s'occupe de la ligne au dessous de "unitLine"
    if (unitLine != pMatrix->pUnitEnd->M)
    {
        pUnit = pUnitDelete->pDown;
        pUnit->pUp = pUnit->pUp->pUp;
        while (pUnit->N != pMatrix->pUnitEnd->N)
        {
            pUnit = pUnit->pRight;
            pUnit->pUp = pUnit->pUp->pUp;
        }
    }
    else
    {
        pMatrix->pUnitEnd = pMatrix->pUnitEnd->pUp;
    }

    // on met la ligne "unitLine" a la poubelle
    while (pUnitDelete->N != pMatrix->pUnitEnd->N)
    {
        pNextUnitDelete = pUnitDelete->pRight;
        Remove_unit(pUnitDelete);
        pUnitDelete = pNextUnitDelete;
    }
    Remove_unit(pUnitDelete);
}

/* Met à la poubelle une colonne donnée de la matrice ---------------------------------------------------------------------------
Les coordonnées doivent être à jour
Après l'opération, les coordonnées ne sont pas mises à jour */
void Remove_column(int unitColumn, PTmatrix pMatrix)
{
    if (unitColumn > pMatrix->pUnitEnd->N)
    {
        return;
    }

    // "pUnitDelete" et "pNextUnitDelete" pointeront les maillons de la colonne à supprimée
    PTunit pUnitDelete, pNextUnitDelete;
    PTunit pUnit;
    pUnitDelete = Unit_at(0, unitColumn, pMatrix);

    // on commence par changer les pointeurs pointants sur la colonne à supprimée
    // on s'occupe d'abord de la colonne à gauche de "unitColumn"
    if (unitColumn != 0)
    {
        pUnit = pUnitDelete->pLeft;
        pUnit->pRight = pUnit->pRight->pRight;
        while (pUnit->M != pMatrix->pUnitEnd->M)
        {
            pUnit = pUnit->pRight;
            pUnit->pRight = pUnit->pRight->pRight;
        }
    }
    else
    {
        pMatrix->pUnit00 = pMatrix->pUnit00->pRight;
    }
    // ensuite on s'occupe de la colonne à droite de "unitColumn"
    if (unitColumn != pMatrix->pUnitEnd->N)
    {
        pUnit = pUnitDelete->pRight;
        pUnit->pLeft = pUnit->pLeft->pLeft;
        while (pUnit->M != pMatrix->pUnitEnd->M)
        {
            pUnit = pUnit->pRight;
            pUnit->pLeft = pUnit->pLeft->pLeft;
        }
    }
    else
    {
        pMatrix->pUnitEnd = pMatrix->pUnitEnd->pLeft;
    }

    // on met la colonne "unitColumn" à la poubelle
    while (pUnitDelete->M != pMatrix->pUnitEnd->M)
    {
        pNextUnitDelete = pUnitDelete->pDown;
        Remove_unit(pUnitDelete);
        pUnitDelete = pNextUnitDelete;
    }
    Remove_unit(pUnitDelete);
}

/* Met dans la poubelle la matrice donnée (tout comme ses maillons) -------------------------------------------------------------
*/
void Remove_matrix(PTmatrix pMatrix)
{
// à faire ...
}

/* Change les coordonnées d'un maillon par les coordonnées en paramètres --------------------------------------------------------
 */
void Set_unit_coord(int unitLine, int unitColumn, PTunit pUnit)
{
    pUnit->M = unitLine;
    pUnit->N = unitColumn;
}

/* Met à jour les coordonnées des maillons à droite du maillon donné en paramètre -----------------------------------------------
Cette fonction considère que les coordonnées du maillon donné sont les bonnes */
void Update_row_coord(PTunit pUnitLeft)
{
    int unitLine = pUnitLeft->M;
    int unitColumn = pUnitLeft->N;
    PTunit pUnit = pUnitLeft;

    while (pUnit->pRight != NULL)
    {
        pUnit = pUnit->pRight;
        unitColumn = unitColumn + 1;
        Set_unit_coord(unitLine, unitColumn, pUnit);
    }
}

/* Met à jour les coordonnées des maillons en bas du maillon donné en paramètre -----------------------------------------------
Cette fonction considère que les coordonnées du maillon donné sont les bonnes */
void Update_column_coord(PTunit pUnitUp)
{
    int unitLine = pUnitUp->M;
    int unitColumn = pUnitUp->N;
    PTunit pUnit = pUnitUp;

    while (pUnit->pDown != NULL)
    {
        pUnit = pUnit->pDown;
        unitLine = unitLine + 1;
        Set_unit_coord(unitLine, unitColumn, pUnit);
    }
}

/* Met à jour les coordonnées des maillons d'une matrice ------------------------------------------------------------------------
/!\ le pointeur sur le premier maillon a besoin d'être au bon endroit /!\ */
void Update_matirix_coord(PTmatrix pMatrix)
{
    int unitLine = 0;
    int unitColumn = 0;
    PTunit pUnit = pMatrix->pUnit00;

    // on met les coordonnées de "pUnit00" à (0,0)
    Set_unit_coord(0, 0, pUnit);
    // puis on met à jour toute la ligne et toute la colonne sur laquelle "pUnit00" se trouve
    Update_column_coord(pUnit);
    Update_row_coord(pUnit);

    // enfin on met à jour les autres lignes
    while (pUnit->pDown != NULL)
    {
        pUnit = pUnit->pDown;
        Update_row_coord(pUnit);
    }
}

/* Met à jour la valeur maximum absolue de la matrice donnée --------------------------------------------------------------------
Les coordonnées doivent être à jour */
void Update_data_max(PTmatrix pMatrix)
{
    int i = 0;
    PTunit pUnit;

    // on parcour les lignes de la matrice
    while (i <= pMatrix->pUnitEnd->M)
    {
        pUnit = Unit_at(i, 0, pMatrix);
        Compare_dataMax_to(pUnit->data, pMatrix);
        // on parcour les colonnes de la matrice
        while (pUnit->N != pMatrix->pUnitEnd->N)
        {
            pUnit = pUnit->pRight;
            Compare_dataMax_to(pUnit->data, pMatrix);
        }
        i = i + 1;
    }
}

// Interface ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Efface tout le terminal et met le curseur à la ligne 1 -----------------------------------------------------------------------
 */
void Clear_console()
{
    system("cls");
    Put_cursor_to(1, 0);
}

/* Renvoie l'entrée du clavier qu'elle soit simple ou double --------------------------------------------------------------------
471 = flèche retour chariot
472 = flèche HAUT <----------
473 = flèche aller en haut
475 = flèche GAUCHE <--------
477 = flèche DROITE <--------
479 = flèche fin de ligne
480 = flèche BAS <-----------
481 = flèche aller en bas
482 = touche "Inser"
483 = touche "Suppr"
*/
short int Input_keyboard()
{
    char c = getch();
    short int n = c;

    if (n != -32)
    {
        return (n);
    }

    c = getch();
    n = 400 + c;
    return (n);
}

/* Changer la couleur de l'écriture ---------------------------------------------------------------------------------------------
 */
void Change_text_color(int background, int front)
{
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (16 * background) + front);
}

/* Met le curseur à la ligne "line" et à la colonne "column" --------------------------------------------------------------------
 */
void Put_cursor_to(int line, int column)
{
    COORD mycoord;

    mycoord.X = column;
    mycoord.Y = line;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), mycoord);
}

/* Efface "n" lignes de l'interface à partir de la ligne "line" et remet le curseur sur la ligne "line" -------------------------
 */
void Empty_line(int line, int n)
{
    int i = 0;
    Put_cursor_to(line, 0);
    while (i < n)
    {
        printf("\33[2K\r\n");
        i++;
    }
    Put_cursor_to(line, 0);
}

/* Affiche les commandes de séléctions des menus --------------------------------------------------------------------------------
 */
void Display_commandes_menus()
{
    printf("\n\n\n\t\t'fleche du haut' : menu precedente");
    printf("\tfleche du bas : option suivante");
    printf("\t\t'ENTRER' : selectionner");
    printf("\t'ECHAP' : Quitter");
}

/* Efface le terminal et affiche le titre du menu principal ---------------------------------------------------------------------
 */
void Display_main_menu_title()
{
    Clear_console();
    printf("\t**********************************\n");
    printf("\t*         MENU PRINCIPAL         *\n");
    printf("\t**********************************\n");
}

/* Affiche le menu principal ----------------------------------------------------------------------------------------------------
 */
void Display_main_menu(int select)
{
    Empty_line(6, SIZE_MAIN_MENU + 2);
    if (select == 1)
    {
        Change_text_color(WHITE, BLACK);
        printf("\n\t1 - Ajouter une matrice");
        Change_text_color(BLACK, WHITE);
        printf("\n\t2 - Supprimer une matrice");
        printf("\n\t3 - Afficher les matrices\n");
        printf("\n\tESC - Quitter");
    }
    else if (select == 2)
    {
        printf("\n\t1 - Ajouter une matrice");
        Change_text_color(WHITE, BLACK);
        printf("\n\t2 - Supprimer une matrice");
        Change_text_color(BLACK, WHITE);
        printf("\n\t3 - Afficher les matrices\n");
        printf("\n\tESC - Quitter");
    }
    else if (select == 3)
    {
        printf("\n\t1 - Ajouter une matrice");
        printf("\n\t2 - Supprimer une matrice");
        Change_text_color(WHITE, BLACK);
        printf("\n\t3 - Afficher les matrices\n");
        Change_text_color(BLACK, WHITE);
        printf("\n\tESC - Quitter");
    }
    else
    {
        printf("\n\t1 - Ajouter une matrice");
        printf("\n\t2 - Supprimer une matrice");
        printf("\n\t3 - Afficher les matrices\n");
        Change_text_color(WHITE, BLACK);
        printf("\n\tESC - Quitter");
        Change_text_color(BLACK, WHITE);
    }

    Display_commandes_menus();
}

/* Renvoie le numéro de l'option à surligner en fonction de "inputKeyboard" -----------------------------------------------------
*/
short int Update_Highlight_option_menu(short int inputKeyboard, short int select, int sizeMenu)
{
    // '↑'
    if (inputKeyboard == 472 && select > 1)
    {
        return (select - 1);
    }
    // '↓'
    else if (inputKeyboard == 480 && select < sizeMenu)
    {
        return (select + 1);
    }
    // si l'utilisateur entre un chiffre
    else if (48 < inputKeyboard && inputKeyboard < (48 + sizeMenu))
    {
        return (inputKeyboard - 48);
    }
    else
    {
        return (select);
    }
}

/* Renvoie une variable pour la selection du menu principale --------------------------------------------------------------------
 */
short int Select_main_menu()
{
    short int select = 1;
    short int inputKeyboard;

    // tant que différent de 'echap'
    while (inputKeyboard != 27)
    {
        Display_main_menu(select);
        inputKeyboard = Input_keyboard();
        select = Update_Highlight_option_menu(inputKeyboard, select, SIZE_MAIN_MENU);

        // 'entrer'
        if (inputKeyboard == 13)
        {
            return (select);
        }
        // 'echap'
        else if (inputKeyboard == 27)
        {
            return (SIZE_MAIN_MENU);
        }
    }
}

/* Renvoie la matrice à surligner en fonction de "keyboardInput" ----------------------------------------------------------------
*/
PTmatrix Update_Highlight_matrix(short int inputKeyboard, PTmatrix pHighlightMatrix)
{
    // '↑'
    if (inputKeyboard == 472 && pHighlightMatrix->pPrevious != pStratMatrix)
    {
        return (pHighlightMatrix->pPrevious);
    }
    // '↓'
    else if (inputKeyboard == 480 && pHighlightMatrix->pNext != pEndMatrix)
    {
        return (pHighlightMatrix->pNext);
    }
}

/* Affiche l'entête de la liste des matrices ------------------------------------------------------------------------------------
*/
void Display_matrix_list_header()
{
    Change_text_color(DARK_BLUE, WHITE);
    printf("\tNom de la matrice");
    Change_text_color(BLACK , WHITE);
    printf("    | ");
    Change_text_color(DARK_GREEN, WHITE);
    printf("(nombre de ligne ; nombre de colonne)\n");
    Change_text_color(BLACK , WHITE);
}

/* Affiche la liste des matrices ------------------------------------------------------------------------------------------------
*/
void Display_matrix_list(PTmatrix pHighlightMatrix)
{
    if(pStratMatrix->pNext == pEndMatrix)
    {
        return;
    }

    PTmatrix pMatrix = pStratMatrix->pNext;

    while(pMatrix != pEndMatrix)
    {
        if(pMatrix == pHighlightMatrix)
        {
            Change_text_color(WHITE, BLACK);
            printf("\t%20s | (%d ; %d)\n",pMatrix->name, (pMatrix->pUnitEnd->M + 1), (pMatrix->pUnitEnd->N + 1));
            Change_text_color(BLACK, WHITE);
        }
        else
        {
            printf("\t%20s | (%d ; %d)\n",pMatrix->name, (pMatrix->pUnitEnd->M + 1), (pMatrix->pUnitEnd->N + 1));
        }

        pMatrix = pMatrix->pNext;
    }

    Display_matrix(pHighlightMatrix, 6, 70, NULL);
}

/* Affiche les données de la matrice à partir de la ligne et de la colonne données en paramètre ---------------------------------
Les coordonnées doivent être à jour */
void Display_matrix(PTmatrix pMatrix, int line, int column, PTunit pHighlightUnit)
{
    if(pMatrix == NULL)
    {
        return;
    }
    
    int i = 0;
    // espace + 1 que prend la valeur max de la matrice
    int dataMaxSpace = Space_of(floor(pMatrix->dataMaxAbs)) + 4;
    PTunit pUnit;

    // on parcour les lignes de la matrice
    while (i <= pMatrix->pUnitEnd->M)
    {
        pUnit = Unit_at(i, 0, pMatrix);

        Put_cursor_to(line + i, column);
        if (pUnit == pHighlightUnit)
        {
            Change_text_color(WHITE, BLACK);
            printf("%*.2f", dataMaxSpace, pUnit->data);
            Change_text_color(BLACK, WHITE);
        }
        else
        {
            printf("%*.2f", dataMaxSpace, pUnit->data);
        }

        // on parcour les colonnes de la matrice
        while (pUnit->N != pMatrix->pUnitEnd->N)
        {
            pUnit = pUnit->pRight;
            if (pUnit == pHighlightUnit)
            {
                Change_text_color(WHITE, BLACK);
                printf("%*.2f", dataMaxSpace, pUnit->data);
                Change_text_color(BLACK, WHITE);
            }
            else
            {
                printf("%*.2f", dataMaxSpace, pUnit->data);
            }
        }
        i = i + 1;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                            MAIN                                                             //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void main()
{
    char OK;
    short int select = 1;
    short int inputKeyboard;
    int i, j;
    int unitLine, unitColumn;
    PTmatrix pMatrix = NULL;

    // Allocation de la mémoire pour les bidons de la liste des matrices
    pStratMatrix = (PTmatrix)malloc(sizeof(*pStratMatrix));
    pEndMatrix = (PTmatrix)malloc(sizeof(*pEndMatrix));
    // Initiallisation des bidons de la liste des matrices
    pStratMatrix->pNext = pEndMatrix;
    pEndMatrix->pPrevious = pStratMatrix;
    pStratMatrix->pPrevious = NULL;
    pEndMatrix->pNext = NULL;
    pStratMatrix->pUnit00 = NULL;
    pEndMatrix->pUnit00 = NULL;
    pStratMatrix->pUnitEnd = NULL;
    pEndMatrix->pUnitEnd = NULL;
    // Allocation de la mémoire pour les bidons des listes poubelles
    pBENstartMatrix = (PTmatrix)malloc(sizeof(*pBENstartMatrix));
    pBENendMatrix = (PTmatrix)malloc(sizeof(*pBENendMatrix));
    pBENstartUnit = (PTunit)malloc(sizeof(*pBENstartUnit));
    pBENendUnit = (PTunit)malloc(sizeof(*pBENendUnit));
    // Initialisation des bidons des listes poubelles
    pBENstartMatrix->pNext = pBENendMatrix;
    pBENendMatrix->pPrevious = pBENstartMatrix;
    pBENstartMatrix->pPrevious = NULL;
    pBENendMatrix->pNext = NULL;
    pBENstartMatrix->pUnit00 = NULL;
    pBENendMatrix->pUnit00 = NULL;
    pBENstartMatrix->pUnitEnd = NULL;
    pBENendMatrix->pUnitEnd = NULL;
    pBENstartUnit->pRight = pBENendUnit;
    pBENendUnit->pLeft = pBENstartUnit;
    pBENstartUnit->pLeft = NULL;
    pBENendUnit->pRight = NULL;
    pBENstartUnit->pUp = NULL;
    pBENstartUnit->pDown = NULL;
    pBENendUnit->pUp = NULL;
    pBENendUnit->pDown = NULL;

    while (select != SIZE_MAIN_MENU)
    {
        Display_main_menu_title();
        select = Select_main_menu();
        // Ajouter une matrice --------------------------------------------------------------------------------------------------
        if (select == 1)
        {
            Clear_console();
            Change_text_color(LIGHT_GREEN, BLACK);
            printf("\t--- Ajouter une matrice ---\n\n\n");
            Change_text_color(BLACK, WHITE);

            printf("Entrez le nombre de ligne de votre matrice : ");
            scanf("%d", &unitLine);
            printf("Entrez le nombre de colonne de votre matrice : ");
            scanf("%d", &unitColumn);
            pMatrix = Add_matrix(unitLine, unitColumn);

            Empty_line(4, 4);
            printf("Donnez un nom a votre matrice : ");
            scanf("%s", &(pMatrix->name));
            Fill_matrix(pMatrix);

            Empty_line(4, unitLine * (unitColumn + 1) + 1);
            Display_matrix(pMatrix, 3, 0, NULL);
        }
        // Supprimer une matrice ------------------------------------------------------------------------------------------------
        else if (select == 2)
        {
            // faire la fonction ...
        }
        // Afficher la liste des matrices ---------------------------------------------------------------------------------------
        else if (select == 3 && pStratMatrix->pNext != pEndMatrix)
        {
            
            pMatrix = pStratMatrix->pNext;
            Clear_console();
            Change_text_color(LIGHT_GREEN, BLACK);
            printf("\t--- Votre liste de matrice ---\n\n\n");
            Change_text_color(BLACK, WHITE);

            Display_matrix_list_header();
            // tant que l'entrée clavier est différente de 'ECHAP'
            while(inputKeyboard != 27)
            {   
                inputKeyboard = Input_keyboard();
                Empty_line(6, numberOfMatrix);
                pMatrix = Update_Highlight_matrix(inputKeyboard, pMatrix);
                Display_matrix_list(pMatrix);
            }
        }
        if (select != SIZE_MAIN_MENU)
        {
            Change_text_color(BLACK, ORANGE);
            printf("\n\n\nAppuiez sur une touche pour continuez ...");
            Change_text_color(BLACK, WHITE);
            OK = getch();
        }
    }
}