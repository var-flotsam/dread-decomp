// CCharClassDamageTriggerComponent's unit (tools/tu_map.py): the component's initialisation. After
// the base one (fn_7100a46e00) the character class (+0x10) is kept at +0xbf8 when it is a
// CCharClassDamageTriggerComponent; when the component is enabled (+0x21) the class's damage is
// copied in: its amount (the value its id at +0x70 names, fn_7100789f5c, or the plain one at +0x68
// for the empty id) and the four words after it go to both copies (+0xbe4, +0xbc4), then the type
// id (+0x88), the word at +0xa0 and the flags at +0x90/+0x91/+0x94. Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CCharClassAttackComponent.h"
#include "game/CCharClassBaseTriggerComponent.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CClass;
using base::reflection::UnkReflObject;

struct UnkSteamJetComponent;
struct UnkSteamJetArg;

extern CClass g_unk_717274b070 __attribute__((visibility("hidden")));

static inline const CClass* DamageTriggerClass() {
    fn_7100a49840();
    return &g_unk_717274b070;
}

// A damage amount, copied as a whole.
struct UnkDamageAmount_dtc {
    f32 value;
};

struct UnkDamageExtra_dtc {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
};

struct UnkCharClassDamageTrigger_dtc {
    u8 pad_0x0[0x68];
    UnkDamageAmount_dtc amount;  // 0x68
    u8 pad_0x6c[4];
    CStrId amount_id;      // 0x70
    UnkDamageExtra_dtc extra;  // 0x78
    CStrId type;           // 0x88
    u8 flag_0x90;
    u8 flag_0x91;
    // Read through a plain byte pointer (the original reloads it after the word store).
    u8 Flag90() const { return *reinterpret_cast<const u8*>(&flag_0x90); }
    u8 pad_0x92[2];
    u32 field_0x94;
    u8 pad_0x98[8];
    u32 field_0xa0;
};

struct UnkDamageTriggerComponent_dtc {
    u8 pad_0x0[0x10];
    UnkReflObject* char_class;  // 0x10
    u8 pad_0x18[9];
    bool enabled;  // 0x21
    u8 pad_0x22[0xadc - 0x22];
    u32 field_0xadc;
    u8 pad_0xae0;
    u8 flag_0xae1;
    u8 pad_0xae2[7];
    u8 flag_0xae9;
    u8 pad_0xaea[0xb74 - 0xaea];
    u32 field_0xb74;
    CStrId type;  // 0xb78
    u8 pad_0xb80[0xbc4 - 0xb80];
    UnkDamageAmount_dtc amount;  // 0xbc4
    UnkDamageExtra_dtc extra;  // 0xbc8
    u8 pad_0xbd8[8];
    u8 flag_0xbe0;
    u8 pad_0xbe1[3];
    UnkDamageAmount_dtc base_amount;  // 0xbe4
    UnkDamageExtra_dtc base_extra;  // 0xbe8
    UnkCharClassDamageTrigger_dtc* def;  // 0xbf8
};
static_assert(__builtin_offsetof(UnkDamageTriggerComponent_dtc, def) == 0xbf8, "UnkDamageTriggerComponent_dtc layout");

static inline void CopyExtra_dtc(UnkDamageExtra_dtc* dst, const UnkDamageExtra_dtc* src) {
    dst->field_0x0 = src->field_0x0;
    dst->field_0x4 = src->field_0x4;
    dst->field_0x8 = src->field_0x8;
    dst->field_0xc = src->field_0xc;
}

bool fn_7100a4add0(UnkDamageTriggerComponent_dtc* self, UnkSteamJetArg* arg) {
    bool result = fn_7100a46e00(reinterpret_cast<UnkSteamJetComponent*>(self), arg);
    UnkReflObject* obj = self->char_class;
    self->def = obj && fn_710009d50c(obj->vf_0x0(), DamageTriggerClass())
                    ? reinterpret_cast<UnkCharClassDamageTrigger_dtc*>(obj)
                    : nullptr;
    if (self->enabled) {
        if (self->def->amount_id.data != &g_unk_7101d47108->empty)
            self->base_amount.value = fn_7100789f5c(&self->def->amount_id);
        else
            self->base_amount = self->def->amount;
        CopyExtra_dtc(&self->base_extra, &self->def->extra);
        self->amount = self->base_amount;
        CopyExtra_dtc(&self->extra, &self->def->extra);
        self->type = self->def->type;
        self->field_0xb74 = self->def->field_0xa0;
        self->flag_0xae1 = self->def->flag_0x90;
        self->flag_0xae9 = self->def->flag_0x91;
        self->field_0xadc = self->def->field_0x94;
        self->flag_0xbe0 = self->def->Flag90();
    }
    return result;
}
