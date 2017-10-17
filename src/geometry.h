#ifndef RAYTRACING_H
#define RAYTRACING_H

/* Constantes importantes. */

#define INFINITO 1e10
#define PI 3.14159265358979323846
#define MAX_REFLEX 5

/* Estrutura que define um vetor ou ponto no espaço. */

typedef struct {
    double x;
    double y;
    double z;
} vetor;

typedef vetor ponto;

/* Estrutura para armazenar uma esfera. */
typedef struct {
    ponto centro;
    int raio;
} esfera;

/* Estrutura para armazenar um cor RGB normalizada. */
typedef struct {
    double r;
    double g;
    double b;
} cor;

/* Normaliza um vetor. */
vetor normalizar(vetor* v1);

/* Multiplica o vetor por um escalar. */
vetor mult_e(vetor* v1, double k);

/* Multiplica dois vetores coordenada a coordenada. */
vetor mult_v(vetor* v1, vetor* v2);

/* Faz o produto escalar entre dois vetores. */
double prod_e(vetor* v1, vetor* v2);

/* Faz a substração de v1 por v2 (retorna v1 - v2). */
vetor sub_v(vetor* v1, vetor* v2);

/* Faz a soma de dois vetores. */
vetor soma_v(vetor* v1, vetor* v2);

/* Faz a negação do vetor (retorna -v1). */
vetor neg_v(vetor *v1);

/* Retorna a norma do vetor. */
double modulo(vetor *v1);

/* Verifica se um determinado raio faz interseção com uma esfera. */
int intersecao_esfera(vetor *origem_raio, vetor *direcao_raio, esfera *esf, double *t0, double *t1);

/* Faz a operação de raytracing. */
cor raytrace(vetor *origem_raio, vetor *direcao_raio, esfera *esferas, int num_esferas, int reflexoes);

#endif // RAYTRACING_H


