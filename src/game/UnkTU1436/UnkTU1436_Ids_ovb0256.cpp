// Unit 1436 of tools/tu_map.py (no name hint): when the owner's target is in mode 1 its flag is raised, then
// the two string ids at +0x48 and +0x50 are copied from the source's ids at +0x150 and +0x158. Names and
// layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkIdSource_b256 {
    u8 pad_0x0[0x150];
    CStrId first;   // 0x150
    CStrId second;  // 0x158
};

struct UnkFlagTarget_b256 {
    u8 pad_0x0[0x1610];
    struct Info {
        u8 pad_0x0[0xec];
        s32 mode;  // 0xec
    }* info;  // 0x1610
    u8 pad_0x1618[0xcc];
    u8 flag;  // 0x16e4
};

struct UnkOwnerLink_b256 {
    u8 pad_0x0[0x240];
    UnkFlagTarget_b256* target;  // 0x240
};

struct UnkIdHolder_b256 {
    u8 pad_0x0[0x30];
    UnkOwnerLink_b256* owner;  // 0x30
    u8 pad_0x38[8];
    UnkIdSource_b256* source;  // 0x40
    CStrId first;              // 0x48
    CStrId second;             // 0x50
};

void fn_7100955450(UnkIdHolder_b256* self) {
    if (UnkOwnerLink_b256* o = self->owner)
        if (UnkFlagTarget_b256* t = o->target)
            if (t->info && t->info->mode == 1)
                t->flag = 1;
    self->first = self->source->first;
    self->second = self->source->second;
}
