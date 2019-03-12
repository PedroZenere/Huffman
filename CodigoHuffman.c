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
int inserirOrdenado (TLista *pLista, TNo *x);
int removerPrimeiro (TLista *pLista);

//Função Arvore:
TNo* criarNo(TItem x);

//Funçoes Huffman:
TNo* MontaArvoreHuffman(TLista *pLista);
void ReorganizaLista(TLista *pLista, TNo *novo);
void ImprimirTabela(TNo *Raiz, long int totalOcorrencia, int tamanhoBits);
void TaxaCompressao(float somaOcorrencia, long int totalOcorrencia);
int BuscaCaracter(TNo *raiz, char caracter, TNo *pX);
void InserirArvoreBalanceada(TNo *raiz, TNo *novo);
TNo* MontaArvoreCaracter(FILE *arquivo);

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
	} else if(novo->NoCelula->item.frequencia > pLista->pPrimeiro->NoCelula->item.frequencia) {
		TCelula *pAux = pLista->pPrimeiro;
		while (pAux != NULL && pAux->NoCelula->item.frequencia < novo->NoCelula->item.frequencia) {
			pAux = pAux->pProx;
		} if(pAux == NULL) {
			pLista->pUltimo->pProx = novo;
			novo->pAnt = pLista->pUltimo;
			pLista->pUltimo = novo;
		} else {
			pAux = pAux->pAnt;
			novo->pProx = pAux->pProx;
			novo->pAnt = pAux;
			pAux->pProx = novo;
			novo->pProx->pAnt = novo;
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
	if(pAux->pProx == NULL){
		pLista->pPrimeiro = NULL;
	}else {
		pLista->pPrimeiro = pAux->pProx;
		pLista->pPrimeiro->pAnt = NULL;
		free (pAux);
	}
	
	return 1;
}

//----------------------------------//

TNo* criarNo(TItem x) {
	TNo* pAux = (TNo*) malloc(sizeof(TNo));
	pAux->item = x;
	pAux->pEsq= NULL;
	pAux->pDir= NULL;
	return pAux;
}

//----------------------------------//

void ReorganizaLista(TLista *pLista, TNo *novo){
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
		
		somaFrequencia = pPrimeiro->NoCelula->item.frequencia + pSegundo->NoCelula->item.frequencia;
		
		novo->item.frequencia = somaFrequencia;
		novo->pEsq = pPrimeiro->NoCelula;
		novo->pDir = pSegundo->NoCelula;
		
		ReorganizaLista(pLista, novo);
	}
	return pLista->pPrimeiro->NoCelula;
}

void PercorreArvore(TNo *p, char *binario, int nivel, long int totalCaracteres, double *totalHuffman){
	if(p == NULL)
		return;
	
	char simbolo = p->item.simbolo;
	float frequencia = p->item.frequencia;
	float ocorrencia = (frequencia / 100.0) * totalCaracteres;
	int bitsHuffman = ocorrencia * nivel;

	if(p->item.simbolo != '\0'){ //Se o simbolo for diferente de 'VAZIO'
		printf("\t|%10c|%6.1f|%16.0f|%12s|%14d|\n", simbolo, frequencia, ocorrencia, binario, bitsHuffman);
		*totalHuffman += bitsHuffman;
	}
	binario[nivel] = '0';
	binario[nivel+1] = '\0';
	PercorreArvore(p->pEsq, binario, nivel+1, totalCaracteres, totalHuffman);
	binario[nivel] = '1';
	binario[nivel+1] = '\0';
	PercorreArvore(p->pDir, binario, nivel+1, totalCaracteres, totalHuffman);
}

void ImprimirTabela(TNo *Raiz, long int totalOcorrencia, int tamanhoBits){
	char binario[tamanhoBits];
	int nivel = 0;
	double totalHuffman = 0;
	int frequencia = Raiz->item.frequencia;
	
	binario[0] = '0';
	
	printf("\n\n --------------------------- TABELA DE SIMBOLOS --------------------------- \n\n");
	printf("\t+----------+------+----------------+------------+--------------+\n");
	printf("\t| Caracter |  %%   | Nº Ocorrências |  Binario   | Bits Huffman |\n");
	printf("\t+----------+------+----------------+------------+--------------+\n");
	
	PercorreArvore(Raiz, binario, nivel, (totalOcorrencia/tamanhoBits), &totalHuffman);
	
	printf("\t+----------+------+----------------+------------+--------------+\n");
	printf("\t|  TOTAL   |%6d|%16ld|            |%14.0f|\n", frequencia ,(totalOcorrencia/tamanhoBits), totalHuffman);
	printf("\t+----------+------+----------------+------------+--------------+\n");	
	
	TaxaCompressao(totalHuffman, totalOcorrencia);
}

void TaxaCompressao(float somaOcorrencia, long int totalOcorrencia){
// somaOcorrencia: Recebe a soma total dos caracteres codificados
// totalOcorrencia: Recebe o total de caracteres que o texto contem no total

	float taxa = 0.0;
	
	taxa = (1-(somaOcorrencia/totalOcorrencia))*100;
	
	printf("\nTaxa de Compressao Aproximada: %f%%", taxa);
	
}

int BuscaCaracter(TNo *raiz, char caracter, TNo *pX){
	return 0;
}

void InserirArvoreBalanceada(TNo *raiz, TNo *novo){
	//Organizar a arvore em ordem alfabetica ou por frequencia?
	//Acredito que por frequencia vai ser mais eficiente
}

TNo* MontaArvoreCaracter(FILE *arquivo){ //Montar arvore Binaria dos Caracteres
	TNo *novo, *raiz, *pX;
	TItem item;
	char caracter;
	
	pX = NULL;
	
	caracter = fgetc(arquivo);
	item.simbolo = caracter;
	item.frequencia = 1;
	raiz = criarNo(item);
	
	while((caracter = fgetc(arquivo)) != EOF){
		if(BuscaCaracter(raiz, caracter, pX)){
			pX->item.frequencia++;
		} else {
			item.simbolo = caracter;
			item.frequencia = 1;
			novo = criarNo(item);
			InserirArvoreBalanceada(raiz, novo);
		}
	}
}

int main(int argc, char **argv)
{	
	//Criando a referencia da lista, e da arvore
	TLista lista;
	TItem item;
	TNo *raiz;
	int quantidade, i, tamanhoBits;
	long int totalOcorrencia;
	
	//iniciando a lista
	iniciarLista(&lista);
	//iniciando a arvore
	raiz = NULL;
	
	printf("--------------------------- CODIFICACAO DE HUFFMAN ---------------------------\n\n");
	printf("Insira a quantidade de simbolos: ");
	scanf("%d", &quantidade);
	printf("Insira os Simbolos e sua respectiva Ocorrencia:\n");
	for(i=0;i<quantidade;i++){
		scanf("\n%c", &item.simbolo);
		scanf("%f", &item.frequencia);
		//Lê um item, chama a função CriaNo, a mesma cria o Nó e manda para a função inserirOrdenado
		inserirOrdenado(&lista, criarNo(item));
	}

	printf("Insira a quantidade total de Ocorrencias: ");
	scanf(" %ld", &totalOcorrencia);
	printf("Insira a quantidade de espaço para armazenar em Bits: ");
	scanf("%d", &tamanhoBits);
	
	raiz = MontaArvore(&lista);
	ImprimirTabela(raiz, totalOcorrencia, tamanhoBits);
	
	return 0;
}

/*
TESTE:
a 48 c 9 ? 10 g 12 k 4 p 17 -> k 4, c 9, ? 10, g 12, p 17, a 48 

TESTE 2 (SLIDE):
INPUT: A 2 B 5 C 1 E 2 P 1 0 18 1 13 2 7 3 12 4 9 5 6 6 11 7 7 8 2 9 4
ORDENADO: (C,1) (P,1) (A,2) (E,2) (8,2) (9,4) (B,5) (5,6) (2,7) (7,7) (4,9) (6,11) (3,12) (1,13) (0,18)
CARACTER: 8 BITS
TOTAL OCORRENCIA: 4500000000 (4.5Gb)
TAXA DE COMPRESSAO: 55.63%

*/
