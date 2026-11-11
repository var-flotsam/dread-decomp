// CSluggerAcidBallMovementComponent: after the base movement update, adds the scaled direction of
// the launcher (when the settings ask for it) to the velocity, copies two settings and, the first
// time, switches the flag at +0xf9 on and calls vf_0x260. With it, the same update of
// CVulkranMagmaBallMovementComponent (fn_71006df774, its own unit), whose direction is flattened and
// capped. Names and layouts are placeholders.
#include "game/UnkTU2523.h"
#include "types.h"

// Returned in memory (x8): the copy constructor is user-provided (as in
// CKraidBouncingCreaturesMovementComponent_3.cpp).
struct UnkKraidCreatureVec {
    f32 x;
    f32 y;
    f32 z;
    UnkKraidCreatureVec(const UnkKraidCreatureVec& other) : x(other.x), y(other.y), z(other.z) {}
};

struct UnkAcidSettings {
    u8 pad_0x0[0x11a];
    bool add_direction;  // 0x11a
    u8 pad_0x11b[1];
    f32 scale;  // 0x11c
    u8 pad_0x120[0x14];
    u32 field_0x134;
    u32 field_0x138;
};

struct UnkAcidLauncherLink {
    u8 pad_0x0[0x10];
    void* launcher;
};

struct UnkAcidBallMovement {
    virtual void vf_0x0();
    u8 pad_0x8[0xf1];
    bool field_0xf9;
    u8 pad_0xfa[0x3be];
    f32 velocity_x;  // 0x4b8
    f32 velocity_y;
    f32 velocity_z;
    u8 pad_0x4c4[0x10c];
    UnkAcidSettings* settings;  // 0x5d0
    u32 field_0x5d8;
    u32 field_0x5dc;
};

struct UnkSelf;
void fn_7100ac3880(UnkSelf* self);

static inline UnkAcidLauncherLink* LinkOf(UnkAcidBallMovement* self) {
    return *reinterpret_cast<UnkAcidLauncherLink**>(reinterpret_cast<char*>(self) + 0x230);
}

static inline void CallSlot(UnkAcidBallMovement* self) {
    (*reinterpret_cast<void (***)(UnkAcidBallMovement*)>(self))[0x260 / 8](self);
}

void fn_71006c7a40(UnkAcidBallMovement* self) {
    fn_7100ac3880(reinterpret_cast<UnkSelf*>(self));
    UnkAcidLauncherLink* link = LinkOf(self);
    if (link && link->launcher && self->settings->add_direction) {
        UnkKraidCreatureVec dir = fn_7100d7be08(link->launcher);
        f32 scale = self->settings->scale;
        f32 x = scale * dir.x;
        f32 y = scale * dir.y;
        f32 z = scale * dir.z;
        self->velocity_x = x + self->velocity_x;
        self->velocity_y = y + self->velocity_y;
        self->velocity_z = z + self->velocity_z;
    }
    UnkAcidSettings* settings = self->settings;
    self->field_0x5d8 = settings->field_0x134;
    self->field_0x5dc = settings->field_0x138;
    if (self->field_0xf9 != true) {
        self->field_0xf9 = true;
        CallSlot(self);
    }
}

// CVulkranMagmaBallMovementComponent: the direction is flattened to z = 0 and capped at 600 in x and
// y, and no settings are copied.
void fn_71006df774(UnkAcidBallMovement* self) {
    fn_7100ac3880(reinterpret_cast<UnkSelf*>(self));
    UnkAcidLauncherLink* link = LinkOf(self);
    if (link && link->launcher && self->settings->add_direction) {
        UnkKraidCreatureVec dir = fn_7100d7be08(link->launcher);
        dir.z = 0.0f;
        if (dir.x > 600.0f)
            dir.x = 600.0f;
        if (dir.y > 600.0f)
            dir.y = 600.0f;
        f32 scale = self->settings->scale;
        f32 x = scale * dir.x;
        f32 y = scale * dir.y;
        f32 z = scale * dir.z;
        self->velocity_x = x + self->velocity_x;
        self->velocity_y = y + self->velocity_y;
        self->velocity_z = z + self->velocity_z;
    }
    if (self->field_0xf9 != true) {
        self->field_0xf9 = true;
        CallSlot(self);
    }
}
