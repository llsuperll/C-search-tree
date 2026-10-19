#include <stdio.h>
#include <stdlib.h>
#include "../include/tree.h"
#include "../include/ui.h"
#include "../include/input.h"

int menu_choice(int choice, Node** root) {
    switch(choice) {
        case 1:
            case_insert(root);
            break;
        case 2:
            case_delete(root);
            break;
        case 3:
            case_bypass(root);
            break;
        case 4:
            case_find_by_key(root);
            break;
        case 5:
            case_find_closest_key(root);
            break;
        case 6:
            case_print_tree(root);
            break;
        case 7:
            case_graph(root);
            break;
        case 8:
            case_import(root);
            break;
        default:
            return 0;
    }
    return 1;
}

void display_menu() {
    printf(
        "\n=== Меню работы с деревом ===\n"
        "(1) Добавить новый элемент\n"
        "(2) Удалить элемент\n"
        "(3) Обход в прямом порядке\n"
        "(4) Поиск элемента по ключу\n"
        "(5) Поиск наиболее близкого ключа\n"
        "(6) Вывод дерева\n"
        "(7) Графический вывод дерева\n"
        "(8) Импорт дерева из файла\n"
        "Выберите действие (1-8): "
    );
}

const char* error_message(ErrorCode code) {
    switch(code) {
        case OK: 
            return "Успешно";
        case ERR_MEMORY: 
            return "Ошибка выделения памяти";
        case ERR_FILE: 
            return "Ошибка работы с файлом";
        case ERR_FILE_READ:
            return "Отсутсвует часть данных в файле или они некорректны";
        case ERR_ELEMENT_NOT_FOUND:
            return "Элемент не найден!";
        default: 
            return "Неизвестная ошибка";
    }
}

void case_insert(Node** root) {
    unsigned int info = 0;
    scanf("%*c");
    char* key = my_getline("Введите ключ: ", stdin);
    if (input(&info, "Введите инфо: ") == -1) {
        free(key);
        return;
    }
    ErrorCode err = insert(root, key, info);
    if (err == OK) {
        printf("Элемент успешно добавлен\n");
    } else {
        printf("%s\n", error_message(err));
    }
    if (key) {
        free(key);
    }
}

void case_delete(Node** root) {
    scanf("%*c");
    char* key = my_getline("Введите ключ: ", stdin);
    ErrorCode err = delete(root, key);
    if (err == OK) {
        printf("Элемент успешно удален\n");
    } else {
        printf("%s\n", error_message(err));
    }
    if (key) {
        free(key);
    }
}

void case_print_tree(Node** root) {
    printf("=== Дерево: ===\n");
    print_tree(*root, "", 0);
    return;
}

void case_bypass(Node** root) {
    scanf("%*c");
    char* start = my_getline("Введите начало диапазона: ", stdin);
    scanf("%*c");
    char* end = my_getline("Введите конец диапазона: ", stdin);
    printf("=== Прямой обход дерева: ===\n");
    bypass_in_order(*root, start, end);
    if (start) {
        free(start);
    }
    if (end) {
        free(end);
    }
    return;
}

void case_find_by_key(Node** root) {
    scanf("%*c");
    char* key = my_getline("Введите ключ: ", stdin);
    Node** arr = NULL;
    int size = 0;
    ErrorCode err = find_key(&arr, &size, *root, key);
    if (err == OK) {
        printf("Элементов найдено: %u\n", size);
        for (int i = 0; i < size; i++) {
            printf("Ключ: %s, Значение: %u\n", arr[i] -> key, arr[i] -> info);
        }
        free(arr);
    } else {
        printf("%s\n", error_message(err));
    }
    if (key) {
        free(key);
    }
}

void case_find_closest_key(Node** root) {
    scanf("%*c");
    char* key = my_getline("Введите ключ: ", stdin);
    Node** arr = NULL;
    int size = 0;
    ErrorCode err = find_closest_key(&arr, &size, *root, key);
    if (err == OK) {
        printf("Элементов найдено: %u\n", size);
        for (int i = 0; i < size; i++) {
            printf("Ключ: %s, Значение: %u\n", arr[i] -> key, arr[i] -> info);
        }
        free(arr);
    } else {
        printf("%s\n", error_message(err));
    }
    if (key) {
        free(key);
    }
}

void case_import(Node** root) {
    scanf("%*c");
    char* filename = my_getline("Введите имя файла: ", stdin);
    ErrorCode err = import_from_file(root, filename);
    if (err == OK) {
        printf("Данные успешно импортированы\n");
    } else {
        printf("%s\n", error_message(err));
    }
    if (filename) {
        free(filename);
    }
}

void case_graph(Node** root) {
    scanf("%*c");
    char* filename = my_getline("Введите имя файла: ", stdin);
    ErrorCode err = graphviz_tree(*root, filename);
    if (err == OK) {
        printf("Дерево построено в файле %s.png\n", filename);
    } else {
        printf("%s\n", error_message(err));
    }
    if (filename) {
        free(filename);
    }
}
