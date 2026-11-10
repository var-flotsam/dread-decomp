// CCharClassStopMusicEvent's unit (tools/tu_map.py): the event's run. The music manager (g_unk_71726bc560,
// created on first use, game/sound/CMusicManager.h) stops, on every layer, channel 2 or 4 (by the flag at
// +0x1c) over the event's time (+0x18): fn_7100fd7b44, or fn_7100fd82dc with the name of the file path
// at +0x10 unless it is the empty one. The mask types are src/game/UnkLoadingScreen.cpp's. Names and
// layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/sound/CMusicManager.h"
#include "types.h"

extern void* g_unk_71726bc560;
struct UnkLoadingMask {
    u32 bits;
    UnkLoadingMask() : bits(0) {}
    UnkLoadingMask(u32 b) : bits(b) {}
    UnkLoadingMask(const UnkLoadingMask& o) : bits(o.bits) {}
};
template <class T>
struct UnkLoadingMaskOf {
    static const UnkLoadingMask& None() {
        static UnkLoadingMask s;
        return s;
    }
    static const UnkLoadingMask& All() {
        static UnkLoadingMask s(~None().bits & 0x1ff);
        return s;
    }
};
struct UnkLoadingMaskTag;
void fn_7100fd7b44(void* manager, UnkLoadingMask channels, UnkLoadingMask mask, f32 time);
void fn_7100fd82dc(void* manager, const char* name, UnkLoadingMask channels, UnkLoadingMask mask, f32 time);

static inline void* MusicManager() {
    void* manager = g_unk_71726bc560;
    if (!manager) {
        manager = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(manager);
        g_unk_71726bc560 = manager;
    }
    return manager;
}

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
        UnkLoadingMask kind(self->field_0x1c ? 4 : 2);
        f32 time = self->time;
        fn_7100fd7b44(manager, kind, UnkLoadingMaskOf<UnkLoadingMaskTag>::All(), time);
        return;
    }
    char* data = reinterpret_cast<char*>(self->path);
    u32 uses;
    __builtin_memcpy(&uses, data + 0x44, 4);
    ++uses;
    __builtin_memcpy(data + 0x44, &uses, 4);
    const char* name = reinterpret_cast<char*>(self->path) + 0x10;
    UnkLoadingMask kind(self->field_0x1c ? 4 : 2);
    f32 time = self->time;
    fn_7100fd82dc(manager, name, kind, UnkLoadingMaskOf<UnkLoadingMaskTag>::All(), time);
}
