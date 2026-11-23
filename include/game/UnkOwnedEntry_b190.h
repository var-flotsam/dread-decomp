#pragma once

#include "game/UnkTU2577.h"
#include "types.h"

struct UnkNamedIds;

struct UnkOwnedEntry_b190 {
    union {
        UnkNamedIds* ptr;
    };
    explicit UnkOwnedEntry_b190(UnkNamedIds* p) : ptr(p) {}
    UnkOwnedEntry_b190(UnkOwnedEntry_b190&& o) {
        UnkNamedIds* moved = o.ptr;
        o.ptr = nullptr;
        ptr = moved;
    }
    ~UnkOwnedEntry_b190() {
        UnkNamedIds* old = ptr;
        ptr = nullptr;
        if (old) {
            fn_7100ef1e1c(old);
            operator delete(old);
        }
    }
    UnkOwnedEntry_b190() : ptr(nullptr) {}
};
