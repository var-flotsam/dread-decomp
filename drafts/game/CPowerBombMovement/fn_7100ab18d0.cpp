// CPowerBombMovement's unit (tools/tu_map.py unit 1612): the bomb explodes. Flashes the screen, sends the
// explosion event, calls the owner's script, hooks the bomb's listener onto the owner's timeline and
// starts the explosion's damage area. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CSceneComponent.h"
#include "game/CTimelineComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2656.h"
#include "game/UnkTimelineComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

__attribute__((visibility("hidden"))) extern u8 g_unk_71727541a0[];  // the explosion event's key
extern u64 g_unk_710164b848;                                         // the timeline id

void fn_7100a2d4c8(UnkWeakTarget* target, void* listener);
void fn_710082efe8(UnkTimelineComponent* timeline, UnkWeakHandle** out);

struct UnkPbVec_lab18d0 {
    f32 x;
    f32 y;
    f32 z;
};

struct UnkPbOwner_lab18d0 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44, the transform's translation
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x178];
    UnkScriptCaller* script;  // 0x1e0
    u8 pad_0x1e8[0x40];
    void* shape;  // 0x228
    u8 pad_0x230[0xb8];
    UnkTimelineComponent* timeline;  // 0x2e8
};

struct UnkPowerBomb_lab18d0;
void fn_7100ab2404(UnkPowerBomb_lab18d0* self);

// A listener: the object and function it calls, and the counted link to what it listens to.
struct UnkPbListener_lab18d0 {
    void** vtable;
    UnkPowerBomb_lab18d0* context;               // 0x8
    void (*callback)(UnkPowerBomb_lab18d0*);     // 0x10
    UnkWeakHandle* link;                         // 0x18
};

struct UnkPowerBomb_lab18d0 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkPbOwner_lab18d0* owner;  // 0x30
    u8 pad_0x38[0x480];
    bool field_0x4b8;
    f32 field_0x4bc;
    u8 pad_0x4c0[0x10];
    UnkPbListener_lab18d0 listener;  // 0x4d0

    // vtable slot 0x2f8: starts a damage area at `position`.
    void vf_0x2f8(u32 kind, const UnkPbVec_lab18d0* position, void* source, u32 a, bool b, u32 c, bool d,
                  f32 radius) {
        typedef void (*Fn)(UnkPowerBomb_lab18d0*, u32, const UnkPbVec_lab18d0*, void*, u32, bool, u32, bool, f32);
        reinterpret_cast<Fn>(vtable[0x2f8 / 8])(this, kind, position, source, a, b, c, d, radius);
    }
};

void fn_7100ab18d0(UnkPowerBomb_lab18d0* self) {
    {
        void* effects = reinterpret_cast<u8*>(g_unk_7101d47310) + 0x1078;  // the screen flash player
        UnkPbVec_lab18d0 color;
        color.x = 1.0f;
        color.y = 1.0f;
        color.z = 1.0f;
        fn_7101053850(effects, reinterpret_cast<UnkVec*>(&color), 0, 1.0f, 0.6f);
    }
    {
        UnkPbVec_lab18d0 zero;
        zero.x = 0.0f;
        zero.y = 0.0f;
        zero.z = 0.0f;
        fn_7100e038b0(g_unk_7101d47310, g_unk_71727541a0, reinterpret_cast<UnkVec*>(&zero), 0);
    }
    self->field_0x4b8 = false;
    if (UnkScriptCaller* script = self->owner->script)
        fn_7100305088(script, "OnExplosion", g_unk_710162e2ed);
    fn_7100d7c828(reinterpret_cast<UnkActor*>(self->owner), &g_unk_710164b848, false);
    if (UnkTimelineComponent* timeline = self->owner->timeline) {
        UnkPbListener_lab18d0* listener = &self->listener;
        listener->context = self;
        listener->callback = fn_7100ab2404;
        if (listener->link) {
            if (UnkWeakTarget* target = listener->link->target)
                fn_7100a2d4d0(target, listener);
            ResetRef(&listener->link);
        }
        fn_710082efe8(timeline, &listener->link);
        fn_7100a2d4c8(listener->link ? listener->link->target : nullptr, listener);
    }
    UnkPbOwner_lab18d0* owner = self->owner;
    UnkPbVec_lab18d0 position = {owner->x, owner->y, owner->z};
    self->vf_0x2f8(0x20, &position, nullptr, 0x2b8, true, 0x2b0, true, self->field_0x4bc);
    if (void* shape = self->owner->shape)
        fn_71009f19b0(shape, false);
}
