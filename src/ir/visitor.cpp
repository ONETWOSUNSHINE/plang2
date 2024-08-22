#include "ir/visitor.h"

#include <iostream>

using namespace ir;

#define ENTER VISITOR_ENTER
#define EXIT VISITOR_EXIT
#define TRAVERSE VISITOR_TRAVERSE
#define TRAVERSE_COL VISITOR_TRAVERSE_COL

NodePtr Visitor::getParent() {
    if (m_path.empty())
        return nullptr;
    auto i = std::prev(m_path.end());
    if (i == m_path.begin())
        return nullptr;
    return (--i)->pNode;
}

bool Visitor::visitNode(ir::NodePtr &_pNode) {
    return true;
}

bool Visitor::walkUpFromNode(ir::NodePtr &_pNode) {
    return visitNode(_pNode);
}

bool Visitor::traverseNode(const NodePtr &_pNode) {
    switch (_pNode->getNodeKind()) {
        case Node::COLLECTION: {
            TRAVERSE_COL(Node, TopLevel, (Collection<Node> &) _pNode);
            return true;
        }
        case Node::TYPE: {
            auto typePtr = std::static_pointer_cast<Type>(_pNode);
            return traverseType(typePtr);
        }
        case Node::NAMED_VALUE: {
            auto namedValuePtr = std::static_pointer_cast<NamedValue>(_pNode);
            return traverseNamedValue(namedValuePtr);
        }
        case Node::STATEMENT: {
            auto statementPtr = std::static_pointer_cast<Statement>(_pNode);
            return traverseStatement(statementPtr);
        }
        case Node::EXPRESSION: {
            auto expressionPtr = std::static_pointer_cast<Expression>(_pNode);
            return traverseExpression(expressionPtr);
        }
        case Node::MODULE: {
            auto modulePtr = std::static_pointer_cast<Module>(_pNode);
            return traverseModule(modulePtr);
        }
        case Node::CLASS: {
            auto classPtr = std::static_pointer_cast<Class>(_pNode);
            return traverseClass(classPtr);
        }
        case Node::LABEL: {
            auto labelPtr = std::static_pointer_cast<Label>(_pNode);
            return traverseLabel(labelPtr);
        }
        case Node::MESSAGE: {
            auto messagePtr = std::static_pointer_cast<Message>(_pNode);
            return traverseMessage(messagePtr);
        }
        case Node::PROCESS: {
            auto processPtr = std::static_pointer_cast<Process>(_pNode);
            return traverseProcess(processPtr);
        }
        case Node::UNION_CONSTRUCTOR_DECLARATION: {
            auto unionConstructorDeclarationPtr = std::static_pointer_cast<UnionConstructorDeclaration>(_pNode);
            return traverseUnionConstructorDeclaration(unionConstructorDeclarationPtr);
        }
        case Node::ELEMENT_DEFINITION: {
            auto elementDefinitionPtr = std::static_pointer_cast<ElementDefinition>(_pNode);
            return traverseElementDefinition(elementDefinitionPtr);
        }
        case Node::STRUCT_FIELD_DEFINITION: {
            auto structFieldDefinitionPtr = std::static_pointer_cast<StructFieldDefinition>(_pNode);
            return traverseStructFieldDefinition(structFieldDefinitionPtr);
        }
        case Node::ARRAY_PART_DEFINITION: {
            auto arrayPartDefinitionPtr = std::static_pointer_cast<ArrayPartDefinition>(_pNode);
            return traverseArrayPartDefinition(arrayPartDefinitionPtr);
        }
        case Node::SWITCH_CASE: {
            auto switchCasePtr = std::static_pointer_cast<SwitchCase>(_pNode);
            return traverseSwitchCase(switchCasePtr);
        }
        case Node::MESSAGE_HANDLER: {
            auto messageHandlerPtr = std::static_pointer_cast<MessageHandler>(_pNode);
            return traverseMessageHandler(messageHandlerPtr);
        }
    }

    return true;
}

bool Visitor::traverseType(std::shared_ptr<Type> &_pType) {
    switch (_pType->getKind()) {
    case Type::TYPE: {
        auto typePtr = std::static_pointer_cast<TypeType>(_pType);
        return traverseTypeType(typePtr);
    }
    case Type::ENUM: {
        auto enumPtr = std::static_pointer_cast<EnumType>(_pType);
        return traverseEnumType(enumPtr);
    }
    case Type::STRUCT: {
        auto structPtr = std::static_pointer_cast<StructType>(_pType);
        return traverseStructType(structPtr);
    }
    case Type::UNION: {
        auto unionPtr = std::static_pointer_cast<UnionType>(_pType);
        return traverseUnionType(unionPtr);
    }
    case Type::ARRAY: {
        auto arrayPtr = std::static_pointer_cast<ArrayType>(_pType);
        return traverseArrayType(arrayPtr);
    }
    case Type::SET: {
        auto setPtr = std::static_pointer_cast<SetType>(_pType);
        return traverseSetType(setPtr);
    }
    case Type::MAP: {
        auto mapPtr = std::static_pointer_cast<MapType>(_pType);
        return traverseMapType(mapPtr);
    }
    case Type::LIST: {
        auto listPtr = std::static_pointer_cast<ListType>(_pType);
        return traverseListType(listPtr);
    }
    case Type::SUBTYPE: {
        auto subtypePtr = std::static_pointer_cast<Subtype>(_pType);
        return traverseSubtype(subtypePtr);
    }
    case Type::RANGE: {
        auto rangePtr = std::static_pointer_cast<Range>(_pType);
        return traverseRange(rangePtr);
    }
    case Type::PREDICATE: {
        auto predicatePtr = std::static_pointer_cast<PredicateType>(_pType);
        return traversePredicateType(predicatePtr);
    }
    case Type::PARAMETERIZED: {
        auto parameterizedPtr = std::static_pointer_cast<ParameterizedType>(_pType);
        return traverseParameterizedType(parameterizedPtr);
    }
    case Type::NAMED_REFERENCE: {
        auto namedReferencePtr = std::static_pointer_cast<NamedReferenceType>(_pType);
        return traverseNamedReferenceType(namedReferencePtr);
    }
    case Type::REFERENCE: {
        auto refPtr = std::static_pointer_cast<RefType>(_pType);
        return traverseRefType(refPtr);
    }
    }

    ENTER(Type, _pType);
    EXIT();
}

bool Visitor::traverseTypeType(std::shared_ptr<TypeType> &_pType) {
    ENTER(TypeType, _pType);  
    TRAVERSE(TypeDeclaration, TypeTypeDecl, _pType->getDeclaration(), _pType, TypeType, setDeclaration);
    EXIT();
}

bool Visitor::traverseEnumType(std::shared_ptr<EnumType> &_pType) {
    ENTER(EnumType, _pType);
    TRAVERSE_COL(EnumValue, EnumValueDecl, _pType->getValues());
    EXIT();
}

bool Visitor::traverseStructType(std::shared_ptr<StructType> &_pType) {
    ENTER(StructType, _pType);
    TRAVERSE_COL(NamedValue, StructFieldDeclNameOrd, _pType->getNamesOrd());
    TRAVERSE_COL(NamedValue, StructFieldDeclTypeOrd, _pType->getTypesOrd());
    TRAVERSE_COL(NamedValue, StructFieldDeclNameSet, _pType->getNamesSet());
    EXIT();
}

bool Visitor::traverseUnionType(std::shared_ptr<UnionType> &_pType) {
    ENTER(UnionType, _pType);
    TRAVERSE_COL(UnionConstructorDeclaration, UnionConstructorDecl, _pType->getConstructors());
    EXIT();
}

bool Visitor::traverseArrayType(std::shared_ptr<ArrayType> &_pType) {
    ENTER(ArrayType, _pType);
    TRAVERSE(Type, ArrayDimType, _pType->getDimensionType(), _pType, ArrayType, setDimensionType);
    TRAVERSE(Type, ArrayBaseType, _pType->getBaseType(), _pType, DerivedType, setBaseType);
    EXIT();
}

bool Visitor::traverseSetType(std::shared_ptr<SetType> &_pType) {
    ENTER(SetType, _pType);
    TRAVERSE(Type, SetBaseType, _pType->getBaseType(), _pType, DerivedType, setBaseType);
    EXIT();
}

bool Visitor::traverseMapType(std::shared_ptr<MapType> &_pType) {
    ENTER(MapType, _pType);
    TRAVERSE(Type, MapIndexType, _pType->getIndexType(), _pType, MapType, setIndexType);
    TRAVERSE(Type, MapBaseType, _pType->getBaseType(), _pType, DerivedType, setBaseType);
    EXIT();
}

bool Visitor::traverseListType(std::shared_ptr<ListType> &_pType) {
    ENTER(ListType, _pType);
    TRAVERSE(Type, ListBaseType, _pType->getBaseType(), _pType, DerivedType, setBaseType);
    EXIT();
}

bool Visitor::traverseRefType(std::shared_ptr<RefType> &_pType) {
    ENTER(RefType, _pType);
    TRAVERSE(Type, RefBaseType, _pType->getBaseType(), _pType, DerivedType, setBaseType);
    EXIT();
}

bool Visitor::traverseSubtype(std::shared_ptr<Subtype> &_pType) {
    ENTER(Subtype, _pType);
    TRAVERSE(NamedValue, SubtypeParam, _pType->getParam(), _pType, Subtype, setParam);
    TRAVERSE(Expression, SubtypeCond, _pType->getExpression(), _pType, Subtype, setExpression);
    EXIT();
}

bool Visitor::traverseRange(std::shared_ptr<Range> &_pType) {
    ENTER(Range, _pType);
    TRAVERSE(Expression, RangeMin, _pType->getMin(), _pType, Range, setMin);
    TRAVERSE(Expression, RangeMax, _pType->getMax(), _pType, Range, setMax);
    EXIT();
}

bool Visitor::traversePredicateType(std::shared_ptr<PredicateType> &_pType) {
    ENTER(PredicateType, _pType);
    TRAVERSE_COL(Param, PredicateTypeInParam, _pType->getInParams());

    for (size_t i = 0; i < _pType->getOutParams().size(); ++i) {
        auto br = std::make_shared<Branch>(_pType->getOutParams().get(i));

        TRAVERSE(Label, PredicateTypeBranchLabel, br->getLabel(), br, Branch, setLabel);
        TRAVERSE(Formula, PredicateTypeBranchPreCondition, br->getPreCondition(), br, Branch, setPreCondition);
        TRAVERSE(Formula, PredicateTypeBranchPostCondition, br->getPostCondition(), br, Branch, setPostCondition);
        TRAVERSE_COL(Param, PredicateTypeOutParam, *br);
    }

    TRAVERSE(Formula, PredicateTypePreCondition, _pType->getPreCondition(), _pType, PredicateType, setPreCondition);
    TRAVERSE(Formula, PredicateTypePostCondition, _pType->getPostCondition(), _pType, PredicateType, setPreCondition);
    EXIT();
}

bool Visitor::traverseParameterizedType(std::shared_ptr<ParameterizedType> &_pType) {
    ENTER(ParameterizedType, _pType);
    TRAVERSE_COL(NamedValue, ParameterizedTypeParam, _pType->getParams());
    TRAVERSE(Type, ParameterizedTypeBase, _pType->getActualType(), _pType, ParameterizedType, setActualType);
    EXIT();
}

bool Visitor::traverseNamedReferenceType(std::shared_ptr<NamedReferenceType> &_pType) {
    ENTER(NamedReferenceType, _pType);
    TRAVERSE_COL(Expression, NamedTypeArg, _pType->getArgs());
    EXIT();
}

bool Visitor::traverseDerivedType(std::shared_ptr<DerivedType> &_pType) {
    return true;
}

// Named.

bool Visitor::traverseNamedValue(std::shared_ptr<NamedValue> & _pVal) {
    switch (_pVal->getKind()) {
        case NamedValue::ENUM_VALUE: {
            auto enumPtr = std::static_pointer_cast<EnumValue>(_pVal);
            return traverseEnumValue(enumPtr);
            }
        case NamedValue::PREDICATE_PARAMETER: {
            auto paramPtr = std::static_pointer_cast<Param>(_pVal);
            return traverseParam(paramPtr);
            }
        case NamedValue::LOCAL:
        case NamedValue::GLOBAL: {
            auto variablePtr = std::static_pointer_cast<Variable>(_pVal);
            return traverseVariable(variablePtr);
        }
    }

    ENTER(NamedValue, _pVal);
    TRAVERSE(Type, NamedValueType, _pVal->getType(), _pVal, NamedValue, setType);
    EXIT();
}

bool Visitor::traverseEnumValue(std::shared_ptr<EnumValue> &_pVal) {
    ENTER(EnumValue, _pVal);
    if (getRole() != R_EnumValueDecl) {
        TRAVERSE(Type, EnumValueType, _pVal->getType(), _pVal, NamedValue, setType);
    }
    EXIT();
}

bool Visitor::traverseParam(std::shared_ptr<Param> &_pVal) {
    ENTER(Param, _pVal);
    TRAVERSE(Type, ParamType, _pVal->getType(), _pVal, NamedValue, setType);
    EXIT();
}

bool Visitor::traverseVariable(std::shared_ptr<Variable> &_pVal) {
    ENTER(Variable, _pVal);
    TRAVERSE(Type, VariableType, _pVal->getType(), _pVal, NamedValue, setType);
    EXIT();
}

// Expressions.

bool Visitor::traverseExpression(std::shared_ptr<Expression> &_pExpr) {
    switch (_pExpr->getKind()) {
    case Expression::WILD: {
        auto wildPtr = std::static_pointer_cast<Wild>(_pExpr);
        return traverseWild(wildPtr);
    }
    case Expression::LITERAL: {
        auto literalPtr = std::static_pointer_cast<Literal>(_pExpr);
        return traverseLiteral(literalPtr);
    }
    case Expression::VAR: {
        auto variableReferencePtr = std::static_pointer_cast<VariableReference>(_pExpr);
        return traverseVariableReference(variableReferencePtr);
    }
    case Expression::PREDICATE: {
        auto predicateReferencePtr = std::static_pointer_cast<PredicateReference>(_pExpr);
        return traversePredicateReference(predicateReferencePtr);
    }
    case Expression::UNARY: {
        auto unaryPtr = std::static_pointer_cast<Unary>(_pExpr);
        return traverseUnary(unaryPtr);
    }
    case Expression::BINARY: {
        auto binaryPtr = std::static_pointer_cast<Binary>(_pExpr);
        return traverseBinary(binaryPtr);
    }
    case Expression::TERNARY: {
        auto ternaryPtr = std::static_pointer_cast<Ternary>(_pExpr);
        return traverseTernary(ternaryPtr);
    }   
    case Expression::TYPE: {
        auto typeExprPtr = std::static_pointer_cast<TypeExpr>(_pExpr);
        return traverseTypeExpr(typeExprPtr);
    }
    case Expression::COMPONENT: {
        auto componentPtr = std::static_pointer_cast<Component>(_pExpr);
        return traverseComponent(componentPtr);
    }         
    case Expression::FUNCTION_CALL: {
        auto functionCallPtr = std::static_pointer_cast<FunctionCall>(_pExpr);
        return traverseFunctionCall(functionCallPtr);
    }       
    case Expression::FORMULA_CALL: {
        auto formulaCallPtr = std::static_pointer_cast<FormulaCall>(_pExpr);
        return traverseFormulaCall(formulaCallPtr);
    }     
    case Expression::LAMBDA: {
        auto lambdaPtr = std::static_pointer_cast<Lambda>(_pExpr);
        return traverseLambda(lambdaPtr);
    }      
    case Expression::BINDER: {
        auto binderPtr = std::static_pointer_cast<Binder>(_pExpr);
        return traverseBinder(binderPtr);
    }      
    case Expression::FORMULA: {
        auto formulaPtr = std::static_pointer_cast<Formula>(_pExpr);
        return traverseFormula(formulaPtr);
    }
    case Expression::CONSTRUCTOR: {
        auto constructorPtr = std::static_pointer_cast<Constructor>(_pExpr);
        return traverseConstructor(constructorPtr);
    }        
    case Expression::CAST: {
        auto castExprPtr = std::static_pointer_cast<CastExpr>(_pExpr);
        return traverseCastExpr(castExprPtr);
    }
  
    }

    return true;
}

bool Visitor::traverseWild(std::shared_ptr<Wild> &_pExpr) {
    ENTER(Wild, _pExpr);
    EXIT();
}

bool Visitor::traverseLiteral(std::shared_ptr<Literal> &_pExpr) {
    ENTER(Literal, _pExpr);
    EXIT();
}

bool Visitor::traverseVariableReference(std::shared_ptr<VariableReference> &_pExpr) {
    ENTER(VariableReference, _pExpr);
    EXIT();
}

bool Visitor::traversePredicateReference(std::shared_ptr<PredicateReference> &_pExpr) {
    ENTER(PredicateReference, _pExpr);
    EXIT();
}

bool Visitor::traverseUnary(std::shared_ptr<Unary> &_pExpr) {
    ENTER(Unary, _pExpr);
    TRAVERSE(Expression, UnarySubexpression, _pExpr->getExpression(), _pExpr, Unary, setExpression);
    EXIT();
}

bool Visitor::traverseBinary(std::shared_ptr<Binary> &_pExpr) {
    ENTER(Binary, _pExpr);
    TRAVERSE(Expression, BinarySubexpression, _pExpr->getLeftSide(), _pExpr, Binary, setLeftSide);
    TRAVERSE(Expression, BinarySubexpression, _pExpr->getRightSide(), _pExpr, Binary, setRightSide);
    EXIT();
}

bool Visitor::traverseTernary(std::shared_ptr<Ternary> &_pExpr) {
    ENTER(Ternary, _pExpr);
    TRAVERSE(Expression, TernarySubexpression, _pExpr->getIf(), _pExpr, Ternary, setIf);
    TRAVERSE(Expression, TernarySubexpression, _pExpr->getThen(), _pExpr, Ternary, setThen);
    TRAVERSE(Expression, TernarySubexpression, _pExpr->getElse(), _pExpr, Ternary, setElse);
    EXIT();
}

bool Visitor::traverseTypeExpr(std::shared_ptr<TypeExpr> &_pExpr) {
    ENTER(TypeExpr, _pExpr);
    TRAVERSE(Type, TypeExprValue, _pExpr->getContents(), _pExpr, TypeExpr, setContents);
    EXIT();
}

bool Visitor::traverseComponent(std::shared_ptr<Component> &_pExpr) {
    switch (_pExpr->getComponentKind()) {
    case Component::ARRAY_PART: {
        auto arrayPartExprPtr = std::static_pointer_cast<ArrayPartExpr>(_pExpr);
        return traverseArrayPartExpr(arrayPartExprPtr);
    }      
    case Component::STRUCT_FIELD: {
        auto fieldExprPtr = std::static_pointer_cast<FieldExpr>(_pExpr);
        return traverseFieldExpr(fieldExprPtr);
    }        
    case Component::MAP_ELEMENT: {
        auto mapElementExprPtr = std::static_pointer_cast<MapElementExpr>(_pExpr);
        return traverseMapElementExpr(mapElementExprPtr);
    }   
    case Component::LIST_ELEMENT: {
        auto listElementExprPtr = std::static_pointer_cast<ListElementExpr>(_pExpr);
        return traverseListElementExpr(listElementExprPtr);
    }       
    case Component::REPLACEMENT: {
        auto replacementPtr = std::static_pointer_cast<Replacement>(_pExpr);
        return traverseReplacement(replacementPtr);
    }       
    case Component::RECOGNIZER: {
        auto recognizerExprPtr = std::static_pointer_cast<RecognizerExpr>(_pExpr);
        return traverseRecognizerExpr(recognizerExprPtr);
    }     
    case Component::ACCESSOR: {
        auto accessorExprPtr = std::static_pointer_cast<AccessorExpr>(_pExpr);
        return traverseAccessorExpr(accessorExprPtr);
    }
            
    }

    return true;
}

bool Visitor::traverseArrayPartExpr(std::shared_ptr<ArrayPartExpr> &_pExpr) {
    ENTER(ArrayPartExpr, _pExpr);
    TRAVERSE(Expression, ArrayPartObject, _pExpr->getObject(), _pExpr, Component, setObject);
    TRAVERSE_COL(Expression, ArrayPartIndex, _pExpr->getIndices());
    EXIT();
}

bool Visitor::traverseFieldExpr(std::shared_ptr<FieldExpr> &_pExpr) {
    ENTER(FieldExpr, _pExpr);
    TRAVERSE(Expression, FieldObject, _pExpr->getObject(), _pExpr, Component, setObject);
    EXIT();
}

bool Visitor::traverseMapElementExpr(std::shared_ptr<MapElementExpr> &_pExpr) {
    ENTER(MapElementExpr, _pExpr);
    TRAVERSE(Expression, MapElementObject, _pExpr->getObject(), _pExpr, Component, setObject);
    TRAVERSE(Expression, MapElementIndex, _pExpr->getIndex(), _pExpr, MapElementExpr, setIndex);
    EXIT();
}

bool Visitor::traverseListElementExpr(std::shared_ptr<ListElementExpr> &_pExpr) {
    ENTER(ListElementExpr, _pExpr);
    TRAVERSE(Expression, ListElementObject, _pExpr->getObject(), _pExpr, Component, setObject);
    TRAVERSE(Expression, ListElementIndex, _pExpr->getIndex(), _pExpr, ListElementExpr, setIndex);
    EXIT();
}

bool Visitor::traverseReplacement(std::shared_ptr<Replacement> &_pExpr) {
    ENTER(Replacement, _pExpr);
    TRAVERSE(Expression, ReplacementObject, _pExpr->getObject(), _pExpr, Component, setObject);
    TRAVERSE(Constructor, ReplacementValue, _pExpr->getNewValues(), _pExpr, Replacement, setNewValues);
    EXIT();
}

bool Visitor::traverseRecognizerExpr(std::shared_ptr<RecognizerExpr> &_pExpr) {
    ENTER(RecognizerExpr, _pExpr);
    TRAVERSE(Expression, RecognizerExpression, _pExpr->getObject(), _pExpr, Component, setObject);
    EXIT();
}

bool Visitor::traverseAccessorExpr(std::shared_ptr<AccessorExpr> &_pExpr) {
    ENTER(AccessorExpr, _pExpr);
    TRAVERSE(Expression, AccessorExpression, _pExpr->getObject(), _pExpr, Component, setObject);
    EXIT();
}

bool Visitor::traverseFunctionCall(std::shared_ptr<FunctionCall> &_pExpr) {
    ENTER(FunctionCall, _pExpr);
    TRAVERSE(Expression, FunctionCallee, _pExpr->getPredicate(), _pExpr, FunctionCall, setPredicate);
    TRAVERSE_COL(Expression, FunctionCallArgs, _pExpr->getArgs());
    EXIT();
}

bool Visitor::traverseFormulaCall(std::shared_ptr<FormulaCall> &_pExpr) {
    ENTER(FormulaCall, _pExpr);
    TRAVERSE_COL(Expression, FormulaCallArgs, _pExpr->getArgs());
    EXIT();
}

bool Visitor::traverseLambda(std::shared_ptr<Lambda> &_pExpr) {
    ENTER(Lambda, _pExpr);
    auto predicate = std::make_shared<AnonymousPredicate>(_pExpr->getPredicate());
    if (!_traverseAnonymousPredicate(predicate))
        return false;
    EXIT();
}

bool Visitor::traverseBinder(std::shared_ptr<Binder> &_pExpr) {
    ENTER(Binder, _pExpr);
    TRAVERSE(Expression, BinderCallee, _pExpr->getPredicate(), _pExpr, Binder, setPredicate);
    TRAVERSE_COL(Expression, BinderArgs, _pExpr->getArgs());
    EXIT();
}

bool Visitor::traverseFormula(std::shared_ptr<Formula> &_pExpr) {
    ENTER(Formula, _pExpr);
    TRAVERSE_COL(NamedValue, FormulaBoundVariable, _pExpr->getBoundVariables());
    TRAVERSE(Expression, Subformula, _pExpr->getSubformula(), _pExpr, Formula, setSubformula);
    EXIT();
}

bool Visitor::traverseConstructor(std::shared_ptr<Constructor> &_pExpr) {
    switch (_pExpr->getConstructorKind()) {
    case Constructor::STRUCT_FIELDS: {
        auto structConstructorPtr = std::static_pointer_cast<StructConstructor>(_pExpr);
        return traverseStructConstructor(structConstructorPtr);
    }
    case Constructor::ARRAY_ELEMENTS: {
        auto arrayConstructorPtr = std::static_pointer_cast<ArrayConstructor>(_pExpr);
        return traverseArrayConstructor(arrayConstructorPtr);
    }      
    case Constructor::SET_ELEMENTS: {
        auto setConstructorPtr = std::static_pointer_cast<SetConstructor>(_pExpr);
        return traverseSetConstructor(setConstructorPtr);
    }      
    case Constructor::MAP_ELEMENTS: {
        auto mapConstructorPtr = std::static_pointer_cast<MapConstructor>(_pExpr);
        return traverseMapConstructor(mapConstructorPtr);
    }       
    case Constructor::LIST_ELEMENTS: {
        auto listConstructorPtr = std::static_pointer_cast<ListConstructor>(_pExpr);
        return traverseListConstructor(listConstructorPtr);
    }        
    case Constructor::ARRAY_ITERATION: {
        auto arrayIterationPtr = std::static_pointer_cast<ArrayIteration>(_pExpr);
        return traverseArrayIteration(arrayIterationPtr);
    }      
    case Constructor::UNION_CONSTRUCTOR: {
        auto unionConstructorPtr = std::static_pointer_cast<UnionConstructor>(_pExpr);
        return traverseUnionConstructor(unionConstructorPtr);
    }
          
    }

    return true;
}

bool Visitor::traverseStructConstructor(std::shared_ptr<StructConstructor> &_pExpr) {
    ENTER(StructConstructor, _pExpr);
    TRAVERSE_COL(StructFieldDefinition, StructFieldDef, *_pExpr);
    EXIT();
}

bool Visitor::traverseArrayConstructor(std::shared_ptr<ArrayConstructor> &_pExpr) {
    ENTER(ArrayConstructor, _pExpr);
    TRAVERSE_COL(ElementDefinition, ArrayElementDef, *_pExpr);
    EXIT();
}

bool Visitor::traverseSetConstructor(std::shared_ptr<SetConstructor> &_pExpr) {
    ENTER(SetConstructor, _pExpr);
    TRAVERSE_COL(Expression, SetElementDef, *_pExpr);
    EXIT();
}

bool Visitor::traverseMapConstructor(std::shared_ptr<MapConstructor> &_pExpr) {
    ENTER(MapConstructor, _pExpr);
    TRAVERSE_COL(ElementDefinition, MapElementDef, *_pExpr);
    EXIT();
}

bool Visitor::traverseListConstructor(std::shared_ptr<ListConstructor> &_pExpr) {
    ENTER(ListConstructor, _pExpr);
    TRAVERSE_COL(Expression, ListElementDef, *_pExpr);
    EXIT();
}

bool Visitor::traverseArrayIteration(std::shared_ptr<ArrayIteration> &_pExpr) {
    ENTER(ArrayIteration, _pExpr);
    TRAVERSE_COL(NamedValue, ArrayIterator, _pExpr->getIterators());
    TRAVERSE(Expression, ArrayIterationDefault, _pExpr->getDefault(), _pExpr, ArrayIteration, setDefault);
    TRAVERSE_COL(ArrayPartDefinition, ArrayIterationPart, *_pExpr);
    EXIT();
}

bool Visitor::traverseUnionConstructor(std::shared_ptr<UnionConstructor> &_pExpr) {
    ENTER(UnionConstructor, _pExpr);
    TRAVERSE_COL(VariableDeclaration, UnionCostructorVarDecl, _pExpr->getDeclarations());
    TRAVERSE_COL(StructFieldDefinition, UnionCostructorParam, *_pExpr);
    EXIT();
}

bool Visitor::traverseCastExpr(std::shared_ptr<CastExpr> &_pExpr) {
    ENTER(CastExpr, _pExpr);
    TRAVERSE(TypeExpr, CastToType, _pExpr->getToType(), _pExpr, CastExpr, setToType);
    TRAVERSE(Expression, CastParam, _pExpr->getExpression(), _pExpr, CastExpr, setExpression);
    EXIT();
}

// Statements.

bool Visitor::traverseStatement(std::shared_ptr<Statement> &_pStmt) {
    switch (_pStmt->getKind()) {
    case Statement::BLOCK: {
        auto blockPtr = std::static_pointer_cast<Block>(_pStmt);
        return traverseBlock(blockPtr);
    }       
    case Statement::PARALLEL_BLOCK: {
        auto parallelBlockPtr = std::static_pointer_cast<ParallelBlock>(_pStmt);
        return traverseParallelBlock(parallelBlockPtr);
    }      
    case Statement::JUMP: {
        auto jumpPtr = std::static_pointer_cast<Jump>(_pStmt);
        return traverseJump(jumpPtr);
    }        
    case Statement::ASSIGNMENT: {
        auto assignmentPtr = std::static_pointer_cast<Assignment>(_pStmt);
        return traverseAssignment(assignmentPtr);
    }      
    case Statement::MULTIASSIGNMENT: {
        auto multiassignmentPtr = std::static_pointer_cast<Multiassignment>(_pStmt);
        return traverseMultiassignment(multiassignmentPtr);
    }        
    case Statement::CALL: {
        auto callPtr = std::static_pointer_cast<Call>(_pStmt);
        return traverseCall(callPtr);
    }        
    case Statement::SWITCH: {
        auto switchPtr = std::static_pointer_cast<Switch>(_pStmt);
        return traverseSwitch(switchPtr);
    }        
    case Statement::IF: {
        auto ifPtr = std::static_pointer_cast<If>(_pStmt);
        return traverseIf(ifPtr);
    }       
    case Statement::FOR: {
        auto forPtr = std::static_pointer_cast<For>(_pStmt);
        return traverseFor(forPtr);
    }        
    case Statement::WHILE: {
        auto whilePtr = std::static_pointer_cast<While>(_pStmt);
        return traverseWhile(whilePtr);
    }      
    case Statement::BREAK: {
        auto breakPtr = std::static_pointer_cast<Break>(_pStmt);
        return traverseBreak(breakPtr);
    }        
    case Statement::WITH: {
        auto withPtr = std::static_pointer_cast<With>(_pStmt);
        return traverseWith(withPtr);
    }       
    case Statement::RECEIVE: {
        auto receivePtr = std::static_pointer_cast<Receive>(_pStmt);
        return traverseReceive(receivePtr);
    }        
    case Statement::SEND: {
        auto sendPtr = std::static_pointer_cast<Send>(_pStmt);
        return traverseSend(sendPtr);
    }        
    case Statement::TYPE_DECLARATION: {
        auto typeDeclarationPtr = std::static_pointer_cast<TypeDeclaration>(_pStmt);
        return traverseTypeDeclaration(typeDeclarationPtr);
    }        
    case Statement::VARIABLE_DECLARATION: {
        auto variableDeclarationPtr = std::static_pointer_cast<VariableDeclaration>(_pStmt);
        return traverseVariableDeclaration(variableDeclarationPtr);
    }        
    case Statement::FORMULA_DECLARATION: {
        auto formulaDeclarationPtr = std::static_pointer_cast<FormulaDeclaration>(_pStmt);
        return traverseFormulaDeclaration(formulaDeclarationPtr);
    }        
    case Statement::LEMMA_DECLARATION: {
        auto lemmaDeclarationPtr = std::static_pointer_cast<LemmaDeclaration>(_pStmt);
        return traverseLemmaDeclaration(lemmaDeclarationPtr);
    }        
    case Statement::PREDICATE_DECLARATION: {
        auto predicatePtr = std::static_pointer_cast<Predicate>(_pStmt);
        return traversePredicate(predicatePtr);
    }        
    case Statement::VARIABLE_DECLARATION_GROUP: {
        auto variableDeclarationGroupPtr = std::static_pointer_cast<VariableDeclarationGroup>(_pStmt);
        return traverseVariableDeclarationGroup(variableDeclarationGroupPtr);
    }
           
    }

    ENTER(Statement, _pStmt);
    TRAVERSE(Label, StmtLabel, _pStmt->getLabel(), _pStmt, Statement, setLabel);
    EXIT();
}

bool Visitor::traverseBlock(std::shared_ptr<Block> &_pStmt) {
    ENTER(Block, _pStmt);
    TRAVERSE(Label, StmtLabel, _pStmt->getLabel(), _pStmt, Statement, setLabel);
    TRAVERSE_COL(Statement, Stmt, *_pStmt);
    EXIT();
}

bool Visitor::traverseParallelBlock(std::shared_ptr<ParallelBlock> &_pStmt) {
    ENTER(ParallelBlock, _pStmt);
    TRAVERSE(Label, StmtLabel, _pStmt->getLabel(), _pStmt, Statement, setLabel);
    TRAVERSE_COL(Statement, Stmt, *_pStmt);
    EXIT();
}

bool Visitor::traverseJump(std::shared_ptr<Jump> &_pStmt) {
    ENTER(Jump, _pStmt);
    TRAVERSE(Label, StmtLabel, _pStmt->getLabel(), _pStmt, Statement, setLabel);
    EXIT();
}

bool Visitor::traverseAssignment(std::shared_ptr<Assignment> &_pStmt) {
    ENTER(Assignment, _pStmt);
    TRAVERSE(Label, StmtLabel, _pStmt->getLabel(), _pStmt, Statement, setLabel);
    TRAVERSE(Expression, LValue, _pStmt->getLValue(), _pStmt, Assignment, setLValue);
    TRAVERSE(Expression, RValue, _pStmt->getExpression(), _pStmt, Assignment, setExpression);
    EXIT();
}

bool Visitor::traverseMultiassignment(std::shared_ptr<Multiassignment> &_pStmt) {
    ENTER(Multiassignment, _pStmt);
    TRAVERSE(Label, StmtLabel, _pStmt->getLabel(), _pStmt, Statement, setLabel);
    TRAVERSE_COL(Expression, LValue, _pStmt->getLValues());
    TRAVERSE_COL(Expression, RValue, _pStmt->getExpressions());
    EXIT();
}

bool Visitor::traverseCall(std::shared_ptr<Call> &_pStmt) {
    ENTER(Call, _pStmt);
    TRAVERSE(Label, StmtLabel, _pStmt->getLabel(), _pStmt, Statement, setLabel);
    TRAVERSE(Expression, PredicateCallee, _pStmt->getPredicate(), _pStmt, Call, setPredicate);
    TRAVERSE_COL(VariableDeclaration, PredicateVarDecl, _pStmt->getDeclarations());
    TRAVERSE_COL(Expression, PredicateCallArgs, _pStmt->getArgs());

    for (size_t i = 0; i < _pStmt->getBranches().size(); ++i) {

        auto br = std::make_shared<CallBranch>(_pStmt->getBranches().get(i));
        TRAVERSE(Statement, PredicateCallBranchHandler, br->getHandler(), br, CallBranch, setHandler);
        TRAVERSE_COL(Expression, PredicateCallBranchResults, *br);
    }

    EXIT();
}

bool Visitor::traverseSwitch(std::shared_ptr<Switch> &_pStmt) {
    ENTER(Switch, _pStmt);
    TRAVERSE(Label, StmtLabel, _pStmt->getLabel(), _pStmt, Statement, setLabel);
    TRAVERSE(VariableDeclaration, SwitchParamDecl, _pStmt->getParamDecl(), _pStmt, Switch, setParamDecl);
    TRAVERSE(Expression, SwitchArg, _pStmt->getArg(), _pStmt, Switch, setArg);
    TRAVERSE(Statement, SwitchDefault, _pStmt->getDefault(), _pStmt, Switch, setDefault);
    TRAVERSE_COL(SwitchCase, SwitchCase, *_pStmt);
    EXIT();
}

bool Visitor::traverseIf(std::shared_ptr<If> &_pStmt) {
    ENTER(If, _pStmt);
    TRAVERSE(Label, StmtLabel, _pStmt->getLabel(), _pStmt, Statement, setLabel);
    TRAVERSE(Expression, IfArg, _pStmt->getArg(), _pStmt, If, setArg);
    TRAVERSE(Statement, IfBody, _pStmt->getBody(), _pStmt, If, setBody);
    TRAVERSE(Statement, IfElse, _pStmt->getElse(), _pStmt, If, setElse);
    EXIT();
}

bool Visitor::traverseFor(std::shared_ptr<For> &_pStmt) {
    ENTER(For, _pStmt);
    TRAVERSE(Label, StmtLabel, _pStmt->getLabel(), _pStmt, Statement, setLabel);
    TRAVERSE(VariableDeclaration, ForIterator, _pStmt->getIterator(), _pStmt, For, setIterator);
    TRAVERSE(Expression, ForInvariant, _pStmt->getInvariant(), _pStmt, For, setInvariant);
    TRAVERSE(Statement, ForIncrement, _pStmt->getIncrement(), _pStmt, For, setIncrement);
    TRAVERSE(Statement, ForBody, _pStmt->getBody(), _pStmt, For, setBody);
    EXIT();
}

bool Visitor::traverseWhile(std::shared_ptr<While> &_pStmt) {
    ENTER(While, _pStmt);
    TRAVERSE(Label, StmtLabel, _pStmt->getLabel(), _pStmt, Statement, setLabel);
    TRAVERSE(Expression, WhileInvariant, _pStmt->getInvariant(), _pStmt, While, setInvariant);
    TRAVERSE(Statement, WhileBody, _pStmt->getBody(), _pStmt, While, setBody);
    EXIT();
}

bool Visitor::traverseBreak(std::shared_ptr<Break> &_pStmt) {
    ENTER(Break, _pStmt);
    TRAVERSE(Label, StmtLabel, _pStmt->getLabel(), _pStmt, Statement, setLabel);
    EXIT();
}

bool Visitor::traverseWith(std::shared_ptr<With> &_pStmt) {
    ENTER(With, _pStmt);
    TRAVERSE(Label, StmtLabel, _pStmt->getLabel(), _pStmt, Statement, setLabel);
    TRAVERSE_COL(Expression, WithArg, _pStmt->getArgs());
    TRAVERSE(Statement, WithBody, _pStmt->getBody(), _pStmt, With, setBody);
    EXIT();
}

bool Visitor::traverseReceive(std::shared_ptr<Receive> &_pStmt) {
    ENTER(Receive, _pStmt);
    TRAVERSE(Label, StmtLabel, _pStmt->getLabel(), _pStmt, Statement, setLabel);
    TRAVERSE(Expression, ReceiveTimeout, _pStmt->getTimeout(), _pStmt, Receive, setTimeout);
    TRAVERSE(Statement, ReceiveTimeoutHandler, _pStmt->getTimeoutHandler(), _pStmt, Receive, setTimeoutHandler);
    TRAVERSE_COL(MessageHandler, ReceiveHandler, *_pStmt);
    EXIT();
}

bool Visitor::traverseSend(std::shared_ptr<Send> &_pStmt) {
    ENTER(Send, _pStmt);
    TRAVERSE(Label, StmtLabel, _pStmt->getLabel(), _pStmt, Statement, setLabel);
    TRAVERSE_COL(Expression, SendArgs, _pStmt->getArgs());
    EXIT();
}

bool Visitor::traverseTypeDeclaration(std::shared_ptr<TypeDeclaration> &_pStmt) {
    ENTER(TypeDeclaration, _pStmt);
    TRAVERSE(Label, StmtLabel, _pStmt->getLabel(), _pStmt, Statement, setLabel);
    TRAVERSE(Type, TypeDeclBody, _pStmt->getType(), _pStmt, TypeDeclaration, setType);
    EXIT();
}

bool Visitor::traverseVariableDeclaration(std::shared_ptr<VariableDeclaration> &_pStmt) {
    ENTER(VariableDeclaration, _pStmt);
    TRAVERSE(Label, StmtLabel, _pStmt->getLabel(), _pStmt, Statement, setLabel);
    TRAVERSE(Variable, VarDeclVar, _pStmt->getVariable(), _pStmt, VariableDeclaration, setVariable);
    TRAVERSE(Expression, VarDeclInit, _pStmt->getValue(), _pStmt, VariableDeclaration, setValue);
    EXIT();
}

bool Visitor::traverseFormulaDeclaration(std::shared_ptr<FormulaDeclaration> &_pStmt) {
    ENTER(FormulaDeclaration, _pStmt);
    TRAVERSE(Label, StmtLabel, _pStmt->getLabel(), _pStmt, Statement, setLabel);
    TRAVERSE_COL(NamedValue, FormulaDeclParams, _pStmt->getParams());
    TRAVERSE(Type, FormulaDeclResultType, _pStmt->getResultType(), _pStmt, FormulaDeclaration, setResultType);
    TRAVERSE(Expression, FormulaDeclBody, _pStmt->getFormula(), _pStmt, FormulaDeclaration, setFormula);
    TRAVERSE(Expression, FormulaDeclMeasure, _pStmt->getMeasure(), _pStmt, FormulaDeclaration, setMeasure);
    EXIT();
}

bool Visitor::traverseLemmaDeclaration(std::shared_ptr<LemmaDeclaration> &_pStmt) {
    ENTER(LemmaDeclaration, _pStmt);
    TRAVERSE(Label, StmtLabel, _pStmt->getLabel(), _pStmt, Statement, setLabel);
    TRAVERSE(Expression, LemmaDeclBody, _pStmt->getProposition(), _pStmt, LemmaDeclaration, setProposition);
    EXIT();
}

bool Visitor::traversePredicate(std::shared_ptr<Predicate> &_pStmt) {
    ENTER(Predicate, _pStmt);
    TRAVERSE(Label, StmtLabel, _pStmt->getLabel(), _pStmt, Statement, setLabel);

    if (!_traverseAnonymousPredicate(_pStmt))
        return false;

    EXIT();
}

bool Visitor::traverseVariableDeclarationGroup(std::shared_ptr<VariableDeclarationGroup> &_pStmt) {
    ENTER(VariableDeclarationGroup, _pStmt);
    TRAVERSE(Label, StmtLabel, _pStmt->getLabel(), _pStmt, Statement, setLabel);
    TRAVERSE_COL(VariableDeclaration, VarDeclGroupElement, *_pStmt);
    EXIT();
}

// Misc.

bool Visitor::traverseUnionConstructorDeclaration(std::shared_ptr<UnionConstructorDeclaration> &_pCons) {
    ENTER(UnionConstructorDeclaration, _pCons);
    TRAVERSE(Type, UnionConsFields, _pCons->getFields(), _pCons, UnionConstructorDeclaration, setFields);
    EXIT();
}

bool Visitor::traverseStructFieldDefinition(std::shared_ptr<StructFieldDefinition> &_pCons) {
    ENTER(StructFieldDefinition, _pCons);
    TRAVERSE(Expression, StructFieldValue, _pCons->getValue(), _pCons, StructFieldDefinition, setValue);
    EXIT();
}

bool Visitor::traverseElementDefinition(std::shared_ptr<ElementDefinition> &_pCons) {
    ENTER(ElementDefinition, _pCons);
    TRAVERSE(Expression, ElementIndex, _pCons->getIndex(), _pCons, ElementDefinition, setIndex);
    TRAVERSE(Expression, ElementValue, _pCons->getValue(), _pCons, ElementDefinition, setValue);
    EXIT();
}

bool Visitor::traverseArrayPartDefinition(std::shared_ptr<ArrayPartDefinition> &_pCons) {
    ENTER(ArrayPartDefinition, _pCons);
    TRAVERSE_COL(Expression, ArrayPartCond, _pCons->getConditions());
    TRAVERSE(Expression, ArrayPartValue, _pCons->getExpression(), _pCons, ArrayPartDefinition, setExpression);
    EXIT();
}

bool Visitor::traverseLabel(std::shared_ptr<Label> &_pLabel) {
    ENTER(Label, _pLabel);   
    EXIT();
}

bool Visitor::traverseSwitchCase(std::shared_ptr<SwitchCase> &_pCase) {
    ENTER(SwitchCase, _pCase);
    TRAVERSE_COL(Expression, SwitchCaseValue, _pCase->getExpressions());
    TRAVERSE(Statement, SwitchCaseBody, _pCase->getBody(), _pCase, SwitchCase, setBody);
    EXIT();
}

bool Visitor::_traverseAnonymousPredicate(const std::shared_ptr<AnonymousPredicate> &_pDecl) {
    TRAVERSE_COL(Param, PredicateInParam, _pDecl->getInParams());

    for (size_t i = 0; i < _pDecl->getOutParams().size(); ++i) {
 
        auto br = std::make_shared<Branch>(_pDecl->getOutParams().get(i));
 
        TRAVERSE(Label, PredicateBranchLabel, br->getLabel(), br, Branch, setLabel);
        TRAVERSE(Formula, PredicateBranchPreCondition, br->getPreCondition(), br, Branch, setPreCondition);
        TRAVERSE(Formula, PredicateBranchPostCondition, br->getPostCondition(), br, Branch, setPostCondition);
        TRAVERSE_COL(Param, PredicateOutParam, *br);
    }

    TRAVERSE(Formula, PredicatePreCondition, _pDecl->getPreCondition(), _pDecl, AnonymousPredicate, setPreCondition);
    TRAVERSE(Formula, PredicatePostCondition, _pDecl->getPostCondition(), _pDecl, AnonymousPredicate, setPostCondition);
    TRAVERSE(Expression, PredicateMeasure, _pDecl->getMeasure(), _pDecl, AnonymousPredicate, setMeasure);
    TRAVERSE(Block, PredicateBody, _pDecl->getBlock(), _pDecl, AnonymousPredicate, setBlock);

    return true;
}

bool Visitor::_traverseDeclarationGroup(const std::shared_ptr<DeclarationGroup> &_pDecl) {
    TRAVERSE_COL(Predicate, PredicateDecl, _pDecl->getPredicates());
    TRAVERSE_COL(TypeDeclaration, TypeDecl, _pDecl->getTypes());
    TRAVERSE_COL(VariableDeclaration, VarDecl, _pDecl->getVariables());
    TRAVERSE_COL(FormulaDeclaration, FormulaDecl, _pDecl->getFormulas());
    TRAVERSE_COL(LemmaDeclaration, LemmaDecl, _pDecl->getLemmas());
    TRAVERSE_COL(Message, MessageDecl, _pDecl->getMessages());
    TRAVERSE_COL(Process, ProcessDecl, _pDecl->getProcesses());
    return true;
}

bool Visitor::traverseDeclarationGroup(std::shared_ptr<DeclarationGroup> &_pType) {
    return true;
}

bool Visitor::traverseModule(std::shared_ptr<Module> &_pModule) {
    ENTER(Module, _pModule);

    TRAVERSE_COL(Param, ModuleParam, _pModule->getParams());
    TRAVERSE_COL(Module, ModuleDecl, _pModule->getModules());

    if (!_traverseDeclarationGroup(_pModule))
        return false;

    TRAVERSE_COL(Class, ClassDecl, _pModule->getClasses());

    EXIT();
}

bool Visitor::traverseClass(std::shared_ptr<Class> &_pClass) {
    ENTER(Class, _pClass);

    if (!_traverseDeclarationGroup(_pClass))
        return false;

    EXIT();
}

bool Visitor::traverseMessage(std::shared_ptr<Message> &_pMessage) {
    ENTER(Message, _pMessage);
    TRAVERSE_COL(Param, MessageParam, _pMessage->getParams());
    EXIT();
}

bool Visitor::traverseProcess(std::shared_ptr<Process> &_pProcess) {
    ENTER(Process, _pProcess);
    TRAVERSE_COL(Param, ProcessInParam, _pProcess->getInParams());

    for (size_t i = 0; i < _pProcess->getOutParams().size(); ++i) {
        auto br = std::make_shared<Branch>(_pProcess->getOutParams().get(i));

        TRAVERSE(Label, ProcessBranchLabel, br->getLabel(), br, Branch, setLabel);
        TRAVERSE(Formula, ProcessBranchPreCondition, br->getPreCondition(), br, Branch, setPreCondition);
        TRAVERSE(Formula, ProcessBranchPostCondition, br->getPostCondition(), br, Branch, setPostCondition);
        TRAVERSE_COL(Param, ProcessOutParam, *br);
    }

    TRAVERSE(Block, ProcessBody, _pProcess->getBlock(), _pProcess, Process, setBlock);
    EXIT();
}

bool Visitor::traverseMessageHandler(std::shared_ptr<MessageHandler> &_pMessageHandler) {
    ENTER(MessageHandler, _pMessageHandler);
    TRAVERSE(Statement, MessageHandlerBody, _pMessageHandler->getBody(), _pMessageHandler, MessageHandler, setBody);
    EXIT();
}
