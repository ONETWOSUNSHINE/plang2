/// \file visitor_llvm.h
///


#ifndef VISITOR_LLVM_H_
#define VISITOR_LLVM_H_

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

class LLVMVisitor {
public:
    virtual llvm::Value* generateExpression(const ir::ExpressionPtr& _pNode) = 0;
    virtual llvm::Value* generateLiteral(const ir::LiteralPtr& _pNode) = 0;

    virtual llvm::Value* generateWild(const ir::WildPtr& _pNode) = 0;
    virtual llvm::Value* generateVariableReference(const ir::VariableReferencePtr& _pNode) = 0;
    virtual llvm::Value* generatePredicateReference(const ir::PredicateReferencePtr& _pNode) = 0;
    virtual llvm::Value* generateUnary(const ir::UnaryPtr& _pNode) = 0;
    virtual llvm::Value* generateBinary(const ir::BinaryPtr& _pNode) = 0;
    virtual llvm::Value* generateTernary(const ir::TernaryPtr& _pNode) = 0;
    virtual llvm::Value* generateTypeExpr(const ir::TypeExprPtr& _pNode) = 0;
    virtual llvm::Value* generateComponent(const ir::ComponentPtr& _pNode) = 0;
    virtual llvm::Value* generateFunctionCall(const ir::FunctionCallPtr& _pNode) = 0;
    virtual llvm::Value* generateFormulaCall(const ir::FormulaCallPtr& _pNode) = 0;
    virtual llvm::Value* generateLambda(const ir::LambdaPtr& _pNode) = 0;
    virtual llvm::Value* generateBinder(const ir::BinderPtr& _pNode) = 0;
    virtual llvm::Value* generateFormula(const ir::FormulaPtr& _pNode) = 0;
    virtual llvm::Value* generateConstructor(const ir::ConstructorPtr& _pNode) = 0;

    virtual llvm::Value* generateStructConstructor(const ir::StructConstructorPtr& _pNode) = 0;
    virtual llvm::Value* generateStructFieldDefinition(const ir::StructFieldDefinitionPtr& _pNode) = 0;


    virtual llvm::Value* generateArrayConstructor(const ir::ArrayConstructorPtr& _pNode) = 0;
    virtual llvm::Value* generateSetConstructor(const ir::SetConstructorPtr& _pNode) = 0;
    virtual llvm::Value* generateMapConstructor(const ir::MapConstructorPtr& _pNode) = 0;
    virtual llvm::Value* generateListConstructor(const ir::ListConstructorPtr& _pNode) = 0;
    virtual llvm::Value* generateArrayIteration(const ir::ArrayIterationPtr& _pNode) = 0;
    virtual llvm::Value* generateUnionConstructor(const ir::UnionConstructorPtr& _pNode) = 0;



    virtual llvm::Value* generateCastExpr(const ir::CastExprPtr& _pNode) = 0;

    virtual llvm::Value* generateArrayPartExpr(const ir::ArrayPartExprPtr& _pNode) = 0;
    virtual llvm::Value* generateFieldExpr(const ir::FieldExprPtr& _pNode) = 0;
    virtual llvm::Value* generateMapElementExpr(const ir::MapElementExprPtr& _pNode) = 0;
    virtual llvm::Value* generateListElementExpr(const ir::ListElementExprPtr& _pNode) = 0;
    virtual llvm::Value* generateReplacement(const ir::ReplacementPtr& _pNode) = 0;
    virtual llvm::Value* generateRecognizerExpr(const ir::RecognizerExprPtr& _pNode) = 0;
    virtual llvm::Value* generateAccessorExpr(const ir::AccessorExprPtr& _pNode) = 0;

    virtual llvm::Type *generateType(const ir::TypePtr &_pNode) = 0;
    virtual llvm::Type *generateTypeType(const ir::TypeTypePtr &_pNode) = 0;
    virtual llvm::Type *generateEnumType(const ir::EnumTypePtr &_pNode) = 0;
    virtual llvm::Type *generateEnumValue(const ir::EnumValuePtr &enumValue) = 0;

    virtual llvm::Type *generateStructType(const ir::StructTypePtr &_pNode) = 0;


    virtual llvm::Type *generateUnionType(const ir::UnionTypePtr &_pNode) = 0;
    virtual llvm::Type *generateArrayType(const ir::ArrayTypePtr &_pNode) = 0;
    virtual llvm::Type *generateSetType(const ir::SetTypePtr &_pNode) = 0;
    virtual llvm::Type *generateMapType(const ir::MapTypePtr &_pNode) = 0;
    virtual llvm::Type *generateListType(const ir::ListTypePtr &_pNode) = 0;
    virtual llvm::Type *generateSubtype(const ir::SubtypePtr &_pNode) = 0;
    virtual llvm::Type *generateRange(const ir::RangePtr &_pNode) = 0;
    virtual llvm::Type *generatePredicateType(const ir::PredicateTypePtr &_pNode) = 0;
    virtual llvm::Type *generateParameterizedType(const ir::ParameterizedTypePtr &_pNode) = 0;
    virtual llvm::Type *generateNamedReferenceType(const ir::NamedReferenceTypePtr &_pNode) = 0;
    virtual llvm::Type *generateRefType(const ir::RefTypePtr &_pNode) = 0;


    




};





#endif /* VISITOR_LLVM_H_ */