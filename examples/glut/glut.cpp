#include <GL/glut.h>
#include <corona.h>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
using namespace corona;


GLuint g_texture;


inline void coord(float x, float y) {
  glTexCoord2f(x, y);
  glVertex2f(x, y);
}


void display() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, 1, 1, 0, -1, 1);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glBegin(GL_QUADS);
  coord(0, 0);
  coord(1, 0);
  coord(1, 1);
  coord(0, 1);
  glEnd();

  glutSwapBuffers();
}


void reshape(int w, int h) {
  glViewport(0, 0, w, h);
}


int main(int argc, char** argv) {
  glutInit(&argc, argv);

  auto_ptr<Image> img(OpenImage("f03n0g08.png", FF_AUTODETECT, PF_R8G8B8A8));
  if (!img.get()) {
    puts("Error loading image");
    return EXIT_FAILURE;
  }

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(640, 480);
  glutCreateWindow("Corona/GLUT Image Test");

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);

  glEnable(GL_TEXTURE_2D);
  glGenTextures(1, &g_texture);
  glBindTexture(GL_TEXTURE_2D, g_texture);
  glTexImage2D(
    GL_TEXTURE_2D, 0, GL_RGBA8, img->getWidth(), img->getHeight(),
    0, GL_RGBA, GL_UNSIGNED_BYTE, img->getPixels());

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glutMainLoop();
}
