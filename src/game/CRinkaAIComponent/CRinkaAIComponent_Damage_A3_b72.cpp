// CRinkaAIComponent unit (tools/tu_map.py, the name from reflection): fn_710065e238 fills a damage request
// after the base attack's (fn_7101105c90): it clears the two flags at +0xd4, sets the amount (fn_71008c1538)
// from a per-state table of values while the state at +0x146b8 is 0-4, and clears the amount when the flag at
// +0x146c8 is set or any central unit (fn_7100772c98's list) has a linked (fn_71003b0c4c, +0x438) record whose
// state at +0x70 is 2. Names and layouts are placeholders.
#include "game/CBossAIComponent.h"
#include "game/UnkTU1176.h"
#include "game/UnkTU2702.h"
#include "types.h"

// Declared as src/game/CKraidAIComponent/CKraidAIComponent_2.cpp does.
struct UnkKraidAI2;
struct UnkKraidRequest;

// Declared as src/game/CGobblerAIComponent/CGobblerAIComponent_Spawn_b60.cpp does.
struct UnkZiplineLinkTarget_b49;
struct UnkZiplineLink_b49;

// Declared as src/game/CCharClassStartCentralUnitProtoEvent/CCharClassStartCentralUnitProtoEvent.cpp does.
struct UnkCentralUnitList;

struct UnkRequest;
void fn_71008c1538(UnkRequest* request, f32 value);

// The unit's five per-state amounts (statics the unit's initialiser fills).
extern f32 g_unk_7172716448 __attribute__((visibility("hidden")));
extern f32 g_unk_717271644c __attribute__((visibility("hidden")));
extern f32 g_unk_7172716450 __attribute__((visibility("hidden")));
extern f32 g_unk_7172716454 __attribute__((visibility("hidden")));
extern f32 g_unk_7172716458 __attribute__((visibility("hidden")));

struct UnkRinkaState_A3_b72 {
    u8 pad_0x0[0x70];
    s32 state;
};

struct UnkRinkaRecord_A3_b72 {
    u8 pad_0x0[0x1b8];
    UnkRinkaState_A3_b72* state;
};

struct UnkRinkaTarget_A3_b72 {
    u8 pad_0x0[0x10];
    UnkRinkaRecord_A3_b72* record;
};

struct UnkRinkaUnit_A3_b72 {
    u8 pad_0x0[0x438];
    u8 link[0x8];
};

struct UnkRinkaEntry_A3_b72 {
    u8 pad_0x0[0x10];
    UnkRinkaUnit_A3_b72* unit;
};

struct UnkRinkaList_A3_b72 {
    UnkRinkaEntry_A3_b72** data;
    u32 count;
};

struct UnkRinkaRequest_A3_b72 {
    u8 pad_0x0[0xd4];
    u8 field_0xd4;
    u8 field_0xd5;
};

struct UnkRinkaAI_A3_b72 {
    u8 pad_0x0[0x146b8];
    u32 state;  // 0x146b8
    u8 pad_0x146bc[0xc];
    bool field_0x146c8;
};

void fn_710065e238(UnkRinkaAI_A3_b72* self, void* arg, UnkRinkaRequest_A3_b72* request) {
    fn_7101105c90(reinterpret_cast<UnkKraidAI2*>(self), arg, reinterpret_cast<UnkKraidRequest*>(request));
    request->field_0xd4 = 0;
    request->field_0xd5 = 0;
    const f32* amount;
    s32 state = self->state;
#if defined(__aarch64__)
    asm("" : "+r"(state));  // the original sign-extends the state at the table load
#endif
    switch (state) {
    case 0: amount = &g_unk_7172716448; goto set;
    case 1: amount = &g_unk_717271644c; goto set;
    case 2: amount = &g_unk_7172716450; goto set;
    case 3: amount = &g_unk_7172716454; goto set;
    case 4: amount = &g_unk_7172716458; goto set;
    default: break;
    set:
        fn_71008c1538(reinterpret_cast<UnkRequest*>(request), *amount);
    }
    if (!self->field_0x146c8) {
        UnkRinkaList_A3_b72* list = reinterpret_cast<UnkRinkaList_A3_b72*>(fn_7100772c98());
        for (UnkRinkaEntry_A3_b72** it = list->data, **end = list->data + list->count;; ++it) {
            if (it == end)
                return;
            UnkRinkaUnit_A3_b72* unit = *it ? (*it)->unit : nullptr;
            if (!unit)
                continue;
            UnkRinkaTarget_A3_b72* target = *reinterpret_cast<UnkRinkaTarget_A3_b72**>(
                fn_71003b0c4c(reinterpret_cast<UnkZiplineLink_b49*>(unit->link)));
            if (target && target->record && target->record->state && target->record->state->state == 2)
                break;
        }
    }
    fn_71008c1538(reinterpret_cast<UnkRequest*>(request), 0.0f);
}
