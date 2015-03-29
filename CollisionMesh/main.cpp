#define _USE_MATH_DEFINES // for M_PI
#include <math.h>
#include <algorithm>
#include "CollisionMesh.h"
#include "Terrain.h"
#include "Bouncy.h"
#include "MakeOctree.h"
#include "Bbox.h"
#include "Octree.h"
#include <GL/glut.h>

// In case we sonehow include SDL.h which #defines main
#ifdef main
#undef main
#endif


// Yuck, global variables!
CollisionMesh mesh;
Bouncy bouncy;
Bouncy B2;
BBox world;
Octree* Tree;
bool DrawTree;


// Variables for mouse control
static Vec3f pos(0, 0, -150.0f);
static float xRot = 10.0f;
static float yRot = 0;
// Mouse buttons
static int mouseButton[3] = { GLUT_UP, GLUT_UP, GLUT_UP };

void mousedown(int button, int state, int x, int y)
{
  mouseButton[button] = state;
}

void mousemove(int x, int y)
{
  static int oldx = x;
  static int oldy = y;
  int xdiff = x - oldx;
  int ydiff = y - oldy;
  oldx = x;
  oldy = y;
  
  if (mouseButton[GLUT_LEFT_BUTTON] == GLUT_DOWN)
  {
    // Rotate
    yRot += (float)xdiff;
    xRot += (float)ydiff;
  }
  else if (mouseButton[GLUT_MIDDLE_BUTTON] == GLUT_DOWN)
  {
    // Pan
    pos = Vec3f(pos.X() + (float)xdiff, pos.Y() - (float)ydiff, pos.Z());
    // screen y-coord is "upside-down"
  }
  else if (mouseButton[GLUT_RIGHT_BUTTON] == GLUT_DOWN)
  {
    // Zoom
    pos = Vec3f(pos.X(), pos.Y(), pos.Z() + (float)ydiff);
  }
}

void onKey(unsigned char key, int, int)
{
    switch (key)
    {
    case 'p':
    case 'P':
        // Restart bouncy
        bouncy.SetPos(Vec3f((float)(rand() % 200 - 100), 80.0f, (float)(rand() % 200 - 100)));
        bouncy.SetVel(Vec3f(0, 0, 0));
		B2.SetPos(Vec3f((float)(rand() % 200 - 100), 80.0f, (float)(rand() % 200 - 100)));
        B2.SetVel(Vec3f(0, 0, 0));
        break;
    case 'q':
    case 'Q':
        exit(0);
        break;
	case 't':
	case 'T':
		DrawTree = !DrawTree;
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(pos.X(), pos.Y(), pos.Z());
    glRotatef(xRot, 1.0f, 0, 0);
    glRotatef(yRot, 0, 1.0f, 0);

    glDisable(GL_LIGHTING);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if(DrawTree)
	{
		Tree->Draw();
	}
	else
	{
		 mesh.Draw();
	}
    glEnable(GL_LIGHTING);

	Tree->highlight(false);
    bouncy.Update(Tree);
	B2.Update(Tree);
	bouncy.Collides(&B2);
    bouncy.Draw();
	B2.Draw();
	

    glutSwapBuffers();
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
	int width = 600;
	int height = 600;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(width, height);
	glutCreateWindow("Press P to drop new balls and T to display the octree in use");
	glutDisplayFunc(display);
    glutKeyboardFunc(onKey);
    glutMouseFunc(mousedown);
    glutMotionFunc(mousemove);
    glutPassiveMotionFunc(mousemove);

    glClearColor(0, 0, 0, 0);

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective(60.0, 1.0, 1.0, 1024.0);
	
    glEnable(GL_DEPTH_TEST); 

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    bouncy.SetPos(Vec3f(-50.0f, 50.0f, 20.0f));
	B2.SetPos(Vec3f(50.0f, 50.0f, -20.0f));


    // Generate 'terrain' from heightmap
    // Create collision mesh from terrain
    Terrain terrain;
	world = BBox(-100,100,-100,100,-100,100);
    if (terrain.LoadHeightmap("heightmap.png"))
    {
        terrain.MakeMesh(&mesh, 40, 5.0f);
		Tree = MakeOctree(mesh.getTris(),world,0);
    }
	DrawTree = false;

	glutMainLoop();
}

