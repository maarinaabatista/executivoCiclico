
#include <string.h>
#include <stdio.h>


// Função que calcula o Máximo Divisor Comum (MDC) - Tempo de ciclo Secundário
int calcularMDC(int a, int b) {
    int resto;
    
    // Algoritmo de Euclides para calcular o MDC
    while (b != 0) {
        resto = a % b;
        a = b;
        b = resto;
    }
    
    return a;
}
// Função que calcula o (MMC) - Tempo de ciclo Primário
int calcularMMC(int a, int b) {
    return (a * b) / calcularMDC(a, b);
}

