// behaviortree::CEmmySearchingAction's unit (tools/tu_map.py unit 805): the search's start: resets the
// action's state, plays the two search sounds the AI's blackboard asks for at the actor's position (else
// forgets the animations they go with) and tells the AI it searches (fn_71005553a8). Names and layouts
// are placeholders.
#include "base/UnkPooledStrId.h"
#include "base/core/AssetName.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CBlackboard.h"
#include "game/UnkActor.h"
#include "game/behaviortree/CBehavior.h"
#include "game/sound/UnkSoundHandle.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

struct UnkEntityWithSound;
UnkSoundHandle fn_7100d7c2f0(UnkEntityWithSound* self, const u32* params, bool a, bool b, bool c);

// The blackboard flags that ask for the two sounds, and the sounds' file path.
extern CStrId g_unk_71726df1a8;
extern CStrId g_unk_71726df1b8;
__attribute__((visibility("hidden"))) extern base::UnkPooledStrId<g_unk_7101d47200> g_unk_71726df1e8;

struct UnkVec3_l516f9c {
    f32 x;
    f32 y;
    f32 z;
};

// The sound description the entity plays, with its defaults (as UnkTU811_PlaySound_l552af0.cpp's).
struct UnkSoundDesc_l516f9c {
    u32 kind;
    u32 field_0x4;
    u64 field_0x8;
    u32 field_0x10;
    u32 field_0x14;  // unset in the original
    base::core::AssetName name;  // 0x18
    UnkVec3_l516f9c position;    // 0x28
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

    UnkSoundDesc_l516f9c()
        : kind(0), field_0x4(0), field_0x8(0), field_0x10(0), position{0.0f, 0.0f, 0.0f}, field_0x34(-1.0f),
          field_0x38(1.0f), field_0x3c(500.0f), field_0x40(2000.0f), field_0x44(1.0f), field_0x48(0.0f),
          field_0x4c(0.0f), field_0x50(0.0f), field_0x54(-1.0f), field_0x58(-1.0f), field_0x5c(-1.0f),
          field_0x60(0.5f), field_0x64(5.0f), field_0x68(10.0f), field_0x6c(false), field_0x6d(false),
          field_0x6e(false) {}
    // Out of line in another unit (src/game/CAbilitySonar/fn_7100333544.cpp).
    ~UnkSoundDesc_l516f9c() asm("fn_710034e0b8");
};
static_assert(__builtin_offsetof(UnkSoundDesc_l516f9c, field_0x6c) == 0x6c, "layout");

struct UnkEmmyAI_l516f9c : base::reflection::UnkReflObject {
    u8 pad_0x8[0x1258];
    CBlackboard blackboard;  // 0x1260
};

struct UnkEmmyAnimLink_l516f9c {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
    UnkWeakHandle* handle;  // 0x18

    // Stops listening to the animation the handle points at, and drops the handle.
    void Detach() {
        if (UnkWeakHandle* h = handle) {
            if (UnkWeakTarget* target = h->target)
                fn_710117b3f0(target, this);
            ResetRef(&handle);
        }
    }
};

struct UnkEmmySearchingAction_l516f9c {
    u8 pad_0x0[0x18];
    UnkActor* actor;  // 0x18
    u8 pad_0x20[0x10];
    u64 animation;   // 0x30
    u64 animation2;  // 0x38
    UnkEmmyAnimLink_l516f9c link;  // 0x40
    u32 field_0x60;
    bool field_0x64;
    u8 pad_0x65[0x12a8c];
    bool stopped;  // 0x12af1
    u8 pad_0x12af2[0x42];
    bool field_0x12b34;
    bool pending;  // 0x12b35
    bool targetPlayed;  // 0x12b36
    u8 pad_0x12b37;
    u64 targetAnimation;  // 0x12b38
};
static_assert(__builtin_offsetof(UnkEmmySearchingAction_l516f9c, targetAnimation) == 0x12b38, "layout");

void fn_710051dae0(UnkEmmySearchingAction_l516f9c* self);
void fn_71005553a8(UnkEmmyAI_l516f9c* ai, bool searching);

// The actor's position: the translation of its transform.
static inline UnkVec3_l516f9c Position_l516f9c(const UnkActor* actor) {
    return {actor->field_0x44, actor->field_0x54, actor->field_0x64};
}

// Plays `desc` with the path `path` at the actor's position.
static inline void PlaySearchSound_l516f9c(UnkEmmySearchingAction_l516f9c* self, UnkSoundDesc_l516f9c* desc,
                                           const base::UnkPooledStrId<g_unk_7101d47200>& path) {
    desc->kind = 0;
    desc->name.path = path;
    desc->field_0x6d = true;
    desc->position = Position_l516f9c(self->actor);
    desc->field_0x38 = 1.0f;
    desc->field_0x3c = 3001.0f;
    desc->field_0x40 = 3000.0f;
    desc->field_0x44 = 1.0f;
    fn_7100d7c2f0(reinterpret_cast<UnkEntityWithSound*>(self->actor), &desc->kind, false, true, true);
}

void fn_7100516f9c(UnkEmmySearchingAction_l516f9c* self) {
    fn_710051dae0(self);
    self->field_0x60 = 0;
    self->animation = ~0ull;
    self->animation2 = ~0ull;
    self->field_0x12b34 = true;
    self->pending = false;
    self->stopped = false;
    self->field_0x64 = false;
    self->targetPlayed = false;
    self->targetAnimation = ~0ull;

    UnkEmmyAI_l516f9c* ai = static_cast<UnkEmmyAI_l516f9c*>(self->actor->field_0x240);
    if (ai && !fn_710009d50c(ai->vf_0x0(), fn_710053b934()))
        ai = nullptr;
    CBlackboard* blackboard = &ai->blackboard;

    bool first = false;
    CBlackboard::CSection* section = fn_7101047270(blackboard, g_unk_71726b7cb8);
    if (section && (fn_710006d11c(section, &g_unk_71726df1a8, &first), first)) {
        static UnkSoundDesc_l516f9c s_first;
        PlaySearchSound_l516f9c(self, &s_first, g_unk_71726df1e8);
    } else {
        self->animation = ~0ull;
        self->link.Detach();
    }

    bool second = false;
    section = fn_7101047270(blackboard, g_unk_71726b7cb8);
    if (section && (fn_710006d11c(section, &g_unk_71726df1b8, &second), second)) {
        static UnkSoundDesc_l516f9c s_second;
        PlaySearchSound_l516f9c(self, &s_second, g_unk_71726df1e8);
    } else {
        self->animation2 = ~0ull;
        self->link.Detach();
    }

    fn_71005553a8(ai, true);
}
