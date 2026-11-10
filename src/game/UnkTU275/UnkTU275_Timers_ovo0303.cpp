// Unit 275: the update of a timed effect (the object behind +0x108 is the model it drives): when active,
// it follows its owner's position, scales the time step by the global time scale (clamped to what the
// effect allows while the scale is off 1), counts down its three timers (+0x15c, +0x168 with a
// fade between two levels written to the model's colour, +0x174 with a callback when it runs out), then
// lets the model update. Names and layouts are placeholders.
#include "game/UnkSphere.h"
#include "types.h"

struct UnkLockonModel_ovb0092;
struct UnkColor_ovb0092;
struct UnkSource_710020c780;
void fn_71001bff28(UnkLockonModel_ovb0092* model, u32 index, UnkColor_ovb0092* color);
void fn_71001bfea4(UnkLockonModel_ovb0092* model, u32 index, const UnkColor_ovb0092* color);
void fn_71001bffc4(UnkSource_710020c780* source, f32 dt);
f32 fn_7100150e50();
struct UnkTimedEffect_ovo0303;
void fn_71001f530c(UnkTimedEffect_ovo0303* self);

struct UnkTimedEffect_ovo0303 {
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
    virtual void vf_0x50(bool flag);
    u8 pad_0x8[0x1c];
    f32 position_x;  // 0x24
    u8 pad_0x28[0xc];
    f32 position_y;  // 0x34
    u8 pad_0x38[0xc];
    f32 position_z;  // 0x44
    u8 pad_0x48[0x38];
    UnkSphere sphere;  // 0x80
    u8 pad_0xd0[0x10];
    f32 time_limit;  // 0xe0
    u8 pad_0xe4[0x15];
    bool active;  // 0xf9
    bool follow;  // 0xfa
    u8 pad_0xfb[2];
    bool timer_a_running;  // 0xfd
    u8 pad_0xfe[0xa];
    void* model;  // 0x108
    u8 pad_0x110[0x4c];
    f32 timer_a;  // 0x15c
    f32 level;  // 0x160
    f32 duration;  // 0x164
    f32 timer_b;  // 0x168
    f32 level_from;  // 0x16c
    f32 level_to;  // 0x170
    f32 timer_c;  // 0x174
    u8 pad_0x178[8];
    bool restart_b;  // 0x180
    bool arg_c;  // 0x181
    bool scaled;  // 0x182
};
static_assert(__builtin_offsetof(UnkTimedEffect_ovo0303, scaled) == 0x182, "layout");

bool fn_71001f508c(UnkTimedEffect_ovo0303* self, f32 dt) {
    if (!self->active)
        return false;
    if (self->follow) {
        UnkVec3f position(self->position_x, self->position_y, self->position_z);
        fn_7100216978(&self->sphere, &position);
    }
    f32 scale = fn_7100150e50();
    if (scale > 0.0f) {
        if (self->scaled && (!(scale + 0.01f >= 1.0f) || !(scale - 0.01f <= 1.0f))) {
            f32 limit = self->time_limit;
            if (limit > 0.0f) {
                f32 capped = !(limit >= 1.0f) ? limit : 1.0f;
                scale = capped;
                if (limit <= 0.5f)
                    scale = 0.5f;
            }
            dt = scale * dt;
        }
    }
    if (self->timer_a > 0.0f && self->timer_a < 3.4028235e38f) {
        self->timer_a = self->timer_a - dt;
        self->timer_a_running = self->timer_a > 0.0f;
    }
    if (self->timer_b > 0.0f) {
        f32 remaining = self->timer_b - dt;
        self->timer_b = remaining;
        if (remaining <= 0.0f) {
            bool restart = self->restart_b;
            self->timer_b = 0.0f;
            if (restart) {
                self->vf_0x50(false);
                remaining = self->timer_b;
            } else {
                remaining = 0.0f;
            }
        }
        f32 ratio = remaining / self->duration;
        f32 level = self->level_from;
        f32 to = self->level_to;
        if (!(ratio >= 1.0f)) {
            if (ratio < 0.0f)
                level = to;
            else
            {
                f32 inverse = 1.0f - ratio;
                level = level * ratio + to * inverse;
            }
        }
        self->level = level;
        f32 color[4];
        color[0] = 1.0f;
        color[1] = 1.0f;
        color[2] = 1.0f;
        color[3] = 1.0f;
        fn_71001bff28(reinterpret_cast<UnkLockonModel_ovb0092*>(self->model), 0,
                      reinterpret_cast<UnkColor_ovb0092*>(color));
        color[3] = !(level <= 0.0f) ? level : 0.0f;
        fn_71001bfea4(reinterpret_cast<UnkLockonModel_ovb0092*>(self->model), 0,
                      reinterpret_cast<const UnkColor_ovb0092*>(color));
    }
    if (self->timer_c > 0.0f) {
        f32 remaining = self->timer_c - dt;
        self->timer_c = remaining;
        if (remaining <= 0.0f) {
            self->timer_c = 0.0f;
            self->vf_0x50(self->arg_c);
        }
    }
    fn_71001f530c(self);
    fn_71001bffc4(reinterpret_cast<UnkSource_710020c780*>(self->model), dt);
    return self->active;
}
