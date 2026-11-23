// sound::CMusicManager::CTunableMusicManager's unit: stops the request at an index (fading over the given
// time, or its own when the time is negative), clears its file paths and name, and tells its callback the
// name of the path it played. Names and layouts are placeholders.
#include "../../../../game_aca57c/UnkBufferString.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/snd/CSoundSystem.h"
#include "game/UnkTU23.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;

using UnkString40_lfd5ecc = UnkBufferString<0x40, g_unk_7101a1b678>;

struct UnkPathRecord_lfd5ecc {
    CFilePathStrId path;
    u8 pad_0x8[0x10];
};

struct UnkPathRecordVec_lfd5ecc {
    UnkPathRecord_lfd5ecc* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool autoGrow;
    u32 flags;
    void* allocator;
};

using UnkRequestCallback_lfd5ecc = void (*)(const char* path);

struct UnkRequest_lfd5ecc {
    u32 state;  // 0x0
    u8 pad_0x4[0x8];
    f32 fade_time;  // 0xc
    u8 pad_0x10[0xc];
    u32 field_0x1c;
    u8 pad_0x20[0x8];
    UnkPathRecordVec_lfd5ecc paths;  // 0x28
    UnkRequestCallback_lfd5ecc on_stop;  // 0x50
    CStrId name;  // 0x58
};
static_assert(sizeof(UnkRequest_lfd5ecc) == 0x60);

struct UnkRequestArray_lfd5ecc {
    UnkRequest_lfd5ecc* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool autoGrow;

    UnkRequest_lfd5ecc& operator[](u32 i) {
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

struct UnkMusicMgr_lfd5ecc {
    u8 pad_0x0[0xb8];
    UnkRequestArray_lfd5ecc requests;  // 0xb8
};

// Releases a path id, leaving the empty id (the two-step release: the count is reread after the decrement).
static inline void ResetPath_lfd5ecc(CFilePathStrId& id) {
    using namespace base::global;
    if (UnkStringPool* pool = g_unk_7101d47200) {
        if (id.data && id.data != &pool->empty) {
            --id.data->ref_count;
            if (id.data->ref_count == 0) {
                StringPool_Release(pool, id.data);
                pool = g_unk_7101d47200;
            }
        }
        id.data = &pool->empty;
    }
}

static inline const char* CStr_lfd5ecc(const CRntString* s) {
    return s->data ? s->data : g_unk_710162e2ed;
}

void fn_7100fd5ecc(UnkMusicMgr_lfd5ecc* self, u32 index, bool hold, f32 time) {
    UnkRequest_lfd5ecc& request = self->requests[index];
    if (request.paths.count == 0)
        return;
    request.field_0x1c = 0;
    f32 fade = time < 0.0f ? request.fade_time : time;
    UnkPathRecord_lfd5ecc& first = request.paths.data[0];
    if (hold)
        fn_7100108a98(g_unk_7101d4d820, (u64)&first, true, fade);
    else
        fn_7100108a84(reinterpret_cast<UnkMgr*>(g_unk_7101d4d820), reinterpret_cast<UnkHandle*>(&first), fade);

    // The played path's string, counting the read. The pooled text is a whole CRntString in the original
    // (UnkStringData lays out only its text and key), copied here as one.
    ++first.path.data->reads;
    const CRntString* played = &first.path.data->String();
    UnkString40_lfd5ecc path;
    CRntString_Assign(path.Str(), played);

    for (u64 i = 0; i < request.paths.count; ++i)
        ResetPath_lfd5ecc(request.paths.data[i].path);
    request.paths.count = 0;
    request.name = base::global::CStrId::Empty();
    request.state = 0;
    if (request.on_stop)
        request.on_stop(CStr_lfd5ecc(path.Str()));
}
