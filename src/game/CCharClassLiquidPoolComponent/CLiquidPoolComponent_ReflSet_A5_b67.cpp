// CCharClassLiquidPoolComponent's unit (tools/tu_map.py): a reflected setter for the file-path id at +0xc18
// (copy-assigned from the argument, as CStrId's assignment on the file-path pool); when the owner (+0x30) has the
// object at +0x228 and the id is not empty, it bumps the counter at +0x44 of the id's data and passes the data's
// string (+0x10) and g_unk_7101d58d10 to that object (fn_7101174314). Answers the empty status {0, 0, 2, 1}. Names
// are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CFilePathStrId.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::UnkStringData;
using base::global::UnkStringPool;
using base::reflection::UnkReflValue;

void fn_7101174314(void* object, const void* path, const CRntString* modifiers);

struct UnkPathData_A5_b67 {
    u8 pad_0x0[0x10];
    u8 path[0x34];
    union {
        s32 count;
    };
};

// CStrId's copy assignment (base/global/CStrId.h) on the file-path pool.
struct UnkPathId_A5_b67 : base::global::UnkFilePathStrIdBase {
    UnkPathId_A5_b67& operator=(const UnkPathId_A5_b67& other) {
        if (data != other.data) {
            UnkStringData* d = other.data;
            UnkStringPool* pool = g_unk_7101d47200;
            if (d != &pool->empty)
                ++d->ref_count;
            if (data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0)
                    fn_71000803a4(pool, data);
            }
            data = d;
        }
        return *this;
    }
};

struct UnkLiquidOwner_A5_b67 {
    u8 pad_0x0[0x228];
    void* object;
};

struct UnkLiquidPool_A5_b67 {
    u8 pad_0x0[0x30];
    UnkLiquidOwner_A5_b67* owner;
    u8 pad_0x38[0xbe0];
    UnkPathId_A5_b67 path;  // 0xc18
};

static inline __attribute__((always_inline)) void* ArgPtr_A5_b67(UnkReflValue* arg) {
    if (!arg->type) {
        void* null = nullptr;
        asm("" : "+r"(null));
        return null;
    }
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

UnkReflValue fn_710090396c(void*, char* obj, UnkReflValue* const* args) {
    UnkLiquidPool_A5_b67* self = *reinterpret_cast<UnkLiquidPool_A5_b67**>(obj + 8);
    const UnkPathId_A5_b67* value = static_cast<const UnkPathId_A5_b67*>(ArgPtr_A5_b67(args[0]));
    self->path = *value;
    if (self->owner) {
        if (void* object = self->owner->object) {
            if (value->data != &g_unk_7101d47200->empty) {
                ++reinterpret_cast<UnkPathData_A5_b67*>(value->data)->count;
                fn_7101174314(object, reinterpret_cast<UnkPathData_A5_b67*>(value->data)->path, &g_unk_7101d58d10);
            }
        }
    }
    return base::reflection::refl_EmptyValue();
}
