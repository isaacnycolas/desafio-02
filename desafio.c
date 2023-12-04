#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct MemBlock {
    int start;
    int size;
    struct MemBlock *next;
} MemBlock;

typedef struct MemFreeList {
    MemBlock *head;
} MemFreeList;

MemFreeList* createMemFreeList() {
    MemFreeList *freeList = (MemFreeList*)malloc(sizeof(MemFreeList));
    if (freeList == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    freeList->head = NULL;
    return freeList;
}

void insertBlock(MemFreeList *freeList, int start, int size) {
    MemBlock *newBlock = (MemBlock*)malloc(sizeof(MemBlock));
    if (newBlock == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    newBlock->start = start;
    newBlock->size = size;
    newBlock->next = freeList->head;
    freeList->head = newBlock;
}

int findAndAllocate(MemFreeList *freeList, int size) {
    MemBlock *prev = NULL;
    MemBlock *current = freeList->head;
    while (current != NULL) {
        if (current->size >= size) {
            if (prev != NULL) {
                prev->next = current->next;
            } else {
                freeList->head = current->next;
            }
            int allocatedSpace = current->start;
            free(current);
            return allocatedSpace;
        }
        prev = current;
        current = current->next;
    }
    return -1;
}

void releaseMemory(MemFreeList *freeList) {
    MemBlock *current = freeList->head;
    while (current != NULL) {
        MemBlock *temp = current;
        current = current->next;
        free(temp);
    }
    free(freeList);
}

void displayFreeList(MemFreeList *freeList) {
    MemBlock *current = freeList->head;
    while (current != NULL) {
        printf("Espaço livre em %d de tamanho %d\n", current->start, current->size);
        current = current->next;
    }
}

int main() {
    srand(time(NULL));

    int numProcesses = 5;
    int processSizes[5] = {24, 8, 14, 26, 3};

    MemFreeList *freeList = createMemFreeList();

    // Alocar memória inicial
    for (int i = 0; i < numProcesses; i++) {
        insertBlock(freeList, rand() % 100, processSizes[i]);
    }

    int userDemand;
    printf("Informe a demanda de espaço livre de memória ou -1 para encerrar: ");
    scanf("%d", &userDemand);

    while (userDemand != -1) {
        // Validar a demanda do usuário
        if (userDemand > 0) {
            int allocatedSpace = findAndAllocate(freeList, userDemand);
            if (allocatedSpace != -1) {
                printf("Espaço livre alocado no espaço %d\n", allocatedSpace);
            } else {
                printf("Espaço insuficiente para alocar a demanda de %d\n", userDemand);
            }
            displayFreeList(freeList);
        } else {
            printf("Por favor, insira uma demanda válida maior que 0.\n");
        }

        printf("Informe a demanda de espaço livre de memória ou -1 para encerrar: ");
        scanf("%d", &userDemand);
    }

    // Exibir mensagem de encerramento ou resumo

    // Liberar toda a memória alocada ao encerrar
    releaseMemory(freeList);

    return 0;
}
