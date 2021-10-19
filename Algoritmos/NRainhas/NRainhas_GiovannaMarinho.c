#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

#include <stdlib.h>
#include <stdio.h>

#define LARGURA_TELA 640
#define ALTURA_TELA  640

void Inicializa_Tabuleiro(int *Tabuleiro, int N);
void Imprime_Tabuleiro(int *Tabuleiro, int N);
void Solucao(int *Tabuleiro, int N, int Coluna);
short Posiciona(int *Tabuleiro, int N, int Linha, int Coluna);

//OBS: implementacao desenvolvida em 2018 para o trabalho da disciplina de ATP-II
//Existem algumas limitacoes de interface que podem ser melhoradas, como a entrada de dados
//Foi utilizada a biblioteca de interface grafica Allegro5 (seu arquivo .dll pode ser encontrado nesta pasta)

int main (void)
{
    int N;
    printf("   ---------------------------------------------------------------------\n");
    printf("   ------------------------PROBLEMA DAS N RAINHAS-----------------------\n");
    printf("   ----------------------GIOVANNA CARREIRA MARINHO----------------------\n");
    printf("   ---------------------------------------------------------------------\n\n");
    printf("   DIGITE A QUANTIDADE DE RAINHAS / TAMANHO DO TABULEIRO: ");
    scanf("%d", &N);
    int Tabuleiro[N][N];

    al_init();                      //<Allegro> Inicializando a biblioteca.

    al_init_primitives_addon();     //<Allegro> Inicializando a biblioteca de primitivas (para fazer o tabuleiro).
    ALLEGRO_DISPLAY *janela = NULL; //<Allegro> Criando e inicializando a vari�vel que representa a janela principal.
    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);//<Allegro> Criando a janela, cujo a altura e largura s�o estipuladas pelas variaveis LARGURA_TELA e ALTURA_TELA.

    Inicializa_Tabuleiro(&Tabuleiro[0][0], N);
    Solucao(&Tabuleiro[0][0], N, 0);

    al_destroy_display(janela);     //<Allegro> Finalizando a janela.

    system("pause");
    return 0;
}

void Imprime_Tabuleiro(int *Tabuleiro, int N) //<Allegro>
{
    //Fun��o de impress�o do tabuleiro. Al�m disso mostra qual � a solu��o. Toda vez que a fun��o � chamada a variavel "solucao" � incrementada
    //isso justifica o fato de ser "static".
    //Ela n�o imprime aquele tabuleiro que foi inicializado na fun��o Inicializa_Tabuleiro, e sim o tabuleiro visual para o usu�rio.

    ALLEGRO_FONT* font = al_create_builtin_font();

    static int solucao=0;
    int i, j, p=1, t=LARGURA_TELA/N; //<Allegro> "t" � o tamanho de cada quadrado do tabuleiro.
    for(i=0 ; i<N ; i++) //<Allegro> Impress�o do tabuleiro
    {
        for(j=0 ; j<N ; j++)
        {
            p = !p;  //Toda vez que se muda de posi��o da matriz, a variavel p recebe 0 ou 1, dessa forma, dependendo do seu valor, o quadrado desenhado � de cor diferente.
            p ?  al_draw_filled_rectangle(i*t, j*t, i*t + t, j*t + t, al_map_rgb(255, 255, 255)):
            al_draw_filled_rectangle(i*t, j*t, i*t + t, j*t + t, al_map_rgb(145, 145, 145));
        }
        if (N%2 == 0)
            p = !p;
    }

    for ( i=0; i < N ; i++) //<Allegro> Dispondo as rainhas de acordo com o tabuleiro da l�gica do jogo.
    {
        for (j=0; j < N; j++)
        {
            if(*(Tabuleiro + i*N + j) == 1)
            {
                al_draw_filled_circle(j*t + (t/2), i*t + (t/2), t/4, al_map_rgb(0, 0, 0));
            }
        }
    }

    al_draw_textf(font, al_map_rgb(255, 0, 0), LARGURA_TELA -10, ALTURA_TELA - 30, ALLEGRO_ALIGN_RIGHT, "-GIOVANNA CARREIRA MARINHO");
    al_draw_textf(font, al_map_rgb(0, 0, 0), LARGURA_TELA -10, ALTURA_TELA - 20, ALLEGRO_ALIGN_RIGHT, "-NUMERO DE RAINHAS : %d", N);
    al_draw_textf(font, al_map_rgb(0, 0, 0), LARGURA_TELA -10, ALTURA_TELA - 10, ALLEGRO_ALIGN_RIGHT, "-SOLUCAO : %d", ++solucao);
    al_flip_display();  //<Allegro> Atualiza janela.
    al_rest(1.0);  //<Allegro> Segura a execu��o por 1 segundo.
    return;
}
void Inicializa_Tabuleiro(int *Tabuleiro, int N)
{
    //Diferente da estrat�gia do cavalo, esse tabuleiro � inicializado apenas com 0, ou seja, n�o tem a borda limitadora.
    int i, j;
    for(i=0; i < N; i++)
    {
        for(j=0; j < N; j++)
        {
            *(Tabuleiro + i*N + j) = 0;
        }
    }
    return;
}
void Solucao(int *Tabuleiro, int N, int Linha)
{
    int i;
    if(Linha == N)                      //Todas as rainhas j� foram posicionadas. (Caso base do passo de recurs�o.
    {
        Imprime_Tabuleiro(Tabuleiro, N);//� feito v�rias impressoes de acordo com a quantidade de solu��es. Al�m disso, ao chegar na posi��o final(Solucao(Tabuleiro, N, N);), � realizado uma nova tentativa de
        return;                         //configura��o do tabuleiro (posto que *(Tabuleiro + Linha*N + N-1)=0), at� que se esgote todas as possibilidades e se mostre todas as solu��es poss�veis.
    }
    for(i=0 ; i<N ; i++)
    {
        if(Posiciona(Tabuleiro, N, Linha, i))  //Verifica se pode posicionar a rainha naquela no posi��o.
        {
            *(Tabuleiro + Linha*N + i) = 1;    //Se puder, a posi��o recebe "1", significando que a rainha j� foi posicionada.
            Solucao(Tabuleiro, N, Linha + 1);  //Realiza o procedimento para a proxima linha de maneira recursiva.
            *(Tabuleiro + Linha*N + i) = 0;    //Se n�o conseguir posicionar na pr�xima linha [linha+1], a posi��o anterior [linha] recebe "0". Ou seja, a rainha � removida daquela posi��o
        }                                      //e � analisado a possibilidade de posicion�-la em outra coluna da linha, dessa forma a variavel "i" � incrementada (caso seja menor que N)
    }                                          //de modo a tentar posionar a rainha na proxima coluna. (BACKTRACK)
}                                              //Se n�o der (ou seja, j� foi analisado todas as possibilidades de modo que i>=N), volta � [linha-1], o "i" ser� incrementado a partir da coluna que estava... e assim por diante.
//Ou seja, se n�o achar uma solu��o para a pr�xima posi��o, � procurado uma nova solu��o para a posi��o anterior. Caso n�o h� uma solu��o para essa posi��o anterior, � feio para a anterior, e assim por diante.
short Posiciona(int *Tabuleiro, int N, int Linha, int Coluna)
{
    //� irredundante verificar nas linhas ap�s a rainha, pois � evidente que n�o tem outras rainhas naquelas pr�ximas posi��es, logo ser�
    //verificado apenas nas linhas acima da rainha.
    int i, j;
    //------------------------Linha e coluna-------------------------
    for(i=0 ; i<Linha ; i++)
    {
        if(*(Tabuleiro + i*N + Coluna))//Se encontrar o valor "1" quer dizer que h� uma rainha naquela coluna;
            return 0;                  //logo, � retornado o valor "0", ou seja, ela n�o pode ficar naquela posicao;
    }
    for(i=0 ; i<Coluna ; i++)
    {
        if(*(Tabuleiro + Linha*N + i)) //Se encontrar o valor "1" quer dizer que h� uma rainha naquela linha;
            return 0;                  //logo, � retornado o valor "0", ou seja, ela n�o pode ficar naquela posicao;
    }
    //------------------------Diagonal principal---------------------
    for(i=Linha, j=Coluna ; i>=0 && j>=0 ; i--, j--) //� necessario verificar apenas acima da diagonal principal
    {
        if(*(Tabuleiro + i*N + j))
            return 0;
    }
    //------------------------Diagonal secund�ria--------------------
    for(i=Linha, j=Coluna ; i>=0 && j<N ; i--, j++) //� necessario verificar apenas acima da diagonal secund�ria
    {
        if(*(Tabuleiro + i*N + j))
            return 0;
    }
    //Ap�s todas as verificac�es � retornado "1".
    return 1;
}
