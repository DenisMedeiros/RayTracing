#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include "geometria.h"

/* Configurações básicas. */

#define NUM_ESFERAS 4
#define NUM_PIRAMIDES 1
#define NUM_OBJETOS (NUM_ESFERAS + NUM_PIRAMIDES)

#define Z_NEAR 1.0
#define Z_FAR 80.0

#define LF_X 0.0 // Look from x
#define LF_Y 0.0 // Look from y
#define LF_Z 10.0 // Look from z

#define LA_X 0.0 // Look at x
#define LA_Y 0.0 // Look at y
#define LA_Z 0.0 // Look at z

#define FUNDO_R 1.0
#define FUNDO_G 1.0
#define FUNDO_B 1.0

#define PASSO_PAN 0.1 // Em metros
#define PASSO_GIRO 15 // 15°

#define MAX_REC 0

//#define GERAR_ANIMACAO
//#define SCREEN_FPS 24
 
/* Variáveis globais. */
luz_t luz_ambiente; // Fonte de luz
luz_t luz_local; // Fonte de luz

objeto_t objetos[NUM_OBJETOS]; // Lista de objetos
float *pixels; // Matriz de píxels de 3 canais.
int altura, largura;

double ka;
double kd;
double ks;
double eta;
double os;

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
            pixel = raytrace(&origem, &dir, objetos, &luz_local, &luz_ambiente, NUM_OBJETOS, 0, MAX_REC);
			
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
    
    // Criação das esferas
    objetos[0].tipo = ESFERA;
    objetos[0].esfera = malloc(sizeof(esfera_t));
    objetos[0].esfera->centro.x = 1.0;
    objetos[0].esfera->centro.y = 0.0;
    objetos[0].esfera->centro.z = 0.0;
    objetos[0].esfera->raio = 1;
    objetos[0].cor.x = 1.0;
    objetos[0].cor.y = 0.0;
    objetos[0].cor.z = 0.0; 
	objetos[0].refletivel = 1;
	//*/
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
    objetos[2].esfera->centro.y = -2.0;
    objetos[2].esfera->centro.z = -1.0;
    objetos[2].esfera->raio = 1;
    objetos[2].cor.x = 0.0;
    objetos[2].cor.y = 1.0;
    objetos[2].cor.z = 0.0;
    objetos[2].refletivel = 1; 

    objetos[3].tipo = ESFERA;
    objetos[3].esfera = malloc(sizeof(esfera_t));
    objetos[3].esfera->centro.x = 0.0;
    objetos[3].esfera->centro.y = 2.0;
    objetos[3].esfera->centro.z = 0.0;
    objetos[3].esfera->raio = 1;
    objetos[3].cor.x = 1.0;
    objetos[3].cor.y = 1.0;
    objetos[3].cor.z = 0.0;
    objetos[3].refletivel = 1; 
    //*/ 
    
    objetos[4].tipo = PIRAMIDE;
    objetos[4].piramide = malloc(sizeof(piramide_t));
    
    objetos[4].piramide->vertices[0].x = 5.0;  //0.0  
	objetos[4].piramide->vertices[0].y = -2.0;  //0.0
	objetos[4].piramide->vertices[0].z = 0.0; // -8.0  

    objetos[4].piramide->vertices[1].x = 9.0; //2.0   
	objetos[4].piramide->vertices[1].y = -2.0; //0.0
	objetos[4].piramide->vertices[1].z = 0.0; //-8.0  
	
	objetos[4].piramide->vertices[2].x = 7.0; //1.0    
	objetos[4].piramide->vertices[2].y = -2.0; //2.0
	objetos[4].piramide->vertices[2].z = -2.0; //-8.0

	// Crista
	objetos[4].piramide->vertices[3].x = 7.0; //1.0
	objetos[4].piramide->vertices[3].y = 2.0; //1.0
	objetos[4].piramide->vertices[3].z = 0.0; //-6.0

    objetos[4].cor.x = 0.7;
    objetos[4].cor.y = 0.7;
    objetos[4].cor.z = 0.2;
    
    objetos[4].refletivel = 1;
    
    /*
    objetos[5].tipo = PIRAMIDE;
    objetos[5].piramide = malloc(sizeof(piramide_t));
    
    objetos[5].piramide->vertices[0].x = 5.0;
	objetos[5].piramide->vertices[0].y = 0.0;
	objetos[5].piramide->vertices[0].z = -9.0;

    objetos[5].piramide->vertices[1].x = 7.0;
	objetos[5].piramide->vertices[1].y = 0.0;
	objetos[5].piramide->vertices[1].z = -9.0;

	objetos[5].piramide->vertices[2].x = 6.0;
	objetos[5].piramide->vertices[2].y = 2.0;
	objetos[5].piramide->vertices[2].z = -9.0;

	// Crista
	objetos[5].piramide->vertices[3].x = 6.0;
	objetos[5].piramide->vertices[3].y = 1.0;
	objetos[5].piramide->vertices[3].z = -7.0;

    objetos[5].cor.x = 1.0;
    objetos[5].cor.y = 0.5;
    objetos[5].cor.z = 0.5;
    
    objetos[5].refletivel = 1;
	*/
    
    // Soma deve dar 1.0? não necessariamente kkk
    ka = 0.8;
    kd = 0.2;
    ks = 0.6;
    
    eta = 1.0;
    os = 0.5;
    
    luz_local.posicao.x = 0.0; //0.0
    luz_local.posicao.y = 0.0;  //0.0
    luz_local.posicao.z = -10.0; //10.0

    luz_local.cor.x = 1.0;
    luz_local.cor.y = 1.0;
    luz_local.cor.z = 1.0;
    
    luz_ambiente.cor.x = 1.0;
    luz_ambiente.cor.y = 1.0;
    luz_ambiente.cor.z = 1.0;
    
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

    free(pixels);
    free(objetos[0].esfera);
    free(objetos[1].esfera);
    free(objetos[2].esfera);
    free(objetos[3].esfera);
    return 0;
}
