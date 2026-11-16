// SLogicPathNode unit: the entry at an index of an array of 0x50-byte polymorphic entries (each an
// array of points and a flag), growing the array up to it with default entries: CRntVector::GrowAt.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "game_s2/UnkItem50.h"
#include "types.h"

struct UnkPathPoint {
    void** vtable;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u8 pad_0x1c[0x4];

    void Destroy() { reinterpret_cast<void (*)(UnkPathPoint*)>(vtable[1])(this); }
};

// Two points the entry clears whenever it is made.
struct UnkPathBounds_l05f810 {
    base::math::CVector3D min;
    base::math::CVector3D max;
};

// An array of points (allocation flags 0xf) and a flag; destroyed through vtable slot 1.
struct UnkPathEntry_l05f810 {
    void** vtable;
    base::global::CRntVector<UnkPathPoint> points;
    bool field_0x30;
    UnkPathBounds_l05f810 bounds;

    UnkPathEntry_l05f810()
        : vtable(g_unk_7101cac938 + 2), points(0xf), field_0x30(true), bounds() {}
    UnkPathEntry_l05f810(UnkPathEntry_l05f810&& other)
        : vtable(g_unk_7101cac938 + 2), points(static_cast<base::global::CRntVector<UnkPathPoint>&&>(other.points)),
          field_0x30(other.field_0x30), bounds() {}
    ~UnkPathEntry_l05f810() { reinterpret_cast<void (*)(UnkPathEntry_l05f810*)>(vtable[1])(this); }
};
static_assert(sizeof(UnkPathEntry_l05f810) == 0x50, "layout");

struct UnkIndexedEntry20;
struct UnkIndexedTable20;

UnkIndexedEntry20* fn_710105f810(UnkIndexedTable20* self, u32 index) {
    auto* entries = reinterpret_cast<base::global::CRntVector<UnkPathEntry_l05f810>*>(self);
    if (entries->field_0x14 && entries->count <= index) {
        // First to the next multiple of the step, then to exactly index + 1 entries.
        u32 cap = entries->capacity;
        if (cap <= index && entries->step != 0) {
            u32 grown = (index / entries->step + 1) * entries->step;
            if (cap < grown) {
                entries->Reserve(grown);
                cap = grown;
            }
        }
        u32 n = index + 1;
        if (cap < n)
            entries->Reserve(n);
        for (u32 i = entries->count; i < n; ++i)
            new (&entries->data[i]) UnkPathEntry_l05f810;
        entries->count = n;
    }
    return reinterpret_cast<UnkIndexedEntry20*>(&entries->data[index]);
}
