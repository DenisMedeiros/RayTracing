#ifndef GEOMETRIA_H
#define GEOMETRIA_H

/** Representa o maior valor de distância dos objetos. */
#define INFINITO 1.0e10
/** Valor de PI. */
#define PI 3.14159265358979323846
/** Número máximo de recursões para reflexão e refração. */
#define MAX_REC 5

/** 
 * Estrutura que define um vetor ou ponto no espaço. 
 * Ela contém apenas as 3 componentes para representar o vetor 
 * no espaço.
 */
typedef struct {
    double x;
    double y;
    double z;
} vetor;

/** 
 * Um ponto possui a mesma estrutura de um vetor no espaço. 
 * Logo, a estrutura do vetor pode ser reaproveitada.
 */
typedef vetor ponto;

/** 
 * Estrutura para armazenar um cor RGB normalizada (entre 0 e 1). 
 * Ela contem as 3 componentes de R, G e B, respectivamente. 
 */
typedef struct {
    double r;
    double g;
    double b;
} cor;

/** 
 * Estrutura para armazenar uma esfera. 
 * 
 * Para ser representada, a esfera precisa de um centro e de um raio.
 */
typedef struct {
    ponto centro;
    double raio;
} esfera;

/** 
 * Esta função faz a soma de dois vetores.
 * 
 * @param v1 Ponteiro para o primeiro vetor.
 * @param v2 Ponteiro para o segundo vetor.
 * @return Resultado da soma de v1 e v2 (v1 + v2).
 */
vetor soma_v(vetor* v1, vetor* v2);

/** 
 * Esta função faz a subtração de dois vetores.
 * 
 * @param v1 Ponteiro para o primeiro vetor.
 * @param v2 Ponteiro para o segundo vetor.
 * @return Resultado da subtração de v1 por v2 (v1 - v2).
 */
vetor sub_v(vetor* v1, vetor* v2);

/** 
 * Esta função faz a multiplicação de um vetor por um escalar.
 * 
 * @param v1 Ponteiro para o vetor a ser multiplicado.
 * @param k Escalar que multiplica o vetor v1.
 * @return Produto de k por v1.
 */
vetor mult_e(vetor* v1, double k);

/** 
 * Esta função faz a multiplicação elemento a elemento de dois vetores.
 * 
 * @param v1 Ponteiro para o primeiro vetor.
 * @param v2 Ponteiro para o segundo vetor.
 * @return Resultado da multiplicação elemento a elemento de v1 e v2.
 */
vetor mult_v(vetor* v1, vetor* v2);

/** 
 * Esta função faz negação do vetor.
 * 
 * @param v1 Ponteiro para o vetor.
 * @return Vetor v1 com o sentido invertido (-v1).
 */
vetor neg_v(vetor *v1);

/** 
 * Esta função retorna o módulo (norma) do vetor.
 * 
 * @param v1 Ponteiro para o vetor.
 * @return Módulo (norma) do vetor v1.
 */
double modulo(vetor *v1);

/** 
 * Esta função normaliza um vetor, isto é, faz com que seu módulo seja
 * igual a 1.
 * 
 * @param v1 Ponteiro para o vetor a ser normalizado.
 * @return Vetor v1 normalizado.
 */
vetor normalizar(vetor* v1);

/** 
 * Esta função faz o produto escalar (dot) entre dois vetores.
 * 
 * @param v1 Ponteiro para o primeiro vetor.
 * @param v2 Ponteiro para o segundo vetor.
 * @return Resultado do produto escalar entre v1 e v2.
 */
double prod_e(vetor* v1, vetor* v2);

/**
 * Verifica se um determinado raio intersecta uma esfera no espaço.
 * 
 * @param origem_raio Ponteiro para o ponto no espaço de onde o 
 * raio parte.
 * @param direcao_raio Ponteiro para o vetor que determina a direção
 * do raio.
 * @param esf Ponteiro para a esfera a ser intersectada.
 * @param t0 Ponteiro para a distância horizontal entre o ponto de 
 * origem e o primeiro ponto de interseção (é modificada na função).
 * @param t1 Ponteiro para a distância horizontal entre o ponto de 
 * origem e o segundo ponto de interseção (é modificada na função).
 * @return 1 se o raio intersecta a esfera, 0 caso contrário.
 */ 
int intersecao_esfera(ponto *origem_raio, vetor *direcao_raio, esfera *esf, double *t0, double *t1);

/** 
 * Faz a operação de raytracing resursiva. 
 * 
 * @param origem_raio Ponteiro para o ponto no espaço de onde o 
 * raio parte.
 * @param direcao_raio Ponteiro para o vetor que determina a direção
 * do raio.
 * @param esferas Array com esferas colocadas no espaço.
 * @param num_esferas Número de esferas do array anterior.
 * 
 * 
 */
cor raytrace(ponto *origem_raio, vetor *direcao_raio, esfera *esferas, int num_esferas);

#endif // GEOMETRIA_H


