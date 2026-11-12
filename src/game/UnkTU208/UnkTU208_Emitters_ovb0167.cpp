// Unit 208 of tools/tu_map.py (0x710016b51c-0x710016e03c): particle emitter objects and their per-particle
// updates. Emitters keep an owner (+0x8) whose scene allocates per-particle arrays on first use.
// Names and layouts are placeholders.
#include <new>

#include "base/UnkMatrix44.h"
#include "base/UnkVector.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

extern "C" int rand();
extern "C" f32 fmodf(f32 x, f32 y);
extern "C" f32 sqrtf(f32 x);

struct UnkMat4f;
UnkVec3f fn_7100078170(const UnkMat4f* m, const UnkVec3f* v);

extern char g_unk_7101a1db18[];
extern char g_unk_7101a1d968[];
extern char g_unk_7101a1da28[];
extern char g_unk_7101a1d9c8[];

struct UnkScene_ovb0167 {
    u8 pad_0x0[0x38c];
    u32 count;  // 0x38c
};

// Twelve-byte per-particle entry, zeroed.
struct UnkEntry_ovb0167 {
    u32 words[3] = {};
};

struct UnkOwner_ovb0167 {
    void* vtable;
    UnkScene_ovb0167* scene;  // 0x8
    u8 pad_0x10[0x10];
    void* entries;  // 0x20
};

static inline void EnsureEntries(UnkOwner_ovb0167* owner) {
    if (!owner->entries) {
        u32 n = owner->scene->count;
        UnkEntry_ovb0167* p = static_cast<UnkEntry_ovb0167*>(fn_7100083ba0((u64)n * sizeof(UnkEntry_ovb0167), 0x1a));
        if (n) {
            UnkEntry_ovb0167* e = p;
            do {
                new (e) UnkEntry_ovb0167;
                ++e;
            } while (e != p + n);
        }
        owner->entries = p;
    }
}

struct UnkEmitter_ovb0167 {
    char* vtable;
    UnkOwner_ovb0167* owner;  // 0x8
    void* field_0x10;
    void* field_0x18;
};

void fn_710016bdec(UnkEmitter_ovb0167* self, UnkOwner_ovb0167* owner) {
    self->owner = owner;
    self->vtable = g_unk_7101a1db18 + 0x10;
    EnsureEntries(owner);
    self->vtable = g_unk_7101a1d968 + 0x10;
    u8* scene = reinterpret_cast<u8*>(self->owner->scene);
    self->field_0x10 = scene + 0x108;
    self->field_0x18 = scene + 0x1d8;
}

void fn_710016c9a8(UnkEmitter_ovb0167* self, UnkOwner_ovb0167* owner, void* arg) {
    self->vtable = g_unk_7101a1db18 + 0x10;
    self->owner = owner;
    EnsureEntries(owner);
    self->field_0x10 = arg;
    self->vtable = g_unk_7101a1da28 + 0x10;
}

struct UnkEmitter3_ovb0167 {
    char* vtable;
    UnkOwner_ovb0167* owner;  // 0x8
    void* field_0x10;
    void* field_0x18;
    f32 field_0x20;
    bool field_0x24;
};

void fn_710016c3e8(UnkEmitter3_ovb0167* self, UnkOwner_ovb0167* owner, bool flag, f32 value) {
    self->vtable = g_unk_7101a1db18 + 0x10;
    self->owner = owner;
    EnsureEntries(owner);
    self->vtable = g_unk_7101a1d9c8 + 0x10;
    self->field_0x20 = value;
    self->field_0x24 = flag;
    u8* scene = reinterpret_cast<u8*>(self->owner->scene);
    self->field_0x10 = scene + 0x68;
    self->field_0x18 = scene + 8;
}

extern char g_unk_7101a1db48[];
extern char g_unk_7101a1da58[];

struct UnkEntry8_ovb0167 {
    u32 words[2] = {};
};

struct UnkOwner8_ovb0167 {
    void* vtable;
    UnkScene_ovb0167* scene;  // 0x8
    u8 pad_0x10[0x8];
    void* entries;  // 0x18
};

struct UnkGridEmitter_ovb0167 {
    char* vtable;
    UnkOwner8_ovb0167* owner;  // 0x8
    void* field_0x10;
    u32 cell_count;  // 0x18
    u32 columns;     // 0x1c
    f32 origin_x;    // 0x20
    f32 origin_y;    // 0x24
    void* field_0x28;
};

void fn_710016cea4(UnkGridEmitter_ovb0167* self, UnkOwner8_ovb0167* owner, void* arg, u32 cell_count, u32 columns,
                   u32 rows, void* cells) {
    self->vtable = g_unk_7101a1db48 + 0x10;
    self->owner = owner;
    if (!owner->entries) {
        u32 n = owner->scene->count;
        UnkEntry8_ovb0167* p = static_cast<UnkEntry8_ovb0167*>(fn_7100083ba0((u64)n * sizeof(UnkEntry8_ovb0167), 0x1a));
        if (n) {
            UnkEntry8_ovb0167* e = p;
            do {
                new (e) UnkEntry8_ovb0167;
                ++e;
            } while (e != p + n);
        }
        owner->entries = p;
    }
    self->vtable = g_unk_7101a1da58 + 0x10;
    self->field_0x10 = arg;
    self->cell_count = cell_count;
    self->columns = columns;
    self->field_0x28 = cells;
    self->origin_x = (f32)(columns - 1) * 0.5f;
    self->origin_y = (f32)(rows - 1) * 0.5f;
}

extern char g_unk_7101a1db78[];
extern char g_unk_7101a1da88[];

// A short list of floats the emitter reads: element count, a mode (2 once set), the second and the
// first value and the pointer to the second value.
struct UnkFloatList_ovb0167 {
    u32 count = 0;
    u32 mode = 0x7fffffff;
    f32 second = 0.0f;
    u32 first = 0;
    const f32* rest = nullptr;
};

static inline void SetList(UnkFloatList_ovb0167* list, u32 n, const f32* values) {
    list->count = n;
    list->mode = 2;
    list->second = n ? values[1] : 0.0f;
    list->first = *reinterpret_cast<const u32*>(values++);
    list->rest = n ? values : nullptr;
}

struct UnkOwner4_ovb0167 {
    void* vtable;
    UnkScene_ovb0167* scene;  // 0x8
    u8 pad_0x10[0x8];
    f32* entries;  // 0x18
};

struct UnkCurveEmitter_ovb0167 {
    char* vtable;
    UnkOwner4_ovb0167* owner;  // 0x8
    void* field_0x10;
    f32 field_0x18;
    u32 field_0x1c;
    u32 field_0x20 = 0;
    UnkFloatList_ovb0167 list;  // 0x28
};

void fn_710016d154(UnkCurveEmitter_ovb0167* self, UnkOwner4_ovb0167* owner, void* arg, u32 count, const f32* values,
                   f32 scale) {
    self->vtable = g_unk_7101a1db78 + 0x10;
    self->owner = owner;
    if (!owner->entries)
        owner->entries = static_cast<f32*>(fn_7100083ba0((u64)owner->scene->count * sizeof(f32), 0x1a));
    self->field_0x10 = arg;
    self->field_0x18 = scale;
    self->vtable = g_unk_7101a1da88 + 0x10;
    new (&self->field_0x20) u32(0);
    new (&self->list) UnkFloatList_ovb0167;
    SetList(&self->list, count, values);
}


struct UnkVec2f_ovb0167 {
    f32 x, y;
    UnkVec2f_ovb0167(f32 x_, f32 y_) : x(x_), y(y_) {}
    UnkVec2f_ovb0167(const UnkVec2f_ovb0167& o) : x(o.x), y(o.y) {}
};

static inline f32 RandUnit() {
    return (f32)((rand() & 0x7fff) * (1.0 / 32767.0));
}

UnkVec2f_ovb0167 fn_710016bb40(const UnkVec2f_ovb0167* a, const UnkVec2f_ovb0167* b, bool same_fraction) {
    if (same_fraction) {
        f32 t = RandUnit();
        if (t >= 1.0f)
            return *b;
        if (t < 0.0f)
            return *a;
        f32 s = 1.0f - t;
        f32 px = s * a->x;
        f32 py = s * a->y;
        f32 qx = b->x * t;
        f32 qy = b->y * t;
        return UnkVec2f_ovb0167(px + qx, py + qy);
    }
    f32 ax = a->x;
    f32 bx = b->x;
    f32 tx = RandUnit();
    f32 x = ax + (bx - ax) * tx;
    f32 ay = a->y;
    f32 by = b->y;
    f32 ty = RandUnit();
    f32 y = ay + (by - ay) * ty;
    return UnkVec2f_ovb0167(x, y);
}

UnkVec3f fn_710016bc80(const UnkVec3f* a, const UnkVec3f* b, bool same_fraction) {
    if (same_fraction) {
        f32 t = RandUnit();
        if (t >= 1.0f)
            return *b;
        f32 s = 1.0f - t;
        f32 px = s * a->x;
        f32 py = s * a->y;
        f32 pz = s * a->z;
        f32 qx = b->x * t;
        f32 qy = b->y * t;
        f32 qz = b->z * t;
        return UnkVec3f(px + qx, py + qy, pz + qz);
    }
    f32 ax = a->x;
    f32 bx = b->x;
    f32 tx = RandUnit();
    f32 x = ax + (bx - ax) * tx;
    f32 ay = a->y;
    f32 by = b->y;
    f32 ty = RandUnit();
    f32 y = ay + (by - ay) * ty;
    f32 az = a->z;
    f32 bz = b->z;
    f32 tz = RandUnit();
    f32 z = az + (bz - az) * tz;
    return UnkVec3f(x, y, z);
}

struct UnkTimeList_ovb0167 {
    u32 count;
    u32 mode;
    f32 inline_value;  // 0x8
    u32 first;
    const f32* rest;  // 0x10
};

struct UnkTimed_ovb0167 {
    u8 pad_0x0[0x30];
    UnkTimeList_ovb0167 list_a;  // 0x30
    u8 pad_0x48[0x8];
    UnkTimeList_ovb0167 list_b;  // 0x50
    u8 pad_0x68[0x308];
    f32 clock;  // 0x370
};

static inline const f32* Element(const UnkTimeList_ovb0167* list, u32 index) {
    return list->rest ? list->rest + index : &list->inline_value;
}

f32 fn_710016cdd0(UnkTimed_ovb0167* self, u32 index) {
    f32 t = self->clock;
    f32 start = *Element(&self->list_a, index);
    f32 length = *Element(&self->list_b, index);
    return (t - start) / length;
}

f32 fn_710016ce20(UnkTimed_ovb0167* self, u32 index, bool advance, f32 period) {
    f32 t = self->clock;
    f32 start = *Element(&self->list_a, index);
    if (advance) {
        f32 x = (f32)reinterpret_cast<s64>(self) * 1.0e-6f;
        x += t;
        x = (f32)index * 0.001f + x;
        t = x + start;
    } else {
        t = t - start;
    }
    return fmodf(t, period) / period;
}

// A target holding a vector per particle (+0x20).
struct UnkVec3Target_ovb0167 {
    u8 pad_0x0[0x20];
    UnkVec3f* values;  // 0x20
};

// The source of the added vector: a per-particle one, or the single one at +0x10 without them.
struct UnkVec3Source_ovb0167 {
    u8 pad_0x0[0x10];
    UnkVec3f constant;  // 0x10
    UnkVec3f* values;   // 0x20
};

// The frame (the source's +0x88) carries the particle's position, then its angles and its velocity
// through the engine's transform helpers.
struct UnkFrameSource_ovb0167 {
    u8 pad_0x0[0x88];
    u8 frame[1];  // 0x88
};

struct UnkFrameUpdater_ovb0167 {
    u64 field_0x0;
    UnkVec3Target_ovb0167* positions;     // 0x8
    UnkFrameSource_ovb0167* source;       // 0x10
    UnkVec3Target_ovb0167* angles;        // 0x18
    UnkVec3Target_ovb0167* velocities;    // 0x20
};

static inline void Update(UnkFrameUpdater_ovb0167* self, u32 id) {
    UnkVec3Target_ovb0167* positions = self->positions;
    void* frame = self->source->frame;
    UnkVec3f* position = &positions->values[id];
    UnkVec3f* old_angles = self->angles->values;
    UnkVec3f* velocity = &self->velocities->values[id];
    positions->values[id] = fn_7100078170(static_cast<const UnkMat4f*>(frame), position);
    UnkVec3Target_ovb0167* angles = self->angles;
    {
        UnkVec3f delta = fn_7100076acc(static_cast<const UnkXform*>(frame));
        const UnkVec3f& old = old_angles[id];
        angles->values[id] = UnkVec3f(old.x + delta.x, old.y + delta.y, old.z + delta.z);
    }
    UnkVec3Target_ovb0167* velocities = self->velocities;
    velocities->values[id] = fn_7100077c04(*static_cast<const UnkMatrix44*>(frame), *velocity);
}

void fn_710016c1c8(UnkFrameUpdater_ovb0167* self, u32 id) {
    Update(self, id);
}

void fn_710016c2cc(UnkFrameUpdater_ovb0167* self, const u32* ids, u32 count) {
    for (; count != 0; --count, ++ids)
        Update(self, *ids);
}

struct UnkDirEmitter_ovb0167 {
    u64 field_0x0;
    UnkVec3Target_ovb0167* positions;  // 0x8
    UnkVec3Source_ovb0167* source;     // 0x10
};

static inline __attribute__((always_inline)) UnkVec3f Normalized(const UnkVec3f& v) {
    f32 len = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    f32 inv = 1.0f / (len + 1.0e-4f);
    return UnkVec3f(v.x * inv, v.y * inv, inv * v.z);
}

static inline UnkVec3f Shifted(const UnkVec3f& r) {
    return UnkVec3f(r.x + 1.57f, r.y + 0.0f, r.z + 0.0f);
}

typedef UnkVec3f (*UnkToAngles_ovb0167)(const UnkVec3f&);
void fn_710016ca5c(UnkDirEmitter_ovb0167* self, u32 id) {
    UnkVec3Source_ovb0167* source = self->source;
    const UnkVec3f& v = source->values ? source->values[id] : source->constant;
    UnkVec3f dir = Normalized(v);
    UnkVec3f r = ((UnkToAngles_ovb0167)fn_710007a7f0)(dir);
    self->positions->values[id] = UnkVec3f(r.x + 1.57f, r.y + 0.0f, r.z + 0.0f);
}

struct UnkParticle_ovb0167 {
    u8 pad_0x0[0x5c];
    UnkVec3f position;  // 0x5c
};

struct UnkParticleSet_ovb0167 {
    u8 pad_0x0[0x18];
    UnkParticle_ovb0167** particles;  // 0x18
};

struct UnkStepper_ovb0167 {
    u64 field_0x0;
    UnkVec3Target_ovb0167* positions;   // 0x8
    UnkVec3Source_ovb0167* source;      // 0x10
    UnkParticleSet_ovb0167* set;        // 0x18
    f32 distance;                       // 0x20
    bool flat;                          // 0x24
};

static inline void Step(UnkStepper_ovb0167* self, u32 id) {
    UnkVec3f v = self->source->values[id];
    if (self->flat)
        v.y = 0.0f;
    if (self->set) {
        UnkParticle_ovb0167* p = self->set->particles[id];
        if (p) {
            v.x -= p->position.x;
            v.y -= p->position.y;
            v.z -= p->position.z;
        }
    }
    UnkVec3Target_ovb0167* positions = self->positions;
    const UnkVec3f& cur = positions->values[id];
    UnkVec3f dir = Normalized(v);
    f32 d = self->distance;
    positions->values[id] = UnkVec3f(cur.x + d * dir.x, cur.y + d * dir.y, cur.z + d * dir.z);
}

void fn_710016c4c0(UnkStepper_ovb0167* self, u32 id) {
    Step(self, id);
}

void fn_710016c5f8(UnkStepper_ovb0167* self, const u32* ids, u32 count) {
    for (; count != 0; --count, ++ids)
        Step(self, *ids);
}

void fn_710016cb54(UnkDirEmitter_ovb0167* self, const u32* ids, u32 count) {
    UnkVec3Source_ovb0167* source = self->source;
    if (source->values) {
        for (; count != 0; --count, ++ids) {
            u32 id = *ids;
            UnkVec3f dir = Normalized(self->source->values[id]);
            UnkVec3f r = ((UnkToAngles_ovb0167)fn_710007a7f0)(dir);
            self->positions->values[id] = UnkVec3f(r.x + 1.57f, r.y + 0.0f, r.z + 0.0f);
        }
    } else {
        UnkVec3f dir = Normalized(source->constant);
        for (; count != 0; --count, ++ids) {
            u32 id = *ids;
            UnkVec3f r = ((UnkToAngles_ovb0167)fn_710007a7f0)(dir);
            self->positions->values[id] = UnkVec3f(r.x + 1.57f, r.y + 0.0f, r.z + 0.0f);
        }
    }
}
