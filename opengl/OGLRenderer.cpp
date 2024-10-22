#include "OGLRenderer.h"
#include <iostream>
OGLRenderer::OGLRenderer(GLFWwindow* win)
{
    mRenderData.rdWindow = win;
}

bool OGLRenderer::init(unsigned int width, unsigned int height)
{
    mRenderData.rdWidth = width;
    mRenderData.rdHeight = height;

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        Logger::log(1,"%s :Failed to initialize GLAD.\n",__FUNCTION__);
        return false;
    }
    if (!GLAD_GL_VERSION_4_6) {
        Logger::log(1, "%s error: failed to get at least OpenGL 4.6\n", __FUNCTION__);
        return false;
    }
    mShader.loadShaders("Debug/shaders/Fragment.vert","Debug/shaders/Fragment.frag");
    mShader.use();
    mUniformBuffer.init();
    Logger::log(1,"%s : OpenGL Renderer is initialized.\n",__FUNCTION__);
    mUserInterface.init(mRenderData);
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS,&mRenderData.rdNrAttributes);

    // initialize particles
    for(int row = 0; row < ROW; ++row){
        for(int col = 0; col < COL; ++col){
            float x = col * REST_DISTANCE + mRenderData.rdWidth / 3.0f;
            float y = row * REST_DISTANCE + mRenderData.rdHeight / 3.0f;
            bool pinned = (row == 0);
            mParticles.emplace_back(x, y, pinned);
        }
    }
    // initialize constraints
    for (int row = 0; row < ROW; row++) {
        for (int col = 0; col < COL; col++) {
            if (col < COL - 1) {
                mConstraints.emplace_back(&mParticles[row * COL + col], &mParticles[row * COL + col + 1]);
            }
            if (row < ROW - 1) {
                mConstraints.emplace_back(&mParticles[row * COL + col], &mParticles[(row + 1) * COL + col]);
            }
        }
    }

    mVertexBuffer.init();
    return true;
}
glm::vec2 OGLRenderer::normalize(glm::vec2 position){
    glm::vec2 normalizedPosition;
    normalizedPosition.x = (2.0f * position.x) / mRenderData.rdWidth - 1.0f;
    normalizedPosition.y = 1.0f - (2.0f * position.y) / mRenderData.rdHeight;
    return normalizedPosition;
}
void OGLRenderer::resize(unsigned int width, unsigned int height)
{
    mRenderData.rdWidth = width;
    mRenderData.rdHeight = height;
    glViewport(0,0,width,height);
}
void ::OGLRenderer::draw()
{
    double tickTime = glfwGetTime();
    mRenderData.rdTickDiff = tickTime - mLastTickTime;

    mRenderData.rdFrameTime = mFrameTimer.stop();
    mFrameTimer.start();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto& particle : mParticles) {
        particle.applyForce(glm::vec2(0, mRenderData.gravity));
        particle.update(TIME_STEP);
        particle.constrainToBounds(mRenderData.rdWidth, mRenderData.rdHeight);
    }
    for (size_t i = 0; i < 5; i++) {
        for (auto& constraint : mConstraints) {
            constraint.satisfy();
        }
    }
    mParticlePoints.VertexData.vertices.clear();
    mParticlePoints.VertexData.vertices.resize(mParticles.size());
    for(auto i = 0;i < mParticles.size(); ++i){
        mParticlePoints.VertexData.vertices[i].position = normalize(mParticles[i].position);
        mParticlePoints.VertexData.vertices[i].color = glm::vec3(1.0f,1.0f,1.0f);
    }
    mContraintLines.VertexData.vertices.clear();
    for(auto i = 0;i < mConstraints.size(); ++i){
        if(!mConstraints[i].active)
            continue;
        OGLVertex tempVertex;
        tempVertex.position = normalize(mConstraints[i].p1_->position);
        tempVertex.color = glm::vec3(1.0f,1.0f,1.0f);
        mContraintLines.VertexData.vertices.emplace_back(tempVertex);
        tempVertex.position = normalize(mConstraints[i].p2_->position);
        mContraintLines.VertexData.vertices.emplace_back(tempVertex);
    }


    mShader.use();

    //Draw Points
    mVertexBuffer.uploadData(mParticlePoints.getData());
    mVertexBuffer.bindAndDraw(GL_POINTS, 0, mParticlePoints.getData().vertices.size());

    //Draw Lines
    mVertexBuffer.uploadData(mContraintLines.getData());
    mVertexBuffer.bindAndDraw(GL_LINES, 0, mContraintLines.getData().vertices.size());

    glBindVertexArray(0);
    mUserInterface.createFrame(mRenderData);
    mUserInterface.render();

    mLastTickTime = tickTime;


}
float OGLRenderer::point_to_segment_distance(float px, float py, float x1, float y1, float x2, float y2) {
    float ABx = x2 - x1;
    float ABy = y2 - y1;

    float APx = px - x1;
    float APy = py - y1;

    float BPx = px - x2;
    float BPy = py - y2;

    float AB_AP = ABx * APx + ABy * APy;
    float AB_AB = ABx * ABx + ABy * ABy;
    float t = AB_AP / AB_AB;

    // Project point P ont the line segment AB
    if (t < 0.0f) {
        // P is closer to A
        return std::sqrt(APx * APx + APy * APy);
    }
    else if (t > 1.0f) {
        // P is closer to B
        return std::sqrt(BPx * BPx + BPy * BPy);
    }
    else {
        // projection point is on the segment
        float proj_x = x1 + t * ABx;
        float proj_y = y1 + t * ABy;
        return std::sqrt((px - proj_x) * (px - proj_x) + (py - proj_y) * (py - proj_y));
    }
}
Constraint* OGLRenderer::find_nearest_constraint(float mouse_x, float mouse_y,
                                    const std::vector<Constraint>& constraints) {
    Constraint *nearest_constraint = nullptr;
    float min_distance = CLICK_TOLERANCE;

    for (const auto& constraint : constraints) {
        float distatnce = point_to_segment_distance(mouse_x, mouse_y,
                                                    constraint.p1_->position.x, constraint.p1_->position.y,
                                                    constraint.p2_->position.x, constraint.p2_->position.y);
        if (distatnce < min_distance) {
            min_distance = distatnce;
            nearest_constraint = const_cast<Constraint*>(&constraint);
        }
    }
    return nearest_constraint;
}
void OGLRenderer::tear_cloth(float mouse_x, float mouse_y, const std::vector<Particle>& particles,
                std::vector<Constraint>& constraints) {
    Constraint *nearest = find_nearest_constraint(mouse_x, mouse_y, constraints);
    if (nearest) {
        nearest->deactivate();
    }
}

void OGLRenderer::checkMouseStates(double xPos, double yPos)
{
    if(mIsMousePressing){
        mRenderData.rdXLastPos = xPos;
        mRenderData.rdYLastPos = yPos;
        tear_cloth(mRenderData.rdXLastPos, mRenderData.rdYLastPos, mParticles, mConstraints);
    }
}
void OGLRenderer::handleMousePressEvents(double xPos, double yPos)
{
    mRenderData.rdXLastPos = xPos;
    mRenderData.rdYLastPos = yPos;
    mIsMousePressing = true;
}

void OGLRenderer::handleMouseReleasedEvents()
{
    mIsMousePressing = false;
}

void OGLRenderer::cleanUp()
{
    mUserInterface.cleanUp();
    mShader.cleanup();
    mVertexBuffer.cleanup();
}

