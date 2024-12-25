/// \file backend_llvm.h
///


#ifndef BACKEND_LLVM_H_
#define BACKEND_LLVM_H_

#include <iostream>
#include <memory>
#include <string>

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include "ir/base.h"
#include "ir/visitor.h"
#include <list>

#include "ir/types.h"
#include "ir/declarations.h"
#include "ir/expressions.h"
#include "ir/statements.h"

#include "visitor_llvm.h"

void generateLLVM(const ir::ModulePtr &_module, std::wostream & _os = std::wcout);

class GenerateLLVM : public LLVMVisitor {
protected:
    std::unique_ptr<llvm::LLVMContext> context;
    std::unique_ptr<llvm::IRBuilder<>> builder;
    std::unique_ptr<llvm::Module> module;

    std::map<std::wstring, llvm::AllocaInst *> varEnv;
    std::map<std::wstring, llvm::Function*> predEnv;


//     Что такое llvm::AllocaInst?
// llvm::AllocaInst — это инструкция в LLVM IR, которая используется для выделения памяти на стеке. В C или C++ 
// это эквивалентно оператору alloca, который выделяет память для переменной внутри функции. В LLVM это используется для создания локальных переменных в функции.






public:
    GenerateLLVM();
    
    void dumpLLVMIR();
    std::string dumpLLVMIRToString();

    llvm::Value* generateExpression(const ir::ExpressionPtr& _pNode) override;
    llvm::Value* generateLiteral(const ir::LiteralPtr& _pNode) override;
    llvm::Value* generateWild(const ir::WildPtr& _pNode) override;


    llvm::Value* generateVariableReference(const ir::VariableReferencePtr& _pNode) override;
    llvm::Value* generatePredicateReference(const ir::PredicateReferencePtr& _pNode) override;
    llvm::Value* generateUnary(const ir::UnaryPtr& _pNode) override;
    llvm::Value* generateBinary(const ir::BinaryPtr& _pNode) override;
    llvm::Value* generateTernary(const ir::TernaryPtr& _pNode) override;
    llvm::Value* generateTypeExpr(const ir::TypeExprPtr& _pNode) override;
    llvm::Value* generateComponent(const ir::ComponentPtr& _pNode) override;
    llvm::Value* generateFunctionCall(const ir::FunctionCallPtr& _pNode) override;
    llvm::Value* generateFormulaCall(const ir::FormulaCallPtr& _pNode) override;
    llvm::Value* generateLambda(const ir::LambdaPtr& _pNode) override;
    llvm::Value* generateBinder(const ir::BinderPtr& _pNode) override;
    llvm::Value* generateFormula(const ir::FormulaPtr& _pNode) override;
    llvm::Value* generateConstructor(const ir::ConstructorPtr& _pNode) override;

    llvm::Value* generateStructConstructor(const ir::StructConstructorPtr& _pNode) override;
    llvm::Value* generateStructFieldDefinition(const ir::StructFieldDefinitionPtr& _pNode) override;


    llvm::Value* generateArrayConstructor(const ir::ArrayConstructorPtr& _pNode) override;
    llvm::Value* generateSetConstructor(const ir::SetConstructorPtr& _pNode) override;
    llvm::Value* generateMapConstructor(const ir::MapConstructorPtr& _pNode) override;
    llvm::Value* generateListConstructor(const ir::ListConstructorPtr& _pNode) override;
    llvm::Value* generateArrayIteration(const ir::ArrayIterationPtr& _pNode) override;
    llvm::Value* generateUnionConstructor(const ir::UnionConstructorPtr& _pNode) override;


    
    llvm::Value* generateCastExpr(const ir::CastExprPtr& _pNode) override;

    llvm::Value* generateArrayPartExpr(const ir::ArrayPartExprPtr& _pNode) override;
    llvm::Value* generateFieldExpr(const ir::FieldExprPtr& _pNode) override;
    llvm::Value* generateMapElementExpr(const ir::MapElementExprPtr& _pNode) override;
    llvm::Value* generateListElementExpr(const ir::ListElementExprPtr& _pNode) override;
    llvm::Value* generateReplacement(const ir::ReplacementPtr& _pNode) override;
    llvm::Value* generateRecognizerExpr(const ir::RecognizerExprPtr& _pNode) override;
    llvm::Value* generateAccessorExpr(const ir::AccessorExprPtr& _pNode) override;

    llvm::Type *generateType(const ir::TypePtr &_pNode) override;
    llvm::Type *generateTypeType(const ir::TypeTypePtr &_pNode) override;
    llvm::Type *generateEnumType(const ir::EnumTypePtr &_pNode) override;
    llvm::Type *generateEnumValue(const ir::EnumValuePtr &enumValue) override;



    llvm::Type *generateStructType(const ir::StructTypePtr &_pNode) override;
    llvm::Type *generateUnionType(const ir::UnionTypePtr &_pNode) override;
    llvm::Type *generateArrayType(const ir::ArrayTypePtr &_pNode) override;
    llvm::Type *generateSetType(const ir::SetTypePtr &_pNode) override;
    llvm::Type *generateMapType(const ir::MapTypePtr &_pNode) override;
    llvm::Type *generateListType(const ir::ListTypePtr &_pNode) override;
    llvm::Type *generateSubtype(const ir::SubtypePtr &_pNode) override;
    llvm::Type *generateRange(const ir::RangePtr &_pNode) override;
    llvm::Type *generatePredicateType(const ir::PredicateTypePtr &_pNode) override;
    llvm::Type *generateParameterizedType(const ir::ParameterizedTypePtr &_pNode) override;
    llvm::Type *generateNamedReferenceType(const ir::NamedReferenceTypePtr &_pNode) override;
    llvm::Type *generateRefType(const ir::RefTypePtr &_pNode) override;




};





#endif /* BACKEND_LLVM_H_ */