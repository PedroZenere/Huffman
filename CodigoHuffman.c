#include <stdio.h>
#include <stdlib.h>

typedef struct {
	char simbolo;
	float frequencia;
} TItem;

typedef struct TNo {
	TItem item;
	struct TNo *pEsq, *pDir;
} TNo;

typedef struct celula {
	struct celula *pProx;
	struct celula *pAnt;
	TNo *NoCelula;

} TCelula;

typedef struct {
	TCelula *pPrimeiro, *pUltimo;
} TLista;

//Funçoes Lista:
void iniciarLista (TLista *pLista);
int isVazia (TLista *pLista);
int inserirFim (TLista *pLista, TItem x);
int removerPrimeiro (TLista *pLista, TItem *pX);
int removerUltimo (TLista *pLista, TItem *pX);
void imprimir (TLista *pLista, int inverso);
TCelula* buscaLista (TLista *pLista, int chave);

//Funçoes Arvores:
TNo* criarNo(TItem x);
TNo* inserirNo(TNo* pR, TItem x);
void selectionSort (TItem *v, int n);

//Funçoes Huffman:
void MontaArvore(TLista *pLista);
void InserirArvore(TLista *pLista, TNo *novo);
void ImprimirArvore(TNo *Raiz);
void ImprimirTabela(TNo *Raiz);
void TaxaCompressao(TNo *Raiz, int bits, int quantidade);

//----------------------------------//

void iniciarLista (TLista *pLista) {
	pLista->pPrimeiro = NULL;
	pLista->pUltimo = NULL;
}

int isVazia (TLista *pLista) {
	return pLista->pPrimeiro == NULL;
}

int inserirOrdenado (TLista *pLista, TNo *x) {
	TCelula *novo = (TCelula *) malloc (sizeof (TCelula));
	novo->NoCelula = x;
	novo->pProx = NULL;
	novo->pAnt = NULL;
	
	TCelula *pCelula; //Variavel auxiliar para o codigo ficar mais legivel
	TNo *pNo;		  //Variavel auxiliar para o codigo ficar mais legivel
	pCelula = pLista->pPrimeiro;
	
	if (isVazia (pLista)) {
		pLista->pPrimeiro = novo;
		
	} else {
		while(pCelula != NULL) {
			pNo = pCelula->NoCelula; //pNo recebe o No da Celula sendo visitada a cada iteração
			if(x->item.frequencia < pNo->item.frequencia) {
				if(pCelula == pLista->pPrimeiro) { //Insere no Inicio
					novo->pProx = pCelula;
					pCelula->pAnt = novo;
					pLista->pPrimeiro = novo;
				} else if(pCelula == pLista->pUltimo) { //Insere no Fim
					novo->pAnt = pCelula;
					pCelula->pProx = novo;
					pLista->pUltimo = novo;
				} else {
					pCelula->pAnt->pProx = novo; 
					novo->pAnt = pCelula->pAnt;
					pCelula->pAnt = novo;
					novo->pProx = pCelula;
				}
			}
			pCelula = pCelula->pProx;
		}
		
	}
	
	return 1;
}

int removerPrimeiro (TLista *pLista, TItem *pX) {
	if (isVazia (pLista))
		return 0;
	TCelula *pAux;
	pAux = pLista->pPrimeiro;
	*pX = pAux->item;
	pLista->pPrimeiro = pAux->pProx;
	pLista->pPrimeiro->pAnt = NULL;
	free (pAux);
	return 1;
}
	
void imprimir (TLista *pLista, int inverso) {
	TCelula *celula;
	printf("Itens da lista: ");
	
	if(inverso){
		celula = pLista->pUltimo;
	}else{
		celula = pLista->pPrimeiro;
	}
	
	while (celula != NULL) {
		printf ("%d ", celula->item.chave);
		if(inverso){
		celula = celula->pAnt;
		}else{
			celula = celula->pProx;
		}
	}
	printf("\n");
}

TCelula* buscaLista (TLista *pLista, int chave){
	TCelula *pAux = pLista->pPrimeiro;
	while (pAux != NULL) {
		if (pAux->item.chave == chave){
			return pAux;
		}
		pAux = pAux->pProx;
	}
	return NULL;
	
}

//----------------------------------//

TNo* criarNo(TItem x) {
	TNo* pAux = (TNo*) malloc(sizeof(TNo));
	pAux->item = x;
	pAux->pEsq= NULL;
	pAux->pDir= NULL;
	return pAux;
}

TNo* inserirNo(TNo* pR, TItem x) {
	if(pR== NULL)
		pR= criarNo(x);
	else if(pR->pEsq == NULL)
		pR->pEsq= inserirNo(pR->pEsq, x);
	else
		pR->pDir= inserirNo(pR->pDir, x);
	return pR;
}

//----------------------------------//

//AINDA UTIL ESSA FUNCAO?(selectionSort) SE NAO, PODE APAGAR, E PUSH.
void selectionSort (TItem *v, int n) {
	int i, j, min;
	TItem aux;
	
	for (i = 0; i < n-1; i++) {
		min = i;
		for (j = i+1; j < n; j++) {
			if (v[j].frequencia < v[min].frequencia) {
				min = j;
			}
		}
		
		if (i != min) {
			aux = v[min];
			v[min] = v[i];
			v[i] = aux;
		}
	}
}

void InserirArvore(TLista plista, TNo *novo){
	TNo *removido;
	removerPrimeiro(pLista, removido);
	printf("Item removido");
	removerPrimeiro(pLista, removido);
	printf("Item removido");
	InsereOrdenado(pLista, novo);
}

void MontaArvore(TLista *pLista){ //Antiga funcao Soma
	
	float sum;
		
	TCelula *pAux, *pAux2;
	pAux = pLista->pPrimeiro;
	pAux2 = pAux->pProx;
	
	sum = pAux->item.frequencia + pAux2->item.frequencia;
	
	printf("%f\n", sum);
	
	InsereArvore(sum, pAux, pAux2);
}

void ImprimirArvore(TNo *Raiz){
//TODO
}

void ImprimirTabela(TNo *Raiz){
//TODO
}

void TaxaCompressao(TNo *Raiz, int bits, int quantidade){
//TODO
}

int main(int argc, char **argv)
{
	TLista lista;
	TItem item;
	int i, j;
	
	iniciarLista(&lista);
	
	printf("Insira quantos elementos deseja:\n");
	scanf("%d", &j);
	printf("Preencha com os simbolos e a frequencia:\n");
	for(i=0;i<j;i++){
		scanf("\n%c", &item[i].simbolo);
		scanf("%f", &item[i].frequencia);
		inserirFim(&lista, item[i]);
	}
	
	selectionSort(item, i);
	
	for(i=0;i<5;i++){
		printf("%c %f ", item[i].simbolo, item[i].frequencia);
	}
	
	return 0;
}

/*
a 48 c 9 g 12 k 4 p 17

*/
