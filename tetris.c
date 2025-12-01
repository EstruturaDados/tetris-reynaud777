#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// -----------------------------------------------------------
// Estrutura que representa uma peça do Tetris
// nome -> caractere que identifica a peça ('I', 'O', 'T', 'L')
// id   -> número único gerado automaticamente
// -----------------------------------------------------------
typedef struct {
    char nome;
    int id;
} Peca;

// Lista fixa de possíveis nomes das peças
char tipos[4] = {'I', 'O', 'T', 'L'};

// -----------------------------------------------------------
// Função responsável por gerar automaticamente uma nova peça
// Retorna uma struct Peca com nome aleatório e id incremental.
// -----------------------------------------------------------
Peca gerarPeca(int id) {
    Peca nova;
    int indice = rand() % 4;  // sorteia um tipo de peça
    nova.nome = tipos[indice];
    nova.id = id;
    return nova;
}

// -----------------------------------------------------------
// Função para exibir todo o estado da fila
// Trabalha com fila circular, exibindo do front até rear.
// -----------------------------------------------------------
void exibirFila(Peca fila[], int frente, int tras, int tamanho) {
    printf("\nFila de peças:\n");

    if (frente == -1) {
        printf("(fila vazia)\n");
        return;
    }

    int i = frente;
    while (1) {
        printf("[%c %d] ", fila[i].nome, fila[i].id);
        if (i == tras) break;  // chegou ao final da fila
        i = (i + 1) % tamanho;
    }
    printf("\n");
}

// -----------------------------------------------------------
// Inserir nova peça no final da fila (enqueue)
// Usa lógica de fila circular.
// -----------------------------------------------------------
int enqueue(Peca fila[], int *frente, int *tras, int tamanho, Peca p) {

    // Caso especial: fila vazia
    if (*frente == -1) {
        *frente = 0;
        *tras = 0;
        fila[*tras] = p;
        return 1;
    }

    // Verifica se a fila está cheia
    int proxima = (*tras + 1) % tamanho;
    if (proxima == *frente) {
        return 0; // não há espaço
    }

    *tras = proxima;
    fila[*tras] = p;
    return 1;
}

// -----------------------------------------------------------
// Remover peça da frente da fila (dequeue)
// -----------------------------------------------------------
int dequeue(Peca fila[], int *frente, int *tras, int tamanho, Peca *removida) {

    if (*frente == -1) {
        return 0; // fila vazia
    }

    *removida = fila[*frente];

    // Se havia apenas um elemento
    if (*frente == *tras) {
        *frente = -1;
        *tras = -1;
    } else {
        *frente = (*frente + 1) % tamanho;
    }

    return 1;
}

// -----------------------------------------------------------
// Função principal
// -----------------------------------------------------------
int main() {
    srand(time(NULL));  // inicializa números aleatórios

    const int TAM = 5;      // tamanho fixo da fila
    Peca fila[TAM];         // array de peças
    int frente = -1, tras = -1;
    int ultimoID = 0;       // contador para ids únicos

    // Inicializa fila com 5 peças
    for (int i = 0; i < TAM; i++) {
        enqueue(fila, &frente, &tras, TAM, gerarPeca(ultimoID++));
    }

    int opcao;

    do {
        printf("\n============================\n");
        exibirFila(fila, frente, tras, TAM);

        printf("\nOpções:\n");
        printf("1 - Jogar peça (dequeue)\n");
        printf("2 - Inserir nova peça (enqueue)\n");
        printf("0 - Sair\n");
        printf("Sua escolha: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            Peca removida;

            if (dequeue(fila, &frente, &tras, TAM, &removida)) {
                printf("\nPeça jogada: [%c %d]\n", removida.nome, removida.id);
            } else {
                printf("\nA fila está vazia! Não há peça para jogar.\n");
            }

        } else if (opcao == 2) {
            Peca nova = gerarPeca(ultimoID++);

            if (enqueue(fila, &frente, &tras, TAM, nova)) {
                printf("\nPeça inserida: [%c %d]\n", nova.nome, nova.id);
            } else {
                printf("\nA fila está cheia! Não é possível inserir nova peça.\n");
            }
        }

    } while (opcao != 0);

    printf("\nEncerrando o programa...\n");

    return 0;
}
