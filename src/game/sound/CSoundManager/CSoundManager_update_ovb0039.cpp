// sound::CSoundManager unit (tools/tu_map.py, unit 2646): the sound manager's update and the copy
// assignment of a list of two parallel arrays sharing one count. Names and layouts are placeholders.
#include <new>

#include "base/UnkCountedResource.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CSwarmControllerComponent.h"
#include "game/GUI/CAmiiboComposition.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU23.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2615.h"
#include "game/UnkTU315.h"
#include "game/UnkTU32.h"
#include "game/malloc.h"
#include "types.h"

using base::global::CStrId;

// ---- The update ----
//
// Mutes the
// sound system while the game manager (fn_7100decc5c) or the render manager (fn_7100219970) says
// so, updates it, drops the dead weak references of each of its lists (as CShotManager's update,
// src/game/CShotManager.cpp), updates three sound objects and reports the sound count to the
// statistics object as "NUM_SOUNDS".

// base::snd::CSoundSystem as far as its update (slot 0x28).
struct UnkSoundMgr;
struct UnkSoundSystem_ovb0039 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28(f32 dt);
};
void fn_7100108290(UnkSoundMgr* self, bool enabled);
void fn_710010829c(UnkSoundMgr* self, bool enabled);
u32 fn_7100108288(UnkSoundMgr* self);  // the number of sounds

// A weak reference, as src/game/CShotManager.cpp has it.
struct UnkShotWeakTarget {
    void* field_0x0;
    s64 generation;
};
struct UnkShotWeakRef {
    UnkShotWeakTarget* target;
    s64 generation;
};

struct UnkSoundManager_ovb0039 {
    u8 pad_0x0[0x68];
    base::UnkVector<UnkShotWeakRef>* lists;  // 0x68
    u8 pad_0x70[0x8];
    u32 list_count;  // 0x78
};

struct UnkStateSink;
void fn_7100fca0f0(UnkStateSink* self);
void fn_710101e7d8(void* self);
void fn_7100fcd7f0(void* self);

struct UnkStatsObject;

// Removes element i, moving the later ones down.
static inline void EraseAt(base::UnkVector<UnkShotWeakRef>* v, u32 i) {
    if (i < v->count) {
        for (u32 j = i; j < v->count - 1; ++j)
            v->data[j] = v->data[j + 1];
        --v->count;
    }
}

void fn_7101023c3c(UnkSoundManager_ovb0039* self, f32 dt) {
    bool game_running = !fn_7100decc5c(g_unk_7101d47310);
    fn_7100108290(g_unk_7101d4d820, game_running);
    bool render_running = !fn_7100219970(g_unk_7101d47300);
    fn_710010829c(g_unk_7101d4d820, render_running);
    reinterpret_cast<UnkSoundSystem_ovb0039*>(g_unk_7101d4d820)->vf_0x28(dt);

    for (base::UnkVector<UnkShotWeakRef>* list = self->lists; list != self->lists + self->list_count;
         ++list) {
        for (s32 i = (s32)list->count - 1; i >= 0; --i) {
            UnkShotWeakRef* ref = fn_7100748064(list, i);
            UnkShotWeakTarget* target = ref->target;
            // Both generation tests are evaluated (the original tests the -1 one first).
            bool alive = (ref->generation != -1) & (target && target->generation == ref->generation);
            if (target && alive && target->field_0x0)
                continue;
            EraseAt(list, i);
        }
    }

    fn_7100fca0f0(g_unk_717275c380);
    fn_710101e7d8(g_unk_71727b85d0);
    fn_7100fcd7f0(g_unk_71729efa48);

    UnkStatsObject* stats = g_unk_7101d47100;
    if (!stats) {
        stats = static_cast<UnkStatsObject*>(fn_71000839a0(0x78, 0x3f));
        fn_7100131f48(stats);
        g_unk_7101d47100 = stats;
    }
    f32 count = (f32)fn_7100108288(g_unk_7101d4d820);
    fn_7100132248(stats, CStrId("NUM_SOUNDS", true), count);
}

// ---- The list copy ----
//
// The copy assignment of a list of two
// parallel arrays sharing one count, as CAudioComponent's pair list (+0x98 of
// src/game/CAudioComponent/CAudioComponent_Sounds_ovb0015.cpp): 0x18-byte resource records (a
// string id, a file-path id and their counted resource) and string ids. Names and layouts are
// placeholders.


struct UnkIdPair;

// CFilePathStrId's copy constructor with the pointer assigned in the body (the original loads it
// after the record's address is formed).
struct UnkPathId_ovb0039 {
    base::global::UnkStringData* data;
    UnkPathId_ovb0039(const UnkPathId_ovb0039& other) {
        data = other.data;
        if (&g_unk_7101d47200->empty != data)
            ++data->ref_count;
    }
};

// A resource record as its copy constructor sees it.
struct UnkSoundIdResource_ovb0039 {
    CStrId id;
    UnkPathId_ovb0039 path;
    UnkCountedResource* resource;

    UnkSoundIdResource_ovb0039(const UnkSoundIdResource_ovb0039& other)
        : id(other.id), path(other.path), resource(nullptr) {
        if (UnkCountedResource* r = other.resource) {
            ++r->ref_count;
            if (UnkCountedResource* old = resource) {
                if (--old->ref_count == 0)
                    fn_710001737c(old);
            }
            resource = r;
        }
    }
};

struct UnkSoundPairList_ovb0039 {
    UnkSoundIdResource_ovb0039* records;
    CStrId* ids;
    u32 count;
    u32 capacity;
};

// CStrId's copy constructor, with the pointer copied untyped (the count is reloaded after it).
static inline void CopyId(CStrId* dst, const CStrId* src) {
    base::global::UnkStringData* data = src->data;
    __builtin_memcpy(&dst->data, &data, sizeof(data));
    if (&g_unk_7101d47108->empty != data)
        ++data->ref_count;
}

void fn_710102a344(UnkSoundPairList_ovb0039* self);
void fn_7101029ec8(UnkSoundPairList_ovb0039* self, u32 n);

UnkSoundPairList_ovb0039* fn_710102a0ec(UnkSoundPairList_ovb0039* self, const UnkSoundPairList_ovb0039* other) {
    if (self == other)
        return self;
    if (self->ids && self->count) {
        for (u64 i = 0; i < self->count; ++i)
            self->ids[i].Reset();
    }
    if (self->records && self->count) {
        for (u64 i = 0; i < self->count; ++i)
            fn_7100026f70(reinterpret_cast<UnkIdPair*>(&self->records[i]));
    }
    self->count = 0;
    if (other->count != self->capacity) {
        fn_710102a344(self);
        fn_7101029ec8(self, other->count);
    }
    self->count = other->count;
    for (u64 i = 0; i < self->count; ++i)
        new (&self->records[i]) UnkSoundIdResource_ovb0039(other->records[i]);
    self->count = other->count;
    for (u64 i = 0; i < self->count; ++i)
        CopyId(&self->ids[i], &other->ids[i]);
    return self;
}
