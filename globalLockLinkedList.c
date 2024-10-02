#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* head = NULL;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  // Mutex global

// Función para insertar un nodo al inicio de la lista
void insert(int value) {
    pthread_mutex_lock(&mutex);  // Adquirir el candado
    Node* new_node = (Node*) malloc(sizeof(Node));
    new_node->data = value;
    new_node->next = head;
    head = new_node;
    pthread_mutex_unlock(&mutex);  // Liberar el candado
}

// Función para eliminar un nodo con un valor específico
void delete(int value) {
    pthread_mutex_lock(&mutex);  // Adquirir el candado
    Node *current = head, *prev = NULL;

    while (current != NULL && current->data != value) {
        prev = current;
        current = current->next;
    }

    if (current != NULL) {  // Nodo encontrado
        if (prev == NULL) {  // El nodo a eliminar es la cabeza
            head = current->next;
        } else {
            prev->next = current->next;
        }
        free(current);
    }
    pthread_mutex_unlock(&mutex);  // Liberar el candado
}

// Función para buscar un valor en la lista
int search(int value) {
    pthread_mutex_lock(&mutex);  // Adquirir el candado
    Node* current = head;
    while (current != NULL) {
        if (current->data == value) {
            pthread_mutex_unlock(&mutex);  // Liberar el candado
            return 1;  // Encontrado
        }
        current = current->next;
    }
    pthread_mutex_unlock(&mutex);  // Liberar el candado
    return 0;  // No encontrado
}

// Función para imprimir la lista
void print_list() {
    pthread_mutex_lock(&mutex);  // Adquirir el candado
    Node* current = head;
    while (current != NULL) {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
    pthread_mutex_unlock(&mutex);  // Liberar el candado
}

// Función para medir el rendimiento de la inserción
void measure_insert(int num_inserts) {
    clock_t start = clock();
    for (int i = 0; i < num_inserts; i++) {
        insert(i);
    }
    clock_t end = clock();
    printf("Tiempo de inserción: %lf segundos\n", (double)(end - start) / CLOCKS_PER_SEC);
}

// Función para medir el rendimiento de la búsqueda
void measure_search(int num_searches) {
    clock_t start = clock();
    for (int i = 0; i < num_searches; i++) {
        search(i);
    }
    clock_t end = clock();
    printf("Tiempo de búsqueda: %lf segundos\n", (double)(end - start) / CLOCKS_PER_SEC);
}

// Función para medir el rendimiento de la impresión
void measure_print() {
    clock_t start = clock();
    print_list();
    clock_t end = clock();
    printf("Tiempo de impresión: %lf segundos\n", (double)(end - start) / CLOCKS_PER_SEC);
}

// Función principal
int main() {
    measure_insert(1000);
    measure_print();
    measure_search(1000);
    delete(2);
    measure_print();
    return 0;
}
