#include "geometria.h"
#include <math.h>
#include <stdio.h>
/** 
 * Esta função faz a soma de dois vetores.
 * 
 * @param v1 Ponteiro para o primeiro vetor.
 * @param v2 Ponteiro para o segundo vetor.
 * @return Resultado da soma de v1 e v2 (v1 + v2).
 */
vetor_t soma_v(vetor_t *v1, vetor_t *v2)
{
    vetor_t v3;
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
vetor_t sub_v(vetor_t *v1, vetor_t *v2)
{
    vetor_t v3;
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
vetor_t mult_e(vetor_t *v1, double k)
{
    vetor_t v2;
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
vetor_t mult_v(vetor_t *v1, vetor_t *v2)
{
    vetor_t v3;
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
vetor_t neg_v(vetor_t *v1)
{
    vetor_t v2;
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
double modulo(vetor_t *v1)
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
vetor_t normalizar(vetor_t *v1)
{
    vetor_t v2;
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
double prod_e(vetor_t *v1, vetor_t *v2)
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
int intersecao_esfera(ponto_t *origem_raio, vetor_t *direcao_raio, esfera_t *esfera, double *t0, double *t1)
{
    vetor_t distancia;
    double res;
    double quad_raio;
    double quad_cateto;
    double diferenca;
    
    quad_raio = (esfera->raio * esfera->raio);
    
    /* Calcula o vetor distância entre o ponto de origem e o centro da esfera. */
    distancia = sub_v(&esfera->centro, origem_raio); 
    
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
 * Verifica se um determinado raio intersecta uma cubo no espaço.
 * 
 * @param origem_raio Ponteiro para o ponto no espaço de onde o 
 * raio parte.
 * @param direcao_raio Ponteiro para o vetor que determina a direção
 * do raio.
 * @param esf Ponteiro para o cubo a ser intersectado.
 * @param t0 Ponteiro para a distância horizontal entre o ponto de 
 * origem e o primeiro ponto de interseção (é modificada na função).
 * @param t1 Ponteiro para a distância horizontal entre o ponto de 
 * origem e o segundo ponto de interseção (é modificada na função).
 * @return 1 se o raio intersecta o cubo, 0 caso contrário.
 */ 
int intersecao_cubo(ponto_t *origem_raio, vetor_t *direcao_raio, cubo_t *c, double *t0, double *t1)
{
    // TODO
    return 0;
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
 */
cor_t raytrace(ponto_t *origem_raio, vetor_t *direcao_raio, objeto_t *objetos, luz_t *luz, int num_objetos, int num_recursoes)
{
    cor_t cor;
    double tperto, t0, t1;
    int i, j, luz_direta;
	objeto_t *objeto_perto;
	vetor_t normal, refletido, temp, direcao_luz;
	ponto_t ponto_intersec;
	
	vetor_t temp2;
	float t4;
	vetor_t temp3;
 
	objeto_perto = 0;
    tperto = INFINITO; 

    /* Se não tocar nenhum objeto, então a cor será negativa. */
    cor.x = -1.0;
    cor.y = -1.0;
    cor.z = -1.0;
    
    /* Encontra o objeto mais perto da câmera (caso exista). */
    for (i = 0; i < num_objetos; ++i) 
    {
        t0 = INFINITO; 
        t1 = INFINITO;

        switch(objetos[i].tipo)
        {
        case ESFERA:
            intersecao_esfera(origem_raio, direcao_raio, objetos[i].esfera, &t0, &t1);
			break;
        case CUBO:
            intersecao_cubo(origem_raio, direcao_raio, objetos[i].cubo, &t0, &t1);
            break;
        default:
            break;
        }
        
        if(t0 == INFINITO)
        {
			continue;
		}
        
		if (t0 < 0)
		{ 
			t0 = t1;
		}
		
		if (t0 < tperto) 
		{
			tperto = t0;
			objeto_perto = &objetos[i];
		}

    }
    
    // Verifica se algum objeto não foi intersectado.
    if(objeto_perto == 0)
    {
		return cor;
	}
	
	// Pega a cor...
	switch(objeto_perto->tipo)
    {
    case ESFERA:
		temp = mult_e(direcao_raio, tperto);
		ponto_intersec = soma_v(origem_raio, &temp);
		normal = sub_v(&ponto_intersec, &objeto_perto->esfera->centro);
		normal = normalizar(&normal);
		
		if(prod_e(direcao_raio, &normal) > 0)
		{
			normal = neg_v(&normal);
		}
		
		if(objeto_perto->refletivel && num_recursoes < MAX_REC)
		{
			
			temp = mult_e(&normal, 2 * prod_e(direcao_raio, &normal));
			refletido = sub_v(direcao_raio, &temp);
			refletido = normalizar(&refletido);	
			cor = raytrace(&ponto_intersec, &normal, objetos, luz, num_objetos, num_recursoes + 1);
		}
		else // Calculo da luz
		{
			
			direcao_luz = sub_v(&luz->posicao, &ponto_intersec);
			direcao_luz = normalizar(&direcao_luz);
			luz_direta = 1;
			// Percore os demais objetos para ver se há algum na frente.
			for(j = 0; j < num_objetos; j++)
			{
				switch(objetos[j].tipo)
				{
				case ESFERA:
					if(intersecao_esfera(&ponto_intersec, &direcao_luz, objetos[j].esfera, &t0, &t1))
					{
						luz_direta = 0;
					}
					break;
				case CUBO:
					intersecao_cubo(&ponto_intersec, &direcao_luz, objetos[j].cubo, &t0, &t1);
					{
						luz_direta = 0;
					}
					break;
				default:
					break;
				}
				
				if(!luz_direta)
				{
					break;
				}
			}
			
			temp2 = mult_e(&objeto_perto->cor, luz_direta);
			t4 = max(0.0f, prod_e(&normal, &direcao_luz));
			temp3 = mult_e(&temp2, t4);
			
			cor = mult_v(&temp3, &luz->cor);
			
		}
			
		break;
	case CUBO:
		break;
	default:
		break;
	}
      
    return cor;
}
