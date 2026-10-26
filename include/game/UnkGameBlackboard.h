#pragma once

// A blackboard property definition's qualified id, as the hand-written sources see it. The
// definitions are the polymorphic property objects the per-unit static initialisers build
// (UnkPropertyEx<> in game/UnkTUProperty.h: "ITEM_CENTRAL_UNIT_ENERGY",
// "QUARENTINE_OPENED", ...); each holds at +0x10 an id that is joined on first use from a
// prefix, the definition's group id (vtable slot 0x20) and its own id (slot 0x28), with the
// definition itself after it. tools/tu_map.py puts the builder fn_7100ae9a10 in the
// CGameBlackboard unit (1647). Only what the callers share is spelled out; the names and the
// layout are placeholders.

#include "base/global/CStrId.h"

// The object a qualified id names: its group id (vf_0x20) and its own id (vf_0x28).
struct UnkIdSource {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual const base::global::CStrId& vf_0x20();
    virtual const base::global::CStrId& vf_0x28();
};

// An id of the form "prefix:group:own" (empty parts and their ':' left out) and the object it
// names. Callers compare `id` with the empty id (g_unk_7101d470e0) to see whether it is built.
struct UnkQualifiedId {
    base::global::CStrId id;
    UnkIdSource* source;
};

// Builds `self` for `source`: joins `prefix`, the source's group id and its own id with ':'
// (skipping empty parts) into a string, interns it as `self->id` and stores the source
// (src/game_hi/fn_7100ae9a10.cpp).
void fn_7100ae9a10(UnkQualifiedId* self, UnkIdSource* source, const base::global::CStrId& prefix);
