// CBillboardComponent unit (tools/tu_map.py unit 1152): whether any of the billboard entries is on
// screen: for each shown entry (a copy of it), whether the render manager sees one of the four corners
// of the square of its radius around its position. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU1161.h"
#include "game/UnkTU315.h"
#include "types.h"

extern void* g_unk_7101a6da30[];
extern void* g_unk_7101a6da10[];

struct UnkVec3Result {
    f32 x;
    f32 y;
    f32 z;
    UnkVec3Result(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkVec3Result(const UnkVec3Result& o) : x(o.x), y(o.y), z(o.z) {}
};

// A vector copied component by component.
struct UnkVec3_l729978 {
    f32 x, y, z;
    UnkVec3_l729978(const UnkVec3_l729978& o) : x(o.x), y(o.y), z(o.z) {}
};

// A pointer copied on its own.
struct UnkHandle_l729978 {
    void* ptr;
    UnkHandle_l729978(const UnkHandle_l729978& o) : ptr(o.ptr) {}
};

// The vtables (reached through the GOT) a copy takes: the entry's and its part's.
struct UnkEntryVtable_l729978 {
    void** vtable;
    UnkEntryVtable_l729978(const UnkEntryVtable_l729978&) : vtable(g_unk_7101a6da30 + 2) {}
};
struct UnkPartVtable_l729978 {
    void** vtable;
    UnkPartVtable_l729978(const UnkPartVtable_l729978&) : vtable(g_unk_7101a6da10 + 2) {}
};

struct UnkItem_l729978;

struct UnkEntryPart_l729978 {
    UnkPartVtable_l729978 vtable;
    u8 field_0x8[0x2c];
    UnkVec3_l729978 field_0x34;
    u8 field_0x40[0xc];
    UnkHandle_l729978 field_0x50;
    UnkHandle_l729978 field_0x58;
};
static_assert(sizeof(UnkEntryPart_l729978) == 0x60, "UnkEntryPart_l729978 layout");

// One of the 0x148-byte entries (destroyed out of line).
struct UnkBillboardEntry_l729978 {
    UnkEntryVtable_l729978 vtable;
    void* actor;  // 0x8
    u8 field_0x10[0x14];
    bool shown;  // 0x24
    u8 field_0x25[0xb];
    UnkVec3_l729978 pos;  // 0x30
    u32 field_0x3c;
    f32 radius;  // 0x40
    u8 field_0x44[0x2b];
    UnkVec3_l729978 field_0x70;
    UnkVec3_l729978 field_0x7c;
    UnkVec3_l729978 field_0x88;
    UnkVec3_l729978 field_0x94;
    UnkVec3_l729978 field_0xa0;
    u64 field_0xb0;
    base::global::CRntVector<UnkItem_l729978*> items;  // 0xb8
    u32 field_0xe0;
    UnkEntryPart_l729978 part;  // 0xe8

    ~UnkBillboardEntry_l729978() asm("fn_7100745b24");
};
static_assert(sizeof(UnkBillboardEntry_l729978) == 0x148, "UnkBillboardEntry_l729978 layout");

struct UnkBillboards_l729978 {
    u8 pad_0x0[0x50];
    UnkBillboardEntry_l729978* entries;  // 0x50
    u8 pad_0x58[0xb8];
    s32 count;  // 0x110
};

static inline bool IsOnScreen(const UnkVec3Result& point) {
    return fn_7100219588(g_unk_7101d47300, &point, 0, 0);
}

bool fn_7100729978(void* object) {
    UnkBillboards_l729978* self = static_cast<UnkBillboards_l729978*>(object);
    if (self->entries) {
        for (s32 i = 0; i < self->count; ++i) {
            UnkBillboardEntry_l729978 entry(self->entries[i]);
            if (entry.shown) {
                UnkVec3Result pos = fn_7100745e24(reinterpret_cast<UnkParent_71007455dc*>(&entry));
                f32 r = entry.radius;
                if (IsOnScreen(UnkVec3Result(r + pos.x, r + pos.y, pos.z)) ||
                    IsOnScreen(UnkVec3Result(pos.x - r, r + pos.y, pos.z)) ||
                    IsOnScreen(UnkVec3Result(r + pos.x, pos.y - r, pos.z)) ||
                    IsOnScreen(UnkVec3Result(pos.x - r, pos.y - r, pos.z)))
                    return true;
            }
        }
    }
    return false;
}
