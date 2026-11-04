// CSamusHangMovementState::CParams: the reflected copy and move constructors (fn_0x30, fn_0x38; identical), the
// counterparts of the copy-assignment in fn_71009a3048.cpp. The base part (vtable g_unk_7101a22cb8) is copied
// first, then the derived vtable (g_unk_7101a22c88) is stored after its first fields and the rest copied; the last word of the segment at
// +0x84 is not copied but reset to -1. Names and layouts are placeholders.
#include "types.h"

extern void* g_unk_7101a22cb8[];
extern void* g_unk_7101a22c88[];

struct UnkHangParams_A4_b72 {
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
    void* field_0x70;
    u32 field_0x78;
    u32 field_0x7c;
    u32 field_0x80;
    u32 field_0x84;
    u32 field_0x88;
    u32 field_0x8c;
    u32 field_0x90;
    f32 field_0x94;
    void* field_0x98;

    __attribute__((always_inline)) void CopyConstruct(const UnkHangParams_A4_b72* o) {
        vtable = reinterpret_cast<char*>(g_unk_7101a22cb8) + 0x10;
        field_0x8 = o->field_0x8;
        // The base's defaulted copy: its fields as one block, without the tail padding the derived class reuses.
        __builtin_memcpy(&field_0x10, &o->field_0x10, 0x1c);
        field_0x2c = o->field_0x2c;
        field_0x30 = o->field_0x30;
        field_0x38 = o->field_0x38;
        field_0x3c = o->field_0x3c;
        field_0x40 = o->field_0x40;
        field_0x44 = o->field_0x44;
        vtable = reinterpret_cast<char*>(g_unk_7101a22c88) + 0x10;
        field_0x48 = o->field_0x48;
        field_0x4c = o->field_0x4c;
        field_0x50 = o->field_0x50;
        field_0x54 = o->field_0x54;
        field_0x58 = o->field_0x58;
        field_0x5c = o->field_0x5c;
        field_0x60 = o->field_0x60;
        field_0x64 = o->field_0x64;
        field_0x68 = o->field_0x68;
        field_0x70 = o->field_0x70;
        field_0x78 = o->field_0x78;
        field_0x7c = o->field_0x7c;
        field_0x80 = o->field_0x80;
        field_0x84 = o->field_0x84;
        field_0x88 = o->field_0x88;
        field_0x8c = o->field_0x8c;
        field_0x90 = o->field_0x90;
        field_0x94 = -1.0f;
        field_0x98 = o->field_0x98;
    }
};
static_assert(sizeof(UnkHangParams_A4_b72) == 0xa0, "layout");

void fn_71009a2e2c(void* dst, const void* src) {
    static_cast<UnkHangParams_A4_b72*>(dst)->CopyConstruct(static_cast<const UnkHangParams_A4_b72*>(src));
}

void fn_71009a2f34(void* dst, const void* src) {
    static_cast<UnkHangParams_A4_b72*>(dst)->CopyConstruct(static_cast<const UnkHangParams_A4_b72*>(src));
}
