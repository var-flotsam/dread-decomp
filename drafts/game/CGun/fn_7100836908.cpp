// CGun unit: at most once per cooldown (+0x1878, reset to 0.3), plays a sound for the owner's AI
// component: fills a shared sound request (a guarded static) with the sound the AI gives for `kind`
// and the position, and starts it on the entity at +0x18. Names and layouts are placeholders.
#include <new>

#include "base/UnkAtExit.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);

struct UnkEntityWithSound;
UnkSoundHandle fn_7100d7c2f0(UnkEntityWithSound* self, const u32* params, bool a, bool b, bool c);

// The request's destructor (src/game/CAbilitySonar/fn_7100333544.cpp): it releases the two ids.
struct UnkIdPathPairAt18_710034e0b8;
void fn_710034e0b8(UnkIdPathPairAt18_710034e0b8* self);

struct UnkPos_l836908 {
    f32 x, y, z;
};

// A sound request (what fn_7100d7c2f0 reads).
struct UnkSoundRequest_l836908 {
    u32 field_0x0;
    f32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    CStrId id;             // 0x18
    CFilePathStrId path;   // 0x20
    UnkPos_l836908 pos;    // 0x28
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

    UnkSoundRequest_l836908()
        : field_0x0(0), field_0x4(0.0f), field_0x8(0), field_0xc(0), field_0x10(0), id(g_unk_7101d470e0),
          path(g_unk_7101d470e8), pos{0.0f, 0.0f, 0.0f}, field_0x34(-1.0f), field_0x38(1.0f),
          field_0x3c(500.0f), field_0x40(2000.0f), field_0x44(1.0f), field_0x48(0.0f), field_0x4c(0.0f),
          field_0x50(0.0f), field_0x54(-1.0f), field_0x58(-1.0f), field_0x5c(-1.0f), field_0x60(0.5f),
          field_0x64(5.0f), field_0x68(10.0f), field_0x6c(false), field_0x6d(false), field_0x6e(false) {}
};
static_assert(__builtin_offsetof(UnkSoundRequest_l836908, field_0x6e) == 0x6e, "UnkSoundRequest_l836908 layout");

// The request, built on first use behind its guard.
struct UnkGuardedRequest_l836908 {
    u64 guard;
    UnkSoundRequest_l836908 request;
};
extern UnkGuardedRequest_l836908 g_unk_7172725f18 __attribute__((visibility("hidden")));

// The owner's AI component (a CCharClassAIComponent): slot 0xa8 gives the sound for a kind.
struct UnkGunAI_l836908 : base::reflection::UnkReflObject {
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual const CFilePathStrId* vf_0xa8(u32 kind);
};

// The object if it is a CCharClassAIComponent, else null.
inline UnkGunAI_l836908* AsAIComponent(UnkGunAI_l836908* object) {
    if (!object)
        return nullptr;
    return fn_710009d50c(object->vf_0x0(), fn_7100408b2c()) ? object : nullptr;
}

struct UnkGunAIHolder_l836908 {
    u8 pad_0x0[0x10];
    UnkGunAI_l836908* ai;  // 0x10
};

struct UnkGunOwner_l836908 {
    u8 pad_0x0[0x240];
    UnkGunAIHolder_l836908* field_0x240;
};

struct UnkGun_l836908 {
    u8 pad_0x0[0x18];
    UnkEntityWithSound* entity;  // 0x18
    u8 pad_0x20[0x1878 - 0x20];
    f32 cooldown;  // 0x1878
};

void fn_7100836908(UnkGun_l836908* self, u32 kind, const UnkPos_l836908* pos, UnkGunOwner_l836908* owner) {
    if (self->cooldown > 0.0f)
        return;
    self->cooldown = 0.3f;
    if (!owner || !owner->field_0x240)
        return;

    UnkGunAI_l836908* ai = AsAIComponent(owner->field_0x240->ai);

    UnkGuardedRequest_l836908& g = g_unk_7172725f18;
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g.guard), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&g.guard)) {
            new (&g.request) UnkSoundRequest_l836908();
            fn_7100000250(reinterpret_cast<void (*)(void*)>(fn_710034e0b8), &g.request, g_unk_7101a19000);
            __cxa_guard_release(&g.guard);
        }
    }

    UnkSoundRequest_l836908& request = g.request;
    request.field_0x0 = 0;
    request.path = *ai->vf_0xa8(kind);
    request.pos.x = pos->x;
    request.pos.y = pos->y;
    request.pos.z = pos->z;
    request.field_0x6d = true;
    request.field_0x38 = 1.7f;
    request.field_0x44 = 1.05f;
    request.field_0x4 = 0.0f;
    request.field_0x3c = 300.0f;
    request.field_0x40 = 3000.0f;
    if (self->entity)
        fn_7100d7c2f0(self->entity, reinterpret_cast<const u32*>(&request), false, true, true);
}
