// Unit 1134 of tools/tu_map.py (no class name; the track actions of game/UnkAnimationComponent.h).
// fn_710070d8a8 starts an action for a track: the listener at +0x10 hears of it (vtable slot 0),
// a pending start time (+0x160, taken and cleared) is applied when the track asks for it (flag 0x100
// at +0x10), clamped to just under the track's length (+0x28 of the track's +0x10) and at least 0
// (fn_710070d970); else the action takes the track's value at +0x30 (+0xa8). Then +0x38 is reset
// (fn_71007145f0). Names and layouts are placeholders.
#include "types.h"

struct UnkTrackClip_b50 {
    u8 pad_0x0[0x28];
    f32 length;
};

struct UnkTrackDef_b50 {
    u8 pad_0x0[0x10];
    UnkTrackClip_b50* clip;
};

struct UnkTrackStart_b50 {
    UnkTrackDef_b50* def;
    u8 pad_0x8[0x8];
    u32 flags;  // 0x10
    u8 pad_0x14[0x1c];
    u32 field_0x30;
};

struct UnkTrackEvent_b50 {
    u32 flags;
    void* action;
    UnkTrackDef_b50* def;
};

struct UnkTrackListener_b50 {
    virtual void vf_0x0(UnkTrackEvent_b50* event);
};

struct UnkTrackAction_b50 {
    u8 pad_0x0[0x10];
    UnkTrackListener_b50* listener;
    u8 pad_0x18[0x20];
    u8 field_0x38[0x70];
    u32 field_0xa8;
    u8 pad_0xac[0xb4];
    f32 start_time;  // 0x160
};

void fn_710070d970(UnkTrackAction_b50* self, f32 time);
void fn_71007145f0(void* self, const u32* value);

void fn_710070d8a8(UnkTrackAction_b50* self, UnkTrackStart_b50* start) {
    if (UnkTrackListener_b50* listener = self->listener) {
        UnkTrackEvent_b50 event;
        event.action = self;
        event.def = start->def;
        event.flags = start->flags;
        listener->vf_0x0(&event);
    }
    f32 time = self->start_time;
    self->start_time = 0.0f;
    u32 apply = start->flags & 0x100;
    if (!(time >= 0.0f) || apply == 0) {
        self->field_0xa8 = start->field_0x30;
    } else {
        if (UnkTrackDef_b50* def = start->def) {
            if (UnkTrackClip_b50* clip = def->clip) {
                f32 last = clip->length + -0.016666668f;
                time = !(time <= 0.0f) ? (last <= time ? last : time) : 0.0f;
            }
        }
        fn_710070d970(self, time);
    }
    u32 zero = 0;
    fn_71007145f0(self->field_0x38, &zero);
}
