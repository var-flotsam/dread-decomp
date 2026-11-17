// CChozoWarriorXAIComponent's unit (tools/tu_map.py unit 721): updates the land attack's effects. After the
// base update it looks up the owner's (+0x30) four parts by hash; without both targets (the handles at
// +0x154e8) or the two beams it resets them all (fn_710048ecbc). Otherwise it advances the timer at
// +0x154f8, fades the colour in and places each beam and each glow beside its target, offset by the land
// attack's tweak values. Names and layouts are placeholders.
#include "base/color/CColor4F.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CComponent.h"
#include "game/CFXComponent.h"
#include "game/UnkSlots.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU133.h"
#include "game/UnkTU134.h"
#include "game/UnkTU53.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::color::CColor4F;

struct UnkSelf;
void fn_710048ecbc(UnkSelf* self);
bool fn_71001c2a2c(UnkSlots* self, u32 kind, const CColor4F* color, u32 mode);

// The land attack's tweak values.
struct UnkLandTweak_l48ee3c {
    u8 pad_0x0[0x38];
    f32 field_0x38;  // the glow's offset from its target
    f32 field_0x3c;  // how far the eased fade moves the glow
    f32 field_0x40;  // the beam's offset from its target
};

extern UnkLandTweak_l48ee3c g_unk_71726ce200;  // reached through the GOT

struct UnkLandAttack_l48ee3c {
    virtual const base::reflection::CType* GetType();
};

struct UnkLandPart_l48ee3c {
    u8 pad_0x0[0xb];
    u8 kind;  // 0xb
    u8 pad_0xc[0x160 - 0xc];
    UnkSlots* slots;  // 0x160
};

struct UnkLandActor_l48ee3c {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
};

struct UnkLandTarget_l48ee3c {
    u8 pad_0x0[0x10];
    UnkLandActor_l48ee3c* actor;  // 0x10
};

struct UnkLandFx_l48ee3c {
    UnkLandAttack_l48ee3c* attack;
    u8 pad_0x8[0x890 - 0x8];
    UnkLandTarget_l48ee3c* targets[2];  // 0x890
    f32 timer;                          // 0x8a0
    f32 duration;                       // 0x8a4
};

struct UnkLandOwner_l48ee3c {
    u8 pad_0x0[0x280];
    UnkObject* parts;  // 0x280
};

struct UnkChozoWarriorXAI_l48ee3c {
    u8 pad_0x0[0x30];
    UnkLandOwner_l48ee3c* owner;  // 0x30
    u8 pad_0x38[0x14c58 - 0x38];
    UnkLandFx_l48ee3c land;  // 0x14c58
};
static_assert(__builtin_offsetof(UnkChozoWarriorXAI_l48ee3c, land) == 0x14c58, "layout");

// The tweak values of the land attack; the class check's answer does not change which ones.
static inline const UnkLandTweak_l48ee3c& Tweak(UnkLandFx_l48ee3c* land) {
    if (land->attack)
        fn_710009d50c(land->attack->GetType(), fn_710047d5d8());
    return g_unk_71726ce200;
}

static inline UnkLandPart_l48ee3c* FindPart(UnkObject* parts, u64 hash, bool second_kind) {
    auto* part = reinterpret_cast<UnkLandPart_l48ee3c*>(fn_7101152b90(reinterpret_cast<UnkWorld*>(parts), hash));
    if (!part)
        return nullptr;
    bool ok = second_kind ? fn_710114e668(parts, part->kind) : fn_710114e680(parts, part->kind);
    return ok ? part : nullptr;
}

static inline UnkVec3f operator+(const UnkVec3f& a, const UnkVec3f& b) {
    return UnkVec3f(a.x + b.x, a.y + b.y, a.z + b.z);
}

static inline UnkVec3f operator-(const UnkVec3f& a, const UnkVec3f& b) {
    return UnkVec3f(a.x - b.x, a.y - b.y, a.z - b.z);
}

static inline UnkVec3f Position(const UnkLandActor_l48ee3c* actor) {
    return UnkVec3f(actor->x, actor->y, actor->z);
}

void fn_710048ee3c(UnkChozoWarriorXAI_l48ee3c* self, f32 dt) {
    fn_71010c424c(reinterpret_cast<UnkParent*>(self));
    UnkObject* parts = self->owner->parts;
    if (!parts)
        return;
    UnkLandPart_l48ee3c* glow_a = FindPart(parts, 0x89f328dc37ac167b, false);
    UnkLandPart_l48ee3c* glow_b = FindPart(parts, 0x8c9dfb3ece4e323c, false);
    UnkLandFx_l48ee3c* land = &self->land;
    UnkLandPart_l48ee3c* beam_a = FindPart(parts, 0xa0cda40259a7a253, true);
    UnkLandPart_l48ee3c* beam_b = FindPart(parts, 0xa5a377e0a0458614, true);
    if (!land->targets[0] || !land->targets[0]->actor || !land->targets[1] || !land->targets[1]->actor ||
        !beam_b || !beam_a) {
        fn_710048ecbc(reinterpret_cast<UnkSelf*>(self));
        return;
    }

    land->timer += dt;
    f32 ratio = land->timer / land->duration;
    CColor4F color(1.0f, 1.0f, 1.0f, 1.0f);
    f32 t = ratio <= 0.0f ? 0.0f : (!(ratio >= 1.0f) ? ratio : 1.0f);
    f32 fade = t * 0.7f;
    color.a = fade <= 0.0f ? 0.0f : fade;
    f32 eased = fn_7100076020(t, "2");

    const UnkLandTweak_l48ee3c& beam_tweak = Tweak(land);
    UnkVec3f pos = Position(land->targets[1]->actor);
    UnkVec3f beam_pos = UnkVec3f(beam_tweak.field_0x40, 0.0f, 0.0f) + pos;
    fn_7100146d84(reinterpret_cast<UnkFxObject_ovb0041*>(beam_a), &beam_pos);
    fn_7100146e60(reinterpret_cast<UnkSwitchObject*>(beam_a), true);
    pos.x += Tweak(land).field_0x38;
    pos.x += eased * Tweak(land).field_0x3c;
    fn_7100145f28(reinterpret_cast<UnkSwitchObject*>(glow_a), &pos);
    fn_71001c2a2c(glow_a->slots, 0, &color, 0);

    pos = Position(land->targets[0]->actor);
    beam_pos = pos - UnkVec3f(beam_tweak.field_0x40, 0.0f, 0.0f);
    fn_7100146d84(reinterpret_cast<UnkFxObject_ovb0041*>(beam_b), &beam_pos);
    fn_7100146e60(reinterpret_cast<UnkSwitchObject*>(beam_b), true);
    pos.x -= Tweak(land).field_0x38;
    pos.x -= eased * Tweak(land).field_0x3c;
    fn_7100145f28(reinterpret_cast<UnkSwitchObject*>(glow_b), &pos);
    fn_71001c2a2c(glow_b->slots, 0, &color, 0);
}
