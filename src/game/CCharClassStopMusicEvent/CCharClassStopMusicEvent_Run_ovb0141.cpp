// CCharClassStopMusicEvent's unit (tools/tu_map.py): the event's run. The music manager (g_unk_71726bc560,
// created on first use, game/sound/CMusicManager.h) stops, on every layer, channel 2 or 4 (by the flag at
// +0x1c) over the event's time (+0x18): fn_7100fd7b44, or fn_7100fd82dc with the name of the file path
// at +0x10 unless it is the empty one. The full mask is game/UnkMask.h's. Names and
// layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CBossAIComponent.h"
#include "game/UnkMask.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"

static inline void* MusicManager() {
    void* manager = g_unk_71726bc560;
    if (!manager) {
        manager = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(manager);
        g_unk_71726bc560 = static_cast<UnkMgr_0xf18*>(manager);
    }
    return manager;
}

// A file path id's string data as a CRntString at +0x10 (base/global/CStrId.h's UnkStringData).
struct UnkPathString_b0141 {
    u8 pad_0x0[0x10];
    base::global::CRntString name;  // 0x10
};

struct UnkStopMusicEvent_b0141 {
    u8 pad_0x0[0x10];
    base::global::UnkStringData* path;  // 0x10 (a file path id)
    f32 time;                           // 0x18
    bool field_0x1c;
};

void fn_7100ce4510(UnkStopMusicEvent_b0141* self) {
    base::global::UnkStringData* path = self->path;
    base::global::UnkStringData* empty = &g_unk_7101d47200->empty;
    void* manager = MusicManager();
    if (path == empty) {
        UnkMask kind(self->field_0x1c ? 4 : 2);
        f32 time = self->time;
        fn_7100fd7b44(static_cast<UnkMusicMgr_b0019*>(manager), kind, UnkMaskOf<UnkMusicMaskTag>::All(), time);
        return;
    }
    char* data = reinterpret_cast<char*>(self->path);
    u32 uses;
    __builtin_memcpy(&uses, data + 0x44, 4);
    ++uses;
    __builtin_memcpy(data + 0x44, &uses, 4);
    const base::global::CRntString* name = &reinterpret_cast<UnkPathString_b0141*>(self->path)->name;
    UnkMask kind(self->field_0x1c ? 4 : 2);
    f32 time = self->time;
    fn_7100fd82dc(static_cast<UnkMusicMgr_b0019*>(manager), name, kind,
                  UnkMaskOf<UnkMusicMaskTag>::All(), time);
}
