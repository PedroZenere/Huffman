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

//Funções para Manipulação da Lista:
int imprimir (TLista *pLista, int inverso);
void iniciarLista (TLista *pLista);
int isVazia (TLista *pLista);
int inserirOrdenado (TLista *pLista, TNo *x);
int removerPrimeiro (TLista *pLista);

//Função para Manipulação da Arvore AVL:
TNo* criarNo(TItem x);
int altura (TNo* pRaiz);
int fb (TNo* pRaiz);
TNo* rotacaoSimplesDireita(TNo* pR);
TNo* rotacaoSimplesEsquerda (TNo* pR);
TNo* balancaEsquerda(TNo* pR);
TNo* balancaDireita(TNo* pR);
TNo* balanceamento(TNo* pR);
void eAVL(TNo* pR);
void preOrdem(TNo *p);

//Funções de Leitura e Contabilização de todos Caracteres do Arquivo:
TNo* MontaArvoreCaracter(FILE *arquivo, long unsigned int *totalCaracteres);
int BuscaCaracter(TNo *pNo, char caracter, TNo *pX);
TNo* InserirArvoreBalanceada(TNo *raiz, TNo *novo);

//Função para Inserir todos os nós de caracteres na lista:
int MontaLista(TLista *pLista, TNo *pNo,long unsigned int totalCaracteres);

//Funçoes Para Criação e Manipulação da Árvore de Huffman:
TNo* MontaArvoreHuffman(TLista *pLista);
void ReorganizaLista(TLista *pLista, TNo *novo);
void ImprimirTabela(TNo *Raiz, long int totalOcorrencia, int tamanhoBits);
void TaxaCompressao(float somaOcorrencia, long unsigned int totalOcorrencia);

//Funções para Liberação de Memória
void LiberaLista(TLista *pLista);
TNo* LiberaArvore(TNo *pNo);


/* ----------- Funções para Manipulação da Lista -------------- */

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

void iniciarLista (TLista *pLista) {
	pLista->pPrimeiro = NULL;
	pLista->pUltimo = NULL;
}

int isVazia (TLista *pLista) {
	return pLista->pPrimeiro == NULL;
}

int inserirOrdenado (TLista *pLista, TNo *x) {
	TCelula *novo = (TCelula *) malloc (sizeof (TCelula));
	TCelula *pAux = NULL;
	
	novo->NoCelula = x;
	novo->pProx = NULL;
	novo->pAnt = NULL;

	if (isVazia (pLista)) {
		pLista->pPrimeiro = novo;
		pLista->pUltimo = novo;
	} else if(novo->NoCelula->item.frequencia <= pLista->pPrimeiro->NoCelula->item.frequencia){
		pAux = pLista->pPrimeiro;
		pLista->pPrimeiro = novo;
		pLista->pPrimeiro->pProx = pAux;
		pAux->pAnt = pLista->pPrimeiro;
		pLista->pPrimeiro->pAnt = NULL;
	} else if(novo->NoCelula->item.frequencia > pLista->pPrimeiro->NoCelula->item.frequencia) {
		
		pAux = pLista->pPrimeiro;
		
		while (pAux != NULL && pAux->NoCelula->item.frequencia < novo->NoCelula->item.frequencia) {
			pAux = pAux->pProx;
		} 
		
		if(pAux == NULL) {
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
	
	imprimir(pLista, 0);
	
	return 1;
}

int removerPrimeiro (TLista *pLista) {
	if (isVazia (pLista)) {
		return 0;
	}
	TCelula *pAux = NULL;
	pAux = pLista->pPrimeiro;
	if(pAux->pProx == NULL){
		pLista->pPrimeiro = NULL;
	}else {
		pLista->pPrimeiro = pAux->pProx;
		pLista->pPrimeiro->pAnt = NULL;
	}
	free(pAux);
	
	return 1;
}

/* ----------- Função para Manipulação da Arvore AVL -------------- */

TNo* criarNo(TItem x) {
	TNo* pAux = (TNo*) malloc(sizeof(TNo));
	pAux->item = x;
	pAux->pEsq= NULL;
	pAux->pDir= NULL;
	return pAux;
}

int altura (TNo* pRaiz) { 
	int iEsq, iDir; 
	if (pRaiz == NULL) 
		return -1; 
		
	iEsq = altura (pRaiz->pEsq);
	iDir = altura (pRaiz->pDir); 
	
	if (iEsq > iDir) 
		return iEsq + 1; 
	else 
		return iDir + 1; 
}

int fb (TNo* pRaiz) {
	if (pRaiz == NULL)
		return 0;
		
	return altura(pRaiz->pEsq)-altura(pRaiz->pDir);
}

TNo* rotacaoSimplesDireita(TNo* pR) {
	TNo*pAux = pR->pEsq;
	pR->pEsq = pAux->pDir;
	pAux->pDir = pR;
	return pAux;
}

TNo* rotacaoSimplesEsquerda (TNo* pR) {
	TNo *pAux = pR->pDir;
	pR->pDir = pAux->pEsq;
	pAux->pEsq = pR;
	return pAux;
}

TNo* balancaEsquerda(TNo* pR) {
	int gb;
	gb = fb(pR->pEsq);
	if(gb >= 0) {
		pR= rotacaoSimplesDireita(pR);
	} 
	else{
		pR->pEsq = rotacaoSimplesEsquerda(pR->pEsq);
		pR = rotacaoSimplesDireita(pR);
	}
	
	return pR;
}

TNo* balancaDireita(TNo* pR) { 
	int gb = fb(pR->pDir); 
	if(gb <= 0) { 
		pR = rotacaoSimplesEsquerda(pR); 
	} 
	else{ 
		pR->pDir = rotacaoSimplesDireita(pR->pDir);
		pR = rotacaoSimplesEsquerda(pR); 
	} 
	
	return pR;
}

TNo* balanceamento(TNo* pR) {
	int gb = fb(pR);
	if (gb > 1)
		pR = balancaEsquerda(pR);
	else if (gb < -1)
		pR = balancaDireita(pR);
		
	return pR;
}

void eAVL(TNo* pR){
	int gb = fb(pR);
	
	if((gb < -1) || (gb > 1)){
		printf("Arvore não AVL\n");
	}
	else{
		printf("Arvore AVL\n");
	}
	
}

void preOrdem(TNo *p) {
	if (p == NULL) 
		return;
	printf("Caracter: %c Frequencia: %f\n", p->item.simbolo, p->item.frequencia);
	preOrdem(p->pEsq);
	preOrdem(p->pDir);
}



/* ----------- Funções de Leitura e Contabilização de todos Caracteres do Arquivo ----------- */

TNo* MontaArvoreCaracter(FILE *arquivo, long unsigned int *totalCaracteres){ //Montar arvore Binaria dos Caracteres
	TNo *novo, *raiz, pX;
	TItem item;
	char caracter;
	
	caracter = fgetc(arquivo);
	item.simbolo = caracter;
	item.frequencia = 1;
	raiz = criarNo(item);

	while((caracter = fgetc(arquivo)) != EOF){
		if(caracter != '\n'){
			if(BuscaCaracter(raiz, caracter, &pX)){
				
				pX.item.frequencia++;
			} else {
				item.simbolo = caracter;
				item.frequencia = 1;
				novo = criarNo(item);
				
				InserirArvoreBalanceada(raiz, novo);
				raiz = balanceamento(raiz); //balanceia a arvore
			}
		}
		*totalCaracteres += 1;
	}
	
	//eAVL(raiz); //verifica se o balanceamento deu certo
	
	return raiz;
}

int BuscaCaracter(TNo *pNo, char caracter, TNo *pX){
	if(pNo == NULL){
		return 0;
	} else if(pNo->item.simbolo == caracter) {
		pNo->item.frequencia++;
		return 1;
	} else {
		if(caracter < pNo->item.simbolo) {
			return BuscaCaracter(pNo->pEsq, caracter, pX);
		}
		
		if(caracter > pNo->item.simbolo) {
			return BuscaCaracter(pNo->pDir, caracter, pX);
		}
	}
	return 0;
}

TNo* InserirArvoreBalanceada(TNo *raiz, TNo *novo){
	//Organizar a arvore em ordem alfabetica ou por frequencia?
	//Acredito que por frequencia vai ser mais eficiente
	//printf("Entrou\n");
	
	if(raiz == NULL)
		raiz = novo;
	else if(novo->item.simbolo < raiz->item.simbolo)
		raiz->pEsq = InserirArvoreBalanceada(raiz->pEsq, novo);
	else
		raiz->pDir = InserirArvoreBalanceada(raiz->pDir, novo);
	return raiz;
	
}


/* ----------- Função para Inserir todos os nós de caracteres na lista ----------- */

int MontaLista(TLista *pLista, TNo *pNo, long unsigned int totalCaracteres){ //Percorre Arvore de Caracter e inserer os nós na lista de Nó
	if(pNo == NULL){
		return 0;
	} else {
		pNo->item.frequencia = (pNo->item.frequencia / totalCaracteres);
		
		inserirOrdenado(pLista, pNo);
		
		MontaLista(pLista, pNo->pEsq, totalCaracteres);
		pNo->pEsq = NULL;
		
		MontaLista(pLista, pNo->pDir, totalCaracteres);
		pNo->pDir = NULL;
	}
	return 1;
}


/* ----------- Funçoes Para Criação e Manipulação da Árvore de Huffman ----------- */

void ReorganizaLista(TLista *pLista, TNo *novo){
	removerPrimeiro(pLista);
	removerPrimeiro(pLista);
	inserirOrdenado(pLista, novo);
}

TNo* MontaArvoreHuffman(TLista *pLista){ //Retorna o nó raiz da arvore
	TCelula *pPrimeiro, *pSegundo;
	TItem item;
	TNo *novo = NULL;
	float somaFrequencia;
	
	while(pLista->pPrimeiro != pLista->pUltimo){ //Enquanto tiver mais de um elemento na lista
		
		pPrimeiro = pLista->pPrimeiro;
		pSegundo = pPrimeiro->pProx;
		
		somaFrequencia = pPrimeiro->NoCelula->item.frequencia + pSegundo->NoCelula->item.frequencia;
		
		item.simbolo = '\0';
		item.frequencia = somaFrequencia;
		
		novo = criarNo(item);
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
	float ocorrencia = frequencia * totalCaracteres;
	int bitsHuffman = ocorrencia * nivel;

	if(p->item.simbolo != '\0'){ //Se o simbolo for diferente de 'VAZIO'
		printf("\t|%10c|%10.2f|%16.0f|%12s|%14d|\n", simbolo, frequencia*100, ocorrencia, binario, bitsHuffman);
		*totalHuffman += bitsHuffman;
	}
	binario[nivel] = '0';
	binario[nivel+1] = '\0';
	PercorreArvore(p->pEsq, binario, nivel+1, totalCaracteres, totalHuffman);
	binario[nivel] = '1';
	binario[nivel+1] = '\0';
	PercorreArvore(p->pDir, binario, nivel+1, totalCaracteres, totalHuffman);
}

void ImprimirTabela(TNo *Raiz, long int totalCaracteres, int tamanhoBits){
	char binario[tamanhoBits];
	int nivel = 0;
	double totalHuffman = 0;
	int frequencia = Raiz->item.frequencia;
	
	binario[0] = '0';
	
	printf("\n\n ------------------------------ TABELA DE SIMBOLOS ------------------------------\n\n");
	printf("\t+----------+----------+----------------+------------+--------------+\n");
	printf("\t| Caracter |      %%   | Nº Caracteres  |  Binario   | Bits Huffman |\n");
	printf("\t+----------+----------+----------------+------------+--------------+\n");
	
	PercorreArvore(Raiz, binario, nivel, totalCaracteres, &totalHuffman);
	
	printf("\t+----------+----------+----------------+------------+--------------+\n");
	printf("\t|  TOTAL   |%10d|%16ld|            |%14.0f|\n", frequencia , totalCaracteres, totalHuffman);
	printf("\t+----------+----------+----------------+------------+--------------+\n");	
	
	TaxaCompressao(totalHuffman, totalCaracteres * tamanhoBits);
}

void TaxaCompressao(float somaOcorrencia, long unsigned int totalOcorrencia){
// somaOcorrencia: Recebe a soma total dos caracteres codificados
// totalOcorrencia: Recebe o total de caracteres que o texto contem no total

	float taxa = 0.0;
	
	taxa = (1-(somaOcorrencia/totalOcorrencia))*100;
	
	printf("\n\tTaxa de Compressao Aproximada: %f%%\n", taxa);
	
}



/* ----------- Funções para Liberação de Memória ----------- */

void LiberaLista(TLista *pLista){
	TCelula *aux, *apaga;
	
	apaga = pLista->pPrimeiro;
	aux = apaga->pProx;
	
	while(aux != NULL){
		free(apaga);
		apaga = aux;
		aux = aux->pProx;
	}
	pLista->pPrimeiro = NULL;
	pLista->pUltimo = NULL;
}

TNo* LiberaArvore(TNo *pNo){
	if(pNo == NULL){
		return NULL;
	} else {
		printf("Caracter: %c Frequencia: %f\n", pNo->item.simbolo, pNo->item.frequencia);
		LiberaArvore(pNo->pEsq);
		free(pNo->pEsq);
		pNo->pEsq = NULL;
		
		
		LiberaArvore(pNo->pDir);
		free(pNo->pDir);
		pNo->pDir = NULL;
	}
	return pNo;
}



/* ----------- MAIN ----------- */

int main(int argc, char **argv)
{	
	FILE *arquivo;		//Criando a referencia do Arquivo
	TLista lista;		//Criando a referencia da lista
	TNo *raizHuffman;	//Criando a referencia da arvore de Huffman
	TNo *raizCaracter;	//Criando a referencia da arvore de Caracteres
	int tamanhoBits;
	long unsigned int totalCaracteres = 0;
	
	iniciarLista(&lista);	//iniciando a lista
	raizCaracter = NULL;	//iniciando a arvore
	
	printf("------------------------------ CODIFICACAO DE HUFFMAN ------------------------------\n\n");
	printf("Insira a quantidade de espaço para armazenar em Bits: ");
	scanf("%d", &tamanhoBits);
	arquivo = fopen("dados.txt", "r");
	
	if(arquivo == NULL){
		printf("Erro ao abrir arquivo!\n");
		return 0;
	}
	
	raizCaracter = MontaArvoreCaracter(arquivo, &totalCaracteres);	//Montando a Arvore com todos Caracteres do arquivo
	
	MontaLista(&lista, raizCaracter, totalCaracteres); 				//Montando a Lista de nós ordenado para criação da Árvore de Huffman
	
	raizHuffman = MontaArvoreHuffman(&lista); 						//Montando a Árvore de Huffman
	
	ImprimirTabela(raizHuffman, totalCaracteres, tamanhoBits); 		//Imprimindo os Resultados na Tabela

	//printf("Lista : %c %f \n", lista.pPrimeiro->NoCelula->item.simbolo, lista.pPrimeiro->NoCelula->item.frequencia);
	//printf("Lista : %c %f \n", raizCaracter->item.simbolo, raizCaracter->item.frequencia);

	LiberaLista(&lista);
	raizCaracter = LiberaArvore(raizCaracter);
	raizHuffman = LiberaArvore(raizHuffman);

	//free(raizCaracter);
	free(raizHuffman);
	
	fclose(arquivo);

	return 1;
}
