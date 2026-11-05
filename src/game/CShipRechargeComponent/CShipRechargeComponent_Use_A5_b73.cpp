// CShipRechargeComponent unit (tools/tu_map.py, unit 1598): the level-change use. Calls the owner's
// "OnLevelChangeUse" script with the level name and the user's name, takes a weak handle to the user, starts the
// use through fn_7100aa0b04 when a level is named (otherwise flags +0x53a) and requests state 6 from the usable
// component's state machine at +0x100 ("none" 8). Names and layouts are placeholders.
#include "../UnkSmRequest_A5_b73.h"
#include "base/global/CRntString.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkScriptCaller;
bool fn_7100305088(UnkScriptCaller* self, const char* name, const char* fmt, ...);
UnkWeakHandle* fn_7100d78378(UnkWeakTarget* self);
struct UnkUsableVirt_A6_b62;
bool fn_7100aa0b04(UnkUsableVirt_A6_b62* self, u64 a, u64 b, const u8* c);
extern const u8 g_unk_710164b5d8[] __attribute__((visibility("hidden")));

struct UnkShipRecharge_A5_b73;
u64 fn_7100aa1f50(UnkShipRecharge_A5_b73* self, u32 state);
u64 fn_7100aa1888(UnkShipRecharge_A5_b73* self, u32 state);

struct UnkShipRechargeOwner_A5_b73 {
    u8 pad_0x0[0x1e0];
    UnkScriptCaller* script;
};

struct UnkShipRechargeName_A5_b73 {
    u8 pad_0x0[0x10];
    const char* name;
    u8 pad_0x18[0x44 - 0x18];
    s32 reads;
};

struct UnkShipRechargeUser_A5_b73 {
    u8 pad_0x0[0x30];
    UnkShipRechargeName_A5_b73* name;
};

struct UnkShipRecharge_A5_b73 {
    u8 pad_0x0[0x30];
    UnkShipRechargeOwner_A5_b73* owner;
    u8 pad_0x38[0xe8 - 0x38];
    UnkWeakHandle* user;
    u8 pad_0xf0[0x100 - 0xf0];
    UnkSmMachine_A5_b73 machine;
    u8 pad_0x1b0[0x53a - 0x1b0];
    bool field_0x53a;
    u8 pad_0x53b[0x53c - 0x53b];
    u32 field_0x53c;
};
static_assert(__builtin_offsetof(UnkShipRecharge_A5_b73, field_0x53c) == 0x53c, "layout");

// The count is bumped through an aliasing type: the original reloads the name pointer after it.
typedef s32 __attribute__((may_alias)) UnkAliasS32_A5_b73;

static inline const char* OrEmpty_A5_b73(const char* s) {
    return s ? s : "";
}

bool fn_7100a8c318(UnkShipRecharge_A5_b73* self, UnkShipRechargeUser_A5_b73* user, const char* const* level) {
    if (!user)
        return false;
    self->field_0x53a = false;
    if (UnkScriptCaller* script = self->owner->script) {
        const char* a = OrEmpty_A5_b73(*level);
        ++*reinterpret_cast<UnkAliasS32_A5_b73*>(&user->name->reads);
        fn_7100305088(script, "OnLevelChangeUse", "ss", a, OrEmpty_A5_b73(user->name->name));
    }
    AssignRef(&self->user, fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(user)));
    if (*level && **level) {
        u64 a = fn_7100aa1f50(self, 6);
        u64 b = fn_7100aa1888(self, 6);
        fn_7100aa0b04(reinterpret_cast<UnkUsableVirt_A6_b62*>(self), a, b, g_unk_710164b5d8);
    } else {
        self->field_0x53a = true;
    }
    RequestState_A5_b73<15, 8>(&self->machine, 6);
    self->field_0x53c = 0;
    return true;
}

// Once only (+0x539): asks for the text vf_0x220 builds and shows it through vf_0x260 as "ShowDialogChoice".
struct UnkShipRechargeDialog_A5_b73 {
    virtual void vf_0x0();
    base::global::CRntString vf_0x220() {
        return reinterpret_cast<base::global::CRntString (*)(UnkShipRechargeDialog_A5_b73*)>(
            (*reinterpret_cast<void***>(this))[0x220 / 8])(this);
    }
    void vf_0x260(const char* name, const char* text) {
        reinterpret_cast<void (*)(UnkShipRechargeDialog_A5_b73*, const char*, const char*)>(
            (*reinterpret_cast<void***>(this))[0x260 / 8])(this, name, text);
    }
    u8 pad_0x8[0x539 - 0x8];
    bool shown;
};

bool fn_7100a8bb78(UnkShipRechargeDialog_A5_b73* self) {
    if (!self->shown) {
        self->shown = true;
        base::global::CRntString text = self->vf_0x220();
        self->vf_0x260("ShowDialogChoice", OrEmpty_A5_b73(text.data));
    }
    return true;
}
