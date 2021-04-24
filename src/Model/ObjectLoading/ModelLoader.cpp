//
// Created by Charlie Sewell on 1/04/2021.
//

#include "ModelLoader.hpp"

static auto to_glm(aiMatrix4x4t<float> &m){return glm::transpose(glm::make_mat4(&m.a1));}
static inline glm::vec3 vec3_cast(const aiVector3D &v) { return glm::vec3(v.x, v.y, v.z); }
static inline glm::vec2 vec2_cast(const aiVector3D &v) { return glm::vec2(v.x, v.y); }
static inline glm::quat quat_cast(const aiQuaternion &q) { return glm::quat(q.w, q.x, q.y, q.z); }

Model ModelLoader::loadModel(std::string filename)
{
    numBones = 0;
    std::vector<Mesh> meshes;
    std::vector<Bone> bones;
    std::vector<Animation> animations;
    std::map<std::string,unsigned int> boneMap;
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(
        filename, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_LimitBoneWeights | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "Error:" << importer.GetErrorString() << std::endl;
        return(Model(meshes));
    }
    // retrieve the directory path of the filepath
    directory = filename.substr(0, filename.find_last_of('/'));
    glm::mat4 globalInverseTransform = glm::inverse(to_glm(scene->mRootNode->mTransformation));
    // process ASSIMP's root node recursively
    processNode(animations,meshes,bones,boneMap,scene->mRootNode, scene,to_glm(scene->mRootNode->mTransformation));
    for(auto& mesh: meshes){
        mesh.SetupMesh();
    }
    std::cout << filename <<boneMap.size() <<std::endl;

    return Model(meshes,bones,boneMap,rootJoint,animations,globalInverseTransform);
}

void ModelLoader::processNode(std::vector<Animation> &animations,std::vector<Mesh> &meshes, std::vector<Bone> &bones,
                              std::map<std::string, unsigned int> &boneMap, aiNode *node,
                              const aiScene *scene, glm::mat4 transform) {
    // process each mesh located at the current node
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene.
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene, transform *to_glm(node->mTransformation)));
        if(scene->HasAnimations())
        {
            loadBones(meshes, bones, boneMap, i, mesh);
            loadAnimations(animations, scene);
            loadJoints(mesh,scene->mRootNode);
        }
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(animations,meshes,bones,boneMap,node->mChildren[i], scene, transform * to_glm(node->mTransformation));
    }
}

Mesh ModelLoader::processMesh(aiMesh *mesh, const aiScene *scene,glm::mat4 transform)
{
    // data to fill
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<ModelTexture> textures;

    // walk through each of the mesh's vertices
    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;
        // normals
        if (mesh->HasNormals()){
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
        }
        // texture coordinates
        if(mesh->mTextureCoords[0]) 
        {
            // does the mesh contain texture coordinates?
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.textureCoords = vec;
            // tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.tangent = vector;
            // biTangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.biTangent = vector;
        }
        else
            vertex.textureCoords = glm::vec2(0.0f, 0.0f);
        if (mesh->HasTangentsAndBitangents()) 
        {
            vector.x       = mesh->mTangents[i].x;
            vector.y       = mesh->mTangents[i].y;
            vector.z       = mesh->mTangents[i].z;
            vertex.tangent = vector;
            // bitangent
            vector.x         = mesh->mBitangents[i].x;
            vector.y         = mesh->mBitangents[i].y;
            vector.z         = mesh->mBitangents[i].z;
            vertex.biTangent = vector;
        }
        vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for(unsigned int i = 0; i < mesh->mNumFaces; i++){
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

    // 1. diffuse maps
    std::vector<ModelTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    std::vector<ModelTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<ModelTexture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<ModelTexture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    // return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices, textures,transform);
}

// checks all material textures of a given type and loads the textures if they're not loaded yet.
// the required info is returned as a Texture struct.
std::vector<ModelTexture> ModelLoader::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
    std::vector<ModelTexture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for(unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if(!skip)
        {   // if texture hasn't been loaded already, load it
            ModelTexture texture;
            std::string fileName = this->directory +"/" + str.C_Str();
            texture.texture = Texture::Create(fileName);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
        }
    }
    return textures;
}
void ModelLoader::loadAnimations(std::vector<Animation> &animations, const aiScene *scene){
    for(unsigned int i=0; i < scene->mNumAnimations;++i)
    {
        std::map<std::string,BoneAnimation> animationMap;
        //Load in all animations from a model
        for(unsigned int j =0; j < scene->mAnimations[i]->mNumChannels; ++j)
        {
            //Animation for a singular bone
            BoneAnimation boneAnim = {};
            std::string name = scene->mAnimations[i]->mChannels[j]->mNodeName.C_Str();
            boneAnim.numPositions = scene->mAnimations[i]->mChannels[j]->mNumPositionKeys;
            boneAnim.numRotations = scene->mAnimations[i]->mChannels[j]->mNumRotationKeys;
            for(unsigned int k=0;k < boneAnim.numPositions; ++k)
            {
                //Pushing back the Position at a certain time in the animation
                boneAnim.posKey.emplace_back(scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mTime, vec3_cast(scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue));
            }
            for(unsigned int k=0;k < boneAnim.numRotations; ++k)
            {
                //Pushing back the Rotation at a certain time in the animation
                boneAnim.rotKey.emplace_back(scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mTime, quat_cast(scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue));
            }
            //Place it in a map with the name so animation can be called
            animationMap.emplace(name,boneAnim);
        }
        //std::cout << scene->mAnimations[i]->mName.C_Str() <<std::endl;
        Animation anim = Animation(scene->mAnimations[i]->mName.C_Str(),animationMap, scene->mAnimations[i]->mDuration,scene->mAnimations[i]->mTicksPerSecond);
        animations.push_back(anim);
    }

}
static inline Joint RecurseJoints(aiNode *node, aiNode *rootScene) {
    Joint parent = {};
    for (size_t i = 0; i < node->mNumChildren; ++i) {
        parent.children.push_back(
            RecurseJoints(rootScene->FindNode(node->mChildren[i]->mName), rootScene));
    }
    parent.name      = std::string(node->mName.C_Str());
    parent.transform = to_glm(node->mTransformation);
    return parent;
}
static inline aiNode *FindRootJoint(aiMesh *mesh, aiNode *root) {
    std::map<std::string, aiString> boneNames = {};
    for (size_t i = 0; i < mesh->mNumBones; ++i) {
        boneNames.insert({std::string(mesh->mBones[i]->mName.C_Str()), mesh->mBones[i]->mName});
    }
    for (size_t i = 0; i < mesh->mNumBones; ++i) {
        auto *boneNode      = root->FindNode(mesh->mBones[i]->mName);
        auto boneNodeParent = boneNode->mParent->mName;
        if (!boneNames.count(std::string(boneNodeParent.C_Str()))) {
            return root->FindNode(boneNodeParent);
        }
    }
    return nullptr;
}

void ModelLoader::loadJoints(aiMesh *mesh, aiNode *root) {
    auto rootBone = FindRootJoint(mesh, root);
    if (rootBone == nullptr) {
        std::cerr << "ERROR: ROOT JOINT NOT FOUND";
        EXIT_FAILURE;
    }
    rootJoint = RecurseJoints(rootBone, root);
}
/*
void ModelLoader::loadJoints(Joint &rootJoint, aiNode *rootNode){
    if(rootNode == nullptr)
    {
        return;
    }
    rootJoint.name = rootNode->mName.data;
    rootJoint.transform = to_glm(rootNode->mTransformation);
    rootJoint.childrenCount = rootNode->mNumChildren;

    for (int i = 0; i < rootNode->mNumChildren; i++)
    {
        Joint newData;
        loadJoints(newData, rootNode->mChildren[i]);
        rootJoint.children.push_back(newData);
    }
}
*/
void ModelLoader::loadBones(std::vector<Mesh> &meshes, std::vector<Bone> &bones,std::map<std::string,unsigned int> &boneMap, unsigned int meshIndex, const aiMesh *mesh)
{
    for (unsigned i = 0 ; i < mesh->mNumBones; ++i) {
        unsigned boneIndex = 0;
        std::string boneName(mesh->mBones[i]->mName.data);

        if (boneMap.find(boneName) == boneMap.end()) {
            std::cout << boneName <<std::endl;
            boneIndex = numBones;
            ++numBones;
            Bone bi;
            bones.push_back(bi);
        }
        else {
            boneIndex = boneMap[boneName];
        }

        boneMap[boneName] = boneIndex;
        bones[boneIndex].offset = to_glm(mesh->mBones[i]->mOffsetMatrix);

        for (unsigned j = 0 ; j < mesh->mBones[i]->mNumWeights; ++j) {
            unsigned VertexID = mesh->mBones[i]->mWeights[j].mVertexId;
            float Weight = mesh->mBones[i]->mWeights[j].mWeight;
            meshes.at(meshIndex).addBoneData(VertexID, boneIndex, Weight);
        }
    }
}