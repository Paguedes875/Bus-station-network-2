/* iaed-23 - ist1107028 - project2 */
/*
 * Ficheiro:  paragens.c
 * Autor:  Inês Duarte Rodrigues Almeida Paredes
 * Descrição: Código remetente às funções que tratam das paragens.
*/

/*  Bibliotecas */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "project2.h"

                                /* Comando p */
                    
/*============================================================================*/
/*  
    Recebe um array com o nome, a latitude e longitude, e cria uma nova paragem.
*/

int verifica_args(char** args) {
    if ((conv_double(args[1]) < -90 || conv_double(args[1]) > 90) 
        && (conv_double(args[2]) < -180 || conv_double(args[2]) > 180))
        return FALSE;
    return TRUE;
}

/* Cria uma nova paragem */
Paragem* cria_paragem(char** args) {
    Paragem* nova_paragem = NULL;

    malloc_paragem(&nova_paragem);
    malloc_nome(&(nova_paragem->nome), strlen(args[0]));
    strcpy(nova_paragem->nome, args[0]);

    nova_paragem->latitude = conv_double(args[1]);
    nova_paragem->longitude = conv_double(args[2]);
    nova_paragem->n_carreiras = 0;
    nova_paragem->next = NULL;
    nova_paragem->prev = NULL;
    return nova_paragem;
}


/* Adiciona à lista de paragens */
void adicionar_paragem(ListaParagens* lp, Paragem* nova_paragem) {
    Paragem* paragem_atual = NULL;

    /* Caso a lista de paragens está vazia */
    if (lp->head == NULL) {
        lp->head = nova_paragem;
        nova_paragem->prev = NULL;
        nova_paragem->next = NULL;
        return;
    }
    paragem_atual = lp->head;

    /* Caso exista paragens na lista e vai até à última */
    while (paragem_atual->next != NULL) {
        paragem_atual = paragem_atual->next;
    }
    paragem_atual->next = nova_paragem;
    nova_paragem->prev = paragem_atual;
    nova_paragem->next = NULL;
}


/*
    Recebe o nome de uma paragem e a head da lista de paragens e retorna
    a paragem, se existir, ou retorna NULL.
*/

Paragem* encontraParagem(char* args, Paragem* head_p) {
    Paragem* paragem_temp = head_p;

    while (paragem_temp != NULL) {
        if (strcmp(paragem_temp->nome, args) == 0)
            return paragem_temp;

        paragem_temp = paragem_temp->next;
    }
    return NULL;
}


/* Função que recebe a head da lista de paragens e lista todas as paragens. */
void listar_paragens(Paragem* head_p) {
    Paragem* paragem_temp = head_p;

    /* Caso a lista tenha paragens, vão ser listadas */
    while (paragem_temp) {
        printf("%s: %16.12f %16.12f %d\n", paragem_temp->nome, paragem_temp->latitude, 
            paragem_temp->longitude, paragem_temp->n_carreiras);
        paragem_temp = paragem_temp->next;
    }
}


/* Função que recebe uma paragem e lista a sua informação. */
void listar_uma_paragem(Paragem* par) {
    printf("%16.12f %16.12f\n", par->latitude, par->longitude);
}

void paragens_destino(Paragem* par, TodasCarreiras* lst_c) {
    Carreira* carr = lst_c->head;
    Ligacoes* ligacao_temp = NULL;
    char** carreiras = NULL;
    int m, i;

    if (par->n_carreiras == 0)
        return;

    carreiras = (char**) malloc(sizeof(char*) * par->n_carreiras);
        for (i = 0; i < par->n_carreiras; i++)
            carreiras[i] = (char*) malloc(sizeof(char) * NOME);

    while (carr) {
        if (carr->lst_ligacoes == NULL) {
            carr = carr->next;
            continue;
        }
        ligacao_temp = carr->lst_ligacoes;

        /* Vai até à última ligacao */
        while (ligacao_temp->next != NULL)
            ligacao_temp = ligacao_temp->next;

        if (strcmp(par->nome, ligacao_temp->destino)) {
            if (strlen(carr->nome) > NOME){
                carreiras[m] = realloc(carreiras[m], sizeof(char) * (strlen(carr->nome)+1));
                if (!carreiras[m]) {
                    printf("No memory.\n");
                    exit(1);
                }
            }
            strcpy(carreiras[m], carr->nome);
            m++;
        }
        carr = carr->next;   
    }
    if (m > 0) {
        for (i = 0; i < m - 1; i++)
            printf("%s ", carreiras[i]);
        printf(" %s\n", carreiras[i]);
    }
    for (i = 0; i < par->n_carreiras; i++)
        free(carreiras[i]);
    free(carreiras);
}

/*============================================================================*/