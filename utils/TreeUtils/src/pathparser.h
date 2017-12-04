#ifndef PATHPARSER_H
#define PATHPARSER_H

#include "parser.h"

#include <QString>
#include "CommonIncludes.h"

class PathParser : public Parser
{
public:
    static void parseAndWrite(const std::string &obj, const std::string &output);
    static void write(const std::string &path, std::vector<glm::vec3> &vertices, std::vector<std::vector<size_t>> &directedEdges, std::vector<std::vector<glm::vec3>> &paths);
    static void read(const std::string &path, std::vector<glm::vec3> &vertices, std::vector<std::vector<size_t>> &directedEdges, std::vector<std::vector<glm::vec3>> &paths);
};

#endif // PARSER_H
