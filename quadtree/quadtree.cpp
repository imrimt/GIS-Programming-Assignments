/*****************************************************************************
 File:   quadtree.cpp
 Author: Son D. Ngo
 Date:   November 2016
 
 Description: Cpp file that implements functions to operate on quad tree
  
 ******************************************************************************/

#include "quadtree.h"

using namespace std;

/* create a new empty tree */
quadtree* quadtree_init() {

    quadtree* tree = new quadtree;
    assert(tree); 

    tree->root = NULL; 
    tree->count = tree->height = 0; 
    return tree; 
}


//private function to recursively free the subtree rooted at node
void treeNode_free(treeNode* node) {
    if (node->nw) treeNode_free(node->nw);
    if (node->ne) treeNode_free(node->ne);
    if (node->sw) treeNode_free(node->sw);
    if (node->se) treeNode_free(node->se);

    delete node;
}


// free all memory allocated for the tree, including the tree
//  itself
void quadtree_free(quadtree *tree) {
    if (!tree) return; 
    treeNode_free(tree->root); 
    free(tree); 
}


// create a tree representing the  array of points 
quadtree* quadtree_build(const vector<point3D>& points, int n, int max_points) {

    quadtree* tree = quadtree_init();

    max_points_per_leaf = max_points;
    height = numNodes = 0;

    square firstSqr;

    firstSqr = findBoundingSquare(points);

    treeNode* root = buildQuadtree(points, firstSqr);

    // saving information to the tree
    tree->root = root;
    tree->max_points_per_leaf = max_points_per_leaf;
    tree->height = root->height;
    tree->count = numNodes + 1; // add one for yourself
    tree->boundingSquare = firstSqr;

    return tree;
}

// recursive function that builds a quadtree given a square 
//  and a set of points
treeNode* buildQuadtree(const vector<point3D>& points, const square &S) {

    treeNode* node = new (nothrow) treeNode();
    assert(node);

    node->k = points.size();

    int numPoints = points.size();

    // if points have at most max_poitns_per_leaf, then store P in node
    // and return
    if (numPoints <= max_points_per_leaf) {
        if (numPoints == 0) {
            return NULL;
        }
        node->p = points;
        node->k = numPoints;
        node->height = 0;
        node->ne = node->nw = node->se = node->sw = NULL;
    }

    // partition S into 4 quadrants, then partition P into 4 corresponding 
    //  subsets
    else {

        double midX = (S.Xmax + S.Xmin) / 2.00,
               midY = (S.Ymax + S.Ymin) / 2.00;

        // if the square is too small then return NULL
        if ((S.Xmax - S.Xmin) < EPSILON || (S.Ymax - S.Ymin) < EPSILON) {
            return NULL;
        }

        vector<point3D> NEpoints, NWpoints, SEpoints, SWpoints;

        square NEsquare, NWsquare, SEsquare, SWsquare;

        // Partition into 4 sub-squares
        NEsquare.buildSquare(midX, S.Xmax, S.Ymin, midY);
        NWsquare.buildSquare(S.Xmin, midX, S.Ymin, midY);
        SEsquare.buildSquare(midX, S.Xmax, midY, S.Ymax);
        SWsquare.buildSquare(S.Xmin, midX, midY, S.Ymax);

        // place each point into its corresponding square
        for (auto point : points) {
            if (point.x < midX) {
                if (point.y < midY) {
                    NWpoints.push_back(point);
                }
                else {
                    SWpoints.push_back(point);
                }
            }
            else {
                if (point.y < midY) {
                    NEpoints.push_back(point);
                }
                else {
                    SEpoints.push_back(point);
                }
            }
        }

        // increase number of nodes by 4 since there is a split
        numNodes += 4;

        // initial height of node with children is 1
        node->height = 1;

        int maxHeight = INT_MIN;

        node->ne = buildQuadtree(NEpoints, NEsquare);
        if (node->ne) {
            maxHeight = node->ne->height;
        }

        node->nw = buildQuadtree(NWpoints, NWsquare);
        if (node->nw) {
            if (maxHeight < node->nw->height) {
                maxHeight = node->nw->height;
            }
        }
        
        node->se = buildQuadtree(SEpoints, SEsquare);
        if (node->se) {
            if (maxHeight < node->se->height) {
                maxHeight = node->se->height;
            }
        }

        node->sw = buildQuadtree(SWpoints, SWsquare);
        if (node->sw) {
            if (maxHeight < node->sw->height) {
                maxHeight = node->sw->height;
            }
        }

        // take the max height of each children and add to your height
        node->height += maxHeight;
    }

    return node;
}

// find the bounding square for a set of points. 
square findBoundingSquare(const vector<point3D>& points) {

    square S;

    double Xmin = INT_MAX,
           Xmax = INT_MIN, 
           Ymin = INT_MAX,
           Ymax = INT_MIN;

    for (int i = 0; i < points.size(); i++) {
        point3D temp = points[i];
        Xmin = temp.x < Xmin ? temp.x : Xmin;
        Xmax = temp.x > Xmax ? temp.x : Xmax;
        Ymin = temp.y < Ymin ? temp.y : Ymin;
        Ymax = temp.y > Ymax ? temp.y : Ymax;
    }

    double xDiff = Xmax - Xmin,
           yDiff = Ymax - Ymin;

    // take the maximum between width and height to get a square that surrounds
    // all points
    if (xDiff > yDiff) {
        S.Xmin = Xmin;
        S.Xmax = Xmax;
        S.Ymin = Ymin - (xDiff - yDiff) / 2.00;
        S.Ymax = Ymax + (xDiff - yDiff) / 2.00;
    }
    else {
        S.Xmin = Xmin - (yDiff - xDiff) / 2.00;
        S.Xmax = Xmax + (yDiff - xDiff) / 2.00;
        S.Ymin = Ymin;
        S.Ymax = Ymax;
    }

    return S;
}

