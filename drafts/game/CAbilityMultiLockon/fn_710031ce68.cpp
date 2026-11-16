// CAbilityMultiLockon's unit (tools/tu_map.py, unit 431): activates the target of the first of the
// five aim slots whose target is not active yet (flag at +0x1b4) and returns it, or null.
// Names and layouts are placeholders.
#include "game/CScenario.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU341.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

class UnkActor_7100233910;
void fn_7100233910(UnkActor_7100233910* self, bool value);
void fn_7100a2bc3c(void*);

// A slot's target as the activation sees it.
struct UnkLockonTarget_l31ce68 {
    u8 pad_0x0[0x1a8];
    void* field_0x1a8;
    u8 pad_0x1b0[0x4];
    bool active;  // 0x1b4
    u8 pad_0x1b5[0x133];
    void* field_0x2e8;
};
static_assert(__builtin_offsetof(UnkLockonTarget_l31ce68, field_0x2e8) == 0x2e8, "UnkLockonTarget_l31ce68 layout");

struct UnkMultiLockon_l31ce68 {
    u8 pad_0x0[0x3a0];
    UnkWeakHandle* slots[5];  // 0x3a0
};

// A counted reference to a slot's handle (the slots are always filled).
struct UnkSlotRef_l31ce68 {
    UnkWeakHandle* p;
    UnkSlotRef_l31ce68(UnkWeakHandle* h) : p(h) { ++p->ref_count; }
    ~UnkSlotRef_l31ce68() {
        if (--p->ref_count == 0)
            p->vf_0x10();
    }
    UnkLockonTarget_l31ce68* operator->() const { return reinterpret_cast<UnkLockonTarget_l31ce68*>(p->target); }
    UnkLockonTarget_l31ce68* Get() const { return reinterpret_cast<UnkLockonTarget_l31ce68*>(p->target); }
};

UnkLockonTarget_l31ce68* fn_710031ce68(UnkMultiLockon_l31ce68* self) {
    for (u32 i = 0; i < 5; ++i) {
        UnkSlotRef_l31ce68 ref(self->slots[i]);
        if (!ref->active) {
            if (!ref->field_0x1a8) {
                if (UnkGameManagerSub_0x608* points = g_unk_7101d47310->field_0x608)
                    fn_71011a2770(points, ref.Get());
            }
            if (ref->field_0x2e8)
                fn_7100a2bc3c(ref->field_0x2e8);
            fn_7100233910(reinterpret_cast<UnkActor_7100233910*>(ref.Get()), true);
            fn_71002334fc(reinterpret_cast<UnkStruct_71002334fc*>(ref.Get()), true);
            return ref.Get();
        }
    }
    return nullptr;
}
