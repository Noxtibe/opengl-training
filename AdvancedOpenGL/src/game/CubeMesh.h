#ifndef CUBEMESH_H
#define CUBEMESH_H

#include <GL/glew.h>

class CubeMesh 
{
public:

  void load();
  void draw();
  void drawTesselation();
  void clean();

private:

  GLuint vao;
  GLuint buffer;
};

#endif