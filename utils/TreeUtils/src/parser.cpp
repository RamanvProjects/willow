#include "parser.h"
#include <iterator>
#include <QTextStream>
#include <QFile>
#include <QRegExp>
#include <iostream>
#include <fstream>
#include <QStringList>

void Parser::parseAndWrite(const std::string &obj, const std::string &output)
{
    std::vector<glm::vec3> vertices;
    std::vector<std::vector<size_t>> directedEdges;
    Parser::read(obj,vertices,directedEdges);
    Parser::write(output,vertices,directedEdges);
}

void Parser::write(const std::__cxx11::string &path, std::vector<glm::vec3> &vertices, std::vector<std::vector<size_t> > &directedEdges)
{
    std::ofstream file;
    file.open(path);
//        file.write("{\n");
    file << "{\n";
    for(size_t i = 0; i < directedEdges.size(); i++)
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
        file << "],\n";
    }
    file << "}\n";
    file.close();
}

void Parser::read(const std::string &path, std::vector<glm::vec3> &vertices, std::vector<std::vector<size_t>> &directedEdges)
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

