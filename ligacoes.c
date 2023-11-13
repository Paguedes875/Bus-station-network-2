/* iaed-23 - ist1107028 - project2 */
/*
 * Ficheiro:  ligacoes.c
 * Autor:  Inês Duarte Rodrigues Almeida Paredes
 * Descrição: Código remetente às funções que tratam das paragens.
*/

/*  Bibliotecas */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "project2.h"

                                /* Comando l */

/*============================================================================*/

/* Retorna TRUE se o custo e a duração são positivos e FALSE caso contrário. */
int valida_custo_duracao(double custo, double duracao) {
    if (custo >= 0 && duracao >= 0)
        return TRUE;

    return FALSE;
}


/*
    Recebe um nome da carreira, da origem, da destino, o custo e a duração
    da ligação e cria uma nova ligação.
*/
Ligacoes* cria_ligacao(char** args) {
    Ligacoes* nova_ligacao;
    malloc_ligacoes(&nova_ligacao);
    malloc_nome(&(nova_ligacao->carreira1), strlen(args[0]));
    strcpy(nova_ligacao->carreira1, args[0]);
    malloc_nome(&(nova_ligacao->origem), strlen(args[1]));
    strcpy(nova_ligacao->origem, args[1]);
    malloc_nome(&(nova_ligacao->destino), strlen(args[2]));
    strcpy(nova_ligacao->destino, args[2]);
    nova_ligacao->custo = conv_double(args[3]);
    nova_ligacao->duracao = conv_double(args[4]);
    nova_ligacao->next = NULL;
    nova_ligacao->prev = NULL;
       
    return nova_ligacao;
}


/*
    Recebe o nome de uma paragem e uma carreira e retorna a ligação, se existir,
    ou retorna NULL.
*/

Ligacoes* encontraParagem_Carreira(char* args, Carreira* carr) {
    Ligacoes* ligacao_temp = NULL;
    Ligacoes* prev = NULL;

    /* Caso não exista ligações*/
    if (carr->lst_ligacoes == NULL)
        return NULL;

    ligacao_temp = carr->lst_ligacoes;
    /* Vai procurar a primeira ligação com a paragem */
    while (ligacao_temp != NULL) {
        if (strcmp(ligacao_temp->origem, args) == 0)
            return ligacao_temp;
        prev = ligacao_temp;
        ligacao_temp = ligacao_temp->next;
    }
    if (strcmp(prev->destino, args) == 0)
        return prev;
        
    return NULL;
}


/*
    Recebe uma carreira e uma ligação e adiciona a ligação ao fim da lista da
    ligações da carreira.
*/

void add_ligacao_fim (Carreira* carr, Ligacoes* liga) {
    /* Caso não exista ligacoes */
    if (carr->lst_ligacoes == NULL) {
        carr->lst_ligacoes = liga;
    } else {
        Ligacoes* ligacao_temp = carr->lst_ligacoes;

        /* Vai até à última ligacao */
        while (ligacao_temp->next != NULL)
            ligacao_temp = ligacao_temp->next;
        
        /* Adiciona a nova ligação despois da última */
        ligacao_temp->next = liga;
        liga->prev = ligacao_temp;
    }

    liga->next = NULL;

    carr->custoTotal += liga->custo;
    carr->duracaoTotal += liga->duracao;
    carr->n_paragens++;
}


/*
    Recebe uma carreira e uma ligação e adiciona a ligação ao início da lista da 
    ligações da carreira.
*/

void add_ligacao_inicio(Carreira* carr, Ligacoes* liga) {
    /* Caso não exista ligacoes */
    if (carr->lst_ligacoes == NULL) {
        carr->lst_ligacoes = liga;
    } else {
        /* Adiciona antes da primeira ligação */
        carr->lst_ligacoes->prev = liga;
        liga->next = carr->lst_ligacoes;

        /* Atualiza o ponteiro da primeira ligação */
        carr->lst_ligacoes = liga;
    }

    carr->custoTotal += liga->custo;
    carr->duracaoTotal += liga->duracao;
    carr->n_paragens++;
}


/*
    Recebe um nome de uma paragem, uma lista de paragens e uma carreira e
    adiciona a paragem à lista de paragens da carreira e aumenta o numero
    de carreiras.
*/

void add_Carreira_Paragem(char* par, ListaParagens* lp, Carreira* carr) {
    Paragem* p = lp->head;
    Paragem* nova_paragem = NULL;
    malloc_paragem(&nova_paragem);
    malloc_nome(&(nova_paragem->nome), strlen(par));
    strcpy(nova_paragem->nome, par);

    /* Caso não exista paragens na carreira */
    if (carr->lst_paragens == NULL) {
        carr->lst_paragens = nova_paragem;
        nova_paragem->next = NULL;
        nova_paragem->prev = NULL;
    } else {
        /* Se existir adiciona no inicio */
        carr->lst_paragens->prev = nova_paragem;
        nova_paragem->prev = NULL;
        nova_paragem->next = carr->lst_paragens;
        carr->lst_paragens = nova_paragem;
    } 

    /* Verifica se a paragem já existe na lista de paragens */
    while (p) {
        if (strcmp(p->nome, par) == 0) {
            p->n_carreiras++;
            return;
        }
        p = p->next;
    }  
}


/*
    Função que adiciona a primeira ligação entre duas paragens de uma determinada 
    carreira. Recebe como argumentos a carreira, a matriz de strings e a lista
    de paragens.
*/

void add_primeira_ligacao(Carreira* carr, char** args, ListaParagens* lp) {
    Ligacoes* liga;
    add_Carreira_Paragem(args[1], lp, carr);
    if (strcmp(args[1], args[2]) != 0)
        add_Carreira_Paragem(args[2], lp, carr);
    liga = cria_ligacao(args);
    add_ligacao_fim(carr, liga);
}

/*
    Função principal que adiciona ligacoes entre duas paragens de uma determinada 
    carreira. Recebe como argumentos a matriz de strings, a lista de paragens e 
    a head da lista de carreiras.
*/

void add_ligacoes(char** args, ListaParagens* lp, Carreira* head_c) {
    Carreira* carr = head_c;
    Ligacoes* idx1, *idx2, *temp;
    Ligacoes* liga;

    /* Vai encontrar a carreira correspondete */
    while (carr->next != NULL) {
        if (strcmp(carr->nome, args[0]) == 0)
            break;
        carr = carr->next;
    }

    /* Caso não exista nenhuma ligação */
    if (carr->lst_ligacoes == NULL)
        add_primeira_ligacao(carr, args, lp);
    
    else {
        idx1 = carr->lst_ligacoes;
        temp = carr->lst_ligacoes;
        while (temp->next != NULL)
            temp = temp->next;
        idx2 = temp;

        /* Caso a paragem origem seja igual à primeira paragem da carreira */
        if (strcmp(args[1], idx2->destino) == 0){
            if (encontraParagem_Carreira(args[2], carr) == NULL)
                add_Carreira_Paragem(args[2], lp, carr);
            liga = cria_ligacao(args);
            add_ligacao_fim(carr, liga);
        }

        /* Caso a paragem destino seja igual à primeira paragem da carreira */
        else if (strcmp(args[2], idx1->origem) == 0){
            if (encontraParagem_Carreira(args[1], carr) == NULL)
                add_Carreira_Paragem(args[1], lp, carr);
            liga = cria_ligacao(args);
            add_ligacao_inicio(carr, liga);
        } else 
            printf("link cannot be associated with bus line.\n");
    }
}
/*============================================================================*/
