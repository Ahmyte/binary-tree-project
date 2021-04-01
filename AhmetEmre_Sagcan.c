/*Ahmet Emre Sağcan 150119042
  This code takes an input from file and builds two type of binary trees.*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct Textual
{
    char word[20];
    int id;
    int frequency;
    struct Textual* left;
    struct Textual* right;
}Textual;

typedef Textual* TextualPtr;

//Function declarations
TextualPtr NewNode(char* word, int freq, int id);
TextualPtr InsertAlphabetic(TextualPtr root, char key[20], int freq, int id, int *depth);
TextualPtr InsertLevelOrder(TextualPtr *textuals, TextualPtr root, int i, int line_count);
int FindDepth(TextualPtr root, int id, int level);
void InOrderLNR (TextualPtr current_textual);
void PostOrderLRN(TextualPtr current_textual);
void PreOrderNLR(TextualPtr current_textual);
void SortByFrequency(int line_count, TextualPtr *textuals);
void GetNode(FILE *fptr, char line[], char word[], char tempFreq[], char temp_id[], int *frequency, int *id);
void AskPrintOrder(TextualPtr root);

int main()
{
    //Variable declarations
    FILE* fptr;
    int i,j, frequency, id, depth_sum = 0, line_count = 0;
    char check, line[50],word[50], temp_freq[5], temp_id[4];

    //Counts the number of lines in the input file.
    if(fptr = fopen("input.txt", "r"))
    {
        for (check = getc(fptr); check != EOF; check = getc(fptr))
            if (check == '\n')  line_count++;
    }
    else
    {
        printf("Couldn't open the input file.");
        return -1;
    }
    TextualPtr textuals[line_count]; //Holds the nodes to build a binary tree in future.

    fseek(fptr, 0, SEEK_SET); //Goes back to the start of the file.

    //Taking the needed fields to build root.
    GetNode(fptr, line, word, temp_freq, temp_id, &frequency, &id);
    TextualPtr root = NewNode(word, frequency, id); //Root of the 1st tree.
    textuals[0] = NewNode(word, frequency, id); //Stores this node but this will be used in BT.
    depth_sum = (1 * frequency);

    //Taking the needed fields to build other nodes.
    for(i = 1; i < line_count && !feof(fptr); i++)
    {
        int depth = 0; //This is a temporary variable to hold the depth of the current node.
        GetNode(fptr, line, word, temp_freq, temp_id, &frequency, &id); //Gets the node from input file.
        InsertAlphabetic(root, word, frequency,id, &depth); //Inserts the node.
        textuals[i] = NewNode(word, frequency, id); //Stores the nodes that will be used in BT.
        depth_sum += (depth * frequency); //Sums up each node's access time.
    }
    puts("LNR:");
    InOrderLNR(root);//Prints the tree with in-order traversal.
    puts("END\n");
    printf("The total access time of the binary search tree is: %d\n\n", depth_sum);

    fseek(fptr, 0, SEEK_SET); //Goes back to the start of the file.

    SortByFrequency(line_count, textuals); //Sorts the array by frequency
    TextualPtr root_freq = textuals[0];
    InsertLevelOrder(textuals,root_freq,0,line_count); //Inserts the sorted array to build a binary tree.
    depth_sum = 0;
    //Finds the depth of each node then sums up their access time.
    for(i = 0; i < line_count; i++)
    {
        int level = 1;
        depth_sum += FindDepth(root_freq,textuals[i]->id,level) * textuals[i]->frequency;
    }
    AskPrintOrder(root_freq);
    printf("\nEND\nThe total access time of the binary tree is: %d\n", depth_sum);
    fclose(fptr);
    return 0;
}

void GetNode(FILE *fptr, char line[], char word[], char tempFreq[], char temp_id[], int *frequency, int *id)
{
    //Parses the line with delimiter ',' and assign them to the appropriate variables.
    fgets(line, 50, fptr);
    strcpy(temp_id, strtok(line, ","));
    strcpy(word, strtok(NULL, ","));
    strcpy(tempFreq, strtok(NULL, ","));

    //Converts strings to needed integers.
    *frequency = atoi(tempFreq);
    *id = atoi(temp_id);
}

void SortByFrequency(int line_count, TextualPtr *textuals)
{
    //Compares the frequency of current and the next node in array.
    int i, j;
    TextualPtr tempTextual = NULL;
    for (i = 0; i < line_count; ++i)
    {
        for (j = i + 1; j < line_count; ++j)
        {
            if (textuals[i]->frequency < textuals[j]->frequency)
            {
                tempTextual =  textuals[i];
                textuals[i] = textuals[j];
                textuals[j] = tempTextual;
            }
        }
    }
}

TextualPtr NewNode(char* word, int freq, int id)
{
    //Uses the taken input to build a new node.
    TextualPtr p = malloc(sizeof(Textual));
    strcpy(p->word, word);
    p->id = id;
    p->frequency = freq;
    p->left = NULL;
    p->right = NULL;
    return p;
}

TextualPtr InsertAlphabetic(TextualPtr root, char key[20], int freq, int id, int *depth)
{
    (*depth)++; //Increments in every recursion to count every depth level
    char tempKey[20];
    strcpy(tempKey, key);

    //Makes every character lower case because we need to make string comparisons logically correct.
    int i;
    for(i = 0; i < 20 ; i++)
        tempKey[i] = tolower(tempKey[i]);
    if(root==NULL)
        return NewNode(key, freq, id);
    //If it is equal or bigger, directs to right side. If not directs to left side.
    else if(strcmp(tempKey,root->word) >= 0)
        root->right = InsertAlphabetic(root->right, key,freq,id,depth);
    else
        root->left = InsertAlphabetic(root->left,key,freq,id,depth);
    return root;
}

TextualPtr InsertLevelOrder(TextualPtr *textuals, TextualPtr root, int i, int line_count)
{
    //Inserts each node in the array in level order.
    if (i < line_count)
    {
        TextualPtr temp = textuals[i]; //Holds the current node in a temporary node.
        root = temp;
        //Always inserts the (2i + 1) indexed node in the array for the left child, i here is the index of current node.
        root->left = InsertLevelOrder(textuals, root->left, 2 * i + 1, line_count);
        //Same happens for the right child but this time (2i + 2) indexed node of the array is used.
        root->right = InsertLevelOrder(textuals, root->right, 2 * i + 2, line_count);
    }
    return root;
}

void InOrderLNR(TextualPtr current_textual) //Printing by in-order traversal.
{
    if(current_textual != NULL)
    {
        InOrderLNR(current_textual->left);
        printf("ID:%d\t%s,%d\n",current_textual->id,current_textual->word,current_textual->frequency);
        InOrderLNR(current_textual->right);
    }
}

void PostOrderLRN(TextualPtr current_textual) //Printing by post-order traversal.
{
    if(current_textual != NULL)
    {
        PostOrderLRN(current_textual->left);
        PostOrderLRN(current_textual->right);
        printf("ID:%d\t%s,%d\n",current_textual->id,current_textual->word,current_textual->frequency);
    }
}

void PreOrderNLR(TextualPtr current_textual) //Printing by pre-order traversal.
{
    if(current_textual != NULL)
    {
        printf("ID:%d\t%s,%d\n",current_textual->id,current_textual->word,current_textual->frequency);
        PreOrderNLR(current_textual->left);
        PreOrderNLR(current_textual->right);
    }
}

void AskPrintOrder(TextualPtr root) //Asks the users choice of printing
{
    int choice = 0;
    puts("1.LRN\t2.LNR\t3.NLR\nEnter which type of order you want to print: ");
    scanf("%d", &choice);
    if(choice == 1)         PostOrderLRN(root);
    else if(choice == 2)    InOrderLNR(root);
    else if(choice == 3)    PreOrderNLR(root);
}

int FindDepth(TextualPtr root, int id, int level) //Finds the depth of the given node.
{
    if (root == NULL)
        return 0;
    //First of all, tries to find the node in the tree.
    if (root->id == id)
        return level;
    //When traversing the tree, function also increments level variable with each depth level.
    int down_level = FindDepth(root->left, id, level+1);

    if (down_level != 0)
        return down_level;

    down_level = FindDepth(root->right, id, level+1);
    return down_level;
}
