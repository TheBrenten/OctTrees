#include "Mesh.h"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "VertexBufferLayout.h"
#define say(x) std::cout << x << std::endl
std::vector<std::string> splitString(std::string str, char delimiter) {
    std::vector<std::string> pieces;
    std::string word = "";
    for (auto x : str) {
        if (x == delimiter) {
            pieces.push_back(word);
            word = "";
        }
        else {
            word += x;
        }
    }
    pieces.push_back(word);
    return pieces;
}
#include <iostream>
Mesh::Mesh(std::string filepath) {


    std::ifstream stream(filepath);
    std::string line;

    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> faces;

    while (getline(stream, line)) {
        std::string word = "";
        std::vector<std::string> pieces = splitString(line, ' ');
        //std::cout<< pieces[0] << std::endl;

        if (pieces[0] == "v") {
            float x = stof(pieces[1]);
            float y = stof(pieces[2]);
            float z = stof(pieces[3]);
            positions.push_back({ x,y,z });
        }
        if (pieces[0] == "vt") {
            float u = stof(pieces[1]);
            float v = stof(pieces[2]);
            texCoords.push_back({ u, v });
        }
        // dont care about normals yet
        if (pieces[0] == "vn") {
            float x = stof(pieces[1]);
            float y = stof(pieces[2]);
            float z = stof(pieces[3]);
            normals.push_back({ x,y,z });
        }
        if (pieces[0] == "f") {
            unsigned int a = stoi(splitString(pieces[1], '/')[0]);
            unsigned int b = stoi(splitString(pieces[2], '/')[0]);
            unsigned int c = stoi(splitString(pieces[3], '/')[0]);
            //std::cout << b << std::endl;
            say(a << " " << b << " " << c);
            faces.push_back({ a,b,c });
        }
    }
    int count = positions.size() * 3 * 5;
    vertices = new float[count];
    indices = new unsigned int[faces.size()*3];
    //std::cout << count << std::endl;
    for (int i = 0; i < positions.size(); i++) {
        vertices[i*5+0] = positions[i+0][0];
        vertices[i*5+1] = positions[i+0][1];
        vertices[i*5+2] = positions[i+0][2];
        vertices[i*5+3] = texCoords[i+0][0];
        vertices[i*5+4] = texCoords[i+0][1];

    }

    for (int i = 0; i < faces.size();i++) {
        indices[i*3+0] = faces[i][0] - 1;
        indices[i*3+1] = faces[i][1] - 1;
        indices[i*3+2] = faces[i][2] - 1;

    }
    vb = new VertexBuffer(vertices, count * sizeof(float));
    ib = new IndexBuffer(indices, faces.size() * 3);
    VertexBufferLayout layout;
    layout.push<float>(3);
    layout.push<float>(2);
    va.addBuffer(*vb, layout);

}

Mesh::~Mesh() {
    delete vb;
    delete ib;
    delete[] vertices;
    delete[] indices;
}
