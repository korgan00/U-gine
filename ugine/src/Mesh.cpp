#include "Mesh.h"
#include "State.h"
#include "pugixml.hpp"
using namespace std;

Mesh::Mesh() {
}

void Mesh::addBuffer(const shared_ptr<Buffer>& buffer,
					 const Material& material) {
	_bufferList.push_back(pair<std::shared_ptr<Buffer>, Material>(buffer, material));
}


size_t Mesh::getNumBuffers() const {
	return _bufferList.size();
}
const shared_ptr<Buffer>& Mesh::getBuffer(size_t index) const {
	return _bufferList[index].first;
}
shared_ptr<Buffer>& Mesh::getBuffer(size_t index) {
	return _bufferList[index].first;
}
const Material& Mesh::getMaterial(size_t index) const {
    return _bufferList[index].second;
}
Material& Mesh::getMaterial(size_t index) {
    return _bufferList[index].second;
}

void Mesh::draw() {
	size_t amount = _bufferList.size();
    //glm::mat4 mvp = State::projectionMatrix * State::viewMatrix * State::modelMatrix;

	for (size_t i = 0; i < amount; ++i) {
        _bufferList[i].second.prepare();
		_bufferList[i].first->draw(*(_bufferList[i].second.getShader()));
	}
}

template <typename T>
vector<T> splitStr(const std::string& str, char delim) {
	vector<T> elems;
	stringstream sstream(str);
	string item;
	if (str != "") {
		while (std::getline(sstream, item, delim)) {
			istringstream istream(item);
			T elem;
			istream >> elem;
			elems.push_back(elem);
		}
	}
	return elems;
}

vector<Vertex> dataToVertices(vector<float> coords, vector<float> texcoords, vector<float> normals, 
                              vector<float> tangents, vector<float> boneIndices, vector<float> boneWeights) {
	vector<Vertex> vertices;

	auto coordsIt = coords.begin();
    auto texcoordsIt = texcoords.begin();
    auto normalsIt = normals.begin();
    auto tangentsIt = tangents.begin();
    auto boneIndicesIt = boneIndices.begin();
    auto boneWeightsIt = boneWeights.begin();

    bool hasTexCoords = texcoords.size() != 0;
    bool hasNormals = normals.size() != 0;
    bool hasTangents = tangents.size() != 0;
    bool hasBoneIndices = boneIndices.size() != 0;
    bool hasBoneWeights = boneWeights.size() != 0;

	while (coordsIt != coords.end()) {
		float x = *(coordsIt++);
		float y = *(coordsIt++);
		float z = *(coordsIt++);
		glm::vec3 position = {x, y, z};

        glm::vec2 texCoords;
        if (hasTexCoords) {
            float u = *(texcoordsIt++);
            float v = *(texcoordsIt++);
            texCoords = { u, v };
        }

        glm::vec3 normal;
        if (hasNormals) {
            float x = *(normalsIt++);
            float y = *(normalsIt++);
            float z = *(normalsIt++);
            normal = { x, y, z };
        }

        glm::vec3 tangent;
        if (hasTangents) {
            float x = *(tangentsIt++);
            float y = *(tangentsIt++);
            float z = *(tangentsIt++);
            tangent = { x, y, z };
        }

        glm::vec4 boneIdxs;
        if (hasBoneIndices) {
            float x = *(boneIndicesIt++);
            float y = *(boneIndicesIt++);
            float z = *(boneIndicesIt++);
            float w = *(boneIndicesIt++);
            boneIdxs = { x, y, z, w };
        }

        glm::vec4 boneW;
        if (hasBoneWeights) {
            float x = *(boneWeightsIt++);
            float y = *(boneWeightsIt++);
            float z = *(boneWeightsIt++);
            float w = *(boneWeightsIt++);
            boneW = { x, y, z, w };
        }
		vertices.push_back({position, texCoords, normal, tangent, boneIdxs, boneW});
	}

	return vertices;
}
inline std::string extractPath(std::string filename) {
	while (filename.find('\\') != std::string::npos)
		filename.replace(filename.find('\\'), 1, 1, '/');
	size_t pos = filename.rfind('/');
	if (pos == std::string::npos) return "";
	filename = filename.substr(0, pos);
	if (filename.size() > 0) filename += '/';
	return filename;
}

std::shared_ptr<Texture> textureFromNode(pugi::xml_node textureNode, const string& path) {
    using namespace pugi;
    string textureStr = textureNode.text().as_string();
    vector<string> textureList;
    textureList = splitStr<string>(textureStr, ',');
    std::shared_ptr<Texture> texture;
    if (textureList.size() == 1) {
        texture = Texture::load((path + textureList[0]).c_str());
    } else if (textureList.size() == 6) {
        texture = Texture::load((path + textureList[0]).c_str(), (path + textureList[1]).c_str(),
                                (path + textureList[2]).c_str(), (path + textureList[3]).c_str(),
                                (path + textureList[4]).c_str(), (path + textureList[5]).c_str());
    }
    return texture;
}

Material nodeToMaterial(pugi::xml_node materialNode, const char* filename, const shared_ptr<Shader>& shader) {
    using namespace pugi;
    string path = extractPath(filename);

    Material m(textureFromNode(materialNode.child("texture"), path), shader);
    m.setNormalTexture(textureFromNode(materialNode.child("normal_texture"), path));
    m.setReflectionTexture(textureFromNode(materialNode.child("reflect_texture"), path));
    m.setRefractionTexture(textureFromNode(materialNode.child("refract_texture"), path));

    xml_node colorNode = materialNode.child("color");
    if (colorNode) {
        vector<float> colorComponents;
        colorComponents = splitStr<float>(colorNode.text().as_string(), ',');
        m.setColor(glm::vec4(colorComponents[0], colorComponents[1], colorComponents[2], 1.0f));
    }
    xml_node shininessNode = materialNode.child("shininess");
    if (shininessNode) {
        m.setShininess(static_cast<GLubyte>(shininessNode.text().as_uint()));
    }
    xml_node refractCoefNode = materialNode.child("refract_coef");
    if (refractCoefNode) {
        m.setRefractionCoef(static_cast<float>(refractCoefNode.text().as_float()));
    }
    xml_node cullingNode = materialNode.child("culling");
    if (cullingNode) {
        m.setCulling(static_cast<bool>(cullingNode.text().as_bool()));
    }
    xml_node depthwriteNode = materialNode.child("depthwrite");
    if (depthwriteNode) {
        m.setDepthWrite(static_cast<bool>(depthwriteNode.text().as_bool()));
    }

    xml_node blendNode = materialNode.child("blend");
    if (blendNode) {
        if (blendNode.text().as_string() == "alpha") {
            m.setBlendMode(BlendMode::ALPHA);
        } else if (blendNode.text().as_string() == "add") {
            m.setBlendMode(BlendMode::ADD);
        } else if (blendNode.text().as_string() == "mul") {
            m.setBlendMode(BlendMode::MUL);
        }
    }
    return m;
}

void nodeToBuffers(shared_ptr<Mesh> mesh, pugi::xml_node buffersNode, 
                   const char* filename, const shared_ptr<Shader>& shader) {
    using namespace pugi;

    for (xml_node bufferNode = buffersNode.child("buffer"); bufferNode; bufferNode = bufferNode.next_sibling("buffer")) {
        vector<float> coords = splitStr<float>(bufferNode.child("coords").text().as_string(), ',');
        vector<GLushort> indices = splitStr<GLushort>(bufferNode.child("indices").text().as_string(), ',');
        xml_node texcoordsNode = bufferNode.child("texcoords");
        vector<float> texcoords;
        if (texcoordsNode) {
            texcoords = splitStr<float>(texcoordsNode.text().as_string(), ',');
        }
        xml_node normalsNode = bufferNode.child("normals");
        vector<float> normals;
        if (normalsNode) {
            normals = splitStr<float>(normalsNode.text().as_string(), ',');
        }
        xml_node tangentsNode = bufferNode.child("tangents");
        vector<float> tangents;
        if (tangentsNode) {
            tangents = splitStr<float>(tangentsNode.text().as_string(), ',');
        }

        xml_node boneIndicesNode = bufferNode.child("bone_indices");
        vector<float> boneIndices;
        if (boneIndicesNode) {
            boneIndices = splitStr<float>(boneIndicesNode.text().as_string(), ',');
        }
        xml_node boneWeightsNode = bufferNode.child("bone_weights");
        vector<float> boneWeights;
        if (boneWeightsNode) {
            boneWeights = splitStr<float>(boneWeightsNode.text().as_string(), ',');
        }

        vector<Vertex> vertices = dataToVertices(coords, texcoords, normals, tangents, boneIndices, boneWeights);

        shared_ptr<Buffer> buffer = make_shared<Buffer>(vertices.data(), vertices.size(),
            indices.data(), indices.size());

        mesh->addBuffer(buffer, nodeToMaterial(bufferNode.child("material"), filename, shader));
    }
}


void nodeToBones(shared_ptr<Mesh> mesh, pugi::xml_node bonesNode,
                 const char* filename, const shared_ptr<Shader>& shader) {
    using namespace pugi;

    for (xml_node boneNode = bonesNode.child("bone"); boneNode; boneNode = boneNode.next_sibling("bone")) {
        
        xml_node nameNode = boneNode.child("name");
        xml_node parentNode = boneNode.child("parent");
        int parentNodeIdx = -1;
        if (parentNode) {
            parentNodeIdx = mesh->getBoneIndex(parentNode.text().as_string());
        }
        Bone currentBone(nameNode.text().as_string(), parentNodeIdx);

        xml_node invPoseNode = boneNode.child("inv_pose");
        vector<float> invPoseValues = splitStr<float>(invPoseNode.text().as_string(), ',');
        currentBone.setInvPoseMatrix(glm::mat4( invPoseValues[0],  invPoseValues[1],  invPoseValues[2],  invPoseValues[3],
                                                invPoseValues[4],  invPoseValues[5],  invPoseValues[6],  invPoseValues[7],
                                                invPoseValues[8],  invPoseValues[9],  invPoseValues[10], invPoseValues[11],
                                                invPoseValues[12], invPoseValues[13], invPoseValues[14], invPoseValues[15]));

        xml_node positionsNode = boneNode.child("positions");
        vector<float> positionValues = splitStr<float>(positionsNode.text().as_string(), ',');
        for (int i = 0; i < positionValues.size(); i +=4) {
            currentBone.addPosition(static_cast<uint16_t>(positionValues[i]), { positionValues[i + 1], positionValues[i + 2], positionValues[i + 3] });
        }

        xml_node rotationsNode = boneNode.child("rotations");
        vector<float> rotationValues = splitStr<float>(rotationsNode.text().as_string(), ',');
        for (int i = 0; i < rotationValues.size(); i += 5) {
            currentBone.addRotation(static_cast<uint16_t>(rotationValues[i]), glm::quat(rotationValues[i + 1], rotationValues[i + 2], rotationValues[i + 3], rotationValues[i + 4]));
        }

        xml_node scalesNode = boneNode.child("scales");
        vector<float> scaleValues = splitStr<float>(scalesNode.text().as_string(), ',');
        for (int i = 0; i < scaleValues.size(); i += 4) {
            currentBone.addScale(static_cast<uint16_t>(scaleValues[i]), { scaleValues[i + 1], scaleValues[i + 2], scaleValues[i + 3] });
        }

        mesh->addBone(currentBone);
    }
}

shared_ptr<Mesh> Mesh::load(const char* filename, 
							const shared_ptr<Shader>& shader) {
	using namespace pugi;

	shared_ptr<Mesh> mesh = make_shared<Mesh>();
	xml_document doc;
	xml_parse_result result = doc.load_file(filename);
	if (result) {
		xml_node meshNode = doc.child("mesh");

        xml_node buffersNode = meshNode.child("buffers");
        nodeToBuffers(mesh, buffersNode, filename, shader);

        xml_node lastFrameNode = meshNode.child("last_frame");
        mesh->setLastFrame(static_cast<GLushort>(lastFrameNode.text().as_uint()));

        xml_node bonesNode = meshNode.child("bones");
        nodeToBones(mesh, bonesNode, filename, shader);

		return mesh;
	}

	return nullptr;
}

void Mesh::addBone(const Bone& bone) {
    _boneList.push_back(bone);
}
const std::vector<Bone>& Mesh::getBones() const {
    return _boneList;
}
GLushort Mesh::getLastFrame() const {
    return _lastFrame;
}
void Mesh::setLastFrame(GLushort lastFrame) {
    _lastFrame = lastFrame;
}
int Mesh::getBoneIndex(const char* name) const {
    for (int i = 0; i < _boneList.size(); i++) {
        if (strcmp(_boneList[i].getName(), name) == 0) {
            return i;
        }
    }
    return -1;
}