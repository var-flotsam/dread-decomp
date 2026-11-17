// CHyperBeamGun's unit (tools/tu_map.py): arming the hyper beam. Ends a pending shot, sends the arming
// request to the owner's character class and the timeline value, sizes the charge effect, restarts the two
// charge sounds and refreshes the CCharClassFireActionTrack track. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkDestructors.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CAnimationComponent.h"
#include "game/CFXComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU2523.h"
#include "game/UnkTimelineCalls.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;
using base::global::UnkStringData;
using base::reflection::UnkReflTypeId;
using base::reflection::UnkReflTypeIdTag;

using UnkPathMember_l843a50 = base::UnkStrIdMemberOf<g_unk_7101d47200>;

// A string id copied from another (retained), released in two steps.
struct UnkSoundName_l843a50 {
    UnkStringData* data;

    explicit UnkSoundName_l843a50(const CStrId& id) : data(id.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    ~UnkSoundName_l843a50() {
        using base::UnkStrIdMember;
        reinterpret_cast<UnkStrIdMember*>(this)->~UnkStrIdMember();
    }
};

// A file path copied from another (retained), released in two steps.
struct UnkSoundPath_l843a50 {
    UnkStringData* data;

    explicit UnkSoundPath_l843a50(const CFilePathStrId& id) : data(id.data) {
        if (&g_unk_7101d47200->empty != data)
            ++data->ref_count;
    }
    ~UnkSoundPath_l843a50() { reinterpret_cast<UnkPathMember_l843a50*>(this)->~UnkPathMember_l843a50(); }
    void operator=(const UnkPathMember_l843a50& other) { *reinterpret_cast<UnkPathMember_l843a50*>(this) = other; }
};

// The sound description the owner plays, with its defaults.
struct UnkSoundDesc_l843a50 {
    u32 kind;
    u32 field_0x4;
    u64 field_0x8;
    u32 field_0x10;
    u32 field_0x14;  // unset in the original
    UnkSoundName_l843a50 name;  // 0x18
    UnkSoundPath_l843a50 path;  // 0x20
    f32 position[3];            // 0x28
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
    f32 field_0x5c;
    f32 field_0x60;
    f32 field_0x64;
    f32 field_0x68;
    bool field_0x6c;
    bool field_0x6d;
    bool field_0x6e;

    UnkSoundDesc_l843a50()
        : kind(0), field_0x4(0), field_0x8(0), field_0x10(0), name(g_unk_7101d470e0), path(g_unk_7101d470e8),
          position{0.0f, 0.0f, 0.0f}, field_0x34(-1.0f), field_0x38(1.0f), field_0x3c(500.0f), field_0x40(2000.0f),
          field_0x44(1.0f), field_0x48(0.0f), field_0x4c(0.0f), field_0x50(0.0f), field_0x54(-1.0f),
          field_0x58(-1.0f), field_0x5c(-1.0f), field_0x60(0.5f), field_0x64(5.0f), field_0x68(10.0f),
          field_0x6c(false), field_0x6d(false), field_0x6e(false) {}
};
static_assert(__builtin_offsetof(UnkSoundDesc_l843a50, field_0x6c) == 0x6c, "layout");

struct UnkEntityWithSound;
UnkSoundHandle fn_7100d7c3b0(UnkEntityWithSound* self, const u32* params, bool a, bool b, bool c);

// The handle's view of the reference block.
struct UnkSoundRefView_l843a50 {
    void* playing;
    s64 serial;
};

static inline bool IsPlaying(const UnkSoundHandle& handle) {
    const UnkSoundRefView_l843a50* ref = reinterpret_cast<const UnkSoundRefView_l843a50*>(handle.ref);
    return ref && ref->serial == handle.serial && handle.serial != -1 && ref->playing;
}

// The scale the charge effect is given (its vf_0x20).
struct UnkScale_l843a50 {
    f32 x;
    f32 y;
    f32 z;
};

struct UnkChargePart_l843a50 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(const UnkScale_l843a50* scale);
};

// The hyper beam's tunables: the charge times.
struct UnkHyperBeamTunable_l843a50 {
    u8 pad_0x0[0x38];
    f32 field_0x38;
    f32 field_0x3c;
    f32 field_0x40;
    f32 field_0x44;
};
extern UnkHyperBeamTunable_l843a50 g_unk_7172726520;

struct CHyperBeamGun_l843a50 {
    u8 pad_0x0[0x18];
    UnkActor* owner;  // 0x18
    u8 pad_0x20[0x1884];
    u32 field_0x18a4;
    u8 pad_0x18a8[0x10];
    UnkSoundHandle sound_a;  // 0x18b8
    UnkSoundHandle sound_b;  // 0x18c8
    u8 pad_0x18d8[0x20];
    bool field_0x18f8;
    bool field_0x18f9;
    u8 pad_0x18fa[0x2];
    f32 field_0x18fc;
    bool field_0x1900;
    bool field_0x1901;
    bool field_0x1902;
    u8 pad_0x1903[0x1];
    f32 field_0x1904;
    f32 field_0x1908;
    u8 pad_0x190c[0x4];
    f32 field_0x1910;
};
static_assert(__builtin_offsetof(CHyperBeamGun_l843a50, field_0x1910) == 0x1910, "layout");

struct UnkHyperBeamGun;
void fn_71008449ec(UnkHyperBeamGun* self, bool charged, bool cancelled);

// The timeline ids of the arming.
extern const u64 g_unk_710164ac90;
extern const u64 g_unk_710164ac98;
// The charge sounds' file paths.
__attribute__((visibility("hidden"))) extern UnkPathMember_l843a50 g_unk_71727265c0;
__attribute__((visibility("hidden"))) extern UnkPathMember_l843a50 g_unk_71727265c8;
// The name of the track that arms the beam.
__attribute__((visibility("hidden"))) extern CStrId g_unk_71727265b8;

// Sends the mode `value` under `hash` to the character class, with its descriptor and the type id
// (refl_EmitU32 of base/reflection/UnkReflStatics.h, with the two arguments in the caller's scope).
static inline void SendMode(void* target, u64 hash, base::reflection::UnkReflTypeIdArg* mode, u32 value,
                            const UnkDesc_SpitSend* desc, base::reflection::UnkReflTypeIdArg* type_id) {
    mode->id = value;
    type_id->id = UnkReflTypeId<UnkReflTypeIdTag>::id();
    fn_7101176690(target, hash, 0xf4061ff25a2163d6ull, mode, &g_unk_710164b958, desc, type_id);
}

static inline void SendTimelineId(UnkTimelineComponent* timeline, const u64* id) {
    base::reflection::UnkReflTypeIdArg arg;
    arg.id = UnkReflTypeId<UnkReflTypeIdTag>::id();
    fn_7100a2cb7c(timeline, id, &arg.id, false, -1.0f);
}

static inline void StopSound(CHyperBeamGun_l843a50* self, UnkSoundHandle* sound) {
    if (IsPlaying(*sound))
        fn_7100d7c510(reinterpret_cast<UnkShipRechargeEntity_b49a4*>(self->owner), sound, 0.0f, 0.0f);
}

void fn_7100843a50(char* raw, bool on) {
    CHyperBeamGun_l843a50* self = reinterpret_cast<CHyperBeamGun_l843a50*>(raw);
    if (self->field_0x1900 && self->field_0x1902) {
        fn_71008449ec(reinterpret_cast<UnkHyperBeamGun*>(self), on, !self->field_0x1901);
        // A shot still pending once ended skips the arming: only the fire track is refreshed. The
        // jump passes the declarations below, so the original keeps each in its own stack slot.
        if (self->field_0x1900 && self->field_0x1902)
            goto refresh_track;
    }
    // The arguments of the four requests below.
    base::reflection::UnkReflTypeIdArg armed_mode, armed_type;
    base::reflection::UnkReflTypeIdArg unarmed_mode, unarmed_type;
    base::reflection::UnkReflTypeIdArg track_mode, track_type;
    base::reflection::UnkReflTypeIdArg other_mode, other_type;
    {
        const u64* key = &g_unk_710164ac98;
        if (UnkAnimationComponent* animation = static_cast<UnkAnimationComponent*>(self->owner->field_0x220)) {
            if (self->field_0x18f8) {
                if (fn_710117ae38(animation, 0x2b58fe4f8b7b9dfdull, false))
                    SendMode(self->owner->field_0x220, 0x90b0b24b6a6a9e1dull, &armed_mode, 1, &g_unk_7172769794,
                             &armed_type);
                else
                    SendMode(self->owner->field_0x220, 0x90b0b24b6a6a9e1dull, &unarmed_mode, 0, &g_unk_7172769740,
                             &unarmed_type);
                key = &g_unk_710164ac90;
            } else if (fn_710117af10(reinterpret_cast<UnkStruct_710117af10*>(animation),
                                     reinterpret_cast<UnkCls_710117af10**>(&g_unk_71727265b8), false)) {
                SendMode(self->owner->field_0x220, 0x1f116f3ab15940d6ull, &track_mode, 1, &g_unk_7172769740,
                         &track_type);
            } else if (fn_710117ae38(animation, 0x9d4fd5f36b87313aull, false)) {
                SendMode(self->owner->field_0x220, 0x1f370da637b7fc4bull, &other_mode, 1, &g_unk_7172769740,
                         &other_type);
            }
        }
        self->field_0x1910 = 0.0f;
        if (UnkTimelineComponent* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
            SendTimelineId(timeline, key);
            self->field_0x1910 = 0x1.1111p-4f;
        }

        UnkWorld* world = static_cast<UnkWorld*>(self->owner->field_0x280);
        if (UnkChargePart_l843a50* part =
                reinterpret_cast<UnkChargePart_l843a50*>(fn_7101152b90(world, 0x805489787ebd4a19ull))) {
            f32 time;
            f32 base;
            if (self->field_0x1900) {
                if (self->field_0x18a4 != 0)
                    time = g_unk_7172726520.field_0x44;
                else if (self->field_0x18fc > 0.4f)
                    time = g_unk_7172726520.field_0x3c;
                else
                    time = g_unk_7172726520.field_0x40;
                base = 0.0f;
            } else {
                time = g_unk_7172726520.field_0x38;
                base = 0.2f;
            }
            self->field_0x1908 = time;
            UnkScale_l843a50 scale;
            if (self->field_0x1910 > 0.0f) {
                scale.x = 0.0f;
                scale.y = 0.0f;
                scale.z = 0.0f;
            } else {
                f32 s = base + self->field_0x1904 / time * 0.5f;
                scale.x = s;
                scale.y = s;
                scale.z = s;
            }
            part->vf_0x20(&scale);
        }

        if (!self->field_0x1900) {
            StopSound(self, &self->sound_a);
            StopSound(self, &self->sound_b);
            UnkSoundDesc_l843a50 desc;
            desc.kind = 0;
            desc.path = g_unk_71727265c0;
            desc.field_0x5c = 2.0f;
            desc.field_0x6d = false;
            desc.field_0x6e = true;
            self->sound_a = fn_7100d7c3b0(reinterpret_cast<UnkEntityWithSound*>(self->owner),
                                          reinterpret_cast<const u32*>(&desc), false, true, true);
            desc.path = g_unk_71727265c8;
            self->sound_b = fn_7100d7c3b0(reinterpret_cast<UnkEntityWithSound*>(self->owner),
                                          reinterpret_cast<const u32*>(&desc), false, true, true);
        }
    }

refresh_track:
    UnkTrackQuery query = {fn_7100cf6ef8(), nullptr, -1.0f};
    if (UnkAnimationComponent* animation = static_cast<UnkAnimationComponent*>(self->owner->field_0x220)) {
        fn_710117b08c(animation, &query);
        if (query.result && query.result->sources.data != &g_unk_7101d47108->empty)
            fn_710117af10(reinterpret_cast<UnkStruct_710117af10*>(animation),
                          reinterpret_cast<UnkCls_710117af10**>(&query.result->sources), false);
    }
}
