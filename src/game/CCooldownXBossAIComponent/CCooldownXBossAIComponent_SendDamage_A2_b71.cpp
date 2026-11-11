// CCooldownXBossAIComponent's unit (tools/tu_map.py unit 754): when the owner (+0x30) has the component at
// +0x1b8, builds a damage request (fn_71008c0c74) of value 500 (fn_71008c1538) with the unit's static string
// id at +0xb8 and hands it to that component (virtual 0x188); always returns true. Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "game/UnkTU1370.h"
#include "types.h"

using base::global::CStrId;

extern CStrId g_unk_71726d8738 __attribute__((visibility("hidden")));

struct UnkRequest;
struct UnkStruct_71008c0c74;
struct UnkBase_71003cc474;
void fn_71008c1538(UnkRequest* request, f32 value);

struct UnkCooldownRequest_A2_b71 {
    u8 raw[0x100];

    UnkCooldownRequest_A2_b71() { fn_71008c0c74(reinterpret_cast<UnkStruct_71008c0c74*>(this)); }
    ~UnkCooldownRequest_A2_b71() { fn_71008c1558(reinterpret_cast<UnkBase_71003cc474*>(this)); }
    CStrId& Id() { return *reinterpret_cast<CStrId*>(raw + 0xb8); }
};

struct UnkCooldownTarget_A2_b71 {
    void** vtable;
    void vf_0x188(UnkCooldownRequest_A2_b71* request) {
        reinterpret_cast<void (*)(UnkCooldownTarget_A2_b71*, UnkCooldownRequest_A2_b71*)>(vtable[0x188 / 8])(this,
                                                                                                    request);
    }
};

struct UnkCooldownOwner_A2_b71 {
    u8 pad_0x0[0x1b8];
    UnkCooldownTarget_A2_b71* field_0x1b8;
};

struct UnkCooldownAI_A2_b71 {
    u8 pad_0x0[0x30];
    UnkCooldownOwner_A2_b71* owner;  // 0x30
};

bool fn_71004d473c(UnkCooldownAI_A2_b71* self) {
    if (self->owner->field_0x1b8) {
        UnkCooldownRequest_A2_b71 request;
        fn_71008c1538(reinterpret_cast<UnkRequest*>(&request), 500.0f);
        request.Id() = g_unk_71726d8738;
        self->owner->field_0x1b8->vf_0x188(&request);
    }
    return true;
}
