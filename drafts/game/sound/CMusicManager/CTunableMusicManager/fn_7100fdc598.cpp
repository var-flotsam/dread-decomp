// sound::CMusicManager::CTunableMusicManager's unit: sets a value (0x8 of each request, 0x38 of each tune) on
// every active request and every tune that plays the given file path. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"

using base::global::CFilePathStrId;

static inline bool SamePath_lfdc598(const CFilePathStrId& a, const CFilePathStrId& b) {
    return a.data == b.data;
}

struct UnkPathRecord;
void fn_7100456ac0(UnkPathRecord* self);

struct UnkPathRecord_lfdc598 {
    CFilePathStrId path;
    u8 pad_0x8[0x10];
};

struct UnkPathVec_b0019;
void fn_7100456980(UnkPathVec_b0019* v, u32 n);

// The request's file-path list (fn_7100456980 is its out-of-line reserve).
struct UnkPathVec_lfdc598 {
    UnkPathRecord_lfdc598* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool autoGrow;
    u32 flags;
    void* allocator;

    void Reserve(u32 n) { fn_7100456980(reinterpret_cast<UnkPathVec_b0019*>(this), n); }

    UnkPathRecord_lfdc598& operator[](u32 i) {
        if (autoGrow && count <= i) {
            u32 n = capacity;
            if (capacity <= i && step != 0)
                n = (i / step + 1) * step;
            Reserve(n);
            Reserve(i + 1);
            for (u32 j = count; j < i + 1; ++j)
                fn_7100456ac0(reinterpret_cast<UnkPathRecord*>(&data[j]));
            count = i + 1;
        }
        return data[i];
    }
};

struct UnkRequest_lfdc598 {
    u8 pad_0x0[0x8];
    f32 value;  // 0x8
    u8 pad_0xc[0x1c];
    UnkPathVec_lfdc598 paths;  // 0x28
    u8 pad_0x50[0x10];
};
static_assert(sizeof(UnkRequest_lfdc598) == 0x60);

struct UnkRequestArray_lfdc598 {
    UnkRequest_lfdc598* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool autoGrow;

    UnkRequest_lfdc598& operator[](u64 i) {
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

struct UnkTuneItem_lfdc598 {
    CFilePathStrId path;
    u8 pad_0x8[0x10];
};

struct UnkTune_lfdc598 {
    u8 pad_0x0[0x8];
    UnkTuneItem_lfdc598* items;  // 0x8
    u32 count;                   // 0x10
    u8 pad_0x14[0x24];
    f32 value;  // 0x38
    u8 pad_0x3c[0x14];
};
static_assert(sizeof(UnkTune_lfdc598) == 0x50);

struct UnkTuneArray_lfdc598 {
    UnkTune_lfdc598* data;
    u32 count;
};

struct UnkTuneNode_lfdc598 {
    u8 pad_0x0[0x8];
    UnkTuneArray_lfdc598* array;  // 0x8
    UnkTuneNode_lfdc598* next;    // 0x10
};

struct UnkMusicMgr_lfdc598 {
    u8 pad_0x0[0xb8];
    UnkRequestArray_lfdc598 requests;  // 0xb8
    u8 pad_0xd0[0xa0];
    UnkTuneNode_lfdc598* tunes;  // 0x170
};

bool fn_7100fdc598(void* manager, f32 value, const CFilePathStrId* id) {
    UnkMusicMgr_lfdc598* self = static_cast<UnkMusicMgr_lfdc598*>(manager);
    for (u64 i = 0; i < self->requests.count; ++i) {
        if (self->requests.data[i].paths.count == 0)
            continue;
        if (SamePath_lfdc598(self->requests[i].paths[0].path, *id))
            self->requests[i].value = value;
    }
    for (UnkTuneNode_lfdc598* n = self->tunes; n; n = n->next) {
        UnkTuneArray_lfdc598& a = *n->array;
        for (u64 i = 0; i < a.count; ++i) {
            UnkTune_lfdc598& t = a.data[i];
            for (u64 j = 0; j < t.count; ++j)
                if (SamePath_lfdc598(t.items[j].path, *id))
                    a.data[i].value = value;
        }
    }
    return false;
}
