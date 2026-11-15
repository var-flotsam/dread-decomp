// CTunableAIManager's unit: switches the "emmydetection" effect of the owner's component (+0x280) on or off:
// off when the level is not a real one (below 2 or the maximum), when the owner's component at +0x260 is busy
// (fn_710113d83c), or when a listed object is an Emmy proto AI component, or one that holds a timer or flag
// while its flag at +0x27720 is set. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CFXComponent.h"
#include "game/CGrabComponent.h"
#include "game/UnkTU39.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkListed_ldae6dc : UnkReflObject {
    u8 pad_0x8[0x27720 - 0x8];
    u8 field_0x27720;
    u8 pad_0x27721[0x29418 - 0x27721];
    u8 timer[0x29f26 - 0x29418];  // 0x29418
    u8 field_0x29f26;
};

struct UnkListedHolder_ldae6dc {
    u8 pad_0x0[0x10];
    UnkListed_ldae6dc* object;  // 0x10
};

struct UnkDetectOwner_ldae6dc {
    u8 pad_0x0[0x260];
    void* field_0x260;
    u8 pad_0x268[0x18];
    UnkRinkaControl_ruc* field_0x280;
};

struct UnkDetectList_ldae6dc {
    u8 pad_0x0[8];
    u32 count;  // 0x8
    UnkListedHolder_ldae6dc* entries[1];  // 0xc
} __attribute__((packed));

static bool NoneBlocks(UnkDetectList_ldae6dc* self) {
    UnkListedHolder_ldae6dc** it = self->entries;
    for (UnkListedHolder_ldae6dc** end = it + self->count; it != end; ++it) {
        UnkListedHolder_ldae6dc* entry = *it;
        if (entry && entry->object) {
            if (fn_710009d50c(entry->object->vf_0x0(), fn_71004fa488()))
                return false;
        }
        UnkListed_ldae6dc* object = entry ? entry->object : nullptr;
        if (fn_710002fb70(object->timer) || object->field_0x29f26) {
            if ((*it)->object->field_0x27720)
                return false;
        }
    }
    return true;
}

void fn_7100dae6dc(UnkDetectList_ldae6dc* self, s32 level, UnkDetectOwner_ldae6dc* owner) {
    UnkRinkaControl_ruc* fx = owner->field_0x280;
    if (!fx)
        return;
    bool on = false;
    if (level != 0x7fffffff && level >= 2) {
        if (!owner->field_0x260 || !fn_710113d83c(owner->field_0x260))
            on = NoneBlocks(self);
    }
    fn_710114ffb8(fx, fn_7100001570("emmydetection", 13), on, false);
}
