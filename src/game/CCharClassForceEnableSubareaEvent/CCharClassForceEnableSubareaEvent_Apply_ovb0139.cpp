// CCharClassForceEnableSubareaEvent's unit (tools/tu_map.py): applies the event. Unless its setup id (+0x10) is empty it builds a sub-area mask (all nine bits when
// the flag at +0x18 is set, otherwise the individual flags +0x19..+0x21), hands it to the sub-area manager g_unk_7101d47360 with the
// setup id and enabling, then nudges the manager g_unk_7101d47368. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection.h"
#include "types.h"

using base::global::CStrId;

struct UnkManager;
extern UnkManager* g_unk_7101d47360;
extern void* g_unk_7101d47368;
void fn_7100ee1f34(void*);
void fn_7100eeeda4(void* manager, const CStrId* setup, bool enable, const u32* mask);

struct UnkSubareaStaticZero_ovb0139 {
    u32 value;
    UnkSubareaStaticZero_ovb0139() : value(0) {}
};

inline const UnkSubareaStaticZero_ovb0139& UnkSubareaStaticZero() {
    static UnkSubareaStaticZero_ovb0139 s_value asm("g_unk_7172719980");
    return s_value;
}

inline u32 UnkSubareaAllMask() {
    static u32 s_mask asm("g_unk_7172719970") = ~UnkSubareaStaticZero().value & 0x1ff;
    return s_mask;
}

struct UnkSubareaEvent_ovb0139 {
    void** vtable;
    u8 pad_0x8[0x8];
    CStrId setup;      // 0x10
    bool all;          // 0x18
    bool bits[9];      // 0x19
};

void fn_7100baedc0(UnkSubareaEvent_ovb0139* self) {
    if (self->setup.data == &g_unk_7101d47108->empty)
        return;
    alignas(8) u32 mask = 0;
    if (self->all) {
        mask |= UnkSubareaAllMask();
    } else {
        for (int i = 0; i < 9; i++)
            if (self->bits[i])
                mask |= 1u << i;
    }
    void* manager = g_unk_7101d47360;
    if (!manager) {
        manager = fn_71000839a0(0xae8, 0x3f);
        fn_7100ee1f34(manager);
        g_unk_7101d47360 = static_cast<UnkManager*>(manager);
    }
    fn_7100eeeda4(manager, &self->setup, true, &mask);
    void* other = g_unk_7101d47368;
    if (!other) {
        other = fn_71000839a0(0x4c8, 0x3f);
        fn_7100d8ced8(other);
        g_unk_7101d47368 = other;
    }
    fn_7100d8ef04(other, 0.033333302f);
}
