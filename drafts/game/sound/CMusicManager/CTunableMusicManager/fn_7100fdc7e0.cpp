// sound::CMusicManager::CTunableMusicManager's unit: sets a second value (0x14 of each request, 0x3c of each
// tune) on every active request and every tune that plays the given file path. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"

using base::global::CFilePathStrId;

static inline bool SamePath_lfdc7e0(const CFilePathStrId& a, const CFilePathStrId& b) {
    return a.data == b.data;
}

struct UnkPathRecord;
void CChozoCommanderAIComponent_ConstructFilePathRecord(UnkPathRecord* self);

struct UnkPathRecord_lfdc7e0 {
    CFilePathStrId path;
    u8 pad_0x8[0x10];
};

struct UnkPathVec_b0019;
void fn_7100456980(UnkPathVec_b0019* v, u32 n);

// The request's file-path list (fn_7100456980 is its out-of-line reserve).
struct UnkPathVec_lfdc7e0 {
    UnkPathRecord_lfdc7e0* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool autoGrow;
    u32 flags;
    void* allocator;

    void Reserve(u32 n) { fn_7100456980(reinterpret_cast<UnkPathVec_b0019*>(this), n); }

    UnkPathRecord_lfdc7e0& operator[](u32 i) {
        if (autoGrow && count <= i) {
            u32 n = capacity;
            if (capacity <= i && step != 0)
                n = (i / step + 1) * step;
            Reserve(n);
            Reserve(i + 1);
            for (u32 j = count; j < i + 1; ++j)
                CChozoCommanderAIComponent_ConstructFilePathRecord(reinterpret_cast<UnkPathRecord*>(&data[j]));
            count = i + 1;
        }
        return data[i];
    }
};

struct UnkRequest_lfdc7e0 {
    u8 pad_0x0[0x14];
    f32 value;  // 0x14
    u8 pad_0x18[0x10];
    UnkPathVec_lfdc7e0 paths;  // 0x28
    u8 pad_0x50[0x10];
};
static_assert(sizeof(UnkRequest_lfdc7e0) == 0x60);

struct UnkRequestArray_lfdc7e0 {
    UnkRequest_lfdc7e0* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool autoGrow;

    UnkRequest_lfdc7e0& operator[](u64 i) {
        if (autoGrow && i >= count) {
            u32 n = capacity;
            if (i >= capacity && step != 0)
                n = ((u32)i / step + 1) * step;
            fn_7100fddcb4(reinterpret_cast<base::global::CRntVector<UnkMusicEntry_0x60>*>(this), n);
            fn_7100fde4d8(reinterpret_cast<UnkArray*>(this), i + 1);
        }
        return data[i];
    }
};

struct UnkTuneItem_lfdc7e0 {
    CFilePathStrId path;
    u8 pad_0x8[0x10];
};

struct UnkTuneValue_lfdc7e0 {
    f32 value;
};

struct UnkTune_lfdc7e0 {
    u8 pad_0x0[0x8];
    UnkTuneItem_lfdc7e0* items;  // 0x8
    u32 count;                   // 0x10
    u8 pad_0x14[0x28];
    UnkTuneValue_lfdc7e0 value;  // 0x3c
    u8 pad_0x40[0x10];
};
static_assert(sizeof(UnkTune_lfdc7e0) == 0x50);

struct UnkTuneArray_lfdc7e0 {
    UnkTune_lfdc7e0* data;
    u32 count;
};

struct UnkTuneNode_lfdc7e0 {
    u8 pad_0x0[0x8];
    UnkTuneArray_lfdc7e0* array;  // 0x8
    UnkTuneNode_lfdc7e0* next;    // 0x10
};

struct UnkMusicMgr_lfdc7e0 {
    u8 pad_0x0[0xb8];
    UnkRequestArray_lfdc7e0 requests;  // 0xb8
    u8 pad_0xd0[0xa0];
    UnkTuneNode_lfdc7e0* tunes;  // 0x170
};

bool fn_7100fdc7e0(void* manager, const CFilePathStrId* id, f32 value) {
    UnkMusicMgr_lfdc7e0* self = static_cast<UnkMusicMgr_lfdc7e0*>(manager);
    for (u64 i = 0; i < self->requests.count; ++i) {
        if (self->requests.data[i].paths.count == 0)
            continue;
        if (SamePath_lfdc7e0(self->requests[i].paths[0].path, *id))
            self->requests[i].value = value;
    }
    for (UnkTuneNode_lfdc7e0* n = self->tunes; n; n = n->next) {
        UnkTuneArray_lfdc7e0& a = *n->array;
        for (u64 i = 0; i < a.count; ++i) {
            UnkTune_lfdc7e0& t = a.data[i];
            for (u64 j = 0; j < t.count; ++j)
                if (SamePath_lfdc7e0(t.items[j].path, *id))
                    a.data[i].value = UnkTuneValue_lfdc7e0{value};
        }
    }
    return false;
}
