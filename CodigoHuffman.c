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
int imprimir (TLista *pLista, int inverso);
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
void TaxaCompressao(int somaOcorrencia, int totalOcorrencia, int tamanhoBits);

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
	printf("ITEM a ser Inserido: %f\n", novo->NoCelula->item.frequencia);

	if (isVazia (pLista)) {
		pLista->pPrimeiro = novo;
		pLista->pUltimo = novo;
	} else if(novo->NoCelula->item.frequencia <= pLista->pPrimeiro->NoCelula->item.frequencia){
		TCelula *pAux;
		pAux = pLista->pPrimeiro;
		pLista->pPrimeiro = novo;
		pLista->pPrimeiro->pProx = pAux;
		pAux->pAnt = pLista->pPrimeiro;
		pLista->pPrimeiro->pAnt = NULL;
		printf("1\n");
	} else if(novo->NoCelula->item.frequencia > pLista->pPrimeiro->NoCelula->item.frequencia) {
		TCelula *pAux = pLista->pPrimeiro;
		while (pAux != NULL && pAux->NoCelula->item.frequencia < novo->NoCelula->item.frequencia) {
			pAux = pAux->pProx;
		} if(pAux == NULL) {
			//quer dizer que é a ultima posição
			pLista->pUltimo->pProx = novo;
			novo->pAnt = pLista->pUltimo;
			pLista->pUltimo = novo;
			printf("Entrou aqui\n");
		} else {
			pAux = pAux->pAnt;
			printf("pAux: %f\n", pAux->NoCelula->item.frequencia);
			novo->pProx = pAux->pProx;
			novo->pAnt = pAux;
			pAux->pProx = novo;
			novo->pProx->pAnt = novo;
			printf("Insere Meio\n");
		}
	}
	
	imprimir(pLista, 0);
	
	return 1;
}

int removerPrimeiro (TLista *pLista) {
	if (isVazia (pLista)) {
		return 0;
	}
	TCelula *pAux;
	pAux = pLista->pPrimeiro;
	printf("\nPRIMEIRO DA LISTA: %f\n", pAux->NoCelula->item.frequencia);
	if(pAux->pProx == NULL){
		pLista->pPrimeiro = NULL;
	}else {
		pLista->pPrimeiro = pAux->pProx;
		pLista->pPrimeiro->pAnt = NULL;
		printf("ITEM REMOVIDO: %f\n", pAux->NoCelula->item.frequencia);
		//printf("ELEMENTO: %f\n", pLista->pPrimeiro->NoCelula->item.frequencia);
		free (pAux);
		printf("Lista após remover primeiro:\n");
		imprimir(pLista, 0);
	}
	
	return 1;
}
	
int imprimir (TLista *pLista, int inverso) {
	TCelula *celula;
	printf("Itens da lista: ");
	if((isVazia(pLista))){
		printf("Lista Vazia\n");
		return 0;
	}
	
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
	printf("\n\n");
	return 0;
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
	TNo *novo = NULL;
	float somaFrequencia;
	
	while(pLista->pPrimeiro != pLista->pUltimo){ //Enquanto tiver mais de um elemento na lista
		novo = (TNo*) malloc(sizeof(TNo));
		TCelula *pPrimeiro, *pSegundo;
		
		pPrimeiro = pLista->pPrimeiro;
		pSegundo = pPrimeiro->pProx;
		
		printf("PR: %f\n", pPrimeiro->NoCelula->item.frequencia);
		printf("SEG: %f\n", pSegundo->NoCelula->item.frequencia);
	
		somaFrequencia = pPrimeiro->NoCelula->item.frequencia + pSegundo->NoCelula->item.frequencia;
		
		printf("Frequencia: %f\n", somaFrequencia);
		
		novo->item.frequencia = somaFrequencia;
		
		//inserirNo(novo, pPrimeiro->NoCelula->item); //Isso aqui vai funcionar agora tambem!!
		//inserirNo(novo, pSegundo->NoCelula->item);
		novo->pEsq = pPrimeiro->NoCelula;
		printf("No Esquerdo: %f      ", novo->pEsq->item.frequencia);
		novo->pDir = pSegundo->NoCelula;
		printf("No Direito: %f      ", novo->pDir->item.frequencia);
		printf("\n");
		
		printf("IMPRIME NO MONTA\n");
		imprimir(pLista, 0);
		
		ReorganizaLista(pLista, novo);

	}
	
	//printf("Imprime após o break:\n");
	//imprimir(pLista, 0);
	
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
	int ocorrencia = (p->item.frequencia / 100) * totalOcorrencia;
	int bitsHuffman = ocorrencia * nivel;

	if(p->item.simbolo != '\0'){ //Se o simbolo for diferente de 'VAZIO'
		printf("\t|%10c|%16d|%12d|%14d|\n", simbolo, ocorrencia, binario, bitsHuffman);
		PercorreArvore(p->pEsq, (binario*10), nivel+1, totalOcorrencia);
		PercorreArvore(p->pDir, (binario*10)+1, nivel+1, totalOcorrencia);
	} else {
		PercorreArvore(p->pEsq, (binario*10), nivel+1, totalOcorrencia);
		PercorreArvore(p->pDir, (binario*10)+1, nivel+1, totalOcorrencia);
	}
	
}

void ImprimirTabela(TNo *Raiz, int totalOcorrencia){
	int binario = 0;
	int nivel = 0;
	
	printf("Tabela: \n");
	printf("\t+----------+----------------+------------+--------------+\n");
	printf("\t| Caracter | Nº Ocorrências |  Binario   | Bits Huffman |\n");
	printf("\t+----------+----------------+------------+--------------+\n");
	PercorreArvore(Raiz, binario, nivel, totalOcorrencia);
	printf("\t+----------+----------------+------------+--------------+\n");

}

void TaxaCompressao(int somaOcorrencia, int totalOcorrencia, int tamanhoBits){
// somaOcorrencia: Recebe a soma total dos caracteres codificados
// totalOcorrencia: Recebe o total de caracteres que o texto contem no total
// tamanhoBits: Recebe o tamanho fixo em bits para cada caracter
	float taxa = 0.0;
	
	taxa = ((1 - (somaOcorrencia/(totalOcorrencia*tamanhoBits)))*100);
	
	printf("\nTaxa de Compressao aproximada: %f\n", taxa);
	

}

int main(int argc, char **argv)
{	
	//Criando a referencia da lista, e o Tno Node
	TLista lista;
	TItem item;
	TNo *raiz;
	int quantidade, i, totalOcorrencia, tamanhoBits;
	raiz = NULL;
	
	//iniciando a lista
	iniciarLista(&lista);
	printf("Insira a quantidade de simbolos:\n");
	scanf("%d", &quantidade);
	printf("\nInsira os Simbolos e sua respectiva Ocorrencia:\n");
	for(i=0;i<quantidade;i++){
		scanf("\n%c", &item.simbolo);
		scanf("%f", &item.frequencia);
		//Lê um item, chama a função CriaNo, a mesma cria o Nó e manda para a função inserirOrdenado
		inserirOrdenado(&lista, criarNo(item));
	}

	printf("\nInsira a quantidade total de Ocorrencias: ");
	scanf(" %d", &totalOcorrencia);
	//printf("\nDa Main:\n");
	//imprimir(&lista, 0);
	/*
	printf("\nInsira a quantidade de espaço para armazenar em Bits:");
	scanf("%d", &tamanhoBits);
	*/

	raiz = MontaArvore(&lista);
	
	ImprimirTabela(raiz, totalOcorrencia);
	
	return 0;
}

/*
TESTE:
a 48 c 9 ? 10 g 12 k 4 p 17 -> k 4, c 9, ? 10, g 12, p 17, a 48 

*/
