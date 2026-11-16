// Unit 1445 (CCharacterMovement): on the frame after a contact, clears or clips the two stored velocities
// against the contact normal, unless a recorded contact faces away from it. Names and layouts are placeholders.
#include <algorithm>
#include "types.h"
#include "base/UnkNoArg.h"

struct UnkVec3_l97762c {
    f32 x, y, z;
};

static inline f32 Dot_l97762c(const UnkVec3_l97762c& a, const UnkVec3_l97762c& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

struct UnkContactTarget_l97762c {
    u8 unk_0x0[0x10];
    void* object;  // 0x10
};

struct UnkContactRef_l97762c {
    UnkContactTarget_l97762c* target;  // 0x0

    void* Get() const { return target != nullptr ? target->object : nullptr; }
};

struct UnkIgnoreEntry_l97762c {
    UnkContactRef_l97762c ref;  // 0x0
    u8 unk_0x8[0x8];
};

struct UnkIgnoreList_l97762c {
    UnkIgnoreEntry_l97762c* data;  // 0x0
    u32 count;                     // 0x8

    bool Contains(void* object) const {
        for (const UnkIgnoreEntry_l97762c* it = data; it != data + count; ++it) {
            void* other = it->ref.Get();
            if (other != nullptr && other == object)
                return true;
        }
        return false;
    }
};

#pragma pack(push, 4)
struct UnkContact_l97762c {
    UnkContactRef_l97762c ref;  // 0x0
    u8 unk_0x8[0x18];
    UnkVec3_l97762c normal;     // 0x20
    u8 unk_0x2c[0x1c];
};
#pragma pack(pop)
static_assert(sizeof(UnkContact_l97762c) == 0x48, "layout");

struct UnkContactList_l97762c {
    u32 count;                      // 0x0
    UnkContact_l97762c data[1];     // 0x4

    const UnkContact_l97762c* begin() const { return data; }
    const UnkContact_l97762c* end() const { return data + count; }
};

struct UnkMoveState_l97762c {
    u8 unk_0x0[0x10];
    u32 frame;                      // 0x10
    UnkVec3_l97762c velocity;       // 0x14
    UnkVec3_l97762c push;           // 0x20
    u8 unk_0x2c[0x2c];
    UnkContactList_l97762c contacts;  // 0x58

    void Stop() {
        velocity.x = 0.0f;
        velocity.y = 0.0f;
        velocity.z = 0.0f;
        push.x = 0.0f;
        push.y = 0.0f;
        push.z = 0.0f;
    }
};
static_assert(__builtin_offsetof(UnkMoveState_l97762c, contacts) == 0x58, "layout");

struct UnkSurface_l97762c {
    u8 unk_0x0[0x20];
    UnkVec3_l97762c normal;  // 0x20
    u8 unk_0x2c[0x4];
    f32 friction;            // 0x30
    u8 unk_0x34[0x4];
    f32 grip;                // 0x38
};

struct UnkContactInfo_l97762c {
    u32 frame;  // 0x0
    u8 unk_0x4[0x8];
    s32 kind;   // 0xc
};

struct UnkMoveConfig_l97762c {
    u8 unk_0x0[0x60];
    f32 min_grip;  // 0x60
};

static inline void ClipAgainst_l97762c(UnkVec3_l97762c& v, const UnkVec3_l97762c& normal) {
    f32 into = Dot_l97762c(v, normal);
    into = !(into < 0.0f) ? into : 0.0f;
    v.x = v.x - normal.x * into;
    v.y = v.y - normal.y * into;
    v.z = v.z - normal.z * into;
}

static inline void Damp_l97762c(UnkVec3_l97762c& v, f32 k) {
    v.x = v.x - v.x * k;
    v.y = v.y - v.y * k;
    v.z = v.z - v.z * k;
}

void fn_710097762c(const UnkSurface_l97762c* surface, const UnkContactInfo_l97762c* info, UnkNoArg,
                   const UnkMoveConfig_l97762c* config, UnkMoveState_l97762c* state,
                   const UnkIgnoreList_l97762c* ignore) {
    if (state->frame + 1 == info->frame) {
        switch (info->kind) {
        case 1:
            if (surface->grip >= config->min_grip) {
                state->Stop();
                return;
            }
            break;
        case 2: {
            const UnkContact_l97762c* end = state->contacts.end();
            // A contact facing away from the surface, not counting ignored objects.
            const UnkContact_l97762c* away =
                std::find_if(state->contacts.begin(), end, [&](const UnkContact_l97762c& contact) {
                    if (ignore != nullptr) {
                        void* object = contact.ref.Get();
                        if (object != nullptr && ignore->Contains(object))
                            return false;
                    }
                    return Dot_l97762c(contact.normal, surface->normal) < 0.0f;
                });
            if (away != end) {
                state->Stop();
                return;
            }
            break;
        }
        }
    }

    ClipAgainst_l97762c(state->velocity, surface->normal);
    ClipAgainst_l97762c(state->push, surface->normal);
    f32 friction = surface->friction;
    if (friction < 0.0f) {
        f32 k = (friction + 1.0f) * 0.5f;
        Damp_l97762c(state->velocity, k);
        Damp_l97762c(state->push, k);
    }
}
