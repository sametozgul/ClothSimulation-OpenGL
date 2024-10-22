#ifndef CONTRAINTLINES_H
#define CONTRAINTLINES_H

#include "OGLRenderData.h"
class ContraintLines
{
public:
    OGLMesh VertexData;
    const OGLMesh getData(){return VertexData;};
};

#endif // CONTRAINTLINES_H
