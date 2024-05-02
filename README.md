
# Projeto Executivo Cíclico

O Executivo Cíclico é um modelo de escalonamento comumente utilizado em sistemas de tempo real e sistemas embarcados. Ele opera dividindo o tempo em ciclos e executando tarefas em cada ciclo, garantindo que as tarefas sejam executadas de acordo com seus requisitos de periodicidade. O objetivo é garantir que todas as tarefas sejam atendidas dentro de seus períodos e que o sistema seja eficiente em termos de utilização de recursos.

## Funcionalidades

1. **Cálculo dos tempos de ciclo**:
   - O tempo de ciclo primário é calculado como o Mínimo Múltiplo Comum (MMC) de todos os períodos das tarefas.
   - O tempo de ciclo secundário é determinado como o Máximo Divisor Comum (MDC) dos períodos das tarefas.

2. **Verificar viabilidade do escalonamento**:
   -  A taxa de utilização é calculada e partir desse valor foi estabelecida a condição que verifica se o sistema é viável ou não.

3. **Cálculo do escalonamento usando a heurística SETF**:
   - Aplica a heurística Menor Tempo de Execução Primeiro (SETF) para sugerir um escalonamento que prioriza a execução das tarefas com menor tempo de execução.
    - #### Funcionamento
        1. **Ordenação**: As tarefas são ordenadas em ordem crescente de tempo de execução.
        2. **Execução**: O sistema executa as tarefas da fila ordenada, priorizando as mais curtas primeiro.
           
## Estrutura do Projeto

O projeto consiste nos seguintes arquivos:

- `executivoCiclico.c`: Implementação do sistema de Executivo Cíclico.
- `tarefas.json`: Arquivo de entrada no formato JSON que especifica as informações das tarefas.
- `tCiclo.c`: Implementação das funções que calculam os tempos de ciclo primário e secundário
- `execuCiclicoEXE`: Código principal compilado
  
## Detalhes de Implementação

- A estrutura `Tarefa` é definida para armazenar as informações de cada tarefa.
- Funções são implementadas para calcular o MMC e o MDC.
- A heurística SETF é implementada para ordenar as tarefas com base no tempo de execução.
- As informações das tarefas são lidas do arquivo JSON e armazenadas em um array de estruturas.
- Os tempos de ciclo são calculados e o escalonamento é sugerido com base na heurística SETF.
- Verifica-se se a utilização da CPU é viável antes de gerar o escalonamento.

## Formato de Entrada
Para fornecer a carga de trabalho ao sistema, é necessário utilizar um arquivo de entrada no formato JSON. Cada tarefa é representada por um objeto com os seguintes atributos:

- **id**: Identificador único para cada tarefa.
- **periodo**: Período de cada tarefa, representando o intervalo de tempo entre as execuções.
- **tempo_execucao**: Tempo necessário para completar a tarefa.
- **prioridade**: Número que indica a prioridade da tarefa, sendo 1 a mais alta.

## Formato da Saída
Após processar o arquivo de entrada, o sistema apresentará os tempos de ciclo calculados e o escalonamento sugerido com base na heurística selecionada. A saída será exibida no console e seguirá o seguinte formato:

```bash
Cálculo de Ciclos para o Executivo Cíclico:
-------------------------------------------
Tempo de Ciclo Primário: 200 unidades de tempo
Tempo de Ciclo Secundário: 50 unidades de tempo

Escalonamento Sugerido (Heurística: Menor Tempo de Execução Primeiro):
----------------------------------------------------------------------
Ciclo 1:
  - Tarefa3: tempo de execução = 5, período = 50, prioridade = 3
  - Tarefa1: tempo de execução = 10, período = 100, prioridade = 1
  - Tarefa2: tempo de execução = 20, período = 200, prioridade = 2
Ciclo 2:
  - Tarefa3: tempo de execução = 5, período = 50, prioridade = 3
  - Tarefa1: tempo de execução = 10, período = 100, prioridade = 1
Ciclo 3:
  - Tarefa3: tempo de execução = 5, período = 50, prioridade = 3
Ciclo 4:
  - Tarefa3: tempo de execução = 5, período = 50, prioridade = 3

Resumo:
-------
Total de Ciclos: 4
Utilização da CPU: 30.00%


```
## Compilação e Execução

Para compilar o projeto, faça o clone do repositório e utilize o seguinte comando no terminal na pasta do projeto:

```bash
$ sudo apt-get install libjson-c-dev
$ gcc -o execuCiclicoEXE  executivoCicliclo.c tCiclo.c -ljson-c
```
Para executar o programa, utilize o comando:

```bash
./execuCiclicoEXE
```
## Descrição do Código

Aqui estão descritas as principais funções utilizadas no código.

* Para armazenar as informações de cada tarefa foi definida uma estrutura utilizando o seguinte modelo
```c
typedef struct {
    char *id;
    int periodo;
    int tempo_execucao;
    int prioridade;
} Tarefa;
```
* Para calcular os tempos de ciclos primário e secundário presentes no arquivo `tCiclo.c` foram criadas as seguintes funções para calcular o MDC(Máximo Divisor Comum)e o MMC(Mínimo Múltiplo Comum) 
```c

int calcularMDC(int a, int b) {
    int resto;

    while (b != 0) {
        resto = a % b;
        a = b;
        b = resto;
    }
    return a;
}
int calcularMMC(int a, int b) {
    return (a * b) / calcularMDC(a, b);
```
* Para utilizar o SETF foi necessário criar uma função que realiza a comparação entre o tempo de execução entre duas tarefas e retorna um valor inteiro que indica a ordem relativa das tarefas:
  
```c
int compararTempoExecucao(const void *a, const void *b) {
    Tarefa *t1 = (Tarefa *)a;
    Tarefa *t2 = (Tarefa *)b;

    return t1->tempo_execucao - t2->tempo_execucao;
}

```
### Função main()
Aqui são executadas as principais funções do código comoa leitura de um arquivo JSON, a análise dos dados, o cálculo de MMC e MDC, o escalonamento baseado em SETF de tarefas e a impressão dos resultados.

* **Arquivo json** : Nesta seção, o código abre o arquivo JSON "tarefas.json", lê seu conteúdo e armazena em um buffer. Se houver algum problema na abertura do arquivo, uma mensagem de erro é exibida.
  
```c
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
```
* Para obter o array de tarefas do objeto JSON e o número total de tarefas no array utilizamos:
  
```c
json_object_object_get_ex(parsed_json, "tarefas", &tarefas_obj);
n_tarefas = json_object_array_length(tarefas_obj);
```
* **Para ler e armazenar as tarefas**: Foi criado um `for` que percorre o array de tarefas do objeto JSON, extrai as informações de cada tarefa (como id, período, tempo de execução e prioridade) e as armazena em uma estrutura de dados do tipo Tarefa.

```c
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
}
```
* **Ordenar Tarefas** : Para ordenar as tarefas com base em seus tempos de execução, foi utilizado um algoritmo de ordenação `qsort()` na função `main()`. Este algoritmo recebe parâmetros que incluem a função `compararTempoExecucao`, a qual permite a comparação entre duas tarefas com base nesse critério.
  
```c
 qsort(tarefas, n_tarefas, sizeof(Tarefa), compararTempoExecucao);
```
* Em seguida, com as tarefas ordenadas, o Mínimo Múltiplo Comum (MMC) e o Máximo Divisor Comum (MDC) dos períodos das tarefas é calculado. Ele itera sobre todas as tarefas e utiliza funções auxiliares (calcularMMC() e calcularMDC()) para realizar esses cálculos.

```c
// Cálculo do MMC e MDC dos períodos das tarefas
int mmc = tarefas[0].periodo;
int mdc = tarefas[0].periodo;

for (i = 1; i < n_tarefas; i++) {
    mmc = calcularMMC(mmc, tarefas[i].periodo);
    mdc = calcularMDC(mdc, tarefas[i].periodo);
}
```
* **Taxa de Utilização**: Foi definido como calcular a taxa de utilização das tarefas somando a razão entre o tempo de execução e o período de cada tarefa. Isso é feito para cada tarefa presente no sistema.
  
```c

float utilizacao = 0.0;
for (i = 0; i < n_tarefas; i++) {
    utilizacao += (float)tarefas[i].tempo_execucao / tarefas[i].periodo;
}
```

* **Verificação da Viabilidade do Sistema** : Foi definido como verificar se a taxa de utilização calculada é maior que 1. Se for, isso indica que o sistema não é viável e uma mensagem é impressa para indicar isso.
  
```c
if (utilizacao > 1) {
    printf("O sistema não é viável.\n");
    return 1;
}
```

* **Número total de ciclos**:Aqui, o código calcula o número total de ciclos necessários para executar todas as tarefas no sistema. Ele divide o MMC pelo MDC dos períodos das tarefas.
  
```c
int totalCiclos = mmc / mdc;
```

* **Escalonamento** : Um `for` foi criado para percorrer todos os ciclos do Executivo Cíclico e, para cada ciclo, verifica quais tarefas podem ser executadas nesse ciclo com base no período do ciclo primário. Em seguida, imprime informações sobre as tarefas que podem ser executadas no ciclo atual
```c
  for (int ciclo = 1; ciclo <= totalCiclos; ciclo++) {
    
    for (i = 0; i < n_tarefas; i++) {
        
        if ((ciclo - 1) * tarefas[i].periodo < mmc) {
            printf("Ciclo %d:\n", ciclo+1);
            printf("  - %s: Te = %d, P = %d\n",
                   tarefas[i].id, tarefas[i].tempo_execucao, tarefas[i].periodo);
        }
    }
}
```

* **Resultados** : Em seguida, são impressos na tela os resultados obtidos
```c
printf("Cálculo de Ciclos para o Executivo Cíclico:\n");
printf("-------------------------------------------\n");
printf("Tempo de Ciclo Primário: %d unidades de tempo\n", mmc);
printf("Tempo de Ciclo Secundário: %d unidades de tempo\n", mdc);

printf("\nEscalonamento Sugerido (Heurística: Menor Tempo de Execução Primeiro):\n");
printf("----------------------------------------------------------------------\n");


printf("\nResumo:\n");
printf("-------\n");
printf("Total de Ciclos: %d\n", totalCiclos);
printf("Utilização da CPU: %.2f%%\n", utilizacao*100);

printf("---------------------");
```
