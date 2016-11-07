
/* 
  
*/ 

#ifndef __quadtree_h
#define __quadtree_h


#include <stddef.h> //for size_t


typedef struct {float x,y,z;} point3D;

typedef struct _treeNode treeNode;

struct _treeNode {
  int k;   //how many points in this quadrant 
  point3D* p; /* If this is a leaf node, p represents the point stored
		in this leaf.  If this is not a leaf node, p is NULL.
	     */
  treeNode  *nw, *ne, *sw, *se; /* children. */
  };


typedef struct _quadtree {
  treeNode *root;  //root of the tree
  int max_points_per_leaf; 
  size_t count; //number of nodes in the tree
  size_t height; //height of the tree
  //....
} quadtree;



/* create a new tree representing the given array of points */
quadtree* quadtree_build(point3D *points, int n, int max_points_per_leaf);

/* free all memory allocated for the tree, including the tree
   itself */
void quadtree_free(quadtree *tree);

/* print out information about the tree including height, number of
 nodes, and each node in an pre-order traversal */
void quadtree_print(quadtree *tree);


#endif
