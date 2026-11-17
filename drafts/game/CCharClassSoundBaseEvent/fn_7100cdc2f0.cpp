// CCharClassSoundBaseEvent's unit (tools/tu_map.py unit 2296): plays one of up to eight sound paths, picked
// at random among those set, on the target's entity from a shared description, then fades the started sound
// when a fade time is given; returns the sound's handle. Names and layouts are placeholders.
#include <stdlib.h>

#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU101.h"
#include "game/UnkTU102.h"
#include "game/UnkTU2523.h"
#include "game/sound/UnkSoundDesc.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;

struct UnkCharClassSoundEvent_b0080;

// The handle's view of the reference block.
struct UnkSoundRefView_lcdc2f0 {
    void* sound;
    s64 serial;
};

static inline const UnkSoundRefView_lcdc2f0* View(const UnkSoundHandle& handle) {
    return reinterpret_cast<const UnkSoundRefView_lcdc2f0*>(handle.ref);
}

static inline bool Valid(const UnkSoundHandle& handle) {
    const UnkSoundRefView_lcdc2f0* ref = View(handle);
    return ref && ref->serial == handle.serial && handle.serial != -1;
}

struct UnkSoundEntity_lcdc2f0 {
    u8 pad_0x0[0x1b5];
    bool field_0x1b5;
};

// What the event plays on.
struct UnkSoundTarget_lcdc2f0 {
    UnkSoundEntity_lcdc2f0* entity;
};

static inline const CFilePathStrId* PathOf(const void* path) {
    return static_cast<const CFilePathStrId*>(path);
}

// A path is set when its text has more than one character.
static inline bool IsSet(const CFilePathStrId* path) {
    return path->data->String().length > 1;
}

// The path's string, counting the read.
static inline const CRntString& StringOf(const CFilePathStrId& path) {
    ++path.data->reads;
    return path.data->String();
}

UnkSoundHandle fn_7100cdc2f0(UnkCharClassSoundEvent_b0080* self, const void* a10, const void* a18,
                             const void* a20, const void* a28, const void* a30, const void* a38, const void* a40,
                             f32 f58, f32 f5c, f32 f60, f32 f64, f32 f68, f32 f70, f32 f74, f32 f7c,
                             const void* a48, const void* a90, const CStrId* id, const void* a88, bool b6c,
                             u32 u98, bool b80, bool b81, f32 f78, void* arg) {
    UnkSoundTarget_lcdc2f0* target = static_cast<UnkSoundTarget_lcdc2f0*>(arg);
    if (!target->entity || target->entity->field_0x1b5)
        return g_unk_7108b6c460;

    UnkSoundHandle handle;
    handle.ref = nullptr;
    handle.serial = -1;
    const CFilePathStrId* paths[8] = {PathOf(a10), PathOf(a18), PathOf(a20), PathOf(a28),
                                      PathOf(a30), PathOf(a38), PathOf(a40), PathOf(a48)};
    if (!IsSet(paths[0]))
        return handle;

    int count = 0;
    for (int i = 0; i < 8; i++) {
        if (IsSet(paths[i]))
            count++;
    }
    int pick = rand() % count;
    // The first set path from the picked one on, or the first path.
    const CFilePathStrId* path;
    if (IsSet(paths[0]) && pick < 1)
        path = paths[0];
    else if (IsSet(paths[1]) && pick < 2)
        path = paths[1];
    else if (IsSet(paths[2]) && pick < 3)
        path = paths[2];
    else if (IsSet(paths[3]) && pick < 4)
        path = paths[3];
    else if (IsSet(paths[4]) && pick < 5)
        path = paths[4];
    else if (IsSet(paths[5]) && pick < 6)
        path = paths[5];
    else if (IsSet(paths[6]) && pick < 7)
        path = paths[6];
    else if (IsSet(paths[7]) && pick < 8)
        path = paths[7];
    else
        path = paths[0];

    f32 value = f64;
    if (f68 > f64) {
        f32 t = (rand() & 0x7fff) * (1.0 / 32767.0);
        value = (f68 - f64) * t + f64;
    }

    if (StringOf(GameManagerIdTable().field_0x5f8).key == StringOf(*path).key)
        return handle;

    static UnkSoundDesc s_desc;
    s_desc.kind.field_0x0 = u98;
    s_desc.path = *path;
    const CStrId* name = static_cast<const CStrId*>(a88);
    if (name->data == &g_unk_7101d47108->empty)
        name = &GameManagerIdTable().field_0xd90;
    s_desc.kind.field_0x4 = fn_7100106820(name->Text());
    s_desc.field_0x38 = f58;
    s_desc.field_0x3c = f5c;
    s_desc.field_0x6d = f60 > f5c;
    s_desc.field_0x40 = f60;
    s_desc.field_0x44 = value;
    s_desc.field_0x6c = false;
    s_desc.field_0x4c = f70;
    s_desc.field_0x6e = b81;
    s_desc.field_0x5c = f7c;
    {
        CStrId event(*id);
        handle = fn_7100d7c374(reinterpret_cast<UnkEntityWithSound*>(target->entity), &s_desc, &event, b6c, b80,
                               true);
    }
    if (Valid(handle) && View(handle)->sound && f78 >= 0.0f)
        fn_7100d7c510(reinterpret_cast<UnkShipRechargeEntity_b49a4*>(target->entity), &handle, f74, f78);
    return handle;
}
