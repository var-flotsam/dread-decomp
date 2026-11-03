// CCharClassQuetzoaXAIComponent: a check that defers to the base character class when the flag at
// +0x50c is set; otherwise answers 3 unless bit 15 of the info's flags at +0xb4 is set, and then 2
// unless the actor's AI component is a CQuetzoaXAIComponent with its flag at +0x14bc8 clear.
// Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "types.h"

struct UnkQuetzoaXAI {
    void** vtable;
    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkQuetzoaXAI*)>(vtable[0])(this);
    }
};

struct UnkQuetzoaXCharClass {
    u8 pad_0x0[0x50c];
    bool field_0x50c;
};

struct UnkQuetzoaXInfo {
    u8 pad_0x0[0xb4];
    u32 flags;  // 0xb4
};

// The base character class's check, called with the same arguments (declared without them in its
// generated definition).
u32 fn_7100414168();

u32 fn_710063ee0c(UnkQuetzoaXCharClass* self, UnkActor* actor, void* arg, UnkQuetzoaXInfo* info) {
    if (self->field_0x50c)
        return fn_7100414168();
    if (!(info->flags & 0x8000))
        return 3;
    UnkQuetzoaXAI* ai = static_cast<UnkQuetzoaXAI*>(actor->field_0x240);
    if (ai && fn_710009d50c(ai->vf_0x0(), fn_710063efe4()) && !reinterpret_cast<char*>(ai)[0x14bc8])
        return 3;
    return 2;
}
