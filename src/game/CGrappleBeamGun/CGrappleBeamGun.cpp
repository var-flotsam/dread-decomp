// CGrappleBeamGun: fn_7100833cf4 releases the grapple when the gun allows it (virtual slot 9): the
// grabbed object at +0x1880 gets its state set (fn_7100232864) and, unless it is busy at +0x1a8, is
// registered with the game manager's points (fn_71011a2770); the hooked object at +0x1888 is released
// (fn_71008189a4) and its damage request at +0x348 set to the gun's value (virtual slot 48); then the
// release sound plays at the hooked object's position when the owner has its component at +0x1e0.
// Names and layouts are placeholders.
#include "base/math/UnkZeroVector3.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CScenario.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::math::CVector3D;

struct UnkRequest;
struct UnkEntityState;

// The sound handle fn_7100ed6910 returns (as src/game/CHyperBeamGun.cpp declares it).
struct UnkHyperBeamSoundRef;
struct UnkHyperBeamSoundHandle {
    UnkHyperBeamSoundRef* ref;
    s64 serial;
};

struct UnkGrappleObject {
    u8 pad_0x0[0x30];
    UnkActor* actor;  // 0x30
    u8 pad_0x38[0x1a8 - 0x38];
    void* field_0x1a8;
    u8 pad_0x1b0[0x348 - 0x1b0];
    u8 request[0x8];  // 0x348
};

struct UnkGrappleRef {
    u8 pad_0x0[0x10];
    UnkGrappleObject* obj;  // 0x10
};

static inline UnkGrappleObject* RefObject(UnkGrappleRef* ref) {
    return ref ? ref->obj : nullptr;
}

struct UnkGrapplePosition {
    f32 x, y, z;
};

struct UnkGrappleOwner {
    u8 pad_0x0[0x1e0];
    void* field_0x1e0;
};

struct UnkGrappleBeamGun {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual bool vf_0x48();
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
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual void vf_0x160();
    virtual void vf_0x168();
    virtual void vf_0x170();
    virtual void vf_0x178();
    virtual f32 vf_0x180();

    u8 pad_0x8[0x10];
    UnkGrappleOwner* owner;  // 0x18
    u8 pad_0x20[0x1874 - 0x20];
    u32 field_0x1874;
    u8 pad_0x1878[0x8];
    UnkGrappleRef* grabbed;  // 0x1880
    UnkGrappleRef* hooked;  // 0x1888
};

void fn_7100232864(UnkEntityState* entity, bool value);
void fn_71008189a4(UnkGrappleObject* obj);
void fn_71008c1538(UnkRequest* request, f32 value);
UnkHyperBeamSoundHandle fn_7100ed6910(void* player, const CVector3D* pos, u32 id, u32 kind, bool flag);

bool fn_7100833cf4(UnkGrappleBeamGun* self) {
    if (!self->vf_0x48())
        return false;
    if (self->grabbed && self->grabbed->obj) {
        fn_7100232864(reinterpret_cast<UnkEntityState*>(self->grabbed->obj), true);
        if (!RefObject(self->grabbed)->field_0x1a8)
            if (UnkGameManagerSub_0x608* points = g_unk_7101d47310->field_0x608)
                fn_71011a2770(points, RefObject(self->grabbed));
    }
    if (self->hooked && self->hooked->obj) {
        fn_71008189a4(self->hooked->obj);
        UnkGrappleObject* obj = RefObject(self->hooked);
        f32 value = self->vf_0x180();
        fn_71008c1538(reinterpret_cast<UnkRequest*>(obj->request), value);
    }
    if (self->owner && self->owner->field_0x1e0) {
        void* player = g_unk_71726af0b0;
        UnkActor* actor = self->hooked->obj->actor;
        f32 x = actor->field_0x44;
        f32 y = actor->field_0x54;
        f32 z = actor->field_0x64;
        UnkGrapplePosition pos;
        pos.x = x;
        pos.y = y;
        pos.z = z;
        fn_7100ed6910(player, reinterpret_cast<const CVector3D*>(&pos), self->field_0x1874, 3, false);
    }
    return true;
}
