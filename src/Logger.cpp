#include "Logger.hpp"
#include "DotLogger.hpp"
#include <cstdint>
#include <sstream>
#include <string>

extern Logger LOG;

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
    .shape = "record",
};

static const DotLogger::Style EdgeStyle = {};

static const DotLogger::Style EdgeMainStyle = 
{
    // .tailport="s",
    .headport="n",
};

static const DotLogger::Style EdgeLhsStyle  = 
{
    // .tailport="s",
    .headport="w",
};



static const DotLogger::Style EdgeRhsStyle  = 
{
    // .tailport="s",
    .headport="e",
};

static const DotLogger::Style EdgeConsStyle  = 
{
    // .tailport="s",
    .headport="e",
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


std::string Logger::makeNodeLabel(const DebugInfo& info)
{
    std::stringstream label;
    label <<  "{name | value | addr | history}  | {" << info.originalName <<  " | " <<
        info.value.str() <<   " | " << info.address <<  " | " << info.history << "}";
    return label.str();
}
