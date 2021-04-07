//
// Created by Charlie Sewell on 7/04/2021.
//

#include "SplashScreen.hpp"
#include "Controller/Yokai.hpp"
SplashScreen::SplashScreen()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(Yokai::getInstance().window.getWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 150");
    std::vector<Vertex> verts;
    verts.resize(4);
    verts[0].position= glm::vec3(400,200,1);
    verts[1].position= glm::vec3(400,1520,1);
    verts[2].position= glm::vec3(680,200,1);
    verts[3].position= glm::vec3(680,1520,1);
    std::vector<unsigned int> indices = {0,1,2,3,1};
    vao = VertexArrayBuffer::Create(verts,indices);
    texture = Texture::Create("content/Textures/exit_screen.png");
    splashShader = new Shader("content/Shaders/vertexShader.frag","content/Shaders/fragmentShader.frag");
}
void SplashScreen::setTexture(std::string texturePath)
{
    texture =  Texture::Create(texturePath);
}
void SplashScreen::draw()
{
    ImGui::Begin("Thanks For Playing");
    ImGui::Image((void*)(intptr_t)texture->getID(),ImVec2(1920, 1080));
    ImGui::End();
}