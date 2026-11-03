// Deleting destructors of one small class shape found in the CAbilityOpticCamouflage,
// CAbilityShinespark and CAbilitySpeedBooster units (tools/tu_map.py units 432, 433 and 435): the
// members at +0x30..+0x60 are cleared, the class's vtable (g_unk_<addr> + 0x10) is set back, the
// reflected value at +0x18 is destroyed (fn_71000a1c60) and the object freed. Names are placeholders.
#include <new>

#include "base/reflection/UnkReflCast.h"
#include "types.h"

extern char g_unk_7101a22fe8[];
extern char g_unk_7101a234f8[];
extern char g_unk_7101a239c8[];

struct UnkReflValueHolder {
    char* vtable;
    u8 pad_0x8[0x10];
    base::reflection::UnkReflValue value;  // 0x18
    u64 field_0x30;
    u64 field_0x38;
    u64 field_0x40;
    u64 field_0x48;
    u64 field_0x50;
    u64 field_0x58;
};

static inline void DeleteHolder(UnkReflValueHolder* self, char* vtable) {
    self->field_0x50 = 0;
    self->field_0x58 = 0;
    self->field_0x40 = 0;
    self->field_0x48 = 0;
    self->field_0x30 = 0;
    self->field_0x38 = 0;
    self->vtable = vtable + 0x10;
    fn_71000a1c60(&self->value);
    operator delete(self);
}

void fn_710031dd08(UnkReflValueHolder* self) {
    DeleteHolder(self, g_unk_7101a22fe8);
}

void fn_7100332080(UnkReflValueHolder* self) {
    DeleteHolder(self, g_unk_7101a234f8);
}

void fn_710033f458(UnkReflValueHolder* self) {
    DeleteHolder(self, g_unk_7101a239c8);
}
