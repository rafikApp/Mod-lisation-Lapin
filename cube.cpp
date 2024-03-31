#include <cstdio>
#include <cstdlib>
/****************************************************************/
/* works on mac using freeglut and xquartz                      */
/* you can install freeglut using homebrew                      */
/* xquartz can be downloaded at http://xquartz.macosforge.org   */
/****************************************************************/
#include <GL/freeglut.h>
#include <jpeglib.h>
#include <jerror.h>

#ifdef __WIN32
#pragma comment (lib, "jpeg.lib")
#endif

#include <cmath>
/*class Point*/
GLfloat ambientLight[] = {0.2, 0.2, 0.2, 1};  // Lumière ambiante
GLfloat directionalLight[] = {0, -1, -1, 1.0};  // Lumière directionnelle
GLfloat lightPosition[] = {0, 5, 5, 1};  // Position de la lumière directionnelle
bool lum = false;

unsigned char image[450*300*3];
unsigned char calimero[256*256*3];
unsigned char texture[256* 2][256* 2][3];
GLfloat earAngle = 0.0; // Angle pour l'animation des oreilles
bool earDirection = true; // Direction initiale de l'animation
GLfloat VarRotate=40;
bool mainRotate=false;
int devant=0;
bool tombe=false;
int test =90;
bool sauter=false;
bool descend=false;
bool initialiser=false;
bool descend2=false;
bool sauter2=false;
bool bloque=false;
bool tombe1=false;
bool tombe2=false;
bool mort=false;
GLfloat caX = 0.0f;
GLfloat caY = 0.0f;
GLuint lapin;
GLuint cube;
GLfloat rotatSpeed = 2.0f;
GLfloat zoomLevel = 1.0;
class Point{
    public :
	//coordonnées x, y et z du point
	double x;
	double y;
	double z;
	// couleur r, v et b du point
	float r;
	float g;
	float b;
};
//Tableau pour stocker les sommets du cube et leur couleur
Point pCube[8]={
    {-0.5,-0.5, 0.5,1.0,0.0,0.0},
    { 0.5, -0.5, 0.5,0.0,1.0,0.0},
    { 0.5, -0.5, -0.5,0.0,0.0,1.0},
    { -0.5, -0.5, -0.5,1.0,1.0,1.0},
    { -0.5,0.5, 0.5,1.0,0.0,0.0},
    { 0.5, 0.5, 0.5,0.0,1.0,0.0},
    { 0.5, 0.5, -0.5,0.0,0.0,1.0},
    { -0.5, 0.5, -0.5,1.0,1.0,1.0}};

//Tableau pour stocker les indices des sommets par face pour le cube
int fCube[6][4]={
  {0,3,2,1},
  {0,1,5,4},
  {1,2,6,5},
  {2,3,7,6},
  {0,4,7,3},
  {4,5,6,7}};

char presse;
int anglex,angley,x,y,xold,yold;

/* Prototype des fonctions */
void affichage();
void clavier(unsigned char touche,int x,int y);
void reshape(int x,int y);
void idle();
void mouse(int bouton,int etat,int x,int y);
void mousemotion(int x,int y);
void animateEars(int value);
void loadJpegImage(char *fichier);
void touches(int key, int x, int y);
void setup();
void cube1();
void loadJpegFace(char *fichier);
int main(int argc,char **argv)
{
  /* initialisation de glut et creation
     de la fenetre */
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowPosition(200,200);
  glutInitWindowSize(500,500);
  glutCreateWindow("cube");

  /* Initialisation d'OpenGL */
  glClearColor(0.0,0.0,0.0,0.0);
  glColor3f(1.0,1.0,1.0);
  glPointSize(2.0);
  glEnable(GL_DEPTH_TEST);

    /* Parametrage du placage de textures */
       glGenTextures(1, &lapin);
glBindTexture(GL_TEXTURE_2D, lapin);
loadJpegImage("./txt7.jpg");
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,450,300,0,
	       GL_RGB,GL_UNSIGNED_BYTE,image);
  glEnable(GL_TEXTURE_2D);

  glGenTextures(2, &cube);
glBindTexture(GL_TEXTURE_2D, cube);
loadJpegFace("./texture.jpg");
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,256,256,0,
	       GL_RGB,GL_UNSIGNED_BYTE,image);
  glEnable(GL_TEXTURE_2D);
  /* enregistrement des fonctions de rappel */
  glutTimerFunc(0, animateEars, 0); // Démarrer l'animation des oreilles
  glutDisplayFunc(affichage);

  glutSpecialFunc(touches);
  glutKeyboardFunc(clavier);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutMotionFunc(mousemotion);

  /* Entree dans la boucle principale glut */
  glutMainLoop();
  return 0;
}

void EllipsoideSansTexture(float x , float y , float z, int n)
{
glBegin(GL_QUADS);

for(int i = 0; i < n; i++) {
float t1 = 2.0f * M_PI * i/n ;
float t1_next = 2.0f * M_PI * (i + 1) / n;
for(int j = 0; j < n; j++) {
float t2 = M_PI * j/n ;
float t2_next = M_PI * (j + 1) / n;
// On définit les points de la première face
float x1 = x * cos(t1) * sin(t2);
float y1 = y * sin(t1) * sin(t2);
float z1 = z * cos(t2);
float x2 = x * cos(t1_next) * sin(t2);
float y2 = y * sin(t1_next) * sin(t2);
float z2 = z * cos(t2);
float x3 = x * cos(t1_next) * sin(t2_next);
float y3 = y * sin(t1_next) * sin(t2_next);
float z3 = z * cos(t2_next);
float x4 = x * cos(t1) * sin(t2_next);
float y4 = y * sin(t1) * sin(t2_next);
float z4 = z * cos(t2_next);
glVertex3f(x1, y1, z1);
glVertex3f(x2, y2, z2);
glVertex3f(x3, y3, z3);
glVertex3f(x4, y4, z4);
}
}
glEnd();
}
void Ellipsoide(float x , float y , float z, int n)
{
glBegin(GL_QUADS);

for(int i = 0; i < n; i++) {
float t1 = 2.0f * M_PI * i/n ;
float t1_next = 2.0f * M_PI * (i + 1) / n;

for(int j = 0; j < n; j++) {
float t2 = M_PI * j/n ;
float t2_next = M_PI * (j + 1) / n;

// On définit les points de la première face
float x1 = x * cos(t1) * sin(t2);
float y1 = y * sin(t1) * sin(t2);
float z1 = z * cos(t2);

float x2 = x * cos(t1_next) * sin(t2);
float y2 = y * sin(t1_next) * sin(t2);
float z2 = z * cos(t2);

float x3 = x * cos(t1_next) * sin(t2_next);
float y3 = y * sin(t1_next) * sin(t2_next);
float z3 = z * cos(t2_next);

float x4 = x * cos(t1) * sin(t2_next);
float y4 = y * sin(t1) * sin(t2_next);
float z4 = z * cos(t2_next);

float u1 = 0.5f * (1.0f + atan2(z1, x1) / M_PI);
float v1 = 0.5f * (1.0f - asin(y1) / M_PI);

float u2 = 0.5f * (1.0f + atan2(z2, x2) / M_PI);
float v2 = 0.5f * (1.0f - asin(y2) / M_PI);

float u3 = 0.5f * (1.0f + atan2(z3, x3) / M_PI);
float v3 = 0.5f * (1.0f - asin(y3) / M_PI);

float u4 = 0.5f * (1.0f + atan2(z4, x4) / M_PI);
float v4 = 0.5f * (1.0f - asin(y4) / M_PI);

glTexCoord2f(u1, v1);
glVertex3f(x1, y1, z1);

glTexCoord2f(u2, v2);
glVertex3f(x2, y2, z2);

glTexCoord2f(u3, v3);
glVertex3f(x3, y3, z3);

glTexCoord2f(u4, v4);
glVertex3f(x4, y4, z4);
}
}
glEnd();
}
void setup() {
    if(mort){
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f); // Rouge pur pour l'arrière-plan
    }
    else
          glClearColor(0.5f, 1.0f, 0.5f, 1.0f);
}
void Sphere(const float radius,const int numSegments,const int numSlices) {
    glBegin(GL_QUAD_STRIP);

    for (int i = 0; i <= numSegments; ++i) {
        double lat0 = M_PI * (-0.5 + (double)(i - 1) / numSegments);
        double z0 = sin(lat0);
        double zr0 = cos(lat0);

        double lat1 = M_PI * (-0.5 + (double)i / numSegments);
        double z1 = sin(lat1);
        double zr1 = cos(lat1);

        for (int j = 0; j <= numSlices; ++j) {
            double lng = 2 * M_PI * (double)(j - 1) / numSlices;
             double x = cos(lng);
            double y = sin(lng);

            // Coordonnées du premier point
            glVertex3f(radius * x * zr0, radius * y * zr0, radius * z0);
            // Coordonnées du deuxième point
            glVertex3f(radius * x * zr1, radius * y * zr1, radius * z1);
        }
    }

    glEnd();
  }
void animateEars(int value) {
    const float maxAngle = 5.0f; // Angle maximum pour l'animation
    const float step = 1.0f; // Pas d'animation

    if (earDirection) {
        earAngle += step;
        if (earAngle >= maxAngle) {
            earDirection = false; // Changer la direction de l'animation
        }
    } else {
        earAngle -= step;
        if (earAngle <= -maxAngle) {
            earDirection = true; // Changer la direction de l'animation
        }
    }

    glutPostRedisplay(); // Demande un rafraîchissement de l'affichage
    glutTimerFunc(50, animateEars, 0); // Appel récursif pour continuer l'animation
}
void affichage()
{
 // int i,j;
  /* effacement de l'image avec la couleur de fond */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_LIGHTING);  // Activation de l'éclairage

  if(lum)
  {
  // Lumière ambiante
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

    // Lumière directionnelle
    glLightfv(GL_LIGHT0, GL_DIFFUSE, directionalLight);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glEnable(GL_LIGHT1);  // Activation de la lumière directionnelle
  }
  else
  {
    glDisable(GL_LIGHTING);  // Désactivation de l'éclairage
  }


  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glRotatef(angley,1.0,0.0,0.0);
  glRotatef(anglex,0.0,1.0,0.0);
  glRotatef(caY, 1.0f, 0.0f, 0.0f);
  glRotatef(caX, 0.0f, 1.0f, 0.0f);
  glOrtho(-10.0, 10.0 , -10.0 / zoomLevel, 10.0 / zoomLevel, -10.0 / zoomLevel, 10.0/ zoomLevel);


  glPushMatrix();
   glBindTexture(GL_TEXTURE_2D, lapin);
  setup();
   glTranslatef(0,0,devant);

if(tombe)
      {
         glRotatef(-90,0,0,1);
         bloque=true;
         mort=true;
         if(tombe1)
            {glTranslatef(3.486,0,-1.3);}
         else
         glTranslatef(3.486,0,0.3);

      }
if(sauter)
      {
        glTranslatef(0,1.7,1);
      }
if(descend)
      {

        sauter=false;
        descend=false;
        sauter2=false;
        descend2=false;
        glTranslatef(0,0,-1.5);
      }
if(descend2)
      {

        sauter=false;
        descend2=false;
        descend=false;
        tombe=false;
        devant+=2;
        sauter2=false;
        glTranslatef(0,0,1);
        glutPostRedisplay();
      }
if(initialiser)
       {
        glTranslatef(0,0,0);
        initialiser=false;
        devant=0;
        tombe=false;
        sauter2=false;
        sauter=false;
        descend2=false;
        descend=false;
        bloque=false;
        tombe1=false;
        tombe2=false;
        mort=false;
       }
if(sauter2)
{
    glTranslatef(0,1.7,-0.25);

   // sauter2=false;


}
  //tete
  //glPushMatrix();//**********
  glColor3f(1,1,1);
  Ellipsoide(0.97,1.2,1,20);
  glTranslatef(-0.2,-0.3,0);
  Ellipsoide(0.9,0.7,1.25,20);
  glTranslatef(0.2,0.3,0);
  //glPopMatrix();

  //oreille
  glPushMatrix();//****************
  glTranslatef(0,1.3,0.4);
    glRotatef(earAngle, 1, 0, 0);
  Ellipsoide(0.3,0.8,0.3,20);
  glTranslatef(0,-1.3,-0.4);
  glTranslatef(0,1.3,-0.4);
    glRotatef(earAngle, 0, 0, 1);
  Ellipsoide(0.3,0.8,0.3,20);
  glTranslatef(0,-1.3,0.4);
  glPopMatrix();//**********************

  //yeux

  glColor3f(1,1,1);
  glTranslatef(-0.8,0.4,0.4);
  EllipsoideSansTexture(0.1,0.4,0.2,20);
  glColor3f(0,0,0);
  glTranslatef(-0.05,-0.1,0);
  EllipsoideSansTexture(0.1,0.31,0.2,20);
  glTranslatef(0.05,0.1,0);
  glTranslatef(0.8,-0.4,-0.4);
  glColor3f(1,1,1);

  glTranslatef(-0.8,0.4,-0.4);
  EllipsoideSansTexture(0.1,0.4,0.2,20);
  glColor3f(0,0,0);
  glTranslatef(-0.05,-0.1,0);
  EllipsoideSansTexture(0.1,0.31,0.2,20);
    glTranslatef(0.05,0.1,0);


    glTranslatef(0.8,-0.4,0.4);

glColor3f(1,1,1);





  //corps
  glTranslatef(0,-2.35,0);
  Ellipsoide(0.9,1.25,0.9,20);
  glTranslatef(0,2.35,0);

  //jambe

  glTranslatef(0,-3.65,0.4);
  Ellipsoide(0.3,0.7,0.3,20);
  glTranslatef(0,3.65,-0.4);//retour
  glTranslatef(0,-3.65,-0.4);
  Ellipsoide(0.3,0.7,0.3,20);
  glTranslatef(0,3.65,0.4);//retour

  //main
  glPushMatrix();//***************
  glTranslatef(0,-1.8,-0.9);
  glRotatef(VarRotate,1,0,0);
  Ellipsoide(0.3,0.9,0.3,20);
  glPopMatrix();//****************

  glPushMatrix();//*********************
  glTranslatef(0,-1.8,0.9);
  glRotatef(-VarRotate,1,0,0);
  Ellipsoide(0.3,0.9,0.3,20);
  glPopMatrix();//**********************


//nez
 glColor3f(1.0, 0.5, 0.5);
 glTranslatef(-0.99,-0.05,0);
 EllipsoideSansTexture(0.1,0.1,0.2,20);
 glTranslatef(0.99,0.05,0);//retour

 //bouche
 //langue
 glColor3f(0.7,0,0.1);
 glTranslatef(-1.025,-0.4,0);
EllipsoideSansTexture(0.07,0.3,0.25,20);
 glTranslatef(1.025,0.4,0);//retour
//dents
glColor3f(1,1,1);
glTranslatef(-1.03,-0.37,0);
glutSolidCube(0.15);

glTranslatef(1.03,0.37,0);//retour
  //Pattes
  glPushMatrix();//***********
  glTranslatef(-0.12,-4.3,-0.4);
  Ellipsoide(0.38,0.2,0.29,20);
  glTranslatef(0.12,-4.3,-0.4);//retour
  glPopMatrix();

  glTranslatef(-0.12,-4.3,0.4);
  Ellipsoide(0.38,0.2,0.29,20);
  glTranslatef(0.12,4.3,-0.4);


  glTranslatef(0,-3.486,3.9);
glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
//cube
glPushMatrix();
glBindTexture(GL_TEXTURE_2D, cube);
glTranslatef(0,-3.486,3.9);
cube1();
glBindTexture(GL_TEXTURE_2D, 0);
glTranslatef(0,3.486,-3.9);
glPopMatrix();
    //Repère
    //axe x en rouge
    glBegin(GL_LINES);
        glColor3f(1.0,0.0,0.0);
    	glVertex3f(0, 0,0.0);
    	glVertex3f(2, 0,0.0);
    glEnd();
    //axe des y en vert
    glBegin(GL_LINES);
    	glColor3f(0.0,1.0,0.0);
    	glVertex3f(0, 0,0.0);
    	glVertex3f(0, 2,0.0);
    glEnd();
    //axe des z en bleu
    glBegin(GL_LINES);
    	glColor3f(0.0,0.0,1.0);
    	glVertex3f(0, 0,0.0);
    	glVertex3f(0, 0,2.0);
    glEnd();

  glFlush();

  //On echange les buffers
     // glDisable(GL_LIGHTING);  // Désactivation de l'éclairage à la fin

  glutSwapBuffers();
}
 void loadJpegImage(char *fichier)
{
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE *file;
  unsigned char *ligne;

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);
#ifdef __WIN32
  if (fopen_s(&file,fichier,"rb") != 0)
    {
      fprintf(stderr,"Erreur : impossible d'ouvrir le fichier texture.jpg\n");
      exit(1);
    }
#elif __GNUC__
  if ((file = fopen(fichier,"rb")) == 0)
    {
      fprintf(stderr,"Erreur : impossible d'ouvrir le fichier texture.jpg\n");
      exit(1);
    }
#endif
  jpeg_stdio_src(&cinfo, file);
  jpeg_read_header(&cinfo, TRUE);

  if ((cinfo.image_width!=450)||(cinfo.image_height!=300)) {
    fprintf(stdout,"Erreur : l'image doit etre de taille 256x256\n");
    exit(1);
  }
  if (cinfo.jpeg_color_space==JCS_GRAYSCALE) {
    fprintf(stdout,"Erreur : l'image doit etre de type RGB\n");
    exit(1);
  }

  jpeg_start_decompress(&cinfo);
  ligne=image;
  while (cinfo.output_scanline<cinfo.output_height)
    {
      ligne=image+3*450*cinfo.output_scanline;
      jpeg_read_scanlines(&cinfo,&ligne,1);
    }
  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
}
void loadJpegFace(char *fichier)
{
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE *file;
  unsigned char *ligne;

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);
#ifdef __WIN32
  if (fopen_s(&file,fichier,"rb") != 0)
    {
      fprintf(stderr,"Erreur : impossible d'ouvrir le fichier texture.jpg\n");
      exit(1);
    }
#elif __GNUC__
  if ((file = fopen(fichier,"rb")) == 0)
    {
      fprintf(stderr,"Erreur : impossible d'ouvrir le fichier texture.jpg\n");
      exit(1);
    }
#endif
  jpeg_stdio_src(&cinfo, file);
  jpeg_read_header(&cinfo, TRUE);

  if ((cinfo.image_width!=256)||(cinfo.image_height!=256)) {
    fprintf(stdout,"Erreur : l'image doit etre de taille 256x256\n");
    exit(1);
  }
  if (cinfo.jpeg_color_space==JCS_GRAYSCALE) {
    fprintf(stdout,"Erreur : l'image doit etre de type RGB\n");
    exit(1);
  }

  jpeg_start_decompress(&cinfo);
  ligne=image;
  while (cinfo.output_scanline<cinfo.output_height)
    {
      ligne=image+3*256*cinfo.output_scanline;
      jpeg_read_scanlines(&cinfo,&ligne,1);
    }
  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
 for(int i=0;i<256*2;i++)
{
    for(int j=0;j<256*2;j++)
    {
        if((i<256 && j<256)||(i>=256 && j>=256))
        {
            texture[i][j][0]=image[(i%256)*256*3+(j%256)*3];
            texture[i][j][1]=image[(i%256)*256*3+(j%256)*3+1];
            texture[i][j][2]=image[(i%256)*256*3+(j%256)*3+2];
        }
        else
        {
            texture[i][j][0]=255;
            texture[i][j][1]=255;
            texture[i][j][2]=255;
        }
    }
}
}
void touches(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            // Rotate the camera to the left
            caX -= rotatSpeed;
            glutPostRedisplay();
            break;
        case GLUT_KEY_RIGHT:
            // Rotate the camera to the right
            caX += rotatSpeed;
            glutPostRedisplay();
            break;
        case GLUT_KEY_UP:
            // Rotate the camera up
            caY -= rotatSpeed;
            glutPostRedisplay();
            break;
        case GLUT_KEY_DOWN:
            // Rotate the camera down
            caY += rotatSpeed;
            glutPostRedisplay();
            break;
    }

    glutPostRedisplay(); // Indicate to OpenGL to update the display
}
void clavier(unsigned char touche,int x,int y)
{
  switch (touche)
    {
    case 'p': /* affichage du carre plein */
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
      glutPostRedisplay();
      break;
    case 'f': /* affichage en mode fil de fer */
      glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
      glutPostRedisplay();
      break;
    case 's' : /* Affichage en mode sommets seuls */
      glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
      glutPostRedisplay();
      break;
    case 'd':
      glEnable(GL_DEPTH_TEST);
      glutPostRedisplay();
      break;
    case 'D':
      glDisable(GL_DEPTH_TEST);
      glutPostRedisplay();
      break;
    case 'a'://Les faces à l'envers s'affichent en fil de fer
      glPolygonMode(GL_FRONT,GL_FILL);
      glPolygonMode(GL_FRONT,GL_LINE);
      glutPostRedisplay();
    break;
    case 'x':{
    if(!bloque)
    {
    if(mainRotate)
    {
        VarRotate=40;
        mainRotate=false;
        glutPostRedisplay();

    }
    else
    {
        VarRotate=90;
        mainRotate=true;
        glutPostRedisplay();
    }

    }

    break;}
    case 'q' : {/*la touche 'q' permet de quitter le programme */
      exit(0);
      break;
    }
    case 'l':
    {
    if(bloque==false)
    {
        devant=devant+1;
        if(devant==3)
        {
        tombe=true;
        tombe1=true;
        }
        else
            tombe=false;
        if(sauter)
           {descend2=true;
          // devant=devant+2;

           }
        if(sauter2)
            descend2=true;

        } break;
    }
        case 'L':
    {
    if(bloque==false)
{
        if(sauter)
          {descend=true;
                devant=devant-1;
          }
          else if(devant==6){
            tombe=true;
            tombe2=true;
          }
          else if(sauter2)
          {
            descend=true;
            devant=2;
          }
          else{
          devant=devant-1;
          }

        }
        break;
    }
    case 'Z':
         zoomLevel += 0.1; // Augmentez le niveau de zoom, ajustez la valeur selon vos besoins
         glutPostRedisplay(); // Demandez à GLUT de redessiner la scène
         break;
    case 'z':
         zoomLevel -= 0.1; // Diminuez le niveau de zoom, ajustez la valeur selon vos besoins
         if (zoomLevel < 0.1) zoomLevel = 0.1; // Assurez-vous que le zoom ne devient pas trop petit
         glutPostRedisplay(); // Demandez à GLUT de redessiner la scène
         break;
    case ' ':
    if(!bloque){

          if(devant==2)  {sauter=true;
          devant++;}
          else if(devant==6) { sauter2=true;devant-=2;}
          }
          break;
    case 'i':
    {
    initialiser=true;
    break;
    }
    case 'e':
    {
    lum = !lum;
    break;
    }


}
}
void reshape(int x,int y)
{
  if (x<y)
    glViewport(0,(y-x)/2,x,x);
  else
    glViewport((x-y)/2,0,y,y);
}

void mouse(int button, int state,int x,int y)
{
  /* si on appuie sur le bouton gauche */
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    presse = 1; /* le booleen presse passe a 1 (vrai) */
    xold = x; /* on sauvegarde la position de la souris */
    yold=y;
  }
  /* si on relache le bouton gauche */
  if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    presse=0; /* le booleen presse passe a 0 (faux) */
}

void mousemotion(int x,int y)
  {
    if (presse) /* si le bouton gauche est presse */
    {
      /* on modifie les angles de rotation de l'objet
	 en fonction de la position actuelle de la souris et de la derniere
	 position sauvegardee */
      anglex=anglex+(x-xold);
      angley=angley+(y-yold);
      glutPostRedisplay(); /* on demande un rafraichissement de l'affichage */
    }

    xold=x; /* sauvegarde des valeurs courante de le position de la souris */
    yold=y;
  }
void cube1()
{
  glPushMatrix();
glBegin(GL_QUADS);

glColor3f(1,1,1);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


  glTexCoord2f(0.0,0.0);   glVertex3f(-0.5, 0.5, 0.5);
  glTexCoord2f(0.0,0.5);   glVertex3f(-0.5,-0.5, 0.5);
  glTexCoord2f(0.5,0.5);   glVertex3f( 0.5,-0.5, 0.5);
  glTexCoord2f(0.5,0.0);   glVertex3f( 0.5, 0.5, 0.5);


  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexCoord2f(0.0,0.0);   glVertex3f( 0.5, 0.5, 0.5);
  glTexCoord2f(0.0,3.0);   glVertex3f( 0.5,-0.5, 0.5);
  glTexCoord2f(3.0,3.0);   glVertex3f( 0.5,-0.5,-0.5);
  glTexCoord2f(3.0,0.0);   glVertex3f( 0.5, 0.5,-0.5);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);


  glTexCoord2f(0,0);   glVertex3f( 0.5, 0.5,-0.5);
  glTexCoord2f(0,3);   glVertex3f( 0.5,-0.5,-0.5);
  glTexCoord2f(3,3);   glVertex3f(-0.5,-0.5,-0.5);
  glTexCoord2f(3.0,0.0);   glVertex3f(-0.5, 0.5,-0.5);



  glTexCoord2f(0.0,0.0);   glVertex3f(-0.5, 0.5,-0.5);
  glTexCoord2f(0.0,0.5);   glVertex3f(-0.5,-0.5,-0.5);
  glTexCoord2f(0.5,0.5);   glVertex3f(-0.5,-0.5, 0.5);
  glTexCoord2f(0.5,0.0);   glVertex3f(-0.5, 0.5, 0.5);


  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexCoord2f(0.0,0.0);   glVertex3f(-0.5, 0.5,-0.5);
  glTexCoord2f(0.0,3.0);   glVertex3f(-0.5, 0.5, 0.5);
  glTexCoord2f(3.0,3.0);   glVertex3f( 0.5, 0.5, 0.5);
  glTexCoord2f(3.0,0.0);   glVertex3f( 0.5, 0.5,-0.5);


  glTexCoord2f(0.0,0.0);   glVertex3f(-0.5,-0.5,-0.5);
  glTexCoord2f(0.5,0);   glVertex3f(-0.5,-0.5, 0.5);
  glTexCoord2f(0.5,1);   glVertex3f( 0.5,-0.5, 0.5);
  glTexCoord2f(0,1);   glVertex3f( 0.5,-0.5,-0.5);
  glEnd();
  glPopMatrix();


}
