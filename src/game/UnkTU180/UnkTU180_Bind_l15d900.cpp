// Unit 180's device layer (tools/tu_map.py unit 180): binds a shader program on the command buffer
// (or unbinds every stage when there is none) and, when asked, binds the uniform blocks of its
// stages for the current frame's buffer. Names and layouts are placeholders.
#include "game/UnkTU172.h"
#include "game/UnkTU180.h"
#include "types.h"

struct UnkGfxUniformBlock_l15d900 {
    u8 pad_0x0[0x30];
    s32 binding;  // 0x30, -1 when unbound
    u64 size;  // 0x38
    u8 pad_0x40[0xb0 - 0x40];
    u64 addresses[5];  // 0xb0, one per frame buffer
};
static_assert(sizeof(UnkGfxUniformBlock_l15d900) == 0xd8, "layout");

struct UnkGfxShader_l15d900 {
    u8 pad_0x0[0x4c];
    u32 block_count;  // 0x4c
    u8 pad_0x50[0xb0 - 0x50];
    UnkGfxUniformBlock_l15d900* blocks;  // 0xb0
};

// The program record as this function reads it (UnkGfxProgram in include/game/UnkTU180.h).
struct UnkGfxProgram_l15d900 {
    UnkGfxShader_l15d900* stages[3];  // 0x0
    u32 stage_bits;  // 0x18
    u8 pad_0x1c[4];
    u8 program[0x130];  // 0x20
};

extern bool g_unk_7101a1d810;  // the bound program's first flag
extern bool g_unk_7101a1d811;  // set when the program binding changed
extern void (*g_unk_716e29b810)(void* cmd, void* program, u32 stages);
extern void (*g_unk_716e29b828)(void* cmd, u32 stage, s32 binding, u64 address, u64 size);

// Through wrappers: the arguments are evaluated before the entry point is loaded.
static inline void BindProgram(void* cmd, void* program, u32 stages) {
    g_unk_716e29b810(cmd, program, stages);
}

static inline void BindUniformBuffer(void* cmd, u32 stage, s32 binding, u64 address, u64 size) {
    g_unk_716e29b828(cmd, stage, binding, address, size);
}

static inline void BindUniformBlocks(UnkGfxProgram_l15d900* program, u32 stage) {
    for (u64 i = 0; i < program->stages[stage]->block_count; ++i) {
        UnkGfxUniformBlock_l15d900* block = &program->stages[stage]->blocks[i];
        if (block->binding == -1)
            continue;
        BindUniformBuffer(fn_7100153d50(), stage, block->binding,
                          block->addresses[fn_7100153da0()], block->size);
    }
}

void fn_710015d900(void* program_record, bool second, bool uniforms) {
    UnkGfxProgram* bound = static_cast<UnkGfxProgram*>(program_record);
    if (g_unk_716e2a3448 == bound && g_unk_7101a1d810 == second)
        return;
    g_unk_716e2a3448 = bound;
    g_unk_7101a1d811 = true;
    g_unk_7101a1d810 = second;
    auto* program = static_cast<UnkGfxProgram_l15d900*>(program_record);
    void* cmd = fn_7100153d50();
    if (!program) {
        BindProgram(cmd, nullptr, 7);
        return;
    }
    BindProgram(cmd, program->program, program->stage_bits);
    if (!g_unk_7101a1d810)
        BindProgram(fn_7100153d50(), nullptr, 2);
    if (!uniforms)
        return;
    BindUniformBlocks(program, 0);
    if (second)
        BindUniformBlocks(program, 1);
    if (program->stages[2])
        BindUniformBlocks(program, 2);
}
