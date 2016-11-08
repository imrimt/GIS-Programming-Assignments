/* 

*/

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
static void treeNode_free(treeNode* node) {
    //fill in 
    if (node->nw) delete node->nw;
    if (node->ne) delete node->ne;
    if (node->sw) delete node->sw;
    if (node->se) delete node->se;
}


/* free all memory allocated for the tree, including the tree
itself */
void quadtree_free(quadtree *tree) {

if (!tree) return; 
    treeNode_free(tree->root); 
    free(tree); 
}


/* create a tree representing the  array of points */
quadtree* quadtree_build(const vector<point3D>& points, int n, int max_points_per_leaf) {

    quadtree* tree = quadtree_init();

    // max_points_per_leaf = max_points_per_leaf;

    square firstSqr;

    firstSqr = findSurroundingSquare(points);

    printf("Xmin = %.2f\n", firstSqr.Xmin);
    printf("Xmax = %.2f\n", firstSqr.Xmax);
    printf("Ymin = %.2f\n", firstSqr.Ymin);
    printf("Ymax = %.2f\n", firstSqr.Ymax);

    treeNode* root = buildQuadtree(points, firstSqr);

    tree->root = root;
    tree->max_points_per_leaf = max_points_per_leaf;

    return tree;
}

treeNode* buildQuadtree(const vector<point3D>& points, const square &S) {

    treeNode* node = new (nothrow) treeNode();
    assert(node);

    int numPoints = points.size();

    // if points have at most max_poitns_per_leaf, then store P in node
    // and return
    if (numPoints <= max_points_per_leaf) {
        if (numPoints == 0) {
            return NULL;
        }
        node->p = points;
        node->k = numPoints;
        node->ne = node->nw = node->se = node->sw = NULL;
    }

    // partition S into 4 quadrants, then partition P into 4 corresponding 
    // subsets
    else {

        double midX = (S.Xmax + S.Xmin) / 2.00,
               midY = (S.Ymax + S.Ymin) / 2.00;

        printf("Xmin = %.2f\n", S.Xmin);
        printf("Xmax = %.2f\n", S.Xmax);
        printf("Ymin = %.2f\n", S.Ymin);
        printf("Ymax = %.2f\n", S.Ymax);
        printf("Xmid = %.2f\n", midX);
        printf("Ymid = %.2f\n", midY);
        printf("============\n");

        if ((S.Xmax - S.Xmin) < EPSILON || (S.Ymax - S.Ymin) < EPSILON) {
            return NULL;
        }

        vector<point3D> NEpoints, NWpoints, SEpoints, SWpoints;

        square NEsquare, NWsquare, SEsquare, SWsquare;

        NEsquare.Xmin = midX;
        NEsquare.Ymin = S.Ymin;
        NEsquare.Xmax = S.Xmax;
        NEsquare.Ymax = midY;

        NWsquare.Xmin = S.Xmin;
        NWsquare.Ymin = S.Ymin;
        NWsquare.Xmax = midX;
        NWsquare.Ymax = midY;

        SEsquare.Xmin = midX;
        SEsquare.Ymin = midY;
        SEsquare.Xmax = S.Xmax;
        SEsquare.Ymax = S.Ymax;

        SWsquare.Xmin = S.Xmin;
        SWsquare.Ymin = midY;
        SWsquare.Xmax = midX;
        SWsquare.Ymax = S.Ymax;

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

        node->ne = buildQuadtree(NEpoints, NEsquare);
        node->nw = buildQuadtree(NWpoints, NWsquare);
        node->se = buildQuadtree(SEpoints, SEsquare);
        node->sw = buildQuadtree(SWpoints, SWsquare);

        node->k = node->ne->k + node->nw->k + node->se->k + node->sw->k;
    }

    return node;
}

square findSurroundingSquare(const vector<point3D>& points) {

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

    S.Xmin = Xmin;
    S.Xmax = Xmax;
    S.Ymin = Ymin;
    S.Ymax = Ymax;

    return S;
}

