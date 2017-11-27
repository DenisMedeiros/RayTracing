#include <stdio.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "geometria.h"

/** Configurações dos objetos. */
#define NUM_ESFERAS 4
#define NUM_PIRAMIDES 1
#define NUM_CUBOS 1
#define NUM_PLANOS 1
#define NUM_OBJETOS (NUM_ESFERAS + NUM_PIRAMIDES + NUM_CUBOS + NUM_PLANOS)

/** Configurações de visualização (câmera). */
#define Z_NEAR 1.0
#define Z_FAR 80.0

#define LF_X 0.0 // Look from x
#define LF_Y 0.0 // Look from y
#define LF_Z 10.0 // Look from z

#define LA_X 0.0 // Look at x
#define LA_Y 0.0 // Look at y
#define LA_Z 0.0 // Look at z

/** Cor de fundo da cena. */
#define FUNDO_R 0.0
#define FUNDO_G 0.0
#define FUNDO_B 0.0

/** Configurações da movimentação sob a cena. */
#define PASSO_PAN 0.1 // Em metros
#define PASSO_GIRO 15 // 15°

/** Configurações da recursão. */
#define MAX_REC 0

/** Configurações da animação. */
//#define GERAR_ANIMACAO
//#define SCREEN_FPS 24
 
/** Variáveis globais. */
luz_t luz_ambiente; // Luz ambiente
luz_t luz_local; // Fonte de luz local (pontual)

objeto_t objetos[NUM_OBJETOS]; // Lista de objetos
float *pixels; // Matriz de píxels de 3 canais.
int altura, largura;

double ka; // Coeficiente da luz ambiente.
double kd; // Coeficiente da luz difusa.
double ks; // Coeficiente da luz especular.
double eta; // Índice de brilho
double os; // Propriedade de reflexão do material

#ifdef GERAR_ANIMACAO
/** Função que faz as mudanças da animação. */
void loop(int x)
{ 
    glRotatef(-5.0, 1.0, 0.0, 0.0);
    glRotatef(6.0, 0.0, 1.0, 0.0);
    glRotatef(-7.0, 0.0, 0.0, 1.0);
    glutPostRedisplay();
    glutTimerFunc(1000 / SCREEN_FPS, loop, 0); // Configura o timer novamente.
}
#endif

void init(void) 
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glShadeModel(GL_SMOOTH);
#ifdef GERAR_ANIMACAO
    glutTimerFunc(1000 / SCREEN_FPS, loop, 0);
#endif
}

void display(void)
{
    GLint view_port[4];  
    GLdouble model_view[16];
    GLdouble projection[16]; 
    GLdouble win_x, win_y; // Coornadas x e y.
    GLdouble x_near, y_near, z_near;
    GLdouble x_far, y_far, z_far;

    int i, j, nova_largura, nova_altura;
    ponto_t origem; // Ponto de origem
    vetor_t dir; // Vetor direção.
    cor_t pixel;
    
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();    
        
    // Fazendo a rasterização.
    glGetIntegerv(GL_VIEWPORT, view_port); // Obtém x, y, largura e altura.
    glGetDoublev(GL_MODELVIEW_MATRIX, model_view); 
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    
    nova_largura = view_port[2];
    nova_altura = view_port[3];
    
    // Verifica se a janela mudou de tamanho.
    if(nova_largura != largura || nova_altura != altura)       
    {
        altura = nova_altura;
        largura = nova_largura;
        
        if(pixels != NULL)
        {
            free(pixels);
            pixels = NULL;
        }
        
        pixels = (float *) malloc(altura * largura * 3 * sizeof(float));
    }
    
    // Define os pontos na tela.
    win_x = 0;
    win_y = 0;


    for(i = 0; i < altura; i++) // Percorre as linhas (altura)
    {
        for(j = 0; j < largura; j++) // Percorre as colunas (largura)
        {
            win_x = j;
            win_y = i;
            
            // Obtém o ponto no plano near.
            gluUnProject(win_x, win_y, 0.0, model_view, projection, view_port, &x_near, &y_near, &z_near);
            
            // Obtém o ponto no plano far.
            gluUnProject(win_x, win_y, 1.0, model_view, projection, view_port, &x_far, &y_far, &z_far);
            
            // Cria o vetor origem
            origem.x = x_near;
            origem.y = y_near;
            origem.z = z_near;
            
            // Cria o vetor direção.
            dir.x = x_far - x_near;
            dir.y = y_far - y_near;
            dir.z = z_far - z_near;

            // Normaliza o vetor direção.
            dir = normalizar(&dir);
            
            // Faz o raytracing.
            pixel = raytrace(&origem, &dir, &luz_local, &luz_ambiente,  objetos, NUM_OBJETOS, 0, MAX_REC);
            
            // Verifica se veio alguma cor (se não, não houve insersecção.
            if(pixel.x != -1)
            {
                pixels[(i * largura * 3) + (j * 3) + 0] = pixel.x;
                pixels[(i * largura * 3) + (j * 3) + 1] = pixel.y;
                pixels[(i * largura * 3) + (j * 3) + 2] = pixel.z;    
            }
            else
            {
                pixels[(i * largura * 3) + (j * 3) + 0] = FUNDO_R;
                pixels[(i * largura * 3) + (j * 3) + 1] = FUNDO_G;
                pixels[(i * largura * 3) + (j * 3) + 2] = FUNDO_B;                  
            }
            
        }
    }

    glDrawPixels(view_port[2], view_port[3], GL_RGB, GL_FLOAT, pixels);
    glPopMatrix();
    glutSwapBuffers();
    glutSwapBuffers();

}

void reshape (int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h); 
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60.0, (GLfloat) w/(GLfloat) h, Z_NEAR, Z_FAR);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt (LF_X, LF_Y, LF_Z, LA_X, LA_Y, LA_Z, 0.0, 1.0, 0.0);
}

void keyboard (unsigned char key, int x, int y)
{
    switch (key) 
    {
    case 'a':
        glTranslatef(-PASSO_PAN, 0, 0);
        break;
    case 'd':
        glTranslatef(PASSO_PAN, 0, 0);
        break;
    case 's':
        glTranslatef(0, -PASSO_PAN, 0);
        break;
    case 'w':
        glTranslatef(0, PASSO_PAN, 0);
        break; 
    case 'o':
        glTranslatef(0, 0, -PASSO_PAN);
        break;
    case 'p':
        glTranslatef(0, 0, PASSO_PAN);
        break;         
    case 'j':
        glRotatef(PASSO_GIRO, 1.0, 0.0, 0.0);
        break;
    case 'k':
        glRotatef(PASSO_GIRO, 0.0, 1.0, 0.0);
        break;    
    case 'l':
        glRotatef(PASSO_GIRO, 0.0, 0.0, 1.0);
        break;    
    default:
        break;
    }
         
    glutPostRedisplay();

}

int main(int argc, char** argv)
{
    int i;
    // Criação dos objetos.
    
    objetos[0].tipo = ESFERA;
    objetos[0].esfera = malloc(sizeof(esfera_t));
    objetos[0].esfera->centro.x = 0.0;
    objetos[0].esfera->centro.y = 0.0;
    objetos[0].esfera->centro.z = 0.0;
    objetos[0].esfera->raio = 1;
    objetos[0].cor.x = 1.0;
    objetos[0].cor.y = 0.0;
    objetos[0].cor.z = 0.0; 
    objetos[0].refletivel = 1;
    
    objetos[1].tipo = ESFERA;
    objetos[1].esfera = malloc(sizeof(esfera_t));
    objetos[1].esfera->centro.x = -2.0;
    objetos[1].esfera->centro.y = 0.0;
    objetos[1].esfera->centro.z = 1.0;
    objetos[1].esfera->raio = 1;
    objetos[1].cor.x = 0.0;
    objetos[1].cor.y = 0.0;
    objetos[1].cor.z = 1.0;
    objetos[1].refletivel = 1; 
    
    objetos[2].tipo = ESFERA;
    objetos[2].esfera = malloc(sizeof(esfera_t));
    objetos[2].esfera->centro.x = 0.0;
    objetos[2].esfera->centro.y = -3.0;
    objetos[2].esfera->centro.z = -1.0;
    objetos[2].esfera->raio = 1;
    objetos[2].cor.x = 0.0;
    objetos[2].cor.y = 1.0;
    objetos[2].cor.z = 0.0;
    objetos[2].refletivel = 1; 

    objetos[3].tipo = ESFERA;
    objetos[3].esfera = malloc(sizeof(esfera_t));
    objetos[3].esfera->centro.x = 0.0;
    objetos[3].esfera->centro.y = 3.0;
    objetos[3].esfera->centro.z = 0.0;
    objetos[3].esfera->raio = 1;
    objetos[3].cor.x = 1.0;
    objetos[3].cor.y = 1.0;
    objetos[3].cor.z = 0.0;
    objetos[3].refletivel = 1; 
    
    objetos[4].tipo = PIRAMIDE;
    objetos[4].piramide = malloc(sizeof(piramide_t));
    objetos[4].piramide->vertices[0].x = 0.0;    
    objetos[4].piramide->vertices[0].y = 0.0;  
    objetos[4].piramide->vertices[0].z = -8.0; 
    objetos[4].piramide->vertices[1].x = 4.0;  
    objetos[4].piramide->vertices[1].y = 0.0; 
    objetos[4].piramide->vertices[1].z = -8.0;  
    objetos[4].piramide->vertices[2].x = 2.0;     
    objetos[4].piramide->vertices[2].y = 4.0; 
    objetos[4].piramide->vertices[2].z = -8.0; 
    objetos[4].piramide->vertices[3].x = 2.0; // Crista
    objetos[4].piramide->vertices[3].y = 2.0; // Crista
    objetos[4].piramide->vertices[3].z = -6.0; // Crista
    objetos[4].cor.x = 0.7;
    objetos[4].cor.y = 0.7;
    objetos[4].cor.z = 0.2;
    objetos[4].refletivel = 1;
    
    objetos[5].tipo = PLANO;
    objetos[5].plano = malloc(sizeof(plano_t));
    objetos[5].plano->ponto.x = 0.0;
    objetos[5].plano->ponto.y = 0.0;
    objetos[5].plano->ponto.z = -20.0;
    objetos[5].plano->normal.x = 0.0;
    objetos[5].plano->normal.y = 1.0;
    objetos[5].plano->normal.z = 1.0;    
    objetos[5].cor.x = 1.0;
    objetos[5].cor.y = 1.0;
    objetos[5].cor.z = 1.0;
    objetos[5].refletivel = 1;
    
    objetos[6].tipo = CUBO;
    objetos[6].cubo = malloc(sizeof(cubo_t));
    
    objetos[6].cubo->vertices[0].x = -3.0;
    objetos[6].cubo->vertices[0].y = 5.0;
    objetos[6].cubo->vertices[0].z = -1.0;

    objetos[6].cubo->vertices[1].x = -3.0;
    objetos[6].cubo->vertices[1].y = 3.0;
    objetos[6].cubo->vertices[1].z = -1.0;

    objetos[6].cubo->vertices[2].x = -1.0;
    objetos[6].cubo->vertices[2].y = 5.0;
    objetos[6].cubo->vertices[2].z = -1.0;
    
    objetos[6].cubo->vertices[3].x = -1.0;
    objetos[6].cubo->vertices[3].y = 3.0;
    objetos[6].cubo->vertices[3].z = -1.0;
    
    objetos[6].cubo->vertices[4].x = -3.0;
    objetos[6].cubo->vertices[4].y = 5.0;
    objetos[6].cubo->vertices[4].z = 2.0;

    objetos[6].cubo->vertices[5].x = -3.0;
    objetos[6].cubo->vertices[5].y = 3.0;
    objetos[6].cubo->vertices[5].z = 2.0;

    objetos[6].cubo->vertices[6].x = -1.0;
    objetos[6].cubo->vertices[6].y = 5.0;
    objetos[6].cubo->vertices[6].z = 2.0;
    
    objetos[6].cubo->vertices[7].x = -1.0;
    objetos[6].cubo->vertices[7].y = 3.0;
    objetos[6].cubo->vertices[7].z = 2.0;

    objetos[6].cor.x = 1.0;
    objetos[6].cor.y = 0.0;
    objetos[6].cor.z = 0.0;
    
    objetos[6].refletivel = 1; 
        
    // Parâmetros da equação de Phong.
    ka = 0.4;
    kd = 0.2;
    ks = 0.4;
    eta = 1.0;
    os = 0.8;
    
    // Luz pontual.
    luz_local.posicao.x = 0.0; 
    luz_local.posicao.y = -0.5;  
    luz_local.posicao.z = 10.0; 
    luz_local.cor.x = 1.0;
    luz_local.cor.y = 1.0;
    luz_local.cor.z = 1.0;
    
    // Luz ambiente.
    luz_ambiente.cor.x = 1.0;
    luz_ambiente.cor.y = 1.0;
    luz_ambiente.cor.z = 1.0;
    
    // Propriedades da janela.
    largura = 400;
    altura = 400;

    pixels = (float *) malloc(altura * largura * 3 * sizeof(float));
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(largura, altura); 
    glutInitWindowPosition (100, 100);
    glutCreateWindow (argv[0]);
    init();
    glutDisplayFunc(display); 
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    // Libera a memória alocada ao final.
    free(pixels);
    for(i = 0; i < NUM_OBJETOS; i++)
    {
        if (objetos[i].tipo == ESFERA)
        {
            free(objetos[i].esfera);
        }
        else if (objetos[i].tipo == PIRAMIDE)        
        {
            free(objetos[i].piramide);
        }
        else if (objetos[i].tipo == CUBO)        
        {
            free(objetos[i].cubo);
        }        
        else if (objetos[i].tipo == PLANO)        
        {
            free(objetos[i].plano);
        }        
    }

    return 0;
}
