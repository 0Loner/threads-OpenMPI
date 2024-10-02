#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct Node {
    int data;
    struct Node* next;
    pthread_mutex_t node_mutex;  // Mutex por nodo
} Node;

Node* head = NULL;
pthread_mutex_t list_mutex = PTHREAD_MUTEX_INITIALIZER;  // Proteger acceso a la cabeza

// Función para insertar un nodo al inicio de la lista
void insert(int value) {
    Node* new_node = (Node*) malloc(sizeof(Node));
    new_node->data = value;
    pthread_mutex_init(&new_node->node_mutex, NULL);  // Inicializar el mutex del nodo

    pthread_mutex_lock(&list_mutex);  // Proteger acceso a la cabeza
    new_node->next = head;
    head = new_node;
    pthread_mutex_unlock(&list_mutex);  // Liberar la cabeza
}

// Función para eliminar un nodo con un valor específico
void delete(int value) {
    pthread_mutex_lock(&list_mutex);  // Proteger la cabeza
    Node *current = head, *prev = NULL;
    if (current != NULL) pthread_mutex_lock(&current->node_mutex);  // Bloquear primer nodo
    pthread_mutex_unlock(&list_mutex);  // Liberar acceso a la cabeza

    while (current != NULL && current->data != value) {
        Node* next = current->next;
        if (next != NULL) pthread_mutex_lock(&next->node_mutex);  // Bloquear siguiente nodo
        pthread_mutex_unlock(&current->node_mutex);  // Liberar nodo actual
        prev = current;
        current = next;
    }

    if (current != NULL) {  // Nodo encontrado
        if (prev == NULL) {  // Eliminar la cabeza
            pthread_mutex_lock(&list_mutex);  // Proteger la cabeza
            head = current->next;
            pthread_mutex_unlock(&list_mutex);  // Liberar la cabeza
        } else {
            prev->next = current->next;
        }
        pthread_mutex_unlock(&current->node_mutex);  // Liberar el mutex antes de eliminar
        pthread_mutex_destroy(&current->node_mutex);  // Destruir el mutex del nodo
        free(current);
    }
}

// Función para buscar un valor en la lista
int search(int value) {
    pthread_mutex_lock(&list_mutex);  // Proteger la cabeza
    Node* current = head;
    if (current != NULL) pthread_mutex_lock(&current->node_mutex);  // Bloquear primer nodo
    pthread_mutex_unlock(&list_mutex);  // Liberar acceso a la cabeza

    while (current != NULL) {
        if (current->data == value) {
            pthread_mutex_unlock(&current->node_mutex);  // Liberar nodo actual
            return 1;  // Encontrado
        }
        Node* next = current->next;
        if (next != NULL) pthread_mutex_lock(&next->node_mutex);  // Bloquear siguiente nodo
        pthread_mutex_unlock(&current->node_mutex);  // Liberar nodo actual
        current = next;
    }
    return 0;  // No encontrado
}

// Función para imprimir la lista
void print_list() {
    pthread_mutex_lock(&list_mutex);  // Proteger la cabeza
    Node* current = head;
    if (current != NULL) pthread_mutex_lock(&current->node_mutex);  // Bloquear primer nodo
    pthread_mutex_unlock(&list_mutex);  // Liberar acceso a la cabeza

    while (current != NULL) {
        printf("%d -> ", current->data);
        Node* next = current->next;
        if (next != NULL) pthread_mutex_lock(&next->node_mutex);  // Bloquear siguiente nodo
        pthread_mutex_unlock(&current->node_mutex);  // Liberar nodo actual
        current = next;
    }
    printf("NULL\n");
}

// Función para medir el rendimiento de la inserción
void measure_insert(int num_inserts) {
    clock_t start = clock();
    for (int i = 0; i < num_inserts; i++) {
        insert(i);
    }
    clock_t end = clock();
    printf("Tiempo de insercion: %lf segundos\n", (double)(end - start) / CLOCKS_PER_SEC);
}

// Función para medir el rendimiento de la búsqueda
void measure_search(int num_searches) {
    clock_t start = clock();
    for (int i = 0; i < num_searches; i++) {
        search(i);
    }
    clock_t end = clock();
    printf("Tiempo de busqueda: %lf segundos\n", (double)(end - start) / CLOCKS_PER_SEC);
}

// Función para medir el rendimiento de la impresión
void measure_print() {
    clock_t start = clock();
    print_list();
    clock_t end = clock();
    printf("Tiempo de impresion: %lf segundos\n", (double)(end - start) / CLOCKS_PER_SEC);
}

// Función principal
int main() {
    measure_insert(100000);
    measure_search(1000);
    return 0;
}
