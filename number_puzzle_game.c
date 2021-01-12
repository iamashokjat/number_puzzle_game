#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <unistd.h>

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

#define HORIZONTALLINE 196
#define DHORIZONTALLINE 205
#define VERTICALLINE 179

//Node to be pushed to the stack containing the new_row,new_col,old_row,old_col
struct node {
   int new_row;
   int new_col;
   int old_row;
   int old_col;
   struct node * next;
}* top, * top1, * temp;

//Node to fix the control of empty space after undo operation
struct Pair {
   int row;
   int col;
}* new_pair;

//Function to push the previous operation into the stack.
void push(int new_row, int new_col, int old_row, int old_col) {

   if (top == NULL) {
      top = (struct node * ) malloc(sizeof(struct node));
      top -> next = NULL;
      top -> new_row = new_row;
      top -> new_col = new_col;
      top -> old_row = old_row;
      top -> old_col = old_col;
   } else {
      temp = (struct node * ) malloc(sizeof(struct node));
      temp -> new_row = new_row;
      temp -> new_col = new_col;
      temp -> old_row = old_row;
      temp -> old_col = old_col;
      temp -> next = top;
      top = temp;
   }
}

void swap(int * a, int * b) {
   int temp = * a;
   * a = * b;
   * b = temp;
}

//function to print and check the game_box for displaying result
int print_and_check(int ** game_box, int size, int * move) {
   system("cls");
   int count = 0, i, j, k, l;
   int final = 1;

   printf("\t\t\t");

   for (i = 0; i < 60; i++) {
      printf("%c", DHORIZONTALLINE);
   }

   printf("\n\n\t\t\t\t\t\tPUZZLE GAME\n\n");

   printf("\t\t\t");
   for (i = 0; i < 60; i++) {
      printf("%c", DHORIZONTALLINE);
   }
   printf("\n\t\t\tMOVES:%d\t\t\t\tUNDO(Double Press Z)\n\n", * move);

   printf("\n\n\n\t\t\t\t\t   ");
   for (k = 0; k < size * 2 - 1; k++) {
      printf(" ");
      printf("%c", HORIZONTALLINE);
      printf(" ");
   }
   printf("\n");

   for (i = 0; i < size; i++) {
      printf("\t\t\t\t\t   %c", VERTICALLINE);
      for (j = 0; j < size; j++) {

         if (game_box[i][j] == -1) {
            printf("    ");
         } else {
            printf("%3d ", game_box[i][j]);
         }

         if (j != size - 1) {
            printf("%c", VERTICALLINE);
         }

         if (game_box[i][j] == final++) {
            count++;
         }
      }
      printf("%c\n\t\t\t\t\t   ", VERTICALLINE);
      for (k = 0; k < size * 2 - 1; k++) {
         printf(" ");
         printf("%c", HORIZONTALLINE);
         printf(" ");
      }

      printf("\n");
   }

   return count;
}

//function to undo the previous operation using stack
struct Pair * undo(int ** game_box, int size, int * move) {
   top1 = top; //Temparary storing the top in top1

   if (top1 == NULL) {
      printf("\a");
      return NULL;
   } else
      top1 = top1 -> next;

   new_pair = (struct Pair * ) malloc(sizeof(struct Pair));
   int new_row = top -> new_row;
   int new_col = top -> new_col;
   int old_row = top -> old_row;
   int old_col = top -> old_col;

   new_pair -> row = old_row;
   new_pair -> col = old_col;

   swap( & game_box[new_row][new_col], & game_box[old_row][old_col]);
   --( * move);
   free(top);
   top = top1;
   return new_pair;
}

//function to shuffle the game_box ensuring it is solvable
void shuffle_box(int ** game_box, int difficulty, int size) {
   int i, j;
   srand(time(0));

   int prev_row_pos = size - 1, prev_col_pos = size - 1, new_x, new_y;

   int lower = 0, upper = 3, count = 0;
   int moves[] = {
      UP,
      DOWN,
      LEFT,
      RIGHT
   };
   while (count < difficulty * 20) {
      int selected_move = (rand() % (upper - lower + 1)) + lower;
      int move = moves[selected_move];
      switch (move) {

      case UP:
         new_x = prev_row_pos + 1;
         new_y = prev_col_pos;
         break;
      case DOWN:
         new_x = prev_row_pos - 1;
         new_y = prev_col_pos;
         break;
      case LEFT:
         new_x = prev_row_pos;
         new_y = prev_col_pos + 1;
         break;
      case RIGHT:
         new_x = prev_row_pos;
         new_y = prev_col_pos - 1;
         break;
      default:
         new_x = prev_row_pos;
         new_y = prev_col_pos;
         break;
      }
      if (new_x >= size || new_x < 0 || new_y >= size || new_y < 0) {
         i--;
         continue;
      }
      swap( & game_box[prev_row_pos][prev_col_pos], & game_box[new_x][new_y]);
      count++;
      prev_row_pos = new_x;
      prev_col_pos = new_y;
   }
}

// function to select the mode
int select_mode() {
   int mode, i;

   printf("\t\t\t");

   for (i = 0; i < 50; i++) {
      printf("%c", 205);
   }

   printf("\n\n\t\t\t\t\tSELECT GAME MODE\n\n");
   printf("\t\t\t");

   for (i = 0; i < 50; i++) {
      printf("%c", 205);
   }

   printf("\n\n\t\t\t\t\t   1. 3X3\n\n");
   printf("\t\t\t\t\t   2. 4X4\n\n");
   printf("\t\t\t\t\t   3. 5X5\n\n");
   printf("\n\t\t\t\tEnter option:");
   scanf("%d", & mode);

   system("cls");

   return mode;
}

//function to select the difficulty
int select_difficulty() {
   int difficulty, i;
   printf("\t\t\t");

   for (i = 0; i < 50; i++) {
      printf("%c", DHORIZONTALLINE);
   }

   printf("\n\n\t\t\t\t\tSELECT DIFFICULTY\n\n");

   printf("\t\t\t");
   for (i = 0; i < 50; i++) {
      printf("%c", DHORIZONTALLINE);
   }
   printf("\n\n\t\t\t\t\t   1. EASY\n\n");
   printf("\t\t\t\t\t   2. MEDIUM\n\n");
   printf("\t\t\t\t\t   3. HARD\n\n");
   printf("\n\t\t\t\tEnter option:");
   scanf("%d", & difficulty);
   system("cls");

   return difficulty;
}

//function to display the instruction screen
void instructions() {
   printf("\t\t\t");
   int i;
   int c;
   for (i = 0; i < 50; i++) {
      printf("%c", DHORIZONTALLINE);
   }

   printf("\n\n\t\t\t\t\tInstructions\n\n");

   printf("\t\t\t");
   for (i = 0; i < 50; i++) {
      printf("%c", DHORIZONTALLINE);
   }

   printf("\n\n\t\t\t 1.Given a n×n board with n*n-1 tiles (every tile");
   printf("\n\t\t\t   has one number from 1 to 15)  Eg:3X3 puzzle range");
   printf("\n\t\t\t   from 1 to 8 and one empty space. \n");
   printf("\n\t\t\t 2.The objective is to place the numbers on tiles ");
   printf("\n\t\t\t   in order using the empty space \n");

   printf("\n\t\t\t 3. You can slide four adjacent (left, right, above ");
   printf("\n\t\t\t    and below) tiles into the empty space\n\n\n");

   printf("\n\t\t\t->  Use Arrow Keys for movement.\n");
   printf("\n\t\t\t->  Double Press Z for Undo movement.\n");

   printf("\n\n\n\t\t\t\t   Press SPACE BAR to start.");
   c = getch();

   if (c == 32) {
      system("cls");
      return;
   } else //if any other key is pressed other than space bar
   {
      printf("\a");
      system("cls");
      instructions();
   }
}

//function to display game title
void game_title() {
   int i;
   for (i = 0; i < 10; i++) {
      printf("\n");
   }
   printf("\t\t\t\t");

   for (i = 0; i < 50; i++) {
      printf("*");
   }

   printf("\n\n\t\t\t\t\t   ");

   char title[] = "NUMBER PUZZLE GAME";
   int len = strlen(title);
   for (i = 0; i < len; i++) {
      printf("%c", title[i]);
      usleep(250000);
   }
   usleep(250000);
   printf("\n\n\t\t\t\t");

   for (i = 0; i < 50; i++) {
      printf("*");
   }
   usleep(2000000);
   system("cls");
}

int main() {
   int i, j;
   int difficulty, mode, size;

   game_title(); //first screen of the game

   instructions(); //second screen of the game

   mode = select_mode(); //third screen of the game
   while (1) {
      if (mode >= 1 && mode <= 3) {
         break;
      } else {
         printf("\a");
         mode = select_mode();
      }
   }
   if (mode == 1) {
      size = 3;
   } else if (mode == 2) {
      size = 4;
   } else {
      size = 5;
   }

   difficulty = select_difficulty(); //third screen of the game
   while (1) {
      if (difficulty >= 1 && difficulty <= 3) {
         break;
      } else {
         printf("\a");
         difficulty = select_difficulty();
      }
   }

   int ** game_box = (int ** ) malloc(size * sizeof(int * ));
   for (i = 0; i < size; i++) {
      game_box[i] = (int * ) malloc(size * sizeof(int));
   }

   int number = 1;

   for (i = 0; i < size; i++) {
      for (j = 0; j < size; j++) {
         game_box[i][j] = number++;
      }
   }

   game_box[size - 1][size - 1] = -1;

   int moves_count = 0;
   char key;

   printf("\n");

   //shuffling the game_box
   shuffle_box(game_box, difficulty, size);
   print_and_check(game_box, size, & moves_count);

   int row, col;

   for (row = 0; row < size; row++) {
      int flag = 0;
      for (col = 0; col < size; col++) {
         if (game_box[row][col] == -1) {
            flag = 1;
            break;
         }
      }
      if (flag == 1) {
         break;
      }
   }
   int old_row = row, old_col = col;
   int new_row, new_col;

   while (1) {
      int input;
      getch();
      input = getch();

      switch (input) {
      case KEY_UP:
         new_row = old_row + 1;
         new_col = old_col;
         break;
      case KEY_DOWN:
         new_row = old_row - 1;
         new_col = old_col;
         break;
      case KEY_LEFT:
         new_row = old_row;
         new_col = old_col + 1;
         break;
      case KEY_RIGHT:
         new_row = old_row;
         new_col = old_col - 1;
         break;
      case 'z': {
         struct Pair * pair = undo(game_box, size, & moves_count);
         system("cls");
         if (pair == NULL) {
            print_and_check(game_box, size, & moves_count);
            continue;
         }
         old_row = pair -> row;
         old_col = pair -> col;
         print_and_check(game_box, size, & moves_count);
         break;
      }

      default:
         printf("\t\t\t\tINVALID KEYS: PLEASE USE ARROW KEYS UP,DOWN,LEFT and RIGHT.");
         break;
      }
      if (input == 'z') {
         continue;
      }
      if (new_row >= size || new_row < 0 || new_col >= size || new_col < 0) {
         printf("\a");
         printf("\n\n\n\t\t\t\t\t");
         for (i = 0; i < 25; i++) {
            printf("*");
         }
         printf("\n\t\t\t\t\t\tILLEGAL MOVE\n");

         printf("\t\t\t\t\t");
         for (i = 0; i < 25; i++) {
            printf("*");
         }
         continue;
      }
      moves_count += 1;
      swap( & game_box[old_row][old_col], & game_box[new_row][new_col]);
      push(new_row, new_col, old_row, old_col);
      old_row = new_row;
      old_col = new_col;

      int count = print_and_check(game_box, size, & moves_count);

      if (count == size * size - 1) {
         printf("\n\n\n\t\t\t\t\t");
         for (i = 0; i < 25; i++) {
            printf("*");
         }
         printf("\n\t\t\t\t\t\tYOU WON!!!\n");

         printf("\t\t\t\t\t");
         for (i = 0; i < 25; i++) {
            printf("*");
         }
         break;
      }

   }

   return 0;
}