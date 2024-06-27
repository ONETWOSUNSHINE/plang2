/// \file base.cpp
///

#include <set>

#include "ir/base.h"
#include "ir/types.h"
#include "ir/declarations.h"
#include "ir/visitor.h"

class ChildrenCollector : public ir::Visitor {
public:
    ChildrenCollector(const ir::NodesPtr _pContainer) :
        m_pContainer(_pContainer), m_bRoot(true)
    {}

    bool visitNode(ir::Node& _node) {
        if (m_bRoot) {
            m_bRoot = false;
            return true;
        }
        auto _nodePtr = std::make_shared<ir::Node>(_node);
        m_pContainer->add(_nodePtr);
        return false;
    }

    ir::NodesPtr run(const ir::NodePtr _pNode) {
        if (_pNode && m_pContainer)
            traverseNode(*_pNode);
        return m_pContainer;
    }

private:
    ir::NodesPtr m_pContainer;
    bool m_bRoot;
};

namespace ir {

NodesPtr Node::getChildren() const {
    return ChildrenCollector(std::make_shared<Nodes>()).run(std::const_pointer_cast<Node>(shared_from_this()));
}

bool Node::_less(const NodePtr& _pLeft, const NodePtr& _pRight) {
    if (_pLeft == _pRight)
        return false;
    return (_pLeft && _pRight) ? *_pLeft < *_pRight : !_pLeft && _pRight;
}

bool Node::_equals(const NodePtr& _pLeft, const NodePtr& _pRight) {
    if (_pLeft == _pRight)
        return true;
    return (_pLeft && _pRight) ? *_pLeft == *_pRight : (bool)_pLeft == (bool)_pRight;
}

bool isTypeVariable(const NamedValuePtr &_pVar) {
    if (!_pVar || !_pVar->getType())
        return false;

    return _pVar->getType()->getKind() == Type::TYPE;
}

TypePtr resolveBaseType(const TypePtr &_pType) {
    if (!_pType)
        return NULL;

    TypePtr pType = _pType;

    while (pType) {
        if (pType->getKind() == Type::NAMED_REFERENCE) {
            NamedReferenceTypePtr pRef = std::dynamic_pointer_cast<NamedReferenceType>(pType);

            if (pRef->getDeclaration() && pRef->getDeclaration()->getType())
                pType = pRef->getDeclaration()->getType();
            else
                break;
        } else if (pType->getKind() == Type::PARAMETERIZED) {
            pType = std::dynamic_pointer_cast<ParameterizedType>(pType)->getActualType();
        } else
            break;
    }

    return pType;
}

bool Label::less(const Node& _other) const {
    if (!Node::equals(_other))
        return Node::less(_other);
    return getName() < ((const Label&)_other).getName();
}

bool Label::equals(const Node& _other) const {
    if (!Node::equals(_other))
        return false;
    return getName() == ((const Label&)_other).getName();
}

bool Statement::less(const Node& _other) const {
    if (!Node::equals(_other))
        return Node::less(_other);
    const Statement& other = (const Statement&)_other;
    if (getKind() != other.getKind())
        return getKind() < other.getKind();
    return _less(getLabel(), other.getLabel());
}

bool Statement::equals(const Node& _other) const {
    if (!Node::equals(_other))
        return false;
    const Statement& other = (const Statement&)_other;
    return getKind() == other.getKind() && _equals(getLabel(), other.getLabel());
}

bool NamedValue::less(const Node& _other) const {
    if (!Node::equals(_other))
        return Node::less(_other);
    const NamedValue& other = (const NamedValue&)_other;
    if (getKind() != other.getKind())
        return getKind() < other.getKind();
    if (getName() != other.getName())
        return getName() < other.getName();
    return _less(getType(), other.getType());
}

bool NamedValue::equals(const Node& _other) const {
    if (!Node::equals(_other))
        return false;
    const NamedValue& other = (const NamedValue&)_other;
    return getKind() == other.getKind()
        && getName() == other.getName()
        && _equals(getType(), other.getType());
}

void Param::updateUsed(Node &_root) {
    struct Enumerator : public Visitor {
        std::set<NamedValuePtr> params;

        virtual bool visitParam(Param &_param) {
            _param.setUsed(false);
            params.insert(std::static_pointer_cast<Param>(_param.shared_from_this()));
            return true;
        }
    };

    struct Updater : public Visitor {
        Enumerator enumerator;

        void run(Node &_root) {
            enumerator.traverseNode(_root);
            traverseNode(_root);
        }

        virtual bool visitVariableReference(VariableReference &_val) {
            if (_val.getTarget() && _val.getTarget()->getKind() == NamedValue::PREDICATE_PARAMETER &&
                enumerator.params.find(_val.getTarget()) != enumerator.params.end()) {
                auto paramPtr = std::dynamic_pointer_cast<Param>(_val.getTarget());
                if (paramPtr) {
                    paramPtr->setUsed(true);
                }
            }
            return true;
        }
    } updater;

    updater.run(_root);
}


}
