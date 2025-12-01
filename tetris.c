#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// -----------------------------------------------------------
// Estrutura que representa uma peça do Tetris
// -----------------------------------------------------------
typedef struct {
    char nome;   // tipo da peça ('I', 'O', 'T', 'L')
    int id;      // identificador único
} Peca;

// Tipos possíveis de peças
char tipos[4] = {'I', 'O', 'T', 'L'};

// -----------------------------------------------------------
// Função: gerarPeca
// Gera automaticamente uma peça com tipo aleatório e id único
// -----------------------------------------------------------
Peca gerarPeca(int id) {
    Peca p;
    p.nome = tipos[rand() % 4];
    p.id = id;
    return p;
}

// -----------------------------------------------------------
// Função: exibirFila
// Exibe os elementos da fila circular do front até o rear
// -----------------------------------------------------------
void exibirFila(Peca fila[], int frente, int tras, int tamanho) {
    printf("Fila de peças: ");

    if (frente == -1) {
        printf("(vazia)");
        return;
    }

    int i = frente;
    while (1) {
        printf("[%c %d] ", fila[i].nome, fila[i].id);
        if (i == tras) break;
        i = (i + 1) % tamanho;
    }
}

// -----------------------------------------------------------
// Função: exibirPilha
// Exibe os elementos da pilha do topo até a base
// -----------------------------------------------------------
void exibirPilha(Peca pilha[], int topo) {
    printf("Pilha de reserva (Topo -> Base): ");

    if (topo == -1) {
        printf("(vazia)");
        return;
    }

    for (int i = topo; i >= 0; i--) {
        printf("[%c %d] ", pilha[i].nome, pilha[i].id);
    }
}

// -----------------------------------------------------------
// Enqueue - inserir elemento na fila circular
// -----------------------------------------------------------
int enqueue(Peca fila[], int *frente, int *tras, int tamanho, Peca p) {
    if (*frente == -1) { // fila vazia
        *frente = *tras = 0;
        fila[*tras] = p;
        return 1;
    }

    int proximaPos = (*tras + 1) % tamanho;

    if (proximaPos == *frente) {
        return 0; // fila cheia
    }

    *tras = proximaPos;
    fila[*tras] = p;
    return 1;
}

// -----------------------------------------------------------
// Dequeue - remover elemento da fila circular
// -----------------------------------------------------------
int dequeue(Peca fila[], int *frente, int *tras, int tamanho, Peca *removida) {
    if (*frente == -1) return 0; // fila vazia

    *removida = fila[*frente];

    if (*frente == *tras) {
        *frente = *tras = -1;
    } else {
        *frente = (*frente + 1) % tamanho;
    }

    return 1;
}

// -----------------------------------------------------------
// Push - inserir na pilha
// -----------------------------------------------------------
int push(Peca pilha[], int *topo, int capacidade, Peca p) {
    if (*topo == capacidade - 1) return 0; // pilha cheia

    *topo = *topo + 1;
    pilha[*topo] = p;
    return 1;
}

// -----------------------------------------------------------
// Pop - remover peça do topo da pilha
// -----------------------------------------------------------
int pop(Peca pilha[], int *topo, Peca *removida) {
    if (*topo == -1) return 0; // pilha vazia

    *removida = pilha[*topo];
    *topo = *topo - 1;
    return 1;
}

// -----------------------------------------------------------
// Função principal
// -----------------------------------------------------------
int main() {
    srand(time(NULL));

    const int TAM_FILA = 5;
    const int TAM_PILHA = 3;

    // Estruturas de dados
    Peca fila[TAM_FILA];
    Peca pilha[TAM_PILHA];

    int frente = -1, tras = -1;
    int topo = -1;
    int ultimoID = 0;

    // Inicializa a fila com 5 peças
    for (int i = 0; i < TAM_FILA; i++) {
        enqueue(fila, &frente, &tras, TAM_FILA, gerarPeca(ultimoID++));
    }

    int opcao;

    do {
        printf("\n============================\n");
        printf("ESTADO ATUAL\n");

        exibirFila(fila, frente, tras, TAM_FILA);
        printf("\n");
        exibirPilha(pilha, topo);
        printf("\n============================\n");

        printf("\nOpções:\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça reservada\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            Peca jogada;

            if (dequeue(fila, &frente, &tras, TAM_FILA, &jogada)) {
                printf("\nPeça jogada: [%c %d]\n", jogada.nome, jogada.id);
            }

            // Sempre inserir nova peça após a ação
            enqueue(fila, &frente, &tras, TAM_FILA, gerarPeca(ultimoID++));

        } else if (opcao == 2) {
            Peca reservada;

            if (!dequeue(fila, &frente, &tras, TAM_FILA, &reservada)) {
                printf("\nFila vazia! Não é possível reservar.\n");
            } else if (!push(pilha, &topo, TAM_PILHA, reservada)) {
                printf("\nPilha cheia! Não é possível reservar.\n");
            } else {
                printf("\nPeça reservada: [%c %d]\n", reservada.nome, reservada.id);
            }

            enqueue(fila, &frente, &tras, TAM_FILA, gerarPeca(ultimoID++));

        } else if (opcao == 3) {
            Peca usada;

            if (pop(pilha, &topo, &usada)) {
                printf("\nPeça usada: [%c %d]\n", usada.nome, usada.id);
            } else {
                printf("\nPilha vazia! Nenhuma peça para usar.\n");
            }

            enqueue(fila, &frente, &tras, TAM_FILA, gerarPeca(ultimoID++));

        }

    } while (opcao != 0);

    printf("\nEncerrando o programa...\n");
    return 0;
}
