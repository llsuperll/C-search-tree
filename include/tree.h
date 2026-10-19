#ifndef TREE_H
#define TREE_H

#include <stdio.h> 

typedef struct Node {
    char* key;
    unsigned int info;
    struct Node* left;
    struct Node* right;
    struct Node* next;
} Node;

typedef enum {
    OK = 0,
    ERR_MEMORY = -1,
    ERR_FILE = -2,
    ERR_INFO = -3,
    ERR_FILE_READ = -4,
    ERR_NULL_TREE = -5,
    ERR_ELEMENT_NOT_FOUND = -6,
} ErrorCode;

ErrorCode insert(Node**, const char*, unsigned int);
ErrorCode delete(Node**, const char*);
ErrorCode find_key(Node***, int*, Node*, const char*);
ErrorCode find_closest_key(Node***, int*, Node*, const char*);
ErrorCode import_from_file(Node**, const char*);
ErrorCode graphviz_tree(Node*, const char*);
void fill_dotfile(FILE*, Node*);
void print_tree(Node*, const char*, int);
void free_tree(Node**);
void bypass_in_order(Node*, const char*, const char*);
int compare_strings(const char*, const char*);
#endif 
