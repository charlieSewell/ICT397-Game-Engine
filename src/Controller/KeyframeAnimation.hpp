#pragma once

#include <iostream>
#include <vector>
#include "Model/Model.hpp"
#include "Model/ObjectLoading/ModelLoader.hpp"
#include <fstream>
#include "Model/Components/Transform.hpp"

class KeyframeAnimation 
{
	public:

        KeyframeAnimation();

        void readFile(std::string textPath);

        void setAnimation(std::string name);

        void addAnimation(std::string animation, int firstFrame, int lastFrame);

        std::string getCurrentAnimation();

        void collectModel(std::string modelPath);

        void setCurrentFrame(double dTime);

        int getCurrentFrame();

        int getEndFrame();

        void draw(Transform t);

        void setTPS(float tps);

        float getTPS();

        bool getAnimationFinished();

        void swapAnimationCheck();

        void testing();

	private:

      std::shared_ptr<Shader> shader;
        
      std::vector<Model> models;

      ModelLoader loader;

      int currentFrame;

      int startFrame;

      int endFrame;

      double totalTime;

      float ticksPerSecond;

      std::map<std::string, std::pair<int, int>> animations;

      std::string currentAnimation;

      bool animationFinished;

      std::string tempAnimation;
};