// CActionInstance's unit (tools/tu_map.py): an action instance (0xd0 bytes, reference counted at +0x8,
// vtable g_unk_7101aa50e0) playing an action's animation. Holds its default constructor (fn_7100c95da8),
// two copies of its copy constructor (fn_7100c97ae4, fn_7100c97bb4) and the setter of its time
// (fn_7100c967c8), which snaps to the end, wraps past it and converts to frames (fn_7100b8aff0).
// Names and layouts are placeholders.
#include "game/UnkTU1728.h"
#include "types.h"

extern char g_unk_7101aa50e0[];

struct UnkAnimTiming;
extern "C" f32 fmodf(f32 x, f32 y);

struct UnkActionAnim_ovb0073 {
    u8 pad_0x0[0x14];
    s32 mode;  // 0x14
    u8 pad_0x18[0x10];
    f32 length;  // 0x28
};

struct UnkActionOwner_ovb0073 {
    u8 pad_0x0[0x175];
    bool field_0x175;
};

struct UnkActionTrack_ovb0073 {
    u64 data[4];
    u32 field_0x20;
    u32 field_0x24;
    bool field_0x28;
};

struct UnkActionInstance_ovb0073 {
    char* vtable;
    u32 refs;  // 0x8
    UnkActionAnim_ovb0073* anim;  // 0x10
    void* field_0x18;
    UnkActionOwner_ovb0073* owner;  // 0x20
    void* field_0x28;
    UnkActionTrack_ovb0073 track_a;  // 0x30
    f32 frame;  // 0x60
    f32 time;  // 0x64
    f32 wrapped;  // 0x68
    f32 field_0x6c;
    u32 mask;  // 0x70
    bool field_0x74;
    bool field_0x75;
    bool field_0x76;
    UnkActionTrack_ovb0073 track_b;  // 0x78
    u32 field_0xa8;
    void* field_0xb0;
    void* field_0xb8;
    void* field_0xc0;
    char* field_0xc8;  // shared count (use count at +8)
};

static inline void CopyTrack(UnkActionTrack_ovb0073* dst, const UnkActionTrack_ovb0073* src) {
    __builtin_memcpy(dst->data, src->data, sizeof(dst->data));
    dst->field_0x20 = src->field_0x20;
    dst->field_0x24 = src->field_0x24;
    dst->field_0x28 = src->field_0x28;
}

static inline void CopyActionInstance(UnkActionInstance_ovb0073* self, const UnkActionInstance_ovb0073* other) {
    self->refs = 0;
    self->vtable = g_unk_7101aa50e0 + 0x10;
    __builtin_memcpy(&self->anim, &other->anim, 0x20);
    CopyTrack(&self->track_a, &other->track_a);
    __builtin_memcpy(&self->frame, &other->frame, 0x10);
    self->mask = other->mask;
    __builtin_memcpy(&self->field_0x74, &other->field_0x74, 3);
    CopyTrack(&self->track_b, &other->track_b);
    __builtin_memcpy(&self->field_0xa8, &other->field_0xa8, 0x18);
    self->field_0xc0 = other->field_0xc0;
    char* count = other->field_0xc8;
    self->field_0xc8 = count;
    if (count)
        __atomic_fetch_add(reinterpret_cast<u64*>(count + 8), 1, __ATOMIC_RELAXED);
}

void fn_7100c97ae4(UnkActionInstance_ovb0073* self, const UnkActionInstance_ovb0073* other) {
    CopyActionInstance(self, other);
}

void fn_7100c97bb4(UnkActionInstance_ovb0073* self, const UnkActionInstance_ovb0073* other) {
    CopyActionInstance(self, other);
}

// Sets the time: within 0.001 of the end it snaps to the end; past it, it wraps (kept at +0x68)
// and, unless looping (mode 1 or the owner's flag), the time stays at the end.
void fn_7100c967c8(UnkActionInstance_ovb0073* self, f32 t) {
    UnkActionAnim_ovb0073* anim = self->anim;
    s32 mode = anim->mode;
    f32 length = anim->length;
    if (mode != 1) {
        f32 lo = t - 0.001f;
        f32 hi = t + 0.001f;
        t = (lo <= length && hi >= length) ? length : t;
    }
    f32 time = t;
    if (t >= length) {
        t = fmodf(t, length);
        self->wrapped = t;
        time = length;
        if (self->anim->mode == 1 || self->owner->field_0x175)
            time = t;
    }
    self->time = time;
    self->frame = fn_7100b8aff0(time, reinterpret_cast<const UnkAnimTiming*>(self->anim));
}

void fn_71000684e4(UnkActionTrack_ovb0073* track);  // a track's constructor

// A mask with no bits and one with the low eight, each a guarded static.
struct UnkActionMask_ovb0073 {
    u32 bits;
    UnkActionMask_ovb0073(u32 value) : bits(value) {}
};

inline const UnkActionMask_ovb0073& NoActionMask_ovb0073() {
    static UnkActionMask_ovb0073 s_mask(0);
    return s_mask;
}

inline const UnkActionMask_ovb0073& AllActionMask_ovb0073() {
    static UnkActionMask_ovb0073 s_mask(static_cast<u8>(~static_cast<u8>(NoActionMask_ovb0073().bits)));
    return s_mask;
}

void fn_7100c95da8(UnkActionInstance_ovb0073* self) {
    self->refs = 0;
    self->vtable = g_unk_7101aa50e0 + 0x10;
    self->anim = nullptr;
    self->field_0x18 = nullptr;
    self->owner = nullptr;
    self->field_0x28 = nullptr;
    fn_71000684e4(&self->track_a);
    self->frame = 0.0f;
    self->time = 0.0f;
    self->wrapped = 0.0f;
    self->field_0x6c = 0.0f;
    self->mask = AllActionMask_ovb0073().bits;
    self->field_0x74 = true;
    self->field_0x75 = true;
    self->field_0x76 = true;
    fn_71000684e4(&self->track_b);
    self->field_0xa8 = 0;
    self->field_0xb0 = nullptr;
    self->field_0xb8 = nullptr;
    self->field_0xc0 = nullptr;
    self->field_0xc8 = nullptr;
}
