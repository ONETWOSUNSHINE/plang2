/// \file parser_context.cpp
///

#include <stdarg.h>

#include "parser_context.h"
#include "ir/numbers.h"
#include "ir/builtins.h"
#include "typecheck.h"

using namespace lexer;
using namespace ir;

Context::Context(const ModulePtr& _pModule) {
    for (auto i: _pModule->getPredicates())
        addPredicate(i);

    for (auto i: _pModule->getVariables())
        addVariable(i->getVariable());

    for (auto i: _pModule->getTypes())
        addType(i);

    for (auto i: _pModule->getProcesses())
        addProcess(i);

    for (auto i: _pModule->getFormulas())
        addFormula(i);

    for (auto i: _pModule->getModules()) {
        addModule(i);
        addModuleCtx(i, new Context(i));
    }
}

Context::~Context() {
    if (m_modulesCtxs != NULL)
        for (ModuleContextMap::const_iterator i = m_modulesCtxs->begin(); i != m_modulesCtxs->end(); ++i)
            delete i->second;
    delete m_modulesCtxs;

    delete m_modules;
    delete m_predicates;
    delete m_variables;
    delete m_types;
    delete m_labels;
    delete m_processes;
    delete m_formulas;
    delete m_constructors;
    delete m_pChild;

    if (m_pParent)
        m_pParent->setChild(NULL);
}

void Context::mergeTo(Context *_pCtx, bool _bMergeFailed) {
    if (_pCtx == NULL)
        return;

    mergeChildren(_bMergeFailed);

    if (*_pCtx->m_loc < *m_loc)
        _pCtx->m_loc = m_loc;

    _pCtx->m_messages.splice(_pCtx->m_messages.end(), m_messages);
    _pCtx->m_bFailed = m_bFailed;

    if (m_constructors && (!m_bScope || (m_flags & MERGE_CONSTRUCTORS))) {
        if (!_pCtx->m_constructors)
            std::swap(_pCtx->m_constructors, m_constructors);
        else
            _pCtx->m_constructors->insert(m_constructors->begin(), m_constructors->end());
    }

    if (m_jumps) {
        if (!_pCtx->m_jumps)
            std::swap(_pCtx->m_jumps, m_jumps);
        else
            _pCtx->m_jumps->insert(m_jumps->begin(), m_jumps->end());
    }

    if (m_bScope)
        return;

    if (m_modules) {
        if (!_pCtx->m_modules)
            std::swap(_pCtx->m_modules, m_modules);
        else
            _pCtx->m_modules->insert(m_modules->begin(), m_modules->end());
    }

    if (m_modulesCtxs) {
        if (!_pCtx->m_modulesCtxs)
            std::swap(_pCtx->m_modulesCtxs, m_modulesCtxs);
        else
            _pCtx->m_modulesCtxs->insert(m_modulesCtxs->begin(), m_modulesCtxs->end());
    }

    if (m_predicates) {
        if (!_pCtx->m_predicates)
            std::swap(_pCtx->m_predicates, m_predicates);
        else
            _pCtx->m_predicates->insert(m_predicates->begin(), m_predicates->end());
    }

    if (m_variables) {
        if (!_pCtx->m_variables)
            std::swap(_pCtx->m_variables, m_variables);
        else
            _pCtx->m_variables->insert(m_variables->begin(), m_variables->end());
    }

    if (m_types) {
        if (!_pCtx->m_types)
            std::swap(_pCtx->m_types, m_types);
        else
            _pCtx->m_types->insert(m_types->begin(), m_types->end());
    }

    if (m_labels) {
        if (!_pCtx->m_labels)
            std::swap(_pCtx->m_labels, m_labels);
        else
            _pCtx->m_labels->insert(m_labels->begin(), m_labels->end());
    }

    if (m_processes) {
        if (!_pCtx->m_processes)
            std::swap(_pCtx->m_processes, m_processes);
        else
            _pCtx->m_processes->insert(m_processes->begin(), m_processes->end());
    }

    if (m_formulas) {
        if (!_pCtx->m_formulas)
            std::swap(_pCtx->m_formulas, m_formulas);
        else
            _pCtx->m_formulas->insert(m_formulas->begin(), m_formulas->end());
    }
}

void Context::mergeChildren(bool _bMergeFailed) {
    if (!m_pChild)
        return;

    if (_bMergeFailed || !m_pChild->failed())
        m_pChild->mergeTo(this, _bMergeFailed);

    delete m_pChild;
    m_pChild = NULL;
}

void Context::fmtWarning(const wchar_t *_strFmt, ...) {
    va_list ap;
    const size_t bufSize = 1024;
    wchar_t buf[bufSize];

    va_start(ap, _strFmt);
    vswprintf(buf, bufSize, _strFmt, ap);
    va_end(ap);
    m_messages.push_back(StatusMessage(StatusMessage::Warning, *m_loc, buf));
}

void Context::fmtError(const wchar_t *_strFmt, ...) {
    va_list ap;
    const size_t bufSize = 1024;
    wchar_t buf[bufSize];

    va_start(ap, _strFmt);
    vswprintf(buf, bufSize, _strFmt, ap);
    va_end(ap);
    m_messages.push_back(StatusMessage(StatusMessage::Error, *m_loc, buf));
}

Context *Context::createChild(bool _bScope, int _flags) {
    if (m_pChild)
        delete m_pChild;

    m_pChild = new Context(m_loc, _bScope, _flags | (getFlags() & PARSE_INTERNAL_TYPES));
    m_pChild->setParent(this);

    return m_pChild;
}

ir::ModulePtr Context::getModule(const std::wstring &_strName, bool _bTopLevel /* = false */) const {
    if (m_modules) {
        ModuleMap::const_iterator i = m_modules->find(_strName);
        if (i != m_modules->end())
            return i->second;
    }

    return m_pParent && !_bTopLevel ? m_pParent->getModule(_strName) : ir::ModulePtr();
}

void Context::addModule(const ir::ModulePtr &_pModule) {
    if (m_modules == NULL)
        m_modules = new ModuleMap();
    (*m_modules)[_pModule->getName()] = _pModule;
}

Context* Context::getModuleCtx(const std::wstring &_strName) const {
    if (m_modulesCtxs) {
        ModuleContextMap::const_iterator i = m_modulesCtxs->find(_strName);
        if (i != m_modulesCtxs->end())
            return i->second;
    }

    return m_pParent ? m_pParent->getModuleCtx(_strName) : NULL;
}

void Context::addModuleCtx(const ir::ModulePtr &_pModule, Context *_ctx) {
    if (m_pChild == NULL)
        return;

    if (m_modulesCtxs == NULL)
        m_modulesCtxs = new ModuleContextMap();
    (*m_modulesCtxs)[_pModule->getName()] = _ctx;

    if (*m_loc < *m_pChild->m_loc)
        m_loc = m_pChild->m_loc;

    m_messages.splice(m_messages.end(), m_pChild->m_messages);
}

bool Context::getPredicates(const std::wstring &_strName, ir::Predicates &_predicates) const {
    if (m_predicates) {
        std::pair<PredicateMap::iterator, PredicateMap::iterator> bounds = m_predicates->equal_range(_strName);
        for (PredicateMap::iterator i = bounds.first; i != bounds.second; ++i)
            _predicates.add(i->second);
    }

    if (m_pParent)
        m_pParent->getPredicates(_strName, _predicates);

    if (ir::PredicatePtr pPred = ir::Builtins::instance().find(_strName))
        _predicates.add(pPred);

    return !_predicates.empty();
}

ir::PredicatePtr Context::getPredicate(const std::wstring &_strName) const {
    if (m_predicates) {
        PredicateMap::const_iterator i = m_predicates->find(_strName);
        if (i != m_predicates->end())
            return i->second;
    }

    if (m_pParent)
        return m_pParent->getPredicate(_strName);

    return ir::Builtins::instance().find(_strName);
}

bool Context::addPredicate(const ir::PredicatePtr &_pPred) {
    if (m_predicates == NULL)
        m_predicates = new PredicateMap();

    Predicates predicates;
    getPredicates(_pPred->getName(), predicates);

    for (size_t i = 0; i < predicates.size(); ++i) {
        const PredicatePtr& pPred = predicates.get(i);
        if (pPred->getInParams() == _pPred->getInParams() &&
            pPred->getOutParams() == _pPred->getOutParams() &&
            pPred->getBlock())
            return false;
    }

    m_predicates->insert(std::make_pair(_pPred->getName(), _pPred));
    return true;
}

ir::NamedValuePtr Context::getVariable(const std::wstring &_strName, bool _bLocal) const {
    if (m_variables) {
        VariableMap::const_iterator i = m_variables->find(_strName);
        if (i != m_variables->end())
            return i->second;
    }

    return (m_pParent && (!_bLocal || !m_bScope)) ? m_pParent->getVariable(_strName, _bLocal) : ir::NamedValuePtr();
}

bool Context::addVariable(const ir::NamedValuePtr &_pVar) {
    if (m_variables == NULL)
        m_variables = new VariableMap();
    if (getVariable(_pVar->getName(), true))
        return false;
    return m_variables->insert({_pVar->getName(), _pVar}).second;
}

ir::TypeDeclarationPtr Context::getType(const std::wstring &_strName) const {
    if (m_types) {
        TypeMap::const_iterator i = m_types->find(_strName);
        if (i != m_types->end())
            return i->second;
    }

    return m_pParent ? m_pParent->getType(_strName) : ir::TypeDeclarationPtr();
}

bool Context::addType(const ir::TypeDeclarationPtr &_pType) {
    if (m_types == NULL)
        m_types = new TypeMap();

    const ir::TypeDeclarationPtr pDecl = getType(_pType->getName());
    if (pDecl && pDecl->getType() && _pType->getType() &&
        (pDecl->getType()->getKind() != Type::PARAMETERIZED ||
            pDecl->getType().as<ParameterizedType>()->getActualType()) &&
        (_pType->getType()->getKind() != Type::PARAMETERIZED ||
            _pType->getType().as<ParameterizedType>()->getActualType()))
        return false;

    (*m_types)[_pType->getName()] = _pType;
    return true;
}

ir::LabelPtr Context::getLabel(const std::wstring &_strName) const {
    if (m_labels) {
        LabelMap::const_iterator i = m_labels->find(_strName);
        if (i != m_labels->end())
            return i->second;
    }

    return m_pParent ? m_pParent->getLabel(_strName) : ir::LabelPtr();
}

ir::LabelPtr Context::createLabel(const std::wstring &_strName) {
    LabelPtr pLabel = getLabel(_strName);
    return !pLabel ? new Label(_strName) : pLabel;
}

void Context::addLabel(const ir::LabelPtr &_pLabel) {
    if (m_labels == NULL)
        m_labels = new LabelMap();
    (*m_labels)[_pLabel->getName()] = _pLabel;
}

ir::JumpPtr Context::getJump(const std::wstring &_strName) const {
    if (m_jumps) {
        JumpMap::const_iterator i = m_jumps->find(_strName);
        if (i != m_jumps->end())
            return i->second;
    }

    return !m_bScope && m_pParent ? m_pParent->getJump(_strName) : ir::JumpPtr();
}

void Context::addJump(const ir::JumpPtr &_pJump) {
    if (m_jumps == NULL)
        m_jumps = new JumpMap();
    (*m_jumps)[_pJump->getDestination()->getName()] = _pJump;
}

ir::ProcessPtr Context::getProcess(const std::wstring &_strName) const {
    if (m_processes) {
        ProcessMap::const_iterator i = m_processes->find(_strName);
        if (i != m_processes->end())
            return i->second;
    }

    return m_pParent ? m_pParent->getProcess(_strName) : ir::ProcessPtr();
}

void Context::addProcess(const ir::ProcessPtr &_pProcess) {
    if (m_processes == NULL)
        m_processes = new ProcessMap();
    (*m_processes)[_pProcess->getName()] = _pProcess;
}

ir::FormulaDeclarationPtr Context::getFormula(const std::wstring &_strName) const {
    if (m_formulas) {
        FormulaMap::const_iterator i = m_formulas->find(_strName);
        if (i != m_formulas->end())
            return i->second;
    }

    return m_pParent ? m_pParent->getFormula(_strName) : ir::FormulaDeclarationPtr();
}

bool Context::addFormula(const ir::FormulaDeclarationPtr &_pFormula) {
    if (m_formulas == NULL)
        m_formulas = new FormulaMap();
    const FormulaDeclarationPtr pFormula = getFormula(_pFormula->getName());
    if (pFormula && pFormula->getFormula())
        return false;

    (*m_formulas)[_pFormula->getName()] = _pFormula;
    return true;
}

bool Context::getConstructors(const std::wstring &_strName, ir::UnionConstructorDeclarations &_cons) const {
    if (m_constructors) {
        std::pair<ConsMap::iterator, ConsMap::iterator> bounds = m_constructors->equal_range(_strName);
        for (ConsMap::iterator iCons = bounds.first; iCons != bounds.second; ++iCons)
            _cons.add(iCons->second);
    }

    if (m_pParent)
        m_pParent->getConstructors(_strName, _cons);

    return !_cons.empty();
}

bool Context::hasConstructor(const std::wstring &_strName) const {
    ir::UnionConstructorDeclarations cons;
    getConstructors(_strName, cons);
    return !cons.empty();
}

void Context::addConstructor(const ir::UnionConstructorDeclarationPtr &_pCons) {
    if (!m_constructors)
        m_constructors = new ConsMap();
    m_constructors->insert(std::make_pair(_pCons->getName(), _pCons));
}

tc::FreshTypePtr Context::getFreshType(const std::wstring & _strName) {
    if (m_pParent)
        return m_pParent->getFreshType(_strName);

    if (!m_freshTypes)
        m_freshTypes = new FreshTypeMap();

    auto iFresh = m_freshTypes->find(_strName);
    tc::FreshTypePtr pFresh;

    if (iFresh == m_freshTypes->end()) {
        pFresh = new tc::FreshType();
        m_freshTypes->insert({_strName, pFresh});
    } else
        pFresh = iFresh->second;

    return pFresh;
}

bool Context::consume(int _token1, int _token2, int _token3, int _token4) {
    if (::in(m_loc, _token1, _token2, _token3, _token4)) {
        if (m_loc->getKind() != END_OF_FILE)
            ++m_loc;
        return true;
    }

    return false;
}

const std::wstring &Context::scan(int _nScan, int _nGet) {
    Loc locGet;

    for (int i = 0; i < _nScan; ++i, ++m_loc)
        if (i == _nGet)
            locGet = m_loc;

    return locGet->getValue();
}

void Context::skip(int _nSkip) {
    for (int i = 0; i < _nSkip; ++i)
        ++m_loc;
}

int Context::getIntBits() const {
    if (m_pragma.isSet(Pragma::IntBitness))
        return m_pragma.getIntBitness();

    return getParent() ? getParent()->getIntBits() : Number::GENERIC;
}

int Context::getRealBits() const {
    if (m_pragma.isSet(Pragma::RealBitness))
        return m_pragma.getRealBitness();

    return getParent() ? getParent()->getRealBits() : Number::GENERIC;
}

const ir::Overflow &Context::getOverflow() const {
    if (m_pragma.isSet(Pragma::Overflow) || !getParent())
        return m_pragma.overflow();

    return getParent()->getOverflow();
}

int Context::getFlags() const {
    return m_flags;
}

void Context::setFlags(int _flags) {
    m_flags = _flags;
}

std::wostream &operator << (std::wostream &_os, const StatusMessage &_msg) {
    return _os << L":" << _msg.where.getLine() << L":" << _msg.where.getCol()
                << L": " << (_msg.kind == StatusMessage::Warning ? L"Warning: " : L"Error: ")
                << _msg.str << std::endl;
}
