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

#define MAX_WORD_LENGTH 50

typedef struct node
{
    char letter;
    int is_word;
    struct node *next;
    struct node *children;
} node;

node *create_node(char letter)
{
    node *new_node = malloc(sizeof(node));
    new_node->letter = letter;
    new_node->is_word = 0;
    new_node->next = NULL;
    new_node->children = NULL;
    return new_node;
}

void add_word(node *root, char *word)
{
    node *current_node = root;
    int i = 0;
    while (word[i] != '\0')
    {
        if (current_node->children == NULL)
        {
            current_node->children = create_node(word[i]);
            current_node = current_node->children;
        }
        else
        {
            node *tmp = current_node->children;
            while (tmp->next != NULL && tmp->letter != word[i])
                tmp = tmp->next;
            if (tmp->letter == word[i])
                current_node = tmp;
            else
            {
                tmp->next = create_node(word[i]);
                current_node = tmp->next;
            }
        }
        i++;
    }
    current_node->is_word = 1;
}

void print_words(node *root, char *word, int i)
{
    if (root == NULL)
        return;
    if (root->is_word)
    {
        word[i] = '\0';
        printf("%s\n", word);
    }
    node *tmp = root->children;
    while (tmp != NULL)
    {
        word[i] = tmp->letter;
        print_words(tmp, word, i + 1);
        tmp = tmp->next;
    }
}

void autocomplete(node *root, char *word)
{
    node *current_node = root;
    int i = 0;
    while (word[i] != '\0')
    {
        if (current_node->children == NULL)
        {
            printf("No word found for %s ", word);
            return;
        }
        else
        {
            node *tmp = current_node->children;
            while (tmp->next != NULL && tmp->letter != word[i])
                tmp = tmp->next;
            if (tmp->letter == word[i])
                current_node = tmp;
            else
            {
                printf("No word found for %s ", word);
                return;
            }
        }
        i++;
    }
    print_words(current_node, word, i);
}

int main(void)
{
    FILE *file = fopen("dico.txt", "r");
    if (file == NULL)
    {
        printf("Error opening file");
        return 1;
    }
    char word[MAX_WORD_LENGTH];
    node *root = create_node('\0');
    while (fscanf(file, "%s", word) != EOF)
        add_word(root, word);
    fclose(file);
    char input[MAX_WORD_LENGTH];
    while (1)
    {
        printf("Enter one or more letter(s): ");
        scanf("%s", input);
        for (int i = 0; input[i] != '\0'; i++)
            input[i] = tolower(input[i]);
        autocomplete(root, input);
        printf(" (enter 'q' to quit) ");
        scanf("%s", input);
        if (input[0] == 'q')
            break;
    }
    return 0;
}