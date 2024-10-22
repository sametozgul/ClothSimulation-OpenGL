#ifndef OGLRENDERER_H
#define OGLRENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Timer.h"
#include "OGLRenderData.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "UserInterface.h"
#include "UniformBuffer.h"
#include "Particle.h"
#include "ParticlePoints.h"
#include "Constraint.h"
#include "ContraintLines.h"

const float PARTICLE_RADIUS = 10.0f;
const float TIME_STEP = 0.1f;

const int ROW = 200;
const int COL = 50;
const float REST_DISTANCE = 30.0f;
const float CLICK_TOLERANCE = 5.0f;
class OGLRenderer
{
public:
    OGLRenderer(GLFWwindow* win);
    bool init(unsigned int width, unsigned int height);
    void resize(unsigned int width, unsigned int height);
    void handleMousePressEvents(double xPos, double yPos);
    void handleMouseReleasedEvents();
    void draw();
    void cleanUp();

    glm::vec2 normalize(glm::vec2 position);
    static float point_to_segment_distance(float px, float py, float x1, float y1, float x2, float y2);
    static Constraint *find_nearest_constraint(float mouse_x, float mouse_y, const std::vector<Constraint> &constraints);
    static void tear_cloth(float mouse_x, float mouse_y, const std::vector<Particle> &particles, std::vector<Constraint> &constraints);

    void checkMouseStates(double xPos, double yPos);
private:
    Timer mFrameTimer{};
    OGLRenderData mRenderData{};
    Shader mShader{};
    VertexBuffer mVertexBuffer{};
    UserInterface mUserInterface{};
    UniformBuffer mUniformBuffer{};

    double mLastTickTime = 0.0;

    bool mIsMousePressing = false;
    std::vector<Particle> mParticles;
    ParticlePoints mParticlePoints;
    std::vector<Constraint> mConstraints;
    ContraintLines mContraintLines;
};

#endif // OGLRENDERER_H
