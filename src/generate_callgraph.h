/// \file generate_callgraph.h
///

#ifndef GENERATE_CALLGRAPH_H_
#define GENERATE_CALLGRAPH_H_

#include <queue>
#include <set>

#include "ir/declarations.h"
#include "ir/visitor.h"
#include "callgraph.h"

namespace ir {

void generateCallGraph(Module &_module, CallGraph &_graph);

void printModuleCallGraph(Module &_module, std::wostream &_os);

}

#endif /* GENERATE_CALLGRAPH_H_ */
