#pragma once

// An iterator over an array-like collection, as the reflection collection types use it
// (fn_71002b8e3c, fn_71000bc1b0, fn_71000cd4cc and their copies). It caches the current
// (index, element) pair in a holder that is empty, owns a copy, or refers to the iterator's
// own index and element. Names and roles are placeholders inferred from the code.

#include <new>

#include "base/UnkVector.h"
#include "types.h"

namespace base {

// K is the index type: u32 for arrays, the key (e.g. a string id) for keyed collections.
template <class T, class K = u32>
struct UnkIterValue {
    struct Value {
        K index;
        T element;
    };
    struct Ref {
        const K* index;
        // A union member: the original reloads it after storing through the index.
        union {
            T* element;
        };
    };
    union {
        Value value;
        Ref ref;
    };
    s32 state;  // 0 empty, 1 owns `value`, 2 refers through `ref`

    UnkIterValue() : state(0) {}
    ~UnkIterValue() { Reset(); }

    // Takes over an owned pair, or moves out the pair a reference points to (fn_71000ceb44,
    // fn_71002433a0 and their copies). The reference is read whole before the stores.
    UnkIterValue(UnkIterValue&& other) : state(0) {
        if (other.state == 1) {
            state = 1;
            new (&value) Value(static_cast<Value&&>(other.value));
        } else if (other.state == 2) {
            state = 1;
            Ref r = other.ref;
            new (&value) Value{static_cast<K&&>(*const_cast<K*>(r.index)), static_cast<T&&>(*r.element)};
        }
    }

    // Copy-assigns the pair an iterator refers to (fn_71000ceba4 and its copies).
    UnkIterValue& operator=(const UnkIterValue& other) {
        if (this != &other) {
            Index() = other.Index();
            Element() = other.Element();
        }
        return *this;
    }

    // Trivially destructible elements take a switch, which the original lowers as a range
    // check where the reset is followed by SetRef (fn_71000bc1b0).
    void Reset() {
        if constexpr (__has_trivial_destructor(Value)) {
            switch (state) {
            case 1:
                state = 0;
                break;
            case 2:
                state = 0;
                break;
            }
        } else {
            if (state == 2) {
            } else if (state == 1) {
                value.~Value();
            } else {
                return;
            }
            state = 0;
        }
    }

    void SetRef(const K* index, T* element) {
        Reset();
        state = 2;
        ref.index = index;
        ref.element = element;
    }

    K& Index() { return state == 2 ? *const_cast<K*>(ref.index) : value.index; }
    T& Element() { return state == 2 ? *ref.element : value.element; }

    // Move-assigns the pair an iterator refers to (fn_71002b954c and its copies).
    UnkIterValue& operator=(UnkIterValue&& other) {
        if (this != &other) {
            if (other.state == 1) {
                Index() = other.value.index;
                Element() = static_cast<T&&>(other.value.element);
            } else if (other.state == 2) {
                Index() = *other.ref.index;
                Element() = static_cast<T&&>(*other.ref.element);
            }
        }
        return *this;
    }

    const K& Index() const { return state == 2 ? *ref.index : value.index; }
    const T& Element() const { return state == 2 ? *ref.element : value.element; }
};

template <class T, class K>
bool operator==(const UnkIterValue<T, K>& a, const UnkIterValue<T, K>& b) {
    if (a.Index() != b.Index())
        return false;
    const T* x = a.state == 2 ? a.ref.element : &a.value.element;
    const T* y = b.state == 2 ? b.ref.element : &b.value.element;
    return *x == *y;
}

// The same comparison with the elements copied out first (fn_71000cea70 and its copies; 8-byte
// elements).
template <class T, class K = u32>
bool refl_IterValueEqualByValue(const UnkIterValue<T, K>& a, const UnkIterValue<T, K>& b) {
    if (a.Index() != b.Index())
        return false;
    T x = a.state == 2 ? *a.ref.element : a.value.element;
    T y = b.state == 2 ? *b.ref.element : b.value.element;
    return x == y;
}

// K is the index type, as in UnkIterValue.
template <class T, class K = u32>
struct UnkCollectionIter {
    K index;
    T* element;
    UnkIterValue<T, K> current;

    UnkCollectionIter(K index, T* element) : index(index), element(element) {
        if (element)
            current.SetRef(&this->index, element);
    }

    UnkCollectionIter& operator=(const UnkCollectionIter& other) {
        if (this != &other) {
            index = other.index;
            element = other.element;
            if (element)
                current.SetRef(&index, element);
            else
                current.Reset();
        }
        return *this;
    }
};

// Type-erased helpers of an iterator value's reflected class (CType+0x38, +0x40, +0x48).
template <class T, class K = u32>
void refl_IterValueMove(void* dst, const void* src) {
    new (dst) UnkIterValue<T, K>(static_cast<UnkIterValue<T, K>&&>(*static_cast<UnkIterValue<T, K>*>(const_cast<void*>(src))));
}

template <class T, class K = u32>
void refl_IterValueDestroy(void* obj) {
    static_cast<UnkIterValue<T, K>*>(obj)->~UnkIterValue();
}

template <class T, class K = u32>
void* refl_IterValueCopy(void* dst, const void* src) {
    return &(*static_cast<UnkIterValue<T, K>*>(dst) = *static_cast<const UnkIterValue<T, K>*>(src));
}

// The collection functions a reflected array type points to.
template <class T>
void refl_Begin(const UnkVector<T>* v, UnkCollectionIter<T>* out) {
    *out = UnkCollectionIter<T>(0, v->data);
}

template <class T>
void refl_End(const UnkVector<T>* v, UnkCollectionIter<T>* out) {
    *out = UnkCollectionIter<T>(v->count, v->data + v->count);
}

template <class T>
void refl_Erase(UnkVector<T>* v, const UnkCollectionIter<T>* it) {
    v->EraseAt(it->index);
}

}  // namespace base
