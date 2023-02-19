#include "Logger.hpp"
#include "DotLogger.hpp"
#include <cstdint>
#include <sstream>
#include <string>

extern Logger LOG;
extern uint64_t TempId;
static const DotLogger::Style ConstructorNodeStyle = 
{
    .shape = "invhouse",
    .fillcolor = "#B0B0FF",
    .style = "filled",
    .label = "Ctor"
};

static const DotLogger::Style CopyConstructorNodeStyle = 
{
    .shape = "invhouse",
    .fillcolor = "red",
    .style = "filled",
    .label = "!!!COPY!!!"
};

static const DotLogger::Style MoveAssignmentNodeStyle = 
{
    .shape = "diamond",
    .fillcolor = "#B0D0B0",
    .style = "filled",
    .label = "Move assign"
};

static const DotLogger::Style MoveConstructorNodeStyle = 
{
    .shape = "invhouse",
    .fillcolor = "#B0D0B0",
    .style = "filled",
    .label = "Move"
};

static const DotLogger::Style CopyAssignmentNodeStyle = 
{
    .shape = "diamond",
    .fillcolor = "red",
    .style = "filled",
    .label = "!!!COPY ASSIGN!!!"
};

static const DotLogger::Style OperatorNodeStyle = 
{
    .shape = "invhouse",
    .fillcolor = "#B0B0FF",
    .style = "filled",
    // .label = "!!!COPY ASSIGN!!!"
};

static const DotLogger::Style DefaultNodeStyle = 
{
    .shape = "none",
    // .fillcolor = "#E0E0E0",
    // .fillcolor = "white",
    // .style = "filled"
};

static const DotLogger::Style EdgeStyle = {
    .tailport="s",
    .headport="n",
    .weight="100",
};

static const DotLogger::Style EdgeMainStyle = 
{
    .tailport="s",
    .headport="n",
    .weight="100",
};

static const DotLogger::Style EdgeLhsStyle  = 
{
    .tailport="s",
    .headport="w",
    .weight = "1",
};



static const DotLogger::Style EdgeRhsStyle  = 
{
    .tailport="s",
    .headport="e",
    .weight = "1",
};

static const DotLogger::Style EdgeConsStyle  = 
{
    .tailport="s",
    .headport="e",
    .weight = "1",
};

uint64_t Logger::nodeId = 0;

void Logger::logCreation(const DebugInfo& info)
{
    uint64_t ctorNodeId = ++nodeId;
    info.nodeId = ++nodeId;
    dot_.drawVertex(ctorNodeId, ConstructorNodeStyle);
    DotLogger::Style style = DefaultNodeStyle;
    style.label = makeNodeLabel(info);
    dot_.drawVertex(info.nodeId, style);

    dot_.drawEdge(ctorNodeId, info.nodeId, EdgeStyle);
}

void Logger::logCreationCopy(const DebugInfo& info)
{
    uint64_t ctorNodeId = ++nodeId;
    info.nodeId = ++nodeId;
    dot_.drawVertex(ctorNodeId, CopyConstructorNodeStyle);
    DotLogger::Style style = DefaultNodeStyle;
    style.label = makeNodeLabel(info);
    dot_.drawVertex(info.nodeId, style);

    dot_.drawEdge(info.parentId, ctorNodeId, EdgeConsStyle);
    dot_.drawEdge(ctorNodeId, info.nodeId, EdgeStyle);
}

void Logger::logCreationMove(const DebugInfo& info)
{
    uint64_t ctorNodeId = ++nodeId;
    info.nodeId = ++nodeId;
    dot_.drawVertex(ctorNodeId, MoveConstructorNodeStyle);
    DotLogger::Style style = DefaultNodeStyle;
    style.label = makeNodeLabel(info);
    dot_.drawVertex(info.nodeId, style);

    dot_.drawEdge(info.parentId, ctorNodeId, EdgeConsStyle);
    dot_.drawEdge(ctorNodeId, info.nodeId, EdgeStyle);
}

void Logger::logCreationOperator(const DebugInfo& infoRes, const DebugInfo& infoLhs, const DebugInfo& infoRhs, const char* operatorName)
{
    uint64_t operNodeId = ++nodeId;
    DotLogger::Style oprStyle = OperatorNodeStyle;
    oprStyle.label = operatorName;
    dot_.drawVertex(operNodeId, oprStyle);

    dot_.drawEdge(infoLhs.nodeId, operNodeId, EdgeLhsStyle);
    dot_.drawEdge(infoRhs.nodeId, operNodeId, EdgeRhsStyle);

    infoRes.nodeId = ++nodeId;
    DotLogger::Style style = DefaultNodeStyle;
    style.label = makeNodeLabel(infoRes);
    dot_.drawVertex(infoRes.nodeId, style);

    dot_.drawEdge(operNodeId, infoRes.nodeId, EdgeStyle);
}

void Logger::logCreationOperator(const DebugInfo& infoRes, const DebugInfo& infoLhs, const char* operatorName)
{
    uint64_t operNodeId = ++nodeId;
    DotLogger::Style oprStyle = OperatorNodeStyle;
    oprStyle.label = operatorName;
    dot_.drawVertex(operNodeId, oprStyle);

    dot_.drawEdge(infoLhs.nodeId, operNodeId, EdgeConsStyle);

    infoRes.nodeId = ++nodeId;
    DotLogger::Style style = DefaultNodeStyle;
    style.label = makeNodeLabel(infoRes);
    dot_.drawVertex(infoRes.nodeId, style);

    dot_.drawEdge(operNodeId, infoRes.nodeId, EdgeStyle);
}


void Logger::logAssigmentCopy(const DebugInfo& info)
{
    uint64_t assignNodeId = ++nodeId;
    dot_.drawVertex(assignNodeId, CopyAssignmentNodeStyle);

    dot_.drawEdge(info.parentId, assignNodeId, EdgeRhsStyle);
    dot_.drawEdge(info.nodeId, assignNodeId, EdgeMainStyle);

    info.nodeId = ++nodeId;
    DotLogger::Style style = DefaultNodeStyle;
    style.label = makeNodeLabel(info);
    dot_.drawVertex(info.nodeId, style);
    
    dot_.drawEdge(assignNodeId, info.nodeId, EdgeStyle);
}

void Logger::logAssigmentMove(const DebugInfo& info)
{
    uint64_t assignNodeId = ++nodeId;
    dot_.drawVertex(assignNodeId, MoveAssignmentNodeStyle);

    dot_.drawEdge(info.parentId, assignNodeId, EdgeRhsStyle);
    dot_.drawEdge(info.nodeId, assignNodeId, EdgeMainStyle);

    info.nodeId = ++nodeId;
    DotLogger::Style style = DefaultNodeStyle;
    style.label = makeNodeLabel(info);
    dot_.drawVertex(info.nodeId, style);
    
    dot_.drawEdge(assignNodeId, info.nodeId, EdgeStyle);
}

void Logger::logDeletion(const DebugInfo& info)
{

}


void Logger::enterFunction(const char* newName)
{
    dot_.openSubgraph();
    dot_.setSubgraphLabel(newName);
    
}

void Logger::leaveFunction()
{
    dot_.closeSubgraph();
}


static unsigned ptrToColor(void* ptr)
{
    uintptr_t val = reinterpret_cast<uintptr_t>(ptr);
    val &= 0x1FFFFF;
    val <<= 3;
    return val;
}

std::string Logger::makeNodeLabel(const DebugInfo& info)
{
    std::stringstream label;
    // label <<  "{name | value | addr | history}  | {" << info.originalName <<  " | " <<
        // info.value.str() <<   " | " << info.address <<  " | " << info.history << "}";
    label << "<\n"
             "  <TABLE CELLSPACING=\"0\" BGCOLOR=\"#E0E0E0\">\n"
             "    <TR>\n"
             "    <TD> name </TD>\n"
             "    <TD> " << info.originalName << " </TD>\n"
             "    </TR>\n"
             "    <TR>\n"
             "    <TD> value </TD>\n"
             "    <TD> " << info.value.str()  << " </TD>\n"
             "    </TR>\n"
             "    <TR>\n"
             "    <TD> addr </TD>\n"
             "    <TD BGCOLOR=\"#"<< std::hex << ptrToColor(info.address) <<"\"> " << info.address      << " </TD>\n"
             "    </TR>\n"
             "    <TR>\n"
             "    <TD> history </TD>\n"
             "    <TD> " << info.history      << " </TD>\n"
             "    </TR>\n"
             "  </TABLE>\n>\n";
    return label.str();
}

Logger::~Logger()
{
    // dot_.openSubgraph();
    // dot_.setSubgraphLabel("Legend");
    // dot_.dotfile_ << "rankdir=LR;\n";
    // dot_.drawVertex(++nodeId, {.shape="invhouse", .label="Constructor"});
    // dot_.drawVertex(++nodeId, {.shape="none", .label="    "});
    // dot_.drawVertex(++nodeId, {.shape="diamond", .label="Operator"});
    // dot_.drawVertex(++nodeId, {.shape="none", .label="    "});
    // // dot_.dotfile_ << "{rank=same; "<< nodeId-2<<", " <<nodeId-1<< "}\n";
    // dot_.drawEdge(nodeId-3, nodeId-1, {.label="This", .headport="n", .weight="1000"});
    // dot_.drawEdge(nodeId, nodeId-1, {.label="Lhs", .headport="w", .weight="1000"});
    // dot_.drawEdge(nodeId-2, nodeId-1,   {.label="Rhs", .headport="e", .weight="1000"});

    // dot_.dotfile_ << "Results [shape=none, label = <\n"
    //     "<TABLE>\n"
    //     "<TR>\n"
    //     "<TD> Copies </TD>\n"
    //     "<TD> "<< 0 <<" </TD>\n"
    //     "</TR>\n"
    //     "<TR>\n"
    //     "<TD> Temp objects </TD>\n"
    //     "<TD> "<< TempId <<" </TD>\n"
    //     "</TR>\n"
    //     "</TABLE>\n"
    // ">]\n";

    // dot_.drawVertex(++nodeId, {.shape="plaintext", .fillcolor="#B0B0FF", .style="filled", .label="Other"});
    // dot_.drawVertex(++nodeId, {.shape="plaintext", .fillcolor="#B0D0B0", .style="filled", .label="Moving"});
    // dot_.drawVertex(++nodeId, {.shape="plaintext", .fillcolor="red", .style="filled", .label="Copying"});
    // dot_.drawVertex(++nodeId, C);
    // dot_.closeSubgraph();
    extern uint64_t Copies;
    std::cerr << "Copies: " << Copies << "\nTempObjects: " << TempId << '\n';
}