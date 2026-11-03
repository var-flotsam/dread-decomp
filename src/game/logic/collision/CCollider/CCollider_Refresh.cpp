// game::logic::collision::CCollider (unit 2503 of tools/tu_map.py, the name from reflection):
// refreshes the collider (virtual slots 0x38, then 0x40 with true) and then each of its shapes
// (the pointer array at +0x168, count at +0x164; virtual slot 0x90). Names and layouts are
// placeholders.
#include "types.h"

struct UnkColliderShape {
    void** vtable;
    void vf_0x90() { reinterpret_cast<void (*)(UnkColliderShape*)>(vtable[0x90 / 8])(this); }
};

struct UnkColliderRefresh {
    void** vtable;
    u8 unk_0x8[0x15c];
    u32 count;                     // 0x164
    UnkColliderShape* shapes[1];   // 0x168

    void vf_0x38() { reinterpret_cast<void (*)(UnkColliderRefresh*)>(vtable[0x38 / 8])(this); }
    void vf_0x40(bool value) { reinterpret_cast<void (*)(UnkColliderRefresh*, bool)>(vtable[0x40 / 8])(this, value); }
};

void fn_7100d37334(UnkColliderRefresh* self) {
    self->vf_0x38();
    self->vf_0x40(true);
    u64 bytes = (u64)self->count * sizeof(void*);
    asm("" : "+r"(bytes));
    for (UnkColliderShape **it = self->shapes, **end = (UnkColliderShape**)((char*)it + bytes); it != end; ++it)
        (*it)->vf_0x90();
}
