/* iaed-23 - ist1107028 - project2 */
/*
 * Ficheiro:  project2.c
 * Autor:  Inês Duarte Rodrigues Almeida Paredes
 * Descrição: Programa que corresponde a um sistema de gestão de carreiras de
              transportes públicos.
*/

/*  Bibliotecas */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "project2.h"

/* Função principal */

int main() 
{
    int comando;
    TodasCarreiras* lst_c = cria_todas_carreiras();
    ListaParagens* lst_p = cria_lista_paragens();

    while ((comando = getchar()) != EOF)
        switch(comando){
            case 'q': comando_q(lst_c, lst_p);
                exit(0);
            case 'c': comando_c(lst_c);
                break;
            case 'p': comando_p(lst_p);
                break;
            case 'l': comando_l(lst_c, lst_p);
                break;
            case 'i': comando_i(lst_p, lst_c);
                break;
            case 'f': comando_f(lst_p, lst_c);
                break;
            case 'r': comando_r(lst_c, lst_p);
                break;
            case 'e': comando_e(lst_c, lst_p);
                break;
            case 'a': comando_a(lst_c, lst_p);
                break;
            default: break;
        }
    return 0;
}

                                /*Comandos*/
/*============================================================================*/

/*  Função que adiciona e lista as carreiras    */

void comando_c(TodasCarreiras* lst_c) {
    Carreira* head_c = lst_c->head;
    Carreira* idx_carr, *carr;
    int num_args, i;
    char str[buffer];
    char** args = (char**) malloc(sizeof(char*) * MAX_ARGS);

    for (i = 0; i < MAX_ARGS; i++)
        args[i] = (char*) malloc(sizeof(char) * NOME);
    
    leinput(str);
    num_args = separa_inputs(str, args, MAX_ARGS);

    if (num_args == 0)
        listar_carreiras(head_c);

    else if (num_args == 1){
        idx_carr = encontraCarreira(args[0], head_c);

        if (idx_carr == NULL){
            carr = cria_carreira(args[0]);
            adicionar_carreira(lst_c, carr);

        } else if (idx_carr->n_paragens == 0){}
        
        else
            listar_uma_carreira(idx_carr);

    } else if (num_args == 2){
        idx_carr = encontraCarreira(args[0], head_c);
        if (idx_carr == NULL) {}
        else {
            if (!avalia_arg(args[1]))
            printf("incorrect sort option.\n");

        else if (idx_carr->n_paragens != 0)
            ordena_paragens_inv(idx_carr);  
        }
    }
    liberta_memoria(args);
    free(args);
}

/*  Função que adiciona e lista paragens    */

void comando_p(ListaParagens* lst_p) {
    Paragem* head_p = lst_p->head;
    Paragem* idx_paragem, *paragem;
    int num_args, i;
    char str[buffer];
    char** args = (char**) malloc(sizeof(char*) * MAX_ARGS);

    for (i = 0; i < MAX_ARGS; i++)
        args[i] = (char*) malloc(sizeof(char) * NOME);
    
    leinput(str);
    num_args = separa_inputs(str, args, MAX_ARGS);

    if (num_args == 0)
        listar_paragens(head_p);

    else if (num_args == 3){
        if (encontraParagem(args[0], head_p))
            printf("%s: stop already exists.\n", args[0]);
        else{
            if (verifica_args(args) == TRUE) {
                paragem = cria_paragem(args);
                adicionar_paragem(lst_p, paragem);
            } else
                printf ("invalid location.\n");
        }
    
    } else if (num_args == 1){
        idx_paragem = encontraParagem(args[0], head_p);
        if (idx_paragem == NULL)
            printf("%s: no such stop.\n", args[0]);
        else
            listar_uma_paragem(idx_paragem);
    }
    liberta_memoria(args);
    free(args);
}


/*  Função que adiciona ligações    */

void comando_l(TodasCarreiras* lst_c, ListaParagens* lst_p) {
    Carreira* head_c = lst_c->head;
    Paragem* head_p = lst_p->head;
    int i;
    char str[buffer];
    char** args = (char**) malloc(sizeof(char*) * MAX_ARGS);

    for (i = 0; i < MAX_ARGS; i++)
        args[i] = (char*) malloc(sizeof(char) * NOME);

    leinput(str);
    separa_inputs(str, args, MAX_ARGS);

    if (encontraCarreira(args[0], head_c) == NULL)
        printf("%s: no such line.\n", args[0]);

    else if (encontraParagem(args[1], head_p) == NULL)
        printf("%s: no such stop.\n", args[1]);
    
    else if (encontraParagem(args[2], head_p) == NULL)
        printf("%s: no such stop.\n", args[2]);

    else if (!valida_custo_duracao(conv_double(args[3]), conv_double(args[4])))
        printf("negative cost or duration.\n");

    else 
        add_ligacoes(args, lst_p, head_c);
    
    liberta_memoria(args);
    free(args);
}

void comando_f(ListaParagens* lst_p, TodasCarreiras* lst_c) {
    Paragem* head_p = lst_p->head;
    Paragem* idx_paragem;
    int i;
    char str[buffer];
    char** args = (char**) malloc(sizeof(char*) * MAX_ARGS);

    for (i = 0; i < MAX_ARGS; i++)
        args[i] = (char*) malloc(sizeof(char) * NOME);

    leinput(str);
    separa_inputs(str, args, MAX_ARGS);

    idx_paragem = encontraParagem(args[0], head_p);
    if (idx_paragem == NULL)
        printf("%s: no such stop.\n", args[0]);
    else
        paragens_destino(idx_paragem, lst_c);

    liberta_memoria(args);
    free(args);
}


/*  Função que lista as interseções entre carreiras    */

void comando_i(ListaParagens* lst_p, TodasCarreiras* lst_c) {
    int i, m = 0;
    Paragem* p = lst_p->head;

    while (p) {
        Carreira* carr = lst_c->head;
        /* Aloca memoria para uma matriz de carreiras */
        char** carreiras = (char**) malloc(sizeof(char*) * p->n_carreiras);
        for (i = 0; i < p->n_carreiras; i++)
            carreiras[i] = (char*) malloc(sizeof(char) * NOME);
        
        /* Caso não existirem ligações */
        if (carr->lst_ligacoes == NULL){
            for (i = 0; i < p->n_carreiras; i++)
                free(carreiras[i]);
            free(carreiras);
            return;
        }

        /* Encontra todas as carreiras com a paragem */
        while (carr) {
            Paragem* par = carr->lst_paragens;
            while (par) {
                if (strcmp(par->nome, p->nome) == 0) {
                    strcpy(carreiras[m], carr->nome);
                    m++;
                }
                par = par->next;
            }
            carr = carr->next;
        }
        /* Se for mais do que uma carreira */
        if (m > 1) {
            printf("%s %d:", p->nome, m);
            insertion_sort(carreiras, 0, m - 1);
            for (i = 0; i < m - 1; i++) {
                printf(" %s", carreiras[i]);
            }
            printf(" %s\n", carreiras[i]);
        }
        for (i = 0; i < p->n_carreiras; i++)
            free(carreiras[i]);
        free(carreiras);
        p = p->next;
        m = 0;
    }
}


/* Elimina uma carreira da lista de carreiras. */

void comando_r(TodasCarreiras* lst_c, ListaParagens* lst_p) {
    Carreira* head_c = lst_c->head;
    Carreira* carr;
    int num_args, i;
    char str[buffer];
    char** args = (char**) malloc(sizeof(char*) * MAX_ARGS);

    for (i = 0; i < MAX_ARGS; i++)
        args[i] = (char*) malloc(sizeof(char) * NOME);

    leinput(str);
    num_args = separa_inputs(str, args, MAX_ARGS);

    if (num_args == 1){
        carr = encontraCarreira(args[0], head_c);
        if (carr == NULL)
            printf("%s: no such line.\n", args[0]);
        else {
            remove_carreira(lst_c, carr, lst_p);
        }
    }
    liberta_memoria(args);
    free(args);
}


/* Elimina uma paragem */

void comando_e(TodasCarreiras* lst_c, ListaParagens* lst_p) {
    Paragem* head_p = lst_p->head;
    Paragem* par;
    int i;
    char str[buffer];
    char** args = (char**) malloc(sizeof(char*) * MAX_ARGS);

    for (i = 0; i < MAX_ARGS; i++)
        args[i] = (char*) malloc(sizeof(char) * NOME);

    leinput(str);
    separa_inputs(str, args, MAX_ARGS);

    par = encontraParagem(args[0], head_p);
    if (par == NULL)
        printf("%s: no such stop.\n", args[0]);
    else{
        comando_aux_e(lst_c, args, lst_p);
        remove_paragem(par, lst_p);
    }
    liberta_memoria(args);
    free(args);
}


/* Reenicia o programa */

void comando_a(TodasCarreiras* lst_c, ListaParagens* lst_p) {
    liberta_carreiras(lst_c, lst_p);
    lst_c->head = NULL;
    liberar_lista_paragens(lst_p);
}


/* Liberta a memoria toda e fecha o programa */

void comando_q (TodasCarreiras* lst_c, ListaParagens* lst_p) {
    liberta_carreiras(lst_c, lst_p);
    liberar_lista_paragens(lst_p);
    free(lst_c);
    free(lst_p);
}

/*============================================================================*/

/*                                Funções                                     */

/* Cria uma lista de carreiras global */
TodasCarreiras* cria_todas_carreiras() {
    TodasCarreiras* tc = (TodasCarreiras*) malloc(sizeof(TodasCarreiras));
    tc->head = NULL; 
    return tc;
}

/* Cria uma lista de paragens global */
ListaParagens* cria_lista_paragens() {
    ListaParagens* lp = (ListaParagens*) malloc(sizeof(ListaParagens));
    lp->head = NULL;
    return lp;
}

/* Liberta a memoria dos argumentos */
void liberta_memoria(char** args){
    int i;
    for (i = 0; i < MAX_ARGS; i++)
        free(args[i]);
}

/* Aloca a memoria das carreiras */
void malloc_carreira(Carreira** carr) {
    *carr = (Carreira*) malloc(sizeof(Carreira));
    if (!(*carr)) {
        printf("No memory.\n");
        exit(1);
    }
}

/* Aloca a memoria dos nomes */
void malloc_nome(char** str, int t) {
    *str = (char*) malloc(sizeof(char) * (t+1));
    if (!(*str)) {
        printf("No memory.\n");
        exit(1);
    }
}

/* Aloca a memoria das paragens */
void malloc_paragem(Paragem** p) {
    *p = (Paragem*) malloc(sizeof(Paragem));
    if (!(*p)) {
        printf("No memory.\n");
        exit(1);
    }
}

/* Aloca a memoria das ligacoes */
void malloc_ligacoes(Ligacoes** l) {
    *l = (Ligacoes*) malloc(sizeof(Ligacoes));
    if (!(*l)) {
        printf("No memory.\n");
        exit(1);
    }
}

/*
    Converte uma string que contem um número em ponto flutuante para um valor 
    do tipo double.
*/

double conv_double(char* str) {
    double num;

    num = strtod(str, NULL);
    return num;
}


/*
    Recebe um input, divide em argumentos, consequentemente os armazena num 
    vetor de argumentos e retorna o número de argumentos
*/

int separa_inputs(char* inputs, char** args, int max_args) {
    int i = 0, j = 0, dentro_aspas = FALSE, l, t = NOME;

    for (l = 0; inputs[l] != '\0' && i < max_args; l++) {
        char c = inputs[l];
        if (c == '"') {
            dentro_aspas = !dentro_aspas; 
            continue;
        }
        if (c == ' ' && !dentro_aspas){
            if (j > 0) { 
                args[i][j] = '\0';
                i++;
                j = 0;           /* Passa para o próximo argumento */
                t = NOME;
            }
        } else {
            args[i][j] = c;
            j++;
            if (j == t){ /* Caso o espaço alocado para o nome acaba */
                t *= 2;
                args[i] = realloc(args[i], sizeof(char) * (t));
                if (!args[i]) {
                    printf("No memory.\n");
                    exit(1);
                }
            }    
        }
    }
    if (j > 0) {
        args[i][j] = '\0'; /* Adiciona o último argumento na matriz args */
        i++;
    }
    return i;
}


/*
    Recebe uma string, armazena-a em um array de caracteres e retorna o tamanho
    da string.
*/

int leinput(char str[]) {
    int i = 0, c;

    while (((c = getchar()) != EOF && c != '\n'))
        str[i++] = c;
    str[i] = '\0';

    return i;
}

/*
    Função insertion sort que recebe uma matriz de carreiras, o indice inicial 
    e o indice final, e ordena alfabeticamente.
*/

int menor(char a[], char b[]){
    if (strcmp(a, b) < 0)
        return TRUE;
    return FALSE;
}

void insertion_sort(char** carr, int esq, int dir) {
    int i,j;

    for (i = esq + 1; i <= dir; i++) {
        char* temp;
        malloc_nome(&temp, strlen(carr[i]));

        strcpy(temp, carr[i]);
        j = i-1;
        while (j >= esq && menor(temp, carr[j])) {
            strcpy(carr[j+1], carr[j]);
            j--;
        }
        strcpy(carr[j+1], temp);
    }
}
/*============================================================================*/

/*
    Função que recebe uma lista de paragens e uma carreira e remove o numero de 
    carreiras de cada paragem da carreira.
*/

void remove_numero_carreiras_r(ListaParagens* lst_p, Carreira* carr) {
    Paragem* paragem_temp = lst_p->head;
    Paragem* paragem_temp2 = NULL;
    /* Percorre a lista de paragens */
    while (paragem_temp != NULL) {
        paragem_temp2 = carr->lst_paragens;
        /* Percorre a lista de paragens da carreira */
        while (paragem_temp2 != NULL){
            /* Caso encontre a paragem */
            if (strcmp(paragem_temp->nome, paragem_temp2->nome) == 0) {
                paragem_temp->n_carreiras--;
                break;
            }
            paragem_temp2 = paragem_temp2->next;
        }
        paragem_temp = paragem_temp->next;
    }
}

/*
    Recebe uma lista de carreiras e uma carreira, e liberta toda a memória 
    alocada para essa carreira.
*/

void remove_carreira(TodasCarreiras* tc, Carreira* carr, ListaParagens* lst_p) {
    Carreira* carreira_temp = tc->head;
    Paragem* paragem_temp = NULL;
    Ligacoes* ligacao_temp = NULL;
    Carreira* prev = NULL;

    /* Caso não haja carreiras */
    if (carreira_temp == NULL)
        return;

    /* Percorre toda a lista de carreiras */
    while (carreira_temp != NULL){

        /* Caso encontra a carreira */
        if (carreira_temp == carr){
            remove_numero_carreiras_r(lst_p, carreira_temp);
            /* Percorre todas as ligações e as liberta */
            ligacao_temp = carreira_temp->lst_ligacoes;
            while (ligacao_temp != NULL) {
                Ligacoes* temp1 = ligacao_temp->next;
                free(ligacao_temp->carreira1);
                free(ligacao_temp->origem);
                free(ligacao_temp->destino);
                free(ligacao_temp);
                ligacao_temp = temp1; 
            }

            /* Percorre todas as paragens e as liberta */
            paragem_temp = carreira_temp->lst_paragens;
            while (paragem_temp != NULL) {
                Paragem* temp2 = paragem_temp->next;
                free(paragem_temp->nome);
                free(paragem_temp);
                paragem_temp = temp2;
            }

            /* Caso seja a primeira carreira */
            if (prev == NULL)
                tc->head = carreira_temp->next;
            else
                prev->next = carreira_temp->next;
            free(carreira_temp->nome);
            free(carreira_temp);
            return;
        }
        prev = carreira_temp;
        carreira_temp = carreira_temp->next;
    }
}


/* Liberta toda a memória alocada para a lista de carreiras. */
void liberta_carreiras(TodasCarreiras* lst_c, ListaParagens* lst_p) {
    while (lst_c->head != NULL) {
        Carreira* carreira_temp = lst_c->head->next;    
        remove_carreira(lst_c, lst_c->head, lst_p);
        lst_c->head = carreira_temp;
    }
    lst_c->head = NULL;
}


/*
    Recebe um nome de uma paragem e uma carreira, e liberta toda a memória
    alocada para essa paragem.
*/
void liberta_paragem_carreira (char* n, Carreira* carr) {
    Paragem* paragem_temp = carr->lst_paragens;
    Paragem* prev = NULL;
    /* Percorre toda a lista de paragens da carreira */
    while (paragem_temp != NULL) {
        /* Caso encontre a paragem */
        if (strcmp(paragem_temp->nome, n) == 0) {
            if (prev == NULL)
                carr->lst_paragens = paragem_temp->next;
            else
                prev->next = paragem_temp->next;
            free(paragem_temp->nome);
            free(paragem_temp);
            return;
        }
        prev = paragem_temp;
        paragem_temp = paragem_temp->next;
    }
}


/* Recebe uma lista de paragens e liberta toda a memória alocada nessa lista. */
void liberar_lista_paragens(ListaParagens* lista_paragens) {
    while (lista_paragens->head != NULL) {
        Paragem* paragem_temp = lista_paragens->head->next;      
        free(lista_paragens->head->nome);
        free(lista_paragens->head);
        lista_paragens->head = paragem_temp;
    }
    lista_paragens->head = NULL;
}


/*
    Recebe uma lista de paragens e uma paragem, e liberta toda a memória 
    alocada para essa paragem.
*/

void remove_paragem(Paragem* par, ListaParagens* lst_p) {
    Paragem* paragem_temp = NULL;
    /* Caso não exista a paragem ou paragens na lista de paragens */
    if (par == NULL || lst_p == NULL)
        return;

    paragem_temp = lst_p->head;
    /* paragem_temp vai até à última ligação */
    while (paragem_temp->next)
        paragem_temp = paragem_temp->next;

    /* Caso seja ou não a primeira paragem */
    if (par->prev)
        par->prev->next = par->next;
    else 
        lst_p->head = par->next;

    /* Caso seja ou não a última paragem */
    if (par->next)
        par->next->prev = par->prev;
    else {
        paragem_temp = par->prev;
    }

    free(par->nome);
    free(par);
}


/*
    Recebe uma lista de paragens, um nome de uma paragem e uma carreira, e
    remove o número de carreiras que passam por essa paragem.
*/

void remove_numero_carreiras_e(ListaParagens* lp, char* n, Carreira* carr) {
    Paragem* paragem_temp = lp->head;
    Paragem* paragem_temp2 = carr->lst_paragens;

    /* Percorre toda a lista de paragens */
    while (paragem_temp != NULL) {
        if (strcmp(paragem_temp->nome, n) != 0) {
            /* Percorre toda a lista de paragens da carreira */
            while (paragem_temp2 != NULL) {
                /* Caso encontre a paragem */
                if (strcmp(paragem_temp2->nome, paragem_temp->nome) == 0) {
                    paragem_temp->n_carreiras--;
                    return;
                }
                paragem_temp2 = paragem_temp2->next;
            }
        }
        paragem_temp = paragem_temp->next;
    }
}


/* Remove a primeira ligação */
void remove_primeira_ligacao(Carreira* carr, ListaParagens* lst_p, char* n) {
    Ligacoes* head_l = carr->lst_ligacoes->next;
    /* Caso seja uma única ligação */
    if (head_l == NULL) 
        remove_numero_carreiras_e(lst_p, n, carr);
    
    /* Caso seja uma dupla ligação */
    if (strcmp(carr->lst_ligacoes->destino, n) != 0)
        liberta_paragem_carreira(n, carr);

    carr->custoTotal -= carr->lst_ligacoes->custo;
    carr->duracaoTotal -= carr->lst_ligacoes->duracao;
    carr->n_paragens--;
    free(carr->lst_ligacoes->origem);
    free(carr->lst_ligacoes->destino);
    free(carr->lst_ligacoes->carreira1);
    free(carr->lst_ligacoes);
    carr->lst_ligacoes = head_l;
}


/* Remove a unica ligação */
void remove_ligacao_unica (Carreira* carr, ListaParagens* lst_p, char* n, Ligacoes* ligacao_temp) {
    carr->custoTotal = 0;
    carr->duracaoTotal = 0;
    carr->n_paragens = 0;
    free(ligacao_temp->origem);
    free(ligacao_temp->destino);
    free(ligacao_temp->carreira1);
    free(ligacao_temp);
    carr->lst_ligacoes = NULL;
    /* Remove as carreiras da paragem da ligação eliminada */
    remove_numero_carreiras_e(lst_p, n, carr);
    /* Libera a memoria da paragem eliminada da lista de carreiras */
    liberta_paragem_carreira(n, carr);
}


/* Remove a ultima ligação */
void remove_ultima_ligacao(Carreira* carr, char* n, Ligacoes* ligacao_temp) {
    carr->custoTotal -= ligacao_temp->custo;
    carr->duracaoTotal -= ligacao_temp->duracao;
    carr->n_paragens--;
    /* Liberta a paragem da carreira */
    if (strcmp(ligacao_temp->origem, n) != 0)
        liberta_paragem_carreira(n, carr);
    free(ligacao_temp->origem);
    free(ligacao_temp->destino);
    free(ligacao_temp->carreira1);
    free(ligacao_temp);
}


/* Remove a paragem do extremo da carreira */
void remove_paragem_extremo(char* n, Carreira* carr, ListaParagens* lst_p) {
    Ligacoes* ligacao_temp = carr->lst_ligacoes;
    Ligacoes* prev = NULL;
    /* ligacao_temp vai até à última ligação */
    while (ligacao_temp->next != NULL) {
        prev = ligacao_temp;
        ligacao_temp = ligacao_temp->next;
    }
    /* Caso seja a ultima paragem */
    if (strcmp(ligacao_temp->destino, n) == 0) {
        if (prev != NULL){
            prev->next = NULL;
            remove_ultima_ligacao(carr, n, ligacao_temp);
            return;
        }
        else {
            /* Caso seja a única ligação */
            remove_ligacao_unica(carr, lst_p, n, ligacao_temp);
            return;
        }
    }
    /* Caso seja a primeira paragem */
    if (strcmp(carr->lst_ligacoes->origem, n) == 0) {
        remove_primeira_ligacao(carr, lst_p, n);
        return;
    }
}


/* Remove a paragem do meio da carreira */
void remove_ligacao(char* n, Carreira* carr) {
    Ligacoes* ligacao_temp = carr->lst_ligacoes;
    
    while (ligacao_temp != NULL) {
        if (strcmp(ligacao_temp->destino, n) == 0) {
            Ligacoes* ligacao_temp2 = ligacao_temp->next;

            /* Remove todas as ligações duplas */
            while (strcmp(ligacao_temp2->destino, n) == 0) {
                Ligacoes* temp = ligacao_temp2->next;
                carr->n_paragens--;
                ligacao_temp->custo += ligacao_temp2->custo;
                ligacao_temp->duracao += ligacao_temp2->duracao;
                if (temp->next == NULL)
                    ligacao_temp->next = NULL;
                else
                    temp->next->prev = ligacao_temp;
                /* Liberta a memoria da ligação e vai para a seguinte */
                free(ligacao_temp2->carreira1);
                free(ligacao_temp2->origem);
                free(ligacao_temp2->destino);
                free(ligacao_temp2);
                ligacao_temp2 = temp;
            }
            /* Remove a ligação simples */
            carr->n_paragens--;
            /* Liberta a memoria da destino atual e atribui o nome da paragem seguinte */
            free(ligacao_temp->destino);
            malloc_nome(&ligacao_temp->destino, strlen(ligacao_temp2->destino));
            strcpy(ligacao_temp->destino, ligacao_temp2->destino);
            ligacao_temp->custo += ligacao_temp2->custo;
            ligacao_temp->duracao += ligacao_temp2->duracao;

            liberta_paragem_carreira(n, carr);

            if (ligacao_temp2->next == NULL)
                ligacao_temp->next = NULL;
            else {
                ligacao_temp->next = ligacao_temp2->next;
                ligacao_temp->next->prev = ligacao_temp;
            }
            /* Liberta a memoria da ligação seguinte */
            free(ligacao_temp2->carreira1);
            free(ligacao_temp2->origem);
            free(ligacao_temp2->destino);
            free(ligacao_temp2);
        }
        ligacao_temp = ligacao_temp->next;
    }
}


/* Auxiliar do comando e, que usa outras funções para eliminar paragens */
void comando_aux_e (TodasCarreiras* lst_c, char** args, ListaParagens* lst_p) {
    Carreira* head_c = lst_c->head;
    while (head_c){
        Ligacoes* ligacao_temp = head_c->lst_ligacoes;
        if (ligacao_temp == NULL) {
            head_c = head_c->next;
            continue;
        }

        while (ligacao_temp->next != NULL)
            ligacao_temp = ligacao_temp->next;

        /* Enquanto houver extremos vai continuar a iterar */
        while (strcmp(head_c->lst_ligacoes->origem, args[0]) == 0 || 
            strcmp(ligacao_temp->destino, args[0]) == 0){
                Ligacoes* ligacao_temp2;
                remove_paragem_extremo(args[0], head_c, lst_p);

                /* Atualiza o ponteiro para o fim da lista */
                ligacao_temp2 = head_c->lst_ligacoes;

                /* Caso não haja ligações */
                if (ligacao_temp2 == NULL)
                    break;

                /* ligacao_temp2 vai até ao último */
                while (ligacao_temp2->next != NULL)
                    ligacao_temp2 = ligacao_temp2->next;
                ligacao_temp = ligacao_temp2;
            }
        remove_ligacao(args[0], head_c);
        head_c = head_c->next;
    }
}
