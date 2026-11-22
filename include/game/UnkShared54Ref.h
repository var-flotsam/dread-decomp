#pragma once

#include "game/UnkResourceNode.h"

struct UnkShared54Ref {
    UnkCountedResource* ptr;

    explicit UnkShared54Ref(UnkCountedResource* p) : ptr(nullptr) {
        if (p) {
            ++p->ref_count;
            ptr = p;
        }
    }
    ~UnkShared54Ref();

    UnkShared54Ref& operator=(UnkCountedResource* p) {
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            if (UnkCountedResource* old = ptr) {
                if (--old->ref_count == 0)
                    fn_710001737c(old);
            }
            ptr = p;
        }
        return *this;
    }
};
