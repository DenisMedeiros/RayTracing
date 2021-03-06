#ifndef GEOMETRIA_H
#define GEOMETRIA_H

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

/** Representa o maior valor de distância dos objetos. */
#define INFINITO 1.0e10

/** Valor de PI. */
#define PI 3.14159265358979323846

#define EPSILON 0.0001

/** 
 * Estrutura que define um vetor ou ponto no espaço. 
 * Ela contém apenas as 3 componentes para representar o vetor 
 * no espaço.
 */
typedef struct {
    double x;
    double y;
    double z;
} vetor_t;

/** 
 * Um ponto possui a mesma estrutura de um vetor no espaço. 
 * Logo, a estrutura do vetor pode ser reaproveitada.
 */
typedef vetor_t ponto_t;

/** 
 * Estrutura para armazenar um cor RGB normalizada (entre 0 e 1). 
 * Ela contem as 3 componentes de R, G e B, respectivamente. 
 */
typedef vetor_t cor_t;

/** 
 * Estrutura para armazenar uma esfera. 
 * 
 * Para ser representada, a esfera precisa de um centro e de um raio.
 */
typedef struct {
    ponto_t centro;
    double raio;
} esfera_t;


/** 
 * Estrutura para armazenar um triangulo. 
 * 
 * Ele precisa de 3 vértices para ser representado.
 * 
 * 
 */
typedef struct {
    ponto_t vertices[3];
} triangulo_t;

/** 
 * Estrutura para armazenar uma piramidade. 
 * 
 * Ela precisa de 4 vértices para ser representada.
 * 
 */
typedef struct {
    ponto_t vertices[4];
} piramide_t;

/** 
 * Estrutura para armazenar um cubo. 
 * 
 * Para ser representado, a esfera precisa de um centro e do tamanho do lado.
 * Os vértices devem ser fornecidos iniciando de cima para baixo, depois esquerda
 * para direita e por fim da frente para trás.
 * 
 */
typedef struct {
    ponto_t vertices[8];
} cubo_t;


/** 
 * Estrutura para armazenar um plano. 
 * 
 */
typedef struct {
    ponto_t ponto;
    vetor_t normal;
} plano_t;


/** 
 * Estrutura para armazenar um objeto (pode ser esfera ou cubo).
 * 
 */
typedef struct 
{
    enum {ESFERA, PIRAMIDE, CUBO, PLANO} tipo;
    
    union 
    {
        esfera_t *esfera;
        piramide_t *piramide;
        cubo_t *cubo;
        plano_t *plano;
    };
    
    cor_t cor;
    char refletivel;
    
} objeto_t;

/** 
 * Estrutura para definir a fonte de luz.
 * 
 */
typedef struct {
    ponto_t posicao;
    cor_t cor;
} luz_t;


/** 
 * Esta função faz a soma de dois vetores.
 * 
 * @param v1 Ponteiro para o primeiro vetor.
 * @param v2 Ponteiro para o segundo vetor.
 * @return Resultado da soma de v1 e v2 (v1 + v2).
 */
vetor_t soma_v(vetor_t *v1, vetor_t *v2);

/** 
 * Esta função faz a subtração de dois vetores.
 * 
 * @param v1 Ponteiro para o primeiro vetor.
 * @param v2 Ponteiro para o segundo vetor.
 * @return Resultado da subtração de v1 por v2 (v1 - v2).
 */
vetor_t sub_v(vetor_t *v1, vetor_t *v2);

/** 
 * Esta função faz a multiplicação de um vetor por um escalar.
 * 
 * @param v1 Ponteiro para o vetor a ser multiplicado.
 * @param k Escalar que multiplica o vetor v1.
 * @return Produto de k por v1.
 */
vetor_t mult_e(vetor_t *v1, double k);

/** 
 * Esta função faz a multiplicação elemento a elemento de dois vetores.
 * 
 * @param v1 Ponteiro para o primeiro vetor.
 * @param v2 Ponteiro para o segundo vetor.
 * @return Resultado da multiplicação elemento a elemento de v1 e v2.
 */
vetor_t mult_v(vetor_t *v1, vetor_t *v2);

/** 
 * Esta função faz negação do vetor.
 * 
 * @param v1 Ponteiro para o vetor.
 * @return Vetor v1 com o sentido invertido (-v1).
 */
vetor_t neg_v(vetor_t *v1);

/** 
 * Esta função retorna o módulo (norma) do vetor.
 * 
 * @param v1 Ponteiro para o vetor.
 * @return Módulo (norma) do vetor v1.
 */
double modulo(vetor_t *v1);

/** 
 * Esta função normaliza um vetor, isto é, faz com que seu módulo seja
 * igual a 1.
 * 
 * @param v1 Ponteiro para o vetor a ser normalizado.
 * @return Vetor v1 normalizado.
 */
vetor_t normalizar(vetor_t *v1);

/** 
 * Esta função faz o produto escalar (dot) entre dois vetores.
 * 
 * @param v1 Ponteiro para o primeiro vetor.
 * @param v2 Ponteiro para o segundo vetor.
 * @return Resultado do produto escalar entre v1 e v2.
 */
double prod_e(vetor_t *v1, vetor_t *v2);


/** 
 * Esta função faz o produto vetorial entre dois vetores.
 * 
 * @param v1 Ponteiro para o primeiro vetor.
 * @param v2 Ponteiro para o segundo vetor.
 * @return Resultado do produto escalar entre v1 e v2.
 */
vetor_t prod_v(vetor_t *v1, vetor_t *v2);

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
    esfera_t *esfera, double *t0, double *t1, vetor_t *normal);


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
    piramide_t *piramide, double *t0, double *t1, vetor_t *normal);



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
int intersecao_cubo(ponto_t *origem_raio, vetor_t *direcao_raio, cubo_t *cubo, 
    double *t0, double *t1, vetor_t *normal);

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
    triangulo_t *triangulo, double *t0, vetor_t *normal);
    
    
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
    plano_t *plano, double *t0);    

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
    int max_recursoes);


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
    cor_t *cor_ponto);
    
    
/**
 * Equação de Phong da iluminação, que serve para calcular a cor do objeto.
 * 
 * @param origem_raio Ponteiro para o ponto no espaço de onde o 
 * raio parte.
 * @param luz_local Ponteiro para a luz local (pontual).
 * @param luz_ambiente Ponteiro para a luz ambiente.
 * @param objetos Array de objetos.
 * @param num_objetos Número e objetos no array.
 * @param objeto_perto Objeto mais perto da câmera.
 * @param ponto_intersec Ponto de interseção entre o raio e o objeto.
 * @param normal Vetor normal que indica o plano onde está o ponto.
 * 
 */ 
cor_t calcular_iluminacao(ponto_t *origem_raio, vetor_t *direcao_raio, 
    luz_t *luz_local, luz_t *luz_ambiente, objeto_t *objetos, int num_objetos, 
    objeto_t *objeto_perto, ponto_t *ponto_intersec, vetor_t *normal);

#endif // GEOMETRIA_H


