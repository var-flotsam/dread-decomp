// CKraidShockerSplashMovementComponent's unit: the hit test. A hit on the ground (not on an actor whose +0x240
// or +0x200 is set) before the splash has started spawns two shock waves running out from the owner, one each
// way, keeps weak handles to both and shows the splash's two beams at their fronts; anything else goes to the
// base test fn_7100ac4ec0. Names and layouts are placeholders.
#include "base/color/CColor4F.h"
#include "base/global/CStrId.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "game/CAutsharpAIComponent.h"
#include "game/CCharClassAttackComponent.h"
#include "game/CFXComponent.h"
#include "game/CProjectileMovement.h"
#include "game/CShockWaveComponent.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU133.h"
#include "game/UnkTU1370.h"
#include "game/UnkTU85.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::color::CColor4F;
using base::global::CStrId;
using base::reflection::CType;
using base::reflection::UnkReflTypedPtr;

struct UnkSlots;
bool fn_71001c2a2c(UnkSlots* self, u32 kind, const CColor4F* color, u32 mode);

struct UnkReflField;
struct UnkReflFieldObject;
struct UnkReflValueArg;

__attribute__((visibility("hidden"))) extern CStrId g_unk_71726f44b0;
__attribute__((visibility("hidden"))) extern CStrId g_unk_71726f44b8;

struct UnkShockerActor_l6077d4 : UnkWeakTarget {
    u8 pad_0x8[0x30];
    UnkXform transform;  // 0x38
    u8 pad_0x94[0x1ec];
    UnkWorld* world;  // 0x280
};
static_assert(__builtin_offsetof(UnkShockerActor_l6077d4, world) == 0x280, "actor layout");

struct UnkShockerOther_l6077d4 {
    u8 pad_0x0[0x200];
    void* field_0x200;
    u8 pad_0x208[0x38];
    void* field_0x240;
};

struct UnkShockerComponent_l6077d4 {
    u8 pad_0x0[0x284];
    f32 halfWidth;  // 0x284
};
UnkShockerComponent_l6077d4* fn_710048fba4(UnkShockerActor_l6077d4* actor);

// The shock wave description fn_71009f35b0 builds (0x210 bytes) and fn_71003cc474 destroys; fn_71009f51f8
// spawns the wave it describes for an owner.
struct UnkShockWaveDesc_l6077d4 {
    u8 pad_0x0[0x8];
    char request[0xb8];  // 0x8
    CStrId name;         // 0xc0
    u8 pad_0xc8[0x38];
    f32 field_0x100;
    u8 pad_0x104[0x8];
    u32 field_0x10c;
    bool field_0x110;
    u8 pad_0x111[3];
    UnkVec3f position;  // 0x114
    bool field_0x120;
    u8 pad_0x121[3];
    f32 field_0x124;
    u8 pad_0x128[0x8];
    f32 field_0x130;
    f32 field_0x134;
    bool field_0x138;
    u8 pad_0x139[0x4f];
    UnkWeakHandle* owner;  // 0x188
    u8 pad_0x190[0x20];
    CStrId field_0x1b0;
    u8 pad_0x1b8[0x10];
    f32 speed;      // 0x1c8
    f32 direction;  // 0x1cc
    f32 field_0x1d0;
    f32 field_0x1d4;
    u8 pad_0x1d8[0x24];
    bool field_0x1fc;
    u8 pad_0x1fd[3];
    f32 field_0x200;
    u8 pad_0x204[0xc];
};
static_assert(sizeof(UnkShockWaveDesc_l6077d4) == 0x210, "shock wave description layout");

UnkShockerActor_l6077d4* fn_71009f51f8(char* desc, UnkShockerActor_l6077d4* owner);

struct UnkShockerSplash_l6077d4 {
    u8 pad_0x0[0x30];
    UnkShockerActor_l6077d4* owner;  // 0x30
    u8 pad_0x38[0x5a0];
    UnkWeakHandle* left;   // 0x5d8
    UnkWeakHandle* right;  // 0x5e0
    f32 time;              // 0x5e8
    f32 duration;          // 0x5ec
};
static_assert(__builtin_offsetof(UnkShockerSplash_l6077d4, left) == 0x5d8, "splash layout");

// A reflected field (0xb0 bytes).
struct UnkShockerField_l6077d4 {
    u8 pad_0x0[0xb0];
};

// A class as the lookup sees it: its fields and their name hashes.
struct UnkShockerClass_l6077d4 {
    u8 pad_0x0[0xe8];
    UnkShockerField_l6077d4* fields;  // 0xe8
    u64* keys;                        // 0xf0
    u32 count;                        // 0xf8

    UnkShockerField_l6077d4* End() { return fields + count; }
    UnkShockerField_l6077d4* Find(u64 key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &fields[i];
        return End();
    }
    UnkShockerField_l6077d4* FindField(u64 key) {
        UnkShockerField_l6077d4* it = Find(key);
        return it != End() ? it : nullptr;
    }
};

struct UnkShockerTunable_l6077d4 {
    virtual UnkShockerClass_l6077d4* GetClass();
};

struct UnkShockerStrData_l6077d4 {
    u8 pad_0x0[0x30];
    u64 key;
};

struct UnkShockerValue_l6077d4 {
    const CType* type;
    void* value;
};

// Reads the float field named by `key` of the tunable into `out`, when the tunable has one.
static inline void Get(UnkShockerTunable_l6077d4* obj, u64 key, f32* out) {
    UnkShockerClass_l6077d4* cls = obj->GetClass();
    UnkReflTypedPtr typed;
    typed.type = reinterpret_cast<char*>(obj->GetClass());
    typed.ptr = obj;
    UnkShockerValue_l6077d4 value = {fn_710000abbc(), out};
    if (UnkShockerField_l6077d4* field = cls->FindField(key))
        fn_71000e78e0(reinterpret_cast<UnkReflField*>(field), reinterpret_cast<const UnkReflFieldObject*>(&typed),
                      reinterpret_cast<UnkReflValueArg*>(&value));
}

static inline UnkShockerActor_l6077d4* ActorOf(UnkWeakHandle* handle) {
    return handle ? static_cast<UnkShockerActor_l6077d4*>(handle->target) : nullptr;
}

// Where the actor `handle` points at stands (the handle must point at one).
static inline UnkVec3f PositionOf(UnkWeakHandle* handle) {
    return static_cast<UnkShockerActor_l6077d4*>(handle->target)->transform.GetTranslation();
}

// The world's part with this name hash, or null when there is none or it is not shown.
static inline UnkSwitchObject* BeamOf(UnkWorld* world, u64 hash) {
    UnkPart* part = fn_7101152b90(world, hash);
    if (part && !fn_710114e680(reinterpret_cast<UnkObject*>(world), reinterpret_cast<u8*>(part)[0xb]))
        part = nullptr;
    return reinterpret_cast<UnkSwitchObject*>(part);
}

static inline UnkWeakHandle* Spawn(UnkShockWaveDesc_l6077d4* desc, UnkShockerActor_l6077d4* owner) {
    UnkShockerActor_l6077d4* wave = fn_71009f51f8(reinterpret_cast<char*>(desc), owner);
    return wave ? GetWeakHandle(wave) : nullptr;
}

// Spawns the two shock waves and shows the beams at their fronts.
static inline void Splash(UnkShockerSplash_l6077d4* self) {
    alignas(8) char storage[sizeof(UnkShockWaveDesc_l6077d4)];
    fn_71009f35b0(storage);
    UnkShockWaveDesc_l6077d4* desc = reinterpret_cast<UnkShockWaveDesc_l6077d4*>(storage);
    AssignRef(&desc->owner, GetWeakHandle(self->owner));
    desc->field_0x1b0 = g_unk_71726f44b0;
    desc->position = self->owner->transform.GetTranslation();
    desc->field_0x120 = true;
    desc->field_0x124 = 100.0f;
    desc->field_0x130 = 0.1f;
    desc->field_0x134 = -1.0f;
    desc->field_0x10c = 1;
    desc->name = g_unk_71726f44b8;
    f32 value = desc->field_0x100;
    Get(reinterpret_cast<UnkShockerTunable_l6077d4*>(&g_unk_7172716160),
        reinterpret_cast<UnkShockerStrData_l6077d4*>(g_unk_71726f44b8.data)->key, &value);
    fn_71008c1538(reinterpret_cast<UnkRequest*>(desc->request), value);
    desc->field_0x138 = true;
    desc->field_0x1fc = true;
    desc->field_0x200 = 100.0f;
    desc->speed = 900.0f;
    desc->direction = 1.0f;
    desc->field_0x110 = true;

    desc->field_0x1d0 = self->owner->transform.m[0][3] + 600.0f;
    AssignRef(&self->right, Spawn(desc, self->owner));
    desc->direction = -1.0f;
    desc->field_0x1d4 = self->owner->transform.m[0][3] - 600.0f;
    AssignRef(&self->left, Spawn(desc, self->owner));

    if (ActorOf(self->right) && ActorOf(self->left)) {
        self->time = 0.0f;
        self->duration = 600.0f / desc->speed;
        if (UnkWorld* world = self->owner->world) {
            CColor4F color(1.0f, 1.0f, 1.0f, 0.0f);
            UnkSwitchObject* beam = BeamOf(world, 0x89f328dc37ac167bull);
            UnkVec3f pos = PositionOf(self->right);
            pos.x = pos.x - fn_710048fba4(ActorOf(self->right))->halfWidth;
            fn_7100145e94(beam, true);
            fn_7100145f28(beam, &pos);
            fn_71001c2a2c(reinterpret_cast<UnkSlots*>(beam->field_0x160), 0, &color, 0);

            beam = BeamOf(world, 0x8c9dfb3ece4e323cull);
            pos = PositionOf(self->left);
            pos.x = fn_710048fba4(ActorOf(self->left))->halfWidth + pos.x;
            fn_7100145e94(beam, true);
            fn_7100145f28(beam, &pos);
            fn_71001c2a2c(reinterpret_cast<UnkSlots*>(beam->field_0x160), 0, &color, 0);
        }
    }
    fn_71003cc474(reinterpret_cast<UnkShockWaveMember*>(storage));
}

bool fn_71006077d4(UnkShockerSplash_l6077d4* self, UnkShockerOther_l6077d4* other, void* a2, void* a3, void* a4,
                   void* a5) {
    if (!other)
        return false;
    if (!other->field_0x240 && !other->field_0x200 && self->time == 0.0f) {
        Splash(self);
        return true;
    }
    return fn_7100ac4ec0(reinterpret_cast<UnkObj*>(self), other, a2, a3, a4, a5);
}
