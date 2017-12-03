#ifndef PARSER_H
#define PARSER_H

#include <QString>
#include "CommonIncludes.h"

class Parser
{
public:
    static void parseAndWrite(const std::string &obj, const std::string &output);
    static void read(const std::string &path, std::vector<glm::vec3> &vertices, std::vector<std::vector<size_t>> &directedEdges);
};

#endif // PARSER_H
