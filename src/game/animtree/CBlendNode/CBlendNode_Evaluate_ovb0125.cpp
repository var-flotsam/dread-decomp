// animtree::CBlendNode's unit (tools/tu_map.py unit 406): the evaluation (fn_71002e077c). Below full
// weight the first child evaluates into the pose and channels 1 and 2 are reset; with any weight the
// second child's pose is blended into those channels by the node's weight (+0x28, +0x38). Names and
// layouts are placeholders.
#include "types.h"

struct UnkStruct_71007142e8;
void fn_71007142e8(UnkStruct_71007142e8* self);
struct UnkWeighted_7100714808;
void fn_7100714604(UnkWeighted_7100714808* self, const UnkWeighted_7100714808* other);  // assigns
void fn_71007149f4(UnkWeighted_7100714808* self, const UnkWeighted_7100714808* other, f32 weight, bool flag);

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
