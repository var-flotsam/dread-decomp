// CRedenkiSwarmControllerComponent's unit (tools/tu_map.py unit 977): the close check. Unless fn_710065634c
// says to leave, the state becomes 0 once the target is nearer along the path than the tuned distance.
// Names and layouts are placeholders.
#include "game/CFlockingSwarmControllerComponent.h"
#include "game/CRedenkiSwarmControllerComponent.h"
#include "types.h"

struct UnkPathPoints;
f32 fn_71010598e0(UnkPathPoints* self, u32 a, u32 b);

// Read as one 64-bit word: the area in the low half, the index in the high half.
struct UnkRedenkiPathPos_l656cb0 {
    u64 bits;
    s32 Area() const { return (s32)bits; }
    u32 Index() const { return (u32)(bits >> 32); }
};

// The swarm's target as fn_7100739c64 gives it: its place on the path and its distance from that point.
struct UnkRedenkiTarget_l656cb0 {
    u8 pad_0x0[0x50];
    UnkRedenkiPathPos_l656cb0 pathPos;  // 0x50
    u8 pad_0x58[0xc];
    f32 pathOffset;  // 0x64
};

struct UnkRedenkiArea_l656cb0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual s32 vf_0x28();
};

struct UnkRedenkiTuning_l656cb0 {
    u8 pad_0x0[0x478];
    f32 closeDistance;  // 0x478
};

struct UnkRedenkiPath_l656cb0 {
    u8 pad_0x0[0x10];
    u32 count;  // 0x10
};

struct UnkRedenkiController_l656cb0 {
    void** vtable;
    u8 pad_0x8[0x80 - 0x8];
    UnkRedenkiArea_l656cb0* area;  // 0x80
    u8 pad_0x88[0x2c8 - 0x88];
    UnkRedenkiPath_l656cb0 path;  // 0x2c8
    u8 pad_0x2dc[0x560 - 0x2dc];
    UnkRedenkiTuning_l656cb0* tuning;  // 0x560

    void SetState(u32 a, u32 b) {
        reinterpret_cast<void (*)(UnkRedenkiController_l656cb0*, u32, u32)>(vtable[0x2e8 / 8])(this, a, b);
    }
};

void fn_7100656cb0(UnkRedenkiController_l656cb0* self) {
    bool leave = fn_710065634c(reinterpret_cast<UnkDaivoSwarm_b0012*>(self));
    UnkRedenkiTarget_l656cb0* target = reinterpret_cast<UnkRedenkiTarget_l656cb0*>(
        fn_7100739c64(reinterpret_cast<UnkSwarmUpdate_b65a4*>(self)));
    if (!leave) {
        UnkRedenkiPathPos_l656cb0 pos = target->pathPos;
        u32 index = pos.Index();
        u32 last = self->path.count - 1;
        f32 distance;
        f32 along;
        if (self->area->vf_0x28() != pos.Area() || self->path.count <= index || self->path.count <= last)
            distance = 3.40282347e+38f;
        else if (along = fn_71010598e0(reinterpret_cast<UnkPathPoints*>(&self->path), index, last), last < index)
            distance = along - target->pathOffset;
        else
            distance = along + target->pathOffset;
        if (!(distance < self->tuning->closeDistance))
            return;
    }
    self->SetState(0, 0);
}
