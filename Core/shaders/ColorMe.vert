#version 430 core

layout(location = 0) in vec3 pos;

uniform mat4 mvpMatrix;

uniform float xMin;
uniform float xMax;
uniform float yMin;
uniform float yMax;
uniform float zMin;
uniform float zMax;

layout(location = 0) out uint vertexID_out;
layout(location = 1) out vec3 colorScale;

void main(){
    float xRange = xMax - xMin;
    colorScale.r = (pos.x - xMin) / xRange;
    float yRange = yMax - yMin;
    colorScale.g = (pos.y - yMin) / yRange;
    float zRange = zMax - zMin;
    colorScale.b = (pos.z - zMin) / zRange;
    
    /* if(pos.x > 0.0){
        colorScale.r = 1.0;
    } else {
        colorScale.r = 0.5;
    } */
    // colorScale.r = pos.x / xMax;
    //colorScale.g = 0.0;
    //colorScale.b = 0.0;

    vec4 wPos = vec4(pos, 1.0);
    vertexID_out = gl_VertexID;

    gl_Position = mvpMatrix * wPos;
}