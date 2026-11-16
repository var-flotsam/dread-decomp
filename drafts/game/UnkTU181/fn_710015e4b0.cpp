// Unit 181 of tools/tu_map.py: copies `count` 16-byte rows of each of `arrayCount` values (up to the
// uniform's size times `arrayCount`) into the current frame's buffer of the uniform at `location` of
// the stage's program (0 to 2 select it in the state), and marks it dirty; as unit 180's
// fn_710015e1f0 (src/engine/UnkGfxObjectPools.cpp) does for floats. The last argument is not read.
// Names and layouts are placeholders.
#include <string.h>

#include "game/UnkTU172.h"
#include "game/UnkTU180.h"
#include "game/UnkTU181.h"
#include "types.h"

// As src/engine/UnkGfxObjectPools.cpp defines them.
// A uniform block: one buffer per frame (from +0x40), the size checked at +0x38, a dirty flag.
struct UnkGfxUniformBlock {
    u8 pad_0x0[0x38];
    void* field_0x38;
    u8* buffers[0x12];  // 0x40
    bool dirty;         // 0xd0
    u8 pad_0xd1[0x7];
};
static_assert(sizeof(UnkGfxUniformBlock) == 0xd8, "UnkGfxUniformBlock layout");

// A uniform: its offset (+0x30) and size (+0x34) in its block, and the block's index (+0x38).
struct UnkGfxUniform {
    u8 pad_0x0[0x30];
    u32 offset;
    u32 size;
    u32 block;
    u8 pad_0x3c[0x4];
};

struct UnkGfxStageProgram {
    u8 pad_0x0[0xa8];
    UnkGfxUniform* uniforms;      // 0xa8, by location - 15
    UnkGfxUniformBlock* blocks;   // 0xb0
    bool dirty;                   // 0xb8
};

// The programs of the three stages.
struct UnkGfxStagePrograms {
    UnkGfxStageProgram* programs[3];
};

void fn_710015e4b0(void* state, u32 stage, u64 location, u32 count, u32 a, const UnkMatrix44* values, u32 b) {
    UnkGfxStagePrograms* stages = static_cast<UnkGfxStagePrograms*>(state);
    if (!location || !stages)
        return;
    u32 index = static_cast<u32>(location - 15);
    UnkGfxStageProgram* program;
    switch (stage) {
    case 0:
        program = stages->programs[0];
        break;
    case 1:
        program = stages->programs[1];
        break;
    case 2:
        program = stages->programs[2];
        break;
    default:
        program = nullptr;
        break;
    }
    UnkGfxUniform* uniform = &program->uniforms[index];
    UnkGfxUniformBlock* blocks = program->blocks;
    u32 blockIndex = uniform->block;
    if (blocks[blockIndex].field_0x38) {
        u32 frame = fn_7100153da0();
        UnkGfxUniformBlock* block = &blocks[blockIndex];
        u32 bytes = count * a * 16;
        u32 limit = uniform->size * a;
        memcpy(block->buffers[frame] + uniform->offset, values, bytes < limit ? bytes : limit);
        block->dirty = true;
        program->dirty = true;
        g_unk_716e2a3450 = 1;
    }
}
