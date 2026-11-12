// CMorphBallMovement's unit (hint only; the unit has no reliable name in tools/tu_map.py): functions of the ball movement.
// Each function is described above its code. Names and layouts are placeholders.

#include <math.h>

#include "base/UnkStrIdPathPair.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CAutclastChargeAttack.h"
#include "game/CBreakableTileGroup.h"
#include "game/CCharacterMovement.h"
#include "game/CCollisionComponent.h"
#include "game/CFXComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkModelManager.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU134.h"
#include "game/UnkTU218.h"
#include "game/UnkTU25.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2689.h"
#include "game/UnkTU2739.h"
#include "game/UnkTU57.h"
#include "game/UnkXformNode.h"
#include "types.h"

// CMorphBallMovement's unit: whether the ball is free of the checks that block it. Not in mode 2 with a
// collision component whose id is the one at g_unk_7172739458, not in mode 5, no ignore-collision track
// on the owner's animation component, and the game manager's fn_7100df5a4c off. Names and layouts are
// placeholders.

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkCollisionContainer;

struct UnkMorphBallOwner_b7c4 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkMorphBallLink_b7c4 {
    u8 pad_0x0[0x10];
    UnkReflObject* field_0x10;
};

struct UnkMorphBallLinkHolder_b7c4 {
    u8 pad_0x0[0x10];
    UnkMorphBallLink_b7c4* field_0x10;
};

struct UnkMorphBall_b7c4 {
    u8 pad_0x0[0x30];
    UnkMorphBallOwner_b7c4* owner;  // 0x30
    u8 pad_0x38[0x128 - 0x38];
    UnkMorphBallLinkHolder_b7c4* field_0x128;
    u8 pad_0x130[0x2288 - 0x130];
    s32 mode;  // 0x2288
};

extern CStrId g_unk_7172739458 __attribute__((visibility("hidden")));

bool fn_710094b7c4(UnkMorphBall_b7c4* self) {
    if (self->mode == 2) {
        UnkMorphBallLinkHolder_b7c4* holder = self->field_0x128;
        if (holder) {
            UnkMorphBallLink_b7c4* link = holder->field_0x10;
            if (link) {
                UnkReflObject* object = link->field_0x10;
                if (object) {
                    if (fn_710009d50c(object->vf_0x0(), fn_710085fc44()) &&
                        fn_7100864018(reinterpret_cast<UnkCollisionContainer*>(object), link)->data ==
                            g_unk_7172739458.data)
                        return false;
                }
            }
        }
    }
    if (self->mode == 5)
        return false;
    if (fn_710117b348(static_cast<UnkAnimationComponent*>(self->owner->field_0x220), fn_7100d07ac8()))
        return false;
    return !fn_7100df5a4c(g_unk_7101d47310);
}

// CMorphBallMovement's unit: looks up (and caches at +0x25a0 and +0x25b8) the two components of the
// owner's object (+0x30, its +0x280) that the ids at +0x2560 and +0x2570 name, then tells the object
// about the first with a flag that is set while its byte at +0x121 is clear. Names and layouts are
// placeholders.

struct UnkObject;
struct UnkComponent;

struct UnkMorphBallComponent_9970 {
    u8 pad_0x0[0xb];
    u8 kind;  // 0xb
    u8 pad_0xc[0x121 - 0xc];
    u8 field_0x121;
};

struct UnkMorphBallId_9970 {
    u8 pad_0x0[0x30];
    u64 hash;  // 0x30
};

struct UnkMorphBallOwner_9970 {
    u8 pad_0x0[0x280];
    UnkObject* field_0x280;
};

struct UnkMorphBall_9970 {
    u8 pad_0x0[0x30];
    UnkMorphBallOwner_9970* owner;  // 0x30
    u8 pad_0x38[0x2560 - 0x38];
    UnkMorphBallId_9970* field_0x2560;
    u8 pad_0x2568[8];
    UnkMorphBallId_9970* field_0x2570;
    u8 pad_0x2578[0x2580 - 0x2578];
    u8 pad_0x2580[0x25a0 - 0x2580];
    UnkMorphBallComponent_9970* field_0x25a0;
    u8 pad_0x25a8[0x25b8 - 0x25a8];
    UnkMorphBallComponent_9970* field_0x25b8;
};

struct UnkPart;
struct UnkWorld;
struct UnkWorld;

void fn_7100949970(UnkMorphBall_9970* self) {
    if (!self->field_0x25a0)
        self->field_0x25a0 = reinterpret_cast<UnkMorphBallComponent_9970*>(fn_7101152b90(reinterpret_cast<UnkWorld*>(self->owner->field_0x280), self->field_0x2560->hash));
    if (!self->field_0x25b8) {
        UnkObject* object = self->owner->field_0x280;
        auto* found = reinterpret_cast<UnkMorphBallComponent_9970*>(fn_7101152b90(reinterpret_cast<UnkWorld*>(object),
                                                                                  self->field_0x2570->hash));
        self->field_0x25b8 = found && fn_710114e668(object, found->kind) ? found : nullptr;
    }
    UnkMorphBallComponent_9970* first = self->field_0x25a0;
    if (first) {
        bool clear = first->field_0x121 == 0;
        fn_710114fe8c(reinterpret_cast<UnkWorld*>(self->owner->field_0x280), self->field_0x2560->hash, 1, clear);
    }
}

// CMorphBallMovement's unit: switches the part the id at +0x2588 names on (fn_710114fe8c), hands it the
// owner's position (vf_0x10), then turns it about z by the angle of the up vector at +0x248c/+0x2490,
// taken from a quarter turn and wrapped to be positive (vf_0x18). Names and layouts are placeholders.

struct UnkObject;
struct UnkComponent;

struct UnkMorphBallPart_c0ac {
    void** vtable;
    void vf_0x10(const u32* position) {
        reinterpret_cast<void (*)(UnkMorphBallPart_c0ac*, const u32*)>(vtable[0x10 / 8])(this, position);
    }
    void vf_0x18(const f32* rotation) {
        reinterpret_cast<void (*)(UnkMorphBallPart_c0ac*, const f32*)>(vtable[0x18 / 8])(this, rotation);
    }
};

struct UnkMorphBallActor_c0ac {
    u8 pad_0x0[0x44];
    u32 field_0x44;
    u8 pad_0x48[0x54 - 0x48];
    u32 field_0x54;
    u8 pad_0x58[0x64 - 0x58];
    u32 field_0x64;
    u8 pad_0x68[0x280 - 0x68];
    UnkObject* field_0x280;
};

struct UnkMorphBallId_c0ac {
    u8 pad_0x0[0x30];
    u64 hash;  // 0x30
};

struct UnkMorphBall_c0ac {
    u8 pad_0x0[0x30];
    UnkMorphBallActor_c0ac* owner;  // 0x30
    u8 pad_0x38[0x248c - 0x38];
    f32 field_0x248c;
    f32 field_0x2490;
    u8 pad_0x2494[0x2588 - 0x2494];
    UnkMorphBallId_c0ac* field_0x2588;
};

void fn_710094c0ac(UnkMorphBall_c0ac* self) {
    fn_710114fe8c(reinterpret_cast<UnkWorld*>(self->owner->field_0x280), self->field_0x2588->hash, 1, 1);
    alignas(8) f32 rotation[4];
    UnkMorphBallActor_c0ac* owner = self->owner;
    u32 x = owner->field_0x44;
    u32 y = owner->field_0x54;
    u32 z = owner->field_0x64;
    alignas(8) u32 position[3] = {x, y, z};
    auto* part = reinterpret_cast<UnkMorphBallPart_c0ac*>(fn_7101152b90(reinterpret_cast<UnkWorld*>(owner->field_0x280),
                                                                        self->field_0x2588->hash));
    if (part) {
        part->vf_0x10(position);
        atan2f(1.0f, 0.0f);
        f32 angle = 1.5707964f - atan2f(self->field_0x2490, -self->field_0x248c);
        if (angle < 0.0f)
            angle += 6.283185307179586;
        rotation[0] = 0.0f;
        rotation[1] = 0.0f;
        rotation[2] = angle;
        part->vf_0x18(rotation);
    }
}

// CMorphBallMovement's unit: the hit flags of a breakable tile group hit, as the movement's
// fn_71009ecd98 (src/game/UnkTU1501/UnkTU1501_Movement_ovb0152.cpp) gives them without the gun check:
// bit 0 when fn_71010d9638 accepts the hit and the player is not in the state of fn_7100df5874, plus the
// flags of the query object for the tile. Names and layouts are placeholders.

struct UnkMove_b152;
struct UnkQueryObj;
struct UnkPlayer;

struct UnkHitFlags_b152 {
    u32 bits;
    UnkHitFlags_b152(u32 b) : bits(b) {}
    UnkHitFlags_b152(const UnkHitFlags_b152& o) : bits(o.bits) {}
};
struct UnkHitNode_dcf0 {
    u8 pad_0x0[0x10];
    UnkHitNode_dcf0* field_0x10;
};
struct UnkHitInfo_dcf0 {
    UnkHitNode_dcf0* field_0x0;
};

struct UnkMorphBallActor_dcf0 {
    u8 pad_0x0[0x2e0];
    void* field_0x2e0;
};
struct UnkMorphBall_dcf0 {
    u8 pad_0x0[0x30];
    UnkMorphBallActor_dcf0* actor;  // 0x30
};

struct UnkHitInfo_b152;

struct UnkStaticZero_dcf0 {
    u32 value;
    UnkStaticZero_dcf0() : value(0) {}
};
inline const UnkStaticZero_dcf0& StaticZero_dcf0() {
    static UnkStaticZero_dcf0 s_value asm("g_unk_71726aec10");
    return s_value;
}

UnkHitFlags_b152 fn_710094dcf0(UnkMorphBall_dcf0* self, UnkHitInfo_dcf0* hit) {
    u32 flags = StaticZero_dcf0().value;
    UnkHitFlags_b152 result(flags);
    base::reflection::UnkReflObject* tile =
        reinterpret_cast<base::reflection::UnkReflObject*>(hit->field_0x0->field_0x10->field_0x10);
    if (tile && fn_710009d50c(tile->vf_0x0(), fn_7100ca0484())) {
        u64 unused = 0;
        if (fn_71010d9638(reinterpret_cast<UnkMove_b152*>(self), reinterpret_cast<UnkHitInfo_b152*>(hit), &unused) &&
            !fn_7100df5874(reinterpret_cast<UnkPlayer*>(g_unk_7101d47310))) {
            flags |= 1;
            result.bits = flags;
        }
        if (UnkQueryObj* object = fn_7100ca2048(tile, reinterpret_cast<u8*>(hit) + 0x14, 1)) {
            UnkHitFlags_b152 more = fn_7100301c0c(self->actor->field_0x2e0, object, hit);
            result.bits = more.bits | flags;
        }
    }
    return result;
}

// CMorphBallMovement's unit: spawns the ball's effect at the owner's position (moved ahead by five
// times the velocity at +0x1a4 while bit 4 of the flags at +0x1b4 is set), turned half a turn about
// x when the heading at +0x1d8 points against the delta from +0x2440 to +0x2448, and about z by the
// heading's angle; as the grapple beam's fn_710081ba20 does. Names and layouts are placeholders.

struct UnkFxObject_ovb0041;
struct UnkMorphBallFxVtable_75b0 {
    void* unk_0x0[2];
    void (*set_position)(UnkFxObject_ovb0041* self, const UnkVec3f* v);  // 0x10
    void (*set_rotation)(UnkFxObject_ovb0041* self, const UnkVec3f* v);  // 0x18
    void (*set_scale)(UnkFxObject_ovb0041* self, const UnkVec3f* v);     // 0x20
    void* unk_0x28[13];
    void (*set_loop)(UnkFxObject_ovb0041* self, bool loop);               // 0x90
    void (*set_speed)(UnkFxObject_ovb0041* self, f32 speed);              // 0x98
};

extern const u8 g_unk_7172739470[] __attribute__((visibility("hidden")));  // the effect's path id

static inline UnkMorphBallFxVtable_75b0* Vtable_75b0(UnkFxObject_ovb0041* fx) {
    return *reinterpret_cast<UnkMorphBallFxVtable_75b0**>(fx);
}

static inline UnkModelManager* ModelManager_75b0() {
    if (!g_unk_71726b0cd8) {
        UnkModelManager* m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    return g_unk_71726b0cd8;
}

struct UnkMorphBall_75b0 {
    u8 pad_0x0[0x18c];
    UnkVec3f position;  // 0x18c
    u8 pad_0x198[0x1a4 - 0x198];
    UnkVec3f velocity;  // 0x1a4
    u8 pad_0x1b0[0x1b4 - 0x1b0];
    u8 flags;  // 0x1b4
    u8 pad_0x1b5[0x1d8 - 0x1b5];
    UnkVec3f heading;  // 0x1d8
    u8 pad_0x1e4[0x2440 - 0x1e4];
    f32 field_0x2440;
    f32 field_0x2444;
    f32 field_0x2448;
    f32 field_0x244c;
};

void fn_71009475b0(UnkMorphBall_75b0* self) {
    f32 dx = self->field_0x2448 - self->field_0x2440;
    f32 dy = self->field_0x244c - self->field_0x2444;
    base::math::CVector2D flat;
    fn_710007b430(&flat, reinterpret_cast<const base::math::CVector3D*>(&self->heading));
    f32 dot = dx * flat.x + dy * flat.y;
    f32 turn = atan2f(self->heading.y, self->heading.x);
    f32 half = dot < 0.0f ? 3.1415927f : 0.0f;
    UnkVec3f cur(self->position);
    if (self->flags & 0x10) {
        f32 vx = self->velocity.x * 5.0f;
        f32 vy = self->velocity.y * 5.0f;
        f32 vz = self->velocity.z * 5.0f;
        cur.x = vx + cur.x;
        cur.y = vy + cur.y;
        cur.z = vz + cur.z;
    }
    UnkModelManager* manager = ModelManager_75b0();
    UnkVec3f position(cur);
    UnkVec3f angles(half, 0.0f, turn);
    UnkVec3f scale(1.0f, 1.0f, 1.0f);
    UnkFxObject_ovb0041* fx = fn_71003b7db0(manager, g_unk_7172739470, false, true);
    if (fx) {
        Vtable_75b0(fx)->set_position(fx, &position);
        Vtable_75b0(fx)->set_rotation(fx, &angles);
        Vtable_75b0(fx)->set_scale(fx, &scale);
        fn_7100146dac(reinterpret_cast<UnkSwitchObject*>(fx), true);
        Vtable_75b0(fx)->set_loop(fx, false);
        Vtable_75b0(fx)->set_speed(fx, 1.0f);
        fn_71011a9bb4(manager, fx);
    }
}

// CMorphBallMovement's unit: the reset on entering the ball. Runs fn_71009796dc, fn_710093ed64, sets
// the byte at +0x2489, runs fn_710093efdc, drops the id table's entry at +0xa00 from the owner's
// effect (+0x228, +0x50) through fn_7100199420, hands the value vf_0x2a8 gives to vf_0x2b0 and, when the
// object at +0x2280 has a preset path (+0xc0), resolves the preset ids at +0x2370 from
// "audiopreset:system/snd/presets/" and that name, as fn_7100be4760 does (src/game/CCharClassStopAudioPresetEvent/).
// Names and layouts are placeholders.

using base::global::CRntString;
using base::global::CStrId;

struct UnkPathIds;
struct UnkInlineString;
struct UnkMove_b152;

struct UnkMorphBall_eb64;
void fn_710093ed64(UnkMorphBall_eb64* self);
void fn_710093efdc(UnkMorphBall_eb64* self, u32 a, u32 b, u32 c);

struct UnkIdHolder_eb64 {
    u8 pad_0x0[0x30];
    u64 id;
};

// The ids the builder fills in place, destroyed in reverse order.
struct UnkPresetIds_eb64 {
    union {
        CStrId id;
    };
    union {
        base::UnkStrIdMemberOf<g_unk_7101d47200> path;
    };

    explicit UnkPresetIds_eb64(const CRntString& source) {
        fn_710001f074(reinterpret_cast<UnkPathIds*>(this), reinterpret_cast<const UnkInlineString*>(&source));
    }
    ~UnkPresetIds_eb64() {
        path.~UnkStrIdMemberOf();
        id.~CStrId();
    }
};

// A pooled string: the text at +0x10 and a use count at +0x44.
struct UnkPresetName_eb64 {
    u8 pad_0x0[0x10];
    CRntString text;  // 0x10
    u8 pad_0x10[0x44 - 0x10 - sizeof(CRntString)];
    union {
        s32 use_count;  // 0x44
    };
};

struct UnkMorphBallSub_eb64 {
    u8 pad_0x0[0xc0];
    base::UnkStrIdMemberOf<g_unk_7101d47200> field_0xc0;
};

struct UnkMorphBallEffect_eb64 {
    u8 pad_0x0[0x138];
    u8* field_0x138;
};

struct UnkMorphBallEffectHolder_eb64 {
    u8 pad_0x0[0x50];
    UnkMorphBallEffect_eb64* field_0x50;
};

struct UnkMorphBallActor_eb64 {
    u8 pad_0x0[0x228];
    UnkMorphBallEffectHolder_eb64* field_0x228;
};

struct UnkMorphBall_eb64 {
    void** vtable;
    u8 pad_0x8[0x30 - 0x8];
    UnkMorphBallActor_eb64* owner;  // 0x30
    u8 pad_0x38[0x2280 - 0x38];
    UnkMorphBallSub_eb64* field_0x2280;
    u8 pad_0x2288[0x2370 - 0x2288];
    UnkStrIdPathResource preset;  // 0x2370
    u8 pad_0x2388[0x2489 - 0x2388];
    u8 field_0x2489;

    void* vf_0x2a8() { return reinterpret_cast<void* (*)(UnkMorphBall_eb64*)>(vtable[0x2a8 / 8])(this); }
    void vf_0x2b0(void* value) { reinterpret_cast<void (*)(UnkMorphBall_eb64*, void*)>(vtable[0x2b0 / 8])(this, value); }
};

void fn_710093eb64(UnkMorphBall_eb64* self) {
    fn_71009796dc(reinterpret_cast<UnkMove_b152*>(self));
    fn_710093ed64(self);
    self->field_0x2489 = 1;
    fn_710093efdc(self, 1, 0, 1);
    {
        UnkMorphBallEffect_eb64* effect = self->owner->field_0x228->field_0x50;
        u64 id = reinterpret_cast<UnkIdHolder_eb64*>(GameManagerIdTable().field_0xa00.data)->id;
        fn_7100199420(*reinterpret_cast<void**>(effect->field_0x138 + 0x78), &id, false);
    }
    self->vf_0x2b0(self->vf_0x2a8());
    UnkMorphBallSub_eb64* sub = self->field_0x2280;
    if (sub && sub->field_0xc0.data != &g_unk_7101d47200->empty) {
        UnkStrIdPathResource* preset = &self->preset;
        UnkPresetName_eb64* name = reinterpret_cast<UnkPresetName_eb64*>(sub->field_0xc0.data);
        ++name->use_count;
        fn_71000267f8(preset, reinterpret_cast<const UnkStrIdPathPair&>(static_cast<const UnkPresetIds_eb64&>(
                                  UnkPresetIds_eb64(fn_710007f050("audiopreset:system/snd/presets/",
                                                                  &reinterpret_cast<UnkPresetName_eb64*>(
                                                                       sub->field_0xc0.data)->text)))));
    }
}
