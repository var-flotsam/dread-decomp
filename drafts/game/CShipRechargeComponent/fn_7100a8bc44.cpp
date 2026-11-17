// CShipRechargeComponent unit (tools/tu_map.py): while linked and allowed to recharge, starts the
// recharge sound ("savestation_recharge.wav") on the owner entity unless it is already playing, then
// runs the three recharge steps. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkSound;
struct UnkSoundRef {
    UnkSound* sound;
    s64 serial;
};

struct UnkEntityWithSound;
struct UnkLifeRecharge_b45a2;

UnkSoundHandle fn_7100d7c374(UnkEntityWithSound* self, void* a, void* b, bool c, bool d, bool e);
bool fn_7100aa3330(UnkLifeRecharge_b45a2* self);

struct UnkShipRechargeLink_la8bc44 {
    u8 pad_0x0[0x10];
    void* object;
};

struct UnkShipRecharge_la8bc44;
void fn_7100aa35ac(UnkShipRecharge_la8bc44* self);
void fn_7100aa35ec(UnkShipRecharge_la8bc44* self);
void fn_7100aa35cc(UnkShipRecharge_la8bc44* self);

struct UnkShipRecharge_la8bc44 {
    u8 pad_0x0[0x30];
    UnkEntityWithSound* entity;  // +0x30
    u8 pad_0x38[0xb0];
    UnkShipRechargeLink_la8bc44* link;  // +0xe8
    u8 pad_0xf0[0x449];
    bool field_0x539;
    u8 pad_0x53a[0x6];
    UnkSoundHandle sound;  // +0x540
};
static_assert(__builtin_offsetof(UnkShipRecharge_la8bc44, field_0x539) == 0x539, "");
static_assert(__builtin_offsetof(UnkShipRecharge_la8bc44, sound) == 0x540, "");

// The parameters a sound is played with.
struct UnkSoundParams_la8bc44 {
    u32 field_0x0;
    u32 field_0x4;
    u64 field_0x8;
    u32 field_0x10;
    CStrId id;            // +0x18
    CFilePathStrId path;  // +0x20
    f32 field_0x28;
    f32 field_0x2c;
    f32 field_0x30;
    f32 field_0x34;
    f32 volume;      // +0x38
    f32 field_0x3c;  // +0x3c
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

    UnkSoundParams_la8bc44()
        : field_0x0(0), field_0x4(0), field_0x8(0), field_0x10(0), id(g_unk_7101d470e0),
          path(g_unk_7101d470e8), field_0x28(0.0f), field_0x2c(0.0f), field_0x30(0.0f), field_0x34(-1.0f),
          volume(1.0f), field_0x3c(500.0f), field_0x40(2000.0f), field_0x44(1.0f), field_0x48(0.0f),
          field_0x4c(0.0f), field_0x50(0.0f), field_0x54(-1.0f), field_0x58(-1.0f), field_0x5c(-1.0f),
          field_0x60(0.5f), field_0x64(5.0f), field_0x68(10.0f), field_0x6c(false),
          field_0x6d(false), field_0x6e(false) {}
    ~UnkSoundParams_la8bc44() asm("fn_710034e0b8");
};
static_assert(__builtin_offsetof(UnkSoundParams_la8bc44, field_0x6c) == 0x6c, "");

static inline bool IsPlaying(const UnkSoundHandle& handle) {
    const UnkSoundRef* ref = handle.ref;
    return ref && ref->serial == handle.serial && handle.serial != -1 && ref->sound;
}

void fn_7100a8bc44(UnkShipRecharge_la8bc44* self) {
    if (!self->link || !self->link->object)
        return;
    if (self->field_0x539 || fn_7100aa3330(reinterpret_cast<UnkLifeRecharge_b45a2*>(self)))
        return;
    if (!IsPlaying(self->sound)) {
        static CFilePathStrId s_path("props/savestation/savestation_recharge.wav", true);
        static UnkSoundParams_la8bc44 s_params;
        s_params.field_0x0 = 0;
        s_params.path = s_path;
        s_params.volume = 0.6f;
        s_params.field_0x3c = 500.0f;
        s_params.field_0x40 = 5000.0f;
        s_params.field_0x44 = 1.0f;
        s_params.field_0x6c = true;
        s_params.field_0x6d = true;
        s_params.field_0x4c = 0.25f;
        self->sound = fn_7100d7c374(self->entity, &s_params, &g_unk_7101d470e0, false, true, true);
    }
    fn_7100aa35ac(self);
    fn_7100aa35ec(self);
    fn_7100aa35cc(self);
}
