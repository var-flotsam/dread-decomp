#pragma once

#include "game/UnkRcObject.h"
#include "types.h"

struct UnkRcRef {
    UnkRcObject* ptr;

    UnkRcRef() : ptr(nullptr) {}
    UnkRcRef(const UnkRcRef& other) {
        __builtin_memset(&ptr, 0, sizeof(ptr));
        UnkRcObject* o = other.ptr;
        if (o) {
            ++o->field_0x8;
            Release();
            ptr = o;
        }
    }
    ~UnkRcRef() { Release(); }

    void Release() {
        UnkRcObject* o = ptr;
        if (o && --o->field_0x8 == 0)
            o->vf_0x10();
    }
};
