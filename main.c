// Create a word dictionary in C
// How to proceed:
// We retrieve each word from the file "dico.txt"
// We want to make an "autocomplete" function
// This means => we ask the user to input one or more letters
// The program will then display all the words that start with the letters entered by the user
// Example: 'a' => apple, apricot, ... 'ap' => apple, apricot, ... 'app' => apple, ... 'appl' => apple, ...
// If the user enters a letter that is not in the dictionary, the program will display "No word found"
// If the user enters a letter in UPPERCASE, we put it in lowercase
// Use trie data structure
// Don't forget to free all the memory you allocate
// Use line breaks!
// USER CAN ONLY ENTER LETTERS!!!!!

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 50                  // Maximum length of a word

// Structure of a node
typedef struct node
{
    char letter;                                // Letter of the node
    int is_word;                                // 1 if the node is the last letter of a word, 0 otherwise
    struct node *next;                          // Pointer to the next node
    struct node *children;                      // Pointer to the first child
} node;                                        // Name of the structure

// Function to create a node
node *create_node(char letter)
{
    node *new_node = malloc(sizeof(node));      // Allocate memory for the new node
    new_node->letter = letter;                  // Assign the letter to the node
    new_node->is_word = 0;                      // The node is not the last letter of a word
    new_node->next = NULL;                      // The next node is NULL
    new_node->children = NULL;                  // The first child is NULL
    return new_node;                            // Return the new node
}

// Function to add a word to the dictionary
void add_word(node *root, char *word)
{
    node *current_node = root;                              // Start at the root
    int i = 0;                                              // Index of the letter in the word                    
    while (word[i] != '\0')                                 // While we haven't reached the end of the word
    {
        if (current_node->children == NULL)                 // If the current node has no children
        {
            current_node->children = create_node(word[i]);  // Create a new node with the letter
            current_node = current_node->children;          // Go to the new node
        }
        else                                                
        {
            node *tmp = current_node->children;                     // Else create a temporary node
            while (tmp->next != NULL && tmp->letter != word[i])     // While the next node is not NULL and the letter is not the one we want
                tmp = tmp->next;                                    // Go to the next node
            if (tmp->letter == word[i])                             // If the letter is the one we want  
                current_node = tmp;                                 // Go to the node            
            else
            {
                tmp->next = create_node(word[i]);                   // Else create a new node with the letter
                current_node = tmp->next;                           // Go to the new node
            }
        }
        i++;                                                        // Go to the next letter                
    }
    current_node->is_word = 1;                                      // The last node is the last letter of a word     
}

// Function to print the words
void print_words(node *root, char *word, int i)
{
    if (root == NULL)                   // If the root is NULL
        return;                         // Return   
    if (root->is_word)                  // If the node is the last letter of a word
    {
        word[i] = '\0';                 // Add a null character at the end of the word
        printf("%s\n", word);           // Print the word
    }
    node *tmp = root->children;         // Create a temporary node
    while (tmp != NULL)                 // While the temporary node is not NULL
    {
        word[i] = tmp->letter;                  // Add the letter to the word
        print_words(tmp, word, i + 1);          // Print the words
        tmp = tmp->next;                        // Go to the next node
    }
}

// Function to print the words that start with the letters entered by the user
void autocomplete(node *root, char *word)
{
    node *current_node = root;                            // Start at the root          
    int i = 0;                                          // Index of the letter in the word      
    while (word[i] != '\0')                             // While we are not at the end of a word
    {
        if (current_node->children == NULL)             // If the current node has no children
        {
            printf("No word found for %s ", word);      // Print "No word found"
            return;
        }
        else
        {
            node *tmp = current_node->children;                     // Else create a temporary node
            while (tmp->next != NULL && tmp->letter != word[i])     // While the next node is not NULL and the letter is not the one we want
                tmp = tmp->next;                                    // Go to the next node
            if (tmp->letter == word[i])                             // If the letter is the one we want
                current_node = tmp;                                 // Go to the node
            else
            {
                printf("No word found for %s ", word);              // Else print "No word found"
                return;
            }
        }
        i++;                                                        // Go to the next letter                
    }
    print_words(current_node, word, i);                             // Print the words
}

// Main function
int main(void)
{
    FILE *file = fopen("dico.txt", "r");    // Open the file "dico.txt" in read mode
    if (file == NULL)
    {
        printf("Error opening file");       // If the file is NULL, print "Error opening file"
        return 1;
    }
    char word[MAX_WORD_LENGTH];                 // Create a string to store the words
    node *root = create_node('\0');             // Create the root
    while (fscanf(file, "%s", word) != EOF)     // While we are not at the end of the file
        add_word(root, word);                   // Add the word to the dictionary
    fclose(file);                               // Close the file
    char input[MAX_WORD_LENGTH];                // Create a string to store the letters entered by the user
    while (1)                                   // While the user doesn't enter "quit"
    {
        printf("Enter one or more letter(s): ");    // Ask the user to enter one or more letters
        scanf("%s", input);                         // Store the letters in the string "input"
        for (int i = 0; input[i] != '\0'; i++)      // For each letter in the string "input"
            input[i] = tolower(input[i]);           // Convert the letter to lowercase
        autocomplete(root, input);                  // Print the words that start with the letters entered by the user
        printf(" (enter 'q' to quit or 'c' to continue) ");            // Ask the user if he wants to quit
        scanf("%s", input);                         // Store the answer in the string "input"
        if (input[0] == 'q')                        // If the user enters "q"
            break;                                  // Break the loop
    }
    return 0;
}