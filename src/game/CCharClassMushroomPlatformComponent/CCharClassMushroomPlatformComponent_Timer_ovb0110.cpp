// CCharClassMushroomPlatformComponent's unit (tools/tu_map.py unit 1345): whether the player is on the
// platform, the timed update of its state machine and the destructor. Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManager.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);
struct UnkFlagged;
struct UnkShape;
UnkShape* fn_7100976588(UnkFlagged* flagged);
struct UnkBox2D;
struct UnkContactPoint_ov110 {
    u8 data[0x2c];
};
template <u32 N>
struct UnkContactBuffer {
    u32 count;
    UnkContactPoint_ov110 points[N];
};
bool fn_710012b330(const UnkBox2D* a, const void* b, UnkContactBuffer<4>* out, bool flag);
bool fn_7100d5bfbc(void* tracker, u32 mask, UnkCollisionObject* object, void* arg, u32 flags);

struct UnkGfxFrameSource;
extern UnkGfxFrameSource* g_unk_7101d47328;
struct UnkTimer_7100080c40;
s64 fn_7100080fb4(const UnkTimer_7100080c40* self);
struct UnkActorTarget;
void fn_7101176a88(UnkActorTarget* target, u32 value);

extern base::global::CStrId g_unk_717272c198 __attribute__((visibility("hidden")));

struct UnkMushroomShape_ov110 {
    virtual void vf_0x0();
    u8 pad_0x8[0x40 - 0x8];
    u8 geometry[0x20];  // 0x40
    bool dirty;         // 0x60
    void Update() {
        typedef void (*Fn)(UnkMushroomShape_ov110*);
        (*reinterpret_cast<Fn**>(this))[0x98 / 8](this);
    }
};

struct UnkMushroomPlayer_ov110 {
    u8 pad_0x0[0x200];
    UnkFlagged* movement;  // 0x200
};

struct UnkMushroomOwner_ov110 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* collision;  // 0x2a0
};

struct UnkMushroomAnimation_ov110 {
    void** vtable;
    void SetSpeed(f32 v) { reinterpret_cast<void (*)(UnkMushroomAnimation_ov110*, f32)>(vtable[0x98 / 8])(this, v); }
    void SetBlend(f32 v) { reinterpret_cast<void (*)(UnkMushroomAnimation_ov110*, f32)>(vtable[0xa8 / 8])(this, v); }
};

struct UnkAnimationOwner_ov110 {
    u8 pad_0x0[0x220];
    UnkMushroomAnimation_ov110* animation;  // 0x220
};

struct UnkMushroomPlatform_ov110 {
    u8 pad_0x0[0x30];
    UnkMushroomOwner_ov110* owner;  // 0x30
    u8 pad_0x38[0x208];
    u8 box[0x10];  // 0x240
};

// Whether the player's shape touches the platform's box, or else whether the collision tracker
// reports the platform's collider (g_unk_717272c198) for mask 0x2000.
struct UnkTimerSm_b60;  // src/game/UnkStateMachineSet_b60.cpp

bool fn_710088b7dc(UnkTimerSm_b60* platform) {
    UnkMushroomPlatform_ov110* self = reinterpret_cast<UnkMushroomPlatform_ov110*>(platform);
    UnkMushroomPlayer_ov110* player = reinterpret_cast<UnkMushroomPlayer_ov110*>(fn_7100dec788(g_unk_7101d47310));
    if (!player || !player->movement)
        return false;
    UnkMushroomShape_ov110* shape = reinterpret_cast<UnkMushroomShape_ov110*>(fn_7100976588(player->movement));
    if (!shape)
        return false;
    UnkContactBuffer<4> contacts;
    contacts.count = 0;
    UnkGameManagerSub_0x608* scenario = g_unk_7101d47310->field_0x608;
    if (!scenario)
        return false;
    void* tracker = scenario->field_0x820;
    if (!tracker)
        return false;
    UnkCollisionContainer* collision = self->owner->collision;
    if (!collision)
        return false;
    UnkCollisionObject* object = fn_71008640a4(collision, &g_unk_717272c198);
    if (!object)
        return false;
    const UnkBox2D* box = reinterpret_cast<const UnkBox2D*>(self->box);
    asm("" : "+r"(box));
    if (shape->dirty)
        shape->Update();
    if (fn_710012b330(box, shape->geometry, &contacts, true))
        return true;
    return fn_7100d5bfbc(tracker, 0x2000, object, nullptr, 0);
}

// The platform's small state machine (+0x260); its update is fn_710088af9c.
struct UnkSmMachine_ov110 {
    u8 data[0xb0];
};
void fn_710088af9c(UnkSmMachine_ov110* self, f32 dt);

struct UnkMushroomTimer_ov110 {
    u8 pad_0x0[0x30];
    union {
        UnkAnimationOwner_ov110* anim_owner;
    };
    u8 pad_0x38[0x228];
    UnkSmMachine_ov110 machine;  // 0x260
    u8 pad_0x310[0xe0];
    f32 field_0x3f0;
    u8 pad_0x3f4[0x74];
    f32 field_0x468;
    u8 pad_0x46c[0x74];
    s64 last_time;  // 0x4e0
    u8 pad_0x4e8[0x20];
    void* link;  // 0x508
};
static_assert(__builtin_offsetof(UnkMushroomTimer_ov110, last_time) == 0x4e0, "layout");
static_assert(__builtin_offsetof(UnkMushroomTimer_ov110, link) == 0x508, "layout");

// Advances the machine by the time since the last stamp (microseconds) while either timer (+0x3f0,
// +0x468) runs; when neither does any more, clears the stamp and resets the animation.
void fn_710088c47c(UnkMushroomTimer_ov110* self) {
    if (!self->last_time)
        return;
    s64 now = fn_7100080fb4(reinterpret_cast<const UnkTimer_7100080c40*>(g_unk_7101d47328));
    f32 dt = static_cast<f32>(now - self->last_time) / 1000000.0f;
    f32 first = self->field_0x3f0;
    if (first > 0.0f) {
        fn_710088af9c(&self->machine, dt);
        if (self->field_0x3f0 <= 0.0f)
            dt -= first;
    }
    if (self->field_0x468 > 0.0f)
        fn_710088af9c(&self->machine, dt);
    if (!(self->field_0x3f0 > 0.0f)) {
        if (self->field_0x468 > 0.0f) {
            self->last_time = now;
            return;
        }
        now = 0;
    }
    self->last_time = now;
    if (UnkAnimationOwner_ov110* owner = self->anim_owner) {
        if (UnkMushroomAnimation_ov110* animation = owner->animation) {
            fn_7101176a88(reinterpret_cast<UnkActorTarget*>(animation), 1);
            animation->SetSpeed(2.0f);
            animation->SetBlend(2.0f);
        }
    }
}

extern void* g_unk_7101a7c210[];
extern void* g_unk_7101a22d48[];
extern char g_unk_7101a7c450[];
extern void* g_unk_7101a7c4a8[];

struct UnkMushroomLinkTarget_ov110;
void fn_710088c874(UnkMushroomLinkTarget_ov110* list, UnkMushroomLinkTarget_ov110** slot);
void fn_710115f91c(void* self);

struct UnkMushroomHolderTag_ov110;
using UnkMushroomHolder_ov110 = base::UnkValueHolder<UnkMushroomHolderTag_ov110>;

static inline __attribute__((always_inline)) void DestroyHolder_ov110(UnkMushroomHolder_ov110* self) {
    for (int i = 3; i >= 0; --i)
        self->pairs[i].~UnkClearedPair();
    self->vtable = g_unk_7101a7c450 + 0x10;
    fn_71000a1c60(&self->value);
}

struct UnkMushroomLink_ov110 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();  // 0x10
    u32 ref_count;           // 0x8
    void* owner;             // 0x10
};

struct UnkMushroomListener_ov110 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkMushroomLink_ov110* link;  // 0x18
};

static inline __attribute__((always_inline)) void DestroyListener_ov110(UnkMushroomListener_ov110* self) {
    self->vtable = &g_unk_7101a22d48[2];
    UnkMushroomLink_ov110* link = self->link;
    if (!link)
        return;
    if (void* owner = link->owner) {
        fn_710117b3f0(static_cast<UnkWeakTarget*>(owner), self);
        link = self->link;
        if (!link)
            return;
    }
    if (--link->ref_count == 0)
        link->Release();
}

struct UnkMushroomMachine_ov110 {
    void** vtable;
    u64 field_0x8;
    void* current;   // 0x10
    void* previous;  // 0x18
    u32 count;       // 0x20
};

struct UnkMushroomPlatformDtor_ov110 {
    void** vtable;
    u8 pad_0x8[0x258];
    UnkMushroomMachine_ov110 machine;  // 0x260
    u8 pad_0x288[0x88];
    UnkMushroomHolder_ov110 holder_0x310;
    UnkMushroomHolder_ov110 holder_0x380;
    u8 pad_0x3f0[0x8];
    UnkMushroomHolder_ov110 holder_0x3f8;
    u8 pad_0x468[0x8];
    UnkMushroomHolder_ov110 holder_0x470;
    u8 pad_0x4e0[0x8];
    UnkMushroomListener_ov110 listener;  // 0x4e8
    UnkMushroomLinkTarget_ov110* link;   // 0x508
    void* callback_owner;                // 0x510
    void* callback;                      // 0x518
};
static_assert(__builtin_offsetof(UnkMushroomPlatformDtor_ov110, holder_0x470) == 0x470, "layout");
static_assert(__builtin_offsetof(UnkMushroomPlatformDtor_ov110, callback) == 0x518, "layout");

// The destructor: unhooks the link (+0x508), clears the callback, detaches the listener (+0x4e8), destroys
// the four value holders and the state machine, then the base (fn_710115f91c).
void fn_710088c60c(UnkMushroomPlatformDtor_ov110* self) {
    self->vtable = &g_unk_7101a7c210[2];
    UnkMushroomLinkTarget_ov110** slot = &self->link;
    if (*slot) {
        fn_710088c874(*slot, slot);
        *slot = nullptr;
    }
    self->callback = nullptr;
    self->callback_owner = nullptr;
    DestroyListener_ov110(&self->listener);
    DestroyHolder_ov110(&self->holder_0x470);
    DestroyHolder_ov110(&self->holder_0x3f8);
    DestroyHolder_ov110(&self->holder_0x380);
    DestroyHolder_ov110(&self->holder_0x310);
    self->machine.vtable = &g_unk_7101a7c4a8[2];
    self->machine.current = nullptr;
    self->machine.previous = nullptr;
    self->machine.count = 0;
    fn_710115f91c(self);
}
