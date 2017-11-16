#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "geometria.h"

/* Configurações básicas. */

#define NUM_ESFERAS 4
#define NUM_CUBOS 0
#define NUM_OBJETOS (NUM_ESFERAS + NUM_CUBOS)

#define Z_NEAR 1.0
#define Z_FAR 20.0

#define LF_X 0.0 // Look from x
#define LF_Y 0.0 // Look from y
#define LF_Z 5.0 // Look from z

#define LA_X 0.0 // Look at x
#define LA_Y 0.0 // Look at y
#define LA_Z 0.0 // Look at z

#define FUNDO_R 1.0
#define FUNDO_G 1.0
#define FUNDO_B 1.0

#define MAX_REC 10
 
/* Variáveis globais. */

luz_t luz; // Fonte de luz
objeto_t objetos[NUM_OBJETOS]; // Lista de objetos
float *pixels; // Matriz de píxels de 3 canais.

void init(void) 
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glShadeModel(GL_SMOOTH);
}

void display(void)
{
    GLint view_port[4];  
    GLdouble model_view[16];
    GLdouble projection[16]; 
    GLdouble win_x, win_y; // Coornadas x e y.
    GLdouble x_near, y_near, z_near;
    GLdouble x_far, y_far, z_far;

    int i, j, altura, largura;
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
    
    largura = view_port[2];
    altura = view_port[3];
    
    if(pixels != NULL)
    {
        free(pixels);
        pixels = NULL;
    }
    
    pixels = (float *) malloc(altura * largura * 3 * sizeof(float));
    
    // Define os pontos na tela.
    win_x = 0;
    win_y = 0;


    
    for(i = 0; i < altura; i++) // Percorre as linhas (altura)
    {
        for(j = 0; j < largura; j++) // Percorre as colunas (largura)
        {
            win_x = j;
            win_y = i;
            
            // Subtrai y a altura da janela.
            //win_y = altura - win_y;
            
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
            
            //printf("Ponto origem: (%lf, %lf, %lf)\n", (double) x_near, (double) y_near, (double) z_near);
            //printf("Ponto final: (%lf, %lf, %lf)\n", (double) x_far, (double) y_far, (double) z_far);
            //printf("Vetor direção: (%lf, %lf, %lf)\n", (double) dir.x, (double) dir.y, (double) dir.z);
            //exit(0); // DEBUG: para após o primeiro

            // Faz o raytracing.
            pixel = raytrace(&origem, &dir, objetos, &luz, NUM_OBJETOS, 0, MAX_REC);
			
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
        case 'x':
            glRotatef(15.0, 1.0, 0.0, 0.0);
            glutPostRedisplay();
            break;
        case 'y':
            glRotatef(15.0, 0.0, 1.0, 0.0);
            glutPostRedisplay();
            break;
        case 'z':
            glRotatef(15.0, 0.0, 0.0, 1.0);
            glutPostRedisplay();
            break;
        case 'i':
            luz.posicao.x -= 0.1;    
            glutPostRedisplay();
            break;
        case 'o':
            luz.posicao.y -= 0.1;  
            glutPostRedisplay();
            break;
        case 'p':
            luz.posicao.z -= 0.1;     
            glutPostRedisplay();
            break;
        case 'j':
            luz.posicao.x -= 0.1;     
            glutPostRedisplay();
            break;
        case 'k':
            luz.posicao.y -= 0.1;   
            glutPostRedisplay();
            break;
        case 'l':
            luz.posicao.z -= 0.1;      
            glutPostRedisplay();
            break;
        default:
            break;
    }
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
	
    luz.posicao.x = 0;
    luz.posicao.y = 0;
    luz.posicao.z = 10;

    luz.cor.x = 1.0;
    luz.cor.y = 1.0;
    luz.cor.z = 1.0;

    pixels = NULL;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(100, 100); 
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
