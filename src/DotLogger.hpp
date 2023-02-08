#ifndef DOTLOGGER_HPP
#define DOTLOGGER_HPP

#include <cstdint>
#include <string>

class DotLogger
{
public:
    DotLogger(const char* filename);
    ~DotLogger();
    
    // template<const char* >
    // struct 

    struct Style
    {
       const char* shape; //TODO: Enum
       const char* fgcolor;
       const char* bgcolor;
       const char* style;
       std::string label;
    };

    void drawVertex(uint64_t id, const Style& v);
    void drawEdge  (uint64_t idFrom, uint64_t idTo, const Style& v);
    void openSubgraph();
    void closeSubgraph();
};

#endif /* DOTLOGGER_HPP */
