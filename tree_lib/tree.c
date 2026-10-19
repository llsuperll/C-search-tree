#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/tree.h"
#include "../include/input.h"

Node* find_min_key(Node* root) {
    if (!root) {
        return NULL;
    }
    Node* cur = root;
    while (cur -> left) {
        cur = cur -> left;
    }
    return cur;
}

ErrorCode insert(Node** root, const char* key, unsigned int info) {
    Node* new = malloc(sizeof(Node));
    if (!new) {
        return ERR_MEMORY;
    }
    new -> key = strdup(key);
    new -> info = info;
    new -> left = NULL;
    new -> right = NULL;
    new -> next = NULL;
    Node* cur = *root;
    Node* parent = NULL;
    Node* old_min = find_min_key(*root);
    while (cur) {
        parent = cur;
        if (strcmp(key, cur -> key) >= 0) {
            cur = cur -> right;
        } else {
            cur = cur -> left;
        }
    }
    if (!parent) {
        *root = new;
        return OK;
    } else if (strcmp(key, parent -> key) >= 0) {
        parent -> right = new;
    } else {
        parent -> left = new;
    }

    Node* min_node = find_min_key(*root);
    if (new == min_node) {
        new -> next = old_min;
        if (old_min) {
            Node* prev = old_min;
            while (prev -> next != old_min && prev -> next != NULL) {
                prev = prev -> next;
            }
            if (prev -> next == old_min) {
                prev -> next = new;
            }
        }
        return OK;
    }
    cur = min_node;
    Node* prev = NULL;
    while (cur && strcmp(key, cur -> key) >= 0) {
        prev = cur;
        cur = cur -> next;
    }
    new -> next = cur;
    if (prev) {
        prev -> next = new;
    }
    return OK;
}

ErrorCode delete(Node** root, const char* key) {
    if (!key) {
        return ERR_INFO;
    }
    Node* cur = *root;
    Node* parent = NULL;
    while (cur) {
        int cmp = strcmp(cur -> key, key);
        if (cmp == 0) {
            break;
        } else if (cmp < 0) {
            parent = cur;
            cur = cur -> right;
        } else {
            parent = cur;
            cur = cur -> left;
        }
    }
    if (!cur) {
        return ERR_ELEMENT_NOT_FOUND;
    }

    Node* thread = find_min_key(*root);
    Node* prev = NULL;
    while (thread && thread != cur) {
        prev = thread;
        thread = thread -> next;
    }
    if (prev && !(cur -> right && cur -> left)) {
        prev -> next = cur -> next; 
    }
    if (cur -> left && cur -> right) {
        Node* min_node = find_min_key(cur -> right);
        Node* min_next = min_node -> next;
        free(cur -> key);
        cur -> key = NULL;
        cur -> key = strdup(min_node -> key);
        cur -> info = min_node -> info;
        ErrorCode err = delete(&(cur -> right), min_node -> key);
        cur -> next = min_next;
        return err; 
    } else {
        Node* child = NULL;
        if (cur -> right) {
            child = cur -> right;
        } else if (cur -> left) {
            child = cur -> left;
        }
        if (parent == NULL) {
            *root = child;
        } else if (parent -> right == cur) {
            parent -> right = child;
        } else {
            parent -> left = child;
        }
        free(cur -> key);
        free(cur);
    }
    return OK;
}

void bypass_in_order(Node* root, const char* start, const char* end) {
    if (!root) {
        return;
    }
    Node* cur = find_min_key(root);
    while (cur && compare_strings(cur -> key, start) >= 0 && compare_strings(cur -> key, end) <= 0) {
        printf("%s [%u]\n", cur -> key, cur -> info);
        cur = cur -> next;
    }
}

void free_tree(Node** root) {
    Node* cur = find_min_key(*root);
    Node* tmp = NULL;
    while (cur) {
        tmp = cur;
        cur = cur -> next;
        free(tmp -> key);
        tmp -> key = NULL;
        free(tmp);
        tmp = NULL;
    }
}

void print_tree(Node* node, const char* prefix, int isLeft) {
    if (!node) {
        return;
    }
    printf("%s", prefix);
    if (isLeft) {
        printf("├── ");
    } else {
        printf("└── ");
    }
    printf("%s [%u]\n", node -> key, node -> info);
    char new_pref[256];
    strcpy(new_pref, prefix);
    if (isLeft) {
        strcat(new_pref, "│   ");
    } else {
        strcat(new_pref, "    ");
    }
    print_tree(node -> left, new_pref, 1);
    print_tree(node -> right, new_pref, 0);
}

ErrorCode find_key(Node*** arr, int* arr_size, Node* root, const char* key) {
    if (!key) {
        return ERR_INFO;
    }
    *arr = NULL;
    *arr_size = 0;
    Node* cur = root;
    Node** res = NULL;
    int cap = 0;
    while (cur) {
        int key_cmp = strcmp(cur -> key, key);
        if (key_cmp > 0) {
            cur = cur -> left;
        } else if (key_cmp < 0) {
            cur = cur -> right;
        } else {
            cap += 1;
            res = realloc(res, sizeof(Node*) * cap);
            if (!res) {
                return ERR_MEMORY;
            }
            res[cap - 1] = cur;
            cur = cur -> right;
        }
    }
    *arr = res;
    *arr_size = cap;
    return OK;
}

int compare_strings(const char* str1, const char* str2) {
    int sum1 = 0, sum2 = 0;
    for (int i = 0; str1[i] != '\0'; i++) {
        sum1 += (int)str1[i];
    }
    for (int i = 0; str2[i] != '\0'; i++) {
        sum2 += (int)str2[i];
    }
    return sum1 - sum2;
}

ErrorCode find_closest_key(Node*** arr, int* arr_size, Node* root, const char* key) {
    if (!key) {
        return ERR_INFO;
    }
    *arr = NULL;
    *arr_size = 0;
    Node* cur = find_min_key(root);
    Node** res = NULL;
    int cap = 0, min_diff = 100000;
    while (cur) {
        int cmp = abs(compare_strings(cur -> key, key));
        if (cmp == 0) {
            cur = cur -> next;
            continue;
        } else if (cmp < min_diff) {
            if (res) {
                free(res);
            }
            min_diff = cmp;
            cap = 0;
            res = malloc(sizeof(Node*));
            if (!res) {
                return ERR_MEMORY;
            }
            res[cap] = cur;
            cap += 1;
        } else if (cmp == min_diff) {
            cap += 1;
            res = realloc(res, cap * sizeof(Node*));
            if (!res) {
                return ERR_MEMORY;
            }
            res[cap - 1] = cur;
        }
        cur = cur -> next;
    }
    *arr = res;
    *arr_size = cap;
    return OK;
}

ErrorCode import_from_file(Node** root, const char* filename) {
    if (!filename) {
        return ERR_FILE;
    }
    FILE* file = fopen(filename, "r");
    if (!file) {
        return ERR_FILE;
    }
    free_tree(root);
    Node* new_root = NULL;
    char* line = NULL;
    char* end = NULL;
    char* key = NULL;
    int c = 0, info = 0;
    while ((line = my_getline("", file)) != NULL) {
        if (c % 2 == 0) {
            key = strdup(line);
        } else {
            info = strtol(line, &end, 10);
            if (*end != '\0' || info <= 0) {
                free(line);
                free(key);
                return ERR_FILE_READ;
            }
            if (insert(&new_root, key, info) != OK) {
                free(line);
                free(key);
                return ERR_MEMORY;
            }
            if (key) {
                free(key);
            }
        }
        c += 1;
        free(line);
    }
    if (c % 2 != 0) {
        return ERR_FILE_READ;
    }
    *root = new_root;
    fclose(file);
    return OK;
}

void fill_dotfile(FILE* file, Node* node) {
    if (!node) {
        return;
    }
    fprintf(file, "    \"%p\" [label=\"%s\\nvalue: %u\"];\n", (void*)node, node -> key, node -> info);
    if (node -> left) {
        fprintf(file, "    \"%p\" -> \"%p\";\n", (void*)node, (void*)node -> left);
        fill_dotfile(file, node -> left);
    }
    if (node -> right) {
        fprintf(file, "    \"%p\" -> \"%p\";\n", (void*)node, (void*)node -> right);
        fill_dotfile(file, node -> right);
    }
}

ErrorCode graphviz_tree(Node* root, const char* filename) {
    if (!root) {
        return ERR_NULL_TREE;
    }
    if (!filename) {
        return ERR_FILE;
    }
    FILE* file = fopen(filename, "w");
    if (!file) {
        return ERR_FILE;
    }
    fprintf(file, "digraph BST {\n");
    fprintf(file, "    node [shape=circle, style=filled, fillcolor=\"#22e55d\"];\n");
    fill_dotfile(file, root);
    fprintf(file, "}\n");
    fclose(file);

    char* cmd = calloc(2 * strlen(filename) + 27, sizeof(char));
    if (!cmd) {
        return ERR_MEMORY;
    }
    sprintf(cmd, "dot -Tpng %s -o %s.png", filename, filename);
    pclose(popen(cmd, "r"));
    free(cmd);
    return OK;
}
