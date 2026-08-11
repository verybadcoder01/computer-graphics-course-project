#include "objModel.h"
#include "exceptions/model/modelExceptions.h"
#include "sceneObject/primitive/model/model.h"
#include <filesystem>
#include <source_location>
#include <fstream>

ObjModel::ObjModel(const std::string &name, 
                 const std::filesystem::path &filepath, 
                 const std::filesystem::path &texturePath): filepath(filepath) {
    this->name = name;
    loadFromOBJ(filepath);

    if (!texturePath.empty()) {
        loadTexture(texturePath);
    }
}

void ObjModel::loadTexture(const std::filesystem::path &texturePath) {
    texture = std::make_shared<Texture>(texturePath);
}

void ObjModel::loadFromOBJ(const std::filesystem::path& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw FailedToLoadModelException(
            std::format("Could not load model from path {}", filepath.string()), 
            std::source_location::current()
        );
    }

    std::vector<glm::vec3> tempPositions;
    std::vector<glm::vec3> tempNormals;
    std::vector<glm::vec2> tempTexCoords;

    std::vector<Vertex> tempVertexes;
    std::vector<uint32_t> tempIndices;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }

        std::istringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "v") {
            glm::vec3 pos;
            ss >> pos.x >> pos.y >> pos.z;
            tempPositions.push_back(pos);
        }
        else if (prefix == "vn") {
            glm::vec3 norm;
            ss >> norm.x >> norm.y >> norm.z;
            tempNormals.push_back(norm);
        }
        else if (prefix == "vt") {
            glm::vec2 uv;
            ss >> uv.x >> uv.y;
            tempTexCoords.push_back(uv);
        }
        else if (prefix == "f") {
            std::string vertexStr;
            for (int i = 0; i < 3; ++i) {
                ss >> vertexStr;
                int posIdx = 0, uvIdx = 0, normIdx = 0;
                
                if (sscanf(vertexStr.c_str(), "%d/%d/%d", &posIdx, &uvIdx, &normIdx) == 3) {}
                else if (sscanf(vertexStr.c_str(), "%d//%d", &posIdx, &normIdx) == 2) {
                    uvIdx = 0;
                }
                else if (sscanf(vertexStr.c_str(), "%d", &posIdx) == 1) {
                    uvIdx = 0; normIdx = 0;
                }

                Vertex vertex{};
                if (posIdx > 0 && posIdx <= static_cast<int>(tempPositions.size())) {
                    vertex.position = tempPositions[posIdx - 1];
                }
                if (normIdx > 0 && normIdx <= static_cast<int>(tempNormals.size())) {
                    vertex.normal = tempNormals[normIdx - 1];
                }
                if (uvIdx > 0 && uvIdx <= static_cast<int>(tempTexCoords.size())) {
                    vertex.texCoord = tempTexCoords[uvIdx - 1];
                }

                tempVertexes.push_back(vertex);
                tempIndices.push_back(static_cast<uint32_t>(tempVertexes.size() - 1));
            }
        } // остальные варианты игнорируем (пока?)
    }

    vertices = std::move(tempVertexes);
    indices = std::move(tempIndices);
}

void ObjModel::accept(ObjectVisitor &visitor) {
    visitor.visit(*this);
}
