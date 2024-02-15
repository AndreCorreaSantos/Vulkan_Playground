#include <vector>
#include "model.hpp"
#include <chrono>
#include "db_perlin.hpp"
#include <memory>



namespace lve{
    std::vector<LveModel::Vertex> generateMeshVertices(int numpoints){
    std::vector<LveModel::Vertex> vertices;

    auto currentTime = std::chrono::high_resolution_clock::now();

    float scale = 0.6f;

    for ( int u = 0; u < numpoints; u++){
        float y = u;
        for (int i = 0; i < numpoints; i++){
        float x = i;

        float X = 1.0f;
        float z = 40*db::perlin(x / 64.0f, float(y) /64.0f,1.0f);

        // if( (position[1]< 1.0)&&(position[1]> -1.0) )
        //   {
        //   fragColor = vec3(0.5, 0.35, 0.05);
        //   } else if( position[1] < -12.0) {
        //    fragColor = vec3(0.8,0.8,0.8);
        //   }
        glm::vec3 color = {.1f, .1f, z};
        if((z< 1.0)&&(z> -1.0)){
            color = {0.5f, 0.35f, 0.05f};
        } else if(z < -12.0){
            color = {0.8f,0.8f,0.8f};
        }

        vertices.push_back({{x*scale - 2 ,z,y*scale - 2},color});
        }
    }
    return vertices;
    }


    std::vector<uint32_t> generateMeshIndices(int numpoints){ // function to generate mesh indices for plane
    std::vector<uint32_t> indices;
    for (int u =0;u<numpoints-1;u++){
        for (int i =0;i<numpoints-1;i++){
            int c = i*numpoints + u;
            int r = (i+1)*numpoints + u;
            indices.push_back(c);
            indices.push_back(c+1);
            indices.push_back(r);
            indices.push_back(r);
            indices.push_back(r+1);
            indices.push_back(c+1);
        }
    }
    return indices;
    }

    std::vector<LveModel::Vertex> generateSphereVertices(float radius,float PI,int sectorCount, int stackCount){
    std::vector<LveModel::Vertex> vertices;
    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
    float s, t;                                     // vertex texCoord

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    for(int i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for(int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            vertices.push_back({{x,y,z},{1.0f,1.0f,0.1f}});
        }
    }
    return vertices;
    }

    std::vector<uint32_t> generateMeshIndices(int sectorCount, int stackCount){ // function to generate mesh indices for sphere
    std::vector<uint32_t> indices;
    int k1, k2;
    for(int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack

        for(int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if(i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }
            // k1+1 => k2 => k2+1
            if(i != (stackCount-1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
    return indices;
    }



    // temporary helper function, creates a 1x1x1 mesh centered at offset with an index buffer
    std::unique_ptr<LveModel> createmeshModel(LveDevice& device, glm::vec3 offset) {
    LveModel::Builder modelBuilder{};
    int numpoints = 1000;
    modelBuilder.vertices = generateMeshVertices(numpoints);
    for (auto& v : modelBuilder.vertices) {
        v.position += offset;
    }
    modelBuilder.indices = generateMeshIndices(numpoints);
    return std::make_unique<LveModel>(device, modelBuilder);
    }

    std::unique_ptr<LveModel> createsphereModel(LveDevice& device, glm::vec3 offset){
    int sectorCount = 50;
    int stackCount = 50;
    float pi = 3.14159f;
    float radius = 20.0f;
    LveModel::Builder modelBuilder{};
    modelBuilder.vertices = generateSphereVertices(radius,pi,sectorCount,stackCount);
    for (auto& v : modelBuilder.vertices) {
        v.position += offset;
    }
    modelBuilder.indices = generateMeshIndices(sectorCount,stackCount);
    return std::make_unique<LveModel>(device, modelBuilder);
    }
}