// CSamusMagnetGloveMovementState::CParams: the reflected copy and move constructors (fn_0x30, fn_0x38;
// identical), the counterparts of the assignments in ../CParams_Assign_A4_b72.cpp. The base part (vtable
// g_unk_7101a22cb8) is copied first, then the derived vtable (g_unk_7101a874f0) is stored and the rest copied;
// the word at +0x74 is reset to -1 and the two counted pointers (+0x80, +0x88) are shared. Names and layouts
// are placeholders.
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern void* g_unk_7101a22cb8[];
extern void* g_unk_7101a874f0[];

struct UnkMagnetVec3_ovb0082 {
    f32 x, y, z;
};

struct UnkMagnetParams_ovb0082 {
    void* vtable;
    void* field_0x8;
    void* field_0x10;
    void* field_0x18;
    void* field_0x20;
    u32 field_0x28;
    u32 field_0x2c;
    u32 field_0x30;
    u32 field_0x34;  // not copied
    u32 field_0x38;
    u32 field_0x3c;
    u32 field_0x40;
    u32 field_0x44;
    u32 field_0x48;
    u32 field_0x4c;
    u32 field_0x50;
    u32 field_0x54;
    u32 field_0x58;
    u32 field_0x5c;
    u32 field_0x60;
    u32 field_0x64;
    u32 field_0x68;
    u32 field_0x6c;
    u32 field_0x70;
    f32 field_0x74;
    u32 field_0x78;
    UnkRefObject* field_0x80;
    UnkRefObject* field_0x88;
    u32 field_0x90;
    u32 field_0x94;
    u32 field_0x98;
    UnkMagnetVec3_ovb0082 field_0x9c;

    __attribute__((always_inline)) void CopyConstruct(const UnkMagnetParams_ovb0082* o) {
        vtable = reinterpret_cast<char*>(g_unk_7101a22cb8) + 0x10;
        field_0x8 = o->field_0x8;
        __builtin_memcpy(&field_0x10, &o->field_0x10, 0x1c);
        field_0x2c = o->field_0x2c;
        field_0x30 = o->field_0x30;
        field_0x38 = o->field_0x38;
        field_0x3c = o->field_0x3c;
        field_0x40 = o->field_0x40;
        field_0x44 = o->field_0x44;
        vtable = reinterpret_cast<char*>(g_unk_7101a874f0) + 0x10;
        field_0x48 = o->field_0x48;
        field_0x4c = o->field_0x4c;
        field_0x50 = o->field_0x50;
        field_0x54 = o->field_0x54;
        field_0x58 = o->field_0x58;
        field_0x5c = o->field_0x5c;
        field_0x60 = o->field_0x60;
        field_0x64 = o->field_0x64;
        field_0x68 = o->field_0x68;
        field_0x6c = o->field_0x6c;
        field_0x70 = o->field_0x70;
        field_0x74 = -1.0f;
        field_0x78 = o->field_0x78;
        field_0x80 = nullptr;
        AssignRef(&field_0x80, o->field_0x80);
        field_0x88 = nullptr;
        AssignRef(&field_0x88, o->field_0x88);
        field_0x90 = o->field_0x90;
        field_0x94 = o->field_0x94;
        field_0x98 = o->field_0x98;
        field_0x9c = o->field_0x9c;
    }
};
static_assert(sizeof(UnkMagnetParams_ovb0082) == 0xa8, "layout");

void fn_71009af638(void* dst, const void* src) {
    static_cast<UnkMagnetParams_ovb0082*>(dst)->CopyConstruct(static_cast<const UnkMagnetParams_ovb0082*>(src));
}

void fn_71009af7dc(void* dst, const void* src) {
    static_cast<UnkMagnetParams_ovb0082*>(dst)->CopyConstruct(static_cast<const UnkMagnetParams_ovb0082*>(src));
}
