#include <stdio.h>
#include <json-c/json.h>
#include <string.h>

// Definição da estrutura Tarefa para armazenar informações de cada tarefa
typedef struct {
    char *id;
    int periodo;
    int tempo_execucao;
    int prioridade;
    float taxa_periodicidade;
} Tarefa;

// Função para calcular o Mínimo Múltiplo Comum (MMC) de dois números
int calcularMMC(int a, int b) {
    int temp, c;
    temp = a * b;
    
    // Algoritmo de Euclides para calcular o MMC
    while (b != 0) {
        c = b;
        b = a % b;
        a = c;
    }
    
    return temp / a;
}

// Função para calcular o Máximo Divisor Comum (MDC) de dois números
int calcularMDC(int a, int b) {
    int c;
    
    // Algoritmo de Euclides para calcular o MDC
    while (b != 0) {
        c = a % b;
        a = b;
        b = c;
    }
    
    return a;
}

// Função de comparação para ordenar as tarefas pelo tempo de execução (Menor Tempo de Execução Primeiro)
int compararTempoExecucao(const void *a, const void *b) {
    Tarefa *t1 = (Tarefa *)a;
    Tarefa *t2 = (Tarefa *)b;

    return t1->tempo_execucao - t2->tempo_execucao;
}

int main() {
    FILE *fp;
    struct json_object *parsed_json;
    struct json_object *tarefas_obj;
    struct json_object *tarefa_obj;
    struct json_object *id_obj, *periodo_obj, *tempo_execucao_obj, *prioridade_obj;

    size_t n_tarefas;
    size_t i;

    // Abertura e leitura do arquivo JSON
    fp = fopen("tarefas.json", "r");
    if (fp == NULL) {
        fprintf(stderr, "Não foi possível abrir o arquivo.\n");
        return 1;
    }

    char buffer[1024];
    fread(buffer, sizeof(buffer), 1, fp);
    fclose(fp);

    // Parse do JSON
    parsed_json = json_tokener_parse(buffer);

    // Obtém o array de tarefas do JSON
    json_object_object_get_ex(parsed_json, "tarefas", &tarefas_obj);
    n_tarefas = json_object_array_length(tarefas_obj);

    Tarefa tarefas[n_tarefas]; // Array para armazenar as tarefas

    // Ler e armazenar informações das tarefas do JSON
    for (i = 0; i < n_tarefas; i++) {
        tarefa_obj = json_object_array_get_idx(tarefas_obj, i);

        // Obtém as informações de cada tarefa do JSON
        json_object_object_get_ex(tarefa_obj, "id", &id_obj);
        json_object_object_get_ex(tarefa_obj, "periodo", &periodo_obj);
        json_object_object_get_ex(tarefa_obj, "tempo_execucao", &tempo_execucao_obj);
        json_object_object_get_ex(tarefa_obj, "prioridade", &prioridade_obj);

        // Armazena as informações na estrutura Tarefa
        tarefas[i].id = strdup(json_object_get_string(id_obj));
        tarefas[i].periodo = json_object_get_int(periodo_obj);
        tarefas[i].tempo_execucao = json_object_get_int(tempo_execucao_obj);
        tarefas[i].prioridade = json_object_get_int(prioridade_obj);
        tarefas[i].taxa_periodicidade = 1.0 / tarefas[i].periodo;
    }

    // Ordena as tarefas pelo tempo de execução (Menor Tempo de Execução Primeiro)
    qsort(tarefas, n_tarefas, sizeof(Tarefa), compararTempoExecucao);

    // Cálculo do MMC e MDC dos períodos das tarefas
    int mmc = tarefas[0].periodo;
    int mdc = tarefas[0].periodo;

    for (i = 1; i < n_tarefas; i++) {
        mmc = calcularMMC(mmc, tarefas[i].periodo);
        mdc = calcularMDC(mdc, tarefas[i].periodo);
    }

    // Cálculo da taxa de utilização das tarefas
    float utilizacao = 0.0;
    for (i = 0; i < n_tarefas; i++) {
        utilizacao += (float)tarefas[i].tempo_execucao / tarefas[i].periodo;
    }

    printf("Taxa de Utilização: %.2f\n", utilizacao);

    // Verifica se a taxa de utilização é maior que 1 (sistema inviável)
    if (utilizacao > 1) {
        printf("O sistema não é viável.\n");
        return 1;
    }

    // Cálculo do número total de ciclos e intercâmbios de tarefa por ciclo
    int totalCiclos = mmc / mdc;
    int totalInter = n_tarefas * totalCiclos;
    float utilizacaoCPU = ((float)totalInter / totalCiclos) / n_tarefas * 100;

    printf("Cálculo de Ciclos para o Executivo Cíclico:\n");
    printf("-------------------------------------------\n");
    printf("Tempo de Ciclo Primário: %d unidades de tempo\n", mmc);
    printf("Tempo de Ciclo Secundário: %d unidades de tempo\n", mdc);

    printf("\nEscalonamento Sugerido (Heurística: Menor Tempo de Execução Primeiro):\n");
    printf("----------------------------------------------------------------------\n");

    for (int ciclo = 1; ciclo <= totalCiclos; ciclo++) {
        printf("Ciclo %d:\n", ciclo);

        for (i = 0; i < n_tarefas; i++) {
            printf("  - %s: tempo de execução = %d, período = %d, prioridade = %d\n",
                   tarefas[i].id, tarefas[i].tempo_execucao, tarefas[i].periodo, tarefas[i].prioridade);
        }
    }

    printf("\nResumo:\n");
    printf("-------\n");
    printf("Total de Ciclos: %d\n", totalCiclos);
    printf("Total de Intercâmbios de Tarefa por Ciclo: %d\n", totalInter);
    printf("Utilização da CPU: %.2f%%\n", utilizacaoCPU);

    // Liberação da memória alocada para os IDs das tarefas e do objeto JSON
    for (i = 0; i < n_tarefas; i++) {
        free(tarefas[i].id);
    }

    json_object_put(parsed_json);

    return 0;
}
