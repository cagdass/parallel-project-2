#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "utils.h"

// function to free the list whose root is given
void freelist(struct node* root){
    while(root->next != NULL){
        struct node * tmp = root;
        root = root->next;
        free(tmp);
    }

    free(root);
}

// TAKEN FROM http://stackoverflow.com/questions/9210528/split-string-with-delimiters-in-c
// I know the lines from documents.txt will only be processed in the root processor
// but I was still scared to use the non thread-safe function strtok()
char** str_split( char* str, char delim, int* numSplits ) {
    char** ret;
    int retLen;
    char* c;

    if (( str == NULL ) || ( delim == '\0' )) {
        ret = NULL;
        retLen = -1;
    }
    else {
        retLen = 0;
        c = str;
        do {
            if ( *c == delim ) {
                retLen++;
            }

            ++c;
        } while ( *c != '\0' );

        ret = (char**)malloc( ( retLen + 1 ) * sizeof( *ret ) );
        ret[retLen] = NULL;

        c = str;
        retLen = 1;
        ret[0] = str;

        do {
            if ( *c == delim ) {
                ret[retLen++] = &c[1];
                *c = '\0';
            }

            c++;
        } while ( *c != '\0' );
    }

    if ( numSplits != NULL ) {
        *numSplits = retLen;
    }

    return ret;
}

// read the query file and return the vector of words as an integer array
int* readquery(char* queryfile, int size){
    FILE *fp = fopen(queryfile, "r");
    int* len = 0;
    char* line = NULL;
    while ((getline(&line, &len, fp)) != -1){
    }
    fclose(fp);
    int num = 0;
    char** tokens = str_split(line, ' ', &num);
    int* vals = (int *)malloc(size * sizeof(int));
    int i;
    for(i = 0; i < size; i++){
        vals[i] = atoi(tokens[i]);
    }
    free(tokens);

    return vals;
}

// insertion sorts two given arrays basing on the values of the first array
// so for arguments (vals, ids, length) it sorts the length portion of the given vals array
// and swaps the corresponding id's as well
void insertion_sort(int *arr, int* ids, int length) {
    int i, j ,tmp, tmp2;
    for (i = 1; i < length; i++) {
        j = i;
        while (j > 0 && arr[j - 1] > arr[j]) {
            tmp2 = ids[j];
            ids[j] = ids[j-1];
            ids[j-1] = tmp2;
            tmp = arr[j];
            arr[j] = arr[j - 1];
            arr[j - 1] = tmp;
            j--;
        }
    }
}

// find the similarity of a document with the queried document
int similarity(int* vals, int* query, int size){
    int sim = 0;
    int i;
    for(i = 0; i < size; i++){
        sim += pow(vals[i], query[i]);
    }
    return ((int)sim);
}

// read values of documents from a file, calculate similaties and return as a linked list
struct node* readfile(char* filename, int* query, int dictionary_size, int* list_size){
    FILE *fp = fopen(filename, "r");
    int* len = 0;
    char* line = NULL;
    // int count = 0;
    int num = 0;
    int i;
    (*list_size) = 0;
    // printf("Size comes as %d\n", (*list_size));
    int cursim;
    int *vals = (int*)malloc(dictionary_size * sizeof(int));
    struct node * root = (struct node *)malloc(sizeof(struct node*));
    struct node * ptr = root;
    while ((getline(&line, &len, fp)) != -1){
        // sample input "2: 1 6 4 67"
        char** tokens = str_split(line, ' ', &num);
        // for(i = 0; i < num; i++){
            // printf("Token %d is %s\n", i, tokens[i]);
        // }
        ptr->id = atoi(tokens[0]);
        for(i = 0; i < num - 1; i++){
            vals[i] = atoi(tokens[i+1]);
        }
        // calculate the similarity and put it in the node
        cursim = similarity(vals, query, dictionary_size);
        ptr->similarity = cursim;

        ptr->next = (struct node*)malloc(sizeof(struct node*));
        ptr = ptr->next;

        // increment list_size
        (*list_size) += 1;
        // count++;
        free(tokens);

    }
    fclose(fp);
    // printf("count is %d\n", count);
    // list_size = count;
    // printf("size in readfile %d\n", (*list_size));
    // size = count;

    return root;
}