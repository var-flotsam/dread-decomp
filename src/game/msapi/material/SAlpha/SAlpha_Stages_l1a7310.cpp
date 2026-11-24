// msapi::material::SAlpha's unit (tools/tu_map.py unit 229): reads a pass's texture stages from a
// file (a count, then that many 0x88-byte stages, each added default-built and read in place) and
// keeps the count in the material. Names and layouts are placeholders.
#include "base/global/CRntFile.h"
#include "base/global/CRntVector.h"
#include "base/UnkNoArg.h"
#include "game/msapi/material/SAlpha.h"
#include "types.h"

using base::global::CRntFile;

struct UnkMaterial;
struct UnkMatStageVector_ovb0098;
struct UnkMaterialDef_l1a5aa4;

// As src/game/msapi/material/SAlpha.cpp and the files under SAlpha/ declare and define them.
void SAlpha_ReserveStageArray(UnkMatStageVector_ovb0098* v, u32 n);  // Reserve
void fn_71001a5aa4(UnkMaterialDef_l1a5aa4* self);        // A stage's constructor

struct UnkStage_l1a7310 {
    u8 data[0x88];
};

struct UnkStages_l1a7310 {
    UnkStage_l1a7310* data;
    u32 count;
};

// The material as this writes it: the stage count kept at 0x3a4.
struct UnkMaterialView_l1a7310 {
    u8 pad_0x0[0x3a4];
    u16 stage_count;
};

static inline void Reserve(UnkStages_l1a7310* stages, u32 n) {
    SAlpha_ReserveStageArray(reinterpret_cast<UnkMatStageVector_ovb0098*>(stages), n);
}

// Adds a default stage and returns it.
static inline UnkStage_l1a7310& AddStage(UnkStages_l1a7310* stages) {
    u32 n = stages->count + 1;
    Reserve(stages, n);
    for (u32 i = stages->count; i < n; ++i)
        fn_71001a5aa4(reinterpret_cast<UnkMaterialDef_l1a5aa4*>(&stages->data[i]));
    stages->count = n;
    UnkStage_l1a7310* end = &stages->data[n];
    return n ? end[-1] : *end;
}

// NON_MATCHING(minor): the block reader's first argument, which it does not read, is null here; the
// original leaves x0 as the previous call returned it.
void fn_71001a7310(UnkMaterial* self, CRntFile* file, base::global::CRntVector<u64>* list) {
    auto* stages = reinterpret_cast<UnkStages_l1a7310*>(list);
    s32 count = 0;
    CRntFile_Read(file, &count, sizeof(count), 1);
    Reserve(stages, count);
    for (s32 i = 0; i < count; ++i)
        SAlpha_ReadReflectedFieldsFromFile(UnkNoArg(), file, &AddStage(stages));
    reinterpret_cast<UnkMaterialView_l1a7310*>(self)->stage_count = count;
}
