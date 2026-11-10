// Unit 42 of tools/tu_map.py: the constructor of the filtered walk (UnkTU42_Iter_ovb0274.cpp):
// everything cleared except the two default masks, each the complement of a guarded zero
// (two guarded statics apiece), and the active flag set. Names and layouts are placeholders.
#include "types.h"

// A zero whose constructor isn't constexpr, so the static is guarded.
struct UnkWalkStaticZeroA_ovb0274 {
    u32 value;
    UnkWalkStaticZeroA_ovb0274() : value(0) {}
};

struct UnkWalkStaticZeroB_ovb0274 {
    u32 value;
    UnkWalkStaticZeroB_ovb0274() : value(0) {}
};

inline const UnkWalkStaticZeroA_ovb0274& UnkWalkZeroA() {
    static UnkWalkStaticZeroA_ovb0274 s_value asm("g_unk_7101d57ce8");
    return s_value;
}

inline u32 UnkWalkFilterMask() {
    static u32 s_mask asm("g_unk_7101d57cd8") = ~UnkWalkZeroA().value & 0x3f;
    return s_mask;
}

inline const UnkWalkStaticZeroB_ovb0274& UnkWalkZeroB() {
    static UnkWalkStaticZeroB_ovb0274 s_value asm("g_unk_7101d57cc8");
    return s_value;
}

inline u32 UnkWalkKindMask() {
    static u32 s_mask asm("g_unk_7101d57cb8") = ~UnkWalkZeroB().value & 0xff;
    return s_mask;
}

struct UnkWalkInit_ovb0274 {
    u64 field_0x0;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    u32 filter;  // 0x20
    u32 mask;    // 0x24
    bool active;  // 0x28
};

void fn_71000684e4(UnkWalkInit_ovb0274* self) {
    self->field_0x10 = 0;
    self->field_0x18 = 0;
    self->field_0x0 = 0;
    self->field_0x8 = 0;
    self->filter = UnkWalkFilterMask();
    self->mask = UnkWalkKindMask();
    self->active = true;
}
