#include "Assimp.h"

Model3D::Model3D(GLuint shaderProgIdArg, std::string &parentDirArg, std::string &modelPathArg){
    parentDir = parentDirArg;
    modelPath = modelPathArg;
    shaderProgId = shaderProgIdArg;

    std::string fullModelPath = parentDir + modelPath;

    loadModel(fullModelPath);
}

void Model3D::renderScene(){
    for(unsigned int i = 0; i < existingMeshes.size(); i++){
        existingMeshes[i].renderMesh();
    }
}

void Model3D::loadModel(std::string &fullModelPath){
    Assimp::Importer modelImporter;
    const aiScene* currentScene = modelImporter.ReadFile(
        fullModelPath.c_str(),
        aiProcess_Triangulate | // Converts to triangles if not done so
        aiProcess_FlipUVs | // Flips texture coordinates on Y axis
        aiProcess_GenNormals // Creates normals if model data does not contain
    );

    if(! currentScene || currentScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || ! currentScene->mRootNode){
        std::cout << "Assimp Error " << modelImporter.GetErrorString() << std::endl;
        return;
    }

    processNode(currentScene->mRootNode, currentScene);
}

void Model3D::processNode(aiNode *node, const aiScene *scene){
    // Process the meshes belonging to node
    for(unsigned int i = 0; i < node->mNumMeshes; i++){
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        existingMeshes.push_back(processMesh(mesh, currentScene));
    }

    // Recursive call to process child nodes and process meshes if any are present
    for(unsigned int i = 0; i < node->mNumChildren; i++){
        processNode(node->mChildren[i], scene);
    }

    // Ends only when root node and all child nodes have had their meshes processed
}

Mesh Model3D::processMesh(aiMesh *mesh, const aiScene *scene){
    std::vector<Point> meshPoints;
    std::vector<GLuint> meshIndices;
    std::vector<Texture> meshTextures;

    // Processes all attributes belonging to given mesh (points, indices, textures)
    for(unsigned int i = 0; i < mesh->mNumVertices; i++){
        Point myPoint;
        std::array<GLfloat, 3> currentVertexPos;
        std::array<GLubyte, 4> currentVertexColor = {255, 255, 255, 255};
        std::array<GLfloat, 2> currentVertexUV;
        std::array<GLfloat, 3> currentVertexNorm;

        currentVertexPos = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
        currentVertexNorm = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

        // Assimp allows up to 8 texture coordinates per vertex but we need first 2
        if(mesh->mTextureCoords[0]){ // Do they even exist?
            currentVertexUV = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].x };
            surfaceRenderMode = 0; // Textures applied
        } else {
            currentVertexUV = {0.0f, 0.0f};
            surfaceRenderMode = 1; // No textures applied
        }

        myPoint.pos = currentVertexPos;
        myPoint.color = currentVertexColor;
        myPoint.texCoord = currentVertexUV;
        myPoint.normal = currentVertexNorm;
        
        meshPoints.push_back(myPoint);
    }

    for(unsigned int i = 0; i < mesh->mNumFaces; i++){
        aiFace currentFace = mesh->mFaces[i];
        for(unsigned int f = 0; f < currentFace.mNumIndices; f++) meshIndices.push_back(currentFace.mIndices[f]);
    }

    if(mesh->mMaterialIndex >= 0){
        aiMaterial* activeMaterial = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = loadMaterialTextures(activeMaterial, aiTextureType_DIFFUSE, "texture_diffuse");
        meshTextures.insert(meshTextures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture> specularMaps = loadMaterialTextures(activeMaterial, aiTextureType_SPECULAR, "texture_specular");
        meshTextures.insert(meshTextures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(shaderProgId, meshPoints, meshIndices, meshTextures);
}

GLubyte genRandomColorAttrib(){
    int colorAttrib = std::rand() % 255;
    return GLubyte(colorAttrib);
}

std::vector<Texture> Model3D::loadMaterialTextures(aiMaterial* material, aiTextureType materialType, std::string materialTypeName){
    std::vector<Texture> allTextures;
    for(unsigned int i = 0; i < material->GetTextureCount(materialType); i++){
        aiString texFilePath;
        material -> GetTexture(materialType, i, &texFilePath);
        Texture currentTexture;
        currentTexture.texId = i;
        currentTexture.texFile = (texFilePath.C_Str());
        std::cout << currentTexture.texFile;
        allTextures.push_back(currentTexture);
    }
    return allTextures;
}



