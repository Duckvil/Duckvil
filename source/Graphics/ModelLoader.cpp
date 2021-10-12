#include "Graphics/ModelLoader.h"

namespace Duckvil { namespace Graphics {

    ModelLoader::ModelLoader(const char* _sFilename)
    {
        m_pScene = m_importer.ReadFile(_sFilename,
            aiProcess_Triangulate | aiProcess_FlipUVs
        );

        for(uint32_t i = 0; i < m_pScene->mNumMeshes; i++)
        {
            aiMesh *mesh = m_pScene->mMeshes[i];

            for(uint32_t j = 0; j < mesh->mNumVertices; j++)
            {
                glm::vec3 _vector;
                glm::vec2 _texCoord;

                _vector.x = mesh->mVertices[j].x;
                _vector.y = mesh->mVertices[j].y;
                _vector.z = mesh->mVertices[j].z;

                if(mesh->mTextureCoords[0])
                {
                    _texCoord.x = mesh->mTextureCoords[0][j].x;
                    _texCoord.y = mesh->mTextureCoords[0][j].y;
                }
                else
                {
                    _texCoord = glm::vec2(0, 0);
                }

                m_raw.m_aVertices.push_back(_vector);
                m_raw.m_aTexCoords.push_back(_texCoord);
            }

            for(uint32_t j = 0; j < mesh->mNumFaces; j++)
            {
                aiFace _face = mesh->mFaces[j];

                for(uint32_t k = 0; k < _face.mNumIndices; k++)
                {
                    m_raw.m_aIndices.push_back(_face.mIndices[k]);
                }
            }
        }
    }

    ModelLoader::~ModelLoader()
    {

    }

}}