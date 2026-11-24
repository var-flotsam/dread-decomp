// Unit 210 (a GPU particle system's emitters): spawns particles, giving each new one its attributes'
// default values, a position from the spawn volume (or the default), and the volume's transform.
// Names and layouts are placeholders.
#include "game/UnkMat4f.h"
#include "game/UnkSpawnVolume.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

static_assert(__builtin_offsetof(UnkSpawnVolume, shape) == 0x8c, "UnkSpawnVolume placeholder layout");

UnkVec3f fn_710016e468(const UnkSpawnVolume* self);

// Two floats, copied field by field.
struct UnkVec2f_l16f818 {
    f32 x, y;
    UnkVec2f_l16f818& operator=(const UnkVec2f_l16f818& o) {
        x = o.x;
        y = o.y;
        return *this;
    }
};

// One per-particle attribute: its default value and the per-particle values (null when unused).
template <class T>
struct UnkParticleAttr_l16f818 {
    u8 pad_0x0[0x10];
    T value;
    T* data;

    void Set(u32 particle, const T& v) {
        if (data)
            data[particle] = v;
    }
    void Reset(u32 particle) { Set(particle, value); }
};

struct UnkParticleListener_l16f818 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(u32 particle);  // a particle was spawned
};

struct UnkEmitter_l16f818 {
    u8 pad_0x0[0x8];
    UnkParticleAttr_l16f818<const UnkSpawnVolume*> volume;  // 0x8
    UnkParticleAttr_l16f818<f32> start_time;                // 0x28
    UnkParticleAttr_l16f818<f32> lifetime;                  // 0x48
    UnkParticleAttr_l16f818<UnkVec3f> position;             // 0x68
    UnkParticleAttr_l16f818<UnkVec3f> field_0x90;
    UnkParticleAttr_l16f818<UnkVec3f> field_0xb8;
    UnkParticleAttr_l16f818<UnkVec3f> field_0xe0;
    UnkParticleAttr_l16f818<UnkVec3f> field_0x108;  // transformed by the volume's matrix
    UnkParticleAttr_l16f818<UnkVec3f> field_0x130;
    UnkParticleAttr_l16f818<f32> field_0x158;
    UnkParticleAttr_l16f818<UnkVec2f_l16f818> field_0x178;
    UnkParticleAttr_l16f818<f32> field_0x198;
    UnkParticleAttr_l16f818<UnkVec2f_l16f818> field_0x1b8;
    UnkParticleAttr_l16f818<UnkVec3f> field_0x1d8;  // starts at zero
    UnkParticleListener_l16f818** listeners;         // 0x200
    u8 pad_0x208[4];
    u32 listener_count;  // 0x20c
    u8 pad_0x210[0x370 - 0x210];
    f32 time;  // 0x370
    u8 pad_0x374[0x390 - 0x374];
    u32 count;  // 0x390
    u8 pad_0x394[4];
    u32* indices;  // 0x398
};
static_assert(__builtin_offsetof(UnkEmitter_l16f818, field_0x1d8) == 0x1d8, "layout");
static_assert(__builtin_offsetof(UnkEmitter_l16f818, indices) == 0x398, "layout");

void fn_710016f818(UnkEmitter_l16f818* self, u32 n, const UnkSpawnVolume* volume) {
    for (; n != 0; --n) {
        u32 particle = self->indices[self->count++];
        self->volume.Set(particle, volume);
        self->start_time.Set(particle, self->time);
        self->lifetime.Reset(particle);
        self->position.Set(particle, volume ? fn_710016e468(volume) : self->position.value);
        self->field_0x90.Reset(particle);
        self->field_0xb8.Reset(particle);
        self->field_0x108.Reset(particle);
        self->field_0x130.Reset(particle);
        self->field_0x158.Reset(particle);
        self->field_0x178.Reset(particle);
        self->field_0x198.Reset(particle);
        self->field_0x1b8.Reset(particle);
        self->field_0x1d8.Set(particle, UnkVec3f(0.0f, 0.0f, 0.0f));
        for (u64 i = 0; i < self->listener_count; ++i)
            self->listeners[i]->vf_0x10(particle);
        if (volume && self->field_0x108.data) {
            self->field_0x108.data[particle] = TransformDirectionByMatrix3x3(
                reinterpret_cast<const UnkMatrix44&>(volume->matrix), self->field_0x108.data[particle]);
        }
    }
}
