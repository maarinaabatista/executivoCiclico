#include <stdio.h>
#include <json-c/json.h>

#include <string.h>
typedef struct {
    char *id;
    int periodo;
    int tempo_execucao;
    int prioridade;
    float taxa_periodicidade; // Taxa de periodicidade = 1 / período
} Tarefa;

int calcularMMC(int a, int b) {
    int temp, c;
    temp = a * b;
    
    while (b != 0) {
        c = b;
        b = a % b;
        a = c;
    }
    
    return temp / a;
}

int calcularMDC(int a, int b) {
    int c;
    
    while (b != 0) {
        c = a % b;
        a = b;
        b = c;
    }
    
    return a;
}

int compararTaxaPeriodicidade(const void *a, const void *b) {
    Tarefa *t1 = (Tarefa *)a;
    Tarefa *t2 = (Tarefa *)b;

    // Ordena em ordem decrescente de taxa de periodicidade
    return t2->taxa_periodicidade - t1->taxa_periodicidade;
}

int main() {
    FILE *fp;
    struct json_object *parsed_json;
    struct json_object *tarefas_obj;
    struct json_object *tarefa_obj;
    struct json_object *id_obj, *periodo_obj, *tempo_execucao_obj, *prioridade_obj;

    size_t n_tarefas;
    size_t i;

    fp = fopen("tarefas.json", "r");
    if (fp == NULL) {
        fprintf(stderr, "Não foi possível abrir o arquivo.\n");
        return 1;
    }

    char buffer[1024];
    fread(buffer, sizeof(buffer), 1, fp);
    fclose(fp);

    parsed_json = json_tokener_parse(buffer);

    json_object_object_get_ex(parsed_json, "tarefas", &tarefas_obj);
    n_tarefas = json_object_array_length(tarefas_obj);

    Tarefa tarefas[n_tarefas];

    // Ler e armazenar informações das tarefas
    for (i = 0; i < n_tarefas; i++) {
        tarefa_obj = json_object_array_get_idx(tarefas_obj, i);

        json_object_object_get_ex(tarefa_obj, "id", &id_obj);
        json_object_object_get_ex(tarefa_obj, "periodo", &periodo_obj);
        json_object_object_get_ex(tarefa_obj, "tempo_execucao", &tempo_execucao_obj);
        json_object_object_get_ex(tarefa_obj, "prioridade", &prioridade_obj);

        tarefas[i].id = strdup(json_object_get_string(id_obj));
        tarefas[i].periodo = json_object_get_int(periodo_obj);
        tarefas[i].tempo_execucao = json_object_get_int(tempo_execucao_obj);
        tarefas[i].prioridade = json_object_get_int(prioridade_obj);
        tarefas[i].taxa_periodicidade = 1.0 / tarefas[i].periodo;
    }

    // Ordenar tarefas pela taxa de periodicidade (HRF)
    qsort(tarefas, n_tarefas, sizeof(Tarefa), compararTaxaPeriodicidade);

    // Calcular MMC e MDC dos períodos das tarefas
    int mmc = tarefas[0].periodo;
    int mdc = tarefas[0].periodo;

    for (i = 1; i < n_tarefas; i++) {
        mmc = calcularMMC(mmc, tarefas[i].periodo);
        mdc = calcularMDC(mdc, tarefas[i].periodo);
    }

    printf("Cálculo de Ciclos para o Executivo Cíclico:\n");
    printf("-------------------------------------------\n");
    printf("Tempo de Ciclo Primário: %d unidades de tempo\n", mmc);
    printf("Tempo de Ciclo Secundário: %d unidades de tempo\n", mdc);

    printf("\nEscalonamento Sugerido (Heurística: Maior Taxa de Periodicidade Primeiro - HRF):\n");
    printf("----------------------------------------------------------------------\n");

    for (i = 0; i < n_tarefas; i++) {
        printf("Tarefa %lu:\n", i+1);
        printf("  ID: %s\n", tarefas[i].id);
        printf("  Período: %d\n", tarefas[i].periodo);
        printf("  Tempo de Execução: %d\n", tarefas[i].tempo_execucao);
        printf("  Prioridade: %d\n", tarefas[i].prioridade);
    }

    // Liberar memória alocada
    for (i = 0; i < n_tarefas; i++) {
        free(tarefas[i].id);
    }

    json_object_put(parsed_json);

    return 0;
}
