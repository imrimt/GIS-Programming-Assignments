/* 

 What it does:

 - program is run as:  ./mylasqdt <lastxtfile> <nbpoints>
 
 - reads an ASCII point cloud and builds a quadtree on it so that each
 leaf has at most <nbpoints> points, and renders it in 2D with default
 orthogonal projection.
  
 */


#include "quadtree.h"
#include "mylastoqdt.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <utility>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// colors
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
void draw_points(); 
void print_points(vector<point3D>* p, int k); 
void draw_point(point3D point);
void draw_node(treeNode* node, const square &S);
void draw_quadtree(); 
void display(); 
void reset();

bool isInteger(string str); 

pair<double, double> transform(double x, double y);

/* global variables */
const int WINDOWSIZE = 500;
const int POINT_SIZE  = 4.0f;
const double LINE_WIDTH = 5.0f;
const double openGLRange = 2;

//the point cloud
vector<point3D>* points;
int n;

// the quadtree of the points
quadtree *tree = NULL;

//range of points 
double minX, maxX, minY, maxY; 
double xRange, yRange;

/* ****************************** */
/* print the array of points p of size k */
void print_points(vector<point3D>* p, int k) {
    int i;
    printf("points: ");
    for (i = 0; i < k; i++) {
        printf("[%.2f,%.2f,%.2f] ", (*p)[i].x, (*p)[i].y, (*p)[i].z);
    }
    printf("\n");
    fflush(stdout);  //flush stdout, weird sync happens when using gl thread
}

/* ****************************** */
int main(int argc, char** argv) {
    
    // sanity check for arguments on the command line
    if (argc != 3) {
        cout << "Invalid number of arguments. Please enter in the following format: " << endl
             << "[path to executable] [LiDAR data] [number of points per leaf]" << endl;
        exit(1);
    }

    if (!isInteger(argv[2])) {
        cout << "Invalid argument for coordinates. Please make sure it's a valid integer" << endl;
        exit(1);
    }

    string input = argv[1];
    string numPoints = argv[2];
    string inputPath;
    string fileName;
    max_points_per_leaf = stoi(numPoints);

    // if (max_points_per_leaf < 3) {
    //     cout << "Number of points in a leaf has to be at least 3. Exiting..." << endl;
    //     exit(1);
    // }

    if (input.find_last_of("/") != string::npos) {
        int pos = input.find_last_of("/");
        inputPath = input.substr(0, pos + 1);
        fileName = input.substr(input.find_last_of("/") + 1);
    } 
    else {
        fileName = input;
    }

    MyLAS mylas;

    if (!mylas.readLiDARData(input)) {
        cout << "Unable to read " << input << endl;
        exit(1);
    }

    mylas.buildQuadtree(max_points_per_leaf);

    tree = mylas.getQuadtree();

    minX = tree->boundingSquare.Xmin;
    maxX = tree->boundingSquare.Xmax;
    minY = tree->boundingSquare.Ymin;
    maxY = tree->boundingSquare.Ymax;

    points = mylas.getPoints();
    
    //print bounding box 
    printf("minX=%.2f, maxX = %.2f, minY=%.2f maxY=%.2f\n", minX, maxX, minY, maxY);
    xRange = maxX - minX;
    yRange = maxY - minY;

    //print some info or something
    printf("tree height: %d\n", tree->height);
    printf("number of nodes: %d\n", tree->count);
    printf("number of points: %d\n", tree->root->k);

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
    // set background color white
    glClearColor(1, 1, 1, 0);
    
    /* circular points */
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClear(GL_DEPTH_BUFFER_BIT);
    
    glPointSize(POINT_SIZE);

    /* give control to event handler */
    glutMainLoop();

    quadtree_free(tree);

    return 0;
}

/* ****************************** */
void display(void) {
    
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); //clear the matrix
    

    /* The default GL window is [-1,1]x[-1,1] with the origin in the
       center. Our points are in the range [minX,maxX] x [minY,maxY] 
    */ 
     
    // draw_points();

    draw_quadtree();
 
    /* execute the drawing commands */
    glFlush();
}

/* ****************************** */
/* draw a single point */
void draw_point(point3D point)
{

    // color by classification
    switch (point.c) {
        case 0:
        case 1:
            glColor3fv(yellow);
            break;
        case 2:
            glColor3fv(gray);
            break;
        case 3:
        case 4:
        case 5:
            glColor3fv(green);
            break;
        case 6:
            glColor3fv(black);
            break;
        case 7:
        case 8:
            glColor3fv(magenta);
            break;
        case 9:
            glColor3fv(blue);
            break;
        default:
            glColor3fv(cyan); 
            break;   
    }

    /* our point is in the range minX..maxX, minY..maxY, and the GL
       window is [-1,1]x[-1,1]. We need to transform each point
       appropriately so that [minX,maxX]x[minY,maxY] maps onto
       [-1,1]x[-1,1] 
    */
    double x = 0, y = 0;  //just to initialize with something

    pair<double, double> pTrans = transform(point.x, point.y);

    x = pTrans.first;
    y = pTrans.second;

    // printf("coordinates (%.2f, %.2f)\n", x, y);
    
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
}


/* ****************************** */
/* draw a line between two points */
void draw_line(point3D p1, point3D p2)
{
    // set line color to be red
    glColor3fv(red);

    // need to transform the points so that they are in [-1,1]x[-1,1] 
    pair<double, double> p1Transform = transform(p1.x, p1.y),
                       p2Transform = transform(p2.x, p2.y);

    p1.x = p1Transform.first;
    p1.y = p1Transform.second;
    p2.x = p2Transform.first;
    p2.y = p2Transform.second;

    glLineWidth(LINE_WIDTH);

    glBegin(GL_LINES);
    glVertex2f(p1.x, p1.y);
    glVertex2f(p2.x, p2.y);
    glEnd();
}

/* ****************************** */
/* draw the array of points stored in global vector points 
 */
void draw_points(){

    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    cout << "hi" << endl;
      
    for (unsigned int i = 0; i < points->size(); i++) {
        point3D point = (*points)[i];
        draw_point(point); 
    }
}

/* ****************************** */
/* recursive draw function for drawing a tree rooted at the given node
 */
void draw_node(treeNode *node, const square &S)
{

    if (node == NULL) {
        return;
    }

    // the node is a leaf
    if (!node->p.empty()) {
        // draw all the points in this leaf
        for (auto point : node->p) {
            draw_point(point);
        }
    }

    // the node is an internal node
    else {

        double midX = (S.Xmax + S.Xmin) / 2.00,
               midY = (S.Ymax + S.Ymin) / 2.00;

        square NEsquare, NWsquare, SEsquare, SWsquare;

        // Split into 4 squares to traverse
        NEsquare.buildSquare(midX, S.Xmax, S.Ymin, midY);
        NWsquare.buildSquare(S.Xmin, midX, S.Ymin, midY);
        SEsquare.buildSquare(midX, S.Xmax, midY, S.Ymax);
        SWsquare.buildSquare(S.Xmin, midX, midY, S.Ymax);

        point3D p1, p2; 

        // construct two points of the vertical line
        p1.x = midX;
        p1.y = S.Ymin;
        p2.x = midX;
        p2.y = S.Ymax;

        // draw the vertical line spliting the quadrant
        draw_line(p1, p2);

        // construct two points of the horizontal line
        p1.x = S.Xmin;
        p1.y = midY;
        p2.x = S.Xmax;
        p2.y = midY;

        // draw the horizontal line spliting the quadrant
        draw_line(p1, p2);

        // recurse on children
        draw_node(node->ne, NEsquare);
        draw_node(node->nw, NWsquare);
        draw_node(node->se, SEsquare);
        draw_node(node->sw, SWsquare);
        
    }
}

/* ****************************** */
/* draw the quadtree stored in the global variable tree
 */
void draw_quadtree()
{
    assert(tree);
    draw_node(tree->root, tree->boundingSquare);
}


/* ****************************** */
void keypress(unsigned char key, int x, int y) {
    switch(key) {
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

// return true if a given string represents an integer, otherwise
//  return false
bool isInteger(string str) {
    char start = str[0];
    if (isdigit(start) == false && start != '-') {
        return false;
    } 
    if (start == '0' && str.length() > 1) {
        return false;
    }
    for (unsigned int i = 1; i < str.length(); i++) {
        if (isdigit(str[i]) == false) return false;
    }

    return true;
}

// transform point's coordinates to OpenGL window's coordinate-system
pair<double, double> transform(double x, double y) {
    pair<double, double> result;
    result.first = (double)openGLRange * (x - minX) / xRange - 1;
    result.second = (double)openGLRange * (y - minY) / yRange - 1;
    return result;
}