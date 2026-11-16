// CAnimationComponent's unit (tools/tu_map.py): when the owner's (+0x30) +0x398 object names an
// animation (a non-empty id), plays it on the main track with mode 1, resets the tracks and restarts
// the owner's effect and collision; then copies the owner's record (+0x38) and sets +0x2c8 when the
// owner has nothing at +0x1f0. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "game/CAnimationComponent.h"
#include "game/CCollisionComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU1141.h"
#include "game/UnkTU1730.h"
#include "types.h"

using base::global::CStrId;

// A request's mode (passed by value, through memory), as CAnimationComponent_Tracks_ovb0009.cpp
// defines it.
struct UnkFlags_7101176ea8 {
    u32 value;
    UnkFlags_7101176ea8(u32 v) : value(v) {}
    UnkFlags_7101176ea8(const UnkFlags_7101176ea8& o) : value(o.value) {}
    UnkFlags_7101176ea8& operator=(const UnkFlags_7101176ea8& o) {
        value = o.value;
        return *this;
    }
};

// A time on an animation's clock (src/game/UnkAnimTime.cpp): unit 1 is seconds.
struct UnkAnimTime_ovb0009 {
    f32 value;
    s32 unit;
};

// A key and a time.
struct UnkS16 {
    u64 key;
    UnkAnimTime_ovb0009 time;
};

// A blend descriptor (g_unk_7172769740, g_unk_7172769770).
struct UnkS12 {
    u32 a, b, c;
};

struct UnkStruct_71007142e8;
struct UnkParent;
void fn_71002d58e4(UnkParent* self, const float* dt);
struct UnkAnimEffect;
void fn_7100143618(UnkAnimEffect* self, bool flag, f32 value);

// The main track's name hash.
static const u64 kMainTrack_l176784 = 0xf4061ff25a2163d6;

struct alignas(8) UnkAnimChannel_l176784 {
    u8 data[0x70];
    UnkAnimChannel_l176784() { fn_71007142e8(reinterpret_cast<UnkStruct_71007142e8*>(this)); }
};

// What fn_71002d58e4 is given: a time step, a flag and three channels.
struct UnkAnimUpdate_l176784 {
    f32 dt;
    bool flag;
    UnkAnimChannel_l176784 channels[3];
};

// A 0x5c-byte record of floats (base/UnkInitRecord_0x5c.h: zeroes and unit scales), copied field by field.
struct UnkAnimRecord_l176784 {
    f32 field_0x0;
    f32 field_0x4;
    f32 field_0x8;
    f32 field_0xc;
    f32 field_0x10;
    f32 field_0x14;
    f32 field_0x18;
    f32 field_0x1c;
    f32 field_0x20;
    f32 field_0x24;
    f32 field_0x28;
    f32 field_0x2c;
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    f32 field_0x40;
    f32 field_0x44;
    f32 field_0x48;
    f32 field_0x4c;
    f32 field_0x50;
    f32 field_0x54;
    f32 field_0x58;

    UnkAnimRecord_l176784& operator=(const UnkAnimRecord_l176784& other) {
        if (&other != this) {
            field_0x0 = other.field_0x0;
            field_0x4 = other.field_0x4;
            field_0x8 = other.field_0x8;
            field_0xc = other.field_0xc;
            field_0x10 = other.field_0x10;
            field_0x14 = other.field_0x14;
            field_0x18 = other.field_0x18;
            field_0x1c = other.field_0x1c;
            field_0x20 = other.field_0x20;
            field_0x24 = other.field_0x24;
            field_0x28 = other.field_0x28;
            field_0x2c = other.field_0x2c;
            field_0x30 = other.field_0x30;
            field_0x34 = other.field_0x34;
            field_0x38 = other.field_0x38;
            field_0x3c = other.field_0x3c;
            field_0x40 = other.field_0x40;
            field_0x44 = other.field_0x44;
            field_0x48 = other.field_0x48;
            field_0x4c = other.field_0x4c;
            field_0x50 = other.field_0x50;
            field_0x54 = other.field_0x54;
            field_0x58 = other.field_0x58;
        }
        return *this;
    }
};

struct UnkAnimEffectHolder_l176784 {
    u8 pad_0x0[0x50];
    UnkAnimEffect* effect;
};

struct UnkAnimSource_l176784 {
    u8 pad_0x0[0xa0];
    CStrId name;
};

struct UnkAnimSourceHolder_l176784 {
    u8 pad_0x0[0x10];
    UnkAnimSource_l176784* source;
};

struct UnkAnimOwner_l176784 {
    u8 pad_0x0[0x38];
    UnkAnimRecord_l176784 record;  // 0x38
    u8 pad_0x94[0x1f0 - 0x94];
    void* field_0x1f0;
    u8 pad_0x1f8[0x30];
    UnkAnimEffectHolder_l176784* effects;  // 0x228
    u8 pad_0x230[0x70];
    UnkCollisionContainer* collision;  // 0x2a0
    u8 pad_0x2a8[0xf0];
    UnkAnimSourceHolder_l176784* field_0x398;
};
static_assert(__builtin_offsetof(UnkAnimOwner_l176784, field_0x1f0) == 0x1f0, "layout");
static_assert(__builtin_offsetof(UnkAnimOwner_l176784, effects) == 0x228, "layout");
static_assert(__builtin_offsetof(UnkAnimOwner_l176784, collision) == 0x2a0, "layout");
static_assert(__builtin_offsetof(UnkAnimOwner_l176784, field_0x398) == 0x398, "layout");

struct UnkAnimComponent_l176784 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual void vf_0x160();
    virtual void vf_0x168(f32 time);

    u8 pad_0x8[0x28];
    UnkAnimOwner_l176784* owner;  // 0x30
    u8 pad_0x38[8];
    UnkTrackSet* tracks;  // 0x40
    u8 pad_0x48[0x258 - 0x48];
    UnkAnimRecord_l176784 record;  // 0x258
    u8 pad_0x2b4[0x14];
    u32 field_0x2c8;

    // The track of the entry whose name hash is `key`, or null.
    void* TrackData(u64 key) {
        UnkTrackEntry** it = tracks->field_0xc8.Find(key);
        return (it != tracks->field_0xc8.End() && *it) ? (*it)->field_0x70 : nullptr;
    }

    // fn_7101176690 inlined: the action `id` names to the track whose name hash is `key`.
    void Send(u64 id, u64 key, UnkFlags_7101176ea8 flags, const UnkS16& a, const UnkS12& b,
              UnkFlags_7101176ea8 mask) {
        UnkStruct_7101176ea8* target = reinterpret_cast<UnkStruct_7101176ea8*>(this);
        void* r = fn_7101176d34(target, reinterpret_cast<void*>(id));
        fn_7101177044(target, r, -1, TrackData(key), flags, a, b, mask);
    }

    // Rewinds to 0 and runs an empty update over the tracks.
    void Reset() {
        vf_0x168(0.0f);
        UnkAnimUpdate_l176784 update;
        update.flag = false;
        update.dt = 0.0f;
        fn_71002d58e4(reinterpret_cast<UnkParent*>(tracks), &update.dt);
    }
};
static_assert(__builtin_offsetof(UnkAnimComponent_l176784, record) == 0x258, "layout");
static_assert(__builtin_offsetof(UnkAnimComponent_l176784, field_0x2c8) == 0x2c8, "layout");

void fn_7101176784(UnkAnimComponent_l176784* self) {
    UnkAnimSourceHolder_l176784* holder = self->owner->field_0x398;
    if (holder && holder->source && holder->source->name.data != &g_unk_7101d47108->empty) {
        u64 id = holder->source->name.data->Key();
        u32 mask = UnkAll_SpitSend().value;
        self->Send(id, kMainTrack_l176784, 1, reinterpret_cast<const UnkS16&>(g_unk_710164b968),
                   reinterpret_cast<const UnkS12&>(g_unk_7172769740), mask);
        self->Reset();
        UnkAnimEffectHolder_l176784* effects = self->owner->effects;
        if (effects && effects->effect)
            fn_7100143618(effects->effect, true, 0.0f);
        if (UnkCollisionContainer* collision = self->owner->collision)
            fn_71008615b4(collision);
    }
    self->record = self->owner->record;
    if (!self->owner->field_0x1f0)
        self->field_0x2c8 = 1;
}
