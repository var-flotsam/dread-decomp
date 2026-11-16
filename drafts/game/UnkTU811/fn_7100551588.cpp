// UnkTU811 (CEmmyAIComponent's unit): reacts to two messages named by the game's id table. The first
// enters state 1, sends an action to the entity's animation, plays a sound and starts an effect; the
// second clears a flag and starts another effect. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkAtExit.h"
#include "base/UnkDestructors.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CFXComponent.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CStrId;
using base::global::CFilePathStrId;
using base::global::UnkStringData;

struct UnkEntityWithSound;
UnkSoundHandle fn_7100d7c374(UnkEntityWithSound* self, void* a, void* b, bool c, bool d, bool e);

using UnkPathMember_l551588 = base::UnkStrIdMemberOf<g_unk_7101d47200>;
__attribute__((visibility("hidden"))) extern UnkPathMember_l551588 g_unk_71726e2048;

inline const UnkGameManagerIdTable& IdTable_l551588() {
    static UnkGameManagerIdTable s_table asm("g_unk_7101d552d0");
    return s_table;
}

// A slot of the id table its header has no member for yet.
inline const CStrId& TableId_l551588(u32 offset) {
    return reinterpret_cast<const CStrId*>(&IdTable_l551588())[offset / sizeof(CStrId)];
}

// A string id copied from another (retained).
struct UnkSoundName_l551588 {
    UnkStringData* data;

    explicit UnkSoundName_l551588(const CStrId& id) : data(id.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
};

// A file path copied from another (retained).
struct UnkSoundPath_l551588 {
    UnkStringData* data;

    explicit UnkSoundPath_l551588(const CFilePathStrId& id) : data(id.data) {
        if (&g_unk_7101d47200->empty != data)
            ++data->ref_count;
    }
    void operator=(const UnkPathMember_l551588& other) { *reinterpret_cast<UnkPathMember_l551588*>(this) = other; }
};

// Two distances copied as one.
struct UnkSoundRange_l551588 {
    f32 min;
    f32 max;
};

// A sound description with its defaults; destroyed out of line.
struct UnkSoundDesc_l551588 {
    u32 kind;
    u32 field_0x4;
    u64 field_0x8;
    u32 field_0x10;
    u32 field_0x14;  // unset in the original
    UnkSoundName_l551588 name;  // 0x18
    UnkSoundPath_l551588 path;  // 0x20
    f32 position[3];            // 0x28
    f32 field_0x34;
    f32 field_0x38;
    UnkSoundRange_l551588 range;  // 0x3c
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

    UnkSoundDesc_l551588()
        : kind(0), field_0x4(0), field_0x8(0), field_0x10(0), name(g_unk_7101d470e0), path(g_unk_7101d470e8),
          position{0.0f, 0.0f, 0.0f}, field_0x34(-1.0f), field_0x38(1.0f), range{500.0f, 2000.0f},
          field_0x44(1.0f), field_0x48(0.0f), field_0x4c(0.0f), field_0x50(0.0f), field_0x54(-1.0f),
          field_0x58(-1.0f), field_0x5c(-1.0f), field_0x60(0.5f), field_0x64(5.0f), field_0x68(10.0f),
          field_0x6c(false), field_0x6d(false), field_0x6e(false) {}
    ~UnkSoundDesc_l551588() asm("fn_710034e0b8");
};
static_assert(__builtin_offsetof(UnkSoundDesc_l551588, field_0x6c) == 0x6c, "layout");

struct UnkEntity_l551588 {
    u8 pad_0x0[0x220];
    void* animation;  // 0x220
    u8 pad_0x228[0x280 - 0x228];
    UnkWorld* world;  // 0x280
};

struct UnkEmmyAI_l551588;
struct UnkEmmyAIVtable_l551588 {
    u8 pad_0x0[0x850];
    void (*vf_0x850)(UnkEmmyAI_l551588* self);
};

struct UnkEmmyAI_l551588 {
    UnkEmmyAIVtable_l551588* vtable;
    u8 pad_0x8[0x30 - 0x8];
    UnkEntity_l551588* entity;  // 0x30
    u8 pad_0x38[0x29ef0 - 0x38];
    s32 state;  // 0x29ef0
    u8 pad_0x29ef4[0x1];
    bool field_0x29ef5;
    u8 pad_0x29ef6[0x2a6f0 - 0x29ef6];
    bool field_0x2a6f0;
};
static_assert(__builtin_offsetof(UnkEmmyAI_l551588, field_0x2a6f0) == 0x2a6f0, "layout");

bool fn_7100551588(UnkEmmyAI_l551588* self, const CStrId* message) {
    if (*message == TableId_l551588(0x18c0)) {
        if (self->state != 1) {
            self->state = 1;
            self->vtable->vf_0x850(self);
        }
        if (void* animation = self->entity->animation)
            UnkSpitSend(animation, 0x76df0a29ce276deaull, 0x7c5476647f60cdaaull);
        static UnkSoundDesc_l551588 s_sound asm("g_unk_71726e2320");
        s_sound.kind = 6;
        s_sound.path = g_unk_71726e2048;
        s_sound.field_0x38 = 2.0f;
        s_sound.field_0x6d = true;
        s_sound.field_0x4 = 0;
        s_sound.range = UnkSoundRange_l551588{100.0f, 5000.0f};
        s_sound.field_0x44 = 1.0f;
        fn_7100d7c374(reinterpret_cast<UnkEntityWithSound*>(self->entity), &s_sound,
                      const_cast<CStrId*>(&g_unk_7101d470e0), false, true, true);
        fn_710114fe8c(self->entity->world, 0xe83eac0c8c30776cull, 0, 0);
    }
    if (*message == TableId_l551588(0x970)) {
        self->field_0x29ef5 = false;
        fn_710114fe8c(self->entity->world, 0xaf057066b81fccd4ull, 0, 0);
    }
    self->field_0x2a6f0 = false;
    return false;
}
