#pragma once

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <vector>

#include "glm/glm.hpp"

namespace Duckvil { namespace Graphics {

    class ModelLoader
    {
    public:
        struct Raw
        {
            std::vector<glm::vec3> m_aVertices;
            std::vector<glm::vec2> m_aTexCoords;
            std::vector<uint32_t> m_aIndices;
        };

    private:
        Assimp::Importer m_importer;
        const aiScene* m_pScene;

        Raw m_raw;

    public:
        ModelLoader(const char* _sFilename);
        ~ModelLoader();

        inline const ModelLoader::Raw& GetRaw() const { return m_raw; }
    };

}}