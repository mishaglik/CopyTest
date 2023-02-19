#ifndef DOTLOGGER_HPP
#define DOTLOGGER_HPP

#include <cstddef>
#include <cstdint>
#include <fstream>
#include <ostream>
#include <string>

class DotLogger
{
public:
    DotLogger(const std::string& filename);
    ~DotLogger();
    
    // template<const char* >
    // struct 

    struct Style
    {
       std::string shape; //TODO: Enum
       std::string fgcolor;
       std::string fillcolor;
       std::string style;
       std::string label;
       std::string tailport;
       std::string headport;
       std::string weight;
    };

    void drawVertex(uint64_t id, const Style& v);
    void drawEdge  (uint64_t idFrom, uint64_t idTo, const Style& v);
    void openSubgraph ();
    void closeSubgraph();
    void setSubgraphLabel(const std::string& string);
    
    void print(std::string& s) { dotfile_ << s; }

    unsigned nTabs = 1;

    void tab() { for(size_t i = 0; i < nTabs; ++i) dotfile_ << '\t'; }

    std::string syscommand_;
    std::ofstream dotfile_;
};

std::ostream& operator<<(std::ostream& ostream, const DotLogger::Style& style);

#endif /* DOTLOGGER_HPP */
