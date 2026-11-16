#pragma once

// A sub-id of a qualified name (fn_71003327a4's result): the id and a word after it. Its users read a
// blackboard entry under it. Names and layout are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkGameBlackboard.h"
#include "types.h"

struct UnkSubId_7100fc4b34 {
    base::global::CStrId id;
    u64 field_0x8;
};

// The sub-id of `name` within the qualified name (src/game/CAbilityShinespark/fn_71003327a4.cpp defines it
// with its own view of the arguments).
const UnkSubId_7100fc4b34& fn_71003327a4(UnkQualifiedName* self, const base::global::CStrId* name);
