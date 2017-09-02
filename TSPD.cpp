/* --------------------------------------------------------------- */
/*                                                                 */
/*                            _       _                            */
/*                           |_| | | |                             */
/*                           |   |_| |_                            */
/*                      ====================                       */
/*                                                                 */
/*                                                                 */
/* --------------------------------------------------------------- */
/*        PUC-MG, Unidade COREL. Disciplina de PAA		           */
/*               Graduação em Ciência da Computação                */
/*         2015 Coração Eucarístico, Belo Horizonte - MG           */
/*                     http://www.pucminas.br/                     */
/*                                                                 */
/*        Author: Rodolfo Herman;                                  */
/*                                                                 */
/*                                                                 */
/* --------------------------------------------------------------- */

/* --------------------------------------------------------------- */
/*					ALGORITMO: CAIXEIRO VIAJANTE P. DINAMICA	   */
/* --------------------------------------------------------------- */	



	#include <iostream>
	#include <fstream>
	#include <stdio.h>
	#include <vector>
	#include <cmath>
	#include <time.h>	
	
	using namespace std;
	
	//definir como infinito (o calculo dos caminhos n ultrapassa)
	#define infinito 0x7FFFFFFF
	
	//representacao das coordenadas da cidade
	struct Cidade {
		int x;
		int y;
	};
	
	//celula de valores (reprensetam os caminhos de menor custo) 
	struct Celula {
		double val;
		vector <int> caminhos;
		Celula* prox;
	};
	
	/**
	* Classe para inserir/pegar uma celula de menor custo atraves do vetor caminhos (sao as chaves)
	*/
	class CidadesVisitas {
		//instanciacao inicial
		private:
		vector <Celula*> ini;
		vector <Celula*> fim; 
		
		public:
		/**
		* Construtor da classe, apenas inicializa os ponteiros
		* @param numCidades para facilitar a implementacao, colocamos para criar vetores do tamanho do numero
		* de cidades.
		**/
		CidadesVisitas (int numCidades) {
			ini.resize(numCidades+1);
			fim.resize(numCidades+1);
			for (int x = 0; x < numCidades+1; x++) {
				ini[x] = NULL;
				fim[x] = NULL;
			}
		}
		
		/**
		* Metodo para inserir o menor valor encontrado entre as combinacoes 
		* @param vertice representa em qual vertice o caminho esta passando
		* @param val o menor valor encontrado entre as combinacoes
		* @param caminho representa o caminho passado ate chegar no vertice atual
		**/
		void inserir (int vertice, double val, vector <int> caminho) {
			
			Celula *nova = new Celula;

			//Caso seja o primeira celula a ser inserida
			if (ini[vertice] == NULL) {
				
				nova->val = val;
				nova->caminhos.resize (caminho.size());
				nova->caminhos = caminho;
				nova->prox = NULL;
				
				ini[vertice] = nova;
				fim[vertice] = ini[vertice];
				
			} else {
				//adicionar novas celulas
				nova->val = val;
				nova->caminhos.resize (caminho.size());
				nova->caminhos = caminho;
				nova->prox = NULL;
					
				fim[vertice]->prox = nova;
				fim[vertice] = nova;
				
				nova = NULL;
			}
		}
		
		/**
		* Funcao para recuperar o valor guardado referente a combinacao do caminho passado
		* @param vertice representa em qual vertice o caminho esta passando
		* @param caminho representa a chave para encontrar o caminho
		**/
		double getVal (int vertice, vector <int> caminho) {
			double resposta = infinito;
			Celula *temp = new Celula;
			temp = ini[vertice];
			bool encontrado = true;
			
			if (caminho.size() == 0) {
				return (ini[vertice]->val);
			} else {
				//verificar todas as celulas do vertice desejado
				while (temp != NULL) {
					//verificar se as chaves correspondem ao caminho procurado
					for (int x = 0; x < caminho.size(); x++ ) {
						if (caminho[x] != temp->caminhos[x]) {
							encontrado = false;
						}
					}
					
					if (encontrado) {
						resposta = temp->val;
						break;
					}
					encontrado = true;
					temp = temp->prox;
				}
				return (resposta);
			}
		}
	};	

	
	/**
	* Classe executora do problema do caixeiro viajante
	**/
	class TSPD {
		private:
		//Declaracao de variaveis globais
		FILE *arqEntrada;
		FILE *arqSaida;
		int n;
		int controle;
		Cidade *cidades;
		vector <int> auxComb;
		vector <int> vetor;
		vector < vector <int> > combinacaoElementos;
		vector < vector <double> > mAdjacentes;
		vector <CidadesVisitas*>  custo;
		public:
	
		
		/**
		* Construtor da classe TSPD
		* Inicializar todas as variaveis necessarias para a execucao do algoritmo
		**/		
		TSPD () {
			char nome [256];
			cout << "Informar nome arquivo entrada : ";
			cin >> nome;
			arqEntrada = fopen (nome, "r");
			arqSaida = fopen ("caminho.txt", "w");
			fscanf (arqEntrada, "%d\n", &n);
			vetor.resize(n-1);
			cidades = new Cidade [n+1];
			cidades[0].x = 0;
			cidades[0].y = 0;
			for (int x = 1; x <= n; x++) {
				fscanf (arqEntrada, "%d %d\n", &cidades[x].x, &cidades[x].y);
			}
			fclose(arqEntrada);
			preencherMAdjacentes();
			for (int x = 0; x < (n-1); x++) {
				vetor[x] = x+2;
			}
		}
		
		/**
		* Metodo principal responsavel pela execucao e controle das estruturas para o caixeiro viajante
		**/		
		void algoritmoTSPD () {
			//inicializar as variaveis 
			time_t inicio, fim;
			inicio = clock();
			double custoAux;
			double custoCaminho;
			custo.resize(n-1);
			vector <int> inicial;
			inicial.resize(1);
			inicial[0] = 1;
			for (int x = 0; x < n-1; x++) {
				custo[x] = new CidadesVisitas (n);
			}
			
			//inserir os valores iniciais, ou seja, cidade 1 -> 2, cidade 1 -> 3, cidade 1 -> 4
			for (int x = 2; x <= n; x++ ) {
				custo[0]->inserir(x,mAdjacentes[x][1], extrair (inicial, 1));
			}


			//numero de conjuntos formados a partir do numero de cidades
			//se possuir 5 cidades entao podemos formar 4 subconjuntos, tirando o conjunto vazio
			//aqui serao formados 3 subconjuntos (na documentacao colocamos a explicacao)
			for (int s = 1; s < (n-1); s++) {
				//passar por todas as cidades
				for (int x = 2; x <= n; x++) {
					//variavel global para o funcionamento do metodo combinacoes()
					controle = 0;
					//numero total de combinacoes a partir do tamanho do conjunto
					int numComb = calcularNumCombinacoes(extrair(vetor,x).size(),s);
					alocarCombinacoes (numComb,s);
					auxComb.resize(s);
					combinacoes (extrair(vetor,x),s,0,auxComb);
					//realizar a quantidade combinacoes
					for (int j = 0; j < numComb; j++) {
						vector <int> vetAux = removerVetor (s,j);
						custoCaminho = infinito;
						
						//fazer a quantidade de vezes para cada cidade pertecente ao conjunto
						for (int k = 0; k < s; k++) {
							//calcular o custo
							custoAux = mAdjacentes [x][vetAux[k]] + custo[s-1]->getVal(vetAux[k], extrair (vetAux, vetAux[k]));
							
							//verificar se e' o menor custo passando por esta cidade
							if (custoAux < custoCaminho) {
								custoCaminho = custoAux;
							}
						}
						//apos verificar qual a melhor cidade de menor caminho adiciona-la a estrutura de dados
						custo[s]->inserir (x,custoCaminho, vetAux);
						
					}
					
				}
			}
			//auxiliar para o custo
			custoCaminho = infinito;
			int cidadeBase = 0;
			//realizar a passagem na ultima cidade
			for (int x = 2; x <= n; x++) {
				
				//calcular o custo
				custoAux = mAdjacentes[1][x] + custo[n-2]->getVal(x, extrair (vetor, x));
				
				//verificar se e' o menor custo passando por esta cidade
				if (custoAux < custoCaminho) {
					custoCaminho = custoAux;
					cidadeBase = x;
				}
			}
			
			//Mostrar sem tempo
			//metodo para recuperar o caminho
			//fprintf(arqSaida, "%0.2f\n1 ", custoCaminho);
			//recuperarCaminho (cidadeBase, vetor, (n-2));
			//fclose(arqSaida);
			
			//Mostrar com tempo
			fprintf(arqSaida, "NUMERO CIDADES : %d\nCAMINHO\n%d ", n, 1);
			recuperarCaminho (cidadeBase, vetor, (n-2));
			fim = clock();
			fprintf(arqSaida, "\nCUSTO : %0.2f", custoCaminho);
			fprintf(arqSaida,"\nTEMPO : %0.2f",  (( double(fim) - double(inicio) )/CLOCKS_PER_SEC));
			fclose(arqSaida);
			
		}
			
		
		//Destrutor da classe
		~TSPD () {}
		
		private:
		
		/**
		* Metodo que aloca espaco para a matriz de combinacoes.
		* As combinacoes sao alocadas dinamicamente de acordo com o tamnho do conjunto e o numero de vertices
		* @param linhas recebe o numero de linhas da matriz
		* @param colunas recebe o numero de colunas da matriz
		**/
		void alocarCombinacoes (int linhas, int colunas) {
			combinacaoElementos.resize(linhas);
			for (int x = 0; x < linhas; x++) {
				combinacaoElementos[x].resize(colunas);
			}
		}

		/**
		* Metodo que realiza o calculo da distancia entre dois pontos
		* Aplicada a formula para o calcula da distancia entre dois pontos 
		* @param primeira recebe uma struct do tipo Cidade que possui as coordenadas 'X' e 'Y'
		* @param segunda recebe uma struct do tipo Cidade que possui as coordenadas 'X' e 'Y'
		**/		
		double calcularDistancia (Cidade primeira, Cidade segunda) {			
			double distancia = sqrt ( pow ((primeira.x - segunda.x),2) + pow ((primeira.y - segunda.y), 2));
			return (distancia);
		}
		
		
		/**
		* Metodo que realiza o preenchimento da matriz de distancias entre as cidades
		* E' um grafo onde o valor para o proprio vertice e' preenchido com 0
		* o calculo e' realizado a partir da funcao 'calcularDistancia'
		**/		
		void preencherMAdjacentes () {
			mAdjacentes.resize(n+1);
			for (int x = 0; x < n+1; x++) {
				mAdjacentes[x].resize(n+1);
			}
			for (int x = 0; x <= n; x++) {
				for (int y = 0; y <= n; y++) {
					if (x == 0 || y == 0 || x == y) {
						mAdjacentes[x][y] = 0.0;
					} else {
						mAdjacentes[x][y] = calcularDistancia (cidades[x], cidades[y]);
					}
				}
			}
			delete (cidades);
		}
		
		
		
		/**
		* Metodo que realiza a extracao de um elemento do vetor (conjunto)
		* @param vet recebe o vetor (conjunto) com os vertices
		* @param elemento o elemento para realizar a extracao dentro do vetor (conjunto)
		* @return retorna o vetor (conjunto) sem o vertice extraido
		**/		
		vector <int> extrair (vector <int> vet, int elemento) {
			vector <int> resposta;
			resposta.resize(vet.size()-1);
			int pos = 0;
			for (int x = 0; x < vet.size(); x++) {
				if (vet[x] != elemento) {
					resposta[pos++] = vet[x];
				}
			}
			return (resposta);
		}
		
		
		/**
		* Funcao que calcula o numero de combinacoes do conjunto de vertices.
		* A funcao nao e' eficiente, pois o maximo de combinacoes e' 19, acima disso a variavel int
		* ultrapassa sua representacao
		* @param numEle recebe a quantidade de elementos para combinacao
		* @param tamConj o tamanho do conjunto para dividir como fatorial
		* @return retornar o numero de combinacoes
		**/		
		int calcularNumCombinacoes (int numEle, int tamConj) {
			int combinacaoDividendo = 1;
			int combinacaDivisor = 1;
			int aux = tamConj;
			for (int x = 0; x < tamConj; x++){
				if (numEle != aux) {
					combinacaoDividendo *= numEle--;
				} else if (numEle == aux) {
					numEle--;
					aux--;
				}
			}
			int aux2 = aux;
			for (int x = 0; x < aux2; x++) {
				combinacaDivisor *= aux--; 
			}
			return (combinacaoDividendo/combinacaDivisor);
		}
		
		
		/**
		* Como nos guardamos todas as combinacoes em uma matriz. Cada linha da matriz representa uma combinacao entre os elementos
		* entao escolhemos em qual combinacao estamos, selecionamos a linhas e copiamos os elementos dela para a resposta.
		* @param escolher escolhe linha da matriz que sera copiada para a resposta
		* @param s o tamnho do conjunto (numero de colunas)
		* @return retornar um vetor com a combinacao escolhida
		**/		
		vector <int> removerVetor (int s, int escolher) {
			vector <int> resp;
			resp.resize(s);
			for (int x = 0; x < s; x++) {
				resp[x] = combinacaoElementos[escolher][x];
			}
			return (resp);
		}
		
		
		/**
		* Metodo recursivo que realiza as combinacoes dos elementos do conjunto, de acordo com o tamanho do conjunto desejado. 
		* Por exemplo: {2,3,4} --> combinacao de tamnho 2 --> {2,3}, {2,4}, {3,4}
		* @param arr recebe o conjunto dos elementos
		* @param len o tamanho do conjunto
		* @param posicaoInicial posicao referencia para comecar a selecionar o elemento
		* @param aux vetor auxiliar para preencher os valores na matriz de combinacoes
		**/		
		void combinacoes (vector <int> arr, int len, int posicaoInicial, vector <int> aux) {
			if (len == 0) {
				for (int x = 0; x < aux.size(); x++) {
					combinacaoElementos[controle][x] = aux[x];
				}
				controle++;
				return;
			}
			for (int i = posicaoInicial; i <= arr.size() - len; i++) {
				aux[aux.size() - len] = arr[i];
				//chamda recursiva
				combinacoes (arr, len-1, i+1, aux);
			}		
		}
		
		
		/**
		* Metodo recursivo para realizar a recuperacao do caminho percorrido.
		* Ele realiza a extracao do vertice que possui o menor valor para aquele caminho
		* por exemplo: conjunto de tamanho 3 = {2,3,4} com o vertice base = 5
		* temos que : a primeira cidade e' 5 verifica-se retirando cada elemento do conjunto qual 
		* dara o menor caminho por exemplo se for o 3, entao a proxima cidade e' 3
		* @param caminho e' o vertice base que representara' o caminho passado
		* @param vet e' o vetor de elementos sempre reduzido pelo metodo recursivo, ate chegar o ultimo
		* @param c e' o tamanho do conjunto anterior ao atual, ou seja, se o atual e' 3 o anterior e' 2
		**/		
		void recuperarCaminho (int caminho, vector <int> vet, int c) {
			fprintf(arqSaida, "%d ", caminho);
			vector <int> aux = extrair(vet,caminho);
			if (aux.size() != 0) {
				int custoAux;
				int custoCaminho = infinito;
				int temp = 0;
				for (int x = 0; x < aux.size(); x++) {
					//calcular o custo
					custoAux = custo[c-1]->getVal (aux[x], extrair(aux,aux[x]));
					
					//verificar qual custo e' menor para aquele caminho
					if (custoAux < custoCaminho) {
						custoCaminho = custoAux;
						temp = aux[x];
					}
				}
				//chamada recursiva
				recuperarCaminho (temp, aux, (c-1));				
			}
		}
	
	
	};
	
	
	int main(int argc, char **argv){
		TSPD *novo = new TSPD();
		novo->algoritmoTSPD();
		delete (novo);
		return(0);
	}