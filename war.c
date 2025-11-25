// território.h
#ifndef TERRITORIO_H
#define TERRITORIO_H

typedef struct Territorio {
    int id;
    char nome[50];
    int exercitos;
    struct Territorio **fronteiras; // lista de ponteiros para territórios vizinhos
    int num_fronteiras;
} Territorio;

// Cria um território
Territorio* criar_territorio(int id, const char* nome, int exercitos);

// Conecta dois territórios (bidirecional)
void conectar_territorios(Territorio* t1, Territorio* t2);

// Libera memória do território
void destruir_territorio(Territorio* t);

#endif
// territorio.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "territorio.h"

Territorio* criar_territorio(int id, const char* nome, int exercitos) {
    Territorio* t = malloc(sizeof(Territorio));
    t->id = id;
    strcpy(t->nome, nome);
    t->exercitos = exercitos;
    t->fronteiras = NULL;
    t->num_fronteiras = 0;
    return t;
}

void conectar_territorios(Territorio* t1, Territorio* t2) {
    // Expande a lista de fronteiras de t1
    t1->fronteiras = realloc(t1->fronteiras, (t1->num_fronteiras + 1) * sizeof(Territorio*));
    t1->fronteiras[t1->num_fronteiras] = t2;
    t1->num_fronteiras++;

    // Expande a lista de fronteiras de t2
    t2->fronteiras = realloc(t2->fronteiras, (t2->num_fronteiras + 1) * sizeof(Territorio*));
    t2->fronteiras[t2->num_fronteiras] = t1;
    t2->num_fronteiras++;
}

void destruir_territorio(Territorio* t) {
    free(t->fronteiras);
    free(t);
}

// batalha.h
#ifndef BATALHA_H
#define BATALHA_H

#include "territorio.h"

// Ponteiro para função de cálculo de dano
typedef int (*CalculoDanoFn)(int atacantes, int defensores);

// Funções de cálculo de dano
int dano_padrao(int atacantes, int defensores);
int dano_estrategico(int atacantes, int defensores);

// Realiza um ataque entre dois territórios
int realizar_ataque(Territorio* atacante, Territorio* defensor, CalculoDanoFn calculo_dano);

#endif

// batalha.c
#include <stdio.h>
#include <stdlib.h>
#include "batalha.h"

int dano_padrao(int atacantes, int defensores) {
    // Lógica simples: cada exército mata 1 inimigo
    return defensores > atacantes ? atacantes : defensores;
}

int dano_estrategico(int atacantes, int defensores) {
    // Exércitos de ataque são mais eficientes
    return (atacantes * 2) > defensores ? defensores : (atacantes * 2);
}

int realizar_ataque(Territorio* atacante, Territorio* defensor, CalculoDanoFn calculo_dano) {
    if (atacante->exercitos <= 1) {
        printf("Ataque falhou: atacante precisa de pelo menos 2 exercitos.\n");
        return 0;
    }

    int dano = calculo_dano(atacante->exercitos - 1, defensor->exercitos);
    defensor->exercitos -= dano;
    atacante->exercitos -= dano / 2; // Atacante também perde tropas

    printf("Ataque de %s para %s: %d exercitos perdidos.\n", 
           atacante->nome, defensor->nome, dano);

    if (defensor->exercitos <= 0) {
        defensor->exercitos = 1; // Mínimo de 1 exército para ocupação
        printf("%s foi dominado!\n", defensor->nome);
        return 1; // Conquista bem-sucedida
    }
    return 0;
}

// missao.h
#ifndef MISSAO_H
#define MISSAO_H

#include "territorio.h"

typedef struct {
    char descricao[200];
    int (*verificar_missao)(Territorio** territorios, int num_territorios);
} Missao;

// Missões disponíveis
int missao_conquistar_3_territorios(Territorio** territorios, int num_territorios);
int missao_destruir_exercito_maior(Territorio** territorios, int num_territorios);

// Verifica se alguma missão foi cumprida
int verificar_vitoria(Missao* missoes, int num_missoes, Territorio** territorios, int num_territorios);

#endif

// missao.c
#include "missao.h"

int missao_conquistar_3_territorios(Territorio** territorios, int num_territorios) {
    int conquistados = 0;
    for (int i = 0; i < num_territorios; i++) {
        if (territorios[i]->exercitos > 1) { // Considera "conquistado" se tem >1 exército
            conquistados++;
        }
    }
    return conquistados >= 3;
}

int missao_destruir_exercito_maior(Territorio** territorios, int num_territorios) {
    int maior_exercito = 0;
    for (int i = 0; i < num_territorios; i++) {
        if (territorios[i]->exercitos > maior_exercito) {
            maior_exercito = territorios[i]->exercitos;
        }
    }
    return maior_exercito <= 2; // Vitória se ninguém tem mais que 2 exércitos
}

int verificar_vitoria(Missao* missoes, int num_missoes, Territorio** territorios, int num_territorios) {
    for (int i = 0; i < num_missoes; i++) {
        if (missoes[i].verificar_missao(territorios, num_territorios)) {
            printf("Missao cumprida: %s\n", missoes[i].descricao);
            return 1;
        }
    }
    return 0;
}// main.c
#include <stdio.h>
#include <stdlib.h>
#include "territorio.h"
#include "batalha.h"
#include "missao.h"

int main() {
    // Cria territórios
    Territorio* t1 = criar_territorio(1, "America", 5);
    Territorio* t2 = criar_territorio(2, "Europa", 3);
    Territorio* t3 = criar_territorio(3, "Asia", 4);
    Territorio* t4 = criar_territorio(4, "Africa", 2);

    // Conecta territórios
    conectar_territorios(t1, t2);
    conectar_territorios(t2, t3);
    conectar_territorios(t3, t4);

    // Lista de territórios
    Territorio* territorios[] = {t1, t2, t3, t4};
    int num_territorios = 4;

    // Define missões
    Missao missoes[2] = {
        {"Conquistar 3 territorios", missao_conquistar_3_territorios},
        {"Destruir o maior exercito", missao_destruir_exercito_maior}
    };

    // Loop do jogo
    int turno = 0;
    while (!verificar_vitoria(missoes, 2, territorios, num_territorios)) {
        printf("\n=== Turno %d ===\n", ++turno);
        
        // Simula ataques
        realizar_ataque(t1, t2, dano_padrao);
        realizar_ataque(t3, t2, dano_estrategico);

        // Verifica se alguém venceu
        if (verificar_vitoria(missoes, 2, territorios, num_territorios)) {
            printf("\n*** Fim de Jogo: Vitoria apos %d turnos! ***\n", turno);
            break;
        }

        // Limite de turnos para evitar loop infinito
        if (turno >= 10) {
            printf("\n*** Fim de Jogo: Limite de turnos atingido! ***\n");
            break;
        }
    }

    // Libera memória
    for (int i = 0; i < num_territorios; i++) {
        destruir_territorio(territorios[i]);
    }

    return 0;
}

# Makefile
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
OBJS = main.o territorio.o batalha.o missao.o
TARGET = war_game

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c territorio.h batalha.h missao.h
	$(CC) $(CFLAGS) -c main.c

territorio.o: territorio.c territorio.h
	$(CC) $(CFLAGS) -c territorio.c

batalha.o: batalha.c batalha.h territorio.h
	$(CC) $(CFLAGS) -c batalha.c

missao.o: missao.c missao.h territorio.h
	$(CC) $(CFLAGS) -c missao.c

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean
