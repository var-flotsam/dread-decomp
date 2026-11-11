// animtree::CLayerNode's unit (tools/tu_map.py unit 414): the evaluation (fn_71002e46f8). The base child's
// channels 1 and 2 are copied into the pose, then the layer child's are blended in by its own weight
// (+0x64). Names and layouts are placeholders.
#include "game/UnkTU1141.h"
#include "types.h"

struct UnkStruct_71007142e8;
void fn_71007142e8(UnkStruct_71007142e8* self);
struct UnkWeighted_7100714808;

struct alignas(8) UnkAnimChannel_ovb0125 {
    u8 data[0x70];
    UnkAnimChannel_ovb0125() { fn_71007142e8(reinterpret_cast<UnkStruct_71007142e8*>(this)); }
    UnkWeighted_7100714808* Weighted() { return reinterpret_cast<UnkWeighted_7100714808*>(this); }
};

// The three channels a node evaluates into.
struct UnkAnimPose_ovb0125 {
    UnkAnimChannel_ovb0125 channels[3];
};

struct UnkAnimEvalNode_ovb0125 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void Evaluate(UnkAnimPose_ovb0125* pose, void* arg);  // 0x78
    u8 pad_0x8[0x5c];
    f32 weight;  // 0x64
};

struct UnkAnimChildren_ovb0125 {
    UnkAnimEvalNode_ovb0125* first;
    UnkAnimEvalNode_ovb0125* second;
};

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
