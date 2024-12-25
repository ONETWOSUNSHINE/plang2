#include "backend_llvm.h"
#include <iostream>
#include <memory>
#include "ir/numbers.h"

int main() {
    GenerateLLVM generateLLVM;
    Number *a = new Number();
    a->makeReal(42);
    ir::LiteralPtr literal = std::make_shared<ir::Literal>(a);

    llvm::Value* llvmValue = generateLLVM.generateLiteral(literal);

    if (llvmValue) {
        llvm::errs() << *llvmValue << "\n"; 
    } else {
        std::cerr << "Error!" << std::endl;
    }

    literal->setBool(true); 

    llvmValue = generateLLVM.generateLiteral(literal);
    if (llvmValue) {
        llvm::errs() << *llvmValue << "\n"; 
    } else {
        std::cerr << "Error!" << std::endl;
    }

    return 0;
}
