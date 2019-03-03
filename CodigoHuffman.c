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
int removerPrimeiro (TLista *pLista, TNo *pX);
int removerUltimo (TLista *pLista, TItem *pX);
void imprimir (TLista *pLista, int inverso);
TCelula* buscaLista (TLista *pLista, float frequencia);

//Funçoes Arvores:
TNo* criarNo(TItem x);
TNo* inserirNo(TNo* pR, TItem x);
void preOrdem(TNo *p);
void emOrdem(TNo*p);

//Funçoes Huffman:
void MontaArvore(TLista *pLista);
void Remove2Primeiros(TLista *pLista, TNo *novo);
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
	int achou;
	TCelula *novo = (TCelula *) malloc (sizeof (TCelula));
	novo->NoCelula = x;
	novo->pProx = NULL;
	novo->pAnt = NULL;
	
	TCelula *pCelula; //Variavel auxiliar para o codigo ficar mais legivel
	TNo *pNo;		  //Variavel auxiliar para o codigo ficar mais legivel
	pCelula = pLista->pPrimeiro;
	achou = 0;
	if (isVazia (pLista)) {
		pLista->pPrimeiro = novo;
		pLista->pUltimo = novo; //Faltou inicilizar o primeiro elemento como sendo primeiro e o ultimo da lista
		
	} else {
		while(pCelula != NULL && achou == 0) {
			pNo = pCelula->NoCelula; //pNo recebe o No da Celula sendo visitada a cada iteração
			if(x->item.frequencia < pNo->item.frequencia) {
				if(pCelula == pLista->pPrimeiro) { //Insere no Inicio
					novo->pProx = pCelula;
					pCelula->pAnt = novo;
					pLista->pPrimeiro = novo;
					achou = 1;
				} else if(pCelula == pLista->pUltimo) { //Insere no Fim
					novo->pAnt = pCelula;
					pCelula->pProx = novo;
					pLista->pUltimo = novo;
					achou = 1;
				} else {
					pCelula->pAnt->pProx = novo; 
					novo->pAnt = pCelula->pAnt;
					pCelula->pAnt = novo;
					novo->pProx = pCelula;
					achou = 1;
				}
			}
			pCelula = pCelula->pProx;
		}
		
	}
	
	return 1;
}

int removerPrimeiro (TLista *pLista, TNo *pX) {
	if (isVazia (pLista)) {
		return 0;
	}
	TCelula *pAux;
	pAux = pLista->pPrimeiro;
	pX = pAux->NoCelula;
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
		printf ("%f ", celula->NoCelula->item.frequencia);
		if(inverso){
		celula = celula->pAnt;
		}else{
			celula = celula->pProx;
		}
	}
	printf("\n");
}

TCelula* buscaLista (TLista *pLista, float frequencia){
	TCelula *pAux = pLista->pPrimeiro;
	while (pAux != NULL) {
		if (pAux->NoCelula->item.frequencia == frequencia){
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

void Remove2Primeiros(TLista *pLista, TNo *novo){
	TNo *removido = NULL;
	removerPrimeiro(pLista, removido);
	printf("Item removido (%c , %2.f)\n", removido->item.simbolo, removido->item.frequencia);
	removerPrimeiro(pLista, removido);
	printf("Item removido (%c , %2.f)\n", removido->item.simbolo, removido->item.frequencia);
	inserirOrdenado(pLista, novo);
}

void MontaArvore(TLista *pLista){ //Antiga funcao Soma
	
	TCelula *pAux, *pAux2;
	TNo *novo = (TNo*) malloc(sizeof(TNo));
	float somaFrequencia;
	
	pAux = pLista->pPrimeiro;
	pAux2 = pAux->pProx;
	
	somaFrequencia = pAux->NoCelula->item.frequencia + pAux2->NoCelula->item.frequencia;
	
	printf("%f\n", somaFrequencia);
	
	novo->item.frequencia = somaFrequencia;
	novo->pEsq = pAux->NoCelula;
	novo->pDir = pAux2->NoCelula;
	
	Remove2Primeiros(pLista, novo);
}

void preOrdem(TNo *p) {
	if (p == NULL) 
		return;
	printf("%c\n", p->item.simbolo);
	preOrdem(p->pEsq);
	preOrdem(p->pDir);
}

void emOrdem(TNo*p) {
	if (p == NULL) 
		return;
	preOrdem(p->pEsq);
	printf("%c\n", p->item.simbolo);
	preOrdem(p->pDir);
}

void ImprimirArvore(TNo *Raiz){
//TODO
//Poderiamos chamar a Função em Ordem que percorre a arvore buscando sempre primeiro a Raiz
}

void ImprimirTabela(TNo *Raiz){
//TODO
}

void TaxaCompressao(TNo *Raiz, int bits, int quantidade){
//TODO
}

int main(int argc, char **argv)
{	
	//Criando a referencia da lista, e o Tno Node
	TLista lista;
	TItem item;
	int quantidade, i;
	
	//iniciando a lista
	iniciarLista(&lista);
	printf("Insira a quantidade de simbolos:\n");
	scanf("%d", &quantidade);
	for(i=0;i<quantidade;i++){
		scanf("\n%c", &item.simbolo);
		scanf("%f", &item.frequencia);
		//Lê um item, chama a função CriaNo, a mesma cria o Nó e manda para a função inserirOrdenado
		inserirOrdenado(&lista, criarNo(item));
	}

	return 0;
}

/*
TESTE:
a 48 c 9 g 12 k 4 p 17
*/
