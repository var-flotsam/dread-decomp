// animtree::CLayerNode's unit (tools/tu_map.py unit 414): the evaluation (fn_71002e46f8). The base child's
// channels 1 and 2 are copied into the pose, then the layer child's are blended in by its own weight
// (+0x64). Names and layouts are placeholders.
#include "game/UnkAnimChannel_ovb0125.h"
#include "game/UnkAnimChildren_ovb0125.h"
#include "game/UnkAnimEvalNode_ovb0125.h"
#include "game/UnkAnimPose_ovb0125.h"
#include "game/UnkTU1141.h"
#include "types.h"

struct UnkStruct_71007142e8;
struct UnkWeighted_7100714808;

struct UnkLayerNode_ovb0125 {
    u8 pad_0x0[0x38];
    UnkAnimChildren_ovb0125* children;  // 0x38
};

void fn_71002e46f8(UnkLayerNode_ovb0125* self, UnkAnimPose_ovb0125* pose, void* arg) {
    UnkAnimPose_ovb0125 base;
    self->children->first->Evaluate(&base, arg);
    fn_7100714604(pose->channels[2].Weighted(), base.channels[2].Weighted());
    fn_7100714604(pose->channels[1].Weighted(), base.channels[1].Weighted());
    if (self->children->second->weight > 0.0f) {
        UnkAnimPose_ovb0125 layer;
        self->children->second->Evaluate(&layer, arg);
        fn_71007149f4(pose->channels[2].Weighted(), layer.channels[2].Weighted(), self->children->second->weight, false);
        fn_71007149f4(pose->channels[1].Weighted(), layer.channels[1].Weighted(), self->children->second->weight, false);
    }
}
