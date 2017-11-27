#ifndef OCTREE_H
#define OCTREE_H

#include <cstddef>
#include <vector>
#include "src/util/CommonIncludes.h"
#include "src/engine/geometric/mesh/meshtriangle.h"

class OctreePoint {
public:
    const glm::vec3 position;
    const size_t index;
    OctreePoint() :index(0) { }
    OctreePoint(const glm::vec3& position, const size_t ind) : position(position), index(ind)
    {
    }
    inline const glm::vec3& getPosition() const { return position; }
    inline size_t getIndex() const {return index;}
};


    /**!
     *
     */
    class Octree {
        // Physical position/size. This implicitly defines the bounding
        // box of this node
        glm::vec3 origin;         //! The physical center of this node
        glm::vec3 halfDimension;  //! Half the width/height/depth of this node

        // The tree has up to eight children and can additionally store
        // a point, though in many applications only, the leaves will store data.
        Octree *children[8]; //! Pointers to child octants
        OctreePoint *data;   //! Data point to be stored at a node

        /*
                Children follow a predictable pattern to make accesses simple.
                Here, - means less than 'origin' in that dimension, + means greater than.
                child:	0 1 2 3 4 5 6 7
                x:      - - - - + + + +
                y:      - - + + - - + +
                z:      - + - + - + - +
         */

        public:
        Octree(const glm::vec3& origin, const glm::vec3& halfDimension)
            : origin(origin), halfDimension(halfDimension), data(NULL) {
                // Initially, there are no children
                for(int i=0; i<8; ++i)
                    children[i] = NULL;
            }

        Octree(const Octree& copy)
            : origin(copy.origin), halfDimension(copy.halfDimension), data(copy.data) {

            }

        ~Octree() {
            // Recursively destroy octants
            for(int i=0; i<8; ++i)
                delete children[i];
        }

        // Determine which octant of the tree would contain 'point'
        int getOctantContainingPoint(const glm::vec3& point) const {
            int oct = 0;
            if(point.x >= origin.x) oct |= 4;
            if(point.y >= origin.y) oct |= 2;
            if(point.z >= origin.z) oct |= 1;
            return oct;
        }

        bool isLeafNode() const {
            // This is correct, but overkill. See below.
            /*
                 for(int i=0; i<8; ++i)
                 if(children[i] != NULL)
                 return false;
                 return true;
             */

            // We are a leaf iff we have no children. Since we either have none, or
            // all eight, it is sufficient to just check the first.
            return children[0] == NULL;
        }

        bool insert(OctreePoint* point) {
            // If this node doesn't have a data point yet assigned
            // and it is a leaf, then we're done!
            if(isLeafNode())
            {
                if(data==NULL)
                {
                    data = point;
                    return true;
                }
                else if(point->getPosition() == data->getPosition())
                {
                    return false;
                }
                else
                {
                    // We're at a leaf, but there's already something here
                    // We will split this node so that it has 8 child octants
                    // and then insert the old data that was here, along with
                    // this new data point

                    // Save this data point that was here for a later re-insert
                    OctreePoint *oldPoint = data;
                    data = NULL;

                    // Split the current node and create new empty trees for each
                    // child octant.
                    for(int i=0; i<8; ++i)
                    {
                        // Compute new bounding box for this child
                        glm::vec3 newOrigin = origin;
                        newOrigin.x += halfDimension.x * (i&4 ? .5f : -.5f);
                        newOrigin.y += halfDimension.y * (i&2 ? .5f : -.5f);
                        newOrigin.z += halfDimension.z * (i&1 ? .5f : -.5f);
                        Octree *child = new Octree(newOrigin, halfDimension*.5f);
                        children[i] = child;
                    }

                    // Re-insert the old point, and insert this new point
                    // (We wouldn't need to insert from the root, because we already
                    // know it's guaranteed to be in this section of the tree)
                    children[getOctantContainingPoint(oldPoint->getPosition())]->insert(oldPoint);
                    children[getOctantContainingPoint(point->getPosition())]->insert(point);
                    return true;
                }
            }
            else
            {
                // We are at an interior node. Insert recursively into the
                // appropriate child octant
                int octant = getOctantContainingPoint(point->getPosition());
                children[octant]->insert(point);
                return true;
            }
        }

        // This is a really simple routine for querying the tree for points
        // within a bounding box defined by min/max points (bmin, bmax)
        // All results are pushed into 'results'
        void getPointsInsideBox(const glm::vec3& bmin, const glm::vec3& bmax, std::vector<OctreePoint*>& results) {
            // If we're at a leaf node, just see if the current data point is inside
            // the query bounding box
            if(isLeafNode()) {
                if(data!=NULL) {
                    const glm::vec3& p = data->getPosition();
                    if(p.x>bmax.x || p.y>bmax.y || p.z>bmax.z) return;
                    if(p.x<bmin.x || p.y<bmin.y || p.z<bmin.z) return;
                    results.push_back(data);
                }
            } else {
                // We're at an interior node of the tree. We will check to see if
                // the query bounding box lies outside the octants of this node.
                for(int i=0; i<8; ++i) {
                    // Compute the min/max corners of this child octant
                    glm::vec3 cmax = children[i]->origin + children[i]->halfDimension;
                    glm::vec3 cmin = children[i]->origin - children[i]->halfDimension;

                    // If the query rectangle is outside the child's bounding box,
                    // then continue
                    if(cmax.x<bmin.x || cmax.y<bmin.y || cmax.z<bmin.z) continue;
                    if(cmin.x>bmax.x || cmin.y>bmax.y || cmin.z>bmax.z) continue;

                    // At this point, we've determined that this child is intersecting
                    // the query bounding box
                    children[i]->getPointsInsideBox(bmin,bmax,results);
                }
            }
        }

    };

#endif // OCTREE_H