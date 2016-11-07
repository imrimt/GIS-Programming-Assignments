/* 

*/

#include "quadtree.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

/* create a new empty tree */
quadtree* quadtree_init() {
  
  quadtree* tree = malloc(sizeof(quadtree));
  assert(tree); 

  tree->root = NULL; 
  tree->count = tree->height = 0; 
  return tree; 
}


//private function to recursively free the subtree rooted at node
static void treeNode_free(treeNode* node) {
  //fill in 

}


/* free all memory allocated for the tree, including the tree
   itself */
void quadtree_free(quadtree *tree) {

  if (!tree) return; 
  treeNode_free(tree->root); 
  free(tree); 
}




/* create a tree representing the  array of points */
quadtree* quadtree_build(point3D *points, int n, int max_points_per_leaf) {

  //fill in

  return NULL;
}

