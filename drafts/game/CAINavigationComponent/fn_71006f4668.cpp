// CAINavigationComponent's unit (tools/tu_map.py, unit 1113): the update. While the owner is held
// (a state of 2 at +0x1b8, a CCharClassIgnoreAINavigationTrack track, or frozen) navigation is off:
// the track's named point is followed, and when the hold ends the movement is told the speed it had.
// Otherwise the navigation object updates and the component takes its position. Names and layouts
// are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CComponent.h"
#include "game/CEnemyMovement.h"
#include "game/CFrozenComponent.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/behaviortree/CSwitchBetweenAIModesAction.h"
#include "types.h"

using base::global::CStrId;

struct UnkNavVec_l6f4668 {
    f32 x;
    f32 y;
    f32 z;
};

struct UnkNavState_l6f4668 {
    u8 pad_0x0[0x70];
    s32 state;
};

struct UnkNavMovement_l6f4668 {
    u8 pad_0x0[0x7c];
    u32 field_0x7c;
};

struct UnkNavOwner_l6f4668 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44: the transform's translation column
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x150];
    UnkNavState_l6f4668* field_0x1b8;
    u8 pad_0x1c0[0x30];
    UnkNavMovement_l6f4668* field_0x1f0;
    u8 pad_0x1f8[0x20];
    UnkEffectSink* field_0x218;
    UnkAnimationComponent* field_0x220;
    void* field_0x228;  // the model updater

    UnkNavVec_l6f4668 Position() const { return {x, y, z}; }
};
static_assert(__builtin_offsetof(UnkNavOwner_l6f4668, field_0x228) == 0x228, "UnkNavOwner_l6f4668 layout");

void* fn_71006f4a30(UnkNavOwner_l6f4668* owner);

struct UnkNavTarget_l6f4668;

struct UnkNavigator_l6f4668 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(f32 dt);
    u8 pad_0x8[0x8];
    UnkNavTarget_l6f4668* target;  // 0x10
};

struct UnkNavTarget_l6f4668 {
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
    virtual void vf_0x60(const UnkNavVec_l6f4668* speed);
    u8 pad_0x8[0x30];
    UnkNavVec_l6f4668 position;  // 0x38
};

template <class T>
struct UnkNavOpt_l6f4668 {
    T value;
    bool has;
};

// What the owner's movement (+0x218) is sent when a hold starts.
struct UnkNavSinkParams_l6f4668 {
    void** vtable;
    UnkNavOpt_l6f4668<u32> field_0x8;
    UnkNavOpt_l6f4668<f32> field_0x10;
    UnkNavOpt_l6f4668<UnkNavVec_l6f4668> field_0x18;
    UnkNavOpt_l6f4668<UnkNavVec_l6f4668> field_0x28;
    u32 kind;  // 0x38
};

struct UnkAINavigation_l6f4668 {
    u8 pad_0x0[0x30];
    UnkNavOwner_l6f4668* owner;
    u8 pad_0x38[0x8];
    UnkNavVec_l6f4668 position;  // 0x40
    bool enabled;     // 0x4c
    bool was_held;    // 0x4d
    bool held;        // 0x4e
    CStrId point;     // 0x50: the named point the hold follows
    UnkNavVec_l6f4668 start;  // 0x58: where that point was
    f32 elapsed;      // 0x64: since then
    u8 pad_0x68[0x48];
    UnkNavigator_l6f4668** navigator;  // 0xb0
};
static_assert(__builtin_offsetof(UnkAINavigation_l6f4668, navigator) == 0xb0, "UnkAINavigation_l6f4668 layout");

void fn_71006f4668(UnkAINavigation_l6f4668* self, f32 dt) {
    fn_71010c4234(reinterpret_cast<char*>(self));
    if (!self->enabled)
        return;
    if (self->point.data != &g_unk_7101d47108->empty)
        self->elapsed += dt;
    self->was_held = self->held;
    self->held = false;

    UnkNavState_l6f4668* state = self->owner->field_0x1b8;
    if (state && state->state == 2) {
        self->held = true;
        self->point = g_unk_7101d470e0;
    }
    if (!self->held) {
        if (self->owner->field_0x220 && self->owner->field_0x228) {
            UnkTrackQuery query = {fn_7100d043f8(), nullptr, -1.0f};
            if (UnkAnimationComponent* anim = self->owner->field_0x220)
                fn_710117b08c(anim, &query);
            if (query.result) {
                self->held = true;
                const CStrId& point = query.result->sources;
                if (point.data != &g_unk_7101d47108->empty && point.data != self->point.data) {
                    self->point = point;
                    void* model = self->owner->field_0x228;
                    u64 key = self->point.data->Key();
                    fn_710117364c(model, &key, reinterpret_cast<UnkGrappleAnswer_b50*>(&self->start));
                    self->elapsed = 0.0f;
                }
            }
            if (self->held)
                goto held;
        }
        if (void* frozen = fn_71006f4a30(self->owner))
            if (fn_71007f9f20(frozen))
                self->held = true;
    }
held:
    if (self->was_held) {
        if (self->held)
            goto hold;
        // The hold ended: the speed the point moved at since it started.
        UnkNavVec_l6f4668 speed;
        speed.x = 0.0f;
        speed.y = 0.0f;
        speed.z = 0.0f;
        if (self->point.data != &g_unk_7101d47108->empty) {
            UnkNavVec_l6f4668 now = {0.0f, 0.0f, 0.0f};
            void* model = self->owner->field_0x228;
            u64 key = self->point.data->Key();
            fn_710117364c(model, &key, reinterpret_cast<UnkGrappleAnswer_b50*>(&now));
            UnkNavVec_l6f4668 moved = {now.x - self->start.x, now.y - self->start.y, now.z - self->start.z};
            f32 elapsed = self->elapsed;
            speed.x = moved.x / elapsed;
            speed.y = moved.y / elapsed;
            speed.z = moved.z / elapsed;
            self->point = g_unk_7101d470e0;
        }
        if (self->navigator && *self->navigator && (*self->navigator)->target)
            (*self->navigator)->target->vf_0x60(&speed);
    }
    if (!self->held) {
        if (!self->navigator || !*self->navigator || !(*self->navigator)->target)
            return;
        (*self->navigator)->vf_0x10(dt);
        UnkNavTarget_l6f4668* target = (*self->navigator)->target;
        self->position = target ? target->position : self->owner->Position();
        return;
    }
hold:
    if (UnkEffectSink* sink = self->owner->field_0x218) {
        UnkNavSinkParams_l6f4668 params;
        params.field_0x10.has = false;
        params.kind = 0;
        params.vtable = g_unk_7101a24b80 + 2;
        __builtin_memset(&params.field_0x18, 0, 13);
        __builtin_memset(&params.field_0x28, 0, 13);
        params.field_0x8.value = 0;
        params.field_0x8.has = true;
        fn_71010c7e6c(sink, &params, 0);
    } else if (UnkNavMovement_l6f4668* movement = self->owner->field_0x1f0) {
        movement->field_0x7c = 0;
    }
    self->position = self->owner->Position();
}
