////////////////////////////////////////////////////////////////////////////////
//
// blackjack.c:
// A fully functional blackjack game, played with a 52-card deck.
//
// written by Omar Garcia (ogarcia@scu.edu)
//
// copyright 2018, Omar Garcia, all rights reserved.
////////////////////////////////////////////////////////////////////////////////

#ifdef _WIN32
#include <conio.h>
#else
#include <stdio.h>
#define clrscr() printf("\e[1;1H\e[2J")
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

void welcome();
void play_game(int *, char *);
int draw_card(int *, char *, char *);
void make_a_card(int, char *);
bool get_action(int *, char *, int *, int *, int *);
void dealer_actions(int *, char *, int *, int *, int *);
int did_you_win(int *, int *);

int main()
{
  /* setup of the deck */

  int *deck;
  char *royalty;

  int deck_setup = 2;
  int royalty_setup = 0;

  deck = malloc(53 * sizeof(int));
  royalty = malloc(53 * sizeof(char));

  /* first elt is the size of the pointer array: starts at 53
   * but will decrease as cards are drawn */

  *(deck + 0) = 52;
  *(royalty + 0) = '0';

  /* aces are set up first; given a value of 11 as placeholder */

  for(int i = 1; i < 5; i++)
  {
    *(deck + i) = 11;
    *(royalty + i) = 'A';
  }

  /* rest of the deck set up here */

  for(int i = 5; i < 41; i = i + 4)
  {
    *(deck + i) = deck_setup;
    *(deck + (i + 1)) = deck_setup;
    *(deck + (i + 2)) = deck_setup;
    *(deck + (i + 3)) = deck_setup;
    deck_setup++;

    royalty[i] = '0';
  }

  for(int i = 41; i < 53; i++)
  {
    deck[i] = 10;

    if(i == 45 || i == 49)
    {
      royalty_setup++;
    }

    if(royalty_setup == 0)
    {
      royalty[i] = 'J';
    }
    else if(royalty_setup == 1)
    {
      royalty[i] = 'Q';
    }
    else if(royalty_setup == 2)
    {
      royalty[i] = 'K';
    }
  }

  welcome();

  play_game(deck, royalty);

  return 0;
}

void welcome()
{
  char response[256];

  printf("Welcome to blackjack! \n");
  printf("Type START to begin the game; \n");
  printf("available commands during play are \n");
  printf("HIT, STAND, and QUIT. \n");
  printf(" > ");
  scanf("%s", response);

  while(strcasecmp(response, "START") != 0)
  {
    printf("Type START to begin.\n");
    printf(" > ");
    scanf("%s", response);
  }

  return;
}

void play_game(int *deck, char *royalty)
{
  int you_won;
  bool do_we_quit;
  bool hide_first;

  int score_wins = 0;
  int score_losses = 0;
  int round_number = 0;

  int your_card1;
  int your_card2;
  int your_count;

  int dealer_card1;
  int dealer_card2;
  int dealer_count;

  char response[256];
  char *check_royalty1;
  char *check_royalty2;
  char *check_royalty3;
  char *check_royalty4;

  check_royalty1 = malloc(sizeof(char));
  check_royalty2 = malloc(sizeof(char));
  check_royalty3 = malloc(sizeof(char));
  check_royalty4 = malloc(sizeof(char));

  while(1)
  {
    clrscr();

    round_number++;
    printf("\n \nR O U N D # %d\n", round_number);

    *check_royalty1 = '0';
    *check_royalty2 = '0';
    *check_royalty3 = '0';
    *check_royalty4 = '0';

    dealer_card1 = draw_card(deck, royalty, check_royalty1);
    dealer_card2 = draw_card(deck, royalty, check_royalty2);
    dealer_count = dealer_card1 + dealer_card2;

    your_card1 = draw_card(deck, royalty, check_royalty3);
    your_card2 = draw_card(deck, royalty, check_royalty4);
    your_count = your_card1 + your_card2;

    printf(":::::::::::::::::::::::::\n");
    make_a_card(dealer_card2, check_royalty2);
    printf("Dealer's Hand [COUNT: ???]\n");

    printf("\n\n");

    printf(":::::::::::::::::::::::::\n");
    make_a_card(your_card1, check_royalty3);
    make_a_card(your_card2, check_royalty4);
    printf("Your Hand [COUNT: %d]\n", your_count);

    do_we_quit = get_action(deck, royalty, &your_card1, &your_card2, &your_count);

    if(do_we_quit == true)
    {
      printf("Thanks for playing! Exiting game...\n");
      free(deck); //double check this
      free(royalty);
      free(check_royalty1);
      free(check_royalty2);
      free(check_royalty3);
      free(check_royalty4);
      return;
    }

    /* tests early if player has busted */

    if(your_count > 21)
    {
      printf("You have busted! You lose this round!\n");
      score_losses++;
      printf("type NEXT to move to the next round.\n");
      printf(" > ");
      scanf("%s", response);

      while(strcasecmp(response, "NEXT") != 0)
      {
        printf("Type NEXT to continue.\n");
        printf(" > ");
        scanf("%s", response);
      }
      continue;
    }

    printf("DEALER'S FULL HAND: \n");

    /* show the dealer's hand in full */

    printf(":::::::::::::::::::::::::\n");
    make_a_card(dealer_card1, check_royalty1);
    make_a_card(dealer_card2, check_royalty2);
    printf("Dealer's Hand [COUNT: %d]\n", dealer_count);

    dealer_actions(deck, royalty, &dealer_card1, &dealer_card2, &dealer_count);

    if(dealer_count > 21)
    {
      printf("Dealer has busted! You win this round!\n");
      score_wins++;
      printf("type NEXT to move to the next round.\n");
      printf(" > ");
      scanf("%s", response);

      while(strcasecmp(response, "NEXT") != 0)
      {
        printf("Type NEXT to continue.\n");
        printf(" > ");
        scanf("%s", response);
      }
      continue;
    }

    //if no bust, compare scores (make a function for this)
    you_won = did_you_win(&your_count, &dealer_count);

    if(you_won == 1)
    {
      printf("Congrats! You win this round!\n");
      score_wins++;
      printf("type NEXT to move to the next round.\n");
      printf(" > ");
      scanf("%s", response);

      while(strcasecmp(response, "NEXT") != 0)
      {
        printf("Type NEXT to continue.\n");
        printf(" > ");
        scanf("%s", response);
      }
      continue;
    }
    else if(you_won == -1)
    {
      printf("It's a tie! No one wins.\n");
      printf("type NEXT to move to the next round.\n");
      printf(" > ");
      scanf("%s", response);

      while(strcasecmp(response, "NEXT") != 0)
      {
        printf("Type NEXT to continue.\n");
        printf(" > ");
        scanf("%s", response);
      }
      continue;
    }
    else
    {
      printf("Sorry! You lost this round...\n");
      score_losses++;
      printf("type NEXT to move to the next round.\n");
      printf(" > ");
      scanf("%s", response);

      while(strcasecmp(response, "NEXT") != 0)
      {
        printf("Type NEXT to continue.\n");
        printf(" > ");
        scanf("%s", response);
      }
      continue;
    }
  }

}

/* draw_card: takes the deck, draws a random card, reduces deck size by 1. */
// implement error handling! (no cards left)
int draw_card(int *deck, char *royalty, char *check_royalty)
{
  srand((int)time(NULL));

  /* deck_size is size of deck in INTS */

  int deck_size = *(deck + 0);
  int draw_this = rand() % deck_size;

  /* edge case: this checks for an empty deck and closes the program if true */

  if(deck_size == 0)
  {
      printf("\n================================================\n");
      printf("CRITICAL ERROR: There are no more cards in the deck!\n");
      printf("Thanks for playing! The game will automatically close.\n");
      printf("\n================================================\n");
      exit(0);
  }

  /* ensures we don't try to draw the size of the deck
   * through a constant reroll (in the first slot) */

  while(draw_this == 0)
  {
    sleep(1);
    draw_this = rand() % deck_size;
  }

  int return_this = *(deck + draw_this);

  for(int i = return_this; i < (deck_size - 1); i++)
  {
    *(deck + i) = *(deck + (i + 1));
  }

  deck = realloc(deck, (deck_size - 1) * sizeof(int));
  if(!deck)
  {
    printf("\n================================================\n");
    printf("CRITICAL ERROR: Memory reallocation failed!");
    printf("Thanks for playing! The game will automatically close.");
    printf("\n================================================\n");
    exit(0);
  }
  *(deck + 0) = deck_size - 1;

  if(*(royalty + draw_this) != '0')
  {
    *check_royalty = *(royalty + draw_this);
  }
  else
    *check_royalty = '0';

  return return_this;
}

void make_a_card(int card1, char *check_royalty)
{
    if(*check_royalty == 'A' || *check_royalty == 'J'
      || *check_royalty == 'Q' || *check_royalty == 'K')
    {
      printf("\n");
      printf(" ------ \n");
      printf("|      |\n");
      printf("|  %c   |\n", *check_royalty);
      printf("|      |\n");
      printf(" ------ \n");
      return;
    }
    else if(card1 == 10)
    {
      printf("\n");
      printf(" ------ \n");
      printf("|      |\n");
      printf("|  %d  |\n", card1);
      printf("|      |\n");
      printf(" ------\n");
      return;
    }
    else
    {
      printf("\n");
      printf(" ------ \n");
      printf("|      |\n");
      printf("|  %d   |\n", card1);
      printf("|      |\n");
      printf(" ------ \n");
      return;
    }
}

bool get_action(int *deck, char *royalty, int *your_card1, int *your_card2, int *your_count)
{
  char response[256];
  char *check_royalty;
  check_royalty = malloc(sizeof(char));

  while(1)
  {
    printf("What do you want to do? \n");
    printf(" > ");
    scanf("%s", response);

    int new_card;

    if(strcasecmp(response, "HIT") == 0)
    {
      printf("\nDrawing one card... \n");
      new_card = draw_card(deck, royalty, check_royalty);
      make_a_card(new_card, check_royalty);


      /* conditions for losing if over 21 */
      if((*your_count + new_card) > 21)
      {
        if(*your_card1 == 11)
        {
          *your_card1 = 1;
          *your_count += new_card;
          printf("Your Hand [COUNT: %d]\n", *your_count);
        }
        else if(*your_card2 == 11)
        {
          *your_card2 = 1;
          *your_count += new_card;
          printf("Your Hand [COUNT: %d]\n", *your_count);
        }
        else if(new_card == 11)
        {
          new_card = 1;
          *your_count += new_card;
          printf("Your Hand [COUNT: %d]\n", *your_count);
        }
        else
        {
          *your_count += new_card;
          printf("Your Hand [COUNT: %d]\n", *your_count);
          free(check_royalty);
          return false;
        }
      }
      else
      {
        *your_count += new_card;
        printf("Your Hand [COUNT: %d]\n", *your_count);
      }

      continue;
    }
    else if(strcasecmp(response, "STAND") == 0)
    {
      free(check_royalty);
      return false;
    }
    else if(strcasecmp(response, "QUIT") == 0)
    {
      free(check_royalty);
      return true;
    }
    else
    {
      printf("ERROR: Available commands are HIT, STAND, and QUIT!\n");
      continue;
    }
  }
}

void dealer_actions(int *deck, char *royalty, int *dealer_card1, int *dealer_card2, int *dealer_count)
{
  int new_card;
  char *check_royalty;
  check_royalty = malloc(sizeof(char));

  while(1)
  {
    sleep(1);

    if(*dealer_count >= 17 && *dealer_count <= 21)
    {
      printf("Dealer's Hand [COUNT: %d]\n", *dealer_count);
      return;
    }

    /* tests for aces (soft hand) before booting the dealer */

    else if(*dealer_count > 21)
    {
      if(*dealer_card1 == 11)
      {
        *dealer_card1 = 1;
        *dealer_count = *dealer_card1 + *dealer_card2;
        continue;
      }
      else if(*dealer_card2 == 11)
      {
        *dealer_card2 = 1;
        *dealer_count = *dealer_card1 + *dealer_card2;
        continue;
      }
      else
      {
        printf("Dealer's Hand [COUNT: %d]\n", *dealer_count);
        return;
      }
    }

    else
    {
      printf("Dealer draws a card! \n");
      new_card = draw_card(deck, royalty, check_royalty);
      make_a_card(new_card, check_royalty);
      *dealer_count += new_card;

      sleep(1);
      continue;
    }
  }
}

/* did_you_win: for use with the scoring method. TIES COUNT AS WINS */

int did_you_win(int* your_count, int *dealer_count)
{
  if(*your_count > *dealer_count)
  {
    return 1;
  }
  else if(*your_count == *dealer_count)
  {
    return -1;
  }

  return 0;
}
