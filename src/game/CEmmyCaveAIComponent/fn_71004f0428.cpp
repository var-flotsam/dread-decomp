// Destructor of a component: sets its vtable, destroys a member at +0x2a710 (fn_71004f11f8), frees two
// arrays (+0x2a718, +0x2a710) through the allocator at +0x2a730, releases the string id at +0x2a708 and
// finishes with the base destructor (fn_710053f510).
#include "base/UnkStrIdTable.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CEmmyCaveAIComponent.h"
#include "game/UnkTU811.h"
#include "game/UnkEmmyAIComponent.h"
#include "types.h"

using base::global::CStrId;

struct UnkTailBlock {
    CStrId field_0x0;
    UnkOwnedStrIdTable field_0x8;
};

// The cave E.M.M.I.'s AI component: the base's 0x2a6f8 bytes, then its own block at +0x2a708.
struct UnkEmmyCaveAIComponent_4f0428 : UnkEmmyAIComponent {
    u8 pad_0x2a6f8[0x10];
    UnkTailBlock field_0x2a708;
};

void fn_71004f0428(UnkEmmyCaveAIComponent_4f0428* self) {
    self->vtable = g_unk_7101a43038 + 2;
    self->field_0x2a708.field_0x8.~UnkOwnedStrIdTable();
    self->field_0x2a708.field_0x0.Reset();
    fn_710053f510(self);
}
