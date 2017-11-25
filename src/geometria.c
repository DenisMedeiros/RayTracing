#include "geometria.h"
#include <math.h>
#include <stdio.h>

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

void imprimir_vetor(const char *nome, vetor_t *v1)
{
    printf("%s: (%lf, %lf, %lf)\n", nome, v1->x, v1->y, v1->z);
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
 * Verifica se um determinado raio intersecta uma piramide no espaço.
 * 
 * @param origem_raio Ponteiro para o ponto no espaço de onde o 
 * raio parte.
 * @param direcao_raio Ponteiro para o vetor que determina a direção
 * do raio.
 * @param esf Ponteiro para o piramide a ser intersectado.
 * @param t0 Ponteiro para a distância horizontal entre o ponto de 
 * origem e o primeiro ponto de interseção (é modificada na função).
 * @param t1 Ponteiro para a distância horizontal entre o ponto de 
 * origem e o segundo ponto de interseção (é modificada na função).
 * @return 1 se o raio intersecta o piramide, 0 caso contrário.
 */ 
 
// fiz uma simples modificação: em vez de usar *t0 direto, eu ultilizei uma variavel temporaria temp_t0 
int intersecao_triangulo(ponto_t *origem_raio, vetor_t *direcao_raio, triangulo_t *triangulo, double *t0, vetor_t *normal)
{
    vetor_t v1, v2, temp1_v, temp2_v;
    ponto_t ponto_intersec;
    double denominador;
    double d, t0_temp;
    
    
    v1 = sub_v(&triangulo->vertices[1], &triangulo->vertices[0]);
    v2 = sub_v(&triangulo->vertices[2], &triangulo->vertices[0]);
    
    *normal = prod_v(&v1, &v2); // Vetor normal não normalizado
    *normal = normalizar(normal);
	
    // Step 1: finding P
 
    // check if ray and plane are parallel ?
    denominador = prod_e(normal, direcao_raio);
    if (fabs(denominador) < 0.01)
    {
	return 0;
    }
   
    d = prod_e(normal, &triangulo->vertices[0]);
    t0_temp = (d - prod_e(normal, origem_raio))/denominador;
    
    // check if the triangle is in behind the ray
    if(t0_temp < 0)
    {
	    return 0;
    }
    
    // compute the intersection point using equation 1
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
 * @param esf Ponteiro para o piramide a ser intersectado.
 * @param t0 Ponteiro para a distância horizontal entre o ponto de 
 * origem e o primeiro ponto de interseção (é modificada na função).
 * @param t1 Ponteiro para a distância horizontal entre o ponto de 
 * origem e o segundo ponto de interseção (é modificada na função).
 * @return 1 se o raio intersecta o piramide, 0 caso contrário.
 */ 
int intersecao_piramide(ponto_t *origem_raio, vetor_t *direcao_raio, piramide_t *piramide, double *t0, double *t1, vetor_t *normal)
{
    double temp;
    int i, contagem;
    
    contagem = 0;
    
    // Construindo os triângulos
    triangulo_t triangulos[4];
    
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
    
    for(i = 0; i < 4; i++) //modifiquei alterando o valor de i < 2 para i < 4
    {
        if(intersecao_triangulo(origem_raio, direcao_raio, &triangulos[i], t0, normal))
        {
            contagem++;
            
            if(contagem == 1)
            {
                *t1 = *t0;
            }
            else if (contagem == 2)
            {
                if(*t1 < *t0)
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
cor_t raytrace(ponto_t *origem_raio, vetor_t *direcao_raio, objeto_t *objetos, luz_t *luz_local, luz_t *luz_ambiente, int num_objetos, int num_recursoes, int max_recursoes)
{
    cor_t cor;
    double tperto, t0_esfera, t1_esfera, t0_piramide, t1_piramide;
    int i, j, luz_direta;
    objeto_t *objeto_perto;
    vetor_t normal, normal_pir, direcao_luz;
    ponto_t ponto_intersec;
       
    // Variáveis auxiliares paras as funções vetoriais
    vetor_t temp1_v, temp2_v, temp3_v;
    float temp4_f;
 
    objeto_perto = 0;
    tperto = INFINITO; 
    //tperto_piramide = INFINITO;

    /* Se não tocar nenhum objeto, então a cor será negativa. */
    cor.x = -1.0;
    cor.y = -1.0;
    cor.z = -1.0;
    
    /* Encontra o objeto mais perto da câmera (caso exista). */
    for (i = 0; i < num_objetos; ++i) 
    {
        t0_esfera = INFINITO; 
        t1_esfera = INFINITO;
	t0_piramide = INFINITO; 
        t1_piramide = INFINITO;
        
        switch(objetos[i].tipo)
        {
        case ESFERA:
            intersecao_esfera(origem_raio, direcao_raio, objetos[i].esfera, &t0_esfera, &t1_esfera);
            
	    if(t0_esfera == INFINITO)
	    {
		break;
	    }
	    
	    if (t0_esfera  < 0) // Caso o raio tenha intersectado a borda.
	    { 
		t0_esfera  = t1_esfera;
	    }
	    
	    if (t0_esfera < tperto)
	    {
		tperto = t0_esfera;		
		objeto_perto = &objetos[i];
	    }
			
            
            break;
        case PIRAMIDE:
            intersecao_piramide(origem_raio, direcao_raio, objetos[i].piramide, &t0_piramide, &t1_piramide, &normal_pir);
                        
            if(t0_piramide == INFINITO)
	    {
		break;
	    }
	    
	    if (t0_piramide  < 0) // Caso o raio tenha intersectado a borda.
	    { 
		t0_piramide  = t1_piramide;
	    }
	    
	    if (t0_piramide < tperto)
	    {
		tperto = t0_piramide;
		objeto_perto = &objetos[i];
		normal = normal_pir;
	    }
			
            break;
        default:
            break;
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
        
	// Calcula a direção do vetor que sai do ponto até a fonte de luz.
	direcao_luz = sub_v(&luz_local->posicao, &ponto_intersec);
	direcao_luz = normalizar(&direcao_luz);
	
	// Percore os demais objetos para ver se há algum na frente.
	luz_direta = 1;
	for(j = 0; j < num_objetos; j++)
	{
		switch(objetos[j].tipo)
		{
		case ESFERA:
		    if(intersecao_esfera(&ponto_intersec, &direcao_luz, objetos[j].esfera, &t0_esfera, &t1_esfera))
		    {
			    luz_direta = 0;
		    }
		    break;
		case PIRAMIDE:
		    if(intersecao_piramide(&ponto_intersec, &direcao_luz, objetos[j].piramide, &t0_piramide, &t1_piramide, &normal_pir))
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
	cor = mult_v(&temp3_v, &luz_local->cor);
		    
        break;
        
    case PIRAMIDE:
    	
	// Calcula o ponto de intersecção do raio e da pirâmide.
        temp1_v = mult_e(direcao_raio, tperto);
        ponto_intersec = soma_v(origem_raio, &temp1_v);
   
	// Percore os demais objetos para ver se há algum na frente.
	luz_direta = 1;
	for(j = 0; j < num_objetos; j++)
	{
		switch(objetos[j].tipo)
		{
		case ESFERA:
		    if(intersecao_esfera(&ponto_intersec, &direcao_luz, objetos[j].esfera, &t0_esfera, &t1_esfera))
		    {
			luz_direta = 0;
		    }
		    break;
		case PIRAMIDE:
		    if(intersecao_piramide(&ponto_intersec, &direcao_luz, objetos[j].piramide, &t0_piramide, &t1_piramide, &normal_pir))
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
	normal = normalizar(&normal);
	cor = calcular_iluminacao(origem_raio, luz_local, luz_ambiente, &ponto_intersec, &normal, &temp2_v, &luz_local->cor);
        break;		
		
    
    default:
        break;
    }
      
    return cor;
    
}


cor_t calcular_iluminacao(ponto_t *origem_raio, luz_t *luz_local, luz_t *luz_ambiente, ponto_t *pos_ponto, vetor_t *normal_ponto, cor_t *cor_ponto, vetor_t *especular_ponto)
{
    cor_t ambiente;
    cor_t difusa;
    cor_t especular;
    
    cor_t cor_final;
        
    vetor_t dir_luz;
    vetor_t dir_obs;
    vetor_t refletido;
    double temp1_d;
    vetor_t temp1_v;
    
    // Direção para o observador.
    dir_obs = sub_v(origem_raio, pos_ponto);
    dir_obs = normalizar(&dir_obs);
    
    // Calcula a direção do raio refletido e normaliza-o.
    dir_luz = neg_v(&dir_luz);
    temp1_v = mult_e(normal_ponto, 2 * prod_e(&dir_luz, normal_ponto));
    refletido = sub_v(&dir_luz, &temp1_v);
    refletido = normalizar(&refletido);    
    
    // Calcula a direção da luz (ponto intersec até a luz).
    dir_luz = sub_v(&luz_local->posicao, pos_ponto);
    dir_luz = normalizar(&dir_luz);
    
    // Calcula a luz ambiente;.
    ambiente = mult_e(&luz_ambiente->cor, ka);
    
    // Calcula a luz difusa
    difusa = mult_e(&luz_local->cor, kd * max(0.0f, prod_e(normal_ponto, &dir_luz)));

    // Calcula a luz especular
    temp1_d = max(0.0f, prod_e(&refletido, &dir_obs));
    especular = mult_e(&luz_local->cor, os * ks * pow(temp1_d, eta));
    
    
    //imprimir_vetor("ambiente", &ambiente);
    //imprimir_vetor("difusa", &difusa);
    //imprimir_vetor("especular", &especular);
    
    
    cor_final = soma_v(&ambiente, &difusa);
    cor_final = soma_v(&cor_final, &especular);
    
    cor_final = mult_v(cor_ponto, &cor_final);

    
    //imprimir_vetor("final", &cor_final);
    
    return cor_final;
}




