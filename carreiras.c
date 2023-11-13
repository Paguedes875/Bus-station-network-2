/* iaed-23 - ist1107028 - project2 */
/*
 * Ficheiro:  carreiras.c
 * Autor:  Inês Duarte Rodrigues Almeida Paredes
 * Descrição: Código remetente às funções que tratam das carreiras.
*/

/*  Bibliotecas */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "project2.h"

                                /* Comando c */

/*============================================================================*/                                
/*
    Retorna TRUE se o argumento é igual a "inverso" e FALSE caso contrário.
*/

int avalia_arg(char* args){
    int i, t_arg, t_inv;
    char c[] = "inverso";

    t_arg = strlen(args);
    t_inv = strlen(c);

    if (t_arg < 3 || t_arg > t_inv)
        return FALSE;

    else {
        for (i = 0; i < t_arg; i++)
            if (args[i] != c[i])
                return FALSE;
    }
    return TRUE;
}


/* Recebe um nome, cria uma nova carreira. */

Carreira* cria_carreira(char* args) {
    Carreira* nova_carreira = NULL;

    malloc_carreira(&nova_carreira);
    malloc_nome(&(nova_carreira->nome), strlen(args));

    strcpy(nova_carreira->nome, args);
    nova_carreira->n_paragens = 0;
    nova_carreira->lst_paragens = NULL;
    nova_carreira->lst_ligacoes = NULL;
    nova_carreira->custoTotal = 0;
    nova_carreira->duracaoTotal = 0;
    nova_carreira->next = NULL;
    nova_carreira->prev = NULL;
    return nova_carreira;
}


/* Adiciona a carreira à lista de carreiras */

void adicionar_carreira(TodasCarreiras* tc, Carreira* nova_carreira) {
    Carreira* carreira_atual = NULL;

    /* Caso não exstir carreiras na lista */
    if (tc->head == NULL) {
        tc->head = nova_carreira;
        nova_carreira->prev = NULL;
        nova_carreira->next = NULL;
        return;
    }
    carreira_atual = tc->head;

    /* Caso exista carreiras na lista e vai até à última */
    while (carreira_atual->next != NULL) {
        carreira_atual = carreira_atual->next;
    }
    carreira_atual->next = nova_carreira;
    nova_carreira->prev = carreira_atual;
    nova_carreira->next = NULL;
}


/*
    Recebe o nome de uma carreira e a head da lista de carreiras e retorna 
    a carreira, se existir, ou retorna NULL.
*/

Carreira* encontraCarreira(char* args, Carreira* head_c) {
    Carreira* carreira_temp = head_c;

    while (carreira_temp != NULL) {
        if (strcmp(carreira_temp->nome, args) == 0)
            return carreira_temp;

        carreira_temp = carreira_temp->next;
    }
    return NULL;
}


/*
    Recebe a head da lista de carreiras e imprime o nome da carreira, a paragem 
    de origem, a paragem de destino, o número de paragens, o custo total e a 
    duração total.
*/

void listar_carreiras(Carreira* head_c) {
    Carreira* carreira_temp = head_c;
    Ligacoes* ligacao_temp = NULL;
    
    /* Caso a lista tenha carreiras, vão ser listadas */
    while (carreira_temp != NULL) {
        if (strlen(carreira_temp->nome) < 6) {
           printf("%s", carreira_temp->nome);

        /* Caso a carreira tenha paragens, vão ser listadas */
        if (carreira_temp->n_paragens != 0){
            printf(" %s", carreira_temp->lst_ligacoes->origem);
            ligacao_temp = carreira_temp->lst_ligacoes;
            /* ligacao_temp vai até à última ligação */
            while (ligacao_temp->next != NULL)
                ligacao_temp = ligacao_temp->next;
                
            printf(" %s", ligacao_temp->destino);
            
        printf(" %d %.2f %.2f\n", carreira_temp->n_paragens+1 ,carreira_temp->custoTotal, 
                carreira_temp->duracaoTotal);
        } else
            /* Caso a carreira não tenha paragens */
            printf(" %d %.2f %.2f\n", carreira_temp->n_paragens, carreira_temp->custoTotal, 
                carreira_temp->duracaoTotal); 
        }
        carreira_temp = carreira_temp->next;
    }
}


/*
    Recebe uma carreira e imprime o nome da carreira, a paragem de origem, a 
    paragem de destino, o número de paragens, o custo total e a duração total.
*/

void listar_uma_carreira(Carreira* carr) {
    Ligacoes* ligacao_temp = NULL;

    /* Caso a carreira não tenha paragens */
    if (carr->n_paragens == 0){}
    
    ligacao_temp = carr->lst_ligacoes;

    /* Caso a carreira tenha paragens, vão ser listadas */
    while (ligacao_temp->next!= NULL) {
        printf("%s, ", ligacao_temp->origem);
        ligacao_temp = ligacao_temp->next;
    }

    printf("%s, %s\n", ligacao_temp->origem, ligacao_temp->destino);
}


/*  Recebe um nome de uma carreira e lista todas as paragens por ordem inversa.*/

void ordena_paragens_inv(Carreira* carr) {
    Carreira* carreira_temp = carr;
    Ligacoes* ligacao_temp = NULL;

    /* Caso a lista não tenha ligações */
    if (carreira_temp->lst_ligacoes == NULL)
        return;
    
    ligacao_temp = carreira_temp->lst_ligacoes;

    if (ligacao_temp->next == NULL) {
        printf("%s, %s\n", ligacao_temp->destino, ligacao_temp->origem);
        return;
    }
    /* ligacao_temp vai até à última ligação */
    while (ligacao_temp->next != NULL)
        ligacao_temp = ligacao_temp->next;
    
    /* Vai imprimir as paragens por ordem inversa */
    while (ligacao_temp->prev != NULL) {
        printf("%s, ", ligacao_temp->destino);
        ligacao_temp = ligacao_temp->prev;
    }
    printf("%s, %s\n", ligacao_temp->destino, ligacao_temp->origem);
}
/*============================================================================*/
