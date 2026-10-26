#pragma once

// Types shared by the reflected copy and move constructors of CAttack's subclasses (the class
// names come from reflection, src/reflection/). Each reflected class has a copy constructor
// (descriptor slot 0x30) and a move constructor (0x38); for these classes both are the
// implicit ones, and their members have no separate moves, so the two are identical.

#include "base/UnkVector.h"
#include "types.h"

#include <new>

// CAttack (reflection: size 0x28), the root class. Only its layout and vtable are needed.
struct CAttack {
    virtual ~CAttack();
    u64 field_0x8;
    u32 field_0x10;
    bool field_0x14;
    s64 field_0x18;
    u64 field_0x20;
};

struct UnkAttackRefCounted {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Delete();
    // A union member (no type-based alias info): after the increment in the assignment the
    // pointer being replaced is reloaded.
    union {
        s32 ref_count;
    };
};

// Copying starts empty and assigns only when the source holds an object.
struct UnkAttackRefPtr {
    UnkAttackRefCounted* ptr = nullptr;

    UnkAttackRefPtr(const UnkAttackRefPtr& other) {
        if (other.ptr)
            *this = other;
    }

    UnkAttackRefPtr& operator=(const UnkAttackRefPtr& other) {
        UnkAttackRefCounted* p = other.ptr;
        if (p)
            ++p->ref_count;
        if (UnkAttackRefCounted* old = ptr) {
            if (--old->ref_count == 0)
                old->Delete();
        }
        ptr = p;
        return *this;
    }
};

// What a listener's owner points at (+0x10): it hands out the slot holding its shared handle.
struct UnkAttackTarget {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual UnkAttackRefPtr* GetHandleSlot();
};

// The shared handle created for a target that has none yet (vtable 0x7101a209e0).
struct UnkAttackHandle : UnkAttackRefCounted {
    UnkAttackTarget* target;

    UnkAttackHandle(UnkAttackTarget* t) {
        ref_count = 0;
        target = t;
    }
};

// Assigns a ref-counted pointer unless it already holds `p`.
inline void UnkAttackSetRef(UnkAttackRefPtr& ref, UnkAttackRefCounted* p) {
    if (ref.ptr != p) {
        if (p)
            ++p->ref_count;
        if (UnkAttackRefCounted* old = ref.ptr) {
            if (--old->ref_count == 0)
                old->Delete();
        }
        ref.ptr = p;
    }
}

// A polymorphic member (vtable 0x7101a22d48 and others) with a ref-counted owner. Its copy
// constructor is user-written (the words are copied one by one). Assignment takes the source
// owner's target and shares that target's handle, creating it if needed.
struct UnkAttackListener {
    virtual ~UnkAttackListener();
    u64 field_0x8;
    u64 field_0x10;
    UnkAttackRefPtr owner;

    UnkAttackListener(const UnkAttackListener& other)
        : field_0x8(other.field_0x8), field_0x10(other.field_0x10), owner(other.owner) {}

    __attribute__((always_inline)) UnkAttackListener& operator=(const UnkAttackListener& other) {
        __builtin_memcpy(&field_0x8, &other.field_0x8, 0x10);
        UnkAttackRefCounted* handle = nullptr;
        if (other.owner.ptr) {
            auto* target = static_cast<UnkAttackHandle*>(other.owner.ptr)->target;
            if (target) {
                if (UnkAttackRefPtr* slot = target->GetHandleSlot()) {
                    handle = slot->ptr;
                    if (!handle) {
                        handle = new (fn_71000839a0(0x18, 0x3e)) UnkAttackHandle(target);
                        UnkAttackSetRef(*slot, handle);
                    }
                }
            }
        }
        UnkAttackSetRef(owner, handle);
        return *this;
    }
};

// Words that are copied one by one (a member with a user-written copy).
template <int N>
struct UnkAttackWords {
    u32 w[N];

    UnkAttackWords(const UnkAttackWords& other) {
        for (int i = 0; i < N; i++)
            w[i] = other.w[i];
    }
};
