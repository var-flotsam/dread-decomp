// msapi::material::SAlpha's unit (tools/tu_map.py unit 229): after the uniform locations are found,
// picks for each of ten uniforms of the vertex and fragment stages the four-float value block bound
// to it and copies its values. Names and layouts are placeholders.
#include <string.h>

#include "game/UnkTU194.h"
#include "game/UnkTU230.h"
#include "types.h"

struct UnkMaterial;
struct UnkMaterial_l1a77a4;
struct UnkMaterialValuesVector_l1a77a4;
void fn_71001a77a4(UnkMaterial_l1a77a4* self, s32 stage, UnkMaterialValuesVector_l1a77a4& values);
s32 fn_7100167ce0(u32 value);

// A named value block (0x38 bytes).
struct UnkValueBlock_l1a7410 {
    void* vtable;
    void* name;
    u8 type;  // 0x10, 'f' for floats
    s32 count;  // 0x14
    u64 locations[3];  // 0x18: the uniform location in each stage, 0 for none
    void* values;  // 0x30
};
static_assert(sizeof(UnkValueBlock_l1a7410) == 0x38, "layout");

struct UnkValueBlocks_l1a7410 {
    UnkValueBlock_l1a7410* data;
    u32 count;
    u8 pad_0xc[0x1c];
};

// A pass (0x60 bytes): its stage and its value blocks.
struct UnkPass_l1a7410 {
    void* vtable;
    u32 stage;  // 0x8
    UnkValueBlocks_l1a7410 blocks;  // 0x10
    u8 pad_0x38[0x28];
};
static_assert(sizeof(UnkPass_l1a7410) == 0x60, "layout");

constexpr s32 kAllStages = 0x7fffffff;
constexpr u32 kUniformCount = 10;

struct UnkMaterial_l1a7410 {
    u8 pad_0x0[0x14c];
    f32 stage1_values[kUniformCount][4];  // 0x14c
    u8 pad_0x1ec[4];
    UnkValueBlock_l1a7410* stage1_blocks[kUniformCount];  // 0x1f0
    f32 stage0_values[kUniformCount][4];  // 0x240
    UnkValueBlock_l1a7410* stage0_blocks[kUniformCount];  // 0x2e0
    UnkValueBlocks_l1a7410 blocks;  // 0x330
    UnkPass_l1a7410* passes;  // 0x358
    u32 pass_count;  // 0x360
    u8 pad_0x364[0x2c];
    void* program;  // 0x390
};
static_assert(__builtin_offsetof(UnkMaterial_l1a7410, blocks) == 0x330, "layout");
static_assert(__builtin_offsetof(UnkMaterial_l1a7410, program) == 0x390, "layout");

static inline void Locate(UnkMaterial_l1a7410* self, s32 stage, UnkValueBlocks_l1a7410* blocks) {
    fn_71001a77a4(reinterpret_cast<UnkMaterial_l1a77a4*>(self), stage,
                  *reinterpret_cast<UnkMaterialValuesVector_l1a77a4*>(blocks));
}

// Whether the block is four floats bound at this location.
static inline bool IsBoundVec4(const UnkValueBlock_l1a7410* block, u64 location, u64 wanted) {
    return location == wanted && block->count == 4 && block->type == 'f';
}

void fn_71001a7410(UnkMaterial* material) {
    UnkMaterial_l1a7410* self = reinterpret_cast<UnkMaterial_l1a7410*>(material);
    for (u32 i = 0; i < kUniformCount; ++i) {
        self->stage1_blocks[i] = nullptr;
        self->stage0_blocks[i] = nullptr;
    }
    Locate(self, kAllStages, &self->blocks);
    for (UnkPass_l1a7410* pass = self->passes; pass != self->passes + self->pass_count; ++pass) {
        UnkValueBlocks_l1a7410& blocks = pass->blocks;
        Locate(self, pass->stage, &blocks);
        // The fragment stage's uniforms.
        if (self->program && pass->stage == 1) {
            for (UnkValueBlock_l1a7410* block = blocks.data; block != blocks.data + blocks.count; ++block) {
                for (u32 i = 0; i < kUniformCount; ++i) {
                    u64 wanted = block->locations[1];
                    if (!wanted)
                        continue;
                    u64 location = 0;
                    fn_71001aa0b4(self->program, 1, fn_7100167cc0(i), &location);
                    if (IsBoundVec4(block, location, wanted)) {
                        self->stage1_blocks[i] = block;
                        memcpy(self->stage1_values[i], block->values, block->count * sizeof(f32));
                    }
                }
            }
        }
        // The vertex stage's uniforms.
        if (self->program && !pass->stage) {
            for (UnkValueBlock_l1a7410* block = blocks.data; block != blocks.data + blocks.count; ++block) {
                for (u32 i = 0; i < kUniformCount; ++i) {
                    u64 wanted = block->locations[0];
                    if (!wanted)
                        continue;
                    u64 location = 0;
                    fn_71001aa0b4(self->program, 0, fn_7100167ce0(i), &location);
                    if (IsBoundVec4(block, location, wanted)) {
                        self->stage0_blocks[i] = block;
                        memcpy(self->stage0_values[i], block->values, block->count * sizeof(f32));
                    }
                }
            }
        }
    }
}
