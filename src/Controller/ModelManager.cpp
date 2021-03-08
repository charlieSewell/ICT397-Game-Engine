//
// Created by Charlie Sewell on 23/02/2021.
//

#include "ModelManager.hpp"
auto ModelManager::GetModelID(std::string filename) -> size_t {
    auto id = modelIDtoName.find(filename);
    //stops files being loaded more then once
    if(id == modelIDtoName.end()){
        models[modelCount] = Model(filename.c_str());
        modelIDtoName.emplace(filename,modelCount);
        modelCount++;
        return modelCount-1;
    }
    return(id->second);
}
Model* ModelManager::GetModel(size_t modelID) {
    return &models[modelID];
}
void ModelManager::DrawModel(size_t id, Shader shader) {
    models.at(id).Draw(shader);
}
