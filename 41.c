#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct {
    Node* tail;
    int size;
} CyclicList;

/* ===== COLORS ===== */
#define RESET   ""
#define CYAN    ""
#define YELLOW  ""
#define GREEN   ""
#define RED     ""
#define BLUE    ""

/* ===== CONSOLE ===== */
void clearScreen() { system("cls"); }

int getKey() {
    int ch = _getch();
    if (ch == 0 || ch == 224) {
        ch = _getch();
        if (ch == 72) return 1;  // UP
        if (ch == 80) return 2;  // DOWN
    }
    if (ch == 13) return 3;      // Enter
    if (ch == 27) return 4;      // Esc
    return 0;
}

/* ===== LIST OPERATIONS ===== */
CyclicList* createList() {
    CyclicList* list = (CyclicList*)malloc(sizeof(CyclicList));
    list->tail = NULL;
    list->size = 0;
    return list;
}

void addToHead(CyclicList* list, int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = value;
    if (list->tail == NULL) {
        list->tail = newNode;
        newNode->next = newNode;
    } else {
        newNode->next = list->tail->next;
        list->tail->next = newNode;
    }
    list->size++;
    printf(GREEN "Added %d to head.\n" RESET, value);
}

void addToTail(CyclicList* list, int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = value;
    if (list->tail == NULL) {
        list->tail = newNode;
        newNode->next = newNode;
    } else {
        newNode->next = list->tail->next;
        list->tail->next = newNode;
        list->tail = newNode;
    }
    list->size++;
    printf(GREEN "Added %d to tail.\n" RESET, value);
}

void deleteHead(CyclicList* list) {
    if (list->tail == NULL) {
        printf(RED "List is empty!\n" RESET);
        return;
    }
    if (list->size == 1) {
        printf(GREEN "Deleted: %d\n" RESET, list->tail->data);
        free(list->tail);
        list->tail = NULL;
    } else {
        Node* head = list->tail->next;
        printf(GREEN "Deleted head: %d\n" RESET, head->data);
        list->tail->next = head->next;
        free(head);
    }
    list->size--;
}

void deleteByValue(CyclicList* list, int value) {
    if (list->tail == NULL) {
        printf(RED "List is empty!\n" RESET);
        return;
    }
    Node* curr = list->tail->next;
    Node* prev = list->tail;
    int i;
    for (i = 0; i < list->size; i++) {
        if (curr->data == value) {
            if (curr == list->tail) {
                if (list->size == 1) {
                    free(curr);
                    list->tail = NULL;
                } else {
                    prev->next = curr->next;
                    list->tail = prev;
                    free(curr);
                }
            } else {
                prev->next = curr->next;
                free(curr);
            }
            list->size--;
            printf(GREEN "Deleted: %d\n" RESET, value);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    printf(RED "Element %d not found!\n" RESET, value);
}

void search(CyclicList* list, int value) {
    if (list->tail == NULL) {
        printf(RED "List is empty!\n" RESET);
        return;
    }
    Node* curr = list->tail->next;
    int pos = 0;
    do {
        if (curr->data == value) {
            printf(GREEN "Found %d at position %d\n" RESET, value, pos);
            return;
        }
        curr = curr->next;
        pos++;
    } while (curr != list->tail->next);
    printf(RED "Element %d not found!\n" RESET, value);
}

void printList(CyclicList* list) {
    if (list->tail == NULL) {
        printf(BLUE "List is empty.\n" RESET);
        return;
    }
    printf(BLUE "Cyclic list (%d elements): " RESET, list->size);
    Node* curr = list->tail->next;
    int count = 0;
    do {
        printf("[%d] -> ", curr->data);
        curr = curr->next;
        count++;
    } while (curr != list->tail->next && count <= list->size);
    printf("[%d] -> (back to start)\n", list->tail->next->data);
}

void clearList(CyclicList* list) {
    if (list->tail == NULL) return;
    Node* curr = list->tail->next;
    int n = list->size;
    int i;
    for (i = 0; i < n; i++) {
        Node* temp = curr;
        curr = curr->next;
        free(temp);
    }
    list->tail = NULL;
    list->size = 0;
    printf(GREEN "List cleared.\n" RESET);
}

void destroyList(CyclicList* list) {
    clearList(list);
    free(list);
}

/* ===== MENU ===== */
typedef struct {
    const char* items[15];
    int count;
    int selected;
    const char* title;
} Menu;

void drawMenu(Menu* m) {
    clearScreen();
    printf(CYAN
        "+===========================================+\n"
        "|     CYCLIC SINGLY LINKED LIST             |\n"
        "+===========================================+\n" RESET);
    printf(CYAN "  %s\n\n" RESET, m->title);
    int i;
    for (i = 0; i < m->count; i++) {
        if (i == m->selected)
            printf(YELLOW "  --> %s\n" RESET, m->items[i]);
        else
            printf("      %s\n", m->items[i]);
    }
    printf(BLUE "\n  [UP/DOWN] navigate   [Enter] select   [Esc] back\n" RESET);
}

int runMenu(Menu* m) {
    while (1) {
        drawMenu(m);
        int key = getKey();
        if (key == 1) m->selected = (m->selected - 1 + m->count) % m->count;
        if (key == 2) m->selected = (m->selected + 1) % m->count;
        if (key == 3) return m->selected;
        if (key == 4) return -1;
    }
}

int readInt(const char* prompt) {
    int x;
    printf("%s", prompt);
    scanf("%d", &x);
    while (getchar() != '\n');
    return x;
}

void waitEnter() {
    printf(BLUE "\nPress Enter to continue..." RESET);
    while (getchar() != '\n');
}

/* ===== MAIN MENU ===== */
void mainMenu(CyclicList* list) {
    Menu m;
    m.title = "Main Menu";
    m.count = 8;
    m.selected = 0;
    m.items[0] = "Show list";
    m.items[1] = "Add to head";
    m.items[2] = "Add to tail";
    m.items[3] = "Delete first element";
    m.items[4] = "Delete by value";
    m.items[5] = "Search element";
    m.items[6] = "Clear list";
    m.items[7] = "Exit";

    while (1) {
        int choice = runMenu(&m);
        if (choice == -1 || choice == 7) break;
        clearScreen();
        switch (choice) {
            case 0:
                printList(list);
                break;
            case 1:
                addToHead(list, readInt("Enter value: "));
                break;
            case 2:
                addToTail(list, readInt("Enter value: "));
                break;
            case 3:
                deleteHead(list);
                break;
            case 4:
                printList(list);
                if (list->size > 0)
                    deleteByValue(list, readInt("Enter value to delete: "));
                break;
            case 5:
                if (list->size > 0)
                    search(list, readInt("Enter value to search: "));
                else
                    printf(RED "List is empty!\n" RESET);
                break;
            case 6:
                clearList(list);
                break;
        }
        waitEnter();
    }
}

/* ===== MAIN ===== */
int main() {
    

    CyclicList* list = createList();
    mainMenu(list);
    destroyList(list);

    clearScreen();
    printf(GREEN "Program finished.\n" RESET);
    return 0;
}
