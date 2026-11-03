// CSteamJetComponent: fn_710089e340 runs the base step (fn_7100a46e00), keeps the object at +0x10 cast
// to CCharClassSteamJetComponent at +0xc78 (the getter is defined in this unit, so its class object
// g_unk_717272d7d0 is used directly), takes the damage from the class's float at +0x5c when the flag
// at +0x21 is set (fn_71008c1538 on the request at +0xac0), sets two flags when the class's byte at
// +0x98 is set and the factor at +0xbc8 to one. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::CClass;
using base::reflection::UnkReflObject;

struct UnkRequest;
struct UnkSteamJetArg;

struct UnkCharClassSteamJet {
    u8 pad_0x0[0x5c];
    f32 damage;  // 0x5c
    u8 pad_0x60[0x38];
    bool field_0x98;
};

struct UnkSteamJetComponent {
    u8 pad_0x0[0x10];
    UnkReflObject* field_0x10;
    u8 pad_0x18[0x9];
    bool field_0x21;
    u8 pad_0x22[0xac0 - 0x22];
    u8 request[0x21];  // 0xac0
    bool field_0xae1;
    u8 pad_0xae2[0xbc4 - 0xae2];
    f32 field_0xbc4;
    f32 field_0xbc8;
    u8 pad_0xbcc[0x14];
    bool field_0xbe0;
    u8 pad_0xbe1[0xc78 - 0xbe1];
    UnkCharClassSteamJet* field_0xc78;
};

extern CClass g_unk_717272d7d0 __attribute__((visibility("hidden")));
bool fn_7100a46e00(UnkSteamJetComponent* self, UnkSteamJetArg* arg);
void fn_71008c1538(UnkRequest* request, f32 value);

static inline const CClass* SteamJetClass() {
    fn_710089b048();
    return &g_unk_717272d7d0;
}

bool fn_710089e340(UnkSteamJetComponent* self, UnkSteamJetArg* arg) {
    bool result = fn_7100a46e00(self, arg);
    UnkReflObject* obj = self->field_0x10;
    self->field_0xc78 = obj && fn_710009d50c(obj->vf_0x0(), SteamJetClass())
                            ? reinterpret_cast<UnkCharClassSteamJet*>(obj)
                            : nullptr;
    if (self->field_0x21) {
        self->field_0xbc4 = self->field_0xc78->damage;
        fn_71008c1538(reinterpret_cast<UnkRequest*>(self->request), self->field_0xbc4);
    }
    if (self->field_0xc78->field_0x98) {
        self->field_0xae1 = true;
        self->field_0xbe0 = true;
    }
    self->field_0xbc8 = 1.0f;
    return result;
}
