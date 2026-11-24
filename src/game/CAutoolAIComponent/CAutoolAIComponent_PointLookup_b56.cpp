// CAutoolAIComponent's unit (fn_71003c68a8, fn_71003c6a5c): looks an entry up by position in a flat
// map (values, keys, count at +0x14708) whose keys compare equal within 0.001 per component, then
// either tests the entry's +0x10 object (fn_7100360120) or resets it. Names are placeholders.
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector3D;

// Both bounds tested without short-circuit: with `&&` the compares are emitted the other way round.
static inline bool Near(f32 a, f32 b) {
    f32 lo = a - 0.001f;
    f32 hi = a + 0.001f;
    return (lo <= b) & (hi >= b);
}

static inline bool operator==(const CVector3D& a, const CVector3D& b) {
    return Near(a.x, b.x) && Near(a.y, b.y) && Near(a.z, b.z);
}

struct UnkTimerB56 {
    u8 pad_0x0[0x48];
    u32 start;  // 0x48
    u8 pad_0x4c[0x4];
    u32 value;  // 0x50
};

bool fn_7100360120(UnkTimerB56* self);

struct UnkPointEntryB56 {
    u8 pad_0x0[0x10];
    UnkTimerB56* timer;  // 0x10
};

struct UnkPointMapB56 {
    UnkPointEntryB56** values;
    CVector3D* keys;
    u32 count;

    UnkPointEntryB56** Find(const CVector3D& key) {
        for (u64 i = 0; i != count; ++i) {
            if (keys[i] == key) {
                return &values[i];
            }
        }
        return values + count;
    }

    UnkPointEntryB56*& operator[](const CVector3D& key) {
        u64 i = 0;
        for (; i != count; ++i) {
            if (keys[i] == key) {
                break;
            }
        }
        return values[i];
    }
};

struct UnkAutoolB56 {
    u8 pad_0x0[0x14708];
    UnkPointMapB56 points;  // 0x14708
};

static inline UnkTimerB56* TimerAt(UnkAutoolB56* self, const CVector3D& pos) {
    UnkPointMapB56& map = self->points;
    if (map.Find(pos) == map.values + map.count) {
        return nullptr;
    }
    UnkPointEntryB56* entry = map[pos];
    if (entry == nullptr) {
        return nullptr;
    }
    return entry->timer;
}

bool fn_71003c68a8(UnkAutoolB56* self, const CVector3D& pos) {
    UnkTimerB56* timer = TimerAt(self, pos);
    if (timer == nullptr) {
        return false;
    }
    return fn_7100360120(timer);
}

bool CAutoolAIComponent_ResetEntryAtPoint(UnkAutoolB56* self, const CVector3D& pos) {
    UnkTimerB56* timer = TimerAt(self, pos);
    if (timer == nullptr) {
        return false;
    }
    timer->value = timer->start;
    return true;
}
