
#include "SplashScreen.hpp"
#include "Controller/Yokai.hpp"
#include <glm/gtc/matrix_transform.hpp>

SplashScreen::SplashScreen(const std::string& texturePath)
{
    texture = TextureManager::getInstance().loadTexture(texturePath);
    setupShader();
    active = true;
}
void SplashScreen::setTexture(const std::string& texturePath)
{
    texture = TextureManager::getInstance().loadTexture(texturePath);
}
void SplashScreen::draw()
{
    glDisable(GL_DEPTH_TEST);
    splashShader->useShader();
    TextureManager::getInstance().getTexture(texture)->Bind(1);
    auto& engine = Yokai::getInstance();
    engine.renderer.Draw(*vao,6);
    glEnable(GL_DEPTH_TEST);
}
void SplashScreen::setupShader()
{
    splashShader = new Shader("content/Shaders/vertexShader.vert","content/Shaders/splashFragment.frag");
    splashShader->useShader();
    splashShader->setMat4("model",glm::mat4(1.0));
    splashShader->setMat4("view",glm::mat4(1.0));
    splashShader->setMat4("projection",glm::ortho(0.0f, (float)1920, (float)1080, 0.0f));
    splashShader->setInt("texture_diffuse1", 1);
}

void SplashScreen::setupPanel(float left, float right, float top, float bottom) 
{
    std::vector<Vertex> verts;
    verts.resize(4);
    verts[0].position = glm::vec3(left, top, 1);
    verts[1].position                 = glm::vec3(right, top, 1);
    verts[2].position                 = glm::vec3(left, bottom, 1);
    verts[3].position                 = glm::vec3(right, bottom, 1);
    verts[0].textureCoords            = glm::vec2(0, 0);
    verts[1].textureCoords            = glm::vec2(1, 0);
    verts[2].textureCoords            = glm::vec2(0, 1);
    verts[3].textureCoords            = glm::vec2(1, 1);
    std::vector<unsigned int> indices = {0, 1, 2, 1, 3, 2};
    vao                               = VertexArrayBuffer::Create(verts, indices);

}

void SplashScreen::setActive(bool a)
{
    active = a;
}

bool SplashScreen::getActive() const
{
    return active;
}