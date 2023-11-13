#ifndef MAIN_H
#define MAIN_H

/*  Constantes  */
#define MAX_ARGS 5              /* número máximo de argumentos */
#define buffer 65535            /* número máximo de caracteres na instrução */
#define NOME 51                 /* número de caracteres no nome */

#define TRUE 1                  /* verdadeiro */
#define FALSE 0			        /* falso */


/* Tipos de Dados */

typedef struct paragem {
    char* nome;
    double latitude, longitude;
    int n_carreiras;
    struct paragem* next, *prev;
} Paragem;

typedef struct ligacoes {
    char* carreira1;
    char* origem;
    char* destino;
    double custo, duracao;
    struct ligacoes* next, *prev;
} Ligacoes;

typedef struct carreira{
    char* nome;
    Paragem* lst_paragens;
    Ligacoes* lst_ligacoes;
    int n_paragens;
    double custoTotal, duracaoTotal;
    struct carreira* next, *prev;
} Carreira;

typedef struct todas_carreiras {
    Carreira* head;
} TodasCarreiras;

typedef struct lista_paragens {
    Paragem* head;
} ListaParagens;

/* Prototipos */

TodasCarreiras* cria_todas_carreiras();

ListaParagens* cria_lista_paragens();

void liberta_memoria(char** args);

void malloc_carreira(Carreira** carr);

void malloc_nome(char** str, int t);

void malloc_paragem(Paragem** p);

void malloc_ligacoes(Ligacoes** l);

double conv_double(char* str);

/*============================================================================*/

int separa_inputs(char* inputs, char** args, int max_args);

int leinput(char str[]);

/*============================================================================*/

int avalia_arg(char* args);

Carreira* cria_carreira(char* args);

void adicionar_carreira(TodasCarreiras* tc, Carreira* nova_carreira);

Carreira* encontraCarreira(char* args, Carreira* head_c);

void listar_carreiras(Carreira* head_c);

void listar_uma_carreira(Carreira* carr);

void ordena_paragens_inv(Carreira* carr);

void comando_c(TodasCarreiras* lst_c);

/*============================================================================*/
int verifica_args(char** args);

Paragem* cria_paragem(char** args);

void adicionar_paragem(ListaParagens* lp, Paragem* nova_paragem);

Paragem* encontraParagem(char* args, Paragem* head_p);

void listar_paragens(Paragem* head_p);

void listar_uma_paragem(Paragem* par);

void comando_p(ListaParagens* lst_p);

/*============================================================================*/

int valida_custo_duracao(double custo, double duracao);

Ligacoes* cria_ligacao(char** args);

Ligacoes* encontraParagem_Carreira(char* args, Carreira* carr);

void add_ligacao_fim (Carreira* carr, Ligacoes* liga);

void add_ligacao_inicio(Carreira* carr, Ligacoes* liga);

void add_Carreira_Paragem(char* par, ListaParagens* lp, Carreira* carr);

void add_primeira_ligacao(Carreira* carr, char** args, ListaParagens* lp);

void add_ligacoes(char** args, ListaParagens* lp, Carreira* head_c);

void comando_l(TodasCarreiras* lst_c, ListaParagens* lst_p);

/*============================================================================*/

int menor(char a[], char b[]);

void insertion_sort(char** carr, int esq, int dir);

void comando_i(ListaParagens* lst_p, TodasCarreiras* lst_c);

void paragens_destino(Paragem* par, TodasCarreiras* lst_c);

void comando_f(ListaParagens* lst_p, TodasCarreiras* lst_c);

/*============================================================================*/

void remove_numero_carreiras_r(ListaParagens* lst_p, Carreira* carr);

void remove_carreira(TodasCarreiras* tc, Carreira* carr, ListaParagens* lst_p);

void liberta_carreiras(TodasCarreiras* lst_c, ListaParagens* lst_p);

void comando_r(TodasCarreiras* lst_c, ListaParagens* lst_p);

/*============================================================================*/

void remove_paragem(Paragem* par, ListaParagens* lst_p);

void remove_numero_carreiras_e(ListaParagens* lp, char* n, Carreira* carr);

void remove_primeira_ligacao(Carreira* carr, ListaParagens* lst_p, char* n);

void remove_ligacao_unica (Carreira* carr, ListaParagens* lst_p, char* n, Ligacoes* ligacao_temp);

void remove_ultima_ligacao(Carreira* carr, char* n, Ligacoes* ligacao_temp);

void remove_paragem_extremo(char* n, Carreira* carr, ListaParagens* lst_p);

void remove_ligacao(char* n, Carreira* carr);

void comando_aux_e (TodasCarreiras* lst_c, char** args, ListaParagens* lst_p);

void comando_e(TodasCarreiras* lst_c, ListaParagens* lst_p);

/*============================================================================*/

void liberta_paragem_carreira (char* n, Carreira* carr);

void liberar_lista_paragens(ListaParagens* lista_paragens);

void comando_a(TodasCarreiras* lst_c, ListaParagens* lst_p);

void comando_q (TodasCarreiras* lst_c, ListaParagens* lst_p);

/*============================================================================*/

int main();

#endif