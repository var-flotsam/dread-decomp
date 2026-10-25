#pragma once

// A slot holding a refcounted pointer (base::UnkRefCounted: count at +8, deleted through its
// vtable), shared by the generated clone templates (base/UnkCloneHelpers.h) and the hand-written
// files. The pointer is a union member: the original reloads the old pointer after the count
// increment. The name is a placeholder.

#include "base/UnkDestructors.h"
#include "types.h"

namespace base {

struct UnkRefHandle {
    union {
        UnkRefCounted* ptr;
    };
    void Set(UnkRefCounted* p) {
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            if (UnkRefCounted* old = ptr) {
                if (--old->ref_count == 0)
                    delete old;
            }
            ptr = p;
        }
    }
};

}  // namespace base

// Makes `self` refer to the handle of the object `other` refers to (the 0x18-byte handle with the
// vtable g_unk_7101a209e0 that the object's source hands out, created on first use), or to
// nothing. Two identical copies, instantiations of base::refl_AssignHandle generated in
// src/clones/ (tools/tu_map.py puts the first in the CActor unit, the second in the
// CAbilityMultiLockon unit). Both return self.
base::UnkRefHandle* fn_7100234f78(base::UnkRefHandle* self, const base::UnkRefHandle* other);
base::UnkRefHandle* fn_710031d208(base::UnkRefHandle* self, const base::UnkRefHandle* other);
