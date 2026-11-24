#pragma once

#include "types.h"

namespace base {
namespace reflection {
class CClass;
}  // namespace reflection
}  // namespace base

// An ability of the player's ability set (fn_71003013ec finds one by id): its first virtual returns its
// reflected class, which the callers compare with an ability class getter. Name and layout are
// placeholders.
struct UnkAbility {
    virtual const base::reflection::CClass* vf_0x0();
};
