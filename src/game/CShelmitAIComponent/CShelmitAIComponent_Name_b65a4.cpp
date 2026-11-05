// CShelmitAIComponent's unit (tools/tu_map.py): a name getter. Takes the base component's id
// (fn_710111203c); in mode 1 (+0x14780) it is replaced by the unit's id g_unk_71727071b0. Names and
// layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

extern CStrId g_unk_71727071b0 __attribute__((visibility("hidden")));

struct UnkShelmitAI_b65a4 {
    u8 pad_0x0[0x14780];
    s32 mode;  // 0x14780
};

CStrId fn_710111203c(UnkShelmitAI_b65a4* self);

CStrId fn_71006bc918(UnkShelmitAI_b65a4* self) {
    CStrId id = fn_710111203c(self);
    if (self->mode == 1)
        id = g_unk_71727071b0;
    return id;
}
