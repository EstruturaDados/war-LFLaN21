// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "missao.h"

#define TAMANHO_MAPA 8
#define MAX_TURNOS 20

int main() {
    // Inicialização do gerador de números aleatórios
    srand(time(NULL));
    
    // Variáveis para missões
    char* missoes[5];
    int totalMissoes;
    char* missaoJogador1 = NULL;
    char* missaoJogador2 = NULL;
    
    // Inicializa o vetor de missões
    inicializarMissoes(missoes, &totalMissoes);
    
    // Atribui missões aos jogadores
    atribuirMissao(&missaoJogador1, missoes, totalMissoes);
    atribuirMissao(&missaoJogador2, missoes, totalMissoes);
    
    // Cria e inicializa o mapa
    Territorio* mapa = (Territorio*)calloc(TAMANHO_MAPA, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memoria para o mapa!\n");
        return 1;
    }
    
    // Inicializa territórios
    strcpy(mapa[0].nome, "America"); strcpy(mapa[0].cor, "azul"); mapa[0].tropas = 5;
    strcpy(mapa[1].nome, "Europa"); strcpy(mapa[1].cor, "vermelho"); mapa[1].tropas = 3;
    strcpy(mapa[2].nome, "Asia"); strcpy(mapa[2].cor, "azul"); mapa[2].tropas = 4;
    strcpy(mapa[3].nome, "Africa"); strcpy(mapa[3].cor, "vermelho"); mapa[3].tropas = 2;
    strcpy(mapa[4].nome, "Oceania"); strcpy(mapa[4].cor, "azul"); mapa[4].tropas = 3;
    strcpy(mapa[5].nome, "Antartida"); strcpy(mapa[5].cor, "vermelho"); mapa[5].tropas = 1;
    strcpy(mapa[6].nome, "Amazonia"); strcpy(mapa[6].cor, "azul"); mapa[6].tropas = 2;
    strcpy(mapa[7].nome, "Siberia"); strcpy(mapa[7].cor, "vermelho"); mapa[7].tropas = 4;
    
    // Exibe missões no início do jogo
    exibirMissao(missaoJogador1, "JOGADOR AZUL");
    exibirMissao(missaoJogador2, "JOGADOR VERMELHO");
    
    printf("=== INICIO DO JOGO WAR ===\n");
    exibirMapa(mapa, TAMANHO_MAPA);
    
    // Loop principal do jogo
    int turno;
    for (turno = 1; turno <= MAX_TURNOS; turno++) {
        printf("\n*** TURNO %d ***\n", turno);
        
        // Simula ataques (em um jogo real, isso seria controlado pelos jogadores)
        atacar(&mapa[0], &mapa[1]);  // América ataca Europa
        atacar(&mapa[3], &mapa[2]);  // África ataca Ásia
        
        exibirMapa(mapa, TAMANHO_MAPA);
        
        // Verifica silenciosamente se alguma missão foi cumprida
        if (verificarMissao(missaoJogador1, mapa, TAMANHO_MAPA, "azul")) {
            printf("\n*** VITORIA DO JOGADOR AZUL! ***\n");
            printf("Missao cumprida: %s\n", missaoJogador1);
            break;
        }
        
        if (verificarMissao(missaoJogador2, mapa, TAMANHO_MAPA, "vermelho")) {
            printf("\n*** VITORIA DO JOGADOR VERMELHO! ***\n");
            printf("Missao cumprida: %s\n", missaoJogador2);
            break;
        }
        
        // Reinicia algumas tropas para continuidade do jogo (simulação)
        if (turno % 3 == 0) {
            for (int i = 0; i < TAMANHO_MAPA; i++) {
                mapa[i].tropas += 1;
            }
            printf("\nReforcos chegando para todos os territorios!\n");
        }
    }
    
    // Fim de jogo
    if (turno > MAX_TURNOS) {
        printf("\n*** FIM DE JOGO - LIMITE DE TURNOS ATINGIDO ***\n");
        
        // Verifica quem tem mais territórios
        int territoriosAzul = 0, territoriosVermelho = 0;
        for (int i = 0; i < TAMANHO_MAPA; i++) {
            if (strcmp(mapa[i].cor, "azul") == 0) territoriosAzul++;
            else territoriosVermelho++;
        }
        
        if (territoriosAzul > territoriosVermelho) {
            printf("Jogador Azul vence por ter mais territorios!\n");
        } else if (territoriosVermelho > territoriosAzul) {
            printf("Jogador Vermelho vence por ter mais territorios!\n");
        } else {
            printf("EMPATE!\n");
        }
    }
    
    // Libera memória alocada
    liberarMemoria(mapa, TAMANHO_MAPA, missaoJogador1, missaoJogador2);
    
    printf("\nJogo finalizado. Memoria liberada.\n");
    return 0;
}
