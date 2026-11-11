// CObsydomithonAIComponent unit (tools/tu_map.py, reflection hint). fn_710062e80c fills a damage
// request after the base attack's (fn_7101105c90) and clears the request's flag at +0x2a when its
// source (+0x98) carries the id the unit keeps in g_unk_71726f9440 (at the source's +0x20). Names and
// layouts are placeholders.
#include "game/UnkTU2702.h"
#include "types.h"

// Declared as src/game/CKraidAIComponent/CKraidAIComponent_2.cpp does.
struct UnkKraidAI2;
struct UnkKraidRequest;

extern __attribute__((visibility("hidden"))) u64 g_unk_71726f9440;

struct UnkSource_obs_b43a2 {
    u8 pad_0x0[0x20];
    u64 id;  // 0x20
};

struct UnkRequest_obs_b43a2 {
    u8 pad_0x0[0x2a];
    bool field_0x2a;
    u8 pad_0x2b[0x6d];
    UnkSource_obs_b43a2* source;  // 0x98
};

void fn_710062e80c(UnkKraidAI2* self, void* arg, UnkKraidRequest* request) {
    fn_7101105c90(self, arg, request);
    UnkRequest_obs_b43a2* r = reinterpret_cast<UnkRequest_obs_b43a2*>(request);
    if (UnkSource_obs_b43a2* source = r->source) {
        if (__builtin_expect(source->id == g_unk_71726f9440, 0))
            r->field_0x2a = false;
    }
}
