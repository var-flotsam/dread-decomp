#pragma once

// A component as the functions of the CComponent unit (tools/tu_map.py) see it: fn_71010c4194 is
// a destructor (it stores the vtable, then runs the base destructor fn_7101043934) and
// fn_71010c4250 reads a property of the component. The names and the layout are placeholders.

#include "types.h"

struct UnkComponent;
struct UnkComponentProp;
void fn_71010c4194(UnkComponent* self);
u32 fn_71010c4250(UnkComponent* self, UnkComponentProp* prop);
