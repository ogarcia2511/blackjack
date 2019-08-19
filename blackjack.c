#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void welcome();
void draw_card(int *);

void main()
{
  /* setup of the deck */

  int deck[52];
  int deck_setup = 2;

  /* aces are set up first; given a value of -1 as placeholder */

  for(int i = 0; i < 4; i++)
  {
    deck[i] = -1;
  }

  /* rest of the deck set up here */

  for(int i = 4; i < 40; i = i + 4)
  {
    deck[i] = deck_setup;
    deck[i + 1] = deck_setup;
    deck[i + 2] = deck_setup;
    deck[i + 3] = deck_setup;
    deck_setup++;
  }

  for(int i = 40; i < 52; i++)
  {
    deck[i] = 10;
  }

  for(int i = 0; i < 52; i++)
    printf("%d \n", deck[i]);

  welcome();

  draw_card(&deck);
}

void welcome()
{
  printf("Welcome to blackjack! \n");
}

void draw_card(int *deck)
{
  srand((int)time(NULL));
  int count;
  for(i = 0; i < 52; i++)
  {
    if((deck + 1) != NULL)
      count++;

  }
    printf("array has %d elts", count);

}
