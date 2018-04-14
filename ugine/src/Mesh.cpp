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

vector<Vertex> dataToVertices(vector<float> coords, vector<float> texcoords) {
	vector<Vertex> vertices;
	auto coordsIt = coords.begin();
	auto texcoordsIt = texcoords.begin();
	bool hasTexCoords = texcoords.size() != 0;
	while (coordsIt != coords.end()) {
		float x = *(coordsIt++);
		float y = *(coordsIt++);
		float z = *(coordsIt++);
		glm::vec3 position = {x, y, z};
		glm::vec2 texCoords;
		if (hasTexCoords) {
			float u = *(texcoordsIt++);
			float v = *(texcoordsIt++);
			texCoords = {u, v};
		}
		vertices.push_back({position, texCoords});
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


shared_ptr<Mesh> Mesh::load(const char* filename, 
							const shared_ptr<Shader>& shader) {
	using namespace pugi;

	shared_ptr<Mesh> mesh = make_shared<Mesh>();
	xml_document doc;
	xml_parse_result result = doc.load_file(filename);
	if (result) {
		xml_node meshNode = doc.child("mesh");
		xml_node buffersNode = meshNode.child("buffers");

		for (xml_node bufferNode = buffersNode.child("buffer"); bufferNode; bufferNode = bufferNode.next_sibling("buffer")) {
			xml_node materialNode = bufferNode.child("material");
			string textureStr = materialNode.child("texture").text().as_string();
			vector<float> coords = splitStr<float>(bufferNode.child("coords").text().as_string(), ',');
			vector<GLushort> indices = splitStr<GLushort>(bufferNode.child("indices").text().as_string(), ',');
			xml_node texcoordsNode = bufferNode.child("texcoords");
			vector<float> texcoords;
			if (texcoordsNode) {
				texcoords = splitStr<float>(texcoordsNode.text().as_string(), ',');
			}
			vector<Vertex> vertices = dataToVertices(coords, texcoords);

			shared_ptr<Buffer> buffer = make_shared<Buffer>(vertices.data(), vertices.size(), 
															indices.data(), indices.size());
			
			textureStr = extractPath(filename) + textureStr;
			mesh->addBuffer(buffer, Material(Texture::load(textureStr.c_str()), shader));
		}
		return mesh;
	}

	return nullptr;
}