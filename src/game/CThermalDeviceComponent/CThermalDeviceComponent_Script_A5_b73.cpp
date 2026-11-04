// CThermalDeviceComponent unit (tools/tu_map.py, unit 1601): runs fn_7100a925a0, then, when the component names
// a script function (+0x570), calls it with the owner (fn_7100000790, "o"). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

void fn_7100000790(const char*, const char*, ...);

struct UnkThermalDevice_A5_b73;
void fn_7100a925a0(UnkThermalDevice_A5_b73* self);

// The script name's string data: the text at +0x10 and a use count at +0x44 (bumped when the text is read).
struct UnkThermalName_A5_b73 {
    u8 pad_0x0[0x10];
    const char* text;
    u8 pad_0x18[0x44 - 0x18];
    s32 uses;
};
typedef s32 __attribute__((may_alias)) UnkAliasS32_A5_b73;

struct UnkThermalDevice_A5_b73 {
    u8 pad_0x0[0x30];
    void* owner;
    u8 pad_0x38[0x570 - 0x38];
    UnkThermalName_A5_b73* script;
};

void fn_7100a92b2c(UnkThermalDevice_A5_b73* self) {
    fn_7100a925a0(self);
    if ((u64)self->script == (u64)&g_unk_7101d47108->empty)
        return;
    ++*reinterpret_cast<UnkAliasS32_A5_b73*>(&self->script->uses);
    const char* text = self->script->text;
    fn_7100000790(text ? text : "", "o", self->owner);
}
