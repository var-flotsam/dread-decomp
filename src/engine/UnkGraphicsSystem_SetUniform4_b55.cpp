// Unit 259 (tools/tu_map.py; the graphics system of src/engine/UnkGraphicsSystem.cpp): set a
// four-float uniform of the current program, its id mapped from an index by fn_7100167cc0 or
// fn_7100167ce0, when the program's stage (1 or 0) has it.
#include "game/UnkTU180.h"
#include "game/UnkTU194.h"
#include "game/UnkTU230.h"
#include "types.h"

s32 fn_7100167ce0(u32 value);

struct UnkGfxUniformTarget {
    u8 pad_0x0[0x317af68];
    void* field_0x317af68;  // the program state
    void* field_0x317af70;  // the program
};

struct UnkGfxVec4 {
    f32 v[4];
};

template <auto MapId, u32 kStage>
static __attribute__((always_inline)) void SetUniform4(UnkGfxUniformTarget* self, u32 index, const UnkGfxVec4* value) {
    void* program = self->field_0x317af70;
    u64 location;
    if (fn_71001aa0b4(program, kStage, MapId(index), &location)) {
        const f32 values[4] = {value->v[0], value->v[1], value->v[2], value->v[3]};
        fn_710015e1f0(self->field_0x317af68, kStage, location, 4, values);
    }
}

void fn_71001ccd38(UnkGfxUniformTarget* self, u32 index, const UnkGfxVec4* value) {
    SetUniform4<fn_7100167cc0, 1>(self, index, value);
}

void fn_71001d0900(UnkGfxUniformTarget* self, u32 index, const UnkGfxVec4* value) {
    SetUniform4<fn_7100167ce0, 0>(self, index, value);
}
