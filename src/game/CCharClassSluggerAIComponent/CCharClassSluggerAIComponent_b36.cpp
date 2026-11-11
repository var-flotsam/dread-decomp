// Functions in the CCharClassSluggerAIComponent unit (tools/tu_map.py). Names and layouts are
// placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "game/CCharClassBehaviorTreeAIComponent.h"
#include "types.h"

using base::global::CStrId;

// Declared as src/game/CCharClassGooplotAIComponent/CCharClassGooplotAIComponent.cpp does.
struct UnkObject_7100416db8;
void fn_71006c0044(void* self);
extern void* g_unk_7101a64f60[];

struct UnkSluggerDef {
    void** vtable;
    u8 pad_0x8[0x508];
    u8 field_0x510[0x130];
    CStrId field_0x640;
    f32 field_0x648;
    f32 field_0x64c;
};

// The constructor: the base constructor fn_7100416db8, the vtable, the member at +0x510
// (fn_71006c0044), a copy of the empty string id at +0x640, -1 in both floats after it, then the
// post-construction virtual call (slot 3).
void fn_71006c5618(UnkSluggerDef* self) {
    fn_7100416db8(reinterpret_cast<UnkObject_7100416db8*>(self));
    self->vtable = g_unk_7101a64f60 + 2;
    fn_71006c0044(self->field_0x510);
    new (&self->field_0x640) CStrId(g_unk_7101d470e0);
    self->field_0x648 = -1.0f;
    self->field_0x64c = -1.0f;
    reinterpret_cast<void (*)(UnkSluggerDef*)>(self->vtable[3])(self);
}

// The same constructor without the post-construction call.
void fn_71006c5314(UnkSluggerDef* self) {
    fn_7100416db8(reinterpret_cast<UnkObject_7100416db8*>(self));
    self->vtable = g_unk_7101a64f60 + 2;
    fn_71006c0044(self->field_0x510);
    new (&self->field_0x640) CStrId(g_unk_7101d470e0);
    self->field_0x648 = -1.0f;
    self->field_0x64c = -1.0f;
}
