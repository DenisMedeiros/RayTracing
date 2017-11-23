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
int intersecao_cubo(ponto_t *origem_raio, vetor_t *direcao_raio, cubo_t *cubo, double *t0, double *t1)
{
    
    vetor_t distancia;
    
    // Construindo os triângulos
    triangulo_t triangulos[12];
    
    triangulos[0].vertices[0] = cubo->vertices[0];
    triangulos[0].vertices[1] = cubo->vertices[1];
    triangulos[0].vertices[2] = cubo->vertices[3];
    
    triangulos[1].vertices[0] = cubo->vertices[0];
    triangulos[1].vertices[1] = cubo->vertices[2];
    triangulos[1].vertices[2] = cubo->vertices[3];
    
    triangulos[2].vertices[0] = cubo->vertices[0];
    triangulos[2].vertices[1] = cubo->vertices[2];
    triangulos[2].vertices[2] = cubo->vertices[4];
    
    triangulos[3].vertices[0] = cubo->vertices[2];
    triangulos[3].vertices[1] = cubo->vertices[4];
    triangulos[3].vertices[2] = cubo->vertices[6];
    
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
cor_t raytrace(ponto_t *origem_raio, vetor_t *direcao_raio, objeto_t *objetos, luz_t *luz, int num_objetos, int num_recursoes, int max_recursoes)
{
    cor_t cor, cor_reflexao;
    double tperto, t0, t1;
    int i, j, luz_direta;
    objeto_t *objeto_perto;
    vetor_t normal, refletido, direcao_luz;
    ponto_t ponto_intersec;
    
    // Variáveis auxiliares paras as funções vetoriais
    vetor_t temp1_v, temp2_v, temp3_v;
    float temp4_f;
 
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
        
        if (t0 < 0) // Caso o raio tenha intersectado a borda.
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
    
    // Parte para determinar a cor do pixel com base na iluminação.
    switch(objeto_perto->tipo)
    {
    case ESFERA:
        
        // Calcula o ponto de intersecção do raio e da esfera.
        temp1_v = mult_e(direcao_raio, tperto);
        ponto_intersec = soma_v(origem_raio, &temp1_v);
        
        // Calcula o vetor normal e normaliza-o.
        normal = sub_v(&ponto_intersec, &objeto_perto->esfera->centro);
        normal = normalizar(&normal);
        
        // Inverte o sentido da normal caso ela esteja dentro da esfera.
        if(prod_e(direcao_raio, &normal) > 0)
        {
                normal = neg_v(&normal);
        }
        
        // Parte recursiva do raytracing.
        if(objeto_perto->refletivel && num_recursoes < max_recursoes)
        {
            // Calcula a direção do raio refletido e normaliza-o.
            temp1_v = mult_e(&normal, 2 * prod_e(direcao_raio, &normal));
            refletido = sub_v(direcao_raio, &temp1_v);
            refletido = normalizar(&refletido);    

            cor_reflexao = raytrace(&ponto_intersec, &refletido, objetos, luz, num_objetos, num_recursoes + 1, max_recursoes);

            if(cor_reflexao.x == -1)
            {
                // Calcula a direção do vetor que sai do ponto até a fonte de luz.
                direcao_luz = sub_v(&luz->posicao, &ponto_intersec);
                direcao_luz = normalizar(&direcao_luz);

                temp4_f = max(0.0f, prod_e(&normal, &direcao_luz)); // Entre 0 e 1
                temp3_v = mult_e(&objeto_perto->cor, temp4_f); // Multiplica cor por esse fator (um peso entre 0 e 1)
                cor = mult_v(&temp3_v, &luz->cor);         
            }
            else
            {
                temp1_v = mult_e(&objeto_perto->cor, 0.7);
                temp2_v = mult_e(&cor_reflexao, 0.3);
                cor = soma_v(&temp1_v, &temp2_v);

                cor = mult_e(&cor_reflexao, brilho(&objeto_perto->cor));
                
                //cor = mult_v(&cor_reflexao, &objeto_perto->cor);
            }
        }
        else // Calculo da cor em fução da luz.
        {
            // Calcula a direção do vetor que sai do ponto até a fonte de luz.
            direcao_luz = sub_v(&luz->posicao, &ponto_intersec);
            direcao_luz = normalizar(&direcao_luz);
            
            // Percore os demais objetos para ver se há algum na frente.
            luz_direta = 1;
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
            
            temp2_v = mult_e(&objeto_perto->cor, luz_direta); // Cor do objeto ou sombra
            temp4_f = max(0.0f, prod_e(&normal, &direcao_luz)); // Entre 0 e 1
            temp3_v = mult_e(&temp2_v, temp4_f); // Multiplica cor por esse fator (um peso entre 0 e 1)
            cor = mult_v(&temp3_v, &luz->cor);
            
        }
            
        break;
    case CUBO:
        break;
    default:
        break;
    }
      
    return cor;
    
}


float brilho(cor_t *cor)
{
    return (cor->x + cor->y + cor->z)/3.0;
}
