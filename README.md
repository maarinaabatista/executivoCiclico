
# Projeto Executivo Cíclico

O Executivo Cíclico é um modelo de escalonamento comumente utilizado em sistemas de tempo real e sistemas embarcados. Ele opera dividindo o tempo em ciclos e executando tarefas em cada ciclo, garantindo que as tarefas sejam executadas de acordo com seus requisitos de periodicidade. O objetivo é garantir que todas as tarefas sejam atendidas dentro de seus períodos e que o sistema seja eficiente em termos de utilização de recursos.

## Funcionalidades

1. **Cálculo dos tempos de ciclo**:
   - O tempo de ciclo primário é calculado como o Mínimo Múltiplo Comum (MMC) de todos os períodos das tarefas.
   - O tempo de ciclo secundário é determinado como o Máximo Divisor Comum (MDC) dos períodos das tarefas.

2. **Cálculo do escalonamento usando a heurística SETF**:
   - Aplica a heurística Menor Tempo de Execução Primeiro (SETF) para sugerir um escalonamento que prioriza a execução das tarefas com menor tempo de execução.
    - #### Funcionamento
        1. **Ordenação**: As tarefas são ordenadas em ordem crescente de tempo de execução.
        2. **Execução**: O sistema executa as tarefas da fila ordenada, priorizando as mais curtas primeiro.
## Estrutura do Projeto

O projeto consiste nos seguintes arquivos:

- `executivoCiclico.c`: Implementação do sistema de Executivo Cíclico.
- `tarefas.json`: Arquivo de entrada no formato JSON que especifica as informações das tarefas.

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
  - Tarefa2: tempo de execução = 20, período = 200, prioridade = 2

Ciclo 4:
  - Tarefa3: tempo de execução = 5, período = 50, prioridade = 3
  - Tarefa1: tempo de execução = 10, período = 100, prioridade = 1

Resumo:
-------
Total de Ciclos: 4
Total de Intercâmbios de Tarefa por Ciclo: 6
Utilização da CPU: 85%
```
## Compilação e Execução

Para compilar o projeto, utilize o seguinte comando no terminal:

```bash
gcc -o exec_ciclico executivo_ciclico.c -ljson-c
```
Para executar o programa, utilize o comando:

```bash
./exec_ciclico
```

## Video Demonstração
