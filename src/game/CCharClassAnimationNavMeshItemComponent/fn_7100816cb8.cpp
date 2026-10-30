// A constructor in the CCharClassAnimationNavMeshItemComponent unit, as the behaviour tree nodes' (for
// example src/game/behaviortree/CSetStickySideWalkAnim/fn_7100372124.cpp): the base constructor
// fn_71011acd10, the vtable, the flag at +0x58 set, an empty string id at +0x60, then the
// post-construction virtual call (slot 3). Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStrId;

struct UnkAnimationNavMeshItemDef {
    void** vtable;
    u8 pad_0x8[0x50];
    bool field_0x58;
    u8 pad_0x59[0x7];
    UnkStrId field_0x60;
};

// The base constructor (declared as src/game/CCharClassLifeComponent/fn_71008bca30.cpp does).
struct UnkStruct_71008bca30;
void fn_71011acd10(UnkStruct_71008bca30* self);
extern void* g_unk_7101a751e0[];

void fn_7100816cb8(UnkAnimationNavMeshItemDef* self) {
    fn_71011acd10(reinterpret_cast<UnkStruct_71008bca30*>(self));
    self->vtable = &g_unk_7101a751e0[2];
    self->field_0x58 = true;
    new (&self->field_0x60) UnkStrId("", true);
    reinterpret_cast<void (*)(UnkAnimationNavMeshItemDef*)>(self->vtable[3])(self);
}
