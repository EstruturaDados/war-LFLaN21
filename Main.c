#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_MISSOES 5
#define MAX_TERRITORIOS 10
#define MAX_NOME 30
#define MAX_COR 10
#define MAX_MISSAO_DESC 100

// Estrutura para representar um território
typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
} Territorio;

// Protótipos das funções
void inicializarMapa(Territorio** mapa, int tamanho);
void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
char* alocarMissaoJogador(char* missoes[], int totalMissoes);
int verificarMissao(char* missao, Territorio* mapa, int tamanho, char corJogador[]);
void exibirMissao(char* missao, char corJogador[]);
void atacar(Territorio* atacante, Territorio* defensor);
void exibirMapa(Territorio* mapa, int tamanho);
void liberarMemoria(Territorio* mapa, char* missaoJogador1, char* missaoJogador2, int jogadores);
int validarAtaque(Territorio* atacante, Territorio* defensor);
void simularTurno(Territorio* mapa, int tamanho, char* missaoJogador1, char* missaoJogador2);

// Vetor global de missões disponíveis
char* missoesDisponiveis[MAX_MISSOES] = {
    "Conquistar 3 territorios seguidos",
    "Eliminar todas as tropas da cor vermelha",
    "Controlar 5 territorios simultaneamente",
    "Destruir 10 tropas inimigas",
    "Manter 2 territorios por 3 turnos consecutivos"
};

int main() {
    srand(time(NULL)); // Inicializar semente para números aleatórios
    
    Territorio* mapa = NULL;
    char* missaoJogador1 = NULL;
    char* missaoJogador2 = NULL;
    
    int territorios = 6; // Número de territórios no mapa
    int jogadores = 2;   // Número de jogadores
    
    printf("=== SISTEMA DE MISSÕES ESTRATÉGICAS ===\n\n");
    
    // 1. Inicializar mapa
    inicializarMapa(&mapa, territorios);
    printf("Mapa inicializado com %d territórios.\n", territorios);
    
    // 2. Atribuir missões aos jogadores
    missaoJogador1 = alocarMissaoJogador(missoesDisponiveis, MAX_MISSOES);
    missaoJogador2 = alocarMissaoJogador(missoesDisponiveis, MAX_MISSOES);
    
    printf("\n=== MISSÕES ATRIBUÍDAS ===\n");
    exibirMissao(missaoJogador1, "Azul");
    exibirMissao(missaoJogador2, "Vermelho");
    
    // 3. Simular partida
    printf("\n=== INICIANDO PARTIDA ===\n");
    int turno = 1;
    int vencedor = 0;
    
    while (vencedor == 0 && turno <= 10) { // Máximo de 10 turnos
        printf("\n--- TURNO %d ---\n", turno);
        
        // Exibir estado atual do mapa
        exibirMapa(mapa, territorios);
        
        // Simular ataques
        simularTurno(mapa, territorios, missaoJogador1, missaoJogador2);
        
        // Verificar missões
        int missao1Cumprida = verificarMissao(missaoJogador1, mapa, territorios, "Azul");
        int missao2Cumprida = verificarMissao(missaoJogador2, mapa, territorios, "Vermelho");
        
        if (missao1Cumprida) {
            printf("\n🎉 JOGADOR AZUL CUMPRIU SUA MISSÃO: %s\n", missaoJogador1);
            printf("🏆 VITÓRIA DO JOGADOR AZUL!\n");
            vencedor = 1;
        } else if (missao2Cumprida) {
            printf("\n🎉 JOGADOR VERMELHO CUMPRIU SUA MISSÃO: %s\n", missaoJogador2);
            printf("🏆 VITÓRIA DO JOGADOR VERMELHO!\n");
            vencedor = 2;
        }
        
        turno++;
    }
    
    if (vencedor == 0) {
        printf("\n⏰ Partida terminou em empate após 10 turnos!\n");
    }
    
    // 4. Liberar memória
    liberarMemoria(mapa, missaoJogador1, missaoJogador2, jogadores);
    
    return 0;
}

/**
 * Inicializa o mapa com territórios aleatórios
 * @param mapa Ponteiro para o vetor de territórios (passagem por referência)
 * @param tamanho Número de territórios a criar
 */
void inicializarMapa(Territorio** mapa, int tamanho) {
    // Alocar memória para o mapa
    *mapa = (Territorio*)malloc(tamanho * sizeof(Territorio));
    if (*mapa == NULL) {
        printf("Erro ao alocar memória para o mapa!\n");
        exit(1);
    }
    
    char* nomes[] = {"Floresta", "Montanha", "Planície", "Deserto", "Pantano", 
                     "Colina", "Vale", "Costa", "Ilha", "Planalto"};
    char* cores[] = {"Azul", "Vermelho", "Neutro"};
    
    for (int i = 0; i < tamanho; i++) {
        strcpy((*mapa)[i].nome, nomes[i % 10]);
        
        // Distribuir cores: 2 Azul, 2 Vermelho, 2 Neutro (para 6 territórios)
        if (i < 2) strcpy((*mapa)[i].cor, "Azul");
        else if (i < 4) strcpy((*mapa)[i].cor, "Vermelho");
        else strcpy((*mapa)[i].cor, "Neutro");
        
        (*mapa)[i].tropas = (rand() % 5) + 1; // 1-5 tropas
    }
}

/**
 * Atribui uma missão sorteada ao jogador (passagem por referência)
 * @param destino Ponteiro onde a missão será copiada
 * @param missoes Vetor de strings com missões disponíveis
 * @param totalMissoes Número total de missões disponíveis
 */
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int indiceSorteado = rand() % totalMissoes;
    strcpy(destino, missoes[indiceSorteado]);
}

/**
 * Aloca e atribui uma missão para um jogador
 * @param missoes Vetor de missões disponíveis
 * @param totalMissoes Total de missões
 * @return Ponteiro para a missão alocada
 */
char* alocarMissaoJogador(char* missoes[], int totalMissoes) {
    char* missao = (char*)malloc(MAX_MISSAO_DESC * sizeof(char));
    if (missao == NULL) {
        printf("Erro ao alocar memória para missão!\n");
        exit(1);
    }
    
    atribuirMissao(missao, missoes, totalMissoes);
    return missao;
}

/**
 * Verifica se a missão do jogador foi cumprida (passagem por valor para missão)
 * @param missao Missão a ser verificada
 * @param mapa Vetor de territórios
 * @param tamanho Tamanho do mapa
 * @param corJogador Cor do jogador
 * @return 1 se missão cumprida, 0 caso contrário
 */
int verificarMissao(char* missao, Territorio* mapa, int tamanho, char corJogador[]) {
    // Lógica simples de verificação baseada no texto da missão
    
    if (strstr(missao, "Conquistar 3 territorios seguidos") != NULL) {
        int territoriosConquistados = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) {
                territoriosConquistados++;
            }
        }
        return territoriosConquistados >= 3;
    }
    else if (strstr(missao, "Eliminar todas as tropas da cor vermelha") != NULL) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "Vermelho") == 0 && mapa[i].tropas > 0) {
                return 0;
            }
        }
        return 1;
    }
    else if (strstr(missao, "Controlar 5 territorios simultaneamente") != NULL) {
        int territoriosControlados = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) {
                territoriosControlados++;
            }
        }
        return territoriosControlados >= 5;
    }
    else if (strstr(missao, "Destruir 10 tropas inimigas") != NULL) {
        // Simulação: considerar que destruiu 10 tropas se controla muitos territórios
        int tropasDestruidas = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) != 0) {
                tropasDestruidas += (5 - mapa[i].tropas); // Tropas perdidas
            }
        }
        return tropasDestruidas >= 10;
    }
    else if (strstr(missao, "Manter 2 territorios por 3 turnos consecutivos") != NULL) {
        // Simulação: considerar cumprida se controla pelo menos 2 territórios
        int territoriosControlados = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) {
                territoriosControlados++;
            }
        }
        return territoriosControlados >= 2;
    }
    
    return 0;
}

/**
 * Exibe a missão do jogador (passagem por valor)
 * @param missao Missão a ser exibida
 * @param corJogador Cor do jogador
 */
void exibirMissao(char* missao, char corJogador[]) {
    printf("Jogador %s: %s\n", corJogador, missao);
}

/**
 * Simula um ataque entre dois territórios (passagem por referência)
 * @param atacante Território atacante
 * @param defensor Território defensor
 */
void atacar(Territorio* atacante, Territorio* defensor) {
    if (!validarAtaque(atacante, defensor)) {
        return;
    }
    
    printf("  ⚔️  %s (%s) ataca %s (%s)\n", 
           atacante->nome, atacante->cor, defensor->nome, defensor->cor);
    
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;
    
    printf("    Dados: Atacante %d vs Defensor %d\n", dadoAtacante, dadoDefensor);
    
    if (dadoAtacante > dadoDefensor) {
        // Atacante vence
        printf("    ✅ VITÓRIA! %s conquista %s\n", atacante->cor, defensor->nome);
        
        // Transferir cor e metade das tropas
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = (defensor->tropas / 2) + 1;
        atacante->tropas--; // Atacante perde uma tropa
        
    } else {
        // Defensor vence
        printf("    ❌ DERROTA! %s defende %s\n", defensor->cor, defensor->nome);
        atacante->tropas--; // Atacante perde uma tropa
    }
    
    printf("    Tropas atualizadas: %s (%d), %s (%d)\n",
           atacante->nome, atacante->tropas, defensor->nome, defensor->tropas);
}

/**
 * Valida se um ataque é possível
 * @param atacante Território atacante
 * @param defensor Território defensor
 * @return 1 se ataque válido, 0 caso contrário
 */
int validarAtaque(Territorio* atacante, Territorio* defensor) {
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        return 0; // Não pode atacar próprio território
    }
    
    if (atacante->tropas <= 1) {
        return 0; // Precisa de pelo menos 2 tropas para atacar
    }
    
    return 1;
}

/**
 * Exibe o estado atual do mapa
 * @param mapa Vetor de territórios
 * @param tamanho Tamanho do mapa
 */
void exibirMapa(Territorio* mapa, int tamanho) {
    printf("\n📊 ESTADO DO MAPA:\n");
    for (int i = 0; i < tamanho; i++) {
        printf("  %s: %d tropas (%s)\n", 
               mapa[i].nome, mapa[i].tropas, mapa[i].cor);
    }
}

/**
 * Simula um turno completo com ataques aleatórios
 * @param mapa Mapa do jogo
 * @param tamanho Tamanho do mapa
 * @param missaoJogador1 Missão do jogador 1
 * @param missaoJogador2 Missão do jogador 2
 */
void simularTurno(Territorio* mapa, int tamanho, char* missaoJogador1, char* missaoJogador2) {
    printf("🎲 Simulando ataques...\n");
    
    // Realizar 2-3 ataques aleatórios por turno
    int ataques = (rand() % 2) + 2;
    
    for (int i = 0; i < ataques; i++) {
        int atacanteIdx, defensorIdx;
        
        // Encontrar território atacante válido
        do {
            atacanteIdx = rand() % tamanho;
        } while (mapa[atacanteIdx].tropas <= 1);
        
        // Encontrar território defensor válido
        do {
            defensorIdx = rand() % tamanho;
        } while (!validarAtaque(&mapa[atacanteIdx], &mapa[defensorIdx]));
        
        atacar(&mapa[atacanteIdx], &mapa[defensorIdx]);
    }
}

/**
 * Libera toda a memória alocada dinamicamente
 * @param mapa Ponteiro para o mapa
 * @param missaoJogador1 Missão do jogador 1
 * @param missaoJogador2 Missão do jogador 2
 * @param jogadores Número de jogadores
 */
void liberarMemoria(Territorio* mapa, char* missaoJogador1, char* missaoJogador2, int jogadores) {
    printf("\n🧹 Liberando memória...\n");
    
    if (mapa != NULL) {
        free(mapa);
        printf("  Mapa liberado\n");
    }
    
    if (missaoJogador1 != NULL) {
        free(missaoJogador1);
        printf("  Missão jogador 1 liberada\n");
    }
    
    if (missaoJogador2 != NULL) {
        free(missaoJogador2);
        printf("  Missão jogador 2 liberada\n");
    }
    
    printf("✅ Memória liberada com sucesso!\n");
}
