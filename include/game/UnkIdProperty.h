#pragma once

// A reflected property that holds a string id (vtables 0x7101aa52d0, 0x7101a8d830, 0x7101a88970 and
// 0x7101ca9ea0, one per property type; CBreakableTileManager's "PB_AND_SM" and "VIGNETTE" use the
// first). Destroying one releases the id at 0x170, then the base property (vtable 0x7101a889c8)
// destroys its value holder at 0x10: a member at 0xe0, then a name. Names are placeholders.
#include "base/global/CRntString.h"
#include "base/global/UnkTwoStepStrId.h"
#include "game/CNavMeshItemComponent.h"
#include "types.h"

// The value holder's name, cleaned up out of line (fn_710007ed40) before it is destroyed.
struct UnkIdPropName {
    base::global::CRntString name;
    u64 unk_0x30[0x14];

    ~UnkIdPropName() { CRntString_Clear(&name); }
};

struct UnkIdPropMember {
    u64 unk_0x0[0x12];

    ~UnkIdPropMember() { fn_71009c172c(this); }
};

struct UnkIdPropHolder : UnkIdPropName {
    UnkIdPropMember field_0xd0;
};

struct UnkIdPropBase {
    void** vtable;
    u64 field_0x8;
    UnkIdPropHolder holder;

    ~UnkIdPropBase() { *reinterpret_cast<void***>(this) = g_unk_7101a889c8 + 2; }
};

template <void** Vtable>
struct UnkIdPropertyT : UnkIdPropBase {
    UnkStrIdTwoStep id;

    ~UnkIdPropertyT() { *reinterpret_cast<void***>(this) = Vtable + 2; }
};
static_assert(sizeof(UnkIdPropertyT<g_unk_7101a88970>) == 0x178, "UnkIdPropertyT layout");

// The destructor of CNavMeshItemComponent's "ColliderOffset" and "InitialStage" properties.
void fn_71009bd8c0(UnkIdPropertyT<g_unk_7101a88970>* self);
