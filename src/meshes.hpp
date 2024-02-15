#pragma once

#include <vector>
#include <memory>
#include <chrono>
#include <glm/glm.hpp>
#include "model.hpp"
#include "device.hpp"
#include "db_perlin.hpp"

namespace lve {
    std::vector<LveModel::Vertex> generateMeshVertices(int numpoints);
    std::vector<uint32_t> generateMeshIndices(int numpoints);
    std::vector<LveModel::Vertex> generateSphereVertices(float radius,float PI,int sectorCount, int stackCount);
    std::vector<uint32_t> generateMeshIndices(int sectorCount, int stackCount);
    std::unique_ptr<LveModel> createmeshModel(LveDevice& device, glm::vec3 offset);
    std::unique_ptr<LveModel> createsphereModel(LveDevice& device, glm::vec3 offset);
}
