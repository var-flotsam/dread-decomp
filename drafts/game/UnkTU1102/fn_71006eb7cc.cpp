#include "base/UnkRefHandle.h"
#include "game/UnkActor.h"
#include "types.h"

bool fn_710112a3bc(void* holder);

struct UnkXParasiteEntryHolder_b0252 {
    u8 pad_0x0[0x18];
    UnkActor* actor;  // 0x18
};

struct UnkXParasiteEntryRef_b0252 {
    u8 pad_0x0[0x10];
    UnkXParasiteEntryHolder_b0252* holder;  // 0x10
};

struct UnkXParasiteEntryTable_b0252 {
    UnkXParasiteEntryRef_b0252** data;
    u32 count;
};

struct UnkXParasiteFind_b0252 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x14740 - 0x38];
    UnkXParasiteEntryTable_b0252 entries;  // 0x14740
};
static_assert(__builtin_offsetof(UnkXParasiteFind_b0252, entries) == 0x14740, "placeholder layout");

base::UnkRefHandle* fn_71006eb8cc(base::UnkRefHandle*, const base::UnkRefHandle*);

struct UnkXParasiteResult_b0252 {
    base::UnkRefCounted* ptr;
    ~UnkXParasiteResult_b0252() {
        if (ptr && --ptr->ref_count == 0)
            delete ptr;
    }
};

struct UnkPos3_b0252 {
    f32 x, y, z;
};
static inline UnkPos3_b0252 Pos(const UnkActor* a) { return {a->field_0x44, a->field_0x54, a->field_0x64}; }
static inline UnkPos3_b0252 operator-(const UnkPos3_b0252& a, const UnkPos3_b0252& b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

UnkXParasiteResult_b0252 fn_71006eb7cc(UnkXParasiteFind_b0252* self) {
    UnkXParasiteResult_b0252 result;
    result.ptr = nullptr;
    UnkActor* actor = fn_7100d7c990(self->owner);
    if (actor) {
        UnkXParasiteEntryRef_b0252** it = self->entries.data;
        UnkXParasiteEntryRef_b0252** end = self->entries.count + it;
        f32 best = -1.0f;
        for (; end != it; ++it) {
            UnkXParasiteEntryRef_b0252* entry = *it;
            if (!entry || !entry->holder || !entry->holder->actor)
                continue;
            if (!fn_710112a3bc(entry->holder))
                continue;
            UnkPos3_b0252 a = Pos(actor);
            UnkActor* other = (*it)->holder->actor;
            UnkPos3_b0252 b = Pos(other);
            f32 dx = a.x - b.x;
            f32 dy = a.y - b.y;
            f32 d2 = dx * dx + dy * dy;
            f32 dz = a.z - b.z;
            d2 = d2 + dz * dz;
            if (d2 > best) {
                fn_71006eb8cc(reinterpret_cast<base::UnkRefHandle*>(&result), reinterpret_cast<const base::UnkRefHandle*>(it));
                best = d2;
            }
        }
    }
    return result;
}
