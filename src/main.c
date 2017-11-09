#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "geometria.h"

#define NUM_ESFERAS 2
#define NUM_CUBOS 0
#define NUM_OBJETOS (NUM_ESFERAS + NUM_CUBOS)

objeto_t objetos[NUM_OBJETOS];

float *pixels;

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
            win_x = i;
            win_y = j;
            
            // Subtrai y a altura da janela.
            //win_y = altura - win_y;
            
            // Obtém o ponto no plano near.
            gluUnProject(win_x, win_y, 0.0, model_view, projection, view_port, &x_near, &y_near, &z_near);
            
            // Obtém o ponto no plano near.
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
            //dir = normalizar(&dir);
            
            //printf("Ponto origem: (%lf, %lf, %lf)\n", (double) x_near, (double) y_near, (double) z_near);
            //printf("Vetor direção: (%lf, %lf, %lf)\n", (double) dir.x, (double) dir.y, (double) dir.z);
            //exit(0); // DEBUG: para após o primeiro
            
            // Faz o raytracing.
            pixel = raytrace(&origem, &dir, objetos, NUM_OBJETOS);
            
            pixel.r = 0.1 + 0.001 * j;
            pixel.g = 0.2 + 0.002 * i;
            pixel.b = 0.9;
            
            // Preenche a matriz de pixels.
            pixels[(i * largura * 3) + (j * 3) + 0] = pixel.r;
            pixels[(i * largura * 3) + (j * 3) + 1] = pixel.g;
            pixels[(i * largura * 3) + (j * 3) + 2] = pixel.b; 
        
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
    gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void keyboard (unsigned char key, int x, int y)
{
    switch (key) 
    {
        case 'a':
            //
            glutPostRedisplay();
            break;
        case 'b':
            //
            glutPostRedisplay();
            break;
        case 'c':
            //
            glutPostRedisplay();
            break;
        default:
            break;
    }
}

int main(int argc, char** argv)
{

    // Criação das esferas
    ponto_t centro1, centro2;
    cor_t cor1, cor2;

    centro1.x = 10;
    centro1.y = 10;
    centro1.z = 10;
    
    centro2.x = 10;
    centro2.y = 20;
    centro2.z = 20;

    cor1.r = 1.0;
    cor1.g = 0.0;
    cor1.b = 0.0;

    cor2.r = 0.0;
    cor2.g = 0.0;
    cor2.b = 1.0;

    objetos[0].tipo = ESFERA;
    objetos[0].esfera = malloc(sizeof(esfera_t));
    objetos[0].esfera->centro = centro1;
    objetos[0].esfera->raio = 2;
    objetos[0].esfera->cor = cor1;
   
    objetos[1].tipo = ESFERA;
    objetos[1].esfera = malloc(sizeof(esfera_t));
    objetos[1].esfera->centro = centro2;
    objetos[1].esfera->raio = 2;
    objetos[1].esfera->cor = cor2;

    pixels = NULL;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(640, 480); 
    glutInitWindowPosition (100, 100);
    glutCreateWindow (argv[0]);
    init();
    glutDisplayFunc(display); 
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    free(objetos[0].esfera);
    free(objetos[1].esfera);

    return 0;
}
