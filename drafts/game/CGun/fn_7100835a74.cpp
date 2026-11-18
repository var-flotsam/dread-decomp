// CGun unit (tools/tu_map.py unit 1302): stamps the gun with the game time (g_unk_7101d47310 +0x2444),
// runs its vf_0x1c8, turns a pending request (+0x1873) into an active one (+0x1872), hands the actor's
// position (+0x18; +0x44, +0x54, +0x64) to the game manager's set at +0x2028 (fn_710056a010) and lets
// the actor's object at +0x2b0 update (fn_710035131c). Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU811.h"
#include "types.h"

struct UnkSelf;
void fn_710035131c(UnkSelf* self);

// The game manager seen from here: the set at +0x2028.
struct UnkGameManagerView_l835a74 {
    u8 pad_0x0[0x2028];
    u8 field_0x2028[1];
};

struct UnkGun_l835a74 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
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
    virtual void vf_0x180();
    virtual void vf_0x188();
    virtual void vf_0x190();
    virtual void vf_0x198();
    virtual void vf_0x1a0();
    virtual void vf_0x1a8();
    virtual void vf_0x1b0();
    virtual void vf_0x1b8();
    virtual void vf_0x1c0();
    virtual void vf_0x1c8();

    u8 pad_0x8[0x10];
    UnkActor* actor;  // 0x18
    u8 pad_0x20[0x24];
    f32 time;  // 0x44
    u8 pad_0x48[0x182a];
    bool active;   // 0x1872
    bool pending;  // 0x1873
};
static_assert(__builtin_offsetof(UnkGun_l835a74, active) == 0x1872, "UnkGun_l835a74 placeholder layout");

static inline base::math::CVector3D Position_l835a74(const UnkActor* actor) {
    base::math::CVector3D v;
    v.x = actor->field_0x44;
    v.y = actor->field_0x54;
    v.z = actor->field_0x64;
    return v;
}

void fn_7100835a74(UnkGun_l835a74* self) {
    UnkGameManager* manager = g_unk_7101d47310;
    self->time = manager->field_0x2444;
    self->vf_0x1c8();
    if (self->pending) {
        self->active = true;
        self->pending = false;
    }
    base::math::CVector3D pos = Position_l835a74(self->actor);
    fn_710056a010(reinterpret_cast<UnkGameManagerView_l835a74*>(g_unk_7101d47310)->field_0x2028, &pos);
    if (void* object = self->actor->field_0x2b0)
        fn_710035131c(static_cast<UnkSelf*>(object));
}
