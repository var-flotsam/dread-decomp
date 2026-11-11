#include <new>

#include "base/UnkFreedVectorPair.h"
#include "base/UnkMatrix44.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game_s2/UnkItem50.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

// ---- 3cee0
struct UnkVec3 {
    u32 a, b, c;
};
void fn_710007b444(void*, const UnkVec3*);

extern CStrId g_unk_71727c81e8, g_unk_71727c81f0, g_unk_71727c81f8, g_unk_71727c8200;
extern CStrId g_unk_71727c8208, g_unk_71727c8210, g_unk_71727c8218, g_unk_71727c8220;
extern CStrId g_unk_71727c8188, g_unk_71727c8190;

struct UnkMinimapIconOut {
    f32 x, y;
    f32 x1, y1, x2, y2;
    CStrId id;
};

struct UnkMinimapIconObj {
    u8 unk_0x0[0x38];
    UnkMatrix44 xform;  // 0x38
};

struct UnkMinimapIconKind {
    u8 unk_0x0[0x124];
    u32 kind;
};

struct UnkMinimapIconSelf {
    u8 unk_0x0[0x18];
    u8* obj;  // 0x18
    u8 unk_0x20[0x468];
    UnkMinimapIconKind* kind;  // 0x488
};

void fn_7100e3cee0(UnkMinimapIconSelf* self, UnkMinimapIconOut* out) {
    u32 vb = *(u32*)(self->obj + 0x54);
    u32 va = *(u32*)(self->obj + 0x44);
    u32 vc = *(u32*)(self->obj + 0x64);
    UnkVec3 v = {va, vb, vc};
    fn_710007b444(out, &v);
    f32 z = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->obj + 0x38)).m[2];
    bool neg = z < 0.0f;
    f32 x = out->x;
    f32 y = out->y;
    f32 lo = neg ? -300.0f : 150.0f;
    f32 hi = neg ? -150.0f : 300.0f;
    out->x2 = x + hi;
    out->y2 = y + 300.0f;
    out->x1 = x + lo;
    out->y1 = y + 0.0f;
    const CStrId* id;
    switch (self->kind->kind) {
    case 0: id = neg ? &g_unk_71727c81e8 : &g_unk_71727c81f0; break;
    case 1: id = neg ? &g_unk_71727c81f8 : &g_unk_71727c8200; break;
    case 3: id = neg ? &g_unk_71727c8208 : &g_unk_71727c8210; break;
    case 4: id = neg ? &g_unk_71727c8218 : &g_unk_71727c8220; break;
    case 5: id = neg ? &g_unk_71727c8188 : &g_unk_71727c8190; break;
    default: return;
    }
    out->id = *id;
}

// ---- area item
struct UnkReleasedStrIdB {
    UnkStringData* data;
    __attribute__((always_inline)) UnkReleasedStrIdB(UnkReleasedStrIdB&& o) : data(o.data) { o.data = &g_unk_7101d47108->empty; }
    __attribute__((always_inline)) ~UnkReleasedStrIdB() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkItemMoveB {
    void** vtable;
    UnkSub field_0x8;
    u8 field_0x30;
    u8 pad_0x31[3];
    u64_a4 field_0x34;
    u64_a4 field_0x3c;
    u64_a4 field_0x44;

    __attribute__((always_inline)) UnkItemMoveB(UnkItemMoveB&& other) {
        vtable = g_unk_7101cac938 + 2;
        field_0x8.field_0x0 = other.field_0x8.field_0x0;
        field_0x8.field_0x8 = other.field_0x8.field_0x8;
        field_0x8.field_0xc = other.field_0x8.field_0xc;
        field_0x8.field_0x10 = other.field_0x8.field_0x10;
        field_0x8.field_0x14 = other.field_0x8.field_0x14;
        field_0x8.field_0x18 = other.field_0x8.field_0x18;
        field_0x8.field_0x20 = other.field_0x8.field_0x20;
        u8 flag = other.field_0x30;
        __builtin_memset(&other.field_0x8, 0, 16);
        field_0x30 = flag;
        field_0x44 = 0;
        field_0x3c = 0;
        field_0x34 = 0;
    }
};

struct UnkPathMoveB {
    UnkFreedVector<void*> points;
    UnkFreedVector<void*> indices;
    u32 field_0x50, field_0x54, field_0x58, field_0x5c;
    __attribute__((always_inline)) UnkPathMoveB(UnkPathMoveB&& o)
        : points(static_cast<UnkFreedVector<void*>&&>(o.points)),
          indices(static_cast<UnkFreedVector<void*>&&>(o.indices)), field_0x50(o.field_0x50),
          field_0x54(o.field_0x54), field_0x58(o.field_0x58), field_0x5c(o.field_0x5c) {}
};

struct UnkAreaItemMoveB {
    UnkReleasedStrIdB name;
    u32 field_0x8;
    UnkItemMoveB item;
    u8 field_0x60;
    u32 field_0x64, field_0x68, field_0x6c, field_0x70;
    UnkPathMoveB path0;
    UnkPathMoveB path1;

    __attribute__((always_inline)) UnkAreaItemMoveB(UnkAreaItemMoveB&& o)
        : name(static_cast<UnkReleasedStrIdB&&>(o.name)), field_0x8(o.field_0x8),
          item(static_cast<UnkItemMoveB&&>(o.item)), field_0x60(o.field_0x60), field_0x64(o.field_0x64),
          field_0x68(o.field_0x68), field_0x6c(o.field_0x6c), field_0x70(o.field_0x70),
          path0(static_cast<UnkPathMoveB&&>(o.path0)), path1(static_cast<UnkPathMoveB&&>(o.path1)) {}
};

void fn_7100e51078(UnkAreaItemMoveB* self, UnkAreaItemMoveB* other) {
    new (self) UnkAreaItemMoveB(static_cast<UnkAreaItemMoveB&&>(*other));
}
