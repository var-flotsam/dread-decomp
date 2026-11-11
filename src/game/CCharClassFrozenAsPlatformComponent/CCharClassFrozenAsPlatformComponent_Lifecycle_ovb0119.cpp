// CCharClassFrozenAsPlatformComponent's unit (tools/tu_map.py unit 1261): fn_71007fc9d4, after the
// base step (fn_71011ace94), appends the id table's string id at +0xec8 (the table fn_7100031894
// builds) to the string-id list at +0xd8, and returns the base step's result. Names and layouts are
// placeholders.
#include <new>

#include "base/UnkCloneRecords.h"
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CCharClassComponent.h"
#include "game/CCharClassFrozenComponent.h"
#include "game/UnkStrIdSplit.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkModelEntity;

// The id table (fn_7100031894 builds it; as src/game/CEmmyAIComponent/CEmmyAIComponent_IsStateId_b56.cpp).
struct UnkGameManagerIdTable {
    u8 pad_0x0[0xec8];
    CStrId field_0xec8;
    u8 pad_0xed0[0x2578 - 0xed0];
    UnkGameManagerIdTable() asm("fn_7100031894");
    ~UnkGameManagerIdTable() asm("fn_71000407f0");
};
inline UnkGameManagerIdTable& GameManagerIdTable() {
    static UnkGameManagerIdTable s_table;
    return s_table;
}

struct UnkFrozenAsPlatform_b0119 {
    u8 pad_0x0[0xd8];
    base::UnkStrIdVec ids;  // 0xd8
};

// `arg` (x1) reaches the base step untouched.
bool fn_71007fc9d4(UnkFrozenAsPlatform_b0119* self, void* arg) {
    bool r = fn_71011ace94(reinterpret_cast<UnkModelEntity*>(self), arg);
    self->ids.PushBack(GameManagerIdTable().field_0xec8);
    return r;
}

// CCharClassFrozenAsPlatformComponent's unit (tools/tu_map.py unit 1261): fn_71007fd04c resizes a
// string-id vector (base::UnkStrIdVec) to `count` ids: growing reserves (fn_710000542c) and
// constructs empty ids in place; shrinking releases the ids past the new end. Names are
// placeholders.
void fn_71007fd04c(base::UnkStrIdVec* self, u32 count) {
    if (self->count < count) {
        self->Reserve(count);
        for (u32 i = self->count; i < count; ++i) {
            // An empty id, constructed in place.
            CStrId& id = self->data[i];
            id.data = nullptr;
            if (!g_unk_7101d47108)
                base::global::UnkEnsureStringPool();
            id.data = &g_unk_7101d47108->empty;
        }
    } else if (self->count > count) {
        for (u64 i = count; i < self->count; ++i)
            self->data[i].Reset();
    } else {
        return;
    }
    self->count = count;
}

// CCharClassFrozenAsPlatformComponent's unit (tools/tu_map.py unit 1261): its destructor
// (fn_71007fc850): the string id at +0x108, the string-id vector at +0xd8 (freed through its
// allocator), the string id at +0xd0 and the string at +0xa0 are released, then the base's
// destructor runs (fn_71007ee5bc). Names and layouts are placeholders.
extern char g_unk_7101a74408[];  // the class's vtable (reached through the GOT)

struct UnkFrozenAsPlatformComponent_b0119 {
    char* vtable;
    u8 pad_0x8[0x98];
    u8 field_0xa0[0x30];  // a base::global::CRntString with its storage
    base::UnkStrIdMember field_0xd0;
    base::UnkVector<base::UnkStrIdMember> field_0xd8;
    u8 pad_0x100[0x8];
    base::UnkStrIdMember field_0x108;
};
static_assert(__builtin_offsetof(UnkFrozenAsPlatformComponent_b0119, field_0x108) == 0x108,
              "UnkFrozenAsPlatformComponent_b0119 layout");

void fn_71007fc850(UnkFrozenAsPlatformComponent_b0119* self) {
    self->vtable = g_unk_7101a74408 + 0x10;
    self->field_0x108.~UnkStrIdMemberOf();
    self->field_0xd8.Free();
    self->field_0xd0.~UnkStrIdMemberOf();
    fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(self->field_0xa0));
    fn_71007ee5bc(reinterpret_cast<char*>(self));
}

// CCharClassFrozenAsPlatformComponent's unit (tools/tu_map.py unit 1261): its constructor
// (fn_71007fc694): the base's (fn_71007ee4c8), then the string "DAMAGE" at +0xa0, the empty id at
// +0xd0, a string-id vector at +0xd8 (flags 0xf) reserved for 4, 100.0 at +0x100, the empty id at
// +0x108 and (100, 100) at +0x110 (the class as the constructor sees it). Names and layouts are
// placeholders.
struct UnkFrozenIceBase_b44a2;

struct UnkId_b0119 {
    UnkStringData* data;
};

struct UnkFrozenAsPlatformInit_b0119 {
    char* vtable;
    u8 pad_0x8[0x98];
    u8 field_0xa0[0x30];  // a base::global::CRntString with its storage
    CStrId field_0xd0;
    base::UnkVector<UnkId_b0119> field_0xd8;
    f32 field_0x100;
    u8 pad_0x104[0x4];
    CStrId field_0x108;
    u64 field_0x110;  // two floats

    // Grows the vector at +0xd8: the ids are moved bitwise and the old ones released.
    __attribute__((always_inline)) void ReserveIds(u32 capacity) {
        base::UnkVector<UnkId_b0119>* v = &field_0xd8;
        if (v->capacity >= capacity)
            return;
        auto* ids = static_cast<UnkId_b0119*>(v->allocator->vf_0x40((u64)capacity * sizeof(UnkId_b0119), 8, v->flags));
        if (UnkId_b0119* old = v->data) {
            u64 j = 0;
            for (u32 i = 0; i < v->count; ++i, ++j, ++old) {
                __builtin_memcpy(&ids[j], old, sizeof(UnkId_b0119));
                UnkStringPool* pool = g_unk_7101d47108;
                old->data = &pool->empty;
                if ((pool = g_unk_7101d47108)) {
                    if (old->data != &pool->empty) {
                        --old->data->ref_count;
                        if (old->data->ref_count == 0) {
                            fn_71000803a4(pool, old->data);
                            pool = g_unk_7101d47108;
                        }
                    }
                    old->data = &pool->empty;
                }
            }
            v->allocator->vf_0x48(v->data);
        }
        v->data = ids;
        v->capacity = capacity;
    }
};

void fn_71007fc694(UnkFrozenAsPlatformInit_b0119* self) {
    fn_71007ee4c8(reinterpret_cast<UnkFrozenIceBase_b44a2*>(self));
    self->vtable = g_unk_7101a74408 + 0x10;
    new (self->field_0xa0) base::global::CRntString("DAMAGE");
    new (&self->field_0xd0) CStrId(g_unk_7101d470e0);
    new (&self->field_0xd8) base::UnkVector<UnkId_b0119>(0xf);
    self->ReserveIds(4);
    self->field_0x100 = 100.0f;
    new (&self->field_0x108) CStrId(g_unk_7101d470e0);
    self->field_0x110 = 0x42c8000042c80000;
}
