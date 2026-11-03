// CChozoCommanderAIComponent's unit (tools/tu_map.py unit 665): when the commander's tunable flag
// (+0x2a of g_unk_71726c91d0) is on and the owner's (+0x30) +0x278 object points at this
// component's part at +0x14bf0 (through its +0x1a0), hands that part to fn_710042eb58. Names are
// placeholders.
#include "base/reflection/UnkReflAttributes.h"
#include "types.h"

extern base::reflection::UnkTunable g_unk_71726c91d0;

struct UnkPart665;
void fn_710042eb58(UnkPart665* part);

struct UnkLink665 {
    u8 pad_0x0[0x1a0];
    UnkPart665* part;  // 0x1a0
};

struct UnkOwner665 {
    u8 pad_0x0[0x278];
    UnkLink665* link;  // 0x278
};

struct UnkTunableFlags665 {
    u8 pad_0x0[0x2a];
    bool field_0x2a;
};

struct UnkComponent665 {
    u8 pad_0x0[0x30];
    UnkOwner665* owner;  // 0x30
};

void fn_7100455660(UnkComponent665* self) {
    if (!reinterpret_cast<const UnkTunableFlags665*>(&g_unk_71726c91d0)->field_0x2a) {
        return;
    }
    UnkLink665* link = self->owner->link;
    if (link == nullptr) {
        return;
    }
    UnkPart665* part = reinterpret_cast<UnkPart665*>(reinterpret_cast<u8*>(self) + 0x14bf0);
    if (link->part == part) {
        fn_710042eb58(part);
    }
}
