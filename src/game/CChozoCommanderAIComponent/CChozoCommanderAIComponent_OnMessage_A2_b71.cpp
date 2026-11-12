// CChozoCommanderAIComponent's unit (tools/tu_map.py unit 665): a message handler. Message 0x12 sends the
// owner's (+0x30, fn_7100d7c990) component at +0x1b8 a damage request (fn_71008c0c74) flagged at +0x21,
// with the tunable's value (+0xac of g_unk_7172716160, fn_71008c1538) and the unit's static string id
// at +0xb8 (virtual 0x188); message 0x2a clears the byte at +0x15d06 and calls fn_71004546e0. Always
// returns true. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/UnkActor.h"
#include "game/UnkTU1370.h"
#include "types.h"

using base::global::CStrId;

extern base::reflection::UnkTunable g_unk_7172716160;
extern CStrId g_unk_71726c9500 __attribute__((visibility("hidden")));

struct UnkRequest;
struct UnkStruct_71008c0c74;
struct UnkBase_71003cc474;

struct UnkCommanderRequest_A2_b71 {
    u8 raw[0x100];

    UnkCommanderRequest_A2_b71() { fn_71008c0c74(reinterpret_cast<UnkStruct_71008c0c74*>(this)); }
    ~UnkCommanderRequest_A2_b71() { fn_71008c1558(reinterpret_cast<UnkBase_71003cc474*>(this)); }
    CStrId& Id() { return *reinterpret_cast<CStrId*>(raw + 0xb8); }
};

struct UnkCommanderTarget_A2_b71 {
    void** vtable;
    void vf_0x188(UnkCommanderRequest_A2_b71* request) {
        reinterpret_cast<void (*)(UnkCommanderTarget_A2_b71*, UnkCommanderRequest_A2_b71*)>(vtable[0x188 / 8])(
            this, request);
    }
};

struct UnkCommanderMessage_A2_b71 {
    u8 pad_0x0[0x8];
    s32 id;  // 0x8
};

struct UnkCommanderAI_A2_b71 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x15d06 - 0x38];
    bool field_0x15d06;
};

void fn_71004546e0(UnkCommanderAI_A2_b71* self);

bool fn_71004550f0(UnkCommanderAI_A2_b71* self, UnkCommanderMessage_A2_b71* message) {
    switch (message->id) {
    case 0x2a:
        self->field_0x15d06 = false;
        fn_71004546e0(self);
        break;
    case 0x12: {
        UnkActor* actor = fn_7100d7c990(self->owner);
        if (actor && actor->field_0x1b8) {
            UnkCommanderRequest_A2_b71 request;
            request.raw[0x21] = 1;
            fn_71008c1538(reinterpret_cast<UnkRequest*>(&request),
                          *reinterpret_cast<const f32*>(reinterpret_cast<const char*>(&g_unk_7172716160) + 0xac));
            request.Id() = g_unk_71726c9500;
            static_cast<UnkCommanderTarget_A2_b71*>(actor->field_0x1b8)->vf_0x188(&request);
        }
        break;
    }
    }
    return true;
}
