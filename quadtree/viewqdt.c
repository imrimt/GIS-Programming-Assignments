/* 

 What it does:

 - program is run as:  ./mylasqdt <lastxtfile> <nbpoints>
 
 - reads an ASCII point cloud and builds a quadtree on it so that each
 leaf has at most <nbpoints> points, and renders it in 2D with default
 orthogonal projection.
  
 */


#include "quadtree.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif



GLfloat red[3] = {1.0, 0.0, 0.0};
GLfloat green[3] = {0.0, 1.0, 0.0};
GLfloat blue[3] = {0.0, 0.0, 1.0};
GLfloat black[3] = {0.0, 0.0, 0.0};
GLfloat white[3] = {1.0, 1.0, 1.0};
GLfloat gray[3] = {0.5, 0.5, 0.5};
GLfloat yellow[3] = {1.0, 1.0, 0.0};
GLfloat magenta[3] = {1.0, 0.0, 1.0};
GLfloat cyan[3] = {0.0, 1.0, 1.0};




/* forward declarations of functions */
void display(void);
void keypress(unsigned char key, int x, int y);
void reset(); 


/* global variables */
const int WINDOWSIZE = 500;
const int POINT_SIZE  = 6.0f;

//the point cloud
point3D*  points = NULL;
int n;

// the quadtree of the points
quadtree *tree = NULL;


//range of points 
float minX, maxX, minY, maxY; 


/* ****************************** */
/* print the array of points p of size k */
void print_points(point3D* p, int k) {
  assert(p);
  int i;
  printf("points: ");
  for (i=0; i<k; i++) {
    printf("[%.1f,%.1f,%.1f] ", p[i].x, p[i].y, p[i].z);
  }
  printf("\n");
  fflush(stdout);  //flush stdout, weird sync happens when using gl thread
}




/* ****************************** */
int main(int argc, char** argv) {
    
    // read number of points from user
    if (argc!=3) {
      printf("usage: %s [lasASCIIfile] [k]\n", argv[0]);
        exit(1);
    }
    
    int max_points_per_leaf = atoi(argv[2]);
    printf("you entered input file=%s, max_points_per_leaf=%d\n", argv[1], max_points_per_leaf);
    assert(max_points_per_leaf > 0);


    //QUADTREE stuff 

    //read the points from file into array points; note that points
    //and n are global so they dont need to be parameters
    //readLidarFromFile(argv[1]);

    //build quadtree 
    //tree = quadtree_build(points, n, max_points_per_leaf); 

    //print some info or something 



    //GLUT stuff 
    //------------------------------------------
    /* initialize GLUT  */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WINDOWSIZE, WINDOWSIZE);
    glutInitWindowPosition(100,100);
    glutCreateWindow(argv[0]);
    
    /* register callback functions */
    glutDisplayFunc(display);
    glutKeyboardFunc(keypress);
    
    
    /* init GL */
    /* set background color black*/
    glClearColor(0, 0, 0, 0);
    
    /* circular points */
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glPointSize(POINT_SIZE);

    /* give control to event handler */
    glutMainLoop();
    return 0;
}




/* ****************************** */
/* draw a single point */
void draw_point(point3D point)
{
    glColor3fv(yellow);

    glBegin(GL_POINTS);
    glVertex2f(point.x, point.y);
    glEnd();
}


/* ****************************** */
/* draw a line between two points */
void draw_line(point3D p1, point3D p2)
{
    glColor3fv(cyan);
    
    glBegin(GL_LINES);
    glVertex2f(p1.x, p1.y);
    glVertex2f(p2.x, p2.y);
    glEnd();
}





/* ****************************** */
/* draw the array of points stored in global variable points[] 
 */
void draw_points(){

  const int R= 1;
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  //set color 
  glColor3fv(yellow);   
  
  assert(points);
  int i;
  for (i=0; i<n; i++) {
    draw_point(points[i]); 
  }

}


/* ****************************** */
/* recursive draw function for drawing a tree rooted at the given node
 */
void draw_node(treeNode *node)
{
  //fill in
}

/* ****************************** */
/* draw the quadtree stored in the global variable tree
 */
void draw_quadtree()
{
    assert(tree);
    draw_node(tree->root);
}




/* ****************************** */
void display(void) {
    
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); //clear the matrix
    
    
    /* the default GL window is [-1,1]x[-1,1] with the origin in the
     center the points are in the range (minX,maxX) to (minY,maxY), so
     they need to be mapped to [-1,1] x [-1,1]. This means first
     translate them then scale */

    glScalef(2.0/(maxX-minX), 2.0/(maxY-minY), 1.0);
    glTranslatef(-(maxX-minX)/2, -(maxY-minY)/2, 0);

    //eventually we'll want to call the function that draws the tree
    //draw_quadtree();
   
    //for now we just draw the input points 
    draw_points();
 
    /* execute the drawing commands */
    glFlush();
}




/* ****************************** */
void keypress(unsigned char key, int x, int y) {
  switch(key)    {
  case 'q':
    exit(0);
    break;
  }
}


/* Handler for window re-size event. Called back when the window first appears and
 whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
    
    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);
    
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset
    gluOrtho2D(0.0, (GLdouble) width, 0.0, (GLdouble) height); 
}


