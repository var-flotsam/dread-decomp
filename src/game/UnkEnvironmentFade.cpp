// From unit 2607 of tools/tu_map.py (0x7100f97d90-0x7100f9b32c, no reflected class; its static
// initialiser builds the ids "binaries" and "sources"): a set of render settings that fade from
// the values the render manager has now to new ones over a time, one channel at a time. Each
// channel has its duration, its progress (0 while it runs, 1 when it is done at once) and a flag,
// in three arrays of 29; the values faded live before them. fn_7100d8e398 applies a preset with
// these setters (the object at +0x28 of its first argument). Names and layouts are placeholders.
// Holds fn_7100f97f64, fn_7100f9819c, fn_7100f98244, fn_7100f98370 and fn_7100f98410 (start a
// channel's fade) and fn_7100f989f0 (sets the texture by its path).
#include "base/UnkCountedResource.h"
#include "base/color/CColor4F.h"
#include "base/global/CFilePathStrId.h"
#include "base/math/CVector4D.h"
#include "game/UnkRenderManager.h"
#include "types.h"

using base::color::CColor4F;
using base::global::CFilePathStrId;
using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

// The colours faded are vectors: their assignment is the vector's (fn_710007b518, as
// src/game/CMinimapManager.cpp's colour), and a local one is zeroed and copied member-wise as the
// original's CVector4D is (so one returned by value comes back in memory).
using UnkEnvFadeColor = base::math::UnkVector4D;

// A path id whose release decrements and tests the count in two steps (as
// src/game/CAbilitySonar.cpp's).
struct UnkResPathId {
    UnkStringData* data;

    UnkResPathId(const CFilePathStrId& other) : data(other.data) {
        if (&g_unk_7101d47200->empty != data)
            ++data->ref_count;
    }
    ~UnkResPathId() {
        if (UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
    }
};

// A resource's key: its type's name and its path (as src/game/CAbilitySonar.cpp's).
struct UnkResKey {
    CStrId type;
    UnkResPathId path;

    UnkResKey(const CStrId& t, const CFilePathStrId& p) : type(t), path(p) {}
};

// A counted reference to a resource (base/UnkCountedResource.h; as src/game/CAbilitySonar.cpp's).
struct UnkResRef {
    UnkCountedResource* ptr;

    UnkResRef() : ptr(nullptr) {}
    UnkResRef& operator=(const UnkResRef& other) {
        if (ptr != other.ptr) {
            UnkCountedResource* p = other.ptr;
            if (p)
                ++p->ref_count;
            UnkCountedResource* old = ptr;
            if (old && --old->ref_count == 0)
                fn_710001737c(old);
            ptr = p;
        }
        return *this;
    }
    ~UnkResRef() {
        if (ptr && --ptr->ref_count == 0)
            fn_710001737c(ptr);
    }
};

// A resource type (as src/game_s7/fn_71001b1dd0.cpp registers them): its name first.
struct UnkResType {
    CStrId name;
};

struct UnkResRegistry;
extern UnkResRegistry* g_unk_7101d472f8;

// The registered key for `key` (its parts empty when there is none), and the resource it names.
UnkResKey fn_71000ea448(UnkResRegistry* registry, const UnkResKey* key, void* a);
UnkResRef fn_71000ead80(UnkResRegistry* registry, const UnkResKey* key, u8 flag, void* a);

// A float faded from `from` to `to`.
struct UnkFadePair {
    f32 from;
    f32 to;
};

constexpr u32 kFadeChannels = 29;

struct UnkEnvironmentFade {
    UnkEnvFadeColor field_0x0;   // channel 0: where it starts
    UnkEnvFadeColor field_0x10;  // channel 0: the value now
    UnkEnvFadeColor field_0x20;  // channel 0: where it goes
    u8 pad_0x30[0x54];
    UnkEnvFadeColor field_0x84;  // channel 4: where it starts
    UnkEnvFadeColor field_0x94;  // channel 4: where it goes
    UnkFadePair field_0xa4[8];  // channel 5: two sets of four floats, interleaved
    UnkEnvFadeColor field_0xe4;  // channel 6: where it starts
    UnkEnvFadeColor field_0xf4;  // channel 6: where it goes
    UnkFadePair field_0x104;  // channel 7
    u8 pad_0x10c[0x165];
    u8 field_0x271;  // passed with the texture
    u8 pad_0x272[0x6];
    CFilePathStrId field_0x278;  // the texture's path
    UnkResRef field_0x280;       // the texture
    u8 pad_0x288[0xc0];
    f32 progress[kFadeChannels];
    f32 duration[kFadeChannels];
    bool done[kFadeChannels];

    // Starts channel `i`: it is done at once when the duration is (nearly) zero.
    void Start(u32 i, f32 time) {
        duration[i] = time;
        done[i] = false;
        bool lo_ok = time - 0.001f <= 0.0f;
        bool hi_ok = time + 0.001f >= 0.0f;
        progress[i] = hi_ok && lo_ok ? 1.0f : 0.0f;
    }
};
static_assert(__builtin_offsetof(UnkEnvironmentFade, field_0x104) == 0x104, "UnkEnvironmentFade placeholder layout");
static_assert(__builtin_offsetof(UnkEnvironmentFade, progress) == 0x348, "UnkEnvironmentFade placeholder layout");
static_assert(__builtin_offsetof(UnkEnvironmentFade, duration) == 0x3bc, "UnkEnvironmentFade placeholder layout");
static_assert(__builtin_offsetof(UnkEnvironmentFade, done) == 0x430, "UnkEnvironmentFade placeholder layout");

// The render manager's current values.
void fn_7100219fa0(UnkRenderManager* self, UnkEnvFadeColor* out, u32 arg);

// NON_MATCHING(minor): the original schedules the render manager's GOT page (adrp) before the
// zero argument and the progress store instead of after them (independent instructions).
void fn_7100f97f64(f32 time, UnkEnvironmentFade* self, const UnkEnvFadeColor* to) {
    self->Start(0, time);
    UnkEnvFadeColor now;
    fn_7100219fa0(g_unk_7101d47300, &now, 0);
    fn_710007b518(&self->field_0x0, &now);
    fn_710007b518(&self->field_0x10, &self->field_0x0);
    fn_710007b518(&self->field_0x20, to);
}

void fn_7100219e74(UnkRenderManager* self, CColor4F* inout);

void fn_7100f9819c(f32 time, UnkEnvironmentFade* self, const UnkEnvFadeColor* to) {
    self->Start(4, time);
    CColor4F now(1.0f, 1.0f, 1.0f, 1.0f);
    fn_7100219e74(g_unk_7101d47300, &now);
    UnkEnvFadeColor from = fn_7100030878(&now);
    fn_710007b518(&self->field_0x84, &from);
    fn_710007b518(&self->field_0x94, to);
}

const UnkEnvFadeColor* fn_71002192b8(UnkRenderManager* self);
const UnkEnvFadeColor* fn_71002192c4(UnkRenderManager* self);

void fn_7100f98244(base::math::CVector4D a, base::math::CVector4D b, UnkEnvironmentFade* self, f32 time) {
    self->Start(5, time);
    UnkEnvFadeColor now = *fn_71002192b8(g_unk_7101d47300);
    self->field_0xa4[0].from = now.x;
    self->field_0xa4[1].from = now.y;
    self->field_0xa4[2].from = now.z;
    self->field_0xa4[3].from = now.w;
    fn_710007b518(&now, fn_71002192c4(g_unk_7101d47300));
    self->field_0xa4[4].from = now.x;
    self->field_0xa4[5].from = now.y;
    self->field_0xa4[6].from = now.z;
    self->field_0xa4[7].from = now.w;
    self->field_0xa4[0].to = a.x;
    self->field_0xa4[1].to = a.y;
    self->field_0xa4[2].to = a.z;
    self->field_0xa4[3].to = a.w;
    self->field_0xa4[4].to = b.x;
    self->field_0xa4[5].to = b.y;
    self->field_0xa4[6].to = b.z;
    self->field_0xa4[7].to = b.w;
}

void fn_7100219fec(UnkRenderManager* self, u32 arg, f32* out_value, UnkEnvFadeColor* out_color);

// NON_MATCHING(minor): the original schedules the render manager's GOT page (adrp) before the
// argument addresses and the progress store instead of after the local's zeroing (independent
// instructions).
void fn_7100f98370(f32 time, UnkEnvironmentFade* self, const UnkEnvFadeColor* to) {
    self->Start(6, time);
    f32 value;
    UnkEnvFadeColor now;
    fn_7100219fec(g_unk_7101d47300, 1, &value, &now);
    fn_710007b518(&self->field_0xe4, &now);
    fn_710007b518(&self->field_0xf4, to);
}

// NON_MATCHING(minor): the original computes the scalar out-argument's address (sub x2) right
// after the flag store and saves `self` before zeroing the local colour (independent
// instructions scheduled in another order).
void fn_7100f98410(f32 to, f32 time, UnkEnvironmentFade* self) {
    self->Start(7, time);
    f32 now;
    UnkEnvFadeColor color;
    fn_7100219fec(g_unk_7101d47300, 1, &now, &color);
    self->field_0x104.from = now;
    self->field_0x104.to = to;
}

// UnkResRef's assignment with the other pointer read first.
static inline void Assign(UnkResRef* self, const UnkResRef& other) {
    UnkCountedResource* p = other.ptr;
    if (self->ptr != p) {
        if (p)
            ++p->ref_count;
        UnkCountedResource* old = self->ptr;
        if (old && --old->ref_count == 0)
            fn_710001737c(old);
        self->ptr = p;
    }
}

// The texture's resource type (registered with type 0x14 by src/game_s7/fn_71001b1dd0.cpp).
extern const UnkResType g_unk_716e4540f8;

// The texture parameter as the render manager takes it: the flag at +0x271 and the texture.
struct UnkTextureParam {
    u64 flag;
    UnkCountedResource* texture;
};

// Sets the texture's path: loads the texture it names (none for the empty path) and passes it to
// the render manager with the flag at +0x271.
void fn_7100f989f0(UnkEnvironmentFade* self, const CFilePathStrId& path) {
    CFilePathStrId* own = &self->field_0x278;
    if (own->data == path.data)
        return;
    {
        UnkStringData* d = path.data;
        UnkStringPool* pool = g_unk_7101d47200;
        if (d != &pool->empty)
            ++d->ref_count;
        if (own->data != &pool->empty) {
            --own->data->ref_count;
            if (own->data->ref_count == 0)
                fn_71000803a4(pool, own->data);
        }
        own->data = d;
    }
    if (own->data == &g_unk_7101d47200->empty) {
        if (UnkCountedResource* old = self->field_0x280.ptr) {
            if (--old->ref_count == 0)
                fn_710001737c(old);
            self->field_0x280.ptr = nullptr;
        }
    } else {
        UnkResRegistry* registry = g_unk_7101d472f8;
        UnkResKey key(g_unk_716e4540f8.name, *own);
        UnkResKey found = fn_71000ea448(registry, &key, nullptr);
        if (found.type.data != &g_unk_7101d47108->empty && found.path.data != &g_unk_7101d47200->empty)
            Assign(&self->field_0x280, fn_71000ead80(registry, &found, 0x14, nullptr));
    }
    UnkTextureParam param;
    param.flag = self->field_0x271;
    param.texture = self->field_0x280.ptr;
    fn_7100219294(g_unk_7101d47300, 0xbd88e4a69dc0e5c9, reinterpret_cast<const f32*>(&param), 0);
}
