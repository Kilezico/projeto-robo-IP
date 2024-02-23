#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int *proxMovimento(char tabuleiro[3][3], char simbolo) {
    int jogada[2];
    
    int casasOcupadas = 0;
    int casa[2]; // Só se tiver uma casa ocupada

    // Verifica se é a primeira jogada.
    for (int i=0; i<3; i++)
        for (int j=0; j<3; j++) 
            if (tabuleiro[i][j]) {
                casasOcupadas++;
                casa[0] = i;
                casa[1] = j;
            }
    if (casasOcupadas == 1) {
        // Se o adversário foi o primeiro, então faz a jogada "esperta"
        if (casa[0] == 1 && casa[1] == 1) { // meio
            // Joga do canto
            jogada[0] = (rand() % 2) * 2; // 0 ou 2
            jogada[1] = (rand() % 2) * 2; // 0 ou 2
        } else {
            // Joga no meio
            jogada[0] = 1;
            jogada[1] = 1;
        }
    } else {
        // Caso geral
        // 1. Verifica se pode ganhar agora
        // 1.1 Verifica linhas
        int pecas;
        for (int i=0; i<3; i++) {
            pecas = 0;
            for (int j=0; j<3; j++) 
                if (tabuleiro[i][j] == simbolo) pecas++;
                else if (!tabuleiro[i][j]) {
                    casa[0] = i;
                    casa[1] = j;
                }
            if (pecas == 2) { // Se for ganhar, joga na casa vazia
                jogada[0] = casa[0];
                jogada[1] = casa[1];
            }
        }
        // 1.2 Verifica colunas
        for (int j=0; j<3; j++) {
            pecas = 0;
            for (int i=0; i<3; i++) {
                if (tabuleiro[i][j] == simbolo) pecas++;
                else if (!tabuleiro[i][j]) {
                    casa[0] = i;
                    casa[1] = j;
                }
            }
            if (pecas == 2) { // Se for ganhar, joga na casa vazia
                jogada[0] = casa[0];
                jogada[1] = casa[1];
            }
        }
        // 1.3 Verifica diagonais
        pecas = 0;
        for (int i=0; i<3; i++)
            if (tabuleiro[i][i] == simbolo) pecas++;
            else if (!tabuleiro[i][i]) {
                casa[0] = i;
                casa[1] = i;
            }
        if (pecas == 2) {
            jogada[0] = casa[0];
            jogada[1] = casa[0];
        }
    }

    return jogada;
}

int main() {
    srand(time(NULL)); // Seta a seed    

    char tab[3][3] = {
        '\0', '\0', '\0',
        '\0', '\0', '\0',
        '\0', '\0', '\0'
    }; // Inicia todas as casas como nulas

    return 0;
}