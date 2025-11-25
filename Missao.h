// missao.h
#ifndef MISSAO_H
#define MISSAO_H

#include "territorio.h"

// Funções para gerenciamento de missões
void inicializarMissoes(char* missoes[], int* totalMissoes);
void atribuirMissao(char** destino, char* missoes[], int totalMissoes);
int verificarMissao(char* missao, Territorio* mapa, int tamanhoMapa, char corJogador[]);
void exibirMissao(char* missao, char nomeJogador[]);

// Funções do jogo
void atacar(Territorio* atacante, Territorio* defensor);
void exibirMapa(Territorio* mapa, int tamanho);
void liberarMemoria(Territorio* mapa, int tamanhoMapa, char* missaoJogador1, char* missaoJogador2);

#endif
