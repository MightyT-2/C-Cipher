/***************************************************************************************/
/*                                                                                     */
/* Cipher Program in C, written by Timothy Powell                                      */
/*  A command line program                                                             */
/*                                                                                     */
/*     Created: January 27, 2025                                                       */
/* Last Edited: January 29, 2025                                                       */
/*                                                                                     */
/* This is adapted from a program with the same function written in C++. It asks the   */
/* user whether to encrypt or decrypt, lets him choose an encryption method, and       */
/* outputs both the cipertext and the plaintext.                                       */
/*                                                                                     */
/***************************************************************************************/

/***************************************************************************************/
/*                                      LIBRARIES                                      */
/***************************************************************************************/
#include <ctype.h>  /* isalpha(), tolower()                                            */
#include <stdio.h>  /* fgets(), printf(), scanf(), stdin                               */
#include <stdlib.h> /* exit(), free(), malloc()                                        */

/***************************************************************************************/
/*                                      CONSTANTS                                      */
/***************************************************************************************/
#define CAESAR    'c' /* Used to select the caesar cipher                              */
#define VIGENERE  'v' /* Used to select the vigenere cipher                            */
#define ENCODE    'e' /* Used to select the encoding action                            */
#define DECODE    'd' /* Used to select the decoding action                            */
#define UPPER     'u' /* Used to determine if a character is upper case                */
#define LOWER     'l' /* Used to determine if a character is lower case                */
#define NEITHER   'n' /* Used to determine if a character is not a letter              */
#define LOWER_INT 97  /* Used to make a lowercase letter usable in calculations        */
#define UPPER_INT 65  /* Used to make an uppercase letter usable in calculations       */

/***************************************************************************************/
/*                                       STRUCTS                                       */
/***************************************************************************************/
/* Struct to hold the string in a linked list                                          */
struct letter {
   char          character;       /* Holds the character in the link                   */
   struct letter *next_character; /* Points to the next letter in the list             */
};
typedef struct letter LETTER;

/***************************************************************************************/
/*                                FUNCTION DECLARATIONS                                */
/***************************************************************************************/
/* Gives the user instructions                                                         */
void   give_instructions();

/* Determines if the user wants to continue                                            */
char   user_response();

/* Determines which cipher the user wants to use                                       */
char   get_cipher();

/* Determines if the user wants to encode or decode                                    */
char   get_action();

/* Creates a string                                                                    */
LETTER *create_string();

/* Get the rotation for the caesar cipher                                              */
void   get_rotation(int *rotation);

/* Get the key for the vigenere cipher                                                 */
void   get_key(LETTER *key);

/* Gets a string from the user                                                         */
void   get_string(LETTER *text);

/* Encodes a message using the caesar cipher                                           */
void   encode_caesar_cipher(LETTER *plaintext, int rotation);

/* Decodes a message using the caesar cipher                                           */
void   decode_caesar_cipher(LETTER *ciphertext, int rotation);

/* Encodes a message using a vigenere cipher                                           */
void   encode_vigenere_cipher(LETTER *plaintext, LETTER *key);

/* Decodes a message using a vigenere cipher                                           */
void   decode_vigenere_cipher(LETTER *ciphertext, LETTER *key);

/* Prints a string                                                                     */
void   print_string(LETTER *text);

/* Deletes a string                                                                    */
void   clear_string(LETTER *text);

/* Tells the user goodbye                                                              */
void   farewell();

/* Tests if a character is uppercase or lowercase                                      */
char   test_character(char check);

/* Sets a string to lowercase                                                          */
void   to_lower(LETTER *string);

/***************************************************************************************/
/*                                    MAIN FUNCTION                                    */
/***************************************************************************************/
int    main() {
   LETTER *message, /* Points to the message to be encrypted or decrypted              */
          *key;     /* Points to the key used in encrypting or decrypting              */
   char   cipher,   /* Holds the user's choice of cipher                               */
          action;   /* Holds the user's choice of encryption or decryption             */
   int    rotation; /* Holds the rotation used in encrypting or decrypting             */

   /* Greet the user                                                                   */
   give_instructions();

   /* Loop until the user says to stop                                                 */
   while(user_response() == 'y') {

      /* Get the cipher, action to take, message to apply, and key/rotation            */
      cipher  = get_cipher();
      action  = get_action();
      message = create_string();
      get_string(message);
      if (cipher == CAESAR) {
         get_rotation(&rotation);
      } else if (cipher == VIGENERE) {
         key = create_string();
         get_key(key);
         to_lower(key);
      }

      /* Print the inputted message in its original form                               */
      if (action == ENCODE) {
         printf("\nPlaintext:  ");
      } else if (action == DECODE) {
         printf("\nCiphertext: ");
      }
      print_string(message);

      /* Apply the action with the caesar cipher                                       */
      if (cipher == CAESAR) {
         if (action == ENCODE) {
            encode_caesar_cipher(message, rotation);
         } else if (action == DECODE) {
            decode_caesar_cipher(message, rotation);
         } else {
            printf("\nError with Caesar Cipher.");
         }
      
      /* Apply the action with the vigenere cipher                                     */
      } else if (cipher == VIGENERE) {
         if (action == ENCODE) {
            encode_vigenere_cipher(message, key);
         } else if (action == DECODE) {
            decode_vigenere_cipher(message, key);
         } else {
            printf("\nError with Vigenere Cipher");
         }
      } else {
         printf("\nError with Ciphers.");
      }

      /* Print the inputted message in its changed form                                */
      if (action == 'e') {
         printf("\nCiphertext: ");
      } else {
         printf("\nPlaintext:  ");
      }
      print_string(message);

      /* Print the key/rotation                                                        */
      if (cipher == CAESAR) {
         printf("\nRotation:   %d", rotation);
      } else if (cipher == VIGENERE) {
         printf("\nKey:        ");
         print_string(key);
         clear_string(key);
      }

      /* Clear the string                                                              */
      clear_string(message);
   }

   /* Give the user a farewell                                                         */
   farewell();
   
   return 0;
}

/***************************************************************************************/
/*                                 FUNCTION DEFINITIONS                                */
/***************************************************************************************/
/* Gives the user instructions                                                         */
void   give_instructions() {
   printf("\n");
   printf("\n");
   printf("\nWelcome");
   printf("\n");
   printf("\nThis program takes in a message and either encodes into or decodes from");
   printf("\na cipher of your choice.");

   return;
}

/* Determines if the user wants to encode or decode                                    */
char   user_response() {
   char answer[2]; /* Holds the user's response                                        */

   printf("\n");
   printf("\nWould you like to continue (y/n)?");
   printf("\n");
   do {
      printf(" >> ");
      scanf("%1s", answer);
      answer[0] = tolower(answer[0]);
   } while (answer[0] != 'y' && answer[0] != 'n');

   return answer[0];
}

/* Determines which cipher the user wants to use                                       */
char   get_cipher() {
   char cipher[2]; /* Holds the user's response                                        */

   printf("\n");
   printf("\nWhich cipher would you like to use?");
   printf("\n c) Caesar Cipher");
   printf("\n v) Vigenere Cipher");
   printf("\n");
   do {
      printf(" >> ");
      scanf("%1s", cipher);
      cipher[0] = tolower(cipher[0]);
   } while (cipher[0] != 'c' && cipher[0] != 'v');

   return cipher[0];
}

/* Determines if the user wants to encode or decode                                    */
char   get_action() {
   char action[2]; /* Holds the user's response                                        */

   printf("\n");
   printf("\nWhat would you like to do?");
   printf("\n e) Encode");
   printf("\n d) Decode");
   printf("\n");
   do {
      printf(" >> ");
      scanf("%1s", action);
      action[0] = tolower(action[0]);
   } while (action[0] != 'e' && action[0] != 'd');

   return action[0];
}

/* Creates a string                                                                    */
LETTER *create_string() {
   LETTER *new_string; /* Points to the new string                                     */
   
   /* Create the header of the string                                                  */
   if ((new_string = (LETTER *) malloc(sizeof(LETTER))) == NULL) {
      printf("\nFailed to allocate the header of the string.");
      printf("\nExiting the program.");
      exit(0);
   }
   new_string->character = '~';

   /* Create the trailer of the string                                                 */
   if ((new_string->next_character = (LETTER *) malloc(sizeof(LETTER))) == NULL) {
      printf("\nFailed to allocate the trailer of the string.");
      printf("\nExiting the program.");
      exit(0);
   }
   new_string->next_character->character      = '\0';
   new_string->next_character->next_character = NULL;

   return new_string;
}

/* Get the rotation for the caesar cipher                                              */
void   get_rotation(int *rotation) {
   printf("\nEnter a rotation: ");
   printf("\n");
   scanf("%d", rotation);

   return;
}

/* Get the key for the vigenere cipher                                                 */
void   get_key(LETTER *key) {
   LETTER *held_letter,          /* Points to the letter following the new letter      */
          *current_letter = key; /* Points to the letter receiving the new letter      */
   char   new_character;         /* Holds the character to be added to the string      */

   printf("\nEnter the key: ");
   printf("\n");
   printf(" >> ");
   scanf(" ");
   while (fgets(&new_character, 2, stdin), new_character != '\n') {
      held_letter = current_letter->next_character;
      if ((current_letter->next_character = (LETTER *) malloc(sizeof(LETTER))) == NULL) {
         printf("\nFailed to allocate a letter of the key.");
         printf("\nExiting the program.");
         exit(0);
      }
      current_letter                 = current_letter->next_character;
      current_letter->next_character = held_letter;
      current_letter->character      = new_character;
   }

   return;
}

/* Gets a string from the user                                                         */
void   get_string(LETTER *text) {
   LETTER *held_letter,           /* Points to the letter following the new letter     */
          *current_letter = text; /* Points to the letter receiving the new letter     */
   char   new_character;          /* Holds the character to be added to the string     */

   printf("\nEnter the message: ");
   printf("\n >> ");
   scanf(" ");
   while (fgets(&new_character, 2, stdin), new_character != '\n') {
      held_letter = current_letter->next_character;
      if ((current_letter->next_character = (LETTER *) malloc(sizeof(LETTER))) == NULL) {
         printf("\nFailed to allocate a letter of the string.");
         printf("\nExiting the program.");
         exit(0);
      }
      current_letter                 = current_letter->next_character;
      current_letter->next_character = held_letter;
      current_letter->character      = new_character;
   }
   
   return;
}

/* Encodes a message using the caesar cipher                                           */
void   encode_caesar_cipher(LETTER *plaintext, int rotation) {
   LETTER *current_letter = plaintext->next_character;
                                                 /* Points to the letter being decoded */

   while (current_letter->character != '\0') {
      if (test_character(current_letter->character) == UPPER) {
         current_letter->character =
                     (current_letter->character - UPPER_INT + rotation) % 26 + UPPER_INT;
      } else if (test_character(current_letter->character) == LOWER) {
         current_letter->character =
                     (current_letter->character - LOWER_INT + rotation) % 26 + LOWER_INT;
      }
      current_letter = current_letter->next_character;
   }

   return;
}

/* Decodes a message using the caesar cipher                                           */
void   decode_caesar_cipher(LETTER *ciphertext, int rotation) {
   LETTER *current_letter = ciphertext->next_character;
                                                 /* Points to the letter being decoded */

   while (current_letter->character != '\0') {
      if (test_character(current_letter->character) == UPPER) {
         current_letter->character =
                (current_letter->character - UPPER_INT - rotation + 26) % 26 + UPPER_INT;
      } else if (test_character(current_letter->character) == LOWER) {
         current_letter->character =
                (current_letter->character - LOWER_INT - rotation + 26) % 26 + LOWER_INT;
      }
      current_letter = current_letter->next_character;
   }

   return;
}

/* Encodes a message using a vigenere cipher                                           */
void   encode_vigenere_cipher(LETTER *plaintext, LETTER *key) {
   LETTER *current_letter = plaintext->next_character,
                                                 /* Points to the letter being encoded */
          *key_cycler     = key->next_character; /* Points to the encoding letter      */

   /* Loop while there are non-encoded letters in the message                          */
   while (current_letter->character != '\0') {
      
      /* Cycle to a workable letters in the key                                        */
      while (!isalpha(key_cycler->character)) {
         if (key_cycler->character == '\0') {
            key_cycler = key->next_character;
         } else {
            key_cycler = key_cycler->next_character;
         }
      }

      /* Encode the letter                                                             */
      if (test_character(current_letter->character) == UPPER) {
         current_letter->character = (current_letter->character - UPPER_INT + (key_cycler->character - LOWER_INT) + 26) % 26 + UPPER_INT;
         key_cycler = key_cycler->next_character;
      } else if (test_character(current_letter->character) == LOWER) {
         current_letter->character =  (current_letter->character - LOWER_INT + (key_cycler->character - LOWER_INT) + 26) % 26 + LOWER_INT;
         key_cycler = key_cycler->next_character;
      }

      /* Cycle to a workable letter in the message                                     */
      current_letter = current_letter->next_character;
      while (!isalpha(current_letter->character) && current_letter->character != '\0') {
         current_letter = current_letter->next_character;
      }
   }

   return;
}

/* Decodes a message using a vigenere cipher                                           */
void   decode_vigenere_cipher(LETTER *ciphertext, LETTER *key) {
   LETTER *current_letter = ciphertext->next_character,
                                                 /* Points to the letter being decoded */
          *key_cycler     = key->next_character; /* Points to the decoding letter      */

   /* Loop while there are non-decoded letters in the message                          */
   while (current_letter->character != '\0') {

      /* Cycle to a workable letter in the key                                         */
      while (!isalpha(key_cycler->character)) {
         if (key_cycler->character == '\0') {
            key_cycler = key->next_character;
         } else {
            key_cycler = key_cycler->next_character;
         }
      }

      /* Decode the letter                                                             */
      if (test_character(current_letter->character) == UPPER) {
         current_letter->character = (current_letter->character - UPPER_INT -
                              (key_cycler->character - LOWER_INT) + 26) % 26 + UPPER_INT;
         key_cycler = key_cycler->next_character;
      } else if (test_character(current_letter->character) == LOWER) {
         current_letter->character = (current_letter->character - LOWER_INT -
                              (key_cycler->character - LOWER_INT) + 26) % 26 + LOWER_INT;
         key_cycler = key_cycler->next_character;
      }

      /* Cycle to a workable letter in the message                                     */
      current_letter = current_letter->next_character;
      while (!isalpha(current_letter->character) && current_letter->character != '\0') {
         current_letter = current_letter->next_character;
      }
   }

   return;
}

/* Prints a string                                                                     */
void   print_string(LETTER *text) {
   LETTER *current_letter = text->next_character;
                                                 /* Points to the letter being printed */

   while (current_letter->character != '\0') {
      printf("%c", current_letter->character);
      current_letter = current_letter->next_character;
   }

   return;
}

/* Deletes a string                                                                    */
void   clear_string(LETTER *text) {
   LETTER *current_letter = text, /* Points to the letter being cleared                */
          *next_letter    = current_letter->next_character;
                                   /* Points to the next letter to be cleared          */

   while (current_letter != NULL) {
      next_letter  = next_letter->next_character;
      free(current_letter);
      current_letter = next_letter;
   }

   return;
}

/* Tells the user goodbye                                                              */
void   farewell() {
   printf("\nThank you for using the program!");

   return;
}

/* Tests if a character is uppercase or lowercase                                      */
char   test_character(char check) {
   char test; /* Holds the test's response                                             */

   if (check >= 'a' && check <= 'z') {
      test = LOWER;
   } else if (check >= 'A' && check <= 'Z') {
      test = UPPER;
   } else {
      test = NEITHER;
   }

   return test;
}

/* Sets a string to lowercase                                                          */
void   to_lower(LETTER *string) {
   LETTER *current_letter = string->next_character;
                                  /* Points to the letter being converted to lowercase */

   while (current_letter->character != '\0') {
      current_letter->character = tolower(current_letter->character);
      current_letter            = current_letter->next_character;
   }

   return;
}