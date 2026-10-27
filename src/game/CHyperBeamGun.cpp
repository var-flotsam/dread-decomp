// CHyperBeamGun's unit (the class name from reflection): ending a hyper beam shot. It sends the
// end of the shot to the owner's character class (unless one of three tracks is playing), the
// matching timeline value, fades the shot's sounds out and starts the release sounds, tells the
// game manager (HYPER_BEAM_COMMANDERX) and starts the release effect. Names and layouts are
// placeholders inferred from the code.
#include "../game_s2/UnkSpitSend.h"
#include "base/math/UnkZeroVector3.h"
#include "base/reflection/UnkReflStatics.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/snd/UnkSound.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;
using base::math::CVector3D;
using base::reflection::UnkReflTypeId;
using base::reflection::UnkReflTypeIdTag;

// A handle on a sound: the reference and the serial it was taken with; the sound is gone once the
// reference's serial differs (or the handle's is -1).
struct UnkHyperBeamSoundRef {
    UnkSound* sound;
    s64 serial;
};
struct UnkHyperBeamSoundHandle {
    UnkHyperBeamSoundRef* ref;
    s64 serial;

    UnkSound* Get() const {
        if (ref && ref->serial == serial && serial != -1)
            return ref->sound;
        return nullptr;
    }
};

// The same kind of handle on an effect (fn_7100072328 fades its target).
struct UnkHyperBeamEffectRef {
    void* target;
    s64 serial;
};
struct UnkHyperBeamEffectHandle {
    UnkHyperBeamEffectRef* ref;
    s64 serial;

    void* Get() const {
        if (ref && ref->serial == serial && serial != -1)
            return ref->target;
        return nullptr;
    }
};

// The data of a string id with a plain count (these ids increment it with int alias info).
struct UnkHyperBeamStringData {
    u8 unk_0x0[0x8];
    s32 ref_count;
};

// A file-path id (pool g_unk_7101d47200) whose count is a plain int.
struct UnkHyperBeamPathId {
    UnkStringData* data;

    explicit UnkHyperBeamPathId(const base::global::UnkFilePathStrIdBase& other) : data(other.data) {
        if (&g_unk_7101d47200->empty != data)
            ++reinterpret_cast<UnkHyperBeamStringData*>(data)->ref_count;
    }
    UnkHyperBeamPathId& operator=(const base::global::UnkFilePathStrIdBase& other) {
        if (other.data != data) {
            UnkStringData* d = other.data;
            UnkStringPool* pool = g_unk_7101d47200;
            if (d != &pool->empty)
                ++reinterpret_cast<UnkHyperBeamStringData*>(d)->ref_count;
            if (data != &pool->empty) {
                if (--reinterpret_cast<UnkHyperBeamStringData*>(data)->ref_count == 0)
                    fn_71000803a4(pool, data);
            }
            data = d;
        }
        return *this;
    }
    ~UnkHyperBeamPathId() {
        if (UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty) {
                if (--reinterpret_cast<UnkHyperBeamStringData*>(data)->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
    }
};

// What the release effect is created from (fn_7100d7c614).
struct UnkHyperBeamEffectDesc {
    UnkHyperBeamPathId path;
    u64 field_0x8;
    u32 field_0x10;
    CVector3D field_0x14;
    f32 field_0x20[4];
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    bool field_0x40;
    bool field_0x41;

    UnkHyperBeamEffectDesc()
        : path(g_unk_7101d470e8), field_0x8(0), field_0x10(0x7fffffff), field_0x14(g_unk_710164779c),
          field_0x20{0.0f, 0.0f, 1.0f, 1.0f}, field_0x30(0.5f), field_0x34(0.0f), field_0x38(0.0f),
          field_0x3c(1.0f), field_0x40(false), field_0x41(false) {}
};
static_assert(sizeof(UnkHyperBeamEffectDesc) == 0x48, "");

// A 12-byte zeroed argument (as src/game_s2/fn_710035131c.cpp's).
struct UnkVec {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
};

// The part of the owner's object that is told the shot ended (its vf_0x20).
struct UnkHyperBeamPart {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkVec* arg);
};
struct UnkPart;
struct UnkWorld;
UnkPart* fn_7101152b90(UnkWorld* world, u64 hash);

struct UnkHyperBeamGun {
    u8 pad_0x0[0x18];
    UnkActor* owner;
    u8 pad_0x20[0x1854];
    u32 field_0x1874;
    u8 pad_0x1878[0x28];
    u32 field_0x18a0;
    u32 field_0x18a4;
    UnkHyperBeamSoundHandle field_0x18a8;
    u8 pad_0x18b8[0x20];
    UnkHyperBeamSoundHandle field_0x18d8;
    UnkHyperBeamEffectHandle field_0x18e8;
    bool field_0x18f8;
    bool field_0x18f9;
    f32 field_0x18fc;
};
static_assert(__builtin_offsetof(UnkHyperBeamGun, field_0x18fc) == 0x18fc, "");

extern void* g_unk_71726af0b0;  // what plays the sounds at a position
// The timeline ids of the shot's ends.
extern const u64 g_unk_710164acb0;
extern const u64 g_unk_710164acb8;
extern const u64 g_unk_710164acc0;
extern const u64 g_unk_710164acc8;
// The release effect's file path.
extern base::global::CFilePathStrId g_unk_71727265a0 __attribute__((visibility("hidden")));

UnkHyperBeamSoundHandle fn_7100ed6910(void* player, const CVector3D* pos, u32 id, u32 kind, bool flag);
UnkHyperBeamEffectHandle fn_7100d7c614(UnkActor* owner, const UnkHyperBeamEffectDesc* desc, const CStrId* name,
                                       u32 a, u32 b, u32 c);
void fn_7100e038b0(UnkGameManager* dispatcher, void* key, UnkVec* vec, u64 arg);
void fn_7100072328(void* target, f32 a, f32 b);

// Sets the timeline value `id` (with the type id, after a delay of -1).
static inline void SendTimelineId(UnkTimelineComponent* timeline, const u64* id) {
    base::reflection::UnkReflTypeIdArg arg;
    arg.id = UnkReflTypeId<UnkReflTypeIdTag>::id();
    fn_7100a2cb7c(timeline, id, &arg.id, false, -1.0f);
}

static inline CVector3D OwnerPosition(const UnkActor* owner) {
    CVector3D pos;
    pos.x = owner->field_0x44;
    pos.y = owner->field_0x54;
    pos.z = owner->field_0x64;
    return pos;
}

// NON_MATCHING(minor): scheduling and register allocation: before each fn_7100ed6910 call the original
// zeroes w4 ahead of loading w2 (ours: after), and it keeps the effect's 0.5 and (0, 1) constants in
// x23 and x24 where ours swaps them; the same loads, stores and calls in the same order.
void fn_71008449ec(UnkHyperBeamGun* self, bool charged, bool cancelled) {
    // The end-of-shot request's value and type id (refl_EmitU32 of base/reflection/UnkReflStatics.h,
    // with the two arguments in the function's scope).
    base::reflection::UnkReflTypeIdArg value;
    base::reflection::UnkReflTypeIdArg type_id;
    if (self->field_0x18f8) {
        if (!fn_710117ae38(static_cast<UnkAnimationComponent*>(self->owner->field_0x220), 0x731b29cfbda790eeull, false) &&
            !fn_710117ae38(static_cast<UnkAnimationComponent*>(self->owner->field_0x220), 0x33963557fd146e7aull, false) &&
            !fn_710117ae38(static_cast<UnkAnimationComponent*>(self->owner->field_0x220), 0xc5ab5dad085632f3ull, false)) {
            void* target = self->owner->field_0x220;
            value.id = 0;
            type_id.id = UnkReflTypeId<UnkReflTypeIdTag>::id();
            fn_7101176690(target, 0x90b0b24b6a6a9e1dull, 0xf4061ff25a2163d6ull, &value, &g_unk_710164b958,
                          &g_unk_7172769740, &type_id);
        }
    }
    if (UnkTimelineComponent* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
        if (self->field_0x18f8) {
            if (cancelled)
                SendTimelineId(timeline, &g_unk_710164acb0);
            else
                SendTimelineId(timeline, &g_unk_710164acb8);
        } else {
            if (charged && self->field_0x18fc > 0.4f)
                SendTimelineId(timeline, &g_unk_710164acc0);
            else
                SendTimelineId(timeline, &g_unk_710164acc8);
            if (UnkSound* sound = self->field_0x18a8.Get())
                fn_7100105a80(sound, 0.2f, 0.0f);
            CVector3D pos = OwnerPosition(self->owner);
            self->field_0x18a8 = fn_7100ed6910(g_unk_71726af0b0, &pos, self->field_0x1874, 3, false);
        }
    }
    UnkWorld* world = *reinterpret_cast<UnkWorld**>(reinterpret_cast<char*>(self->owner) + 0x280);
    if (UnkHyperBeamPart* part = reinterpret_cast<UnkHyperBeamPart*>(fn_7101152b90(world, 0x805489787ebd4a19ull))) {
        UnkVec arg;
        arg.field_0x0 = 0;
        arg.field_0x4 = 0;
        arg.field_0x8 = 0;
        part->vf_0x20(&arg);
    }
    {
        UnkGameManager* gm = g_unk_7101d47310;
        CStrId name("HYPER_BEAM_COMMANDERX", true);
        UnkVec arg;
        arg.field_0x0 = 0;
        arg.field_0x4 = 0;
        arg.field_0x8 = 0;
        fn_7100e038b0(gm, &name, &arg, 0);
    }
    if (UnkSound* sound = self->field_0x18d8.Get())
        fn_7100105a80(sound, 0.2f, 0.0f);
    {
        CVector3D pos = OwnerPosition(self->owner);
        self->field_0x18d8 = fn_7100ed6910(g_unk_71726af0b0, &pos, self->field_0x1874, 1, false);
    }
    if (void* effect = self->field_0x18e8.Get())
        fn_7100072328(effect, 0.2f, 0.0f);
    UnkHyperBeamEffectDesc desc;
    desc.path = g_unk_71727265a0;
    desc.field_0x3c = 1.0f;
    desc.field_0x10 = 0;
    desc.field_0x30 = 0.5f;
    desc.field_0x40 = true;
    desc.field_0x41 = true;
    desc.field_0x20[0] = 0.0f;
    desc.field_0x20[1] = 1.0f;
    desc.field_0x20[2] = 1.0f;
    desc.field_0x20[3] = 1.0f;
    self->field_0x18e8 = fn_7100d7c614(self->owner, &desc, &g_unk_7101d470e0, 0, 1, 1);
    self->field_0x18f9 = false;
    self->field_0x18fc = 0.0f;
    self->field_0x18a4 = self->field_0x18a0;
}
