#pragma once

// A resource type's description (class name, file extension, accepted source extensions), built on
// the stack by a static initialiser and handed to a registry global (as in fn_7100005700.cpp). The
// size comes from the stack layout; fn_7100004a44 (src/base/animation/CAnimationResource/fn_7100004a44.cpp) destroys one.

#include "types.h"

namespace unk_static_init {

struct UnkResType {
    u8 field_0x0[0x38];
};

}  // namespace unk_static_init

void fn_7100004578(unk_static_init::UnkResType* self, const char* type_name, const char* ext,
                   const char* const* sources, u32 count);  // the description's constructor
void fn_7100004698(void* registry, const char* name, unk_static_init::UnkResType* types, u32 count);
void fn_7100004a44(unk_static_init::UnkResType* self);  // the description's destructor
void fn_7100004b94(void* registry);                     // the registry's destructor
