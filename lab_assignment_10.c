#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

// Trie structure
struct Trie
{
    struct Trie *children[ALPHABET_SIZE];
    int isEndOfWord;
    int count;
};

// Initializes a trie structure
struct Trie *createTrie()
{
    struct Trie *pTrie = (struct Trie *)malloc(sizeof(struct Trie));
    pTrie->isEndOfWord = 0;
    pTrie->count = 0;

    for (int i = 0; i < ALPHABET_SIZE; i++)
        pTrie->children[i] = NULL;

    return pTrie;
}

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word)
{
    struct Trie *pCrawl = pTrie;

    for (int level = 0; level < strlen(word); level++)
    {
        int index = word[level] - 'a';
        if (!pCrawl->children[index])
        {

            pCrawl->children[index] = createTrie();
        }
        pCrawl = pCrawl->children[index];
    }

    pCrawl->isEndOfWord = 1;
    pCrawl->count++;
}

// Computes the number of occurrences of the word
int numberOfOccurances(struct Trie *pTrie, char *word)
{
    struct Trie *pCrawl = pTrie;

    for (int level = 0; level < strlen(word); level++)
    {
        int index = word[level] - 'a';
        if (!pCrawl->children[index])
            return 0;

        pCrawl = pCrawl->children[index];
    }

    if (pCrawl != NULL && pCrawl->isEndOfWord)
        return pCrawl->count;

    return 0;
}

// Deallocate the trie structure
void deallocateTrie(struct Trie *pTrie)
{
    if (pTrie == NULL)
        return;

    for (int i = 0; i < ALPHABET_SIZE; i++)
        deallocateTrie(pTrie->children[i]);

    free(pTrie);
}

// Reads words from a dictionary file into an array of strings
int readDictionary(char *filename, char **pInWords)
{
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return 0;
    }

    char word[100]; // Assumes a maximum word length of 99 characters
    int count = 0;
    while (fgets(word, sizeof(word), file)) {
        word[strcspn(word, "\n")] = '\0';  // Remove newline character
        pInWords[count] = (char *)malloc(strlen(word) + 1);
        strcpy(pInWords[count], word);
        count++;
    }

    fclose(file);
    return count;
}

int main(void)
{
    char *inWords[256];

    // Read the number of the words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i)
    {
        printf("%s\n", inWords[i]);
    }

    struct Trie *pTrie = createTrie();
    for (int i = 0; i < numWords; i++)
    {
        insert(pTrie, inWords[i]);
    }
    // Parse line by line, and insert each word to the trie data structure
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
    }
    deallocateTrie(pTrie);
    if (pTrie != NULL)
        printf("There is an error in this program\n");
    return 0;
}
