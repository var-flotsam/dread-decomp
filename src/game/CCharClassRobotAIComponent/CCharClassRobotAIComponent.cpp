// Two constructors in the CCharClassRobotAIComponent unit (vtable 0x7101a34550) over the AI
// component base constructor fn_7100416db8: an empty path id at +0x510, an empty string id at
// +0x518 and the word at +0xec set to 1; fn_710041b488 (the reflected class's constructor) then
// makes the post-construction virtual call (slot 3). Names and layouts are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CCharClassBehaviorTreeAIComponent.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

// Declared as src/game/CCharClassGooplotAIComponent/CCharClassGooplotAIComponent.cpp does.
struct UnkObject_7100416db8;
extern void* g_unk_7101a34550[];

struct UnkRobotAIDef {
    void** vtable;
    u8 pad_0x8[0xe4];
    u32 field_0xec;
    u8 pad_0xf0[0x420];
    CFilePathStrId field_0x510;
    CStrId field_0x518;
};

static inline __attribute__((always_inline)) void Construct(UnkRobotAIDef* self) {
    fn_7100416db8(reinterpret_cast<UnkObject_7100416db8*>(self));
    self->vtable = g_unk_7101a34550 + 2;
    new (&self->field_0x510) CFilePathStrId("", true);
    new (&self->field_0x518) CStrId("", true);
    self->field_0xec = 1;
}

// Declared as src/clones/clones_01.cpp does.
void fn_710041b118(char* p) { Construct(reinterpret_cast<UnkRobotAIDef*>(p)); }

void fn_710041b488(UnkRobotAIDef* self) {
    Construct(self);
    reinterpret_cast<void (*)(UnkRobotAIDef*)>(self->vtable[3])(self);
}
