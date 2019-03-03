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
int removerPrimeiro (TLista *pLista);
int removerUltimo (TLista *pLista, TItem *pX);
void imprimir (TLista *pLista, int inverso);
TCelula* buscaLista (TLista *pLista, float frequencia);

//Funçoes Arvores:
TNo* criarNo(TItem x);
TNo* inserirNo(TNo* pR, TItem x);
void preOrdem(TNo *p);
void emOrdem(TNo*p);

//Funçoes Huffman:
TNo* MontaArvore(TLista *pLista);
void ReorganizaLista(TLista *pLista, TNo *novo);
void ImprimirArvore(TNo *Raiz);
void ImprimirTabela(TNo *Raiz, int totalOcorrencia);
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
		pLista->pUltimo = novo;
		
	} else {
		while(pCelula != NULL && achou == 0) {
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
				achou = 1;
			}
			pCelula = pCelula->pProx;
		}
	}
	
	return 1;
}

int removerPrimeiro (TLista *pLista) {
	if (isVazia (pLista)) {
		return 0;
	}
	TCelula *pAux;
	pAux = pLista->pPrimeiro;
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

//----------------------------------//

void ReorganizaLista(TLista *pLista, TNo *novo){ //Antigo Remove2Primeiros
	removerPrimeiro(pLista);
	removerPrimeiro(pLista);
	inserirOrdenado(pLista, novo);
}

TNo* MontaArvore(TLista *pLista){ //Retorna o nó raiz da arvore
	
	TCelula *pPrimeiro, *pSegundo;
	TNo *novo = (TNo*) malloc(sizeof(TNo));
	float somaFrequencia;
	
	
	while(pLista->pPrimeiro != pLista->pUltimo){ //Enquanto tiver mais de um elemento na lista
	
		pPrimeiro = pLista->pPrimeiro;
		pSegundo = pPrimeiro->pProx;
	
		somaFrequencia = pPrimeiro->NoCelula->item.frequencia + pSegundo->NoCelula->item.frequencia;
		
		printf("%f\n", somaFrequencia);
		
		novo->item.frequencia = somaFrequencia;
		novo->pEsq = pPrimeiro->NoCelula;
		novo->pDir = pSegundo->NoCelula;
		
		ReorganizaLista(pLista, novo);

	}
	return pLista->pPrimeiro->NoCelula;
}

void ImprimirArvore(TNo *Raiz){
//TODO
//Poderiamos chamar a Função em Ordem que percorre a arvore buscando sempre primeiro a Raiz
}

void PercorreArvore(TNo *p, int binario, int nivel, int totalOcorrencia){
	if(p == NULL)
		return;
	
	char simbolo = p->item.simbolo;
	int ocorrencia = p->item.frequencia * totalOcorrencia;
	int bitsHuffman = ocorrencia * nivel;
	
	if(p->item.simbolo != '\0'){ //Se o simbolo for diferente de 'VAZIO'
		printf("\t| %c | %d | %d | %d |\n", simbolo, ocorrencia, binario, bitsHuffman);
		PercorreArvore(p->pEsq, (binario*10), nivel+1, totalOcorrencia);
		PercorreArvore(p->pDir, (binario*10)+1, nivel+1, totalOcorrencia);
	} else {
		return;
	}
	
}

void ImprimirTabela(TNo *Raiz, int totalOcorrencia){
	int binario = 0;
	int nivel = 0;
	
	printf("Tabela: \n");
	printf("\t+----------+----------------+---------+--------------+\n");
	printf("\t| Caracter | Nº Ocorrências | Binario | Bits Huffman |\n");
	printf("\t+----------+----------------+---------+--------------+\n");
	PercorreArvore(Raiz, binario, nivel, totalOcorrencia);
	printf("\t+----------+----------------+---------+--------------+\n");

}

void TaxaCompressao(TNo *Raiz, int bits, int quantidade){
//TODO
}

int main(int argc, char **argv)
{	
	//Criando a referencia da lista, e o Tno Node
	TLista lista;
	TItem item;
	TNo *raiz;
	int quantidade, i, totalOcorrencia;
	
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
	printf("\nInsira a quntidade total de Ocorrencias: ");
	scanf(" %d", &totalOcorrencia);
	
	raiz = MontaArvore(&lista);
	
	ImprimirTabela(raiz, totalOcorrencia);
	
	return 0;
}

/*
TESTE:
a 48 c 9 g 12 k 4 p 17
*/
