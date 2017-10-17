#include "raytracing.h"
#include "math.h"

/** 
 * Normaliza um vetor. 
 * @param v1 Ponteiro para o vetor a ser normalizado.
 *
 *
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

/* Multiplica o vetor por um escalar. */
vetor mult_e(vetor* v1, double k)
{
    vetor v2;
    v2.x = v1->x * k;
    v2.y = v1->y * k;
    v2.z = v1->z * k;
    return v2;
}

/* Multiplica dois vetores coordenada a coordenada. */
vetor mult_v(vetor* v1, vetor* v2)
{
    vetor v3;
    v3.x = v1->x * v2->x;
    v3.y = v1->y * v2->y;
    v3.z = v1->z * v2->z;
    return v3;
}

/* Faz o produto escalar entre dois vetores. */
double prod_e(vetor* v1, vetor* v2)
{
    return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

/* Faz a substração de v1 por v2 (retorna v1 - v2). */
vetor sub_v(vetor* v1, vetor* v2)
{
    vetor v3;
    v3.x = v1->x - v2->x;
    v3.y = v1->y - v2->y;
    v3.z = v1->z - v2->z;
    return v3;
}

/* Faz a soma de dois vetores. */
vetor soma_v(vetor* v1, vetor* v2)
{
    vetor v3;
    v3.x = v1->x + v2->x;
    v3.y = v1->y + v2->y;
    v3.z = v1->z + v2->z;
    return v3;
}

/* Faz a negação do vetor (retorna -v1). */
vetor neg_v(vetor *v1)
{
    vetor v2;
    v2.x = -v1->x;
    v2.y = -v1->y;
    v2.z = -v1->z;
    return v2;
}

/* Retorna a norma do vetor. */
double modulo(vetor *v1)
{
    return sqrt(v1->x * v1->x + v1->y * v1->y + v1->z * v1->z);
}

/* Verifica se um determinado raio faz interseção com uma esfera. */
int intersecao_esfera(vetor *origem_raio, vetor *direcao_raio, esfera *esf, double *t0, double *t1)
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

/* Faz a operação de raytracing. */
cor raytrace(vetor *origem_raio, vetor *direcao_raio, esfera *esferas, int num_esferas, int reflexoes)
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
