// Unit 2545 (tools/tu_map.py): adds the elapsed time to the flow's timer, polls for a skip, fades the
// panel's widgets in over half a second and then (or when skipped) moves to state 8.
// Names and layouts are placeholders.
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkStateMachineFind.h"
#include "game/UnkTU223.h"
#include "game/UnkTU2545.h"
#include "game/UnkTU2747.h"
#include "types.h"

struct UnkPanel_b0277;
using UnkGuiObj = UnkGuiDisplayObject;

struct UnkFlowRender_ldd82a8 {
    u8 pad_0x0[0x78];
    void* field_0x78;  // takes a colour (fn_710019c564)
};

struct UnkFlowWidget_ldd82a8 {
    u8 pad_0x0[0x178];
    UnkFlowRender_ldd82a8* render;
};

struct UnkFlowState_ldd82a8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    u64 field_0x8;
    s32 id;
};

// The states are only 4-aligned after the count.
typedef UnkFlowState_ldd82a8* __attribute__((aligned(4))) UnkFlowStatePtr_ldd82a8;

struct UnkFlowMachine_ldd82a8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkFlowState_ldd82a8* previous, UnkFlowState_ldd82a8* next);  // after entering
    virtual void vf_0x28(UnkFlowState_ldd82a8* current, UnkFlowState_ldd82a8* next);   // after leaving

    u64 field_0x8;
    UnkFlowState_ldd82a8* current;
    UnkFlowState_ldd82a8* previous;
    u32 count;
    UnkFlowStatePtr_ldd82a8 states[16];
    s32 pending;
    bool busy;

    static const s32 kNoState = 9;

    UnkFlowState_ldd82a8* FindState(s32 id) { return FindStateById(states, count, id); }

    void RequestState(s32 id) {
        pending = id;
        if (busy)
            return;
        UnkFlowState_ldd82a8* cur = current;
        if (!cur || cur->id != id) {
            if (UnkFlowState_ldd82a8* next = FindState(id)) {
                if (cur) {
                    cur->vf_0x18(id);
                    vf_0x28(current, next);
                }
                previous = current;
                current = next;
                next->vf_0x10(previous ? previous->id : kNoState);
                vf_0x20(previous, next);
            }
        }
        pending = kNoState;
    }
};

struct UnkFlow_ldd82a8 : UnkFlowMachine_ldd82a8 {
    u32 field_0xac;
    f32 time;
    u8 pad_0xb4[0xdc - 0xb4];
    f32 field_0xdc;
    u8 pad_0xe0[0x120 - 0xe0];
    UnkFlowWidget_ldd82a8* field_0x120;
    UnkGuiObj* field_0x128;
    UnkGuiObj* field_0x130;
    u8 pad_0x138[0x158 - 0x138];
    UnkFlowWidget_ldd82a8* field_0x158;
    UnkGuiObj* field_0x160;
    UnkGuiObj* field_0x168;
    UnkGuiObj* field_0x170;
    u8 pad_0x178[0x188 - 0x178];
    UnkGuiObj* field_0x188;
    UnkGuiObj* field_0x190;
    u8 pad_0x198[0x269 - 0x198];
    bool skip;
};
static_assert(__builtin_offsetof(UnkFlow_ldd82a8, time) == 0xb0, "layout");
static_assert(__builtin_offsetof(UnkFlow_ldd82a8, skip) == 0x269, "layout");
static_assert(__builtin_offsetof(UnkFlowMachine_ldd82a8, pending) == 0xa4, "layout");

static inline f32 Clamp01(f32 v) {
    return v <= 0.0f ? 0.0f : (!(v >= 1.0f) ? v : 1.0f);
}

void fn_7100dd82a8(UnkFlow_ldd82a8* self, f32 dt) {
    fn_7100dd60fc(reinterpret_cast<UnkPanel_b0277*>(self));
    self->time += dt;
    f32 alpha = Clamp01(self->time / 0.5f);
    fn_710019c564(self->field_0x120->render->field_0x78, alpha, alpha, alpha, 1.0f);
    if (self->field_0xdc > 0.0f) {
        fn_71011cfba8(self->field_0x188, alpha);
        fn_71011cfba8(self->field_0x190, alpha);
    }
    fn_710019c564(self->field_0x158->render->field_0x78, 1.0f, 1.0f, 1.0f, alpha);
    fn_71011cfba8(self->field_0x160, alpha);
    fn_71011cfba8(self->field_0x168, alpha);
    fn_71011cfba8(self->field_0x170, alpha);
    if (self->field_0x128)
        fn_71011cfba8(self->field_0x128, alpha);
    if (self->field_0x130)
        fn_71011cfba8(self->field_0x130, alpha);
    if (self->time >= 0.5f || self->skip)
        self->RequestState(8);
}
