#include "parser.h"
#include <iterator>
#include <QTextStream>
#include <QFile>
#include <QRegExp>
#include <iostream>
#include <fstream>
#include <QStringList>

void Parser::writeJson(const std::__cxx11::string &path, std::vector<glm::vec3> &vertices, std::vector<std::vector<size_t> > &directedEdges)
{
    std::ofstream file;
    file.open(path);
//        file.write("{\n");
    file << "{" << std::endl;
    size_t i;
    for(i = 0; i < directedEdges.size() - 1; ++i)
    {
        std::vector<size_t> edges = directedEdges.at(i);
        std::string key = glm::to_string(vertices[i]).substr(5);
        key = key.substr(1, key.length()-2);
        file << "\t\"" << key << "\" : [";
        for(std::vector<size_t>::iterator it = edges.begin(); it != edges.end(); ++it)
        {
            std::string value = glm::to_string(vertices[*it]).substr(5);
            value = value.substr(1, value.length()-2);
            file << "\"" << value << "\"";
            if(*it != edges.back())
            {
                file << ",";
            }
        }
        file << "]," << std::endl;
    }

    std::vector<size_t> edges = directedEdges.at(i);
    std::string key = glm::to_string(vertices[i]).substr(5);
    key = key.substr(1, key.length()-2);
    file << "\t\"" << key << "\" : [";
    for(std::vector<size_t>::iterator it = edges.begin(); it != edges.end(); ++it)
    {
        std::string value = glm::to_string(vertices[*it]).substr(5);
        value = value.substr(1, value.length()-2);
        file << "\"" << value << "\"";
        if(*it != edges.back())
        {
            file << ",";
        }
    }
    file << "]" << std::endl;

    file << "}\n";
    file.close();
}

void Parser::writeObj(const std::__cxx11::string &path, std::vector<glm::vec3> &vertices, std::vector<std::vector<size_t> > &directedEdges)
{
    std::ofstream file;
    file.open(path);

    for(auto it = vertices.begin(); it != vertices.end(); ++it)
    {
        const glm::vec3 v = *it;
        file << "v " << v.x << " " << v.y << " " << v.z << std::endl;
    }
    size_t parent = 1;
    for(auto it = directedEdges.begin(); it != directedEdges.end(); ++parent, ++it)
    {
        const std::vector<size_t> children = *it;
        for(auto ft = children.begin(); ft != children.end(); ++ft)
        {
            const size_t child = *ft;
            file << "l " << parent << " " << (child + 1) << std::endl;
        }
    }
    file.close();
}

void Parser::readObj(const std::__cxx11::string &path, std::vector<glm::vec3> &vertices)
{
    // Open the file
    QFile file(path.c_str());
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        std::cout << "ERROR: File could not be read." << std::endl;
        return;
    }

    QTextStream f(&file);
    QString line;
    QRegExp spaces("\\s+");
    do
    {
        line = f.readLine().trimmed();
        QStringList parts = line.split(spaces);
        if (parts.isEmpty()) continue;

        if (parts[0] == "v" && parts.count() >= 4)
        {
            glm::vec3 vert = glm::vec3(parts[1].toFloat(), parts[2].toFloat(), parts[3].toFloat());
            if(glm::abs(vert.x) < .0001)
            {
                vert.x = 0.f;
            }
            if(glm::abs(vert.y) < .0001)
            {
                vert.y = 0.f;
            }
            if(glm::abs(vert.z) < .0001)
            {
                vert.z = 0.f;
            }
            vertices.push_back(vert);
        }
    }
    while (!line.isNull());
}

void Parser::readObj(const std::string &path, std::vector<glm::vec3> &vertices, std::vector<std::vector<size_t>> &directedEdges)
{
    // Open the file
    QFile file(path.c_str());
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        std::cout << "ERROR: File could not be read." << std::endl;
        return;
    }

    QTextStream f(&file);
    QString line;
    QRegExp spaces("\\s+");
    std::vector<QStringList> edgeLists;
    do
    {
        line = f.readLine().trimmed();
        QStringList parts = line.split(spaces);
        if (parts.isEmpty()) continue;

        if (parts[0] == "v" && parts.count() >= 4)
        {
            vertices.push_back(glm::vec3(parts[1].toFloat(), parts[2].toFloat(), parts[3].toFloat()));
        }
        else if (parts[0] == "l" && parts.count() > 1)
        {
            edgeLists.push_back(parts);
        }
    }
    while (!line.isNull());

    directedEdges.resize(vertices.size());
    for(std::vector<QStringList>::iterator it = edgeLists.begin(); it != edgeLists.end(); ++it)
    {
        QStringList currentLine = *it;
        for(size_t i = currentLine.size() - 1; i > 1; i--)
        {
            size_t parent = currentLine[i].toUInt();
            size_t child = currentLine[i-1].toUInt();
            directedEdges.at(parent-1).push_back(child-1);
        }
    }
}

