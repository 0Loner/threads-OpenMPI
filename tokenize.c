#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

#define MAX 100
#define thread_count 4

sem_t sems[thread_count];

void *Tokenize(void* rank) {
    long my_rank = (long) rank;
    int count;
    int next = (my_rank + 1) % thread_count;
    char *fg_rv;
    char my_line[MAX];
    char *my_string;

    // Simulación de entrada para cada hilo (en vez de stdin, usaremos líneas de texto fijas)
    const char *lines[] = {
        "Pease porridge hot.",
        "Pease porridge cold.",
        "Pease porridge in the pot",
        "Nine days old."
    };
    int num_lines = 4;

    sem_wait(&sems[my_rank]);
    if (my_rank < num_lines) {
        strcpy(my_line, lines[my_rank]);  // Obtener la línea correspondiente al hilo
        fg_rv = my_line;
    } else {
        fg_rv = NULL;
    }
    sem_post(&sems[next]);

    while (fg_rv != NULL) {
        printf("Thread %ld > my line = %s\n", my_rank, my_line);
        count = 0;
        my_string = strtok(my_line, " \t\n");  // Tokenizar por espacios, tabulaciones o saltos de línea
        while (my_string != NULL) {
            count++;
            printf("Thread %ld > string %d = %s\n", my_rank, count, my_string);
            my_string = strtok(NULL, " \t\n");
        }

        sem_wait(&sems[my_rank]);
        fg_rv = NULL;  // No más líneas para leer en este ejemplo
        sem_post(&sems[next]);
    }

    return NULL;
}

int main() {
    long thread;
    pthread_t threads[thread_count];

    // Inicialización de semáforos
    for (thread = 0; thread < thread_count; thread++) {
        sem_init(&sems[thread], 0, (thread == 0) ? 1 : 0);
    }

    // Creación de hilos
    for (thread = 0; thread < thread_count; thread++) {
        pthread_create(&threads[thread], NULL, Tokenize, (void*) thread);
    }

    // Unión de hilos
    for (thread = 0; thread < thread_count; thread++) {
        pthread_join(threads[thread], NULL);
    }

    // Destrucción de semáforos
    for (thread = 0; thread < thread_count; thread++) {
        sem_destroy(&sems[thread]);
    }

    return 0;
}

