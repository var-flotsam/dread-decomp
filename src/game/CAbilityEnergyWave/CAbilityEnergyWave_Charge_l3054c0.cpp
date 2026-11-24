// CAbilityEnergyWave unit: shows the rocket ball burst effects for whether the owner's gun is charging,
// prepares the ice charge loop rumble while charging without one, and the charging effect while the
// stored amount is above zero and the gun is not charging. Names and layouts are placeholders.
#include "base/UnkAtExit.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CFXComponent.h"
#include "game/CGunComponent.h"
#include "types.h"

struct UnkComponent_710035131c;

// The part a world shows for an effect name.
struct UnkBurstPart_l3054c0 {
    u8 pad_0x0[0xb];
    u8 kind;  // 0xb
    u8 pad_0xc[0x115];
    bool active;  // 0x121
};

struct UnkGunState_l3054c0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual bool IsCharging();  // 0x48
};

struct UnkGun_l3054c0 {
    u8 pad_0x0[0x128];
    bool disabled;  // 0x128
};

struct UnkOwner_l3054c0 {
    u8 pad_0x0[0x280];
    UnkWorld* world;  // 0x280
    u8 pad_0x288[0x20];
    UnkGun_l3054c0* gun;  // 0x2a8
};

struct UnkOwnerComponent_l3054c0 {
    u8 pad_0x0[0x30];
    UnkOwner_l3054c0* owner;  // 0x30
};

struct UnkObjectHandle_l3054c0 {
    struct Slot {
        void* object;
        u64 id;
    };
    Slot* slot;
    u64 id;

    bool IsValid() const { return slot && slot->id == id && id != (u64)-1 && slot->object; }
};

struct UnkEnergyWave_l3054c0 {
    u8 pad_0x0[0xc0];
    UnkOwnerComponent_l3054c0* component;  // 0xc0
    u8 pad_0xc8[0x310];
    f32 amount;  // 0x3d8
    u8 pad_0x3dc[0x24];
    UnkObjectHandle_l3054c0 rumble;  // 0x400
};

static inline u64 HashOf(const char* name) { return StringCrc64(name, __builtin_strlen(name)); }

// The part of the effect, or null when the world does not show it.
static inline UnkBurstPart_l3054c0* LivePart(UnkWorld* world, u64 hash) {
    auto* part = reinterpret_cast<UnkBurstPart_l3054c0*>(fn_7101152b90(world, hash));
    return fn_710114e668(reinterpret_cast<UnkObject*>(world), part->kind) ? part : nullptr;
}

void fn_71003054c0(UnkEnergyWave_l3054c0* self) {
    UnkOwner_l3054c0* owner = self->component->owner;
    UnkGun_l3054c0* gun = owner ? owner->gun : nullptr;
    UnkWorld* world = owner->world;
    bool charging = false;
    if (gun && !gun->disabled)
        charging = static_cast<UnkGunState_l3054c0*>(fn_710083cd30(reinterpret_cast<UnkComponent_710035131c*>(gun)))->IsCharging();

    bool centerOff = !LivePart(world, HashOf("specialburst_rocketballcenter"))->active;
    fn_710114fe8c(world, HashOf("specialburst_rocketball"), charging, 0);
    fn_710114fe8c(world, HashOf("specialburst_rocketballcenter"), charging, charging && centerOff);

    if (charging && !self->rumble.IsValid()) {
        base::global::CFilePathStrId path = g_unk_7101d470e8;
        static base::global::CFilePathStrId s_chargedLoop("weapons/chargedloop_ice.nvib", true);
        path = s_chargedLoop;
    }

    f32 amount = self->amount;
    UnkWorld* fxWorld = self->component->owner->world;
    fn_710114ffb8(reinterpret_cast<UnkRinkaControl_ruc*>(fxWorld), HashOf("specialburst_rocketballcharging"),
                  (amount > 0.0f) & !charging, false);
}
