#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Verifier.h"
#include <llvm/IR/Constants.h>
#include <llvm/IR/Type.h>
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Utils.h"

#include "backend_llvm.h"
#include "visitor.h"
#include <locale>
#include <codecvt>
#include "node_analysis.h"


GenerateLLVM::GenerateLLVM() {
    context = std::make_unique<llvm::LLVMContext>();
    builder = std::make_unique<llvm::IRBuilder<>>(*context);
    module = std::make_unique<llvm::Module>("Module", *context);
}

void GenerateLLVM::dumpLLVMIR() { module->print(llvm::outs(), nullptr); }

std::string GenerateLLVM::dumpLLVMIRToString() {
  std::string outstr;
  llvm::raw_string_ostream oss(outstr);

  module->print(oss, nullptr);

  return oss.str();
}


llvm::Value* GenerateLLVM::generateExpression(const ir::ExpressionPtr& _pExpr) {
    switch (_pExpr->getKind()) {
    case ir::Expression::WILD:
        return generateWild(_pExpr->as<ir::Wild>());
    case ir::Expression::LITERAL:
        return generateLiteral(_pExpr->as<ir::Literal>());
    case ir::Expression::VAR:
        return generateVariableReference(_pExpr->as<ir::VariableReference>());
    case ir::Expression::PREDICATE:
        return generatePredicateReference(_pExpr->as<ir::PredicateReference>());
    case ir::Expression::UNARY:
        return generateUnary(_pExpr->as<ir::Unary>());
    case ir::Expression::BINARY:
        return generateBinary(_pExpr->as<ir::Binary>());
    case ir::Expression::TERNARY:
        return generateTernary(_pExpr->as<ir::Ternary>());
    case ir::Expression::TYPE:
        return generateTypeExpr(_pExpr->as<ir::TypeExpr>());
    case ir::Expression::COMPONENT:
        return generateComponent(_pExpr->as<ir::Component>());
    case ir::Expression::FUNCTION_CALL:
        return generateFunctionCall(_pExpr->as<ir::FunctionCall>());
    case ir::Expression::FORMULA_CALL:
        return generateFormulaCall(_pExpr->as<ir::FormulaCall>());
    case ir::Expression::LAMBDA:
        return generateLambda(_pExpr->as<ir::Lambda>());
    case ir::Expression::BINDER:
        return generateBinder(_pExpr->as<ir::Binder>());
    case ir::Expression::FORMULA:
        return generateFormula(_pExpr->as<ir::Formula>());
    case ir::Expression::CONSTRUCTOR:
        return generateConstructor(_pExpr->as<ir::Constructor>());
    case ir::Expression::CAST:
        return generateCastExpr(_pExpr->as<ir::CastExpr>());
    }
    return nullptr; 
}

std::string wstrToStr(const std::wstring& wstr) {
    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;
    return converter.to_bytes(wstr);
}

llvm::Value* GenerateLLVM::generateLiteral(const ir::LiteralPtr& _pNode) {

    llvm::Value *literalValue = nullptr;

    switch (_pNode->getLiteralKind()) {
        case ir::Literal::UNIT:
            literalValue = llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), 0);
            break;
        case ir::Literal::NUMBER:
            if (_pNode->getNumber().isReal()) {
                literalValue = llvm::ConstantFP::get(llvm::Type::getDoubleTy(*context), _pNode->getNumber().getFloat());
            } else {
                literalValue = llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), _pNode->getNumber().getInt());
            }
            break;
        case ir::Literal::BOOL:
            literalValue = llvm::ConstantInt::get(llvm::Type::getInt1Ty(*context), _pNode->getBool() ? 1 : 0);
            break;
    }
    
    return literalValue;
}

llvm::Value* GenerateLLVM::generateVariableReference(const ir::VariableReferencePtr& _pNode) {

    const std::wstring& varName = _pNode->getName();

    if (auto target = _pNode->getTarget()) {
        auto it = varEnv.find(varName);
        if (it == varEnv.end()) {
            std::wcerr << "Error: Variable " << varName << " not found in environment!" << std::endl;
            return nullptr; 
        }

        llvm::AllocaInst* allocaInst = it->second;

        return allocaInst;

    } else {
        llvm::AllocaInst* allocaInst = builder->CreateAlloca(llvm::Type::getInt32Ty(*context), nullptr, wstrToStr(varName));
        varEnv[varName] = allocaInst;
        return allocaInst;
    }
}
llvm::Value* GenerateLLVM::generatePredicateReference(const ir::PredicateReferencePtr& _pNode) {
    const std::wstring& predName = _pNode->getName();

    if (auto target = _pNode->getTarget()) {
        auto it = predEnv.find(predName);
        if (it == predEnv.end()) {
            std::wcerr << "Error: Predicate " << predName << " not found in environment!" << std::endl;
            return nullptr;
        }

        llvm::Function* predFunc = it->second;
        return predFunc;

    } else {

        llvm::FunctionType* funcType = llvm::FunctionType::get(llvm::Type::getVoidTy(*context), false);
        llvm::Function* newPredFunc = llvm::Function::Create(funcType, llvm::GlobalValue::ExternalLinkage, wstrToStr(predName), module.get());

        predEnv[predName] = newPredFunc;

        return newPredFunc;
    }
}
llvm::Value* GenerateLLVM::generateUnary(const ir::UnaryPtr& _pNode) {

    llvm::Value* exprValue = generateExpression(_pNode->getExpression());

    if (!exprValue) {
        std::wcerr << "Error: Unable to generate expression for unary operation!" << std::endl;
        return nullptr;
    }

    switch (_pNode->getOperator()) {
        case ir::Unary::PLUS:
            return exprValue;
        case ir::Unary::MINUS:
            return builder->CreateNeg(exprValue, "neg");
        case ir::Unary::BOOL_NEGATE:
            return builder->CreateNot(exprValue, "bool_negate");
        case ir::Unary::BITWISE_NEGATE: {   
            llvm::Value* minusOne = llvm::ConstantInt::get(exprValue->getType(), ~0);
            return builder->CreateXor(exprValue, minusOne, "bitwise_negate");
        }
        default: {
            std::wcerr << "Error: Unsupported unary operator!" << std::endl;
            return nullptr;
        }
    }
}

llvm::Value* GenerateLLVM::generateBinary(const ir::BinaryPtr& _pNode) {  //dodelat
    llvm::Value* leftValue = generateExpression(_pNode -> getLeftSide());
    llvm::Value* rightValue = generateExpression(_pNode -> getRightSide());
    switch(_pNode -> getOperator()) {
        case ir::Binary::REMAINDER: {
            return builder->CreateSRem(leftValue, rightValue, "rem_temp");
        }
        case ir::Binary::IN: {
            
            // m_os << L"member(";
            // VISITOR_TRAVERSE(Expression, BinarySubexpression, _node->getLeftSide(), _node, Binary, setLeftSide);
            // m_os << L", ";
            // VISITOR_TRAVERSE(Expression, BinarySubexpression, _node->getRightSide(), _node, Binary, setRightSide);
            // m_os << L")";
            // VISITOR_EXIT();
        }
        case ir::Binary::BOOL_AND:
            return builder->CreateAnd(leftValue, rightValue, "and_temp");
        case ir::Binary::BOOL_OR:
            return builder->CreateOr(leftValue, rightValue, "or_temp");
        case ir::Binary::BOOL_XOR:
            return builder->CreateXor(leftValue, rightValue, "xor_temp");
        case ir::Binary::ADD:
            return builder->CreateAdd(leftValue, rightValue, "add_temp");
        case ir::Binary::SUBTRACT:
            return builder->CreateSub(leftValue, rightValue, "sub_temp");
        case ir::Binary::MULTIPLY:
            return builder->CreateMul(leftValue, rightValue, "mul_temp");
        case ir::Binary::DIVIDE:
            return builder->CreateSDiv(leftValue, rightValue, "div_temp");
        case ir::Binary::SHIFT_LEFT:
            return builder->CreateShl(leftValue, rightValue, "shl_temp");
        case ir::Binary::SHIFT_RIGHT:
            return builder->CreateLShr(leftValue, rightValue, "shr_temp"); // logical, arithmetic

    }
}

llvm::Value* GenerateLLVM::generateTernary(const ir::TernaryPtr& _pNode) {
    llvm::Function* currentFunction = builder->GetInsertBlock()->getParent();

    llvm::BasicBlock* thenBlock = llvm::BasicBlock::Create(*context, "then", currentFunction);
    llvm::BasicBlock* elseBlock = llvm::BasicBlock::Create(*context, "else", currentFunction);
    llvm::BasicBlock* continueBlock = llvm::BasicBlock::Create(*context, "continue", currentFunction);


    llvm::Value* condition = generateExpression(_pNode->getIf());

    builder->CreateCondBr(condition, thenBlock, elseBlock);

    builder->SetInsertPoint(thenBlock);
    llvm::Value* thenValue = generateExpression(_pNode->getThen());
    builder->CreateBr(continueBlock);

    builder->SetInsertPoint(elseBlock);
    llvm::Value* elseValue = generateExpression(_pNode->getElse());
    builder->CreateBr(continueBlock);

    builder->SetInsertPoint(continueBlock);

    llvm::PHINode* phiNode = builder->CreatePHI(thenValue->getType(), 2, "ternary");
    phiNode->addIncoming(thenValue, thenBlock);
    phiNode->addIncoming(elseValue, elseBlock);
    
    return phiNode;
}

// llvm::Value* GenerateLLVM::generateTypeExpr(const ir::TypeExprPtr& _pNode) {
    
//     llvm::Value* typeValue = generateType(_pNode->getContents());
//     return typeValue;

// }

llvm::Value* GenerateLLVM::generateComponent(const ir::ComponentPtr &_pExpr) {
    switch (_pExpr->getComponentKind()) {
    case ir::Component::ARRAY_PART:
        return generateArrayPartExpr(_pExpr->as<ir::ArrayPartExpr>());
    case ir::Component::STRUCT_FIELD:
        return generateFieldExpr(_pExpr->as<ir::FieldExpr>());
    case ir::Component::MAP_ELEMENT:
        return generateMapElementExpr(_pExpr->as<ir::MapElementExpr>());
    case ir::Component::LIST_ELEMENT:
        return generateListElementExpr(_pExpr->as<ir::ListElementExpr>());
    case ir::Component::REPLACEMENT:
        return generateReplacement(_pExpr->as<ir::Replacement>());
    case ir::Component::RECOGNIZER:
        return generateRecognizerExpr(_pExpr->as<ir::RecognizerExpr>());
    case ir::Component::ACCESSOR:
        return generateAccessorExpr(_pExpr->as<ir::AccessorExpr>());
    }

    return nullptr; 
}

llvm::Value* GenerateLLVM::generateArrayPartExpr(const ir::ArrayPartExprPtr &_pNode) {

    llvm::Value* objectValue = generateExpression(_pNode->getObject());

    if (!objectValue) {
        return nullptr;
    }
    llvm::Type* objectType = generateType(_pNode->getObject()->getType());


    const ir::Collection<ir::Expression>& indices = _pNode->getIndices();
    std::vector<llvm::Value*> indiceValues;
    for (size_t i = 0; i < indices.size(); ++i) {
        llvm::Value* index = generateExpression(indices.get(i));
        if (!index) {
            return nullptr;
        }
        indiceValues.push_back(index);
    }

    return builder->CreateGEP(objectType, objectValue, indiceValues);
}

llvm::Value* GenerateLLVM::generateFieldExpr(const ir::FieldExprPtr& _pNode) {
    llvm::Value* objectValue = generateExpression(_pNode->getObject());
    if (!objectValue)
        return nullptr;

    llvm::Type* objectType = generateType(_pNode->getObject()->getType());

    const auto& fieldsByName = _pNode->getObject()->getType()->as<ir::StructType>()->getNamesOrd(); 
    auto fieldIndex = fieldsByName->findByNameIdx(_pNode->getFieldName());

    return builder->CreateStructGEP(objectType, objectValue, fieldIndex);
}

llvm::Value* GenerateLLVM::generateMapElementExpr(const ir::MapElementExprPtr& _pNode) {
    llvm::Value* objectValue = generateExpression(_pNode->getObject());
    if (!objectValue)
        return nullptr;

    llvm::Value* indexValue = generateExpression(_pNode->getIndex());
    if (!indexValue)
        return nullptr;

    llvm::Type* objectType = generateType(_pNode->getObject()->getType());

    return builder->CreateGEP(objectType, objectValue, indexValue);
}

llvm::Value* GenerateLLVM::generateListElementExpr(const ir::ListElementExprPtr& _pNode) {
    llvm::Value* objectValue = generateExpression(_pNode->getObject());
    if (!objectValue)
        return nullptr;

    llvm::Value* indexValue = generateExpression(_pNode->getIndex());
    if (!indexValue)
        return nullptr;

    llvm::Type* objectType = generateType(_pNode->getObject()->getType());

    return builder->CreateGEP(objectType, objectValue, indexValue);
}

llvm::Value* GenerateLLVM::generateReplacement(const ir::ReplacementPtr& _pNode) { // ya tut

    if(_pNode->getNewValues()->getConstructorKind() != ir::Constructor::ARRAY_ITERATION || !_pNode->getType())
        return nullptr;
    
    llvm::Value* objectValue = generateExpression(_pNode->getObject());
    if(!objectValue)
        return nullptr;
    llvm::Value* newValues = generateConstructor(_pNode->getNewValues());
    if(!newValues)
        return objectValue;

    ir::ArrayIteration& iteration = *_pNode->getNewValues()->as<ir::ArrayIteration>();
    const ir::NamedValues& iterators = iteration.getIterators();
    std::vector<llvm::Value*> iteratorValues;
    for (size_t i = 0; i < iterators.size(); ++i) {
        llvm::Value* iterator = generateNamedValue(iterators.get(i));
        if(!iterator)
            return nullptr;
        iteratorValues.push_back(iterator);
    }
    llvm::Type* objectType = generateType(_pNode->getObject()->getType()->as<ir::ArrayType>()->getRootType());
    std::list<std::pair<llvm::Value*, llvm::Value*>> cases;

    for (size_t i = 0; i< iteration.size(); ++i) 
        cases.push_back({generateExpression(na::resolveCase(iteration.getIterators(), iteration.get(i)->getConditions())),
        generateExpression(iteration.get(i)->getExpression())});
    
    llvm::Value* defaultValue = nullptr;
    
    if(!iteration.getDefault()) {
        defaultValue = generateExpression(_pNode->getObject()->as<ir::ArrayPartExpr>());
        // dodelat
    }
    llvm::Value* resultValue = nullptr;
    for (const auto& casePair: cases) {
        llvm::Value* cond = casePair.first;
        llvm::Value* value = casePair.second;
        resultValue = builder->CreateSelect(cond, value, defaultValue); // ya tut (dodelat)
    }

}

llvm::Value* GenerateLLVM::generateRecognizerExpr(const ir::RecognizerExprPtr& _pNode) { //dodelat
    return nullptr;
}

llvm::Value* GenerateLLVM::generateAccessorExpr(const ir::AccessorExprPtr& _pNode) { //dodelat
    return nullptr;
}

llvm::Value* GenerateLLVM::generateLambda(const ir::LambdaPtr& _pNode) { //dodelat
    return nullptr;
}

llvm::Value* GenerateLLVM::generateBinder(const ir::BinderPtr& _pNode) { // dodelat

    llvm::Value* predicateValue = generateExpression(_pNode->getPredicate());
    if (!predicateValue)
        return nullptr;
    llvm::Type* predicateTypу = generateType(_pNode->getPredicate()->getType());
    const ir::Collection<ir::Expression>& args = _pNode->getArgs();
    std::vector<llvm::Value*> argValues;
    for (size_t i = 0; i < args.size())
}




llvm::Value* GenerateLLVM::generateWild(const ir::WildPtr& _pNode){  //dodelat
    
}


// llvm::Value* GenerateLLVM::generateFormula(const ir::FormulaPtr &_pNode) {        // dodelat
//     if (_pNode->getQuantifier() == ir::Formula::EXISTENTIAL) {
//         llvm::Function* currentFunction = builder->GetInsertBlock()->getParent();

//         // llvm::BasicBlock* entryBlock = llvm::BasicBlock::Create(*context, "entry", currentFunction);

//         switch (_pNode->getQuantifier())
//         {
//         case ir::Formula::EXISTENTIAL:

//             llvm::BasicBlock* entryBlock = llvm::BasicBlock::Create(*context, "entry", currentFunction);

//             llvm::Value* subformula = generateExpression(_pNode->getSubformula());
//             /* code */
//             break;
        
//         default:
//             break;
//         }
//     }

//     VISITOR_ENTER(Formula, _node);

//     m_os << (_node->getQuantifier() == ir::Formula::EXISTENTIAL
//         ? L"EXISTS (" : L"FORALL (");

//     VISITOR_TRAVERSE_COL(NamedValue, FormulaBoundVariable, _node->getBoundVariables());
//     m_os << L"): ";

//     VISITOR_TRAVERSE(Expression, Subformula, _node->getSubformula(), _node, Formula, setSubformula);
//     VISITOR_EXIT();
// }


// llvm::Value* GenerateLLVM::generateFormulaCall(const ir::FormulaCallPtr &_node) {  //dodelat
//     m_os << m_context.nameGenerator().getFormulaName(_node);

//     if (_node->getArgs().empty())
//         return true;

//     m_os << L"(";
//     const bool bResult = Visitor::traverseFormulaCall(_node);
//     m_os << L")";

//     return bResult;
// }

llvm::Value* GenerateLLVM::generateConstructor(const ir::ConstructorPtr& _pNode) {
    switch (_pNode -> getConstructorKind()) {
    case ir::Constructor::STRUCT_FIELDS:
        return generateStructConstructor(_pNode->as<ir::StructConstructor>());
    case ir::Constructor::ARRAY_ELEMENTS:
        return generateArrayConstructor(_pNode->as<ir::ArrayConstructor>());
    case ir::Constructor::SET_ELEMENTS:
        return generateSetConstructor(_pNode->as<ir::SetConstructor>());
    case ir::Constructor::MAP_ELEMENTS:
        return generateMapConstructor(_pNode->as<ir::MapConstructor>());
    case ir::Constructor::LIST_ELEMENTS:
        return generateListConstructor(_pNode->as<ir::ListConstructor>());
    case ir::Constructor::ARRAY_ITERATION:
        return generateArrayIteration(_pNode->as<ir::ArrayIteration>());
    case ir::Constructor::UNION_CONSTRUCTOR:
        return generateUnionConstructor(_pNode->as<ir::UnionConstructor>());
    }
}

llvm::Value* GenerateLLVM::generateStructConstructor(const ir::StructConstructorPtr &_pNode) {
    std::string structName = "Some name"; // ???

    const ir::Collection<ir::StructFieldDefinition, ir::Constructor>& fields = *_pNode;
    std::vector<llvm::Value*> fieldValues;
    std::vector<llvm::Type*> fieldTypes;

    for (size_t i = 0; i<fields.size();++i){
        llvm::Type* filedType = generateType(fields.get(i)->getValue()->getType());
        fieldTypes.push_back(filedType);
    }

    llvm::StructType* structType = llvm::StructType::create(*context, structName);
    structType->setBody(fieldTypes, false);

    llvm::Value* structAlloc = builder->CreateAlloca(structType);
    return structAlloc;


}
llvm::Value* GenerateLLVM::generateStructFieldDefinition(const ir::StructFieldDefinitionPtr &_pNode) {
    //std::wstring fieldName = _pNode->getName();
    llvm::Value* fieldValue = generateExpression(_pNode->getValue());
    return fieldValue;
}



llvm::Type* GenerateLLVM::generateType(const ir::TypePtr &_pNode) {
    switch (_pNode->getKind()) {
    case ir::Type::TYPE:
        return generateTypeType(_pNode->as<ir::TypeType>());
    case ir::Type::ENUM:
        return generateEnumType(_pNode->as<ir::EnumType>());
    case ir::Type::STRUCT:
        return generateStructType(_pNode->as<ir::StructType>());
    case ir::Type::UNION:
        return generateUnionType(_pNode->as<ir::UnionType>());
    case ir::Type::ARRAY:
        return generateArrayType(_pNode->as<ir::ArrayType>());
    case ir::Type::SET:
        return generateSetType(_pNode->as<ir::SetType>());
    case ir::Type::MAP:
        return generateMapType(_pNode->as<ir::MapType>());
    case ir::Type::LIST:
        return generateListType(_pNode->as<ir::ListType>());
    case ir::Type::SUBTYPE:
        return generateSubtype(_pNode->as<ir::Subtype>());
    case ir::Type::RANGE:
        return generateRange(_pNode->as<ir::Range>());
    case ir::Type::PREDICATE:
        return generatePredicateType(_pNode->as<ir::PredicateType>());
    case ir::Type::PARAMETERIZED:
        return generateParameterizedType(_pNode->as<ir::ParameterizedType>());
    case ir::Type::NAMED_REFERENCE:
        return generateNamedReferenceType(_pNode->as<ir::NamedReferenceType>());
    case ir::Type::REFERENCE:
        return generateRefType(_pNode->as<ir::RefType>());
    }
}

llvm::Type *GenerateLLVM::generateTypeType(const ir::TypeTypePtr &_pNode) {
    llvm::Type *type = generateTypeDeclaration(_pNode->getDeclaration());
    return type;
}

llvm::Type *GenerateLLVM::generateEnumValue(const ir::EnumValuePtr &enumValue)
{

    llvm::Type* enumValueType = generateType(enumValue->getType());

    return enumValueType;
}

llvm::Type *GenerateLLVM::generateEnumType(const ir::EnumTypePtr &_pNode)
{
    const ir::Collection<ir::EnumValue> &values = _pNode->getValues();
    std::vector<llvm::Type *> valueTypes;
    for (size_t i = 0; i < values.size(); ++i)
    {
        ir::EnumValuePtr enumValuePtr = std::make_shared<ir::EnumValue>(values.get(i));
        llvm::Type *valueType = generateEnumValue(enumValuePtr);
        valueTypes.push_back(valueType);
    }

    llvm::StructType* structType = llvm::StructType::create(*context, valueTypes, "EnumType");
    return structType;

}

llvm::Type *GenerateLLVM::generateStructType(const ir::StructTypePtr &_pNode)
{
    const ir::Collection<ir::EnumValue> &values = _pNode->getValues();
    std::vector<llvm::Type *> valueTypes;
    for (size_t i = 0; i < values.size(); ++i)
    {
        llvm::Type *valueType = llvm::Type::getInt32Ty(*context);
        valueTypes.push_back(valueType);
    }

    llvm::StructType* structType = llvm::StructType::create(*context, valueTypes, "EnumType");
    return structType;

}

llvm::Value *GenerateLLVM::generateStructFieldDefinition(const ir::StructFieldDefinitionPtr &_pNode) {
    llvm::Value* fieldValue = generateExpression(_pNode->getValue());

}

