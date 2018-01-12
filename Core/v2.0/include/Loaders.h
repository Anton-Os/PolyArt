#include <iostream>
#include <cstdio>
#include <string>
#include <cstring>
#include <stack>
#include <vector>

#include <GL/glew.h>

#ifndef LOADERS_H
    // From LoadData.cpp // Legacy

    GLuint loadData(GLuint VAO, GLuint vertexCount, GLenum drawUsage, GLfloat* pos, GLfloat* color, GLfloat* normal, GLfloat* texCoord);
    void loadIndices(GLuint VAO, GLuint indexCount, GLenum drawUsage, GLuint* index);

    // From FileIO.cpp

    const std::string getParentDirectory(const char* path);
    char* readFile(const char* fileName);
    void writeFile(const std::string& fileName, const std::string& contents);

    // From LoadShaders.cpp

    GLchar* readShaderFile(const char* nameOfShader);
    GLuint compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

    // From LoadTextures.cpp

    GLuint createTexture_gli(const char* Filename);
    GLuint createTexture_stbi(const char* Filename);

    // From LoadModel.cpp

    /* void assimpImport(std::string filepath, GL4_Tree* tree); */

#define LOADERS_H
#endif