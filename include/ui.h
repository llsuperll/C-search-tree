#ifndef HANDLERS_H
#define HANDLERS_H

#include "tree.h"

void display_menu();
int menu_choice(int, Node**);
const char* error_message(ErrorCode);
void case_insert(Node**);
void case_delete(Node**);
void case_print_tree(Node**);
void case_bypass(Node**);
void case_find_by_key(Node**);
void case_find_closest_key(Node**);
void case_import(Node**);
void case_graph(Node**);
#endif
