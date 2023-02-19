#include "DotLogger.hpp"
#include <cstdio>
#include <cassert>
#include <cstdlib>
#include <string>
#include <iostream>

uint64_t TempId = 0;
uint64_t Copies = 0;

DotLogger::DotLogger(const std::string& filename)
{
    char tmpName[L_tmpnam];
    assert(std::tmpnam(tmpName));
    
    syscommand_ = "dot ";
    syscommand_ += tmpName;
    syscommand_ += " -T png -o ";
    syscommand_ += filename;

    dotfile_.open(tmpName);
    assert(dotfile_.is_open());
    dotfile_ << "digraph {\n";
    // dotfile_ << "graph [splines=ortho]\n";
    dotfile_ << "overlap = scale;\n";
}

DotLogger::~DotLogger()
{
    dotfile_ << "}\n";
    dotfile_.close();
    std::cerr << syscommand_ << '\n';

    int res = std::system(syscommand_.c_str());
    if(res)
    {
        std::cerr << "Dot failed with: " << res << '\n';
    }
    
}

void DotLogger::drawVertex(uint64_t id, const Style& v)
{
    tab();
    dotfile_ << id << "[" << v << "]\n";
}

void DotLogger::drawEdge  (uint64_t idFrom, uint64_t idTo, const Style& v)
{
    tab();
    dotfile_ << idFrom << " -> " << idTo << "\t[" << v << "]\n";
}

void DotLogger::openSubgraph()
{
    static int fId = 0;
    tab();
    nTabs++;
    dotfile_ << "subgraph cluster_" << ++fId << "{\n";
    dotfile_ << "style=filled;\n"
        << "fillcolor=\"#00000020\";\n";
}

void DotLogger::setSubgraphLabel(const std::string& string)
{
    tab();
    dotfile_ << "label = \"" << string << "\"\n"; 
}

void DotLogger::closeSubgraph()
{
    nTabs--;
    tab();
    dotfile_ << "}\n";
}


#define STYLE_PRINT_(param) \
    if(!style.param.empty())\
    {                       \
        ostream << #param" = \"" << style.param << "\" ";\
    }

std::ostream& operator<<(std::ostream& ostream, const DotLogger::Style& style)
{
    STYLE_PRINT_(shape);
    STYLE_PRINT_(fgcolor);
    STYLE_PRINT_(fillcolor);
    STYLE_PRINT_(style);
    // STYLE_PRINT_(label);
    if(!style.label.empty())
    {
        if(style.label[0] == '<')
            ostream << "label=" << style.label << " ";
        else
            ostream << "label=\"" << style.label << "\" ";
    }
    STYLE_PRINT_(tailport);
    STYLE_PRINT_(headport);
    STYLE_PRINT_(weight);
    return ostream;
}
#undef STYLE_PRINT_