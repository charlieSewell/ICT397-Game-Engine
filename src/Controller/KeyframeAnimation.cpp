#include "KeyframeAnimation.hpp"

KeyframeAnimation::KeyframeAnimation() 
{
    shader = std::make_unique<Shader>("content/Shaders/vertexShader.vert", "content/Shaders/fragmentShader.frag");
    totalTime = 0;
    ticksPerSecond = 30;
}

void KeyframeAnimation::readFile(std::string textPath) 
{
    std::ifstream file(textPath);
    std::string path;
    if (file) 
    {
        while (std::getline(file, path)) 
        {
            collectModel(path);
        }
    } 
    else 
    {
        std::cout << "Error opening file: " + textPath << std::endl;
    }
}

void KeyframeAnimation::collectModel(std::string modelPath) 
{
    Model model = loader.loadModel(modelPath);
    models.push_back(model);
}

void KeyframeAnimation::setAnimation(std::string name)
{
    currentAnimation = name;
}

std::string KeyframeAnimation::getCurrentAnimation()
{
    return currentAnimation;
}

void KeyframeAnimation::addAnimation(std::string animation, int firstFrame, int lastFrame) 
{
    if (firstFrame < 0) 
    {
        firstFrame = 1;
    }

    if (lastFrame > models.size()) 
    {
        lastFrame = models.size() - 1;
    }

    animations.emplace(animation, std::pair<int, int>(firstFrame, lastFrame));
}

void KeyframeAnimation::setCurrentFrame(double deltaTime) 
{
    totalTime += deltaTime;

    if (totalTime >
        ((animations.at(currentAnimation).second - animations.at(currentAnimation).first + 1) / ticksPerSecond))
    {
        totalTime = 0;
    }

    currentFrame = static_cast<int>(totalTime * ticksPerSecond);
    currentFrame += animations.at(currentAnimation).first - 1;
}

int KeyframeAnimation::getCurrentFrame() 
{
    return currentFrame;
}

void KeyframeAnimation::draw() 
{
    glm::mat4 transform(1.0);
    transform = glm::translate(transform, glm::vec3(512, 25, 512));
    transform = glm::scale(transform, glm::vec3(0.03, 0.03, 0.03));

    models[currentFrame].Draw(*shader, transform);
}

void KeyframeAnimation::setTPS(float tps) 
{
    ticksPerSecond = tps;
}

float KeyframeAnimation::getTPS() 
{
    return ticksPerSecond;
}