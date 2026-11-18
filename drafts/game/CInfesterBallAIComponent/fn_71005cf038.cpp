// CInfesterBallAIComponent's unit (tools/tu_map.py, unit 898): the start. After the base start it takes the
// owner's movement, enables a collision entry, turns the owner, resets the roll state and callbacks, picks the
// roll direction from the body's matrix, queues the first request and resets the owner's timeline entry.
// Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkBehaviorTree.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkXformNode.h"
#include "game/malloc.h"
#include "types.h"

using base::math::CVector2D;

extern base::global::CStrId g_unk_71726ecb78 __attribute__((visibility("hidden")));
extern const base::math::CVector2D g_unk_7101647780;
struct UnkInfesterBallKey_l5cf038;
extern UnkInfesterBallKey_l5cf038 g_unk_710164a118;  // reached through the GOT

struct UnkInfesterBallOwner_l5cf038 {
    void** vtable;
    u8 pad_0x8[0x218 - 0x8];
    void* movement;  // 0x218
    u8 pad_0x220[0x2a0 - 0x220];
    UnkCollisionContainer* collision;  // 0x2a0
    u8 pad_0x2a8[0x2e8 - 0x2a8];
    void* timeline;  // 0x2e8

    void vf_0xd8(const UnkVec3f* angle, bool flag) {
        reinterpret_cast<void (*)(UnkInfesterBallOwner_l5cf038*, const UnkVec3f*, bool)>(vtable[0xd8 / 8])(
            this, angle, flag);
    }
};

struct UnkInfesterBallTuning_l5cf038 {
    u8 pad_0x0[0x50c];
    f32 duration;
};

struct UnkInfesterBallBody_l5cf038 {
    u8 pad_0x0[0x38];
    UnkXform xform;
};

// An object and the function to call on it.
struct UnkInfesterBallCallback_l5cf038 {
    void* object;
    void* function;
};

struct UnkInfesterBallOptVec_l5cf038 {
    bool set;
    CVector2D value;
};

struct UnkInfesterBallRequests_l5cf038 {
    bool field_0x1470e;
    bool field_0x1470f;
    bool field_0x14710;
    bool field_0x14711;
    bool field_0x14712;
    bool field_0x14713;
    bool field_0x14714;
};

struct UnkInfesterBallAI_l5cf038 {
    u8 pad_0x0[0x30];
    UnkInfesterBallOwner_l5cf038* owner;
    u8 pad_0x38[0x1590 - 0x38];
    UnkInfesterBallCallback_l5cf038 field_0x1590;
    u8 pad_0x15a0[0x146c8 - 0x15a0];
    void* movement;                                // 0x146c8
    UnkInfesterBallTuning_l5cf038* tuning;         // 0x146d0
    UnkInfesterBallCallback_l5cf038 field_0x146d8;
    f32 duration;                                  // 0x146e8
    f32 time;                                      // 0x146ec
    UnkInfesterBallBody_l5cf038* body;             // 0x146f0
    u8 pad_0x146f8[0x14704 - 0x146f8];
    bool field_0x14704;
    u32 state;  // 0x14708
    bool field_0x1470c;
    bool reverse;  // 0x1470d
    UnkInfesterBallRequests_l5cf038 requests;  // 0x1470e
    u8 pad_0x14715[0x14720 - 0x14715];
    UnkInfesterBallOptVec_l5cf038 field_0x14720;
    UnkInfesterBallOptVec_l5cf038 field_0x1472c;
    UnkInfesterBallOptVec_l5cf038 field_0x14738;
    UnkSegment2D field_0x14744;
    u8 pad_0x1475c[4];
    f32 phase;  // 0x14760
    u8 pad_0x14764[0x14770 - 0x14764];
    bool field_0x14770;
    u8 pad_0x14771[3];
    f32 blend;      // 0x14774
    CVector2D dir;  // 0x14778
    u64 field_0x14780;
};
static_assert(__builtin_offsetof(UnkInfesterBallAI_l5cf038, field_0x14744) == 0x14744, "layout");
static_assert(__builtin_offsetof(UnkInfesterBallAI_l5cf038, field_0x14780) == 0x14780, "layout");
static_assert(__builtin_offsetof(UnkInfesterBallAI_l5cf038, phase) == 0x14760, "layout");

bool fn_71005cf2e8(UnkInfesterBallAI_l5cf038* self);
void fn_71005d2630(UnkInfesterBallAI_l5cf038* self);
void fn_71005d2634(UnkInfesterBallAI_l5cf038* self);

void fn_71005cf038(UnkInfesterBallAI_l5cf038* self) {
    fn_7100369650(reinterpret_cast<char*>(self));
    self->movement = self->owner->movement;
    if (UnkCollisionObject* entry = fn_71008640a4(self->owner->collision, &g_unk_71726ecb78))
        fn_7100d35a98(reinterpret_cast<UnkEntry*>(entry), true);

    {
        UnkInfesterBallOwner_l5cf038* owner = self->owner;
        UnkVec3f angle(0.0f, -1.5707964f, 0.0f);
        owner->vf_0xd8(&angle, true);
    }

    self->state = 0;
    self->duration = self->tuning->duration;
    self->time = 0.0f;
    self->field_0x14780 = 0;
    self->field_0x14704 = false;
    self->field_0x1470c = false;
    self->field_0x14770 = false;
    self->requests = UnkInfesterBallRequests_l5cf038();
    self->field_0x14720.set = true;
    self->field_0x1472c.set = false;
    self->field_0x14738.set = false;
    self->blend = 0.0f;
    self->phase = 0.0f;
    {
        // field_0x14 is unset in the original.
        UnkSegment2D empty;
        empty.start = CVector2D{0.0f, 0.0f};
        empty.end = CVector2D{0.0f, 0.0f};
        empty.field_0x10 = -1.0f;
        fn_7100125b44(&self->field_0x14744, &empty);
    }
    self->field_0x14720.value.x = g_unk_7101647780.x;
    self->field_0x14720.value.y = g_unk_7101647780.y;
    self->field_0x146d8.object = self;
    self->field_0x146d8.function = reinterpret_cast<void*>(fn_71005d2630);
    self->field_0x1590.object = self;
    self->field_0x1590.function = reinterpret_cast<void*>(fn_71005d2634);

    // Roll the way the body faces; when it faces neither way, by its up axis.
    f32 facing = self->body->xform.GetMatrix().m[2];
    bool reverse;
    if (facing + 0.01f >= 0.0f && facing - 0.01f <= 0.0f) {
        if (self->body->xform.GetMatrix().m[1] < 0.0f)
            reverse = false;
        else
            reverse = true;
    } else {
        reverse = self->body->xform.GetMatrix().m[2] < 0.0f;
    }
    self->reverse = reverse;
    if (reverse)
        self->dir = CVector2D{1.0f, 0.0f};
    else
        self->dir = CVector2D{-1.0f, -0.0f};

    if (fn_71005cf2e8(self))
        self->requests.field_0x1470f = true;
    else
        self->requests.field_0x14710 = true;

    if (auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->timeline)) {
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_710164a118), &mask.id, false, -1.0f);
    }
}
