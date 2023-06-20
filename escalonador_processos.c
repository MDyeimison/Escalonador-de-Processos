#include <stdio.h>
#include <stdlib.h>

#define NUM_FILAS 4

// Definição do PCB (Bloco de Controle de Processos)
struct PCB {
    int pid;
    int prioridade;
    int tempoRestante;
    struct PCB* proximo;
    char nome;
    char data_hora;
    char endereco_inicial;
    char endereco_final;
};

// Função para criar um novo nó do PCB
struct PCB* criarPCB(int pid, int prioridade, int tempoRestante) {
    struct PCB* novoPCB = (struct PCB*)malloc(sizeof(struct PCB));
    novoPCB->pid = pid;
    novoPCB->prioridade = prioridade;
    novoPCB->tempoRestante = tempoRestante;
    novoPCB->proximo = NULL;
    return novoPCB;
}

// Estrutura para representar a fila circular
struct FilaCircular {
    struct PCB* frente;
    struct PCB* tras;
};

// Inicializa uma fila vazia
void inicializarFila(struct FilaCircular* fila) {
    fila->frente = NULL;
    fila->tras = NULL;
}

// Verifica se a fila está vazia
int estaVazia(struct FilaCircular* fila) {
    return (fila->frente == NULL);
}

// Insere um processo na fila
void inserir(struct FilaCircular* fila, struct PCB* processo) {
    if (estaVazia(fila)) {
        fila->frente = processo;
        fila->tras = processo;
        processo->proximo = fila->frente;
    } else {
        fila->tras->proximo = processo;
        fila->tras = processo;
        fila->tras->proximo = fila->frente;
    }
}

// Remove e retorna o primeiro processo da fila
struct PCB* remover(struct FilaCircular* fila) {
    if (estaVazia(fila)) {
        return NULL;
    } else if (fila->frente == fila->tras) {
        struct PCB* temp = fila->frente;
        fila->frente = NULL;
        fila->tras = NULL;
        return temp;
    } else {
        struct PCB* temp = fila->frente;
        fila->frente = fila->frente->proximo;
        fila->tras->proximo = fila->frente;
        return temp;
    }
}

// Exibe os processos na fila
void exibirFila(struct FilaCircular* fila) {
    if (estaVazia(fila)) {
        printf("Fila vazia.\n");
    } else {
        struct PCB* temp = fila->frente;
        printf("Processos na fila: ");
        do {
            printf("[%d] ", temp->pid);
            temp = temp->proximo;
        } while (temp != fila->frente);
        printf("\n");
    }
}

int main() {
    const int quantum[NUM_FILAS] = {2, 4, 6, 8}; // Quantum de tempo para cada fila
    struct FilaCircular filas[NUM_FILAS];

    // Inicializa as filas
    for (int i = 0; i < NUM_FILAS; i++) {
        inicializarFila(&filas[i]);
    }

    // Criando alguns processos para inserir nas filas
    struct PCB* processo1 = criarPCB(1, 2, 6);
    struct PCB* processo2 = criarPCB(2, 0, 10);
    struct PCB* processo3 = criarPCB(3, 1, 4);
    struct PCB* processo4 = criarPCB(4, 3, 8);
    struct PCB* processo5 = criarPCB(5, 1, 5);
    struct PCB* processo6 = criarPCB(6, 2, 7);

    // Inserindo os processos nas filas de acordo com suas prioridades
    inserir(&filas[processo1->prioridade], processo1);
    inserir(&filas[processo2->prioridade], processo2);
    inserir(&filas[processo3->prioridade], processo3);
    inserir(&filas[processo4->prioridade], processo4);
    inserir(&filas[processo5->prioridade], processo5);
    inserir(&filas[processo6->prioridade], processo6);

    // Executando os processos nas filas
    for (int i = 0; i < NUM_FILAS; i++) {
        printf("Executando fila %d (Prioridade %d):\n", i+1, i);

        int tempo = 0;
        while (!estaVazia(&filas[i])) {
            struct PCB* processoAtual = remover(&filas[i]);
            int tempoExecucao = (quantum[i] < processoAtual->tempoRestante) ? quantum[i] : processoAtual->tempoRestante;

            // Executa o processo pelo tempo de quantum ou pelo tempo restante, o que for menor
            for (int t = 0; t < tempoExecucao; t++) {
                printf("Executando processo %d (Prioridade %d) no tempo %d\n", processoAtual->pid, processoAtual->prioridade, tempo);
                tempo++;
                processoAtual->tempoRestante--;
            }

            // Verifica se o processo ainda tem tempo restante
            if (processoAtual->tempoRestante > 0) {
                // Insere o processo novamente na mesma fila com a prioridade reduzida em 1
                processoAtual->prioridade++;
                inserir(&filas[processoAtual->prioridade], processoAtual);
            } else {
                // Libera a memória do processo finalizado
                free(processoAtual);
            }
        }
    }

    return 0;
}
