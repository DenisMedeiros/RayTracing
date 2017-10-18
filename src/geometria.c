#include "geometria.h"
#include "math.h"

/** 
 * Esta função faz a soma de dois vetores.
 * 
 * @param v1 Ponteiro para o primeiro vetor.
 * @param v2 Ponteiro para o segundo vetor.
 * @return Resultado da soma de v1 e v2 (v1 + v2).
 */
vetor soma_v(vetor* v1, vetor* v2)
{
    vetor v3;
    v3.x = v1->x + v2->x;
    v3.y = v1->y + v2->y;
    v3.z = v1->z + v2->z;
    return v3;
}

/** 
 * Esta função faz a subtração de dois vetores.
 * 
 * @param v1 Ponteiro para o primeiro vetor.
 * @param v2 Ponteiro para o segundo vetor.
 * @return Resultado da subtração de v1 por v2 (v1 - v2).
 */
vetor sub_v(vetor* v1, vetor* v2)
{
    vetor v3;
    v3.x = v1->x - v2->x;
    v3.y = v1->y - v2->y;
    v3.z = v1->z - v2->z;
    return v3;
}

/** 
 * Esta função faz a multiplicação de um vetor por um escalar.
 * 
 * @param v1 Ponteiro para o vetor a ser multiplicado.
 * @param k Escalar que multiplica o vetor v1.
 * @return Produto de k por v1.
 */
vetor mult_e(vetor* v1, double k)
{
    vetor v2;
    v2.x = v1->x * k;
    v2.y = v1->y * k;
    v2.z = v1->z * k;
    return v2;
}

/** 
 * Esta função faz a multiplicação elemento a elemento de dois vetores.
 * 
 * @param v1 Ponteiro para o primeiro vetor.
 * @param v2 Ponteiro para o segundo vetor.
 * @return Resultado da multiplicação elemento a elemento de v1 e v2.
 */
vetor mult_v(vetor* v1, vetor* v2)
{
    vetor v3;
    v3.x = v1->x * v2->x;
    v3.y = v1->y * v2->y;
    v3.z = v1->z * v2->z;
    return v3;
}

/** 
 * Esta função faz negação do vetor.
 * 
 * @param v1 Ponteiro para o vetor.
 * @return Vetor v1 com o sentido invertido (-v1).
 */
vetor neg_v(vetor *v1)
{
    vetor v2;
    v2.x = -v1->x;
    v2.y = -v1->y;
    v2.z = -v1->z;
    return v2;
}

/** 
 * Esta função retorna o módulo (norma) do vetor.
 * 
 * @param v1 Ponteiro para o vetor.
 * @return Módulo (norma) do vetor v1.
 */
double modulo(vetor *v1)
{
    return sqrt(v1->x * v1->x + v1->y * v1->y + v1->z * v1->z);
}

/** 
 * Esta função normaliza um vetor, isto é, faz com que seu módulo seja
 * igual a 1.
 * 
 * @param v1 Ponteiro para o vetor a ser normalizado.
 * @return Vetor v1 normalizado.
 */
vetor normalizar(vetor* v1)
{
    vetor v2;
    double norma = modulo(v1);
    v2.x = v1->x/norma;
    v2.y = v1->y/norma;
    v2.z = v1->z/norma;
    return v2;
}

/** 
 * Esta função faz o produto escalar (dot) entre dois vetores.
 * 
 * @param v1 Ponteiro para o primeiro vetor.
 * @param v2 Ponteiro para o segundo vetor.
 * @return Resultado do produto escalar entre v1 e v2.
 */
double prod_e(vetor* v1, vetor* v2)
{
    return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

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
int intersecao_esfera(ponto *origem_raio, vetor *direcao_raio, esfera *esf, double *t0, double *t1)
{
    vetor distancia;
    double res;
    double quad_raio;
    double quad_cateto;
    double diferenca;
    
    quad_raio = (esf->raio * esf->raio);
    
    /* Calcula o vetor distância entre o ponto de origem e o centro da esfera. */
    distancia = sub_v(&esf->centro, origem_raio); 
    
    /* Verifica se o ângulo entre o vetor distância e a direção do raio está entre -90° e +90° */
    res = prod_e(&distancia, direcao_raio);
    
    if(res < 0)
    {
        return 0;
    }
    
    /* Com o teorema de pitágoras, verifica se o raio toca dentro da esfera. */
    /* 'res' contém o módulo do vetor direção. */
    quad_cateto = prod_e(&distancia, &distancia) - res * res;
    
    if(quad_cateto > quad_raio)
    {
        return 0;
    }
    
    diferenca = sqrt(quad_raio - quad_cateto);
    *t0 = res - diferenca;
    *t1 = res + diferenca;

    return 1;
}

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
cor raytrace(ponto *origem_raio, vetor *direcao_raio, esfera *esferas, int num_esferas)
{
    cor c;
    double tperto, t0, t1;
    esfera *esf;
    int i;
 
    tperto = INFINITO;
    esf = 0;
    c.r = 0.0;
    c.g = 0.0;
    c.b = 0.0;
    
    /* Encontra a interseção do raio com uma esfera da cena. */
    for (i = 0; i < num_esferas; ++i) 
    {
        t0 = INFINITO; 
        t1 = INFINITO;
        if (intersecao_esfera(origem_raio, direcao_raio, &esferas[i], &t0, &t1)) 
        {
            if (t0 < 0)
            { 
                t0 = t1;
            }
            if (t0 < tperto) 
            {
                tperto = t0;
                esf = &esferas[i];
            }
        }
    }
    
    /* O raio não tocou nenhuma esfera. */
    if(esf == 0)
    {
        return c;
    }
    
    return c;
}
