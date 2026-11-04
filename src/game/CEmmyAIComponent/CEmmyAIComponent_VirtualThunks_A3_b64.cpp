// Forwarders that tail-call one virtual slot of the object with their own arguments (the slot's
// register shows how many integer arguments are passed on): CEmmyAIComponent's unit and
// behaviortree::CEmmyPatrolBaseAction's (tools/tu_map.py). Names and argument types are placeholders.
#include "types.h"

struct UnkVirtualThunkObj_A3_b64 {
    void** vtable;
};

template <u32 Slot, class... Args>
static inline __attribute__((always_inline)) void CallSlot_A3_b64(UnkVirtualThunkObj_A3_b64* self, Args... args) {
    typedef void (*Fn)(UnkVirtualThunkObj_A3_b64*, Args...);
    reinterpret_cast<Fn>(self->vtable[Slot / 8])(self, args...);
}

void fn_710051645c(UnkVirtualThunkObj_A3_b64* self, u64 a, u64 b) { CallSlot_A3_b64<0xb8>(self, a, b); }
void fn_7100569fec(UnkVirtualThunkObj_A3_b64* self, u64 a, u64 b, u64 c, u64 d, u64 e, u64 f, u64 g) {
    CallSlot_A3_b64<0x568>(self, a, b, c, d, e, f, g);
}
void fn_7100569ff8(UnkVirtualThunkObj_A3_b64* self, u64 a, u64 b, u64 c, u64 d, u64 e, u64 f) {
    CallSlot_A3_b64<0x570>(self, a, b, c, d, e, f);
}
void fn_710056a004(UnkVirtualThunkObj_A3_b64* self, u64 a, u64 b) { CallSlot_A3_b64<0x578>(self, a, b); }

// Returns what slot 0x10 of the object at +0x8 returns: a value returned through x8 (larger than
// 16 bytes; its size is a placeholder).
struct UnkVirtualThunkResult_A3_b64 {
    u64 words[3];
};

struct UnkVirtualThunkInner_A3_b64 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual UnkVirtualThunkResult_A3_b64 vf_0x10();
};

struct UnkVirtualThunkHolder_A3_b64 {
    void* vtable;
    UnkVirtualThunkInner_A3_b64* field_0x8;
};

UnkVirtualThunkResult_A3_b64 fn_7100560b88(UnkVirtualThunkHolder_A3_b64* self) { return self->field_0x8->vf_0x10(); }
