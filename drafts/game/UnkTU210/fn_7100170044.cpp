// A unit with no known name (unit 210): advances a GPU particle emitter by one step, retires the particles
// whose lifetime is over, releases its buffers once none are left and updates its children.
// Names and layouts are placeholders.
#include "types.h"

struct UnkEmitterChild_l170044 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18(u32* indices, u32 count);
};

struct UnkEmitter_l170044 {
    u8 pad_0x0[0x40];
    f32* start_times;  // 0x40
    u8 pad_0x48[0x10];
    f32 default_life;  // 0x58
    u8 pad_0x5c[4];
    f32* lifetimes;  // 0x60
    u8 pad_0x68[0x210 - 0x68];
    UnkEmitterChild_l170044** children;  // 0x210
    u8 pad_0x218[4];
    u32 child_count;  // 0x21c
    u8 pad_0x220[0x370 - 0x220];
    f32 time;  // 0x370
    u8 pad_0x374[0x390 - 0x374];
    u32 count;  // 0x390
    u8 pad_0x394[4];
    u32* indices;  // 0x398
    void* field_0x3a0;
    void* field_0x3a8;
    u8 pad_0x3b0[0x43b - 0x3b0];
    u8 dirty;  // 0x43b
};

extern u32 g_unk_716e352430;
extern void* g_unk_716e34e430[];

void fn_7100170044(UnkEmitter_l170044* self, f32 dt) {
    self->time += dt;
    self->dirty = 1;
    u32 i = 0;
    u32 remaining = self->count;
    while (remaining != 0) {
        u32* indices = self->indices;
        --remaining;
        u32 particle = indices[i];
        const f32* life = self->lifetimes ? self->lifetimes + particle : &self->default_life;
        f32 limit = *life;
        f32 age = self->time - self->start_times[particle];
        if (limit <= age) {
            u32 last = --self->count;
            u32 moved = indices[last];
            indices[i] = moved;
            self->indices[self->count] = particle;
        } else {
            ++i;
        }
    }
    if (self->count == 0) {
        if (self->field_0x3a0) {
            u32 n = g_unk_716e352430;
            g_unk_716e34e430[n] = self->field_0x3a0;
            g_unk_716e352430 = n + 1;
        }
        self->field_0x3a0 = nullptr;
        if (self->field_0x3a8) {
            u32 n = g_unk_716e352430;
            g_unk_716e34e430[n] = self->field_0x3a8;
            g_unk_716e352430 = n + 1;
        }
        self->field_0x3a8 = nullptr;
    }
    u32 child_count = self->child_count;
    for (u64 c = 0; c != child_count; ++c) {
        UnkEmitterChild_l170044* child = self->children[c];
        child->vf_0x18(self->indices, self->count);
    }
}
