#include "parser.h"
#include "pathparser.h"
#include <iterator>
#include <QTextStream>
#include <QFile>
#include <QRegExp>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <QStringList>

void PathParser::parseAndWrite(const std::string &obj, const std::string &output)
{
    std::vector<glm::vec3> vertices;
    std::vector<std::vector<size_t>> directedEdges;
    std::vector<std::vector<glm::vec3>> paths;
    PathParser::read(obj,vertices,directedEdges,paths);
    PathParser::write(output,vertices,directedEdges,paths);
}

void PathParser::write(const std::__cxx11::string &path, std::vector<glm::vec3> &vertices, std::vector<std::vector<size_t> > &directedEdges, std::vector<std::vector<glm::vec3>> &paths)
{
    std::ofstream file;
    file.open(path);
    for(size_t i = 0; i < paths.size(); i++)
    {
        if (i > 0){
            file << "START" << " ";
            std::vector<glm::vec3> currVertices = paths.at(i);
            for(std::vector<glm::vec3>::iterator it = currVertices.begin(); it != currVertices.end(); ++it)
            {
                std::string value = glm::to_string(*it).substr(5);
                value.erase(std::remove(value.begin(), value.end(), ' '), value.end());
                file << value << " ";
            }
            file << "END" << "\n";
        }
    }
    file << "\n";
    file.close();
}

void PathParser::read(const std::string &path, std::vector<glm::vec3> &vertices, std::vector<std::vector<size_t>> &directedEdges, std::vector<std::vector<glm::vec3>> &paths)
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
    std::vector<glm::vec3> currVertices;

    do
    {
        line = f.readLine().trimmed();
        QStringList parts = line.split(spaces);
        if (parts.isEmpty()) continue;

        if (parts[0] == "v" && parts.count() >= 4)
        {
            vertices.push_back(glm::vec3(parts[1].toFloat(), parts[2].toFloat(), parts[3].toFloat()));
            if (parts[1].toFloat()== 0.0 && parts[2].toFloat() == 0.0 && parts[3].toFloat() == 0.0){ // If vertex is (0,0,0) we are at the start of a new path
                paths.push_back(currVertices);
                currVertices.clear();
                currVertices.push_back(glm::vec3(parts[1].toFloat(), parts[2].toFloat(), parts[3].toFloat()));
            }else{
                currVertices.push_back(glm::vec3(parts[1].toFloat(), parts[2].toFloat(), parts[3].toFloat()));
            }
        }
        else if (parts[0] == "l" && parts.count() > 1)
        {
            edgeLists.push_back(parts);
        }
    }
    while (!line.isNull());

    // Need to add back in the final line
    if (currVertices.size() > 0){
        paths.push_back(currVertices);
    }

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

