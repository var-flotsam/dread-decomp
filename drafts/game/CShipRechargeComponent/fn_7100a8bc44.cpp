// CShipRechargeComponent unit (tools/tu_map.py): while linked and allowed to recharge, starts the
// recharge sound ("savestation_recharge.wav") on the owner entity unless it is already playing, then
// runs the three recharge steps. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/sound/UnkSoundDesc.h"
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
        static UnkSoundDesc s_params;
        s_params.kind.field_0x0 = 0;
        s_params.path = s_path;
        s_params.field_0x38 = 0.6f;
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
