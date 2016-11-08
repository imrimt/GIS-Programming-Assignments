
/* 
  
*/ 

#ifndef __quadtree_h
#define __quadtree_h

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <cmath>

#define EPSILON 0.00000000001

using namespace std;

/* GLOBALS */
static int max_points_per_leaf;
static int height;
static int count;

typedef struct {
	double x,y,z;
} point3D;

typedef struct {
	double Xmin, Xmax, Ymin, Ymax;
} square;

typedef struct _treeNode treeNode;

struct _treeNode {
    int k;   //how many points in this quadrant 
    
    /* If this is a leaf node, p represents the vector that stored 
    all the points in this leaf.  If this is not a leaf node, p is NULL.*/
    vector<point3D> p; 

    treeNode *nw, *ne, *sw, *se; /* children. */
};

typedef struct _quadtree {
	treeNode *root;  //root of the tree
  	int max_points_per_leaf;
  	int count; //number of nodes in the tree
  	int height; //height of the tree
} quadtree;

/* initializing an empty tree */
quadtree* quadtree_init();

/* create a new tree representing the given array of points */
quadtree* quadtree_build(const vector<point3D>& points, int n, int max_points_per_leaf);

/* build sub quadtrees */
treeNode* buildQuadtree(const vector<point3D>& points, const square &S);

/* free all memory allocated for the tree, including the tree itself */
void quadtree_free(quadtree *tree);

/* free a specific node of the tree */ 
// static void treeNode_free(treeNode* node);

/* print out information about the tree including height, number of
 nodes, and each node in an pre-order traversal */
void quadtree_print(quadtree *tree);

square findSurroundingSquare(const vector<point3D>& points);

#endif
