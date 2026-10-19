#include <stdio.h>
#include <stdlib.h>
#include "include/tree.h"
#include <sys/time.h>
#include <time.h>
#include <string.h>

#define N 5

char RandomLetter() {
    return 'A' + rand() % 26;
}

char* generate_key() {
    int len = rand() % 5 + 1;
    char* res = malloc(sizeof(char) * len + 1);
    for (int i = 0; i < len; i++) {
        res[i] = RandomLetter();
    }
    res[len] = '\0';
    return res;
}

int main() {
    srand(time(NULL));
    FILE* file = fopen("out.csv", "w");
    FILE* input = fopen("random_strings.txt", "r");
    if (!file) {
        return ERR_FILE;
    }
    Node* root = NULL;
    Node** search = NULL;
    int s = 0;
    ErrorCode err;
    struct timeval start, end;
    double insert_t = 0, find_t = 0, spec_t = 0, bypass_t = 0, del_t = 0;
    char* key;
    char buffer[256];
    fprintf(file, "N,Insert,Search,SpecialSearch,Bypass,Delete\n");
    for (int i = 0; i <= 1000000; i++) {
        fgets(buffer, sizeof(buffer), input);
        key = strdup(buffer);
        int info = i;
        if (i % 10000 == 0) {
            gettimeofday(&start, NULL);
            for (int i = 0; i < N; i++) {
                err = insert(&root, key, info);
            }
            gettimeofday(&end, NULL);
            insert_t = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

            char* s_key = generate_key();
            gettimeofday(&start, NULL);

            for (int i = 0; i < N; i++) {
                err = find_key(&search, &s, root, s_key);
            }
            gettimeofday(&end, NULL);
            find_t = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000.0;
            free(search);
            gettimeofday(&start, NULL);
            for (int i = 0; i < N; i++) {
                err = find_closest_key(&search, &s, root, s_key);
            }
            gettimeofday(&end, NULL);
            spec_t = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000.0;
            free(search);

            gettimeofday(&start, NULL);
            for (int i = 0; i < N; i++) {
                bypass_in_order(root, "", "ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ");
            }
            gettimeofday(&end, NULL);
            bypass_t = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000.0;

            gettimeofday(&start, NULL);
            for (int i = 0; i < N; i++) {
                err = delete(&root, key);
            }
            gettimeofday(&end, NULL);
            del_t = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000.0;

            fprintf(file, "%d,%.1f,%.1f,%.1f,%.1f,%.1f\n", i, insert_t, find_t, spec_t, bypass_t, del_t);
            free(s_key);
        } else {
            err = insert(&root, key, info);
        }
        free(key);
    }
    free_tree(&root);
    fclose(file);
}
