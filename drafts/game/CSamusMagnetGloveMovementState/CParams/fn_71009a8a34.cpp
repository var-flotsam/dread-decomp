// In the CSamusMagnetGloveMovementState::CParams unit: moves the pending magnet target (+0x208) into the current
// one (+0x1c0) and clears the pending one by assigning it a fresh target. Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "game/UnkTU365.h"
#include "game/malloc.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkMagnetSegment_l9a8a34 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    f32 field_0x10 = -1.0f;

    UnkMagnetSegment_l9a8a34() = default;
    UnkMagnetSegment_l9a8a34& operator=(const UnkMagnetSegment_l9a8a34& other) {
        fn_7100125b44(reinterpret_cast<UnkSegment2D*>(this), reinterpret_cast<const UnkSegment2D*>(&other));
        return *this;
    }
};

struct UnkMagnetPlainVec3_l9a8a34 {
    u32 x, y, z;

    UnkMagnetPlainVec3_l9a8a34& operator=(const UnkMagnetPlainVec3_l9a8a34& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }
};

struct UnkMagnetVec3_l9a8a34 {
    u32 x = 0, y = 0, z = 0;

    UnkMagnetVec3_l9a8a34& operator=(const UnkMagnetVec3_l9a8a34& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }
};

struct UnkMagnetRef_l9a8a34 {
    UnkRefObject* ptr = nullptr;

    UnkMagnetRef_l9a8a34() = default;
    UnkMagnetRef_l9a8a34& operator=(const UnkMagnetRef_l9a8a34& other) {
        fn_710031d208(reinterpret_cast<base::UnkRefHandle*>(this), reinterpret_cast<const base::UnkRefHandle*>(&other));
        return *this;
    }
    ~UnkMagnetRef_l9a8a34() {
        if (UnkRefObject* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

struct UnkMagnetSlot_l9a8a34 {
    UnkRefObject* ptr = nullptr;

    UnkMagnetSlot_l9a8a34() = default;
    UnkMagnetSlot_l9a8a34& operator=(const UnkMagnetSlot_l9a8a34& other) {
        fn_71002a5d1c(reinterpret_cast<UnkAssignedSlot*>(this), reinterpret_cast<const UnkAssignedSlot*>(&other));
        return *this;
    }
    ~UnkMagnetSlot_l9a8a34() {
        if (UnkRefObject* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

struct UnkMagnetTarget_l9a8a34 {
    u32 field_0x0;
    UnkMagnetPlainVec3_l9a8a34 field_0x4;
    UnkMagnetSegment_l9a8a34 field_0x10;
    u32 field_0x24;
    UnkMagnetRef_l9a8a34 field_0x28;
    UnkMagnetSlot_l9a8a34 field_0x30;
    UnkMagnetVec3_l9a8a34 field_0x38;
};
static_assert(sizeof(UnkMagnetTarget_l9a8a34) == 0x48, "layout");

struct UnkMagnetState_l9a8a34 {
    u8 pad_0x0[0x1c0];
    UnkMagnetTarget_l9a8a34 current;  // 0x1c0
    UnkMagnetTarget_l9a8a34 pending;  // 0x208
};
static_assert(__builtin_offsetof(UnkMagnetState_l9a8a34, pending) == 0x208, "layout");

void fn_71009a8a34(UnkMagnetState_l9a8a34* self) {
    self->current = self->pending;
    self->pending = UnkMagnetTarget_l9a8a34{};
}
