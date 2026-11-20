// The destructors of reflected property types (vtable 0x7101aa52d0, used for
// CBreakableTileManager's "PB_AND_SM" and "VIGNETTE" properties, and 0x7101ca9ea0): the string
// id at 0x170 is released, then the base property (vtable 0x7101a889c8) destroys its value
// holder at 0x10 (a member at 0xe0, then a name).
#include <new>

#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "game/CBTObserverComponent.h"
#include "game/CBreakableTileManager/CTunableBreakableTileManager.h"
#include "game/CNavMeshItemComponent.h"
#include "game/UnkIdProperty.h"
#include "types.h"

extern void* g_unk_7101ca9ea0[];

using UnkIdProperty = UnkIdPropertyT<g_unk_7101aa52d0>;

void fn_7100ca3be4(UnkIdProperty* self) { self->~UnkIdPropertyT(); }
void fn_7100ca7c00(UnkIdProperty* self) {
    self->~UnkIdPropertyT();
    operator delete(self);
}

// The same destructor for another property type (vtable 0x7101ca9ea0).
using UnkIdProperty2 = UnkIdPropertyT<g_unk_7101ca9ea0>;

void fn_7100e7cdfc(UnkIdProperty2* self) { self->~UnkIdPropertyT(); }
void fn_7100ea64e0(UnkIdProperty2* self) {
    self->~UnkIdPropertyT();
    operator delete(self);
}
