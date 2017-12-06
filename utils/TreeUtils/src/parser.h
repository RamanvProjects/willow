#ifndef PARSER_H
#define PARSER_H

#include <QString>
#include "CommonIncludes.h"

class Parser
{
public:
    static void writeJson(const std::string &path, std::vector<glm::vec3> &vertices, std::vector<std::vector<size_t>> &directedEdges);
    static void writeObj(const std::string &path, std::vector<glm::vec3> &vertices, std::vector<std::vector<size_t>> &directedEdges);
    static void readObj(const std::string &path, std::vector<glm::vec3> &vertices, std::vector<std::vector<size_t>> &directedEdges);
    static void readObj(const std::string &path, std::vector<glm::vec3> &vertices);
};

#endif // PARSER_H
