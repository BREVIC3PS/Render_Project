#pragma once
#include "Mesh/Mesh.h"
#include "shader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

class Model
{
public:

    /*  Functions   */
    Model(const std::string & path, bool gamma = false) : gammaCorrection(gamma)
    {
        loadModel(path);
    }
    void Draw(Shader &shader);
    void PrintNumbers();
    std::vector<Mesh> meshes;
    std::vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
private:
    /*  Model Data  */
    std::string directory;
    bool gammaCorrection;
    /*  Functions   */
    void loadModel(const std::string &path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    void ProcessMaterials(aiMesh* mesh, const aiScene* scene, std::vector<Texture>& textures);
    void ProcessFaces(aiMesh* mesh, std::vector<unsigned int>& indices);
    void ProcessVertices(aiMesh* mesh, std::vector<Vertex>& vertices);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
        const std::string& typeName);
};