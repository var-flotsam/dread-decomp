// CCharClassSoundBaseEvent's unit (tools/tu_map.py unit 2296): plays a sound event on the target's entity
// from a shared description (path, name or the id table's default, volume, a random value between two
// bounds, ...), then fades the started sound when a fade time is given. Names and layouts are placeholders.
#include <stdlib.h>

#include "base/UnkDestructors.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU102.h"
#include "game/UnkTU2523.h"
#include "game/sound/UnkSoundDesc.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::UnkStrIdMember;
using base::UnkStrIdMemberOf;
using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkLoopSoundEvent_ovb0066;

// The handle's view of the reference block.
struct UnkSoundRefView_lcdc910 {
    void* sound;
    s64 serial;
};

static inline const UnkSoundRefView_lcdc910* View(const UnkSoundHandle& handle) {
    return reinterpret_cast<const UnkSoundRefView_lcdc910*>(handle.ref);
}

static inline bool Valid(const UnkSoundHandle& handle) {
    const UnkSoundRefView_lcdc910* ref = View(handle);
    return ref && handle.serial != -1 && ref->serial == handle.serial;
}

// What the event plays on.
struct UnkSoundTarget_lcdc910 {
    UnkEntityWithSound* entity;
};

void fn_7100cdc910(UnkLoopSoundEvent_ovb0066* self, UnkStrIdMemberOf<g_unk_7101d47200>* path,
                   UnkStrIdMember* sound, const CStrId* a, const CStrId* b, f32 f0, f32 f1, f32 f2, f32 f3,
                   bool c, bool d, f32 f4, f32 f5, f32 f6, f32 f7, bool e, bool g, f32 f8, void* arg) {
    static UnkSoundDesc s_desc;
    s_desc.kind.field_0x0 = 0;
    s_desc.path = *reinterpret_cast<const CFilePathStrId*>(path);
    const CStrId* name = b;
    if (name->data == &g_unk_7101d47108->empty)
        name = &GameManagerIdTable().field_0xd90;
    s_desc.kind.field_0x4 = fn_7100106820(name->Text());
    s_desc.field_0x6d = true;
    s_desc.field_0x38 = f0;
    s_desc.field_0x3c = f1;
    s_desc.field_0x40 = f2;
    f32 value = f3;
    if (f4 >= f3) {
        f32 t = (rand() & 0x7fff) * (1.0 / 32767.0);
        value = (f4 - f3) * t + f3;
    }
    s_desc.field_0x44 = value;
    s_desc.field_0x6c = true;
    s_desc.field_0x4c = f5;
    s_desc.field_0x50 = f6;
    s_desc.field_0x5c = f8;
    s_desc.field_0x6e = g;

    UnkSoundTarget_lcdc910* target = static_cast<UnkSoundTarget_lcdc910*>(arg);
    UnkSoundHandle handle = fn_7100d7c374(target->entity, &s_desc, const_cast<CStrId*>(a), c, e, d);
    if (Valid(handle) && View(handle)->sound && f7 >= 0.0f)
        fn_7100d7c510(reinterpret_cast<UnkShipRechargeEntity_b49a4*>(target->entity), &handle, f6, f7);
}
