/// \file utils.h
/// Miscellaneous utility functions.

#ifndef AUTOPTR_H_
#define AUTOPTR_H_

#include <string>
#include <map>
#include <stack>
#include <stdint.h>
#include <assert.h>
#include <memory>


template<typename _Obj>
class Auto {
public:
    Auto() : m_pObj(nullptr) {}

    Auto(const Auto& _other) : m_pObj(_other.m_pObj) {}

    template<typename _Other>
    Auto(const Auto<_Other>& _other) : m_pObj(nullptr) {
        if (_Obj* pOther = _other.ptr()) {
            m_pObj = std::dynamic_pointer_cast<_Obj>(_other.m_pObj);
        }
    }

    // No Auto(void *) constructor is provided to prevent accidentally
    // calling delete on stack-allocated non-counted object, etc.
    Auto(std::shared_ptr<const _Obj>& _pObj) : m_pObj(_pObj) {}

    ~Auto() = default;

    template<typename _Other>
    Auto<_Other> as() const {
        return Auto<_Other>(std::dynamic_pointer_cast<_Other>(m_pObj));
    }

    _Obj* ptr() const {
        return m_pObj != nullptr ? m_pObj.get() : nullptr;
    }

    Auto& operator =(const Auto& _other) {
        if (_other.ptr() != ptr()) {
            m_pObj = _other.m_pObj;
        }

        return *this;
    }

    _Obj& operator *() const {
        return *ptr();
    }

    _Obj* operator ->() const {
        return ptr();
    }

    template<typename _Other>
    bool operator <(const Auto<_Other>& _other) const {
        return (void*)ptr() < (void*)_other.ptr();
    }

    template<typename _Other>
    bool operator ==(const Auto<_Other>& _other) const {
        return (void*)ptr() == (void*)_other.ptr();
    }

    template<typename _Other>
    bool operator !=(const Auto<_Other>& _other) const {
        return (void*)ptr() != (void*)_other.ptr();
    }

    bool operator !() const {
        return ptr() == nullptr;
    }

    operator bool() const {
        return ptr() != nullptr;
    }

    void swap(Auto& _other) {
        std::swap(m_pObj, _other.m_pObj);
    }

    template<class>
    friend class Auto;

    friend class Cloner;

private:
    std::shared_ptr<const _Obj> m_pObj;
};

template<typename _Obj>
inline Auto<_Obj> ptr(const _Obj* _pObj) {
    return Auto<_Obj>(std::make_shared<_Obj>(_pObj));
}

template<typename _Obj>
inline Auto<_Obj> ref(const _Obj* _pOb) {
    return Auto<_Obj>(std::shared_ptr<_Obj>(_pObj));
}
template<class _Comparable>
struct PtrLess {
    bool operator()(const Auto<_Comparable>& _pLhs, const Auto<_Comparable>& _pRhs) const { return *_pLhs < *_pRhs; }
};


// Cloner.

class Cloner {
public:
    template<class _Obj>
    Auto<_Obj> get(const _Obj* _pObj, bool _bKeepOriginal = false) {
        if (_pObj == NULL)
            return NULL;

        Cache::iterator iObj = m_cache.find(_getHandle(_pObj));

        if (iObj != m_cache.end())
            return iObj->second;

        if (_bKeepOriginal)
            return _pObj;

        return _pObj->clone(*this).template as<_Obj>();
    }

    ~Cloner() = default;

    template<class _Obj>
    Auto<_Obj> get(const Auto<_Obj>& _pObj, bool _bKeepOriginal = false) {
        return get(_pObj.ptr(), _bKeepOriginal);
    }

    template<class _Obj>
    void alias(const Auto<_Obj>& _pObj, const Auto<_Obj>& _pOther) {
        _mergeHandles(_getHandle(_pObj.ptr()), _getHandle(_pOther.ptr()));
    }

    template<class _Obj>
    void inject(const Auto<_Obj>& _pNew) {
        m_cache[_getHandle(_pNew.ptr())] = _pNew.m_pObj;
    }

    template<class _Obj>
    void inject(const Auto<_Obj>& _pNew, const Auto<_Obj>& _pOld) {
        const int nHandle = _getHandle(_pNew.ptr());
        _mergeHandles(nHandle, _getHandle(_pOld.ptr()));
        m_cache[nHandle] = _pNew.m_pObj;
    }

    template<class _Obj>
    bool isKnown(const Auto<_Obj>& _pObj) const {
        return m_handles.find(_pObj.ptr()) != m_handles.cend();
    }

    template<class _Obj>
    void* allocate(size_t _cSize, const void* _pOriginal);

    template<class _Obj>
    void* allocate(size_t _cSize, std::shared_ptr<const _Obj>& _pOriginal);


    friend void* operator new(size_t, Cloner&, const void*);

private:
    typedef std::map<int, std::shared_ptr<const void>> Cache;
    typedef std::map<const void*, int> Handles;
    typedef std::multimap<int, const void*> Objects;

    Cache m_cache;
    Handles m_handles;
    Objects m_objects;

    int _getHandle(const void* _pObject);
    void _mergeHandles(int _nHandle, int _nOther);
};

void* operator new(size_t _cSize, Cloner& _cloner, const void* _pOriginal);

template<typename _Obj>
inline Auto<_Obj> clone(const _Obj& _obj) {
    Cloner cloner;
    return cloner.get(&_obj);
}

template<typename _Obj>
inline Auto<_Obj> clone(const Auto<_Obj>& _obj) {
    Cloner cloner;
    return cloner.get(_obj);
}
// We need a sequence point between allocation and evaluation of constructor arguments
// in order to cache uninitialized object and prevent infinite recursion on cyclic references.
#define NEW_CLONE(_ORIGINAL, _CLONER, _CTOR) \
    ((_CLONER).allocate(sizeof(*_ORIGINAL), _ORIGINAL), ::new((_CLONER), _ORIGINAL) _CTOR)


#endif /* AUTOPTR_H_ */
