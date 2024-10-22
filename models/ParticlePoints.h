#ifndef PARTICLEPOINTS_H
#define PARTICLEPOINTS_H

#include <OGLRenderData.h>
class ParticlePoints
{
public:
    OGLMesh VertexData;
    const OGLMesh getData(){return VertexData;};
};

#endif // PARTICLEPOINTS_H
