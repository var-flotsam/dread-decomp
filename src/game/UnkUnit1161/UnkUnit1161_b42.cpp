// Unit 1161 (tools/tu_map.py, no name hint). fn_7100745b24: a destructor: the vtable, then the member
// at +0xe8 (fn_7100745804 destroys it) and the array of pointers at +0xb8 (freed when it holds one).
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/math/CVector.h"
#include "types.h"

extern void* g_unk_7101a6da30[];

struct UnkMember_e8_b42;
void fn_7100745804(UnkMember_e8_b42*);

struct UnkHolder_1161_b42 {
    void** vtable;
    u8 pad_0x8[0xb0];
    base::UnkVector<void*> items;  // 0xb8
    u8 pad_0xe0[0x8];
    UnkMember_e8_b42* member() { return reinterpret_cast<UnkMember_e8_b42*>(reinterpret_cast<char*>(this) + 0xe8); }
};

// Declared as src/game/CBillboardComponent/CBillboardComponent_b40.cpp does: the destructor of
// that unit's 0x148-byte entries.
struct UnkEntry_bbd {
    u8 data[0x148];
    ~UnkEntry_bbd() asm("fn_7100745b24");
};

// Clang 9 ignores the asm label on a destructor and emits its mangled symbols, so this alias gives
// the definition the address's name (as src/base/res/CRes.cpp does).
#if defined(__clang__) && __clang_major__ <= 9
void fn_7100745b24(UnkEntry_bbd*) __attribute__((alias("_ZN12UnkEntry_bbdD2Ev")));
#endif

UnkEntry_bbd::~UnkEntry_bbd() {
    UnkHolder_1161_b42* self = reinterpret_cast<UnkHolder_1161_b42*>(this);
    self->vtable = &g_unk_7101a6da30[2];
    fn_7100745804(self->member());
    self->items.Free();
}

// fn_7100745f80: a point's world position: its local position (+0x30), moved by the parent's
// translation when it has a parent (+0x8, the matrix rows at +0x38), plus the offset at +0x11c.
struct UnkParent_1161_b42 {
    u8 pad_0x0[0x38];
    f32 m[3][4];  // 0x38
};

struct UnkPoint_1161_b42 {
    u8 pad_0x0[0x8];
    UnkParent_1161_b42* parent;  // 0x8
    u8 pad_0x10[0x20];
    base::math::CVector3D local;  // 0x30
    u8 pad_0x3c[0xe0];
    base::math::CVector3D offset;  // 0x11c
};

// Returned in memory: the vector type has a user-written copy constructor.
struct UnkVec3_1161_b42 {
    f32 x, y, z;
    UnkVec3_1161_b42(f32 x, f32 y, f32 z) : x(x), y(y), z(z) {}
    UnkVec3_1161_b42(const UnkVec3_1161_b42& o) : x(o.x), y(o.y), z(o.z) {}
};

UnkVec3_1161_b42 fn_7100745f80(const UnkPoint_1161_b42* self) {
    f32 x, y, z;
    if (const UnkParent_1161_b42* parent = self->parent) {
        f32 px = parent->m[0][3], py = parent->m[1][3], pz = parent->m[2][3];
        x = px + self->local.x;
        y = py + self->local.y;
        z = pz + self->local.z;
    } else {
        x = self->local.x;
        y = self->local.y;
        z = self->local.z;
    }
    return UnkVec3_1161_b42(x + self->offset.x, y + self->offset.y, z + self->offset.z);
}
