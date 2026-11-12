// Unit 2525 (tools/tu_map.py; hint base::reflection): the environment manager of
// src/game/UnkUnit2525/UnkUnit2525_SetResource_b51.cpp (g_unk_7101d47368, 0x4c8 bytes): its constructor
// (fn_7100d8ced8), copy-assignment (fn_7100d90624), deferral flags, environment sound and music, the
// fade reset of its visuals (fn_7100d8f0b4), the destroy step of one table's map values (fn_7100d90304,
// fn_7100d90460) and a pair of 11-bucket maps (fn_7100d8d670). Names and layouts are placeholders.
#include <new>

#include "base/UnkChainedMap.h"
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/color/CColor4F.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/CVector4D.h"
#include "base/reflection.h"
#include "base/snd/CSoundSystem.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU23.h"
#include "game/UnkTU2607.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"
// The 0x50-byte record array fn_7100d90460 is declared with (unit 2601).
#include "../../game/UnkUnit2601/UnkUnit2601_VecCopy_b56.h"

using base::global::CFilePathStrId;

struct UnkSoundMgr;
struct UnkMgr;
struct UnkHandle;

// The environment sound (+0x480): its path and fade time.
struct UnkEnvSound_ovb56 {
    u8 unk_0x0[0x8];
    CFilePathStrId path;  // 0x8
    u8 unk_0x10[0xc];
    f32 fade_time;  // 0x1c
};

struct UnkUnit2525_b51;
struct UnkEnvironmentVisuals;
struct UnkEnvironmentData;

struct UnkEnvironmentManager_ovb56 {
    u8 unk_0x0[0x480];
    UnkEnvSound_ovb56* sound;  // 0x480
    u8 unk_0x488[0x8];
    void* music;  // 0x490
    u8 unk_0x498[0x4a2 - 0x498];
    bool sound_paused;  // 0x4a2
    u8 unk_0x4a3;
    bool deferred_a;  // 0x4a4
    bool deferred_c;  // 0x4a5
    bool deferred_b;  // 0x4a6
    u8 unk_0x4a7;
    void* pending_a;  // 0x4a8
    void* pending_b;  // 0x4b0
    void* pending_c;  // 0x4b8
};
static_assert(__builtin_offsetof(UnkEnvironmentManager_ovb56, pending_c) == 0x4b8, "layout");

// Stops (fading out, or quickly when `quick`) the environment sound; with none, the sound kind 5.
void fn_7100d8e020(UnkEnvironmentManager_ovb56* self, bool quick) {
    if (UnkEnvSound_ovb56* sound = self->sound) {
        if (sound->path.data == g_unk_7101d470e8.data)
            return;
        UnkHandle* handle = reinterpret_cast<UnkHandle*>(&sound->path);
        if (quick)
            fn_7100108a84(reinterpret_cast<UnkMgr*>(g_unk_7101d4d820), handle, 0.2f);
        else
            fn_7100108abc(reinterpret_cast<UnkMgr*>(g_unk_7101d4d820), handle, sound->fade_time, 0.0f);
        return;
    }
    fn_7100108ae8(g_unk_7101d4d820, 5, quick ? 0.0f : 3.0f, 0.0f, false);
}

// Pauses or resumes the environment sound (resuming replays it, fn_7100d8db30).
void fn_7100d8e1d0(UnkEnvironmentManager_ovb56* self, bool paused) {
    if (paused == self->sound_paused)
        return;
    self->sound_paused = paused;
    if (paused) {
        if (self->sound)
            fn_7100d8db30(reinterpret_cast<UnkUnit2525_b51*>(self), self->sound, false);
        return;
    }
    fn_7100d8e020(self, false);
}

// Sets the three deferral flags; a kind whose flag changes applies its pending resource (when it was
// deferred) and drops it.
void fn_7100d8efd0(UnkEnvironmentManager_ovb56* self, bool a, bool b, bool c) {
    if (self->deferred_a != a) {
        if (self->deferred_a && self->pending_a)
            fn_7100d8e398(reinterpret_cast<UnkEnvironmentVisuals*>(self),
                          static_cast<const UnkEnvironmentData*>(self->pending_a), false);
        self->pending_a = nullptr;
    }
    if (self->deferred_b != b) {
        if (self->deferred_b && self->pending_b)
            fn_7100d8db30(reinterpret_cast<UnkUnit2525_b51*>(self), self->pending_b, false);
        self->pending_b = nullptr;
    }
    if (self->deferred_c != c) {
        if (self->deferred_c && self->pending_c)
            fn_7100d8d854(reinterpret_cast<UnkUnit2525_b51*>(self), self->pending_c);
        self->pending_c = nullptr;
    }
    self->deferred_a = a;
    self->deferred_b = b;
    self->deferred_c = c;
}

// The channel mask as src/game/UnkLoadingScreen.cpp defines it (passed through a copy).
struct UnkLoadingMask {
    u32 bits;
    UnkLoadingMask() : bits(0) {}
    UnkLoadingMask(u32 b) : bits(b) {}
    UnkLoadingMask(const UnkLoadingMask& o) : bits(o.bits) {}
};
template <class T>
struct UnkLoadingMaskOf_ovb56 {
    static const UnkLoadingMask& None() {
        static UnkLoadingMask s;
        return s;
    }
    static const UnkLoadingMask& All() {
        static UnkLoadingMask s(~None().bits & 0x1ff);
        return s;
    }
};
struct UnkLoadingMaskTag_ovb56;

// A copy of `mask` with `bit` flipped.
static inline UnkLoadingMask Flipped_ovb56(const UnkLoadingMask& mask, u32 bit) {
    return UnkLoadingMask((mask.bits & bit) ? mask.bits & ~bit : mask.bits | bit);
}
extern UnkMgr_0xf18* g_unk_71726bc560;

// Replays the environment music (fn_7100d8d854) or, with none, has the music manager switch channel 8
// to every channel but 0x100's state flipped.
void fn_7100d8e0a0(void* manager) {
    UnkEnvironmentManager_ovb56* self = static_cast<UnkEnvironmentManager_ovb56*>(manager);
    if (self->music) {
        fn_7100d8d854(reinterpret_cast<UnkUnit2525_b51*>(self), self->music);
        return;
    }
    void* instance = g_unk_71726bc560;
    if (!instance) {
        instance = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(instance);
        g_unk_71726bc560 = static_cast<UnkMgr_0xf18*>(instance);
    }
    fn_7100fd7b44(instance, UnkLoadingMask(8), Flipped_ovb56(UnkLoadingMaskOf_ovb56<UnkLoadingMaskTag_ovb56>::All(), 0x100),
                  0.0f);
}

// The fading render settings (src/game/UnkEnvironmentFade.cpp) at +0x28.
struct UnkEnvironmentFade;
using UnkEnvFadeColor = base::math::UnkVector4D;
using base::math::CVector2D;
void fn_7100f984ac(f32 time, UnkEnvironmentFade* self, const UnkEnvFadeColor* a, const UnkEnvFadeColor* b,
                   const UnkEnvFadeColor* c);
bool fn_7100219f64(UnkRenderManager* self, u32 index);
void fn_7100219e7c(UnkRenderManager* self, u32 index, u32 value);

struct UnkEnvPreset_ovb56 {
    u8 unk_0x0[0xc];
    u32 field_0xc;
};

struct UnkEnvironmentVisuals_ovb56 {
    UnkEnvPreset_ovb56* preset;
    u8 unk_0x8[0x20];
    u8 fade[0x479];  // 0x28
    bool field_0x4a1;
};
static_assert(__builtin_offsetof(UnkEnvironmentVisuals_ovb56, field_0x4a1) == 0x4a1, "layout");

// Sets the fade's three colours to `color` at once.
static inline void SetColors_ovb56(UnkEnvironmentFade* fade, const base::color::CColor4F& color) {
    UnkEnvFadeColor a = fn_7100030878(&color);
    UnkEnvFadeColor b = fn_7100030878(&color);
    UnkEnvFadeColor c = fn_7100030878(&color);
    fn_7100f984ac(0.0f, fade, &a, &b, &c);
}

// With a preset value at its +0xc, turns the render manager's setting 0 on; otherwise turns it off and
// sets the fade's colours (a dim yellow) and range (200 to 700) at once.
void fn_7100d8f0b4(UnkEnvironmentVisuals_ovb56* self) {
    u32 value = self->preset->field_0xc;
    bool on = fn_7100219f64(g_unk_7101d47300, 0);
    if (value) {
        if (!on)
            fn_7100219e7c(g_unk_7101d47300, 1, 0);
        self->field_0x4a1 = true;
        return;
    }
    if (on)
        fn_7100219e7c(g_unk_7101d47300, 0, 0);
    self->field_0x4a1 = false;
    UnkEnvironmentFade* fade = reinterpret_cast<UnkEnvironmentFade*>(self->fade);
    base::color::CColor4F color(0.4f, 0.35f, 0.1f, 1.0f);
    SetColors_ovb56(fade, color);
    CVector2D range = {200.0f, 700.0f};
    fn_7100f985d8(0.0f, fade, &range);
}

using base::UnkAllocator;

// A path entry (0x18 bytes) of an inner array.
struct UnkPathEntry_ovb56 {
    base::UnkStrIdMemberOf<g_unk_7101d47200> path;
    u8 unk_0x8[0x10];
};

// An element (0x50 bytes) holding an array of path entries.
struct UnkPathGroup_ovb56 {
    u64 field_0x0;
    UnkPathEntry_ovb56* data;  // 0x8
    u32 count;                 // 0x10
    u32 capacity;              // 0x14
    u8 unk_0x18[0x10];
    UnkAllocator* allocator;  // 0x28
    u8 unk_0x30[0x20];
};
static_assert(sizeof(UnkPathGroup_ovb56) == 0x50, "layout");

struct UnkPathGroupVec_ovb56 {
    UnkPathGroup_ovb56* data;
    u32 count;
    u32 capacity;
    u8 unk_0x10[0x10];
    UnkAllocator* allocator;  // 0x20
};

// Shrinks the array to `n`, releasing the entries of every dropped group.
__attribute__((noinline)) void fn_7100d90460(UnkRecord50Vec_b56* vec, u32 n) {
    UnkPathGroupVec_ovb56* self = reinterpret_cast<UnkPathGroupVec_ovb56*>(vec);
    for (u64 i = n; i < self->count; ++i) {
        UnkPathGroup_ovb56* group = &self->data[i];
        if (group->data) {
            for (u64 j = 0; j < group->count; ++j)
                group->data[j].path.~UnkStrIdMemberOf();
            group->count = 0;
            group->allocator->vf_0x48(group->data);
            group->capacity = 0;
        }
    }
    self->count = n;
}

// A polymorphic 0x20-byte element, destroyed through its vf_0x8.
struct UnkPolyElem_ovb56 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    u8 unk_0x8[0x18];
};


// The path groups as a member, emptied by its destructor.
struct UnkPathGroups_ovb56 : UnkPathGroupVec_ovb56 {
    ~UnkPathGroups_ovb56() {
        if (data) {
            fn_7100d90460(reinterpret_cast<UnkRecord50Vec_b56*>(this), 0);
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

// The polymorphic elements as a member, destroyed in order by its destructor.
struct UnkPolyElems_ovb56 {
    UnkPolyElem_ovb56* data;
    u32 count;
    u32 capacity;
    u8 unk_0x10[0x10];
    UnkAllocator* allocator;  // 0x20

    ~UnkPolyElems_ovb56() {
        if (data) {
            for (u64 i = 0; i < count; ++i)
                data[i].vf_0x8();
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

// The value's named path groups.
struct UnkNamedPathGroups_ovb56 {
    UnkPathGroups_ovb56 groups;  // 0x0
    base::UnkStrIdMember name;   // 0x28
};

// The 0x70-byte value of a string-id keyed map (destroyed through fn_7100d90304).
struct UnkMapValue_ovb56 {
    base::UnkStrIdMember key;     // 0x0
    UnkNamedPathGroups_ovb56 groups;  // 0x8
    u8 unk_0x38[8];
    UnkPolyElems_ovb56 elems;  // 0x40
    u8 unk_0x68[8];
};
static_assert(sizeof(UnkMapValue_ovb56) == 0x70, "layout");

__attribute__((flatten)) void fn_7100d90304(char* p) {
    reinterpret_cast<UnkMapValue_ovb56*>(p)->~UnkMapValue_ovb56();
}

// The environment state at +0x28 (0x450 bytes), copied by fn_7100d907cc.
struct UnkEnvironmentState_ovb56 {
    u8 unk_0x0[0x450];
    UnkEnvironmentState_ovb56& operator=(const UnkEnvironmentState_ovb56& other) asm("fn_7100d907cc");
};

// The environment manager (0x4c8 bytes) as its copy sees it.
struct UnkEnvironmentManagerCopy_ovb56 {
    void* table_a;  // 0x0
    void* table_b;  // 0x8
    void* table_c;  // 0x10
    base::UnkStrIdMember current_a;  // 0x18
    void* field_0x20;
    UnkEnvironmentState_ovb56 state;  // 0x28
    base::UnkStrIdMember current_b;  // 0x478
    void* resource_b;  // 0x480
    base::UnkStrIdMember current_c;  // 0x488
    void* resource_c;  // 0x490
    u64 field_0x498;
    u8 flags[8];  // 0x4a0
    void* pending_a;  // 0x4a8
    void* pending_b;  // 0x4b0
    void* pending_c;  // 0x4b8
    u8 field_0x4c0[3];
};
static_assert(sizeof(UnkEnvironmentManagerCopy_ovb56) == 0x4c8, "layout");

// The manager's copy-assignment.
__attribute__((flatten)) UnkEnvironmentManagerCopy_ovb56* fn_7100d90624(UnkEnvironmentManagerCopy_ovb56* self,
                                                                         const UnkEnvironmentManagerCopy_ovb56* other) {
    *self = *other;
    return self;
}

using base::global::CStrId;

void fn_7100f97d90(void* state);

// The environment manager (0x4c8 bytes) as its constructor sees it.
struct UnkEnvironmentManagerInit_ovb56 {
    void* table_a;  // 0x0
    void* table_b;  // 0x8
    void* table_c;  // 0x10
    CStrId current_a;  // 0x18
    void* field_0x20;
    u8 state[0x450];  // 0x28
    CStrId current_b;  // 0x478
    void* resource_b;  // 0x480
    CStrId current_c;  // 0x488
    void* resource_c;  // 0x490
    u64 field_0x498;
    bool field_0x4a0;
    bool field_0x4a1;
    bool field_0x4a2;
    bool field_0x4a3;
    bool deferred_a;  // 0x4a4
    bool deferred_c;  // 0x4a5
    bool deferred_b;  // 0x4a6
    u8 unk_0x4a7;
    void* pending_a;  // 0x4a8
    void* pending_b;  // 0x4b0
    void* pending_c;  // 0x4b8
    bool field_0x4c0;
    bool field_0x4c1;
    bool field_0x4c2;
};
static_assert(__builtin_offsetof(UnkEnvironmentManagerInit_ovb56, field_0x4c2) == 0x4c2, "layout");

// Constructs an empty string id in place.
static inline void InitEmpty_ovb56(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

void fn_7100d8ced8(void* p) {
    auto* self = static_cast<UnkEnvironmentManagerInit_ovb56*>(p);
    self->table_a = nullptr;
    self->table_b = nullptr;
    self->table_c = nullptr;
    InitEmpty_ovb56(&self->current_a);
    fn_7100f97d90(self->state);
    InitEmpty_ovb56(&self->current_b);
    self->resource_b = nullptr;
    InitEmpty_ovb56(&self->current_c);
    self->resource_c = nullptr;
    self->field_0x498 = 0;
    self->field_0x4a0 = false;
    self->field_0x4a1 = false;
    self->field_0x4a2 = true;
    self->field_0x4a3 = true;
    self->field_0x4c0 = false;
    self->field_0x4c1 = false;
    self->field_0x4c2 = false;
}

// A chained map started with 11 buckets.
struct UnkMap11_ovb56 : UnkChainedMap {
    __attribute__((always_inline)) UnkMap11_ovb56() { ResizeBuckets(11); }
};

// Two such maps (0x48 bytes each).
struct UnkMapPair_ovb56 {
    UnkMap11_ovb56 first;
    UnkMap11_ovb56 second;
};

__attribute__((flatten)) void fn_7100d8d670(UnkMapPair_ovb56* self) {
    new (self) UnkMapPair_ovb56;
}
