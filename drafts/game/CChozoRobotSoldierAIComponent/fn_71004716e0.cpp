// CChozoRobotSoldierAIComponent's unit (tools/tu_map.py unit 694): picks a jump animation. When a spin
// jump is allowed and the animation's name has "_spinjump" in it, the "_jump" one is used instead if the
// actor's animations have it; otherwise the base's choice (fn_710110fbc8). Names and layouts are placeholders.
#include "../../game_aca57c/UnkBufferString.h"
#include "base/global/CRntString.h"
#include "base/global/CRntStringAssign.h"
#include "base/global/CStrId.h"
#include "game/UnkBufferStringPool.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStringData;

// A string on 0x80 bytes of inline storage.
struct UnkString80_l471920 : UnkBufferString<0x80, g_unk_7101a1b750> {
    explicit UnkString80_l471920(const char* str) : UnkBufferString(str) {}
    // The first `length` characters of `str`.
    UnkString80_l471920(const char* str, u32 length) { CRntString_AssignPrefix(Str(), str, length); }
    explicit UnkString80_l471920(const CRntString& str) { CRntString_Assign(Str(), &str); }
};

UnkString80_l471920 fn_7100471920(const CRntString* text, const char* from, const char* to);

struct UnkStruct_7101176ea8;
bool fn_7101176e78(UnkStruct_7101176ea8* self, void* key, void** out);

struct UnkRobotSoldierTuning_ovb0002 {
    u8 pad_0x0[0x2c];
    f32 field_0x2c;  // the timer's limit
    u8 pad_0x30[0x20];
    f32 field_0x50;  // a chance, 0 to 1
};
extern UnkRobotSoldierTuning_ovb0002 g_unk_71726cc6d8;  // reached through the GOT

struct UnkJumpActor_l4716e0 {
    u8 pad_0x0[0x220];
    UnkStruct_7101176ea8* animations;  // 0x220
};

struct UnkJumpInfo_l4716e0 {
    u8 pad_0x0[0x2];
    s16 field_0x2;
    u8 pad_0x4[0x4];
    f32 field_0x8;
    u8 pad_0xc[0x12];
    s16 field_0x1e;
    u8 pad_0x20[0x4];
    f32 field_0x24;
};

struct UnkRobotSoldierAI_l4716e0 {
    u8 pad_0x0[0x30];
    UnkJumpActor_l4716e0* actor;  // 0x30
    u8 pad_0x38[0x11e6];
    bool field_0x121e;
    u8 pad_0x121f[0x138fd];
    f32 timer;  // 0x14b1c
};
static_assert(__builtin_offsetof(UnkRobotSoldierAI_l4716e0, timer) == 0x14b1c, "layout");

// A string id built from a string (fn_7100000850).
struct UnkAnimId_l4716e0 {
    UnkStringData* data;

    explicit UnkAnimId_l4716e0(const CRntString& str) {
        fn_7100000850(reinterpret_cast<base::global::UnkStrIdBase*>(this), str, true);
    }
    ~UnkAnimId_l4716e0() { reinterpret_cast<CStrId*>(this)->~CStrId(); }
};

void fn_710110fbc8(UnkRobotSoldierAI_l4716e0* self, UnkJumpInfo_l4716e0* info, CStrId* anim);

// Swaps a "_spinjump" animation for its "_jump" one when the actor has it.
static bool UseJumpForSpinJump(UnkRobotSoldierAI_l4716e0* self, CStrId* anim) {
    ++anim->data->reads;
    const CRntString& name = anim->data->String();
    if (CRntString_Find(&name, "_spinjump", 0) < 0)
        return false;
    UnkAnimId_l4716e0 jump(*fn_7100471920(&name, "_spinjump", "_jump").Str());
    if (self->actor->animations &&
        fn_7101176e78(self->actor->animations, reinterpret_cast<void*>(jump.data->Key()), nullptr)) {
        *anim = reinterpret_cast<const CStrId&>(jump);
        return true;
    }
    return false;
}

void fn_71004716e0(UnkRobotSoldierAI_l4716e0* self, UnkJumpInfo_l4716e0* info, CStrId* anim) {
    if (info->field_0x2 != -1 && info->field_0x1e != -1 &&
        (!self->field_0x121e ||
         (!(self->timer >= g_unk_71726cc6d8.field_0x2c) && info->field_0x8 - 50.0f > info->field_0x24))) {
        if (UseJumpForSpinJump(self, anim))
            return;
    }
    fn_710110fbc8(self, info, anim);
}
