#include "ModelLoader.h"
#include <iostream>



ModelLoader* ModelLoader::_instance;
unsigned int ModelLoader::getTexture(const char* path) {
    if (GlobalTexturesMap.count(std::string(path)) > 0) {
        return GlobalTexturesMap[std::string(path)];
    }
    else {
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load and generate the texture
        int width, height, nrChannels;

        stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
        if (data)
        {
            GLuint format;
            switch (nrChannels) {
            case 3:
                format = GL_RGB;
                break;
            case 2:
                break;
            case 1:
                format = GL_RED;
            }
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
        //glBindTexture(GL_TEXTURE_2D, 0);
        GlobalTexturesMap.emplace(std::pair<std::string, unsigned int>(std::string(path), texture));
        return texture;
    }
    
}
bool ModelLoader::loadEmbeddedModel(const rapidjson::Value& model,Model& modelref) {
    using namespace rapidjson;
    if (!model.HasMember("key") || !model["key"].IsString()) {
        std::cerr << "an embedded model doesn't have member, key, or it's not a string" << std::endl;
        return false;
    }
    if (!model.HasMember("positions") || !model["positions"].IsArray()) {
        std::cerr << "an embedded model doesn't have member, positions, or it's not an array" << std::endl;
        return false;
    }
    if (!model.HasMember("indices") || !model["indices"].IsArray()) {
        std::cerr << "an embedded model doesn't have member, indices, or it's not an array" << std::endl;
        return false;
    }
    if (!model.HasMember("uvs") || !model["uvs"].IsArray()) {
        std::cerr << "an embedded model doesn't have member, uvs, or it's not an array" << std::endl;
        return false;
    }
    auto jsonarr_uvs = model["uvs"].GetArray();
    auto jsonarr_pos = model["positions"].GetArray();
    auto jsonarr_ind = model["indices"].GetArray();
    Mesh mesh;
    for (int i = 0; i < jsonarr_pos.Size()/3; i++) {
        Vertex v;
        v.x = jsonarr_pos[i * 3].GetFloat();
        v.y = jsonarr_pos[i * 3 + 1].GetFloat();
        v.z = jsonarr_pos[i * 3 + 2].GetFloat();
        mesh.vertices.push_back(v);
    }
    for (int i = 0; i < jsonarr_uvs.Size()/2; i++) {
        Vertex& v = mesh.vertices[i];
        v.u = jsonarr_uvs[i * 2].GetFloat();
        v.v = jsonarr_uvs[i * 2 + 1].GetFloat();
    }
    for (int i = 0; i < jsonarr_ind.Size(); i++) {
        mesh.faceIndices.push_back(jsonarr_ind[i].GetInt());
    }
    mesh.materialIndex = 0;
    mesh.hasuvs = true;
    modelref.pushMesh(mesh);
    Material mat;
    if (model.HasMember("texture") && model["texture"].IsString()) {
        Texture t;
        t.id = getTexture(model["texture"].GetString());
        t.type = TextureType::AMBIENT;
        mat.textures.push_back(t);
    }
    if (model.HasMember("ambient")) {
        mat.ambient.x = model["ambient"]["r"].GetFloat();
        mat.ambient.y = model["ambient"]["g"].GetFloat();
        mat.ambient.z = model["ambient"]["b"].GetFloat();
    }
    if (model.HasMember("diffuse")) {
        mat.diffuse.x = model["diffuse"]["r"].GetFloat();
        mat.diffuse.y = model["diffuse"]["g"].GetFloat();
        mat.diffuse.z = model["diffuse"]["b"].GetFloat();
    }
    if (model.HasMember("specular")) {
        mat.specular.x = model["specular"]["r"].GetFloat();
        mat.specular.y = model["specular"]["g"].GetFloat();
        mat.specular.z = model["specular"]["b"].GetFloat();
    }
    if (model.HasMember("shininess")) {
        mat.shininess = model["shininess"].GetFloat();
    }
    if (model.HasMember("shininess_strength")) {
        mat.shininess_strength = model["shininess_strength"].GetFloat();
    }
    modelref.pushMaterial(mat);
    return true;
}
bool ModelLoader::loadGlobalModels()
{
    using namespace rapidjson;
    Document doc;
    loadJSONFile(doc, "data/models.json");
    if (!doc.HasMember("models") || !doc["models"].IsArray()) {
        std::cerr << "no member: models in models.json or models isn't an array" << std::endl;
        return false;
    }
    const Value& models = doc["models"];
    GlobalModels = new Model[models.Size()];
    for (SizeType i = 0; i < models.Size(); i++)
    {
        if (!models[i].HasMember("key") || !models[i]["key"].IsString()) {
            std::cerr << "one of models doesn't have member, key, or it's not a string" << std::endl;
            return false;
        }
        std::string key = static_cast<std::string>(models[i]["key"].GetString());
        if (!models[i].HasMember("path") || !models[i]["path"].IsString()) {
            if (!loadEmbeddedModel(models[i],GlobalModels[i])) {
                return false;
            }
        }
        else {
            const char* path = models[i]["path"].GetString();           
            if (!LoadModel(path, GlobalModels[i])) {
                std::cerr << "problem loading " << path << std::endl;
                return false;
            }
        }
        GlobalModels[i].genBuffers();
        GlobalModelsMap[key] = &GlobalModels[i];
    }
    return true;
}
Model * ModelLoader::getModel(std::string key)
{
    return GlobalModelsMap[key];
}
void ModelLoader::SetMaterials(Model& modelref, const aiScene* scene) {
    for (size_t i = 0; i < scene->mNumMaterials; i++) {
        Material material;
        aiMaterial* m = scene->mMaterials[i];
        std::cout << "material " << i << "\n" << std::endl;
        aiColor3D amb, diff, spec;
        float shininess, shininess_strength;
        if (m->Get(AI_MATKEY_COLOR_AMBIENT, amb) == aiReturn_SUCCESS) {
            material.ambient = glm::vec3(amb.r, amb.g, amb.b);
            std::cout << "ambient colour r:" << amb.r << ", g:" << amb.g << " b:" << amb.b << std::endl;
            std::cout << std::endl;
        }
        else {
            std::cout << "no ambient colour in material " << std::endl;
            std::cout << std::endl;
        }
        if (m->Get(AI_MATKEY_COLOR_DIFFUSE, diff) == aiReturn_SUCCESS) {
            material.diffuse = glm::vec3(diff.r, diff.g, diff.b);
            std::cout << "diffuse colour r:" << diff.r << ", g:" << diff.g << " b:" << diff.b << std::endl;
            std::cout << std::endl;
        }
        else {
            std::cout << "no diff colour in material " << std::endl;
            std::cout << std::endl;
        }
        if (m->Get(AI_MATKEY_COLOR_SPECULAR, spec) == aiReturn_SUCCESS) {
            material.specular = glm::vec3(spec.r, spec.g, spec.b);
            std::cout << "specular colour r:" << spec.r << ", g:" << spec.g << " b:" << spec.b << std::endl;
            std::cout << std::endl;
        }
        else {
            std::cout << "no specular colour in material " << std::endl;
            std::cout << std::endl;
        }

        if (m->Get(AI_MATKEY_SHININESS, shininess) == aiReturn_SUCCESS) {
            std::cout << "shininess " << shininess << std::endl;
            std::cout << std::endl;
            material.shininess = shininess;
        }
        else {
            std::cout << "no shininess in material " << std::endl;
            std::cout << std::endl;
        }

        if (m->Get(AI_MATKEY_SHININESS_STRENGTH, shininess_strength) == aiReturn_SUCCESS) {
            std::cout << "shininess strength " << shininess_strength << std::endl;
            std::cout << std::endl;
            material.shininess_strength = shininess_strength;
        }
        else {
            std::cout << "no shininess strength in material " << std::endl;
            std::cout << std::endl;
        }
        aiString amb_path;
        modelref.hasTextures = false;
        if (m->Get(AI_MATKEY_TEXTURE(aiTextureType_AMBIENT, 0), amb_path) == aiReturn_SUCCESS) {
            std::cout << "ambient texture: " << amb_path.C_Str() << std::endl;
            Texture t;
            t.type = TextureType::AMBIENT;
            t.id = getTexture(amb_path.C_Str());
            material.textures.push_back(t);
            modelref.hasTextures = true;
        }
        aiString diff_path;
        if (m->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), diff_path) == aiReturn_SUCCESS) {
            std::cout << "diffuse texture: " << diff_path.C_Str() << std::endl;
            Texture t;
            t.type = TextureType::DIFFUSE;
            t.id = getTexture(diff_path.C_Str());
            material.textures.push_back(t);
            modelref.hasTextures = true;
        }
        aiString spec_path;
        if (m->Get(AI_MATKEY_TEXTURE(aiTextureType_SPECULAR, 0), spec_path) == aiReturn_SUCCESS) {
            std::cout << "specular texture: " << spec_path.C_Str() << std::endl;
            Texture t;
            t.type = TextureType::SPECULAR;
            t.id = getTexture(spec_path.C_Str());
            material.textures.push_back(t);
            modelref.hasTextures = true;
        }
        aiString norm_path;
        if (m->Get(AI_MATKEY_TEXTURE(aiTextureType_NORMALS, 0), norm_path) == aiReturn_SUCCESS) {
            std::cout << "normal map texture: " << norm_path.C_Str() << std::endl;
            Texture t;
            t.type = TextureType::NORMALMAP;
            t.id = getTexture(norm_path.C_Str());
            material.textures.push_back(t);
            modelref.hasTextures = true;
        }
        modelref.pushMaterial(material);
    }
}
void ModelLoader::SetMeshes(Model& modelref, const aiScene* scene) {
    for (size_t i = 0; i < scene->mNumMeshes; i++)
    {
        Mesh mesh;

        aiMesh* m = scene->mMeshes[i];
        mesh.materialIndex = m->mMaterialIndex;
        mesh.hasnormals = m->HasNormals();
        mesh.hasuvs = false;
        if (m->HasPositions()) {
            int num_UVs = m->GetNumUVChannels();
            if (num_UVs > 0) {
                mesh.hasuvs = true;
            }
            for (size_t j = 0; j < m->mNumVertices; j++) {
                Vertex v;
                v.x = m->mVertices[j].x;
                v.y = m->mVertices[j].y;
                v.z = m->mVertices[j].z;

                if (mesh.hasnormals) {
                    v.normx = m->mNormals[j].x;
                    v.normy = m->mNormals[j].y;
                    v.normz = m->mNormals[j].z;
                }
                if (m->HasTextureCoords(0)) {
                    aiVector3D uv = m->mTextureCoords[0][j];
                    v.u = uv.x;
                    v.v = uv.y;
                }

                mesh.vertices.push_back(v);
            }
        }
        if (m->HasFaces()) {
            for (size_t j = 0; j < m->mNumFaces; j++) {
                aiFace face = m->mFaces[j];
                mesh.faceIndices.push_back(face.mIndices[0]);
                mesh.faceIndices.push_back(face.mIndices[1]);
                mesh.faceIndices.push_back(face.mIndices[2]);
            }
        }
        modelref.pushMesh(mesh);
    }
}
bool ModelLoader::LoadModel(const char * path, Model& modelref)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate);
    if (!scene){
        return false;
    }
    SetMaterials(modelref, scene);
    SetMeshes(modelref, scene);
    std::cout << "--------------* processing nodes... *--------------" << std::endl;
    aiNode* root = scene->mRootNode;
    printNodes(root, -1);
    //processNode(root, -1, modelref, aiMatrix4x4());
    modelref.drawnode = new ModelDrawNode();
    processNode(root, modelref.drawnode, glm::mat4(1.0), modelref);
    return true;
}

void ModelLoader::processNode(aiNode* node, ModelDrawNode* targetParent, glm::mat4 accTransform, Model& modelref) {
    ModelDrawNode* parent;
    glm::mat4 transform;

    // if node has meshes, create a new scene object for it
    if (node->mNumMeshes > 0) {
        ModelDrawNode* newObject = new ModelDrawNode();
        targetParent->myChildren.push_back(newObject);
        // to do - accumulate transforms properly - pretty sure this way isn't right. need to collect in vector and reverse it i think
        newObject->myTransform = transform = convertMatrix(node->mTransformation) * accTransform;
        // copy the meshes
        for (size_t i = 0; i < node->mNumMeshes; i++)
        {
            int meshindex = node->mMeshes[i];
            newObject->myMeshes.push_back(&modelref.meshes[meshindex]);
            
        }

        // the new object is the parent for all child nodes
        parent = newObject;
        transform = glm::mat4(1.0);
    }
    else {
        // if no meshes, skip the node, but keep its transformation
        parent = targetParent;
        transform = convertMatrix(node->mTransformation) * accTransform;
    }

    // continue for all child nodes
    for (size_t i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], parent, transform,modelref);
    }
}
void ModelLoader::processNode(aiNode* node, int rdepth, Model& modelref, aiMatrix4x4 matrix) {
    // this one transforms the vertices of the meshes on cpu, once as they are loaded.
    // The other one recreates the heirachy aiNode as a tree of ModelDrawNodes
    // which is drawn recursively with the vertices being transformed in the shader
    rdepth++;
    matrix *= node->mTransformation;
    if (node->mNumMeshes > 0) {
        for (size_t i = 0; i < node->mNumMeshes; i++) {
            int meshindex = node->mMeshes[i];
            
            for (size_t j = 0; j < modelref.meshes[meshindex].vertices.size(); j++) {
                Vertex& vert = modelref.meshes[meshindex].vertices[j];

                aiVector3D pos(vert.x, vert.y, vert.z);
                pos *= matrix;
                vert.x = pos.x;
                vert.y = pos.y;
                vert.z = pos.z;

                aiVector3D norm(vert.normx, vert.normy, vert.normz);
                aiVector3D translation, scale;
                aiQuaternion rotation;
                matrix.Decompose(scale, rotation, translation);
                aiMatrix4x4 rotation_matrix = aiMatrix4x4(rotation.GetMatrix());
                aiMatrix4x4 scaling_matrix;
                scaling_matrix = scaling_matrix.Inverse();
                scaling_matrix = scaling_matrix.Transpose();
                aiMatrix4x4::Scaling(scale, scaling_matrix);
                norm *= (rotation_matrix * scaling_matrix);
                vert.normx = norm.x;
                vert.normy = norm.y;
                vert.normz = norm.z;
            }
        }
    }
    
    if (node->mNumChildren > 0) {
        for (size_t i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], rdepth, modelref, matrix);
        }
    }
    std::cout << std::endl;

}


inline void ModelLoader::indent(int num) {
    for (int i = 0; i < num; i++) {
        std::cout << "\t";
    }
}
void ModelLoader::printNodes(aiNode* node, int rdepth) {
    std::cout << std::endl;
    rdepth++;
    indent(rdepth);
    std::cout << "-------------> node " << node->mName.data << " <-------------" << std::endl;
    indent(rdepth);
    std::cout << "number of meshes: " << node->mNumMeshes << std::endl;

    if (node->mNumMeshes > 0) {
        indent(rdepth);
        std::cout << "MESHES:" << std::endl;
        for (size_t i = 0; i < node->mNumMeshes; i++) {
            indent(rdepth);
            int meshindex = node->mMeshes[i];
            std::cout << "index " << meshindex << std::endl;
        }
    }
    indent(rdepth);
    std::cout << "number of children: " << node->mNumChildren << std::endl;
    
    if (node->mTransformation.IsIdentity()) {
        indent(rdepth);
        std::cout << "transform matrix is identity" << std::endl;
    }
    else {
        indent(rdepth);
        std::cout << "transform matrix is not identity" << std::endl;
    }
    if (node->mNumChildren > 0) {
        indent(rdepth);
        std::cout << "{" << std::endl;
        for (size_t i = 0; i < node->mNumChildren; i++)
        {
            printNodes(node->mChildren[i], rdepth);
        }
        indent(rdepth);
        std::cout << "}" << std::endl;
    }
    std::cout << std::endl;
}

inline glm::mat4 ModelLoader::convertMatrix(const aiMatrix4x4 &aiMat)
{
    return {
    aiMat.a1, aiMat.b1, aiMat.c1, aiMat.d1,
    aiMat.a2, aiMat.b2, aiMat.c2, aiMat.d2,
    aiMat.a3, aiMat.b3, aiMat.c3, aiMat.d3,
    aiMat.a4, aiMat.b4, aiMat.c4, aiMat.d4
    };
}
