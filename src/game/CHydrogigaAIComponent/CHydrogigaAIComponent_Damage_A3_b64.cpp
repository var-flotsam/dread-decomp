// CHydrogigaAIComponent's unit (tools/tu_map.py): fills a damage request after the base attack's
// (fn_7101105c90), then marks it (+0xc8 = 1) when the name the owner's collision container (+0x2a0)
// gives the request's source (+0x98), or the source's own id (+0x20), is one of the unit's five ids at
// g_unk_71726eb430 (the first against ids 1 and 0, the second against ids 4, 2 and 3). Names and
// layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// Declared as src/game/CKraidAIComponent/CKraidAIComponent_2.cpp does.
struct UnkKraidAI2;
struct UnkKraidRequest;
void fn_7101105c90(UnkKraidAI2* self, void* arg, UnkKraidRequest* request);

struct UnkCollisionContainer;
CStrId* fn_7100864018(UnkCollisionContainer* self, void* object);

extern CStrId g_unk_71726eb430[5] __attribute__((visibility("hidden")));

struct UnkHydrogigaSource_A3_b64 {
    u8 pad_0x0[0x20];
    CStrId id;
};

struct UnkHydrogigaRequest_A3_b64 {
    u8 pad_0x0[0x98];
    UnkHydrogigaSource_A3_b64* source;
    u8 pad_0xa0[0xc8 - 0xa0];
    u32 field_0xc8;
};

struct UnkHydrogigaOwner2_A3_b64 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* collision;
};

struct UnkHydrogigaAI2_A3_b64 {
    u8 pad_0x0[0x30];
    UnkHydrogigaOwner2_A3_b64* owner;
};

void fn_71005c05b0(UnkHydrogigaAI2_A3_b64* self, void* arg, UnkHydrogigaRequest_A3_b64* request) {
    fn_7101105c90(reinterpret_cast<UnkKraidAI2*>(self), arg, reinterpret_cast<UnkKraidRequest*>(request));
    UnkCollisionContainer* collision = self->owner->collision;
    CStrId name(collision ? *fn_7100864018(collision, request->source) : g_unk_7101d470e0);
    CStrId id(request->source ? request->source->id : g_unk_7101d470e0);
    CStrId* ids = g_unk_71726eb430;
    asm("" : "+r"(ids));
    if (name.data == ids[1].data || id.data == ids[4].data || id.data == ids[2].data || id.data == ids[3].data ||
        name.data == ids[0].data)
        request->field_0xc8 = 1;
}
