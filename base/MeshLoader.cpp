#include <iostream>
using std::cout;
using std::endl;

#include "base/MeshLoader.h"
#include "base/Director.h"
#include "renderer/Renderer.h"
#include "node/Mesh.h"

using namespace tinyxml2;

#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif

XMLError MeshLoader::loadMesh(const std::string& fileName, Mesh& dst)
{
	// implement with xml parser
	tinyxml2::XMLDocument xmlDoc;
	tinyxml2::XMLError result = xmlDoc.LoadFile(fileName.c_str());
	XMLCheckResult(result);

	XMLNode* root = xmlDoc.FirstChild();
	if (root == nullptr) return XML_ERROR_FILE_READ_ERROR;

	XMLElement* element = root->FirstChildElement("Name");
	if (element == nullptr) return XML_ERROR_PARSING_ELEMENT;

	// parse name
	std::string name = element->GetText();

	element = root->FirstChildElement("PrimitiveType");
	if (element == nullptr) return XML_ERROR_PARSING_ELEMENT;

	// parse primitive type
	std::string primitiveType = element->GetText();

	element = root->FirstChildElement("VertexType");
	if (element == nullptr) return XML_ERROR_PARSING_ELEMENT;

	// parse vertex type
	std::string vertexType = element->GetText();

	// parse vertices
	element = root->FirstChildElement("Vertices");
	if (element == nullptr) return XML_ERROR_PARSING_ELEMENT;

	XMLElement* listElement = element->FirstChildElement("List");
	int numberOfVertices = std::atoi(listElement->Attribute("NumberOfVertices"));

	int i = 0;
	VertexBase* vertices = nullptr;

	if (vertexType == "V3F_C4F")
	{
		auto v = new V3F_C4F[numberOfVertices];

		i = 0;
		listElement = listElement->FirstChildElement("Vertex");
		while (listElement != nullptr)
		{
			v[i].position.x = std::atof(listElement->Attribute("x"));
			v[i].position.y = std::atof(listElement->Attribute("y"));
			v[i].position.z = std::atof(listElement->Attribute("z"));

			v[i].color.r = std::atof(listElement->Attribute("r"));
			v[i].color.g = std::atof(listElement->Attribute("g"));
			v[i].color.b = std::atof(listElement->Attribute("b"));
			v[i].color.a = std::atof(listElement->Attribute("a"));

			++i;
			listElement = listElement->NextSiblingElement("Vertex");
		}

		vertices = new V3F_C4F[i];
		memcpy(vertices, v, i * sizeof(V3F_C4F));
		delete[] v;
	}
	else if (vertexType == "V3F_T2F")
	{
		auto v = new V3F_T2F[numberOfVertices];

		i = 0;
		listElement = listElement->FirstChildElement("Vertex");
		while (listElement != nullptr)
		{
			v[i].position.x = std::atof(listElement->Attribute("x"));
			v[i].position.y = std::atof(listElement->Attribute("y"));
			v[i].position.z = std::atof(listElement->Attribute("z"));

			v[i].texCoord.u = std::atof(listElement->Attribute("u"));
			v[i].texCoord.v = std::atof(listElement->Attribute("v"));

			++i;
			listElement = listElement->NextSiblingElement("Vertex");
		}

		vertices = new V3F_T2F[i];
		memcpy(vertices, v, i * sizeof(V3F_T2F));
		delete[] v;
	}

	// parse indices
	element = root->FirstChildElement("Indices");
	if (element == nullptr) return XML_ERROR_PARSING_ELEMENT;

	listElement = element->FirstChildElement("List");
	int numberOfIndices = std::atoi(listElement->Attribute("NumberOfIndices"));
	IndexBuffer* indices = new IndexBuffer[numberOfIndices];

	i = 0;
	listElement = listElement->FirstChildElement("Index");
	while (listElement != nullptr)
	{
		int index;
		listElement->QueryIntText(&index);
		indices[i] = index;

		++i;
		listElement = listElement->NextSiblingElement("Index");
	}

	dst._name = name;
	dst._primitiveType = primitiveType;
	dst._numOfVertices = numberOfVertices;
	dst._vertices = vertices;
	dst._numOfIndices = numberOfIndices;
	dst._indices = indices;

	return XML_SUCCESS;
}

