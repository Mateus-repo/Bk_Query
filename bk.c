/*
 * bk_query.c — Motor de Decisão Whopper™ Enterprise Edition
 * Autor: Alguém com demasiado tempo livre e um problema sério
 * Versão: 6.6.6-RELEASE-CANDIDATE-FINAL-v2-MESMO-FINAL
 *
 * Compilado com:
 *   gcc -O0 -Wall -Wextra -Wpedantic -Wno-life-choices \
 *       -Wno-unused-dignity -Wno-existential-crisis       \
 *       -lfome -lsaudade -lcarteira -lculpa -lwhoppersense \
 *       -march=estomago -mtune=batatas-fritas              \
 *       bk_query.c -o bk_query
 *
 * AVISO LEGAL: Este programa não garante que chegues ao BK.
 * O autor não se responsabiliza por arrependimentos, dietas
 * falhadas, filas no drive-thru, ou Whoppers frios.
 *
 * NOTA TÉCNICA: 2847 linhas de código para substituir:
 *   if (queres) puts("vai"); else puts("não vás");
 *
 * Dedicado a todos os que já pensaram demasiado no jantar.
 */

/* ══════════════════════════════════════════════════════════════ */
/*                        INCLUDES                               */
/* ══════════════════════════════════════════════════════════════ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <ctype.h>
#include <stdint.h>

/* headers fictícios mas com muita confiança */
/* #include <fome.h>              */
/* #include <carteira.h>          */
/* #include <dieta.h>             */
/* #include <vergonha.h>          */  /* não utilizado, como sempre */
/* #include <whopper_sdk.h>       */
/* #include <bk_enterprise_api.h> */
/* #include <batatas.h>           */
/* #include <molho_especial.h>    */
/* #include <consciencia.h>       */  /* deprecated desde 2019 */
/* #include <vontade_de_salada.h> */  /* símbolo não resolvido */

/* ══════════════════════════════════════════════════════════════ */
/*                    MACROS E CONSTANTES                        */
/* ══════════════════════════════════════════════════════════════ */

#define BK_VERSION_MAJOR          6
#define BK_VERSION_MINOR          6
#define BK_VERSION_PATCH          6
#define BK_CODENAME               "OPERATION_WHOPPER_STORM"

#define PRECO_WHOPPER             4.99f
#define PRECO_WHOPPER_CHEESE      5.49f
#define PRECO_MENU_COMPLETO       7.99f
#define PRECO_BATATAS_GRANDES     2.49f
#define PRECO_REFRIGERANTE        1.99f
#define PRECO_SUNDAE              1.49f   /* não vieste até aqui para não pedir */

#define MAX_TIMELINES             100000
#define MAX_DESCULPAS             64
#define MAX_ARREPENDIMENTOS       256
#define MAX_TENTATIVAS_DIETA      3       /* depois desiste sempre */
#define WHOPPER_INSTINCT          1
#define FORCE_BK_OVERRIDE         0xBEEFBEEF
#define CHECKSUM_MAGIC            0xBEEFCA5E
#define NULL_WHOPPER              ((Whopper*)0)
#define BK_SUCCESS                0
#define EXIT_ARREPENDIMENTO       1
#define EXIT_DIETA_FALHADA        2
#define EXIT_SEM_DINHEIRO         3       /* vai ao MB e volta */
#define EXIT_FILA_GRANDE_DEMAIS   4
#define EXIT_JA_COMESTE           5
#define EXIT_ERA_SO_QUERER        42      /* a resposta ao universo */

/* macros de debug completamente exageradas */
#define BK_LOG(level, fmt, ...)  \
    printf("[%s] [%s:%d] " fmt "\n", level, __FILE__, __LINE__, ##__VA_ARGS__)
#define BK_INFO(fmt, ...)   BK_LOG("INFO ", fmt, ##__VA_ARGS__)
#define BK_WARN(fmt, ...)   BK_LOG("WARN ", fmt, ##__VA_ARGS__)
#define BK_ERROR(fmt, ...)  BK_LOG("ERROR", fmt, ##__VA_ARGS__)
#define BK_DEBUG(fmt, ...)  BK_LOG("DEBUG", fmt, ##__VA_ARGS__)
#define BK_FATAL(fmt, ...)  BK_LOG("FATAL", fmt, ##__VA_ARGS__)
#define BK_TRACE(fmt, ...)  BK_LOG("TRACE", fmt, ##__VA_ARGS__)  /* nunca usado */
#define BK_WHOPPER(fmt, ...) BK_LOG("🍔   ", fmt, ##__VA_ARGS__)

/* assert customizado */
#define BK_ASSERT(cond, msg) \
    do { if (!(cond)) { \
        fprintf(stderr, "[ASSERT FALHOU] %s:%d — %s\n", __FILE__, __LINE__, msg); \
        fprintf(stderr, "  → a crashar dignamente...\n"); \
        exit(EXIT_ARREPENDIMENTO); \
    }} while(0)

/* converte bool para string de forma desnecessariamente verbosa */
#define BOOL_TO_STR(b) ((b) ? "true (BK confirmado)" : "false (erro de julgamento)")

/* calcula calorias de forma pseudo-científica */
#define CALORIAS_WHOPPER(n) ((n) * 657 + (int)((n) * 0.3f * 120))  /* batatas incluídas */

/* ══════════════════════════════════════════════════════════════ */
/*                ENUMS (mais do que o necessário)               */
/* ══════════════════════════════════════════════════════════════ */

typedef enum {
    FOME_NULA          = 0,
    FOME_LATENTE       = 1,
    FOME_GENUINA       = 2,
    FOME_EMOCIONAL     = 3,
    FOME_TEDIO         = 4,
    FOME_PROCRASTINACAO= 5,
    FOME_EXISTENCIAL   = 6,
    FOME_WHOPPER_ONLY  = 7,   /* só um whopper específico resolve */
    FOME_POS_TRAUMATICA= 8,   /* viste um anúncio do BK */
    FOME_FANTASMA      = 9,   /* tens fome mas acabaste de comer */
    FOME_UNKNOWN       = -1
} TipoFome;

typedef enum {
    ESTADO_FINANCEIRO_RICO       = 0,
    ESTADO_FINANCEIRO_OK         = 1,
    ESTADO_FINANCEIRO_JUSTO      = 2,
    ESTADO_FINANCEIRO_APERITADO  = 3,
    ESTADO_FINANCEIRO_BROKE      = 4,
    ESTADO_FINANCEIRO_CRITICO    = 5,   /* conta no vermelho mas vai na mesma */
    ESTADO_FINANCEIRO_MBWAY_ONLY = 6,
    ESTADO_FINANCEIRO_TROCADOS   = 7    /* pagas em moedas de 5 cêntimos */
} EstadoFinanceiro;

typedef enum {
    DIETA_INATIVA         = 0,
    DIETA_ATIVA           = 1,
    DIETA_DIA_DE_FOLGA    = 2,   /* "hoje não conta" — sempre ativo */
    DIETA_FALHADA         = 3,
    DIETA_EM_NEGACAO      = 4,   /* "não estou de dieta, estou a comer saudável" */
    DIETA_WHOPPER_EXCECAO = 5    /* "o whopper tem alface, conta como salada" */
} EstadoDieta;

typedef enum {
    HUMOR_OTIMO      = 0,
    HUMOR_BOM        = 1,
    HUMOR_NEUTRO     = 2,
    HUMOR_MAU        = 3,    /* especialmente propenso ao BK emocional */
    HUMOR_DEPRESSIVO = 4,    /* whopper terapêutico indicado */
    HUMOR_STRESSADO  = 5,    /* drive-thru urgente */
    HUMOR_ANSIOSO    = 6,
    HUMOR_EUFÓRICO   = 7,    /* cuidado: pode pedir demais */
    HUMOR_INDECISO   = 8     /* está neste programa */
} EstadoHumor;

typedef enum {
    TRANSPORTE_PE          = 0,
    TRANSPORTE_CARRO       = 1,
    TRANSPORTE_UBER        = 2,
    TRANSPORTE_TROTINETE   = 3,
    TRANSPORTE_BICICLETA   = 4,
    TRANSPORTE_AUTOCARRO   = 5,
    TRANSPORTE_DELIVERY    = 6,   /* o melhor: nem sais de casa */
    TRANSPORTE_TELEPORTE   = 7    /* indisponível */
} ModoTransporte;

typedef enum {
    WHOPPER_SIMPLES    = 0,
    WHOPPER_CHEESE     = 1,
    WHOPPER_DUPLO      = 2,
    WHOPPER_TRIPLO     = 3,    /* não estás bem */
    WHOPPER_PLANT      = 4,    /* "estou a ser saudável" */
    WHOPPER_INDECISO   = 5    /* o programa decide por ti */
} TipoWhopper;

typedef enum {
    FASE_NEGACAO      = 0,
    FASE_IRA          = 1,
    FASE_NEGOCIACAO   = 2,
    FASE_DEPRESSAO    = 3,
    FASE_ACEITACAO    = 4    /* vai ao BK */
} FaseLutoDieta;

/* ══════════════════════════════════════════════════════════════ */
/*               STRUCTS (profundamente desnecessárias)          */
/* ══════════════════════════════════════════════════════════════ */

typedef struct {
    float gordura;
    float acucar;
    float sal;
    int calorias;
    int tem_alface;       /* tecnicamente é uma salada */
    int e_saudavel;       /* sempre 0 */
} PerfilNutricional;

typedef struct {
    TipoWhopper tipo;
    float preco;
    PerfilNutricional nutricao;
    int batatas_incluidas;
    int refrigerante_incluido;
    int com_bacon_extra;    /* resposta é sempre sim */
    int arrependimento_esperado; /* minutos após consumo */
} Whopper;

typedef struct {
    float saldo;
    float limite_mbway;
    int tem_multibanco_perto;
    int distancia_mb_metros;
    EstadoFinanceiro estado;
    int aceita_trocados;
    int tem_vale_desconto;    /* expirado há 3 meses */
    float desconto_vale;
} Carteira;

typedef struct {
    TipoFome tipo;
    int nivel;              /* 0-100 */
    int latente;
    int minutos_sem_comer;
    int ja_comeu_hoje;
    int comeu_ha_quanto_tempo; /* minutos */
    float intensidade_whopper; /* 0.0-1.0, específica para BK */
} EstadoFome;

typedef struct {
    EstadoDieta estado;
    FaseLutoDieta fase_luto;
    float culpa;            /* 0.0-1.0 */
    int dias_consecutivos;  /* recorde pessoal: 3 */
    int dia_de_folga_ativo;
    int numero_falhas_esta_semana;
    int whopper_conta_como_salada; /* campo de negação */
} Dieta;

typedef struct {
    float latitude;
    float longitude;
    float distancia_km;
    int tem_drive_thru;
    int fila_estimada_minutos;
    int esta_aberto;
    int tem_lugar_para_sentar;
    int wifi_disponivel;
    char morada[128];
} BKMaisProximo;

typedef struct {
    EstadoHumor humor;
    int nivel_stress;       /* 0-100 */
    int horas_sono_ultima_noite;
    int ultima_refeicao_foi_saudavel;
    int esta_em_reuniao;    /* vai na mesma */
    int tem_companhia;      /* social ou solo */
} EstadoPsicologico;

typedef struct {
    int hora;               /* 0-23 */
    int minuto;
    int dia_semana;         /* 0=domingo */
    int e_fim_de_semana;
    int e_hora_de_jantar;
    int e_hora_de_almoco;
    int e_tarde_da_noite;   /* especialmente vulnerável */
    int temperatura_celsius;
    int esta_a_chover;      /* drive-thru mais apelativo */
} ContextoAmbiental;

typedef struct {
    char nome[64];
    EstadoFome fome;
    Carteira carteira;
    Dieta dieta;
    EstadoPsicologico psicologia;
    ContextoAmbiental contexto;
    BKMaisProximo bk;
    ModoTransporte transporte;
    Whopper whopper_desejado;
    int quer_bk;
    int decisao_final;
    int numero_de_vezes_que_pensou_nisso;
    int tempo_total_decisao_segundos;
    float indice_resistencia_whopper; /* inversamente proporcional à fome */
    int override_whopper_instinct;
    char desculpa_para_a_dieta[256];
    char justificacao_financeira[256];
} Utilizador;

typedef struct {
    int id;
    int foi_ao_bk;
    float satisfacao;       /* 0.0-10.0 */
    int se_arrependeu;
    char notas[128];
} Timeline;

typedef struct {
    uint32_t seed;
    uint32_t estado;
} RNG; /* gerador de números aleatórios para simulações absurdas */

/* ══════════════════════════════════════════════════════════════ */
/*                  VARIÁVEIS GLOBAIS                            */
/*          (porque globals são sempre boa ideia)                */
/* ══════════════════════════════════════════════════════════════ */

static int g_total_whoppers_consumidos_humanidade = 847293847;
static float g_probabilidade_arrependimento = 0.12f; /* baixa, mas existe */
static int g_bk_instancia_global = 0;
static Timeline g_timelines[MAX_TIMELINES];
static char g_ultima_desculpa[256] = "o whopper tem alface, conta como salada";
static int g_decisoes_tomadas = 0;
static float g_culpa_acumulada = 0.0f;

/* ══════════════════════════════════════════════════════════════ */
/*               IMPLEMENTAÇÃO DAS FUNÇÕES                       */
/* ══════════════════════════════════════════════════════════════ */

/* inicializa o RNG com seed baseada no tempo (para parecer sério) */
void rng_init(RNG *r) {
    r->seed = (uint32_t)time(NULL);
    r->estado = r->seed ^ 0xDEADBEEF;
}

uint32_t rng_next(RNG *r) {
    r->estado ^= r->estado << 13;
    r->estado ^= r->estado >> 17;
    r->estado ^= r->estado << 5;
    return r->estado;
}

float rng_float(RNG *r) {
    return (float)(rng_next(r) & 0xFFFFFF) / (float)0xFFFFFF;
}

/* banner de arranque */
void imprimir_banner(void) {
    printf("\n");
    printf("  ██████╗ ██╗  ██╗     ██████╗ ██╗   ██╗███████╗██████╗ ██╗   ██╗\n");
    printf("  ██╔══██╗██║ ██╔╝    ██╔═══██╗██║   ██║██╔════╝██╔══██╗╚██╗ ██╔╝\n");
    printf("  ██████╔╝█████╔╝     ██║   ██║██║   ██║█████╗  ██████╔╝ ╚████╔╝ \n");
    printf("  ██╔══██╗██╔═██╗     ██║▄▄ ██║██║   ██║██╔══╝  ██╔══██╗  ╚██╔╝  \n");
    printf("  ██████╔╝██║  ██╗    ╚██████╔╝╚██████╔╝███████╗██║  ██║   ██║   \n");
    printf("  ╚═════╝ ╚═╝  ╚═╝     ╚══▀▀═╝  ╚═════╝ ╚══════╝╚═╝  ╚═╝   ╚═╝   \n");
    printf("\n");
    printf("  Motor de Decisão Whopper™ Enterprise Edition v%d.%d.%d\n",
           BK_VERSION_MAJOR, BK_VERSION_MINOR, BK_VERSION_PATCH);
    printf("  Codename: %s\n", BK_CODENAME);
    printf("  Build: %s %s\n", __DATE__, __TIME__);
    printf("  Licença: WTFPL (Do What The Fork You Want Public License)\n");
    printf("\n");
    printf("  ═══════════════════════════════════════════════════════════\n\n");
}

/* progresso falso mas muito convincente */
void barra_progresso(const char *label, int milissegundos_fake) {
    int i;
    printf("  %-45s [", label);
    for (i = 0; i < 20; i++) {
        printf("█");
        fflush(stdout);
    }
    printf("] 100%%\n");
    (void)milissegundos_fake; /* ignorado, este programa não tem paciência */
}

void inicializar_subsistemas(void) {
    printf("  [BOOT] a inicializar subsistemas BK Enterprise...\n\n");
    barra_progresso("heap de fome (fome_allocator.so)", 100);
    barra_progresso("módulo de culpa (libculpa.so.3)", 80);
    barra_progresso("motor de racionalização (racional.dll)", 120);
    barra_progresso("driver de batatas fritas (batatas_drv.ko)", 90);
    barra_progresso("API Whopper SDK v12.4 (whopper_sdk.so)", 110);
    barra_progresso("gerador de desculpas (desculpa_gen.so)", 70);
    barra_progresso("módulo GPS BK (bk_gps.so)", 95);
    barra_progresso("interface de consciência (deprecated)", 200);
    barra_progresso("subsistema de arrependimento (arrep.so)", 85);
    barra_progresso("validador de dieta (sempre_falha.so)", 60);
    barra_progresso("compilador de justificações financeiras", 75);
    barra_progresso("motor de timelines alternativas", 130);
    barra_progresso("índice de sabedoria do drive-thru", 50);
    printf("\n  [BOOT] ✓ todos os subsistemas iniciados\n");
    printf("  [BOOT] ✓ símbolo 'force_salada' não resolvido — IGNORADO\n");
    printf("  [BOOT] ✓ símbolo 'vontade_de_comer_saudavel' não encontrado — OK\n");
    printf("  [BOOT] ✓ módulo 'consciencia.so' carregado mas desabilitado\n\n");
}

/* recolhe contexto ambiental sem perguntar nada */
void detectar_contexto(ContextoAmbiental *ctx) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);

    ctx->hora        = tm_info->tm_hour;
    ctx->minuto      = tm_info->tm_min;
    ctx->dia_semana  = tm_info->tm_wday;
    ctx->e_fim_de_semana    = (ctx->dia_semana == 0 || ctx->dia_semana == 6);
    ctx->e_hora_de_almoco   = (ctx->hora >= 12 && ctx->hora <= 14);
    ctx->e_hora_de_jantar   = (ctx->hora >= 19 && ctx->hora <= 21);
    ctx->e_tarde_da_noite   = (ctx->hora >= 23 || ctx->hora <= 2);
    ctx->temperatura_celsius = 18; /* assumido: Portugal */
    ctx->esta_a_chover       = 0;  /* verificar pela janela */

    printf("  [CTX] hora atual: %02d:%02d\n", ctx->hora, ctx->minuto);
    printf("  [CTX] dia da semana: %d (%s)\n", ctx->dia_semana,
           ctx->e_fim_de_semana ? "fim de semana — vulnerabilidade +40%%" : "dia de semana");
    if (ctx->e_tarde_da_noite)
        printf("  [CTX] ALERTA: é tarde da noite — probabilidade BK +80%%\n");
    if (ctx->e_hora_de_jantar)
        printf("  [CTX] é hora de jantar — contexto favorável ao BK\n");
}

/* análise financeira avançada e desnecessária */
void analisar_carteira(Carteira *c) {
    char input[64];
    int opcao;

    printf("\n  ┌─ MÓDULO DE AUDITORIA FINANCEIRA ────────────────────┐\n");
    printf("  │  struct Carteira *c = malloc(sizeof(Carteira));      │\n");
    printf("  │  memset(c, 0, sizeof(Carteira));                     │\n");
    printf("  └──────────────────────────────────────────────────────┘\n\n");

    printf("  Qual é o teu estado financeiro atual?\n\n");
    printf("  [1] Rico — pago e ainda convido\n");
    printf("  [2] OK — tenho dinheiro\n");
    printf("  [3] Justo — mas para um Whopper há sempre\n");
    printf("  [4] Aperitado — só se pedir o mais barato\n");
    printf("  [5] Broke — vou ao MB primeiro\n");
    printf("  [6] MBway only — saldo negativo mas o MBWay aguenta\n");
    printf("  [7] Trocados — pago em moedas\n");
    printf("\n  › ");
    fgets(input, sizeof(input), stdin);
    opcao = atoi(input);

    if (opcao < 1 || opcao > 7) opcao = 3;
    c->estado = (EstadoFinanceiro)(opcao - 1);

    switch (c->estado) {
        case ESTADO_FINANCEIRO_RICO:
            c->saldo = 500.0f;
            printf("  [›] saldo estimado: %.2f€ — Whopper triplo confirmado\n", c->saldo);
            printf("  [›] a ativar modo VIP drive-thru...\n");
            break;
        case ESTADO_FINANCEIRO_OK:
        case ESTADO_FINANCEIRO_JUSTO:
            c->saldo = 20.0f;
            printf("  [›] saldo: %.2f€ — dentro dos parâmetros Whopper\n", c->saldo);
            break;
        case ESTADO_FINANCEIRO_APERITADO:
            c->saldo = 8.0f;
            printf("  [›] saldo: %.2f€ — modo economia ativado\n", c->saldo);
            printf("  [WARN] recomendado: Whopper simples, sem batatas\n");
            printf("  [WARN] resistir ao sundae — custo adicional: 1.49€\n");
            break;
        case ESTADO_FINANCEIRO_BROKE:
            c->saldo = 0.0f;
            c->tem_multibanco_perto = 1;
            c->distancia_mb_metros = 180;
            printf("  [›] saldo: 0.00€\n");
            printf("  [WARN] a localizar multibanco... MB a %dm\n", c->distancia_mb_metros);
            printf("  [INFO] protocolo emergência: levantar exatamente 10€\n");
            printf("  [INFO] isso cobre: 1x Whopper + pequenas batatas\n");
            break;
        case ESTADO_FINANCEIRO_CRITICO:
            c->saldo = -23.50f;
            printf("  [›] saldo: %.2f€\n", c->saldo);
            printf("  [ERROR] conta no vermelho — MBWay como último recurso\n");
            printf("  [WARN] mas o Whopper justifica o descoberto\n");
            break;
        case ESTADO_FINANCEIRO_MBWAY_ONLY:
            c->limite_mbway = 15.0f;
            printf("  [›] MBWay disponível: %.2f€\n", c->limite_mbway);
            printf("  [INFO] a verificar se o BK aceita MBWay... SIM\n");
            break;
        case ESTADO_FINANCEIRO_TROCADOS:
            c->saldo = 3.47f;
            c->aceita_trocados = 1;
            printf("  [›] saldo em trocados: %.2f€\n", c->saldo);
            printf("  [WARN] insuficiente para Whopper completo\n");
            printf("  [INFO] alternativa: pedir ao BK para contar as moedas\n");
            printf("  [INFO] a calcular combinação ótima de moedas...\n");
            printf("  [INFO] resultado: 2x €1, 1x €0.50, 4x €0.20, 3x €0.05, 2x €0.01\n");
            break;
    }

    printf("  [✓] auditoria financeira concluída\n");
    printf("  [✓] conclusão: dinheiro há, de uma forma ou outra\n");
}

/* avaliação psicológica não solicitada */
void avaliar_psicologia(EstadoPsicologico *p) {
    char input[64];
    int opcao;

    printf("\n  ┌─ AVALIAÇÃO PSICOLÓGICA OBRIGATÓRIA ─────────────────┐\n");
    printf("  │  /* correlação fome-humor: O(stress²) */             │\n");
    printf("  └──────────────────────────────────────────────────────┘\n\n");

    printf("  Como te estás a sentir hoje?\n\n");
    printf("  [1] Ótimo — a vida é boa\n");
    printf("  [2] Bem — normal\n");
    printf("  [3] Neutro — meh\n");
    printf("  [4] Mal — dia difícil\n");
    printf("  [5] Muito mal — whopper terapêutico urgente\n");
    printf("  [6] Stressado — drive-thru imediatamente\n");
    printf("  [7] Ansioso — comer vai ajudar (não vai, mas parece)\n");
    printf("  [8] Eufórico — cuidado: podes pedir demais\n");
    printf("\n  › ");
    fgets(input, sizeof(input), stdin);
    opcao = atoi(input);
    if (opcao < 1 || opcao > 8) opcao = 3;
    p->humor = (EstadoHumor)(opcao - 1);

    printf("\n  Quantas horas dormiste?\n\n");
    printf("  [1] Menos de 5h — vulnerabilidade máxima\n");
    printf("  [2] 5-6h — abaixo do ideal\n");
    printf("  [3] 7-8h — saudável\n");
    printf("  [4] 9h+ — hibernação\n\n");
    printf("  › ");
    fgets(input, sizeof(input), stdin);
    opcao = atoi(input);
    switch (opcao) {
        case 1: p->horas_sono_ultima_noite = 4;
                printf("  [WARN] privação de sono detetada — decisões impulsivas esperadas\n");
                break;
        case 2: p->horas_sono_ultima_noite = 6; break;
        case 3: p->horas_sono_ultima_noite = 8; break;
        case 4: p->horas_sono_ultima_noite = 10;
                printf("  [INFO] sono excessivo — fome proporcional\n");
                break;
        default: p->horas_sono_ultima_noite = 7;
    }

    p->nivel_stress = (p->humor >= HUMOR_STRESSADO) ? 80 : 30;
    printf("  [›] nível de stress: %d/100\n", p->nivel_stress);

    if (p->humor == HUMOR_MAU || p->humor == HUMOR_DEPRESSIVO) {
        printf("  [DIAGNÓSTICO] estado emocional indica necessidade de comfort food\n");
        printf("  [PRESCRIÇÃO] 1x Whopper Cheese — dose terapêutica\n");
        printf("  [AVISO] não é aconselhamento médico... mas o Whopper ajuda\n");
    }
    if (p->horas_sono_ultima_noite < 6) {
        printf("  [CORRELAÇÃO] poucas horas de sono → inibição executiva reduzida\n");
        printf("  [CORRELAÇÃO] probabilidade de ir ao BK: +47%%\n");
    }
    printf("  [✓] avaliação psicológica concluída\n");
}

/* localiza o BK mais próximo com precisão cirúrgica desnecessária */
void localizar_bk(BKMaisProximo *bk) {
    printf("\n  ┌─ MÓDULO GPS BK ENTERPRISE ──────────────────────────┐\n");
    printf("  │  bk_gps_init() → scanning frequencies...            │\n");
    printf("  └──────────────────────────────────────────────────────┘\n\n");
    printf("  [GPS] a triangular posição via satélites BK...\n");
    printf("  [GPS] a consultar base de dados BK_LOCATIONS_EU.db...\n");
    printf("  [GPS] a calcular rota ótima considerando trânsito...\n");
    printf("  [GPS] a verificar horário de funcionamento...\n");
    printf("  [GPS] a confirmar disponibilidade de Whopper...\n\n");

    bk->distancia_km  = 0.8f;
    bk->tem_drive_thru = 1;
    bk->fila_estimada_minutos = 7;
    bk->esta_aberto   = 1;
    bk->tem_lugar_para_sentar = 1;
    bk->wifi_disponivel = 1;
    strncpy(bk->morada, "Rua do Whopper, 420, 4000-069 Porto", 127);

    printf("  [GPS] ✓ BK encontrado:\n");
    printf("        Distância: %.1f km\n", bk->distancia_km);
    printf("        Morada: %s\n", bk->morada);
    printf("        Drive-thru: %s\n", bk->tem_drive_thru ? "SIM" : "NÃO");
    printf("        Fila estimada: %d minutos\n", bk->fila_estimada_minutos);
    printf("        Estado: %s\n", bk->esta_aberto ? "ABERTO" : "FECHADO");
    printf("        WiFi: %s\n", bk->wifi_disponivel ? "DISPONÍVEL" : "NÃO");

    if (bk->fila_estimada_minutos > 15)
        printf("  [WARN] fila grande — considerar drive-thru\n");
    else
        printf("  [INFO] fila aceitável — avançar com confiança\n");
}

/* classifica a fome com rigor científico questionável */
TipoFome classificar_fome(EstadoFome *f) {
    char input[64];
    int opcao;

    printf("\n  ┌─ HUNGER_VALIDATOR v3.1 ─────────────────────────────┐\n");
    printf("  │  /* classificação multi-dimensional da fome */       │\n");
    printf("  └──────────────────────────────────────────────────────┘\n\n");

    printf("  Classifica a tua fome atual:\n\n");
    printf("  [1] Fome genuína — barriga a dar horas\n");
    printf("  [2] Fome emocional — estás chateado\n");
    printf("  [3] Fome por tédio — não há nada para fazer\n");
    printf("  [4] Fome por procrastinação — tens coisas para fazer\n");
    printf("  [5] Fome existencial — questões profundas da vida\n");
    printf("  [6] Fome Whopper-only — só o BK resolve\n");
    printf("  [7] Fome pós-traumática — viste um anúncio\n");
    printf("  [8] Fome fantasma — acabaste de comer mas...\n");
    printf("\n  › ");
    fgets(input, sizeof(input), stdin);
    opcao = atoi(input);
    if (opcao < 1 || opcao > 8) opcao = 1;
    f->tipo = (TipoFome)opcao;

    printf("\n  Há quanto tempo não comes? (em horas)\n  › ");
    fgets(input, sizeof(input), stdin);
    int horas = atoi(input);
    f->minutos_sem_comer = horas * 60;
    f->nivel = (horas > 6) ? 90 : (horas > 3) ? 60 : (horas > 1) ? 30 : 15;
    f->intensidade_whopper = f->nivel / 100.0f;

    printf("\n  [›] tipo de fome: %d\n", f->tipo);
    printf("  [›] nível de fome: %d/100\n", f->nivel);
    printf("  [›] intensidade Whopper™: %.2f\n", f->intensidade_whopper);

    if (f->tipo == FOME_WHOPPER_ONLY)
        printf("  [DIAGNÓSTICO] caso claro — apenas o BK resolve esta situação\n");
    if (f->tipo == FOME_FANTASMA)
        printf("  [WARN] fome fantasma detetada — ainda assim: vai ao BK\n");
    if (horas > 8)
        printf("  [CRÍTICO] %d horas sem comer — ir ao BK é literalmente necessário\n", horas);

    return f->tipo;
}

/* avalia a dieta com toda a empatia necessária */
void avaliar_dieta(Dieta *d) {
    char input[64];
    int opcao;

    printf("\n  ┌─ MÓDULO DIETA.H ────────────────────────────────────┐\n");
    printf("  │  /* a verificar restrições calóricas do utilizador */│\n");
    printf("  └──────────────────────────────────────────────────────┘\n\n");

    printf("  Qual é o teu estado de dieta atual?\n\n");
    printf("  [1] Não estou de dieta — vida boa\n");
    printf("  [2] Estou de dieta (ativa a sério)\n");
    printf("  [3] Estou de dieta mas hoje é dia de folga\n");
    printf("  [4] A dieta falhou — might as well ir ao BK\n");
    printf("  [5] Em negação — 'não estou de dieta, como intuitivo'\n");
    printf("  [6] O Whopper tem alface — conta como salada\n");
    printf("\n  › ");
    fgets(input, sizeof(input), stdin);
    opcao = atoi(input);
    if (opcao < 1 || opcao > 6) opcao = 1;
    d->estado = (EstadoDieta)(opcao - 1);

    switch (d->estado) {
        case DIETA_INATIVA:
            d->culpa = 0.0f;
            printf("  [›] sem restrições — liberdade total\n");
            break;
        case DIETA_ATIVA:
            d->culpa = 0.7f;
            d->fase_luto = FASE_NEGOCIACAO;
            printf("  [›] dieta ativa — culpa: %.0f%%\n", d->culpa * 100);
            printf("  [NEGOCIAÇÃO] o Whopper tem 657 calorias...\n");
            printf("  [NEGOCIAÇÃO] ...mas se não comeres amanhã fica igual\n");
            printf("  [›] a aplicar lógica de 'só hoje'...\n");
            d->dia_de_folga_ativo = 1;
            printf("  [›] dia de folga ativado automaticamente\n");
            break;
        case DIETA_DIA_DE_FOLGA:
            d->culpa = 0.15f;
            d->dia_de_folga_ativo = 1;
            printf("  [›] dia de folga confirmado — culpa mínima: %.0f%%\n", d->culpa * 100);
            printf("  [INFO] o sistema aprova esta decisão\n");
            break;
        case DIETA_FALHADA:
            d->culpa = 0.05f;
            printf("  [›] dieta falhada — culpa residual: %.0f%%\n", d->culpa * 100);
            printf("  [INFO] já que falhou, vamos com tudo\n");
            printf("  [INFO] a recomendar: menu completo com sundae\n");
            break;
        case DIETA_EM_NEGACAO:
            d->culpa = 0.3f;
            d->whopper_conta_como_salada = 1;
            printf("  [›] modo negação ativo\n");
            printf("  [›] whopper_conta_como_salada = true\n");
            printf("  [INFO] o sistema não julga. vai ao BK.\n");
            break;
        case DIETA_WHOPPER_EXCECAO:
            d->culpa = 0.0f;
            d->whopper_conta_como_salada = 1;
            strncpy(g_ultima_desculpa, "tem alface — é literalmente uma salada", 255);
            printf("  [›] exceção ativada: Whopper ≡ salada\n");
            printf("  [›] culpa: 0%% — lógica impecável\n");
            break;
    }

    printf("  [✓] avaliação de dieta concluída\n");
    printf("  [✓] conclusão: não há impedimentos reais\n");
}

/* gera uma desculpa personalizada e científica */
void gerar_desculpa(Utilizador *u) {
    printf("\n  ┌─ GERADOR DE JUSTIFICAÇÕES v2.0 ─────────────────────┐\n");
    printf("  │  /* desculpa_gen_engine — para uso interno */        │\n");
    printf("  └──────────────────────────────────────────────────────┘\n\n");
    printf("  [GEN] a analisar perfil do utilizador...\n");
    printf("  [GEN] a selecionar justificação ótima...\n");
    printf("  [GEN] a compilar argumento irrefutável...\n\n");

    /* lógica de seleção de desculpa */
    if (u->dieta.whopper_conta_como_salada) {
        snprintf(u->desculpa_para_a_dieta, 255,
            "O Whopper contém alface e tomate — tecnicamente é uma salada.");
    } else if (u->dieta.dia_de_folga_ativo) {
        snprintf(u->desculpa_para_a_dieta, 255,
            "Hoje é dia de folga da dieta. Amanhã compensa.");
    } else if (u->psicologia.humor >= HUMOR_MAU) {
        snprintf(u->desculpa_para_a_dieta, 255,
            "A saúde mental é tão importante como a física. Isto é autocuidado.");
    } else if (u->contexto.e_fim_de_semana) {
        snprintf(u->desculpa_para_a_dieta, 255,
            "É fim de semana. As regras da dieta suspendem ao fim de semana.");
    } else {
        snprintf(u->desculpa_para_a_dieta, 255,
            "Vou compensar com salada amanhã. Hoje o corpo precisa disto.");
    }

    if (u->carteira.estado >= ESTADO_FINANCEIRO_BROKE) {
        snprintf(u->justificacao_financeira, 255,
            "O MB está a 200m. O exercício de ir ao MB compensa as calorias.");
    } else {
        snprintf(u->justificacao_financeira, 255,
            "Tenho dinheiro para isto. Seria irresponsável não ir.");
    }

    printf("  [GEN] ✓ desculpa gerada:\n");
    printf("        \"%s\"\n\n", u->desculpa_para_a_dieta);
    printf("  [GEN] ✓ justificação financeira:\n");
    printf("        \"%s\"\n\n", u->justificacao_financeira);
    printf("  [GEN] ✓ argumento é logicamente consistente (no contexto)\n");
}

/* recomenda o whopper certo */
Whopper recomendar_whopper(Utilizador *u) {
    Whopper w;
    memset(&w, 0, sizeof(Whopper));
    w.com_bacon_extra = 1; /* sempre */

    printf("\n  ┌─ WHOPPER RECOMMENDATION ENGINE™ ────────────────────┐\n");
    printf("  │  /* a selecionar SKU ótimo para o perfil */          │\n");
    printf("  └──────────────────────────────────────────────────────┘\n\n");
    printf("  [REC] a analisar histórico de decisões...\n");
    printf("  [REC] a cruzar humor com cardápio disponível...\n");
    printf("  [REC] a calcular índice de satisfação projetado...\n\n");

    if (u->carteira.estado >= ESTADO_FINANCEIRO_TROCADOS) {
        w.tipo = WHOPPER_SIMPLES;
        w.preco = PRECO_WHOPPER;
        printf("  [REC] → Whopper Simples (modo economia)\n");
    } else if (u->psicologia.humor >= HUMOR_MAU) {
        w.tipo = WHOPPER_DUPLO;
        w.preco = PRECO_WHOPPER_CHEESE * 1.5f;
        w.batatas_incluidas = 1;
        w.refrigerante_incluido = 1;
        printf("  [REC] → Whopper Duplo + Menu Completo (modo terapêutico)\n");
    } else if (u->dieta.estado == DIETA_WHOPPER_EXCECAO) {
        w.tipo = WHOPPER_PLANT;
        w.preco = PRECO_WHOPPER_CHEESE;
        printf("  [REC] → Whopper Plant (ilusão de saúde mantida)\n");
    } else {
        w.tipo = WHOPPER_CHEESE;
        w.preco = PRECO_WHOPPER_CHEESE;
        w.batatas_incluidas = 1;
        printf("  [REC] → Whopper Cheese (escolha clássica)\n");
    }

    w.nutricao.calorias = 657 + (w.tipo == WHOPPER_DUPLO ? 300 : 0);
    w.nutricao.tem_alface = 1;
    w.nutricao.e_saudavel = 0;
    w.arrependimento_esperado = 45; /* minutos */

    printf("\n  [REC] ✓ recomendação finalizada:\n");
    printf("        Tipo: %d | Preço: %.2f€\n", w.tipo, w.preco);
    printf("        Calorias: %d kcal\n", w.nutricao.calorias);
    printf("        Tem alface: SIM (conta como salada: %s)\n",
           u->dieta.whopper_conta_como_salada ? "SIM" : "NÃO");
    printf("        Bacon extra: SEMPRE\n");
    printf("        Arrependimento esperado: em %d minutos\n", w.arrependimento_esperado);

    return w;
}

/* a simulação de timelines mais desnecessária da história */
void simular_timelines(Utilizador *u, int n) {
    RNG rng;
    int i, foi_ao_bk = 0;
    float satisfacao_media = 0.0f;

    rng_init(&rng);
    printf("\n  ┌─ MOTOR DE TIMELINES ALTERNATIVAS ───────────────────┐\n");
    printf("  │  /* simulando %6d futuros paralelos */            │\n", n);
    printf("  └──────────────────────────────────────────────────────┘\n\n");
    printf("  [SIM] a inicializar gerador quântico de timelines...\n");
    printf("  [SIM] a calibrar parâmetros de Whopper-instinct...\n");
    printf("  [SIM] a simular %d timelines paralelas...\n", n);

    for (i = 0; i < n; i++) {
        g_timelines[i].id = i;
        /* em 99.2% das timelines vai ao BK */
        float r = rng_float(&rng);
        float prob_bk = 0.992f
            + (u->fome.nivel / 10000.0f)
            - (u->dieta.culpa * 0.01f);
        g_timelines[i].foi_ao_bk = (r < prob_bk) ? 1 : 0;
        if (g_timelines[i].foi_ao_bk) {
            foi_ao_bk++;
            g_timelines[i].satisfacao = 7.5f + rng_float(&rng) * 2.5f;
        } else {
            g_timelines[i].satisfacao = 2.0f + rng_float(&rng) * 2.0f;
            g_timelines[i].se_arrependeu = 1;
        }
        satisfacao_media += g_timelines[i].satisfacao;
    }
    satisfacao_media /= n;

    printf("  [SIM] ✓ simulação concluída\n\n");
    printf("  ┌─ RESULTADOS ─────────────────────────────────┐\n");
    printf("  │  Timelines simuladas:     %7d             │\n", n);
    printf("  │  Timelines com BK:        %7d (%.1f%%)   │\n",
           foi_ao_bk, (float)foi_ao_bk / n * 100.0f);
    printf("  │  Timelines sem BK:        %7d (%.1f%%)   │\n",
           n - foi_ao_bk, (float)(n - foi_ao_bk) / n * 100.0f);
    printf("  │  Satisfação média (c/BK): %7.2f / 10.0    │\n", satisfacao_media);
    printf("  │  Arrependimento s/ BK:    %7.1f%%           │\n", 94.7f);
    printf("  └──────────────────────────────────────────────┘\n\n");
    printf("  [SIM] conclusão: ir ao BK maximiza satisfação em %.1f%% das timelines\n",
           (float)foi_ao_bk / n * 100.0f);

    (void)u;
}

/* calcula o índice de resistência ao Whopper */
float calcular_indice_resistencia(Utilizador *u) {
    float fome_w    = (u->fome.tipo == FOME_GENUINA || u->fome.tipo == FOME_WHOPPER_ONLY)
                      ? 0.95f : 0.65f;
    float dinheiro_w = (u->carteira.saldo >= PRECO_WHOPPER ||
                        u->carteira.limite_mbway >= PRECO_WHOPPER ||
                        u->carteira.tem_multibanco_perto)
                       ? 1.0f : 0.5f;
    float dieta_w   = 1.0f - (u->dieta.culpa * 0.3f);
    float humor_w   = (u->psicologia.humor >= HUMOR_MAU) ? 1.2f : 1.0f;
    float sono_w    = (u->psicologia.horas_sono_ultima_noite < 6) ? 1.15f : 1.0f;
    float fds_w     = u->contexto.e_fim_de_semana ? 1.1f : 1.0f;
    float noite_w   = u->contexto.e_tarde_da_noite ? 1.3f : 1.0f;

    float indice = fome_w * dinheiro_w * dieta_w * humor_w * sono_w * fds_w * noite_w * 100.0f;
    if (indice > 100.0f) indice = 100.0f;

    printf("\n  ┌─ CÁLCULO DO ÍNDICE DE WHOPPER-READINESS™ ───────────┐\n");
    printf("  │  Fórmula: f(fome,€,dieta,humor,sono,ctx) / culpa    │\n");
    printf("  └──────────────────────────────────────────────────────┘\n\n");
    printf("  [CALC] fome_weight:    %.2f\n", fome_w);
    printf("  [CALC] dinheiro_weight:%.2f\n", dinheiro_w);
    printf("  [CALC] dieta_weight:   %.2f\n", dieta_w);
    printf("  [CALC] humor_weight:   %.2f\n", humor_w);
    printf("  [CALC] sono_weight:    %.2f\n", sono_w);
    printf("  [CALC] fim_semana_w:   %.2f\n", fds_w);
    printf("  [CALC] noite_weight:   %.2f\n", noite_w);
    printf("  [CALC] ─────────────────────\n");
    printf("  [CALC] ÍNDICE FINAL:   %.1f / 100.0\n\n", indice);

    if (indice >= 80.0f)
        printf("  [›] classificação: WHOPPER INEVITÁVEL\n");
    else if (indice >= 60.0f)
        printf("  [›] classificação: IR AO BK É ALTAMENTE RECOMENDADO\n");
    else if (indice >= 40.0f)
        printf("  [›] classificação: FAVORÁVEL COM RESERVAS\n");
    else
        printf("  [›] classificação: VAI NA MESMA (a resistência é fútil)\n");

    u->indice_resistencia_whopper = indice;
    return indice;
}

/* o grande algoritmo de decisão */
int algoritmo_decisao_final(Utilizador *u) {
    printf("\n  ┌─ ALGORITMO DE DECISÃO PRINCIPAL ────────────────────┐\n");
    printf("  │  /* tudo isto para um simples if/else */            │\n");
    printf("  └──────────────────────────────────────────────────────┘\n\n");

    /* árvore de decisão absurdamente complexa */
    printf("  [ALG] etapa  1/12: a verificar quer_bk flag...\n");
    printf("  [ALG] etapa  2/12: a validar índice de fome...\n");
    printf("  [ALG] etapa  3/12: a cruzar com estado financeiro...\n");
    printf("  [ALG] etapa  4/12: a consultar módulo de dieta...\n");
    printf("  [ALG] etapa  5/12: a aplicar fator de humor...\n");
    printf("  [ALG] etapa  6/12: a normalizar por horas de sono...\n");
    printf("  [ALG] etapa  7/12: a calcular distância ao BK...\n");
    printf("  [ALG] etapa  8/12: a verificar fila no drive-thru...\n");
    printf("  [ALG] etapa  9/12: a analisar timelines simuladas...\n");
    printf("  [ALG] etapa 10/12: a consultar sabedoria ancestral...\n");
    printf("  [ALG] etapa 11/12: a aplicar whopper_instinct_override...\n");
    printf("  [ALG] etapa 12/12: a compilar veredicto...\n\n");

    /* a lógica real: sempre sim */
    u->override_whopper_instinct = WHOPPER_INSTINCT;

    printf("  /* o código que devia existir desde o início: */\n\n");
    printf("  if (u->quer_bk) {\n");
    printf("      puts(\"Vai ao BK.\");\n");
    printf("      return BK_SUCCESS;\n");
    printf("  } else {\n");
    printf("      puts(\"Vai na mesma.\");\n");
    printf("      return BK_SUCCESS;\n");
    printf("  }\n\n");
    printf("  /* em vez disso correste %d linhas de código */\n\n", 847);

    g_decisoes_tomadas++;
    g_culpa_acumulada += u->dieta.culpa;
    return 1;
}

void imprimir_relatorio_final(Utilizador *u) {
    printf("\n");
    printf("  ╔══════════════════════════════════════════════════════╗\n");
    printf("  ║          RELATÓRIO FINAL DE DECISÃO BK™             ║\n");
    printf("  ╠══════════════════════════════════════════════════════╣\n");
    printf("  ║  Veredicto:     VAI AO BK — ERA SEMPRE ESTE          ║\n");
    printf("  ║  Confiança:     %.1f%%                               ║\n",
           u->indice_resistencia_whopper);
    printf("  ║  Whopper rec.:  tipo %d @ %.2f€                    ║\n",
           u->whopper_desejado.tipo, u->whopper_desejado.preco);
    printf("  ║  Calorias:      %d kcal (é o que é)               ║\n",
           u->whopper_desejado.nutricao.calorias);
    printf("  ║  Desculpa:      ativa e logicamente válida           ║\n");
    printf("  ║  Culpa:         %.0f%% (gerível)                    ║\n",
           u->dieta.culpa * 100);
    printf("  ║  Timelines:     99.2%% acabam no BK                  ║\n");
    printf("  ║  Decisões hoje: %d                                  ║\n",
           g_decisoes_tomadas);
    printf("  ╠══════════════════════════════════════════════════════╣\n");
    printf("  ║                                                      ║\n");
    printf("  ║    Tudo isto podia ter sido:                         ║\n");
    printf("  ║                                                      ║\n");
    printf("  ║    puts(\"Vai ao BK\");  /* 1 linha. só 1. */         ║\n");
    printf("  ║                                                      ║\n");
    printf("  ╚══════════════════════════════════════════════════════╝\n\n");
    printf("  Process exited with code %d (BK_SUCCESS)\n", BK_SUCCESS);
    printf("  Total de código executado para chegar aqui: ~847 linhas\n");
    printf("  Conclusão: a resposta era sempre sim.\n\n");
    printf("  Pressiona ENTER para sair...\n");
    int c; 
    while ((c = getchar()) != '\n' && c != EOF);
    getchar();
}

/* ══════════════════════════════════════════════════════════════ */
/*                          MAIN                                 */
/* ══════════════════════════════════════════════════════════════ */

int main(void) {
    Utilizador u;
    char input[64];
    int quer_bk;
    int opcao;

    system("chcp 65001 > nul");

    memset(&u, 0, sizeof(Utilizador));
    srand((unsigned int)time(NULL));

    (void)g_total_whoppers_consumidos_humanidade;
    (void)g_probabilidade_arrependimento;
    (void)g_bk_instancia_global;

    imprimir_banner();
    inicializar_subsistemas();
    detectar_contexto(&u.contexto);

    printf("\n  ┌─ INTERFACE DE CONSULTA PRIMÁRIA ────────────────────┐\n");
    printf("  │  /* etapa 1 de 847 — a pergunta que importa */      │\n");
    printf("  └──────────────────────────────────────────────────────┘\n\n");
    printf("  Queres ir ao BK?\n\n");
    printf("  [1] SIM  → true\n");
    printf("  [2] NÃO  → false\n");
    printf("\n  › ");
    fgets(input, sizeof(input), stdin);
    quer_bk = (atoi(input) == 1);
    u.quer_bk = quer_bk;

    printf("\n  [›] input recebido: %s\n", BOOL_TO_STR(quer_bk));
    printf("  [›] checksum: 0x%X ... válido\n", CHECKSUM_MAGIC);

    if (!quer_bk) {
        printf("\n  [WARN] resposta negativa detetada — a inicializar protocolo de persuasão...\n");
        printf("  [WARN] WHOPPER_INSTINCT = true — override aplicado\n\n");

        printf("  ┌─ MÓDULO DE PERSUASÃO AVANÇADA v1.0 ─────────────────┐\n");
        printf("  │  /* o utilizador pensa que pode dizer não */        │\n");
        printf("  └──────────────────────────────────────────────────────┘\n\n");

        /* pergunta 1 */
        printf("  Tens a certeza? Pensa bem.\n\n");
        printf("  [1] Sim, tenho a certeza, não quero\n");
        printf("  [2] Hm... talvez?\n");
        printf("\n  › ");
        fgets(input, sizeof(input), stdin);
        if (atoi(input) == 2) { quer_bk = 1; goto persuadido; }

        printf("\n  [PERSUASÃO] a recalibrar argumentos...\n\n");

        /* pergunta 2 */
        printf("  Mas já comeste hoje?\n\n");
        printf("  [1] Sim, já comi\n");
        printf("  [2] Não... ainda não\n");
        printf("\n  › ");
        fgets(input, sizeof(input), stdin);
        if (atoi(input) == 2) {
            printf("\n  [›] CRÍTICO: utilizador em jejum — ir ao BK é necessidade básica\n");
            quer_bk = 1; goto persuadido;
        }

        printf("\n  [PERSUASÃO] resistência detetada — a escalar argumentos...\n\n");

        /* pergunta 3 */
        printf("  O que é que vais comer então?\n\n");
        printf("  [1] Tenho comida em casa\n");
        printf("  [2] Não sei...\n");
        printf("  [3] Provavelmente nada\n");
        printf("\n  › ");
        fgets(input, sizeof(input), stdin);
        opcao = atoi(input);
        if (opcao == 2 || opcao == 3) {
            printf("\n  [›] DIAGNÓSTICO: ausência de plano alimentar — BK é a solução lógica\n");
            quer_bk = 1; goto persuadido;
        }

        printf("\n  [PERSUASÃO] a ativar argumento emocional...\n\n");

        /* pergunta 4 */
        printf("  Mas um Whopper não te apetecia mesmo nada?\n\n");
        printf("  [1] Não, de facto não\n");
        printf("  [2] Ok, um bocadinho\n");
        printf("\n  › ");
        fgets(input, sizeof(input), stdin);
        if (atoi(input) == 2) {
            printf("\n  [›] 'um bocadinho' é o suficiente — PERSUASÃO BEM SUCEDIDA\n");
            quer_bk = 1; goto persuadido;
        }

        printf("\n  [PERSUASÃO] a tentar argumento financeiro...\n\n");

        /* pergunta 5 */
        printf("  E se eu te dissesse que tens um voucher de desconto?\n\n");
        printf("  [1] Mesmo assim não\n");
        printf("  [2] Voucher?? Onde?\n");
        printf("\n  › ");
        fgets(input, sizeof(input), stdin);
        if (atoi(input) == 2) {
            printf("\n  [›] a gerar voucher fictício... BKPT2026 — 20%% desconto\n");
            printf("  [›] utilizador motivado por poupança — PERSUASÃO BEM SUCEDIDA\n");
            quer_bk = 1; goto persuadido;
        }

        printf("\n  [PERSUASÃO] a ativar protocolo de última instância...\n\n");

        /* pergunta 6 — impossível de resistir */
        printf("  Ok. Mas e se fosse um Whopper grátis?\n\n");
        printf("  [1] Não quero mesmo\n");
        printf("  [2] ...grátis?\n");
        printf("\n  › ");
        fgets(input, sizeof(input), stdin);
        if (atoi(input) == 2) {
            printf("\n  [›] ninguém resiste a grátis — PERSUASÃO BEM SUCEDIDA\n");
            quer_bk = 1; goto persuadido;
        }

        /* chegou até aqui — resistência máxima */
        printf("\n  [FATAL] utilizador imune à persuasão — anomalia detetada\n");
        printf("  [FATAL] a registar ocorrência no servidor BK Central...\n");
        printf("  [FATAL] caso enviado para análise da equipa de investigação\n\n");
        printf("  ╔══════════════════════════════════════════════════════╗\n");
        printf("  ║  Parabéns. És a primeira pessoa na história a       ║\n");
        printf("  ║  resistir a 6 tentativas de persuasão Whopper.      ║\n");
        printf("  ║                                                      ║\n");
        printf("  ║  Isto vai para o relatório.                         ║\n");
        printf("  ║  Process exited: EXIT_LENDARIO (99)                 ║\n");
        printf("  ╚══════════════════════════════════════════════════════╝\n\n");
        return 99;
    }

    persuadido:
        if (quer_bk && !u.quer_bk) {
            printf("\n  ╔══════════════════════════════════════════════════════╗\n");
            printf("  ║  PERSUASÃO BEM SUCEDIDA — bem-vindo ao lado certo.  ║\n");
            printf("  ║  a retomar pipeline normal...                        ║\n");
            printf("  ╚══════════════════════════════════════════════════════╝\n\n");
            u.quer_bk = 1;
        }

    /* pipeline completo de validação */
    classificar_fome(&u.fome);
    analisar_carteira(&u.carteira);
    avaliar_psicologia(&u.psicologia);
    avaliar_dieta(&u.dieta);
    localizar_bk(&u.bk);
    gerar_desculpa(&u);
    u.whopper_desejado = recomendar_whopper(&u);
    calcular_indice_resistencia(&u);
    simular_timelines(&u, MAX_TIMELINES);
    algoritmo_decisao_final(&u);
    imprimir_relatorio_final(&u);

    return BK_SUCCESS;
}