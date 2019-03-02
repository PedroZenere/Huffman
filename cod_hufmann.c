#include <stdio.h>
#include <stdlib.h>

typedef struct {
	char simbolo;
	float frequencia;
} TItem;

typedef struct No {
	TItem item;
	struct No *pEsq, *pDir;
} TNo;

typedef struct celula {
	struct celula *pProx;
	TItem item;
} TCelula;

typedef struct {
	TCelula *pPrimeiro, *pUltimo;
} TLista;

void iniciarLista (TLista *pLista);
int isVazia (TLista *pLista);
int inserirFim (TLista *pLista, TItem x);

void iniciarLista (TLista *pLista) {
	pLista->pPrimeiro = NULL;
	pLista->pUltimo = NULL;
}

int isVazia (TLista *pLista) {
	return pLista->pPrimeiro == NULL;
}

int inserirFim (TLista *pLista, TItem x) {
	TCelula *novo = (TCelula *) malloc (sizeof (TCelula));
	novo->item = x;
	novo->pProx = NULL;
	
	if (isVazia (pLista)) {
		pLista->pPrimeiro = novo;
	} else {
		pLista->pUltimo->pProx = novo;
	}
	pLista->pUltimo = novo;
	
	return 1;
}

TNo* criarNo(TItem x) {
	TNo* pAux = (TNo*) malloc(sizeof(TNo));
	pAux->item = x;
	pAux->pEsq= NULL;
	pAux->pDir= NULL;
	return pAux;
}
/*
TNo* inserirNo(TNo* pR, TItem x) {
	if(pR== NULL)
		pR= criarNo(x);
	else if(x.chave < pR->item.chave)
		pR->pEsq= inserirNo(pR->pEsq, x);
	else
		pR->pDir= inserirNo(pR->pDir, x);
	return pR;
}
*/

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

void insereLista(TLista *pLista, TItem *x, int i){
	int k;
	
	for(k=0;k<i;k++){
		inserirFim(pLista, x[k]);
	}
}

void imprimir (TLista *pLista) {
	TCelula *pAux = pLista->pPrimeiro;
	
	printf ("Itens na lista:\n");
	while (pAux != NULL) {
		printf ("%c %f\n", pAux->item.simbolo, pAux->item.frequencia);
		pAux = pAux->pProx;
	}
}

void soma(TLista *pLista){
	
	float sum;
		
	TCelula *pAux, *pAux2;
	pAux = pLista->pPrimeiro;
	pAux2 = pAux->pProx;
	
	sum = pAux->item.frequencia + pAux2->item.frequencia;
	
	printf("%f\n", sum);
	
	InsereArvore(sum, pAux, pAux2);
	
}

int main(int argc, char **argv)
{
	TItem item[5];
	TLista lista;
	iniciarLista (&lista);
	int i;
	
	printf("Preencha com os simbolos e a frequencia:\n");
	for(i=0;i<5;i++){
		scanf("\n%c", &item[i].simbolo);
		//printf("%c\n", item[i].simbolo);
		scanf("%f", &item[i].frequencia);
		//printf("%f\n", item[i].frequencia);
		//printf("%d\n", i);
	}
	
	selectionSort(item, i);
	insereLista(&lista, item, i);
	soma(&lista);
	
	/*
	for(i=0;i<5;i++){
		printf("%c %f ", item[i].simbolo, item[i].frequencia);
	}
	*/
	
	//imprimir(&lista);
	return 0;
}

/*
a 48 c 9 g 12 k 4 p 17

*/
