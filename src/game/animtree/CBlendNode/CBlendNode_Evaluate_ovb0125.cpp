// animtree::CBlendNode's unit (tools/tu_map.py unit 406): the evaluation (fn_71002e077c). Below full
// weight the first child evaluates into the pose and channels 1 and 2 are reset; with any weight the
// second child's pose is blended into those channels by the node's weight (+0x28, +0x38). Names and
// layouts are placeholders.
#include "game/UnkAnimChannel_ovb0125.h"
#include "game/UnkAnimChildren_ovb0125.h"
#include "game/UnkAnimEvalNode_ovb0125.h"
#include "game/UnkAnimPose_ovb0125.h"
#include "game/UnkTU1141.h"
#include "types.h"

struct UnkStruct_71007142e8;
struct UnkWeighted_7100714808;

struct UnkAnimBlendParams_ovb0125 {
    u8 pad_0x0[0x38];
    f32 weight;
};

struct UnkBlendNode_ovb0125 {
    u8 pad_0x0[0x28];
    UnkAnimBlendParams_ovb0125* params;  // 0x28
    u8 pad_0x30[0x8];
    UnkAnimChildren_ovb0125* children;  // 0x38
};

void fn_71002e077c(UnkBlendNode_ovb0125* self, UnkAnimPose_ovb0125* pose, void* arg) {
    if (self->children->second->weight < 1.0f) {
        UnkAnimPose_ovb0125 rest;
        self->children->first->Evaluate(pose, arg);
        fn_7100714604(pose->channels[2].Weighted(), rest.channels[2].Weighted());
        fn_7100714604(pose->channels[1].Weighted(), rest.channels[1].Weighted());
    }
    if (self->children->second->weight > 0.0f) {
        UnkAnimPose_ovb0125 other;
        self->children->second->Evaluate(&other, arg);
        fn_71007149f4(pose->channels[2].Weighted(), other.channels[2].Weighted(), self->params->weight, false);
        fn_71007149f4(pose->channels[1].Weighted(), other.channels[1].Weighted(), self->params->weight, false);
    }
}
