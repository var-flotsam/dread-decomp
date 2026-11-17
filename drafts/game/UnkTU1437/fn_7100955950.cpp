// UnkTU1437: picks the animation an actor turns or idles with. The owner's turn source is asked
// first; failing that, the facing (the source's direction against the actor's transform) chooses
// between the front and back sets, a tweak track of the matching class may fill the request, and
// otherwise one of the named animations is taken. `state` remembers the side (1, 2 front, 3 back).
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAnimationComponent.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::global::CStrId;

// The request the turn source fills: the animation's name key first, -1 when there is none.
struct UnkTurnRequest {
    u64 id;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    u32 field_0x20;
    u32 field_0x24;
    u32 field_0x28;

    UnkTurnRequest& operator=(const UnkTurnRequest& other) {
        id = other.id;
        field_0x8 = other.field_0x8;
        field_0x10 = other.field_0x10;
        field_0x18 = other.field_0x18;
        field_0x20 = other.field_0x20;
        field_0x24 = other.field_0x24;
        field_0x28 = other.field_0x28;
        return *this;
    }
    u64 Key() const { return id; }
    void SetKey(u64 k) { id = k; }
};


// The record of a tweak track: it fills a request.
struct UnkTweakTrack {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18(UnkTurnRequest* request);
    u8 pad_0x8[0x2c];
    bool field_0x34;
};

// The turn source: its direction and whether it turns at all.
struct UnkTurnSource {
    u8 pad_0x0[0x40];
    f32 dir_x;  // 0x40
    f32 dir_y;  // 0x44
    f32 dir_z;  // 0x48
    u8 pad_0x4c[0x30];
    f32 field_0x7c;
};

UnkTurnRequest fn_71009766b8(UnkTurnSource* source, bool flag);
UnkTurnRequest fn_71009767f4(UnkTurnSource* source, bool flag);
UnkTurnRequest fn_7100976944(UnkTurnSource* source, bool flag);

// The animation names the owner turns with.
struct UnkTurnNames {
    u8 pad_0x0[0x8];
    CStrId field_0x8;
    CStrId field_0x10;
    CStrId field_0x18;
    CStrId field_0x20;
    CStrId field_0x28;
    CStrId field_0x30;
    CStrId field_0x38;
};

struct UnkTurnNamesHolder {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual const UnkTurnNames* vf_0x40();
};

struct UnkTurnActorPart {
    u8 pad_0x0[0x1610];
    void* field_0x1610;
};

struct UnkTurnActor {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
    u8 pad_0x78[0x1a0];
    UnkTurnSource* turn;               // 0x218
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x18];
    UnkTurnActorPart* field_0x240;
};
static_assert(__builtin_offsetof(UnkTurnActor, field_0x240) == 0x240, "UnkTurnActor layout");

struct UnkTurnOwner {
    u8 pad_0x0[0x30];
    UnkTurnActor* actor;  // 0x30
    u8 pad_0x38[0x8];
    UnkTurnNamesHolder* names;  // 0x40
};

static inline bool IsEmpty(const CStrId& id) { return id.data == &g_unk_7101d47108->empty; }

static inline bool HasEntry(UnkAnimationComponent* animation, u64 key) {
    return fn_7101176d34(reinterpret_cast<UnkStruct_7101176ea8*>(animation), reinterpret_cast<void*>(key)) != nullptr;
}

static inline bool IsPlaying(UnkAnimationComponent* animation, const CStrId& id) {
    return fn_710117ae38(animation, id.data->Key(), false);
}

static inline bool IsPlayingName(UnkAnimationComponent* animation, const CStrId& id) {
    return fn_710117af10(reinterpret_cast<UnkStruct_710117af10*>(animation),
                         reinterpret_cast<UnkCls_710117af10**>(const_cast<CStrId*>(&id)), false);
}

// Lets the tweak track of class `cls` the actor is in fill the request; false when there is none.
static inline bool FillFromTweak(UnkTurnOwner* self, const base::reflection::CClass* cls, UnkTurnRequest* out) {
    if (UnkAnimationComponent* animation = self->actor->animation) {
        UnkTrackQuery query = {cls, nullptr, -1.0f};
        fn_710117b08c(animation, &query);
        if (query.result) {
            reinterpret_cast<UnkTweakTrack*>(query.result)->vf_0x18(out);
            return true;
        }
    }
    return false;
}

void fn_7100955950(UnkTurnOwner* self, u32* state, UnkTurnRequest* out, bool keep) {
    UnkTurnActor* actor = self->actor;
    if (!actor)
        return;
    UnkTurnSource* source = actor->turn;
    if (!source)
        return;
    UnkAnimationComponent* animation = actor->animation;
    UnkTurnActorPart* part = actor->field_0x240;
    void* part_data = part ? part->field_0x1610 : nullptr;
    const UnkTurnNames* names = self->names->vf_0x40();

    *out = fn_71009766b8(source, false);
    if (out->Key() == (u64)-1) {
      if (animation && part && part_data) {
        if (source->field_0x7c == 0.0f) {
            *out = fn_71009767f4(source, true);
            if (out->Key() == (u64)-1) {
                if (*state != 1 && *state == 2 && !IsEmpty(names->field_0x30) &&
                    HasEntry(animation, names->field_0x30.data->Key())) {
                    if (!FillFromTweak(self, fn_7100c55bc8(), out))
                        out->SetKey(names->field_0x30.data->Key());
                } else if (*state == 3 && !IsEmpty(names->field_0x38) &&
                           HasEntry(animation, names->field_0x38.data->Key())) {
                    if (!FillFromTweak(self, fn_7100c55bc8(), out))
                        out->SetKey(names->field_0x38.data->Key());
                } else if ((!IsEmpty(names->field_0x30) && IsPlayingName(animation, names->field_0x30)) ||
                           (!IsEmpty(names->field_0x38) && IsPlayingName(animation, names->field_0x38))) {
                    const CStrId* id = reinterpret_cast<const CStrId*>(
                        static_cast<u8*>(fn_7101177d2c(animation, 0xf4061ff25a2163d6ull)) + 0x38);
                    out->SetKey(id->data->Key());
                } else {
                    out->SetKey(names->field_0x8.data->Key());
                }
            }
            *state = 1;
        } else {
            f32 x = source->dir_x;
            f32 y = source->dir_y;
            f32 z = source->dir_z;
            const UnkMatrix44& m = fn_710002614c(&self->actor->transform);
            f32 facing = x * m.m[2] + y * m.m[6] + z * m.m[10];
            if (!(facing >= 0.0f)) {
                UnkTrackQuery query = {fn_7100c56928(), nullptr, -1.0f};
                fn_710117b08c(animation, &query);
                if (query.result && !keep) {
                    reinterpret_cast<UnkTweakTrack*>(query.result)->vf_0x18(out);
                } else {
                    const CStrId* id;
                    if (*state != 3 && !IsEmpty(names->field_0x28) &&
                        HasEntry(animation, names->field_0x28.data->Key()) && !IsPlaying(animation, names->field_0x10))
                        id = &names->field_0x28;
                    else if (!IsEmpty(names->field_0x28) && IsPlaying(animation, names->field_0x28))
                        id = &names->field_0x28;
                    else
                        id = &names->field_0x18;
                    out->SetKey(id->data->Key());
                }
                *state = 3;
            } else {
                *out = fn_7100976944(source, true);
                if (out->Key() == (u64)-1) {
                    UnkTrackQuery query = {fn_7100c59098(), nullptr, -1.0f};
                    fn_710117b08c(animation, &query);
                    if (query.result && !keep) {
                        reinterpret_cast<UnkTweakTrack*>(query.result)->vf_0x18(out);
                        if (!reinterpret_cast<UnkTweakTrack*>(query.result)->field_0x34)
                            out->field_0x24 |= 0x8000;
                    } else {
                        const CStrId* id;
                        if (*state != 2 && !IsEmpty(names->field_0x20) &&
                            HasEntry(animation, names->field_0x20.data->Key()) &&
                            !IsPlaying(animation, names->field_0x10))
                            id = &names->field_0x20;
                        else if (!IsEmpty(names->field_0x20) && IsPlaying(animation, names->field_0x20))
                            id = &names->field_0x20;
                        else
                            id = &names->field_0x10;
                        out->SetKey(id->data->Key());
                    }
                }
                *state = 2;
            }
        }
      }
    }
    if (out->Key() == (u64)-1 || !HasEntry(animation, out->Key()))
        out->SetKey(names->field_0x8.data->Key());
}
