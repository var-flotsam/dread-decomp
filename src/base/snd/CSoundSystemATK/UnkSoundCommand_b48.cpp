// CSoundSystemATK unit (tools/tu_map.py): builds a kind-1 command record on the stack (its head
// cleared, then filled from the arguments) and hands it to fn_71000f44cc. Names and layouts are
// placeholders.
#include "types.h"

#pragma pack(push, 4)
struct UnkSoundCommand {
    u32 kind;
    u32 id;
    u32 field_0x8;
    u64 field_0xc;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1c;
    u32 field_0x20;
    u8 payload_0x24[0xe4];
};
#pragma pack(pop)

void fn_71000f44cc(void* self, void* target, UnkSoundCommand* command);

void fn_71000f4488(void* self, void* target, u32 id, u64 a, u32 b, u32 c, u32 d) {
    UnkSoundCommand cmd;
    __builtin_memset(&cmd, 0, 0x24);
    cmd.kind = 1;
    cmd.id = id;
    cmd.field_0xc = a;
    cmd.field_0x14 = b;
    cmd.field_0x18 = c;
    cmd.field_0x1c = d;
    fn_71000f44cc(self, target, &cmd);
}
