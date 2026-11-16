// CTunableMusicManager (tools/tu_map.py unit 2637). fn_7100fdcf2c: marks slot `slot` active or not and, when
// that changes it and the slot's request has paths, passes the change to the sound manager with `time`.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/snd/CSoundSystem.h"
#include "game/UnkTU23.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"

struct UnkPathRecord_lfdcf2c {
    u8 pad_0x0[0x18];
};

struct UnkRequest_lfdcf2c {
    u8 pad_0x0[0x28];
    UnkPathRecord_lfdcf2c* paths;  // 0x28
    u32 path_count;                // 0x30
    u8 pad_0x34[0x2c];
};
static_assert(sizeof(UnkRequest_lfdcf2c) == 0x60);

struct UnkRequestArray_lfdcf2c {
    UnkRequest_lfdcf2c* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool autoGrow;

    UnkRequest_lfdcf2c& operator[](u32 i) {
        if (autoGrow && count <= i) {
            u32 n = capacity;
            if (capacity <= i && step != 0)
                n = (i / step + 1) * step;
            fn_7100fddcb4(reinterpret_cast<base::global::CRntVector<UnkMusicEntry_0x60>*>(this), n);
            fn_7100fde4d8(reinterpret_cast<UnkArray*>(this), i + 1);
        }
        return data[i];
    }
};

void fn_7100108ab0(UnkSoundMgr* mgr, u64 handle, bool flag, f32 value);

void fn_7100fdcf2c(void* manager, u32 a, u32 b, f32 value) {
    UnkMusicManager* self = static_cast<UnkMusicManager*>(manager);
    if (a < self->slot_count) {
        bool active = b & 1;
        if (self->slot_active.GrowAt(a) != active) {
            self->slot_active.GrowAt(a) = active;
            UnkRequest_lfdcf2c& request = reinterpret_cast<UnkRequestArray_lfdcf2c&>(self->entries)[a];
            if (request.path_count != 0)
                fn_7100108ab0(g_unk_7101d4d820, (u64)&request.paths[0], active, value);
        }
    }
}
