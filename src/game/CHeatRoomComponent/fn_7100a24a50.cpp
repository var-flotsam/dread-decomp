// Copies of fn_7100e7cdfc (src/game/CBreakableTileManager/CTunableBreakableTileManager/fn_7100ca3be4.cpp) for other types, with their own callees and data (they name their own callees, data and strings; the comments describe the original): fn_7100a24a50, fn_71009bd8c0.
// The destructors of reflected property types (vtable 0x7101aa52d0, used for
// CBreakableTileManager's "PB_AND_SM" and "VIGNETTE" properties, and 0x7101ca9ea0): the string
// id at 0x170 is released, then the base property (vtable 0x7101a889c8) destroys its value
// holder at 0x10 (a member at 0xe0, then a name).
#include <new>

#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "game/CBTObserverComponent.h"
#include "game/CHeatRoomComponent.h"
#include "game/CNavMeshItemComponent.h"
#include "game/UnkIdProperty.h"
#include "types.h"

// The same destructor for two other property types (vtables 0x7101a8d830 and 0x7101a88970).
using UnkIdProperty2 = UnkIdPropertyT<g_unk_7101a8d830>;

void fn_7100a24a50(UnkIdProperty2* self) { self->~UnkIdPropertyT(); }

void fn_71009bd8c0(UnkIdPropertyT<g_unk_7101a88970>* self) { self->~UnkIdPropertyT(); }
