#include <stdio.h>
#include <stdlib.h>
#include <time.h>


//________________________________________________________________________________
//                         ESTRUTURA DA ARVORE RUBRO NEGRA                       |
//--------------------------------------------------------------------------------
typedef struct No{
    int chave;
    short int cor;
    struct No *pai;
    struct No *filhoEsq;
    struct No *filhoDir;
}No;
//________________________________________________________________________________




//________________________________________________________________________________
//                                 VARIAVEIS GLOBAIS                             |
//--------------------------------------------------------------------------------
enum Cor {Vermelho=0, Preto=1};
No *raiz;
No *null;
//________________________________________________________________________________




//________________________________________________________________________________
//             FUNCAO DE BUSCA (IDENTICA DA BUSCA EM ARVORE BINARIA)             |
//--------------------------------------------------------------------------------
No *buscar(No *no, int chave){
    if(no == null){
        return null;
    }
    if(no->chave == chave){
        return no;
    }
    else if(chave < no->chave){
        return buscar(no->filhoEsq, chave);
    }
    else{
        return buscar(no->filhoDir, chave);
    }
}
//_______________________________________________________________________________




//________________________________________________________________________________
//                   FUNCAO CRIAR NO (alocagem e setup inicial)                  |
//--------------------------------------------------------------------------------
No *criarNo(int chave, No *filhoEsq, No *filhoDir, No *pai){
    No *no = malloc(sizeof(*no));
    no->chave = chave;
    no->pai = pai;
    no->filhoEsq = filhoEsq;
    no->filhoDir = filhoDir;
    //no->cor = Vermelho;
    return no;
}
//________________________________________________________________________________




//________________________________________________________________________________
//                 FUNCAO CRIAR ARVORE (alocagem e setup inicial)                |
//--------------------------------------------------------------------------------
void iniciarArvore(){
    // null e raiz globais
    null = criarNo(0, NULL, NULL, NULL);
    null->cor = Preto;
    raiz = null;
}
//________________________________________________________________________________




//________________________________________________________________________________
//                                 FUNCOES FLAG                                  |
//--------------------------------------------------------------------------------

// FUNCAO OBTER COR DO NO
int qualCor(No *no){
    if(no == null){
        return Preto;
    }
    else return no->cor;
}

// VERIFICA SE O NO TEM FILHO ESQUERDO
int temFilhoEsq(No *no){
    if(no != null && no->filhoEsq!=null){
        return 1;
    }
    return 0;
}

// VERIFICA SE O NO TEM FILHO DIREITO
int temFilhoDir(No *no){
    if(no != null && no->filhoDir!=null){
        return 1;
    }
    return 0;
}

// VERIFICA SE O NO TEM PAI (SE NAO EH RAIZ)
int temPai(No *no){
    if(no!=null && no->pai!=null){
        return 1;
    }
    return 0;
}

// VERIFICA SE O NO TEM IRMAO
int temIrmao(No *no){
    if((no != null)&&(temPai(no))&&(temFilhoDir(no->pai))&&(temFilhoEsq(no->pai))){
        return 1;
    }
    return 0;
}

// VERIFICA SE O NO TEM AVO
int temAvo(No *no){
    if(no->pai != null && no->pai->pai != null){
        return 1;
    }
    return 0;
}

// VERIFICA SE O NO TEM TIO
int temTio(No *no){
    if(temPai(no) && temIrmao(no->pai)){
        return 1;
    }
    return 0;
}

//________________________________________________________________________________




//________________________________________________________________________________
//                        FUNCOES DE MANIPULACAO DA ESTRUTURA                    |
//--------------------------------------------------------------------------------

// FUNCAO TROCA AS CORES ENTRE DOIS NOS
void trocarCores(No *a, No *b){
    int corDeA = a->cor;
    int corDeB = b->cor;
    a->cor = corDeB;
    b->cor = corDeA;
}

// FUNCAO INVERTE COR
void inverterCor(No *no){
    if(no == null){
        return;
    }
    no->cor = !(no->cor);
}

//FUNCAO ALTERAR COR DO NO
void alterarCor(No *no, int cor){
    if(no == null) {
        return;
    }
    no->cor = cor;
}

// RETORNA O PAI DE UM DADO NO
No *paiDoNo(No *no){
    return no->pai;
}

// RETORNA O AVO DE UM DADO NO
No *avoDoNo(No *no){
    return(paiDoNo(no))->pai;
}

// RETORNA O MDM DO NO (MENOR DOS MAIORES)
No *menorDosMaiores(No *no){
    No *aux;
    if(no->filhoDir == null){
        return null;
    }
    aux = no->filhoDir;
    while(aux->filhoEsq != null){
        aux = aux->filhoEsq;
    }
    return aux;
}

//_______________________________________________________________________________




//________________________________________________________________________________
//                              FUNCOES DE ROTACAO                               |
//--------------------------------------------------------------------------------

// ROTACIONAR A ESQUERDA
No *rotacionarEsquerda(No *raiz, No *no){
    No *auxiliar = no->filhoDir;
    no->filhoDir = auxiliar->filhoEsq;
    if(auxiliar->filhoEsq != null){
        auxiliar->filhoEsq->pai = no;
    }
    auxiliar->pai = no->pai;
    if(paiDoNo(no) == null){
        raiz = auxiliar;
    }
    else if(no == paiDoNo(no)->filhoEsq){
        paiDoNo(no)->filhoEsq = auxiliar;
    }
    else{
        paiDoNo(no)->filhoDir = auxiliar;
    }
    auxiliar->filhoEsq = no;
    no->pai = auxiliar;
    return raiz;
}

//ROTACIONAR A DIREITA
No *rotacionarDireita(No *raiz, No *no){
    No *auxiliar = no->filhoEsq;
    no->filhoEsq = auxiliar->filhoDir;
    if(auxiliar->filhoDir != null){
        auxiliar->filhoDir->pai = no;
    }
    auxiliar->pai = no->pai;
    if(paiDoNo(no) == null){
        raiz = auxiliar;
    }
    else if(no == paiDoNo(no)->filhoEsq){
        no->pai->filhoEsq = auxiliar;
    }
    else{
        paiDoNo(no)->filhoDir = auxiliar;
    }
    auxiliar->filhoDir = no;
    no->pai = auxiliar;
    return raiz;
}
//_______________________________________________________________________________




//________________________________________________________________________________
//                    FUNCAO CONSERTA ARVORE APOS UMA INSERSAO                   |
//--------------------------------------------------------------------------------
No *consertaInsercao(No *raiz, No *no){
    No *tio;
    while(paiDoNo(no)->cor == Vermelho){
        if(paiDoNo(no) == avoDoNo(no)->filhoEsq){
            tio = avoDoNo(no)->filhoDir;
            if(tio->cor == Vermelho){
                paiDoNo(no)->cor = Preto;
                tio->cor = Preto;
                avoDoNo(no)->cor = Vermelho;
                no = avoDoNo(no);
            }
            else{
                if(no == paiDoNo(no)->filhoDir){
                    no = paiDoNo(no);
                    raiz = rotacionarEsquerda(raiz, no);
                }
                else{
                    paiDoNo(no)->cor = Preto;
                    avoDoNo(no)->cor = Vermelho;
                    raiz = rotacionarDireita(raiz, avoDoNo(no));
                }
            }
        }

        else{
            tio = avoDoNo(no)->filhoEsq;
            if(tio->cor == Vermelho){
                paiDoNo(no)->cor = Preto;
                tio->cor = Preto;
                avoDoNo(no)->cor = Vermelho;
                no = avoDoNo(no);
            }
            else{
                if(no == paiDoNo(no)->filhoEsq){
                    no = paiDoNo(no);
                    raiz = rotacionarDireita(raiz, no);
                }
                else{
                    paiDoNo(no)->cor = Preto;
                    avoDoNo(no)->cor = Vermelho;
                    raiz = rotacionarEsquerda(raiz, avoDoNo(no));
                }
            }
        }
    }

    raiz->cor = Preto;
    return raiz;
}
//_______________________________________________________________________________




//________________________________________________________________________________
//                             INSERCAO SIMPLES EM BST                           |
//--------------------------------------------------------------------------------
No *inserir(No *raiz, int chave){
    if(buscar(raiz,chave)== null){
        No *novo = criarNo(chave, null, null, null); //NO A SER INSERIDO
        No *caminhador = raiz; // A BUSCA COMECA PELA RAIZ
        No *paix = null; // DURANTE A CAMINHADA SALVAREI O PAI DO NOVO NO
        while(caminhador != null){ //busca o pai do nodo novo
            paix = caminhador;
            if(chave < caminhador->chave){
                caminhador = caminhador->filhoEsq;
            }
            else{
                caminhador = caminhador->filhoDir;
            }
        }
        novo->pai = paix;
        if(paix == null){ //arvore vazia
            raiz = novo;
        }
        else if(chave < paix->chave){
            paix->filhoEsq = novo;
        }
        else{
            paix->filhoDir = novo;
        }
        novo->cor = Vermelho;
        raiz = consertaInsercao(raiz, novo);
    }
    return raiz;
}
//_______________________________________________________________________________




//________________________________________________________________________________
//                               FUNCOES IMPRESSORAS                             |
//--------------------------------------------------------------------------------

// IMPRESSAO SIMPLES
void imprimeNo(No *no){
    printf("CHAVE: %d   ", no->chave);
    printf("COR: ");
    if(no->cor){
        printf("Preto\n");
    }
    else{
        printf("Vermelho\n");
    }
}

// FUNCAO IMPRIME O STATUS DE UM NO, DETALHES
void statusNo(No *no){
    if(no!=NULL && no!=null){
        printf("CHAVE: %d\n", no->chave);
        printf("COR: ");
        if(no->cor){
            printf("Preto\n");
        }
        else{
            printf("Vermelho\n");
        }
        if(temAvo(no))
            printf("TEM AVO DE CHAVE: %d\n", no->pai->pai->chave);
        if(temTio(no)){
            if(no->pai == no->pai->pai->filhoEsq)
                printf("TEM TIO DE CHAVE: %d\n", no->pai->pai->filhoDir->chave);
            else
                printf("TEM TIO DE CHAVE: %d\n", no->pai->pai->filhoEsq->chave);
        }
        if(temPai(no))
            printf("TEM PAI DE CHAVE: %d\n", no->pai->chave);
        if(temIrmao(no)){
            if(no == no->pai->filhoEsq)
                printf("TEM IRMAO DE CHAVE: %d\n", no->pai->filhoDir->chave);
            else
                printf("TEM IRMAO DE CHAVE: %d\n", no->pai->filhoEsq->chave);
        }
        if(temFilhoEsq(no))
            printf("TEM FILHO ESQUERDO DE CHAVE: %d\n", no->filhoEsq->chave);
        if(temFilhoDir(no))
            printf("TEM FILHO DIREITO DE CHAVE: %d\n", no->filhoDir->chave);
        printf("\n");
    }
    else printf("O NO INFORMADO EH NULL ! ! !\n");
}


// IMPRIME CHAVE E COR DOS NOS DE UMA ARVORE EM ORDEM
void impressaoSimplesEmOrdem(No *no){
    if(no == null){
        return;
    }
    impressaoSimplesEmOrdem(no->filhoEsq);
    imprimeNo(no);
    impressaoSimplesEmOrdem(no->filhoDir);
}

// IMPRIME CHAVE E COR DOS NOS DE UMA ARVORE EM PRE ORDEM
void impressaoSimplesPreOrdem(No *no){
    if(no == null){
        return;
    }
    imprimeNo(no);
    impressaoSimplesPreOrdem(no->filhoEsq);
    impressaoSimplesPreOrdem(no->filhoDir);
}

// IMPRIME CHAVE E COR DOS NOS DE UMA ARVORE EM POS ORDEM
void impressaoSimplesPosOrdem(No *no){
    if(no == null){
        return;
    }
    impressaoSimplesPosOrdem(no->filhoEsq);
    impressaoSimplesPosOrdem(no->filhoDir);
    imprimeNo(no);
}

// IMPRIME O STATUS DOS NOS DE UMA ARVORE EM ORDEM
void impressaoCompletaEmOrdem(No *no){
    if(no==null){
        return; // retorno void
    }
    impressaoCompletaEmOrdem(no->filhoEsq);
    statusNo(no);
    impressaoCompletaEmOrdem(no->filhoDir);
}

// IMPRIME O STATUS DOS NOS DE UMA ARVORE EM PRE ORDEM
void impressaoCompletaPreOrdem(No *no){
    if(no==null){
        return; // retorno void
    }
    statusNo(no);
    impressaoCompletaPreOrdem(no->filhoEsq);
    impressaoCompletaPreOrdem(no->filhoDir);
}

// IMPRIME O STATUS DOS NOS DE UMA ARVORE EM POS ORDEM
void impressaoCompletaPosOrdem(No *no){
    if(no==null){
        return; // retorno void
    }
    impressaoCompletaPosOrdem(no->filhoEsq);
    impressaoCompletaPosOrdem(no->filhoDir);
    statusNo(no);
}



//_______________________________________________________________________________




//________________________________________________________________________________
//                    FUNCAO CONSERTA ARVORE APOS UMA REMOCAO                    |
//--------------------------------------------------------------------------------
void consertaRemocao(No *raiz, No *no){
    No *tio;
    while(no!=raiz && no->cor==Preto){
        if(no == paiDoNo(no)->filhoEsq){
            tio = paiDoNo(no)->filhoDir;
            if(tio->cor == Vermelho){
                tio->cor = Preto;
                paiDoNo(no)->cor = Vermelho;
                rotacionarEsquerda(raiz, paiDoNo(no));
                tio = paiDoNo(no)->filhoDir;
            }
            if(tio->filhoEsq->cor == Preto && tio->filhoDir->cor == Preto){
                tio->cor = Vermelho;
                no = paiDoNo(no);
            }
            else if(tio->filhoDir->cor == Preto){
                tio->filhoEsq->cor = Preto;
                tio->cor = Vermelho;
                rotacionarDireita(raiz, tio);
                tio = paiDoNo(no)->filhoDir;
            }
            tio->cor = paiDoNo(no)->cor;
            paiDoNo(no)->cor = Preto;
            tio->filhoDir->cor = Preto;
            rotacionarEsquerda(raiz, paiDoNo(no));
            no = raiz;
        }

        else{
            tio = paiDoNo(no)->filhoEsq;
            if(tio->cor == Vermelho){
                tio->cor = Preto;
                paiDoNo(no)->cor = Vermelho;
                rotacionarDireita(raiz,paiDoNo(no));
                tio = paiDoNo(no)->filhoEsq;
            }
            if(tio->filhoDir->cor == Preto && tio->filhoEsq->cor == Preto){
                tio->cor = Vermelho;
                no = paiDoNo(no);
            }
            else if(tio->filhoEsq->cor == Preto){
                tio->filhoDir->cor = Preto;
                tio->cor = Vermelho;
                rotacionarEsquerda(raiz, tio);
                tio = paiDoNo(no)->filhoEsq;
            }
            tio->cor = paiDoNo(no)->cor;
            paiDoNo(no)->cor = Preto;
            tio->filhoEsq->cor = Preto;
            rotacionarDireita(raiz, paiDoNo(no));
            no = raiz;
        }
    }
    no->cor = Preto;
}
//_______________________________________________________________________________




//________________________________________________________________________________
//                             REMOCAO SIMPLES EM BST                            |
//--------------------------------------------------------------------------------
No *remover(No *raiz, No *noDaChave){
    No* noARemover;
    No* filhoDoNoRemovido;
    if(noDaChave == null)
        return raiz;
    if(noDaChave->filhoEsq == null || noDaChave->filhoDir == null) // Nó sem ambos filhos
        noARemover = noDaChave;
    else
        noARemover = menorDosMaiores(noDaChave); // Caso tenha ambos filhos, pega mDm


    if(noARemover->filhoEsq != null)
        filhoDoNoRemovido = noARemover->filhoEsq;
    else
        filhoDoNoRemovido = noARemover->filhoDir;
    filhoDoNoRemovido->pai = paiDoNo(noARemover);


    if(paiDoNo(noARemover) == null)
        raiz = filhoDoNoRemovido;
    else if(noARemover == paiDoNo(noARemover)->filhoEsq)
        paiDoNo(noARemover)->filhoEsq = filhoDoNoRemovido;
    else
        paiDoNo(noARemover)->filhoDir = filhoDoNoRemovido;


    if(noDaChave != noARemover) // copia chave e dados do noARemover para noDaChave
        noDaChave->chave = noARemover->chave;
    if(noARemover == raiz){
        raiz = null;
        return raiz;
    }
    free(noARemover);
    if(noARemover->cor == Preto)
        consertaRemocao(raiz,filhoDoNoRemovido);
    return raiz;
}
//_______________________________________________________________________________





//________________________________________________________________________________
//        FUNCAO DESTROI ARVORE (MANTEM APENAS A RAIZ APONTANDO PARA null        |
//--------------------------------------------------------------------------------
void destroir(No *no){
    if(no == null){
        return;
    }
    destroir(no->filhoEsq);
    destroir(no->filhoDir);
    free(no);
}
//_______________________________________________________________________________




No *inserirChavesAleatorias(No *raiz, int quantidade){
    int i;
    int chave=0;
    for(i=0;i<quantidade;i++){
        chave = (int)(1 + rand()%100);
        raiz = inserir(raiz,chave);
        printf("%.2lf%%  -  Inserindo: %d\n",(double)(((double)i/(double)quantidade)*100), chave);
    }
    printf("100.00%%\n");
    return raiz;
}

//________________________________________________________________________________
//                           IMPRIMIR LOGO DO PROGRAMA                           |
//--------------------------------------------------------------------------------
void imprimirLogo(){
    printf("     /\\                             \n");
    printf("    /  \\   _ ____   _____  _ __ ___ \n");
    printf("   / /\\ \\ | '__\\ \\ / / _ \\| '__/ _ \\\n");
    printf("  / ____ \\| |   \\ V / (_) | | |  __/\n");
    printf(" /_/    \\_\\_|    \\_/ \\___/|_|  \\___|\n");

    printf("\n");

    printf("  _____       _                \n");
    printf(" |  __ \\     | |               \n");
    printf(" | |__) |   _| |__  _ __ ___   \n");
    printf(" |  _  / | | | '_ \\| '__/ _ \\  \n");
    printf(" | | \\ \\ |_| | |_) | | | (_) | \n");
    printf(" |_|  \\_\\__,_|_.__/|_|  \\___/  \n");

    printf("\n");

    printf("  _   _                       \n");
    printf(" | \\ | |                      \n");
    printf(" |  \\| | ___  __ _ _ __ __ _  \n");
    printf(" | . ` |/ _ \\/ _` | '__/ _` | \n");
    printf(" | |\\  |  __/ (_| | | | (_| | \n");
    printf(" |_| \\_|\\___|\\__, |_|  \\__,_| \n");
    printf("              __/ |           \n");
    printf("             |___/            \n");
    printf("\n\tBy: JOSIAS BEN F. CAVALCANTE - 21753594\n");

    system("pause");
}
//_______________________________________________________________________________




//________________________________________________________________________________
//                                  MENU INICIAL                                 |
//--------------------------------------------------------------------------------
int menuInicial(){
    int opcao;
    printf("___________________________________________________________________________\n");
    printf("|                   A R V O R E    R U B R O    N E G R A                 |\n");
    printf("---------------------------------------------------------------------------\n");
    printf("[1] Gerar e Inserir elementos Aleatorios (simulacao)\n");
    printf("[2] Inserir elemento\n");
    printf("[3] Remover elemento\n");
    printf("[4] Buscar elemento\n");
    printf("[5] Imprimir arvore\n");
    printf("[6] Esvaziar arvore\n");
    printf("[7] Sair\n");
    printf("   Por Favor, selecione uma opcao acima:\n");
    scanf("%d", &opcao);
    return opcao;
}
//_______________________________________________________________________________

int menuBusca(){
    int opcao;
    printf("___________________________________________________________________________\n");
    printf("|                       B U S C A R    E L E M E N T O                    |\n");
    printf("---------------------------------------------------------------------------\n");
    printf("\tPor Favor, selecione uma opcao de detalhamento abaixo:\n");
    printf("[1] Detalhamento Simples (chave e cor)\n");
    printf("[2] Detalhamento Completo (chave, cor e nos relacionais)\n");
    scanf("%d", &opcao);
    return opcao;
}


//________________________________________________________________________________
//                                MENU IMPRESSORAS                               |
//--------------------------------------------------------------------------------
int menuImpressoras(){
    int opcao;
    printf("___________________________________________________________________________\n");
    printf("|                      I M P R I M I R    A R V O R E                     |\n");
    printf("---------------------------------------------------------------------------\n");
    printf("\tPor Favor, selecione um metodo de impressao abaixo:\n");
    printf("[1] Pre-ordem SIMPLES\n");
    printf("[2] Em ordem SIMPLES\n");
    printf("[3] Pos-ordem SIMPLES \n");
    printf("[4] Pre-ordem COMPLETA\n");
    printf("[5] Em ordem COMPLETA\n");
    printf("[6] Pos-ordem COMPLETA\n");
    printf("\n[7] Voltar\n");
    scanf("%d", &opcao);
    return opcao;
}
//_______________________________________________________________________________


//________________________________________________________________________________
//                                  FUNCAO MAIN                                  |
//--------------------------------------------------------------------------------


int main(){
    srand(time(NULL));
    imprimirLogo();
    int opcao, opcao2, chave;
    No *aux;
    iniciarArvore();
    system("cls");
    opcao = menuInicial();
    while(opcao!=7){
        switch(opcao){
            case 1: //Inserir aleatorios
                printf("Chaves geradas aleatoriamente nao serao reinseridas em caso de repeticao.\nDigite o valor da quantidade de chaves (1~99): ");
                scanf("%d", &chave);
                system("cls");
                raiz = inserirChavesAleatorias(raiz, chave);
                printf("Chaves Aleatorias inseridas!\n");
                system("pause");
                system("cls");
            break;



            case 2: //Inserir
                printf("Digite o valor da chave: ");
                scanf("%d", &chave);
                aux = buscar(raiz,chave);
                if(aux==null){
                    raiz = inserir(raiz, chave);
                    system("cls");
                    printf("CHAVE INSERIDA!\n");
                    system("pause");
                    system("cls");
                }
                else{
                    system("cls");
                    printf("A CHAVE JA ESTA NA ARVORE\n");
                    system("pause");
                    system("cls");
                }
            break;





            case 3: //Remover
                if(raiz!=null){
                    printf("Digite o valor da chave: ");
                    scanf("%d", &chave);
                    aux = buscar(raiz, chave);
                    raiz = remover(raiz, aux);
                    system("cls");
                    if(aux != null)
                        printf("CHAVE REMOVIDA!\n");
                    else
                        printf("CHAVE NAO EXISTE\n");
                    system("pause");
                    system("cls");
                }
                else{
                    system("cls");
                    printf("ARVORE VAZIA!\n");
                    system("pause");
                    system("cls");
                }

            break;





            case 4: //Buscar
                system("cls");
                if(raiz!=null){
                    opcao2 = menuBusca();
                    printf("Digite o valor da chave: ");
                    scanf("%d", &chave);
                    aux = buscar(raiz, chave);
                    system("cls");
                    if(aux == null){
                        printf("CHAVE NAO ENCONTRADA\n");
                        system("pause");
                        system("cls");
                    }
                    else{
                        if(opcao2 == 1){
                            imprimeNo(aux);
                        }
                        else if(opcao2==2){
                            statusNo(aux);
                        }
                        system("pause");
                        system("cls");
                    }
                }
                else{
                    printf("ARVORE VAZIA!\n");
                    system("pause");
                    system("cls");
                }
            break;




            case 5: //Imprimir
                system("cls");
                if(raiz != null){
                    opcao = menuImpressoras();
                    system("cls");
                    if(opcao == 1){
                        impressaoSimplesPreOrdem(raiz);
                        system("pause");
                    }
                    else if(opcao == 2){
                        impressaoSimplesEmOrdem(raiz);
                        system("pause");
                    }
                    else if(opcao == 3){
                        impressaoSimplesPosOrdem(raiz);
                        system("pause");
                    }
                    else if(opcao == 4){
                        impressaoCompletaPreOrdem(raiz);
                        system("pause");
                    }
                    else if(opcao == 5){
                        impressaoCompletaEmOrdem(raiz);
                        system("pause");
                    }
                    else if(opcao == 6){
                        impressaoCompletaPosOrdem(raiz);
                        system("pause");
                    }
                }
                else{
                    printf("ARVORE VAZIA!\n");
                    system("pause");
                    }
                system("cls");
            break;




            case 6:
                system("cls");
                destroir(raiz);
                destroir(null);
                iniciarArvore();
                printf("ARVORE ESVAZIADA!\n");
                system("pause");
                system("cls");
            break;






            default:
            break;
        }
        opcao = menuInicial();
    }
    system("cls");
    printf("Obrigado por Utilizar a Arvore Rubro Negra!! :D\n");
    system("pause");
    system("cls");
    destroir(raiz);
    destroir(null);
    return(0);
}

//_______________________________________________________________________________


// FIM DO CÓDIGO. MUITO OBRIGADO.
