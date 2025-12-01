#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// -----------------------------------------------------------
// Estrutura que representa uma peça do Tetris.
// -----------------------------------------------------------
typedef struct {
    char nome;   // Tipo: 'I', 'O', 'T', 'L'
    int id;      // Identificador único incremental
} Peca;

// Tipos possíveis de peças
char tipos[4] = {'I', 'O', 'T', 'L'};

// -----------------------------------------------------------
// Função que gera uma nova peça automaticamente.
// -----------------------------------------------------------
Peca gerarPeca(int id) {
    Peca p;
    p.nome = tipos[rand() % 4];  // sorteia tipo
    p.id = id;
    return p;
}

// -----------------------------------------------------------
// Exibe a fila circular completa.
// -----------------------------------------------------------
void exibirFila(Peca fila[], int frente, int tras, int tamanho) {
    printf("Fila de peças: ");

    if (frente == -1) {
        printf("(vazia)\n");
        return;
    }

    int i = frente;
    while (1) {
        printf("[%c %d] ", fila[i].nome, fila[i].id);
        if (i == tras) break;
        i = (i + 1) % tamanho;
    }
    printf("\n");
}

// -----------------------------------------------------------
// Exibe a pilha do topo até a base.
// -----------------------------------------------------------
void exibirPilha(Peca pilha[], int topo) {
    printf("Pilha de reserva (Topo -> base): ");

    if (topo == -1) {
        printf("(vazia)\n");
        return;
    }

    for (int i = topo; i >= 0; i--) {
        printf("[%c %d] ", pilha[i].nome, pilha[i].id);
    }
    printf("\n");
}

// -----------------------------------------------------------
// Fila circular: inserir no final (enqueue)
// -----------------------------------------------------------
int enqueue(Peca fila[], int *frente, int *tras, int tamanho, Peca p) {
    if (*frente == -1) {
        *frente = *tras = 0;
        fila[*tras] = p;
        return 1;
    }

    int proxima = (*tras + 1) % tamanho;
    if (proxima == *frente) return 0; // fila cheia

    *tras = proxima;
    fila[*tras] = p;
    return 1;
}

// -----------------------------------------------------------
// Fila circular: remover da frente (dequeue)
// -----------------------------------------------------------
int dequeue(Peca fila[], int *frente, int *tras, int tamanho, Peca *removida) {
    if (*frente == -1) return 0;

    *removida = fila[*frente];

    if (*frente == *tras) {
        *frente = *tras = -1;
    } else {
        *frente = (*frente + 1) % tamanho;
    }

    return 1;
}

// -----------------------------------------------------------
// Pilha: inserir no topo
// -----------------------------------------------------------
int push(Peca pilha[], int *topo, int capacidade, Peca p) {
    if (*topo == capacidade - 1) return 0; // pilha cheia

    pilha[++(*topo)] = p;
    return 1;
}

// -----------------------------------------------------------
// Pilha: remover do topo
// -----------------------------------------------------------
int pop(Peca pilha[], int *topo, Peca *removida) {
    if (*topo == -1) return 0;

    *removida = pilha[(*topo)--];
    return 1;
}

// -----------------------------------------------------------
// Trocar a peça da frente da fila com o topo da pilha.
// -----------------------------------------------------------
int trocaSimples(Peca fila[], int frente, Peca pilha[], int topo) {
    if (frente == -1 || topo == -1) return 0;

    Peca temp = fila[frente];
    fila[frente] = pilha[topo];
    pilha[topo] = temp;

    return 1;
}

// -----------------------------------------------------------
// Troca múltipla: 3 primeiras peças da fila ↔ 3 peças da pilha.
// -----------------------------------------------------------
int trocaMultipla(Peca fila[], int frente, int tamanhoFila,
                  Peca pilha[], int topo) {

    if (topo < 2) return 0; // pilha precisa ter ao menos 3
    if (frente == -1) return 0;

    int pos = frente;

    for (int i = 0; i < 3; i++) {
        int indexFila = (pos + i) % tamanhoFila;
        int indexPilha = topo - i;

        Peca temp = fila[indexFila];
        fila[indexFila] = pilha[indexPilha];
        pilha[indexPilha] = temp;
    }

    return 1;
}

// -----------------------------------------------------------
// Função principal
// -----------------------------------------------------------
int main() {
    srand(time(NULL));

    const int TAM_FILA = 5;
    const int TAM_PILHA = 3;

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
        printf("\n==============================================\n");
        printf("ESTADO ATUAL\n");
        exibirFila(fila, frente, tras, TAM_FILA);
        exibirPilha(pilha, topo);
        printf("==============================================\n\n");

        printf("Menu:\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Enviar peça da fila para pilha de reserva\n");
        printf("3 - Usar peça da pilha de reserva\n");
        printf("4 - Trocar frente da fila com topo da pilha\n");
        printf("5 - Troca múltipla (3 da fila ↔ 3 da pilha)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            Peca jogada;
            if (dequeue(fila, &frente, &tras, TAM_FILA, &jogada)) {
                printf("\nPeça jogada: [%c %d]\n", jogada.nome, jogada.id);

                enqueue(fila, &frente, &tras, TAM_FILA, gerarPeca(ultimoID++));
            }

        } else if (opcao == 2) {
            Peca removida;

            if (dequeue(fila, &frente, &tras, TAM_FILA, &removida)) {

                if (push(pilha, &topo, TAM_PILHA, removida)) {
                    printf("\nPeça reservada: [%c %d]\n", removida.nome, removida.id);
                } else {
                    printf("\nPilha cheia! Peça descartada.\n");
                }

                enqueue(fila, &frente, &tras, TAM_FILA, gerarPeca(ultimoID++));
            }

        } else if (opcao == 3) {
            Peca usada;
            if (pop(pilha, &topo, &usada)) {
                printf("\nPeça usada: [%c %d]\n", usada.nome, usada.id);

                enqueue(fila, &frente, &tras, TAM_FILA, gerarPeca(ultimoID++));
            } else {
                printf("\nPilha vazia!\n");
            }

        } else if (opcao == 4) {
            if (trocaSimples(fila, frente, pilha, topo)) {
                printf("\nTroca simples realizada!\n");
            } else {
                printf("\nNão foi possível trocar!\n");
            }

        } else if (opcao == 5) {
            if (trocaMultipla(fila, frente, TAM_FILA, pilha, topo)) {
                printf("\nTroca múltipla realizada!\n");
            } else {
                printf("\nNão foi possível realizar troca múltipla!\n");
            }
        }

    } while (opcao != 0);

    printf("\nEncerrando programa...\n");

    return 0;
}
