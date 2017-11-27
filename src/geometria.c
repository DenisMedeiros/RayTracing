#include "geometria.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

extern double ka;
extern double kd;
extern double ks;
extern double eta;
extern double os;

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
 * Esta função faz o produto vetorial entre dois vetores.
 * 
 * @param v1 Ponteiro para o primeiro vetor.
 * @param v2 Ponteiro para o segundo vetor.
 * @return Resultado do produto escalar entre v1 e v2.
 */
vetor_t prod_v(vetor_t *v1, vetor_t *v2)
{
    vetor_t v3;
    v3.x = v1->y * v2->z - v1->z * v2->y;
    v3.y = v1->z * v2->x - v1->x * v2->z;
    v3.z = v1->x * v2->y - v1->y * v2->x;
    
    return v3;
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
int intersecao_esfera(ponto_t *origem_raio, vetor_t *direcao_raio, 
    esfera_t *esfera, double *t0, double *t1, vetor_t *normal)
{
    vetor_t distancia, temp1_v, ponto_intersec;
    double res, quad_raio, quad_cateto, diferenca;
    
    quad_raio = (esfera->raio * esfera->raio);
    
    // Calcula o vetor distância entre o ponto de origem e o centro da esfera.
    distancia = sub_v(&esfera->centro, origem_raio); 
    
    // Verifica se o ângulo entre o  distância e a direção do raio está 
    // entre -90° e +90°.
    res = prod_e(&distancia, direcao_raio);
    
    if(res < 0)
    {
        return 0;
    }
    
    // Com o teorema de pitágoras, verifica se o raio toca dentro da esfera.
    // 'res' contém o módulo do vetor direção. 
    quad_cateto = prod_e(&distancia, &distancia) - res * res;
    
    // Verifica se o raio passa por fora da esfera.
    if(quad_cateto > quad_raio)
    {
        return 0;
    }
    
    diferenca = sqrt(quad_raio - quad_cateto);
    *t0 = res - diferenca;
    *t1 = res + diferenca;
    
    // Calcula a normal.
    temp1_v = mult_e(direcao_raio, *t0);
    ponto_intersec = soma_v(origem_raio, &temp1_v);

    *normal = sub_v(&ponto_intersec, &esfera->centro);
    *normal = normalizar(normal);
    
    return 1;
}


/**
 * Verifica se um determinado raio intersecta um triângulo no espaço.
 * 
 * @param origem_raio Ponteiro para o ponto no espaço de onde o 
 * raio parte.
 * @param direcao_raio Ponteiro para o vetor que determina a direção
 * do raio.
 * @param triangulo Ponteiro para o triângulo a ser intersectado.
 * @param t0 Ponteiro para a distância horizontal entre o ponto de 
 * origem e o primeiro ponto de interseção (é modificada na função).
 * @param t1 Ponteiro para a distância horizontal entre o ponto de 
 * origem e o segundo ponto de interseção (é modificada na função).
 * @return 1 se o raio intersecta o piramide, 0 caso contrário.
 */ 
int intersecao_triangulo(ponto_t *origem_raio, vetor_t *direcao_raio, 
    triangulo_t *triangulo, double *t0, vetor_t *normal)
{
    vetor_t v1, v2, temp1_v, temp2_v;
    ponto_t ponto_intersec;
    double denominador;
    double d, t0_temp;
    
    // Calcula os vetores a partir dos vértices o triângulo. 
    v1 = sub_v(&triangulo->vertices[1], &triangulo->vertices[0]);
    v2 = sub_v(&triangulo->vertices[2], &triangulo->vertices[0]);
    
    // Calcula o vetor normal à face do triângulo.
    *normal = prod_v(&v1, &v2); // Vetor normal não normalizado
    *normal = normalizar(normal);
    
    // Resultado parcial para encontrar o ponto de intersecção.
    denominador = prod_e(normal, direcao_raio);
    
    if (fabs(denominador) < EPSILON)
    {
    return 0;
    }

    // Encontra o t0 (parâmetro da equação paramétrica).
    d = prod_e(normal, &triangulo->vertices[0]);
    t0_temp = (d - prod_e(normal, origem_raio))/denominador;
    
    // Checa se o triângulo está atrás do ponto de origem do raio (observador).
    if(t0_temp < 0)
    {
        return 0;
    }
    
    // Calcula o ponto de intersecção.
    temp1_v = mult_e(direcao_raio, t0_temp);
    ponto_intersec = soma_v(origem_raio, &temp1_v);

    temp1_v = sub_v(&triangulo->vertices[1], &triangulo->vertices[0]);
    temp2_v = sub_v(&ponto_intersec, &triangulo->vertices[0]);
    temp1_v = prod_v(&temp1_v, &temp2_v);
    if(prod_e(&temp1_v, normal) < 0)
    {
        return 0;
    }

    temp1_v = sub_v(&triangulo->vertices[2], &triangulo->vertices[1]);
    temp2_v = sub_v(&ponto_intersec, &triangulo->vertices[1]);
    temp1_v = prod_v(&temp1_v, &temp2_v);
    if(prod_e(&temp1_v, normal) < 0)
    {
        return 0;
    }

    temp1_v = sub_v(&triangulo->vertices[0], &triangulo->vertices[2]);
    temp2_v = sub_v(&ponto_intersec, &triangulo->vertices[2]);
    temp1_v = prod_v(&temp1_v, &temp2_v);
    if(prod_e(&temp1_v, normal) < 0)
    {
        return 0;
    }

    *t0 = t0_temp;
    return 1;
}

/**
 * Verifica se um determinado raio intersecta uma piramide no espaço.
 * 
 * @param origem_raio Ponteiro para o ponto no espaço de onde o 
 * raio parte.
 * @param direcao_raio Ponteiro para o vetor que determina a direção
 * do raio.
 * @param piramide Ponteiro para o piramide a ser intersectada.
 * @param t0 Ponteiro para a distância horizontal entre o ponto de 
 * origem e o primeiro ponto de interseção (é modificada na função).
 * @param t1 Ponteiro para a distância horizontal entre o ponto de 
 * origem e o segundo ponto de interseção (é modificada na função).
 * @param normal Ponteiro para o vetor normal da face a ser retornado.
 * @return 1 se o raio intersecta o piramide, 0 caso contrário.
 */ 
int intersecao_piramide(ponto_t *origem_raio, vetor_t *direcao_raio, 
    piramide_t *piramide, double *t0, double *t1, vetor_t *normal)
{
    double temp; // Usado no swap de t0 e t1
    int i, contagem;
    triangulo_t triangulos[4];
    
    contagem = 0;
    
    // Construindo os triângulos (faces) a partir dos vértices da pirâmide.
    triangulos[0].vertices[0] = piramide->vertices[0];
    triangulos[0].vertices[1] = piramide->vertices[1];
    triangulos[0].vertices[2] = piramide->vertices[2];
    
    triangulos[1].vertices[0] = piramide->vertices[0];
    triangulos[1].vertices[1] = piramide->vertices[1];
    triangulos[1].vertices[2] = piramide->vertices[3];
    
    triangulos[2].vertices[0] = piramide->vertices[0];
    triangulos[2].vertices[1] = piramide->vertices[2];
    triangulos[2].vertices[2] = piramide->vertices[3];
    
    triangulos[3].vertices[0] = piramide->vertices[1];
    triangulos[3].vertices[1] = piramide->vertices[2];
    triangulos[3].vertices[2] = piramide->vertices[3];
    
    for(i = 0; i < 4; i++) 
    {
    // Verifica quais as duas faces que são intersectadas.
        if(intersecao_triangulo(origem_raio, direcao_raio, &triangulos[i], 
        t0, normal))
        {
            contagem++;
            
            if(contagem == 1)
            {
                *t1 = *t0;
            }
            else if (contagem == 2)
            {
                if(*t1 < *t0) // Caso o novo t0 seja pior, troque com t1.
                {
                    temp = *t1;
                    *t1 = *t0;
                    *t0 = temp;  
                }
  
                return 1;
            }
        }
    }
    
    return 0;
}


    
/**
 * Verifica se um determinado raio intersecta um plano no espaço.
 * 
 * @param origem_raio Ponteiro para o ponto no espaço de onde o 
 * raio parte.
 * @param direcao_raio Ponteiro para o vetor que determina a direção
 * do raio.
 * @param plano Ponteiro para o plano a ser intersectado.
 * @param t0 Ponteiro para a distância horizontal entre o ponto de 
 * origem e o primeiro ponto de interseção (é modificada na função).
 * @param t1 Ponteiro para a distância horizontal entre o ponto de 
 * origem e o segundo ponto de interseção (é modificada na função).
 * @return 1 se o raio intersecta o piramide, 0 caso contrário.
 */ 
int intersecao_plano(ponto_t *origem_raio, vetor_t *direcao_raio, 
    plano_t *plano, double *t0)
{

    double denominador, d, t0_temp;
    
    // Resultado parcial para encontrar o ponto de intersecção.
    denominador = prod_e(&plano->normal, direcao_raio);
    
    if (fabs(denominador) < EPSILON)
    {
    return 0;
    }

    // Encontra o t0 (parâmetro da equação paramétrica).
    d = prod_e(&plano->normal, &plano->ponto);
    t0_temp = (d - prod_e(&plano->normal, origem_raio))/denominador;
    
    // Checa se o triângulo está atrás do ponto de origem do raio (observador).
    if(t0_temp < 0)
    {
        return 0;
    }
    
    *t0 = t0_temp;
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
int intersecao_cubo(ponto_t *origem_raio, vetor_t *direcao_raio, cubo_t *cubo, double *t0, double *t1, vetor_t *normal)
{
    double temp, t0_temp;
	vetor_t normal_temp;
    int i, contagem;
    
    contagem = 0;
    
    // Construindo os triângulos
    triangulo_t triangulos[12];
        
    triangulos[0].vertices[0] = cubo->vertices[0];
    triangulos[0].vertices[1] = cubo->vertices[1];
    triangulos[0].vertices[2] = cubo->vertices[3];
    
    triangulos[1].vertices[0] = cubo->vertices[3];
    triangulos[1].vertices[1] = cubo->vertices[2];
    triangulos[1].vertices[2] = cubo->vertices[0];
    
    triangulos[2].vertices[0] = cubo->vertices[4];
    triangulos[2].vertices[1] = cubo->vertices[2];
    triangulos[2].vertices[2] = cubo->vertices[0];
    
    triangulos[3].vertices[0] = cubo->vertices[6];
    triangulos[3].vertices[1] = cubo->vertices[4];
    triangulos[3].vertices[2] = cubo->vertices[2];
    
    triangulos[4].vertices[0] = cubo->vertices[4];
    triangulos[4].vertices[1] = cubo->vertices[5];
    triangulos[4].vertices[2] = cubo->vertices[6];
    
    triangulos[5].vertices[0] = cubo->vertices[5];
    triangulos[5].vertices[1] = cubo->vertices[6];
    triangulos[5].vertices[2] = cubo->vertices[7];
    
    triangulos[6].vertices[0] = cubo->vertices[1];
    triangulos[6].vertices[1] = cubo->vertices[3];
    triangulos[6].vertices[2] = cubo->vertices[5];
    
    triangulos[7].vertices[0] = cubo->vertices[3];
    triangulos[7].vertices[1] = cubo->vertices[5];
    triangulos[7].vertices[2] = cubo->vertices[7];
    
    triangulos[8].vertices[0] = cubo->vertices[2];
    triangulos[8].vertices[1] = cubo->vertices[3];
    triangulos[8].vertices[2] = cubo->vertices[7];
    
    triangulos[9].vertices[0] = cubo->vertices[2];
    triangulos[9].vertices[1] = cubo->vertices[6];
    triangulos[9].vertices[2] = cubo->vertices[7];
    
    triangulos[10].vertices[0] = cubo->vertices[0];
    triangulos[10].vertices[1] = cubo->vertices[4];
    triangulos[10].vertices[2] = cubo->vertices[5];
    
    triangulos[11].vertices[0] = cubo->vertices[0];
    triangulos[11].vertices[1] = cubo->vertices[1];
    triangulos[11].vertices[2] = cubo->vertices[5];

    *t0 = INFINITO;

    for(i = 0; i < 12; i++) 
    {
        // Verifica quais as duas faces que são intersectadas.
        if(intersecao_triangulo(origem_raio, direcao_raio, &triangulos[i], 
            &t0_temp, &normal_temp))
        {
            
			if(i == 1)
			{
				*t0 = t0_temp; 
				*t1 = t0_temp;
				*normal = normal_temp;
			}
			else
			{
				if(t0_temp < *t0)
				{
					*t1 = *t0;
					*t0 = t0_temp;    
					*normal = normal_temp;
				}  
				else if (t0_temp < *t1)
				{
					*t1 = t0_temp;
				} 
			}
		}

    }
    

    if(*t0 != INFINITO)
    {
        return 1;
    }    

    return 0;
}



/** 
 * Faz a operação de raytracing resursiva. 
 * 
 * @param origem_raio Ponteiro para o ponto no espaço de onde o 
 * raio parte.
 * @param direcao_raio Ponteiro para o vetor que determina a direção
 * do raio.
 * @param luz_local Ponteiro para a luz local (pontual).
 * @param luz_ambiente Ponteiro para a luz ambiente.
 * @param objetos Array com objetos colocados no espaço.
 * @param num_objetos Número de objetos do array de objetos.
 * @param num_reflexoes Número de reflexões (usado na versão recursiva).
 * @param max_recursoes Número máximo de reflexões (usado na versão recursiva).
 * @param num_esferas Número de esferas do array anterior.
 * 
 */
cor_t raytrace(ponto_t *origem_raio, vetor_t *direcao_raio, luz_t *luz_local,  
    luz_t *luz_ambiente, objeto_t *objetos, int num_objetos, int num_reflexoes,  
    int max_recursoes)

{
    cor_t cor_final;
    double tperto, t0_esfera, t1_esfera, t0_piramide, t1_piramide, t0_plano, t0_cubo, t1_cubo;
    int i;
    objeto_t *objeto_perto;
    vetor_t normal, normal_pir, normal_cubo, normal_esf, temp1_v;
    ponto_t ponto_intersec;
    
    objeto_perto = 0;
    tperto = INFINITO; 

    // Se não tocar nenhum objeto, então a cor será negativa. 
    cor_final.x = -1.0;
    cor_final.y = -1.0;
    cor_final.z = -1.0;
    
    
    // Encontra o objeto mais perto da câmera (caso exista).
    for (i = 0; i < num_objetos; ++i)
    {
        t0_esfera = INFINITO; 
        t1_esfera = INFINITO;
        t0_piramide = INFINITO; 
        t1_piramide = INFINITO;
        t0_cubo = INFINITO;
        t1_cubo = INFINITO;
        t0_plano = INFINITO; 
        
        if (objetos[i].tipo == ESFERA)
        {
            intersecao_esfera(origem_raio, direcao_raio, objetos[i].esfera, 
            &t0_esfera, &t1_esfera, &normal_esf);

            if(t0_esfera == INFINITO) // Verifica se não tocou nenhuma.
            {
                continue;
            }

            if (t0_esfera  < 0) // Caso o raio tenha intersectado a borda.
            { 
                t0_esfera  = t1_esfera;
            }
            if (t0_esfera < tperto)
            {
                tperto = t0_esfera;        
                objeto_perto = &objetos[i];
                normal = normal_esf;
                
            }     
        }
        else if (objetos[i].tipo == PIRAMIDE)
        {
            intersecao_piramide(origem_raio, direcao_raio, 
            objetos[i].piramide, &t0_piramide, &t1_piramide, 
            &normal_pir);

            if(t0_piramide == INFINITO) // Verifica se não tocou nenhuma.
            {
                continue;
            }

            if (t0_piramide  < 0) // Caso o raio tenha intersectado a borda.
            { 
                t0_piramide  = t1_piramide;
            }

            // Caso esta pirâmide seja a mais perto do observador.
            if (t0_piramide < tperto)
            {
                tperto = t0_piramide;
                objeto_perto = &objetos[i];
                normal = normal_pir;
            }
        }
        else if (objetos[i].tipo == CUBO)
        {
            
            intersecao_cubo(origem_raio, direcao_raio, 
                objetos[i].cubo, &t0_cubo, &t1_cubo, 
                &normal_cubo);
                
            if(t0_cubo == INFINITO) // Verifica se não tocou nenhuma.
            {
                continue;
            }  

            if (t0_cubo  < 0) // Caso o raio tenha intersectado a borda.
            { 
                t0_cubo  = t1_cubo;
            }

            // Caso esta pirâmide seja a mais perto do observador.
            if (t0_cubo < tperto)
            {
                tperto = t0_cubo;
                objeto_perto = &objetos[i];
                normal = normal_cubo;    
            }              
        }
        
        else if (objetos[i].tipo == PLANO)
        {
            intersecao_plano(origem_raio, direcao_raio, 
                objetos[i].plano, &t0_plano);
            
                
            if(t0_plano == INFINITO) // Verifica se não tocou nenhuma.
            {
               continue;
            }

            // Caso esta pirâmide seja a mais perto do observador.
            if (t0_plano < tperto)
            {
                tperto = t0_plano;
                objeto_perto = &objetos[i];
                normal = objeto_perto->plano->normal;
            }
        }
        else
        {
        }
          
    }
    
    // Verifica se algum objeto não foi intersectado.
    if(objeto_perto == 0) 
    {
        return cor_final;
    }
    
    // Inverte o sentido da normal caso ela esteja dentro da esfera.
    if(prod_e(direcao_raio, &normal) > 0)
    {
        normal = neg_v(&normal);
    }
    
    // Calcula o ponto de intersecção do objeto.
    temp1_v = mult_e(direcao_raio, tperto);
    ponto_intersec = soma_v(origem_raio, &temp1_v);    
    
    cor_final = calcular_iluminacao(origem_raio, direcao_raio, 
        luz_local, luz_ambiente, objetos, num_objetos, 
        objeto_perto, &ponto_intersec, &normal);
    
  
    return cor_final;
    
}


/**
 * Equação de Phong da iluminação, que serve para calcular a cor do objeto.
 * 
 * @param origem_raio Ponteiro para o ponto no espaço de onde o 
 * raio parte.
 * @param luz_local Ponteiro para a luz local (pontual).
 * @param luz_ambiente Ponteiro para a luz ambiente.
 * @param pos_ponto Posição do ponto do objeto a ser avaliado.
 * @param normal_ponto Vetor normal que indica o plano onde está o ponto.
 * @param cor_ponto Cor do ponto a ser pintado.
 * 
 */ 
cor_t equacao_phong(ponto_t *origem_raio, luz_t *luz_local, 
    luz_t *luz_ambiente, ponto_t *pos_ponto, vetor_t *normal_ponto, 
    cor_t *cor_ponto)
{
    cor_t ambiente, difusa, especular, cor_final;
    vetor_t dir_luz, dir_obs, raio_refletido;

    // Variáveis auxiliares no cálculo vetorial.
    double temp1_d;
    vetor_t temp1_v;
    
    // Calcula a direção para o observador do ponto para o observador.
    dir_obs = sub_v(origem_raio, pos_ponto);
    dir_obs = normalizar(&dir_obs);
    
    // Calcula a direção do raio refletido e normaliza-o.
    dir_luz = neg_v(&dir_luz);
    temp1_v = mult_e(normal_ponto, 2 * prod_e(&dir_luz, normal_ponto));
    raio_refletido = sub_v(&dir_luz, &temp1_v);
    raio_refletido = normalizar(&raio_refletido);    
    
    // Calcula a direção da luz (ponto intersec até a luz).
    dir_luz = sub_v(&luz_local->posicao, pos_ponto);
    dir_luz = normalizar(&dir_luz);
    
    // Calcula a luz ambiente;.
    ambiente = mult_e(&luz_ambiente->cor, ka);
    
    // Calcula a luz difusa
    difusa = mult_e(&luz_local->cor, kd * max(0.0f, prod_e(normal_ponto, &dir_luz)));

    // Calcula a luz especular
    temp1_d = max(0.0f, prod_e(&raio_refletido, &dir_obs));
    especular = mult_e(&luz_local->cor, os * ks * pow(temp1_d, eta));
    
    // Calcula a cor final (soma das luzes).
    cor_final = soma_v(&ambiente, &difusa);
    cor_final = soma_v(&cor_final, &especular);
    cor_final = mult_v(cor_ponto, &cor_final);

    return cor_final;
}



cor_t calcular_iluminacao(ponto_t *origem_raio, vetor_t *direcao_raio, 
    luz_t *luz_local, luz_t *luz_ambiente, objeto_t *objetos, int num_objetos, 
    objeto_t *objeto_perto, ponto_t *ponto_intersec, vetor_t *normal)
{
    
    int j, luz_direta;
    vetor_t direcao_luz;
    luz_t luz_local_final;
    cor_t cor_final;
    double t0, t1;
    vetor_t temp1_v;
    
    // Percore os demais objetos para ver se há algum na frente.
    luz_direta = 1;
    
    // Calcula a direção do vetor que sai do ponto até a fonte de luz.
    direcao_luz = sub_v(&luz_local->posicao, ponto_intersec);
    direcao_luz = normalizar(&direcao_luz);
    
    for(j = 0; j < num_objetos; j++)
    {
        //t0 = INFINITO; //alteração
		//t1 = INFINITO; //alteração
		
        if (objetos[j].tipo == ESFERA)
        {
            if(intersecao_esfera(ponto_intersec, &direcao_luz, 
                objetos[j].esfera, &t0, &t1, &temp1_v))
            {
                luz_direta = 0;
            }
        }
        else if (objetos[j].tipo == PIRAMIDE)
        {
            if(intersecao_piramide(ponto_intersec, &direcao_luz, 
                objetos[j].piramide, &t0, &t1, 
                &temp1_v))
            {
                luz_direta = 0;
            }
        }
        else if (objetos[j].tipo == CUBO)
        {
			//normal = &temp1_v; // alterado
                        
            if(intersecao_cubo(ponto_intersec, &direcao_luz, 
                objetos[j].cubo, &t0, &t1, 
                &temp1_v))
            {
				//printf("normal: (%f, %f, %f) \n", temp1_v.x,temp1_v.y, temp1_v.z);  // alterado
                if(objeto_perto != &objetos[j])
                {
					luz_direta = 0;
                }
            }
        }
        else if (objetos[j].tipo == PLANO)
        {
            if(intersecao_plano(ponto_intersec, &direcao_luz, 
            objetos[j].plano, &t0))
            {
                if(objeto_perto != &objetos[j])
                {
                    luz_direta = 0;
                }
            }
        }
        else
        {
        }
        
       if (!luz_direta)
       {
           break;
       }
    }

    luz_local_final = *luz_local;
        
    if(!luz_direta)
    {
        luz_local_final.cor.x = 0.0;
        luz_local_final.cor.y = 0.0;
        luz_local_final.cor.z = 0.0;
    }

	
    cor_final = equacao_phong(origem_raio, &luz_local_final, luz_ambiente, 
        ponto_intersec, normal, &objeto_perto->cor);    
    
    return cor_final;
    
}

