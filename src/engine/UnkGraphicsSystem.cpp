// The graphics system: the 0x3183718-byte object behind g_unk_710b372a98 (game/UnkGlobal_710b372a98.h),
// created by fn_7100150b70 (constructor fn_71001c8294, set-up fn_71001c9718) and reached through
// one-line wrappers at 0x7100150bxx-0x7100150fxx. It wraps the device layer (fn_7100159xxx-fn_7100160xxx):
// render states, the viewport, the display modes the device offers, a table of named vectors and
// per-frame state. tools/tu_map.py: unit 259 (0x71001c6960-0x71001d948c, initialiser fn_71001d9350).
// Names and layouts are placeholders.
#include <nn/os.h>

#include "base/UnkCountedResource.h"
#include "base/UnkDefaultAllocator.h"
#include "base/UnkMatrix44.h"
#include "base/UnkVector.h"
#include "base/color/CColor4F.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::color::CColor4F;
using base::global::UnkStringData;
using base::global::UnkStringPool;
using base::math::CVector4D;
using base::math::UnkVector4D;

// A string id moved bitwise (the moved-from id gets the pool's empty string) and released in two
// steps (the original reloads the id after the decrement).
struct UnkGfxStrId {
    UnkStringData* data;

    UnkGfxStrId(UnkGfxStrId&& other) {
        data = other.data;
        other.data = &g_unk_7101d47108->empty;
    }
    ~UnkGfxStrId() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

// A named vector (0x18 bytes). Its move copies the vector component by component, bitwise: the
// original reloads the moved-from name afterwards.
struct UnkGfxNamedVector {
    UnkGfxStrId name;
    CVector4D value;  // 0x8

    UnkGfxNamedVector(UnkGfxNamedVector&& other) : name(static_cast<UnkGfxStrId&&>(other.name)) {
        __builtin_memcpy(&value.x, &other.value.x, sizeof(f32));
        __builtin_memcpy(&value.y, &other.value.y, sizeof(f32));
        __builtin_memcpy(&value.z, &other.value.z, sizeof(f32));
        __builtin_memcpy(&value.w, &other.value.w, sizeof(f32));
    }
};
static_assert(sizeof(UnkGfxNamedVector) == 0x18, "named vector size");

// Named vectors in an array parallel to their 64-bit keys, with a shared count.
struct UnkGfxNamedVectors {
    UnkGfxNamedVector* values;
    u64* keys;  // 0x8
    u32 count;  // 0x10
    u32 capacity;
    u32 step;
    u32 flags;  // 0x1c
    base::UnkAllocator* allocator;  // 0x20
};
static_assert(sizeof(UnkGfxNamedVectors) == 0x28, "named vectors size");

// One display mode the device reports (fn_710015be10): a name and three words.
struct UnkGfxDisplayMode {
    char name[0x80];
    u32 field_0x80;
    u32 field_0x84;
    u32 field_0x88;

    UnkGfxDisplayMode() = default;
    UnkGfxDisplayMode(const UnkGfxDisplayMode& other) {
        __builtin_strcpy(name, other.name);
        field_0x80 = other.field_0x80;
        field_0x84 = other.field_0x84;
        field_0x88 = other.field_0x88;
    }
};
static_assert(sizeof(UnkGfxDisplayMode) == 0x8c, "display mode size");

// One of two 0x14-byte entries (both {1, 0, 0, 0, 1} after construction).
struct UnkGfxScaleEntry {
    f32 value;  // clamped to [0, 1] (fn_71001d2dcc)
    f32 field_0x4;
    f32 field_0x8;
    f32 field_0xc;
    u32 field_0x10;  // the bits of 1.0f after construction; copied to g_unk_7101a1dbcc each frame
};
static_assert(sizeof(UnkGfxScaleEntry) == 0x14, "scale entry size");

// A 0x50-byte slot of the two slot arrays (8192 at +0x128, 128 at +0x3178178).
struct UnkGfxSlot {
    s32 id;  // -1 when free
    u8 unk_0x4[0xc];
    u32 field_0x10;
    u8 unk_0x14[0x4];
    u64 field_0x18;
    u64 field_0x20;
    u32 field_0x28;
    u8 unk_0x2c[0x4];
    u64 field_0x30;
    u32 field_0x38;
    u8 unk_0x3c[0x4];
    u64 field_0x40;
    bool field_0x48;
    u8 unk_0x49[0x7];

    void Init() {
        id = -1;
        field_0x10 = 0;
        field_0x30 = 0;
        field_0x38 = 0;
        field_0x40 = 0;
        field_0x48 = false;
        field_0x28 = 0;
        field_0x18 = 0;
        field_0x20 = 0;
    }
};
static_assert(sizeof(UnkGfxSlot) == 0x50, "slot size");

// A 0xc18-byte batch of the two batch arrays (8192 each at +0x118168 and +0x1948170).
struct UnkGfxBatch {
    u64 field_0x0;
    u64 field_0x8;
    u8 unk_0x10[0xc00];
    u32 field_0xc10;
    u8 unk_0xc14[0x4];

    void Init() {
        field_0xc10 = 0;
        field_0x0 = 0;
        field_0x8 = 0;
    }
};
static_assert(sizeof(UnkGfxBatch) == 0xc18, "batch size");

// A word followed by 0x10004 bytes the constructor leaves alone (seven of them from +0xa0128).
struct UnkGfxCountedBlock {
    u32 count;
    u8 unk_0x4[0x10004];
};
static_assert(sizeof(UnkGfxCountedBlock) == 0x10008, "counted block size");

// A pointer and a count (two arrays of them at +0x317ab40 and +0x317abb8).
struct UnkGfxCountedRef {
    u64 ptr;
    u32 count;
    u8 unk_0xc[0x4];
};

// A 0x158-byte record (100 of them from +0x317afb0): two zeroed blocks around a word the
// constructor leaves alone.
struct UnkGfxRecord {
    u64 field_0x0[10];
    u8 unk_0x50[0x8];
    u64 field_0x58[32];

    void Init() {
        for (u32 i = 0; i < 10; ++i)
            field_0x0[i] = 0;
        for (u32 i = 0; i < 32; ++i)
            field_0x58[i] = 0;
    }
};
static_assert(sizeof(UnkGfxRecord) == 0x158, "record size");

// An entry of the array at +0x317a9e0 (fn_71001cdba4 in src/game_lo/fn_71001cdba4.cpp appends to
// it): a count-prefixed list of up to 16 pairs of words and one 64-bit value.
struct UnkGfxPairList {
    u32 count;
    u32 items[16][2];  // 0x4
    u64 field_0x88;
};
static_assert(sizeof(UnkGfxPairList) == 0x90, "pair list size");

struct UnkGraphicsSystem {
    void** vtable;  // 0x7101a1e6e0 + 0x10
    UnkGfxNamedVectors named_vectors;  // 0x8
    u64 field_0x30;
    f32 field_0x38;
    f32 field_0x3c;
    f32 field_0x40[5];
    f32 field_0x54;
    f32 field_0x58;
    f32 field_0x5c;  // 2.2, a gamma
    base::UnkVector<UnkGfxDisplayMode> display_modes;  // 0x60
    base::UnkVector<u8> field_0x88;
    base::UnkVector<u8> field_0xb0;  // reserved by fn_71000231f4 (element type unknown)
    u8 unk_0xd8[0x20];
    u32 field_0xf8;
    u8 unk_0xfc[0x24];
    u32 field_0x120;
    u8 unk_0x124[0x4];
    UnkGfxSlot slots[8192];  // 0x128
    UnkGfxCountedBlock blocks[7];  // 0xa0128
    u32 field_0x110160;
    u8 unk_0x110164[0x8004];
    UnkGfxBatch batches[8192];  // 0x118168
    u32 field_0x1948168;
    u8 unk_0x194816c[0x4];
    UnkGfxBatch batches2[8192];  // 0x1948170
    u32 field_0x3178170;
    u8 unk_0x3178174[0x4];
    UnkGfxSlot slots2[128];  // 0x3178178
    u64 field_0x317a978;
    u32 field_0x317a980;
    u8 unk_0x317a984[0x4];
    u8 field_0x317a988[0x10];  // constructed by fn_7100168f10
    u32 field_0x317a998;
    u32 field_0x317a99c;  // the draw mode (fn_71001cb604)
    u8 field_0x317a9a0[0x40];  // constructed by fn_7100216dfc
    base::UnkVector<UnkGfxPairList> pair_lists;  // 0x317a9e0
    // The viewport (src/game_lo/fn_71001cdd74.cpp): x, y, width, height and the depth range.
    s32 viewport[4];  // 0x317aa08
    f32 viewport_min_depth;
    f32 viewport_max_depth;
    void* field_0x317aa20;  // the camera (fn_71001cb604)
    u64 field_0x317aa28[7];
    u32 field_0x317aa60;
    bool field_0x317aa64;
    u8 unk_0x317aa65[0x3];
    u32 field_0x317aa68;
    u8 unk_0x317aa6c[0x4];
    base::UnkAllocator* field_0x317aa70;
    u32 field_0x317aa78;
    u32 field_0x317aa7c;
    base::UnkVector<u64> field_0x317aa80;
    u64 field_0x317aaa8;
    u64 field_0x317aab0;
    UnkGfxScaleEntry scales[2];  // 0x317aab8
    u64 field_0x317aae0[7];
    f32 field_0x317ab18;
    f32 field_0x317ab1c;
    f32 field_0x317ab20;
    u32 field_0x317ab24[5];
    u32 field_0x317ab38;  // the number of refs at +0x317ab40 (7)
    u32 field_0x317ab3c;  // the number of refs at +0x317abb8 (6)
    UnkGfxCountedRef field_0x317ab40[7];
    u32 field_0x317abb0;
    u8 unk_0x317abb4[0x4];
    UnkGfxCountedRef field_0x317abb8[6];
    u32 field_0x317ac18;
    f32 field_0x317ac1c;
    u64 field_0x317ac20[4];
    f32 field_0x317ac40;
    u32 field_0x317ac44[20];
    f32 field_0x317ac94;
    u64 field_0x317ac98[2];
    CVector4D field_0x317aca8;  // the clear colour
    u64 field_0x317acb8[2];
    UnkMatrix44 matrices[4];  // 0x317acc8, identities after construction
    u64 field_0x317adc8;
    u32 field_0x317add0;
    u8 unk_0x317add4[0x4];
    u64 field_0x317add8[3];
    u64 field_0x317adf0[2];
    u64 field_0x317ae00[4];
    u64 field_0x317ae20;  // the buffer bound for the overlay's first uniform block
    u8 unk_0x317ae28[0x8];
    u64 field_0x317ae30[32];
    u64 field_0x317af30[2];
    u32 field_0x317af40;
    f32 field_0x317af44;
    u32 field_0x317af48;
    u8 unk_0x317af4c[0x4];
    u16* field_0x317af50;  // quad indices: 0, 1, 2, 0, 2, 3 for each four vertices (fn_71001c9718)
    u64 field_0x317af58;
    u32 field_0x317af60;
    f32 field_0x317af64;
    void* field_0x317af68;  // the overlay's program state
    void* field_0x317af70;  // the overlay's program
    s32 field_0x317af78;
    bool field_0x317af7c;
    u8 unk_0x317af7d[0x3];
    base::global::CStrId field_0x317af80;
    u64 field_0x317af88;
    u32 field_0x317af90;
    u8 unk_0x317af94[0x4];
    u64 field_0x317af98[3];
    UnkGfxRecord records[100];  // 0x317afb0
    u32 field_0x3183610;
    u8 unk_0x3183614[0x14];
    UnkGfxNamedVectors field_0x3183628;
    s32 field_0x3183650;
    f32 field_0x3183654;
    u32 field_0x3183658;
    u32 field_0x318365c;
    bool field_0x3183660[7];
    bool field_0x3183667;  // the overlay mode (fn_71001cafa0)
    bool field_0x3183668;
    bool field_0x3183669;
    bool field_0x318366a;
    bool field_0x318366b;
    bool field_0x318366c[6];
    bool field_0x3183672;
    bool field_0x3183673;
    bool field_0x3183674;
    bool field_0x3183675[7];
    bool field_0x318367c;
    bool field_0x318367d;
    u8 unk_0x318367e[0x2];
    u64 field_0x3183680[6];
    u32 field_0x31836b0;
    f32 field_0x31836b4[3];
    u64 field_0x31836c0[4];
    u32 field_0x31836e0;
    f32 field_0x31836e4[4];
    u32 field_0x31836f4[3];
    void* light_proxies[3];  // 0x3183700: the omni, spot and segment light proxy models
};
#define UNK_GFX_AT(field, offset) static_assert(__builtin_offsetof(UnkGraphicsSystem, field) == (offset), #field)
UNK_GFX_AT(display_modes, 0x60);
UNK_GFX_AT(slots, 0x128);
UNK_GFX_AT(batches, 0x118168);
UNK_GFX_AT(batches2, 0x1948170);
UNK_GFX_AT(slots2, 0x3178178);
UNK_GFX_AT(pair_lists, 0x317a9e0);
UNK_GFX_AT(field_0x317aa68, 0x317aa68);
UNK_GFX_AT(field_0x317aa80, 0x317aa80);
UNK_GFX_AT(scales, 0x317aab8);
UNK_GFX_AT(field_0x317ab40, 0x317ab40);
UNK_GFX_AT(field_0x317abb8, 0x317abb8);
UNK_GFX_AT(field_0x317ac94, 0x317ac94);
UNK_GFX_AT(field_0x317aca8, 0x317aca8);
UNK_GFX_AT(matrices, 0x317acc8);
UNK_GFX_AT(field_0x317add8, 0x317add8);
UNK_GFX_AT(field_0x317ae20, 0x317ae20);
UNK_GFX_AT(field_0x317af68, 0x317af68);
UNK_GFX_AT(field_0x317af80, 0x317af80);
UNK_GFX_AT(records, 0x317afb0);
UNK_GFX_AT(field_0x3183628, 0x3183628);
UNK_GFX_AT(field_0x3183667, 0x3183667);
UNK_GFX_AT(field_0x3183672, 0x3183672);
UNK_GFX_AT(field_0x318367c, 0x318367c);
UNK_GFX_AT(field_0x3183680, 0x3183680);
UNK_GFX_AT(field_0x31836e4, 0x31836e4);
#undef UNK_GFX_AT
static_assert(sizeof(UnkGraphicsSystem) == 0x3183718, "graphics system size");

// The device layer.
bool fn_7100132a10();  // whether the console is docked (the 1.0 scale) or handheld (0.8)
u32 fn_7100132f80();   // the display width
u32 fn_7100132f90();   // the display height
bool fn_71001598c0();
u32 fn_71001598d0(u32 max, u32* sizes);  // the display sizes (width, height pairs) and their count
void fn_7100159910();
void fn_7100159920();
void fn_7100159ac0();
void fn_7100159bf0(s32 a, s32 b);
void fn_7100159c70();
void fn_710015bd90(s32 x, s32 y, s32 width, s32 height);
void fn_710015bdc0(s32 a, s32 b);
u32 fn_710015be00();  // the number of display modes
bool fn_710015be10(u32 index, UnkGfxDisplayMode* mode);
void fn_710015be40(u32 value);
void fn_710016eae4();

// Render state setters of the device layer (the last argument is always true here).
void fn_71001606f0(s32 a, s32 b, bool c);
void fn_7100160810(s32 a, s32 b, bool c);
void fn_7100160890(s32 a, s32 b, bool c);
void fn_7100160990(s32 a, s32 b, bool c);
void fn_7100160ab0(s32 a, s32 b, bool c);
void fn_7100160bd0(s32 a, f32 value, bool c);
void fn_7100160c90(s32 a, f32 value, bool c);
void fn_7100160d50(s32 a, s32 b, bool c);
void fn_7100160e50(s32 a, s32 b, bool c);
void fn_710015a0e0(u32 value);
void fn_710015a260(u32 value);
void fn_710015a3e0(u32 value);
void fn_710015a500(u32 a, u32 b);
void fn_710015a680(u32 value);
void fn_710015a760(u32 value);
void fn_710015a860(u32 value);
void fn_710015a8c0(u32 value);
void fn_710015aa20(u32 value);
void fn_710015ab00(u32 value);
void* fn_710015db80();
void fn_710015d900(void* a, u32 b, u32 c);
// Sets `count` vectors (four floats each, or one float) of a uniform at `location` of a program state.
void fn_710015e1f0(void* state, u32 stage, u64 location, u32 count, const f32* values);
// The location of uniform `id` of a program's stage, if it has one.
bool fn_71001aa0b4(void* program, u32 stage, u32 id, u64* location);

// The program registry (reached through the GOT; src/game_lo/fn_710014c790.cpp) and the overlay
// program's key.
struct UnkRegistry_710014c790;
extern UnkRegistry_710014c790* unk_710b372d68;
void* fn_7100169be4(UnkRegistry_710014c790* registry, const void* key);
extern __attribute__((visibility("hidden"))) u8 g_unk_716e595bb8[];
extern __attribute__((visibility("hidden"))) f32 g_unk_7101a1e710[4];  // the overlay's tint

// The resolution scale (1 docked, 0.8 handheld) for both axes, and the docked state it was set for.
struct UnkGfxResolutionScale {
    f32 x;
    f32 y;
};
extern __attribute__((visibility("hidden"))) UnkGfxResolutionScale g_unk_7101a1e75c;
extern __attribute__((visibility("hidden"))) bool g_unk_716e595b70;
extern __attribute__((visibility("hidden"))) base::global::CStrId g_unk_716e595b90;
extern __attribute__((visibility("hidden"))) s32 g_unk_716e595b78;  // flips between 0 and 1 each frame
extern bool g_unk_7101a1e60c;
extern u32 g_unk_7101a1dbcc;

// The object whose float at +0x2c fn_71001cc464 eases towards (through the GOT).
struct UnkGfxFrameSource {
    u8 unk_0x0[0x2c];
    f32 field_0x2c;
};
extern UnkGfxFrameSource* g_unk_7101d47328;

// Values the device layer reads (through the GOT), changed under the mutex below.
extern u64 g_unk_716e554928;
extern u64 g_unk_716e554930;
extern __attribute__((visibility("hidden"))) nn::os::MutexType g_unk_716e595cb8;

// Sets the clear colour (the colour's components as a vector).
void fn_71001cc3a8(UnkGraphicsSystem* self, const CColor4F* color) {
    UnkVector4D value = fn_7100030878(color);
    fn_710007b518(&self->field_0x317aca8, &value);
}

// The resolution scale: 1 when docked, 0.8 in handheld mode.
f32 fn_71001d0a94(UnkGraphicsSystem* self) { return fn_7100132a10() ? 1.0f : 0.8f; }

// Sets one of two values the device layer reads, under its mutex, when it changes and the device
// layer allows it.
void fn_71001d2078(UnkGraphicsSystem* self, u64 value) {
    bool ok = fn_71001598c0();
    u64* target = &g_unk_716e554928;
    if (*target != value && ok) {
        fn_710006691c(&g_unk_716e595cb8);
        *target = value;
        fn_7100066920(&g_unk_716e595cb8);
    }
}

void fn_71001d20e0(UnkGraphicsSystem* self, u64 value) {
    bool ok = fn_71001598c0();
    u64* target = &g_unk_716e554930;
    if (*target != value && ok) {
        fn_710006691c(&g_unk_716e595cb8);
        *target = value;
        fn_7100066920(&g_unk_716e595cb8);
    }
}

// Sets scale `index` (0 or 1), clamped to [0, 1].
bool fn_71001d2dcc(UnkGraphicsSystem* self, u32 index, f32 value) {
    if (index <= 1) {
        self->scales[index].value = value <= 0.0f ? 0.0f : (value >= 1.0f ? 1.0f : value);
        return true;
    }
    return false;
}

// Sets the value of the named vector with the given key (the entry past the end when there is
// none, as in the original).
bool fn_71001d7024(UnkGraphicsSystem* self, const u64* key, const CVector4D* value) {
    UnkGfxNamedVectors& t = self->named_vectors;
    u64 n = t.count;
    u64 i = n;
    for (u64 j = 0; j != n; ++j) {
        if (t.keys[j] == *key) {
            i = j;
            break;
        }
    }
    UnkGfxNamedVector* found = &t.values[i];
    if (!found)
        return false;
    fn_710007b518(&t.values[i].value, value);
    return true;
}

// The display size closest to 16:9 (its index in `index`), stopping at the first one larger than
// 1280x720 once one was seen; false when there is none.
static inline __attribute__((always_inline)) bool FindDisplaySize(const u32* sizes, u32 n, s32* index) {
    bool found = false;
    f32 best = 3.4028235e+38f;
    for (u32 i = 0; i < n; ++i) {
        u32 width = sizes[i * 2];
        u32 height = sizes[i * 2 + 1];
        if (height > 720 && found && width > 1280)
            return found;
        f32 diff = __builtin_fabsf(1.7777778f - (f32)width / (f32)height);
        if (diff <= best || !found) {
            best = diff;
            *index = i;
        }
        found = true;
    }
    return found;
}

// NON_MATCHING(minor): where the search stops at a size larger than 1280x720, the original tests the
// found flag again (always set there) before returning the index; ours returns it directly.
s32 fn_71001d2b8c(UnkGraphicsSystem* self) {
    u32 sizes[0x100];
    u32 n = fn_71001598d0(0x20, sizes);
    s32 index = 0;
    if (FindDisplaySize(sizes, n, &index))
        return index;
    return -1;
}

// Per-frame start: the viewport at the scaled display size, the counter at +0x317aa68 counted down
// while +0x3183668 is set, and the value at +0x3c eased towards the frame source's.
void fn_71001cc464(UnkGraphicsSystem* self) {
    f32 display_width = (f32)fn_7100132f80();
    s32 width = (s32)(g_unk_7101a1e75c.x * display_width);
    f32 display_height = (f32)fn_7100132f90();
    s32 height = (s32)(g_unk_7101a1e75c.x * display_height);
    fn_710015bd90(0, 0, width, height);
    fn_7100159910();
    fn_7100159bf0(0, 0);
    if (self->field_0x3183668 && self->field_0x317aa68)
        --self->field_0x317aa68;
    fn_710016eae4();
    UnkGfxFrameSource* source = g_unk_7101d47328;
    f32 value = self->field_0x3c;
    f32 target = source->field_0x2c;
    if (value > 0.0f) {
        f32 step;
        if (self->field_0x318367c) {
            step = 0.0f;
        } else {
            step = target - value;
            step = step > 0.0f ? step : 0.0f;
        }
        target = value + step * self->field_0x38;
    }
    self->field_0x3c = target;
}

// Per-frame end: the resolution scale follows the docked state, the device state is reset and
// the frame parity flips.
void fn_71001d4204(UnkGraphicsSystem* self) {
    bool docked = fn_7100132a10();
    if (docked != g_unk_716e595b70) {
        g_unk_716e595b70 = docked;
        f32 scale = docked ? 1.0f : 0.8f;
        g_unk_7101a1e75c.x = scale;
        g_unk_7101a1e75c.y = scale;
    }
    fn_710015bdc0(0, 0);
    self->field_0x3183672 = false;
    fn_7100159920();
    { base::global::CStrId id(g_unk_716e595b90); }
    if (g_unk_7101a1e60c) {
        fn_7100159c70();
        g_unk_7101a1e60c = false;
    }
    g_unk_7101a1dbcc = self->scales[0].field_0x10;
    g_unk_716e595b78 = 1 - g_unk_716e595b78;
}

// Rebuilds the list of display modes from the device.
void fn_71001d4320(UnkGraphicsSystem* self) {
    fn_7100159ac0();
    fn_710015be40(1);
    self->display_modes.count = 0;
    u32 n = fn_710015be00();
    for (u32 i = 0; i != n; ++i) {
        UnkGfxDisplayMode mode;
        if (fn_710015be10(i, &mode))
            self->display_modes.PushBack(UnkGfxDisplayMode(mode));
    }
}

// Grows the named vectors' arrays to `capacity` entries (the values are moved, the keys copied).
// NON_MATCHING(minor): the key copy loop compares its 64-bit index with the count in 64 bits; the
// original compares the low 32 bits (the same result, the index never exceeds the 32-bit count).
void fn_71001d6c58(UnkGfxNamedVectors* self, u32 capacity) {
    if (self->capacity < capacity) {
        UnkGfxNamedVector* values =
            static_cast<UnkGfxNamedVector*>(self->allocator->vf_0x40((u64)capacity * sizeof(UnkGfxNamedVector), 8, self->flags));
        u64* keys = static_cast<u64*>(self->allocator->vf_0x40((u64)capacity * sizeof(u64), 8, self->flags));
        if (self->values) {
            UnkGfxNamedVector* src = self->values;
            UnkGfxNamedVector* dst = values;
            for (u32 i = 0; i < self->count; ++i, ++src, ++dst) {
                new (dst) UnkGfxNamedVector(static_cast<UnkGfxNamedVector&&>(*src));
                src->~UnkGfxNamedVector();
            }
            self->allocator->vf_0x48(self->values);
        }
        if (u64* old_keys = self->keys) {
            for (u32 i = 0; i < self->count; ++i)
                __builtin_memcpy(&keys[i], &old_keys[i], sizeof(u64));
            self->allocator->vf_0x48(self->keys);
        }
        self->values = values;
        self->keys = keys;
        self->capacity = capacity;
    }
}

// Selects the overlay program and binds it (not decompiled yet).
void fn_71001cc608(UnkGraphicsSystem* self, void* program);

// Drops the overlay program: the device's bindings are cleared (when it has any), then the program
// state and the program are forgotten.
static inline void ResetOverlayProgram(UnkGraphicsSystem* self) {
    if (fn_710015db80()) {
        fn_710015d900(nullptr, 1, 1);
        __builtin_memset(self->field_0x317add8, 0, sizeof(self->field_0x317add8));
        // The four words at +0x317ae00 and the uniform buffer after them.
        __builtin_memset(self->field_0x317ae00, 0, sizeof(self->field_0x317ae00) + sizeof(u64));
        __builtin_memset(self->field_0x317ae30, 0, sizeof(self->field_0x317ae30));
    }
    self->field_0x317af68 = nullptr;
    self->field_0x317af70 = nullptr;
}

// The fn_71001cdd74 (src/game_lo/fn_71001cdd74.cpp) view of this object.
struct UnkStruct_71001cdd74;

// Switches the overlay mode (2D drawing over the scene) on or off, or refreshes it when `refresh`
// is set: the render states for each mode, and on entry the overlay program and its uniforms.
// NON_MATCHING(minor): the declaration in src/game_lo/fn_71001cdd74.cpp types the object as that
// file's struct and the two flags as words (the original takes bools; it reads bit 0 of each, as
// `& 1` does here). With words Clang tests `refresh` before comparing the mode and lays the leaving
// path out first, and through the pointer cast the large offsets no longer share base registers.
// With bools and an UnkGraphicsSystem* it differs only in the scheduling of the reset stores (96%).
void fn_71001cafa0(UnkStruct_71001cdd74* owner, s32 enable_arg, s32 refresh, s32 program_key) {
    UnkGraphicsSystem* self = reinterpret_cast<UnkGraphicsSystem*>(owner);
    bool enable = enable_arg & 1;
    bool was = self->field_0x3183667;
    if (was != enable || (refresh & 1)) {
        self->field_0x3183667 = enable;
        if (enable) {
            if (self->field_0x317af78 != program_key)
                self->field_0x317af70 = nullptr;
            self->field_0x317af78 = program_key;
            self->field_0x3183650 = -1;
            fn_71001cc608(self, fn_7100169be4(unk_710b372d68, g_unk_716e595bb8));
            fn_7100160810(0, 0, true);
            fn_7100160890(0, 0, true);
            fn_7100160ab0(0, 2, true);
            fn_7100160990(0, 2, true);
            fn_7100160d50(0, 0, true);
            fn_7100160bd0(0, 0.0f, true);
            fn_7100160e50(0, 0, true);
            fn_7100160c90(0, 0.0f, true);
            fn_71001606f0(1, 0, true);
            fn_71001606f0(2, 0, true);
            fn_71001606f0(0, 1, true);
            fn_710015a3e0(1);
            fn_710015a760(0);
            fn_710015ab00(3);
            fn_710015a860(0);
            fn_710015a8c0(0);
            fn_710015aa20(0);
            fn_710015a0e0(1);
            fn_710015a260(1);
            if (self->field_0x317ae20) {
                const f32 values[4] = {0.0f, 0.0f, 1.0f, 1.0f};
                fn_710015e1f0(self->field_0x317af68, 0, self->field_0x317ae20, 4, values);
            }
            u64 location;
            if (fn_71001aa0b4(self->field_0x317af70, 1, 0x5a, &location)) {
                const f32 values[4] = {1.0f, 0.0f, 0.0f, 0.0f};
                fn_710015e1f0(self->field_0x317af68, 1, location, 4, values);
            }
            if (fn_71001aa0b4(self->field_0x317af70, 1, 0x5b, &location)) {
                const f32 values[4] = {1.0f, 1.0f / self->field_0x5c, 0.0f, 0.0f};
                fn_710015e1f0(self->field_0x317af68, 1, location, 4, values);
            }
            if (fn_71001aa0b4(self->field_0x317af70, 1, 0x74, &location)) {
                f32 value = self->field_0x3c;
                fn_710015e1f0(self->field_0x317af68, 1, location, 1, &value);
            }
            if (fn_71001aa0b4(self->field_0x317af70, 1, 0xd2, &location))
                fn_710015e1f0(self->field_0x317af68, 1, location, 4, g_unk_7101a1e710);
        } else {
            if (was != enable && !self->field_0x317af7c && self->field_0x317af70)
                ResetOverlayProgram(self);
            fn_7100160ab0(0, 0, true);
            fn_7100160990(0, 0, true);
            fn_7100160d50(0, 0, true);
            fn_7100160bd0(0, 0.0f, true);
            fn_7100160e50(0, 0, true);
            fn_7100160c90(0, 0.0f, true);
            fn_71001606f0(0, 0, true);
            fn_71001606f0(1, 0, true);
            fn_71001606f0(2, 0, true);
            fn_710015a3e0(0);
            fn_710015a760(0);
            fn_710015ab00(3);
            fn_710015a860(1);
            fn_710015a8c0(5);
            fn_710015aa20(1);
            fn_710015a680(0);
            fn_710015a500(1, 0);
            fn_710015a0e0(1);
            fn_710015a260(1);
        }
    }
}

// The vtable (reached through the GOT) and the reserve of the vector at +0xb0.
extern void* unk_7101a1e6e0[];
void fn_71000231f4(base::UnkVector<u8>* vector, u32 capacity);
void fn_7100168f10(void* self);
void fn_7100216dfc(void* self);
// The device's capabilities (fn_71001598a0 sets all three).
struct UnkGfxCaps {
    bool field_0x0;
    bool field_0x1;
    bool field_0x2;
    u8 unk_0x3;
};
void fn_71001598a0(UnkGfxCaps* caps);

// An identity matrix, written out.
static inline void InitIdentity(UnkMatrix44* m) {
    for (u32 i = 1; i < 16; ++i)
        m->m[i] = 0.0f;
    m->m[0] = 1.0f;
    m->m[5] = 1.0f;
    m->m[10] = 1.0f;
    m->m[15] = 1.0f;
}

// The constructor (fn_7100150b70 allocates the object and calls it).
// NON_MATCHING(minor): the same stores and calls, the plain fields in another order (the original
// interleaves them, stores some zeroed ranges twice and unrolls the array loops), the zeroing at
// +0x317ae30 as a memset call, and fn_710007fdb4 duplicated on the two paths of the pool check.
void fn_71001c8294(UnkGraphicsSystem* self) {
    self->vtable = &unk_7101a1e6e0[2];
    UnkGfxNamedVectors& nv = self->named_vectors;
    nv.values = nullptr;
    nv.keys = nullptr;
    nv.count = 0;
    nv.capacity = 0;
    nv.step = 10;
    nv.flags = 0xe;
    nv.allocator = fn_7100083990();
    self->field_0x30 = 0;
    self->field_0x38 = 1.0f;
    self->field_0x3c = 0.0f;
    for (u32 i = 0; i < 5; ++i)
        self->field_0x40[i] = 0.0f;
    self->field_0x54 = 1.0f;
    self->field_0x58 = 0.0f;
    self->field_0x5c = 2.2f;

    // The display modes: no growth step, room for 128.
    base::UnkVector<UnkGfxDisplayMode>& modes = self->display_modes;
    modes.data = nullptr;
    modes.count = 0;
    modes.capacity = 0;
    modes.step = 0;
    modes.field_0x14 = false;
    modes.flags = 0x18;
    modes.allocator = fn_7100083990();
    if (modes.capacity < 0x80) {
        UnkGfxDisplayMode* p = static_cast<UnkGfxDisplayMode*>(
            modes.allocator->vf_0x40(0x80 * sizeof(UnkGfxDisplayMode), 8, modes.flags));
        if (modes.data) {
            __builtin_memcpy(p, modes.data, (u64)modes.count * sizeof(UnkGfxDisplayMode));
            modes.allocator->vf_0x48(modes.data);
        }
        modes.data = p;
        modes.capacity = 0x80;
    }

    base::UnkVector<u8>& v88 = self->field_0x88;
    v88.data = nullptr;
    v88.count = 0;
    v88.capacity = 0;
    v88.step = 10;
    v88.field_0x14 = true;
    v88.flags = 0xf;
    v88.allocator = fn_7100083990();
    base::UnkVector<u8>& vb0 = self->field_0xb0;
    vb0.data = nullptr;
    vb0.count = 0;
    vb0.capacity = 0;
    vb0.step = 10;
    vb0.field_0x14 = true;
    vb0.flags = 0x38;
    vb0.allocator = fn_7100083990();
    fn_71000231f4(&vb0, 0x18);

    self->field_0xf8 = 0;
    self->field_0x120 = 0;
    for (u32 i = 0; i < 8192; ++i)
        self->slots[i].Init();
    for (u32 i = 0; i < 7; ++i)
        self->blocks[i].count = 0;
    self->field_0x110160 = 0;
    for (u32 i = 0; i < 8192; ++i)
        self->batches[i].Init();
    self->field_0x1948168 = 0;
    for (u32 i = 0; i < 8192; ++i)
        self->batches2[i].Init();
    self->field_0x3178170 = 0;
    for (u32 i = 0; i < 128; ++i)
        self->slots2[i].Init();
    self->field_0x317a978 = 0;
    self->field_0x317a980 = 0;
    fn_7100168f10(self->field_0x317a988);
    self->field_0x317a998 = 0x1f;
    self->field_0x317a99c = 0;
    fn_7100216dfc(self->field_0x317a9a0);

    // The pair lists: no growth step, room for 8 (moved as fn_71001cdba4 copies them).
    base::UnkVector<UnkGfxPairList>& lists = self->pair_lists;
    lists.data = nullptr;
    lists.count = 0;
    lists.capacity = 0;
    lists.step = 0;
    lists.field_0x14 = false;
    lists.flags = 0xf;
    lists.allocator = fn_7100083990();
    if (lists.capacity < 8) {
        UnkGfxPairList* p =
            static_cast<UnkGfxPairList*>(lists.allocator->vf_0x40(8 * sizeof(UnkGfxPairList), 8, lists.flags));
        if (UnkGfxPairList* src = lists.data) {
            UnkGfxPairList* dst = p;
            for (u32 i = 0; i < lists.count; ++i, ++src, ++dst) {
                __builtin_memcpy(dst->items, src->items, (u64)src->count * sizeof(src->items[0]));
                dst->count = src->count;
                dst->field_0x88 = src->field_0x88;
                src->count = 0;
            }
            lists.allocator->vf_0x48(lists.data);
        }
        lists.data = p;
        lists.capacity = 8;
    }

    for (u32 i = 0; i < 4; ++i)
        self->viewport[i] = 0;
    self->viewport_min_depth = 0.0f;
    self->viewport_max_depth = 1.0f;
    self->field_0x317aa20 = nullptr;
    for (u32 i = 0; i < 7; ++i)
        self->field_0x317aa28[i] = 0;
    self->field_0x317aa60 = 0;
    self->field_0x317aa64 = true;
    self->field_0x317aa68 = 0;
    self->field_0x317aa70 = fn_7100083990();
    self->field_0x317aa78 = 0xe;
    self->field_0x317aa7c = 0;

    // A vector of 64-bit values, resized to 11 zeroes.
    base::UnkVector<u64>& words = self->field_0x317aa80;
    words.data = nullptr;
    words.count = 0;
    words.capacity = 0;
    words.step = 10;
    words.field_0x14 = true;
    words.flags = 0xe;
    words.allocator = fn_7100083990();
    self->field_0x317aaa8 = 0;
    self->field_0x317aab0 = 0;
    if (words.count < 11) {
        u64* p;
        if (words.capacity < 11) {
            p = static_cast<u64*>(words.allocator->vf_0x40(11 * sizeof(u64), 8, words.flags));
            if (words.data) {
                __builtin_memcpy(p, words.data, (u64)words.count * sizeof(u64));
                words.allocator->vf_0x48(words.data);
            }
            words.capacity = 11;
            words.data = p;
        } else {
            p = words.data;
        }
        __builtin_memset(p + words.count, 0, (u64)(11 - words.count) * sizeof(u64));
        words.count = 11;
    } else if (words.count != 11) {
        words.count = 11;
    }

    for (u32 i = 0; i < 2; ++i) {
        UnkGfxScaleEntry& e = self->scales[i];
        e.value = 1.0f;
        e.field_0x4 = 0.0f;
        e.field_0x8 = 0.0f;
        e.field_0xc = 0.0f;
        e.field_0x10 = 0x3f800000;  // 1.0f
    }
    for (u32 i = 0; i < 7; ++i)
        self->field_0x317aae0[i] = 0;
    self->field_0x317ab18 = 1.0f;
    self->field_0x317ab1c = 840.0f;
    self->field_0x317ab20 = 1000.0f;
    for (u32 i = 0; i < 5; ++i)
        self->field_0x317ab24[i] = 0;
    self->field_0x317ab38 = 7;
    self->field_0x317ab3c = 6;
    for (u32 i = 0; i < 7; ++i) {
        self->field_0x317ab40[i].ptr = 0;
        self->field_0x317ab40[i].count = 0;
    }
    self->field_0x317abb0 = 0;
    for (u32 i = 0; i < 6; ++i) {
        self->field_0x317abb8[i].ptr = 0;
        self->field_0x317abb8[i].count = 0;
    }
    self->field_0x317ac18 = 0;
    self->field_0x317ac1c = 1.0f;
    for (u32 i = 0; i < 4; ++i)
        self->field_0x317ac20[i] = 0;
    self->field_0x317ac40 = 1.0f;
    for (u32 i = 0; i < 20; ++i)
        self->field_0x317ac44[i] = 0;
    self->field_0x317ac94 = 100.0f;
    self->field_0x317ac98[0] = 0;
    self->field_0x317ac98[1] = 0;
    self->field_0x317aca8.x = 0.0f;
    self->field_0x317aca8.y = 0.0f;
    self->field_0x317aca8.z = 0.0f;
    self->field_0x317aca8.w = 0.0f;
    self->field_0x317acb8[0] = 0;
    self->field_0x317acb8[1] = 0;
    for (u32 i = 0; i < 4; ++i)
        InitIdentity(&self->matrices[i]);
    self->field_0x317adc8 = 0;
    self->field_0x317add0 = 0;
    for (u32 i = 0; i < 3; ++i)
        self->field_0x317add8[i] = 0;
    for (u32 i = 0; i < 2; ++i)
        self->field_0x317adf0[i] = 0;
    for (u32 i = 0; i < 4; ++i)
        self->field_0x317ae00[i] = 0;
    self->field_0x317ae20 = 0;
    for (u32 i = 0; i < 32; ++i)
        self->field_0x317ae30[i] = 0;
    for (u32 i = 0; i < 2; ++i)
        self->field_0x317af30[i] = 0;
    // +0x317ae18 is the last of the four words at +0x317ae00.
    self->field_0x317af40 = 0;
    self->field_0x317af44 = 1.0f;
    self->field_0x317af48 = 4000;
    self->field_0x317af50 = nullptr;
    self->field_0x317af58 = 0;
    self->field_0x317af60 = 0;
    self->field_0x317af64 = 1.0f;
    self->field_0x317af68 = nullptr;
    self->field_0x317af70 = nullptr;
    self->field_0x317af78 = 0;
    self->field_0x317af7c = false;
    new (&self->field_0x317af80) base::global::CStrId(base::global::UnkStrIdInline{}, nullptr, false);
    self->field_0x317af88 = 0;
    self->field_0x317af90 = 0;
    for (u32 i = 0; i < 3; ++i)
        self->field_0x317af98[i] = 0;
    for (u32 i = 0; i < 100; ++i)
        self->records[i].Init();
    self->field_0x3183610 = 0;

    UnkGfxNamedVectors& nv2 = self->field_0x3183628;
    nv2.values = nullptr;
    nv2.keys = nullptr;
    nv2.count = 0;
    nv2.capacity = 0;
    nv2.step = 10;
    nv2.flags = 0xe;
    nv2.allocator = fn_7100083990();
    self->field_0x3183650 = -1;
    self->field_0x3183654 = 0.25f;
    self->field_0x3183658 = 0xffffffff;
    self->field_0x318365c = 0;
    self->field_0x3183660[0] = false;
    self->field_0x3183660[1] = true;
    self->field_0x3183660[2] = false;
    self->field_0x3183660[3] = true;
    self->field_0x3183660[4] = true;
    self->field_0x3183660[5] = true;
    self->field_0x3183660[6] = false;
    self->field_0x3183667 = false;
    self->field_0x3183668 = false;
    self->field_0x3183669 = false;
    self->field_0x318366a = true;
    self->field_0x318366b = true;
    for (u32 i = 0; i < 6; ++i)
        self->field_0x318366c[i] = false;
    self->field_0x3183672 = false;
    self->field_0x3183673 = false;
    self->field_0x3183674 = true;
    for (u32 i = 0; i < 7; ++i)
        self->field_0x3183675[i] = false;
    self->field_0x318367c = false;
    self->field_0x318367d = false;
    for (u32 i = 0; i < 6; ++i)
        self->field_0x3183680[i] = 0;
    UnkGfxCaps caps;
    caps.field_0x0 = false;
    caps.field_0x1 = false;
    caps.field_0x2 = false;
    caps.unk_0x3 = 0;
    fn_71001598a0(&caps);
    if (!caps.field_0x2) {
        self->field_0x318366c[2] = false;
        self->field_0x318366c[3] = false;
    }
    self->field_0x31836b0 = 0;
    self->field_0x31836b4[0] = 1.0f;
    self->field_0x31836b4[1] = 1.0f;
    self->field_0x31836b4[2] = 1.0f;
    for (u32 i = 0; i < 4; ++i)
        self->field_0x31836c0[i] = 0;
    self->field_0x31836e0 = 0;
    for (u32 i = 0; i < 4; ++i)
        self->field_0x31836e4[i] = 1.0f;
    for (u32 i = 0; i < 3; ++i)
        self->field_0x31836f4[i] = 0;
}

// Resources (as src/game/CAbilitySonar.cpp and src/game/UnkEnvironmentFade.cpp spell them).
// A resource type (as src/game_s7/fn_71001b1dd0.cpp registers them): its name first.
struct UnkResType {
    base::global::CStrId name;
};
extern const UnkResType g_unk_716e4540f8;  // textures
extern const UnkResType g_unk_716e453ff8;  // models

struct UnkResRegistry;
extern UnkResRegistry* g_unk_7101d472f8;

// A path id whose release decrements and tests the count in two steps.
struct UnkResPathId {
    UnkStringData* data;

    UnkResPathId(const base::global::CFilePathStrId& other) : data(other.data) {
        if (&g_unk_7101d47200->empty != data)
            ++data->ref_count;
    }
    ~UnkResPathId() {
        if (UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
    }
};

// A resource's key: its type's name and its path.
struct UnkResKey {
    base::global::CStrId type;
    UnkResPathId path;

    UnkResKey(const base::global::CStrId& t, const base::global::CFilePathStrId& p) : type(t), path(p) {}
};

// A counted reference to a resource (base/UnkCountedResource.h).
struct UnkResRef {
    UnkCountedResource* ptr;

    UnkResRef() : ptr(nullptr) {}
    ~UnkResRef() {
        if (ptr && --ptr->ref_count == 0)
            fn_710001737c(ptr);
    }
};

UnkResRef fn_71000ead80(UnkResRegistry* registry, const UnkResKey* key, u8 flag, void* a);

// The resource if it is a render resource (msapi::res::CRenderRes), else null.
static inline UnkCountedResource* AsRenderRes(UnkCountedResource* res) {
    if (!res)
        return nullptr;
    const base::reflection::UnkReflObject* obj = reinterpret_cast<const base::reflection::UnkReflObject*>(res);
    return fn_710009d50c(obj->vf_0x0(), fn_71001b122c()) ? res : nullptr;
}

// The unit's functions the set-up calls (not decompiled yet).
void fn_71001cab0c(UnkGraphicsSystem* self);
void fn_71001cae0c(UnkGraphicsSystem* self, const base::global::CStrId* name, UnkCountedResource* texture, void* a);
void fn_71001c9550(UnkGraphicsSystem* self, bool a);

// Registers a texture under a name (a render resource only, else none).
static inline void RegisterTexture(UnkGraphicsSystem* self, const char* name, UnkCountedResource* texture) {
    base::global::CStrId id(name, true);
    fn_71001cae0c(self, &id, AsRenderRes(texture), nullptr);
}

// The display settings the caller passes (all optional).
struct UnkGfxSettings {
    u32 width;
    u32 height;
    f32 scale;  // copied to g_unk_7101a1e758
    bool field_0xc;
    u8 height_shift;
    bool field_0xe;
};

// The device configuration fn_71001587f0 is handed (the definition in
// src/game_s7/fn_71001587f0.cpp reads none of it).
struct UnkGfxDeviceConfig {
    u32 width;
    u32 height;
    u64 field_0x8;
    u64 field_0x10;
    bool field_0x18;  // set when the display size had to be chosen
    bool field_0x19;
};

// A node of the scene registry's map (0x28 bytes): key, value, the list links, the bucket chain.
struct UnkGfxCameraNode {
    void* key;
    void* value;
    UnkGfxCameraNode* next;  // 0x10
    UnkGfxCameraNode* prev;  // 0x18
    UnkGfxCameraNode* chain;  // 0x20
};

// The scene registry (the 0xb8-byte singleton g_unk_716e34c2e0, where cameras, decals and model
// instances are entered): a map keyed by address, with bucket heads in an RntVector and the nodes
// in a list.
struct UnkGfxSceneRegistry {
    u8 unk_0x0[0x8];
    base::UnkAllocator* allocator;
    u32 flags;  // 0x10
    u32 size;   // 0x14
    UnkGfxCameraNode** buckets;  // 0x18
    u32 bucket_count;  // 0x20, the RntVector's size
    u32 bucket_modulus;  // 0x24
    u8 unk_0x28[0x18];
    UnkGfxCameraNode* last;   // 0x40
    UnkGfxCameraNode* first;  // 0x48
};

void* fn_71001cb410(UnkGfxSceneRegistry* manager, UnkCountedResource* model, u32 a);

extern "C" [[noreturn]] void __assert_fail(const char* expr, const char* file, int line, const char* func);

// Singletons the set-up creates on first use (through the GOT): the decal resource manager
// (src/base/spatial/UnkDecalProjector.cpp; 0x40c8 bytes) and the scene registry (0xb8 bytes).
struct UnkDecalResourceMgr;
extern UnkDecalResourceMgr* g_unk_716e352450;
extern void* g_unk_716e34c2e0;
void fn_71001d7548(UnkDecalResourceMgr* self);
void fn_710019dcac(UnkGfxSceneRegistry* self);
void fn_710016999c(UnkRegistry_710014c790* self);
void fn_710016a414();
void fn_710019e088(UnkGfxSceneRegistry* self);
void fn_710019fe7c(UnkDecalResourceMgr* self);
void fn_710019bd2c(void* camera);
void fn_710019c198(void* camera, u32 a);
void fn_710016e75c();
void fn_710019a16c();
bool fn_71001587f0();
void fn_7100132f70(f32 scale, u32 width, u32 height, u32 render_width, u32 render_height);
u32 fn_7100132fc0();
void fn_7100159490(s32 x, s32 y, s32 width, s32 height, f32 min_depth, f32 max_depth);
void fn_7100066674(void* event, bool flag);
void fn_7100066744(void* thread, const char* name, void (*entry)(void*), u32 a, u32 b, void* arg);
void fn_7100066828(void* thread, u32 a);
void fn_71001c7d80(void* arg);  // the loading screen thread

// The render scale from the settings (g_unk_7101a1e75c is the resolution scale).
extern __attribute__((visibility("hidden"))) f32 g_unk_7101a1e758;
// The loading screen thread's state.
extern __attribute__((visibility("hidden"))) bool g_unk_716e595b68;
extern __attribute__((visibility("hidden"))) u8 g_unk_716e595ca8[];  // its event
extern __attribute__((visibility("hidden"))) u8 g_unk_716e595cd8[];  // its thread


// The set-up after construction (fn_7100150b70): the singletons, the device at the configured or
// best display size, the engine's default textures, the loading screen thread, the main camera,
// the overlay mode, the quad index buffer, the light proxy models and the resolution scale.
// NON_MATCHING(minor): the same calls with the same arguments in the same order and the same
// stores; the blocks are laid out differently (the original puts the singletons' creation out of
// line and shares the destructor tails), the loading-thread statics are addressed one by one
// rather than off one base, the camera map's remainder is guarded against a zero modulus (where
// AArch64's udiv gives zero), and the original passes the device configuration to fn_71001587f0,
// which reads none of it.
void fn_71001c9718(UnkGraphicsSystem* self, UnkGfxSettings* settings) {
    using base::global::CFilePathStrId;
    using base::global::CStrId;

    fn_710016a414();
    if (!g_unk_716e352450) {
        UnkDecalResourceMgr* p = static_cast<UnkDecalResourceMgr*>(fn_71000839a0(0x40c8, 0x3f));
        fn_71001d7548(p);
        g_unk_716e352450 = p;
    }
    if (!g_unk_716e34c2e0) {
        UnkGfxSceneRegistry* p = static_cast<UnkGfxSceneRegistry*>(fn_71000839a0(0xb8, 0x3f));
        fn_710019dcac(p);
        g_unk_716e34c2e0 = p;
    }
    if (!unk_710b372d68) {
        UnkRegistry_710014c790* p = static_cast<UnkRegistry_710014c790*>(fn_71000839a0(0x58, 0x3f));
        fn_710016999c(p);
        unk_710b372d68 = p;
    }
    fn_710019e088(static_cast<UnkGfxSceneRegistry*>(g_unk_716e34c2e0));

    UnkGfxDeviceConfig config;
    config.width = 1280;
    config.height = 720;
    config.field_0x8 = 0;
    config.field_0x10 = 0;
    config.field_0x18 = false;
    config.field_0x19 = true;
    if (settings) {
        config.width = settings->width;
        config.height = settings->height;
        config.field_0x18 = settings->field_0xc;
        config.field_0x19 = settings->field_0xe;
        g_unk_7101a1e758 = settings->scale;
    }
    bool started = fn_71001587f0();
    if (!started) {
        // Fall back to the display size closest to 16:9, stopping at the first one larger than
        // 1280x720 once one was seen.
        config.field_0x18 = true;
        u32 sizes[0x100];
        u32 n = fn_71001598d0(0x20, sizes);
        if (n) {
            bool found = false;
            f32 best = 3.4028235e+38f;
            for (u64 i = 0; i != (u64)n * 2; i += 2) {
                u32 width = sizes[(u32)i];
                u32 height = sizes[(u32)i + 1];
                if (height > 720 && found && width > 1280)
                    break;
                f32 diff = __builtin_fabsf(1.7777778f - (f32)width / (f32)height);
                if (diff <= best || !found) {
                    best = diff;
                    found = true;
                    config.width = width;
                    config.height = height;
                }
            }
            if (found) {
                if (settings) {
                    settings->field_0xc = config.field_0x18;
                    settings->width = config.width;
                    settings->height = config.height;
                }
                started = fn_71001587f0();
            }
        }
    }
    if (started) {
        // The original reads the settings here without a null check.
        u32 width = config.width;
        u32 height = config.height;
        f32 scale = g_unk_7101a1e758;
        u32 shifted = height >> (settings->height_shift & 0x1f);
        f32 fwidth = (f32)width;
        f32 fshifted = (f32)shifted;
        u32 render_width;
        u32 render_height;
        if (fwidth * 0.6f <= fshifted) {
            render_width = width;
            render_height = (u32)(fwidth / scale);
        } else {
            render_width = (u32)(scale * fshifted);
            render_height = shifted;
        }
        fn_7100132f70(1.0f, width, height, render_width, render_height);
    }
    fn_71001cab0c(self);

    // The engine's default textures, registered by name.
    UnkResKey debug_key(g_unk_716e4540f8.name, CFilePathStrId("system/engine/textures/debug.bctex", true));
    UnkResRef debug = fn_71000ead80(g_unk_7101d472f8, &debug_key, 8, nullptr);
    RegisterTexture(self, "ERROR", debug.ptr);
    UnkResKey noise_key(g_unk_716e4540f8.name, CFilePathStrId("system/engine/textures/noise.bctex", true));
    UnkResRef noise = fn_71000ead80(g_unk_7101d472f8, &noise_key, 8, nullptr);
    RegisterTexture(self, "NOISE", noise.ptr);
    UnkResKey gray_key(g_unk_716e4540f8.name, CFilePathStrId("system/engine/textures/gray.bctex", true));
    UnkResRef gray = fn_71000ead80(g_unk_7101d472f8, &gray_key, 8, nullptr);
    RegisterTexture(self, "GRAY", gray.ptr);
    UnkResKey dirt_key(g_unk_716e4540f8.name, CFilePathStrId("system/engine/textures/dirt_mask.bctex", true));
    UnkResRef dirt = fn_71000ead80(g_unk_7101d472f8, &dirt_key, 8, nullptr);
    RegisterTexture(self, "DIRT_MASK", dirt.ptr);
    UnkResKey brdf_key(g_unk_716e4540f8.name, CFilePathStrId("system/engine/textures/brdf_lut.bctex", true));
    UnkResRef brdf = fn_71000ead80(g_unk_7101d472f8, &brdf_key, 8, nullptr);
    RegisterTexture(self, "BRDF_LUT", brdf.ptr);

    fn_7100159490(0, 0, config.width, config.height, 0.0f, 1.0f);
    if (fn_71001598c0()) {
        g_unk_716e595b68 = true;
        fn_7100066674(g_unk_716e595ca8, true);
        fn_71000668f8(&g_unk_716e595cb8);
        fn_7100066744(g_unk_716e595cd8, "LoadingScreenThread", fn_71001c7d80, 0, 0, nullptr);
        fn_7100066828(g_unk_716e595cd8, 0);
    }

    // The main camera, added to the camera manager's map.
    UnkGfxSceneRegistry* cameras = static_cast<UnkGfxSceneRegistry*>(g_unk_716e34c2e0);
    void* camera = fn_71000839a0(0x188, 0x37);
    fn_710019bd2c(camera);
    if (camera) {
        UnkGfxCameraNode** bucket;
        if (cameras->bucket_count == 0) {
            bucket = nullptr;
        } else {
            // The remainder as AArch64 computes it (a zero modulus leaves the key).
            u32 key = (u32)(u64)camera;
            u32 modulus = cameras->bucket_modulus;
            u32 index = modulus ? key % modulus : key;
            if (cameras->bucket_count <= index)
                __assert_fail("i < m_iSize", "..\\Base/global/RntVector.h", 0x15b, "operator[]");
            bucket = &cameras->buckets[index];
        }
        UnkGfxCameraNode* head = *bucket;
        UnkGfxCameraNode* node =
            static_cast<UnkGfxCameraNode*>(cameras->allocator->vf_0x40(sizeof(UnkGfxCameraNode), 8, cameras->flags));
        if (node) {
            node->key = camera;
            node->value = camera;
            node->chain = nullptr;
            node->next = nullptr;
            node->prev = nullptr;
        }
        node->chain = head;
        *bucket = node;
        if (!cameras->first) {
            cameras->first = node;
        } else if (cameras->last) {
            node->prev = cameras->last;
            cameras->last->next = node;
        }
        cameras->last = node;
        ++cameras->size;
    }
    self->field_0x317aa20 = camera;
    fn_710019c198(camera, 0);
    fn_71001c9550(self, settings ? settings->height_shift != 0 : false);
    fn_71001cafa0(reinterpret_cast<UnkStruct_71001cdd74*>(self), 0, 1, 0);

    // Indices for quads: 0, 1, 2, 0, 2, 3 for each four vertices.
    base::UnkAllocator* allocator = fn_7100083990();
    u16* indices = static_cast<u16*>(allocator->vf_0x40((u64)self->field_0x317af48 * sizeof(u16), 8, 8));
    u32 count = self->field_0x317af48;
    self->field_0x317af50 = indices;
    if (count >= 6) {
        for (u64 i = 0; i < count / 6; ++i) {
            s32 j = (s32)(i * 6);
            u16 v = (u16)(i << 2);
            self->field_0x317af50[j] = v;
            self->field_0x317af50[j + 1] = v | 1;
            self->field_0x317af50[j + 2] = v | 2;
            self->field_0x317af50[j + 3] = v;
            self->field_0x317af50[j + 4] = v | 2;
            self->field_0x317af50[j + 5] = v | 3;
        }
    }
    fn_710019fe7c(g_unk_716e352450);

    // The light proxy models.
    self->light_proxies[0] = nullptr;
    UnkResKey omni_key(g_unk_716e453ff8.name, CFilePathStrId("system/engine/models/lightsproxy/omni.bcmdl", true));
    UnkResRef omni = fn_71000ead80(g_unk_7101d472f8, &omni_key, 8, nullptr);
    UnkGfxSceneRegistry* manager = static_cast<UnkGfxSceneRegistry*>(g_unk_716e34c2e0);
    self->light_proxies[0] = fn_71001cb410(manager, AsRenderRes(omni.ptr), 0x1e);
    self->light_proxies[1] = nullptr;
    UnkResKey spot_key(g_unk_716e453ff8.name, CFilePathStrId("system/engine/models/lightsproxy/spot.bcmdl", true));
    UnkResRef spot = fn_71000ead80(g_unk_7101d472f8, &spot_key, 8, nullptr);
    manager = static_cast<UnkGfxSceneRegistry*>(g_unk_716e34c2e0);
    self->light_proxies[1] = fn_71001cb410(manager, AsRenderRes(spot.ptr), 0x1e);
    self->light_proxies[2] = nullptr;
    UnkResKey segment_key(g_unk_716e453ff8.name,
                          CFilePathStrId("system/engine/models/lightsproxy/segment.bcmdl", true));
    UnkResRef segment = fn_71000ead80(g_unk_7101d472f8, &segment_key, 8, nullptr);
    manager = static_cast<UnkGfxSceneRegistry*>(g_unk_716e34c2e0);
    self->light_proxies[2] = fn_71001cb410(manager, AsRenderRes(segment.ptr), 0x1e);

    // The resolution scale: 1 docked, else the display's size relative to 1280.
    if (fn_7100132a10()) {
        g_unk_7101a1e75c.x = 1.0f;
        g_unk_7101a1e75c.y = 1.0f;
    } else {
        g_unk_7101a1e75c.y = 1280.0f / (f32)fn_7100132fc0();
        g_unk_7101a1e75c.x = 1280.0f / (f32)fn_7100132fc0();
    }
    fn_710016e75c();
    fn_710019a16c();
}

// The draw modes (fn_71001cb604).
void fn_710015a7d0(u32 value);
void fn_710015aa80(u32 value);
void fn_710015f770(s32 slot, void* object, bool clear);
void fn_710015b160();
s32 fn_710015c460();
u32 fn_7100167cc0(u32 index);  // a uniform id
// The location of the uniform named `name` of a program's stage, 0 when it has none.
void fn_71001aa0cc(void* program, u32 stage, const UnkGfxStrId* name, u64* location);
// Sets `count` matrices of a uniform at `location` of a program state.
void fn_710015e4b0(void* state, u32 stage, u64 location, u32 count, u32 a, const UnkMatrix44* values, u32 b);
void fn_710019c474(void*, const UnkMatrix44*);
void fn_710015bf40(UnkMatrix44*, f32);
void fn_7100161800(void* object);
void fn_7100154420(void* object);
f32 fn_7100132fa0();
f32 fn_7100132fb0();  // the display's aspect correction
void fn_71001ccdd8(UnkGraphicsSystem* self);
void fn_71001cd3a0(UnkGraphicsSystem* self);
// The draw modes' program keys.
extern __attribute__((visibility("hidden"))) u8 g_unk_716e595ba0[];
extern __attribute__((visibility("hidden"))) u8 g_unk_716e595ba8[];
extern __attribute__((visibility("hidden"))) u8 g_unk_716e595bd0[];
extern __attribute__((visibility("hidden"))) u8 g_unk_716e595be8[];
extern __attribute__((visibility("hidden"))) u8 g_unk_716e595bf8[];
extern __attribute__((visibility("hidden"))) u8 g_unk_716e595c10[];

// The camera at +0x317aa20 as the draw modes use it.
struct UnkGfxCamera {
    u8 unk_0x0[0x20];
    UnkMatrix44 projection;  // 0x20
    u8 unk_0x60[0x8];
    void* field_0x68[2];  // indexed by field_0x78
    u32 field_0x78;
    u8 unk_0x7c[0x4];
    void* field_0x80;
};

// A light of the four at +0xd8 (their count at +0xf8): a position at +0x38 and a colour at +0x7c.
struct UnkGfxLight {
    u8 unk_0x0[0x38];
    f32 position[3];  // 0x38
    u8 unk_0x44[0x38];
    f32 color[4];  // 0x7c
};

// Sets the draw mode: the program, render states and uniforms of each mode. `a`, `b`, `c` and `d`
// are uniform values some modes take (5 and 8: `a` and `b`; 8: the first float of `c` and `d`).
// NON_MATCHING(minor): each mode makes the same calls with the same arguments in the same order and
// the same stores; the blocks are laid out and shared differently (the original tail-merges the
// modes' endings and keeps modes 9 and 10 apart, which share one block here).
void fn_71001cb604(UnkGraphicsSystem* self, u32 mode, const f32* a, const f32* b, const f32* c, const f32* d) {
    u16* flags = reinterpret_cast<u16*>(&self->field_0x318366a);
    self->field_0x317a99c = mode;
    u64 location;
    switch (mode) {
    case 1:
        if (!self->field_0x317af7c && self->field_0x317af70)
            ResetOverlayProgram(self);
        fn_710015a0e0(1);
        fn_710015a260(1);
        fn_710015ab00(3);
        fn_710015a7d0(0);
        fn_710015aa80(0);
        *flags = 0x101;
        self->field_0x317af7c = false;
        fn_71001ccdd8(self);
        break;
    case 2:
    case 3:
    case 4:
    case 5:
        fn_71001cc608(self, fn_7100169be4(unk_710b372d68, (mode | 1) == 3 ? g_unk_716e595ba0 : g_unk_716e595ba8));
        fn_710015a0e0(0);
        fn_710015a260(0);
        fn_710015a3e0(0);
        fn_710015a860(1);
        fn_710015aa20(1);
        fn_710015a760(0);
        fn_710015a8c0(4);
        fn_710015f770(0, nullptr, true);
        fn_710015f770(1, nullptr, true);
        fn_710015f770(2, nullptr, true);
        fn_710015f770(3, nullptr, true);
        fn_71001606f0(0, 0, true);
        fn_71001606f0(1, 0, true);
        fn_71001606f0(2, 0, true);
        fn_71001606f0(3, 0, true);
        fn_710015ab00(mode == 2 ? 3 : 2);
        fn_710015aa80(0);
        *flags = 0;
        self->field_0x317af7c = false;
        if (mode == 5) {
            if (a && fn_71001aa0b4(self->field_0x317af70, 0, 0xe0, &location))
                fn_710015e1f0(self->field_0x317af68, 0, location, 1, a);
            if (b && fn_71001aa0b4(self->field_0x317af70, 0, 0xe1, &location))
                fn_710015e1f0(self->field_0x317af68, 0, location, 1, b);
        }
        break;
    case 6:
        fn_710015ab00(3);
        fn_710015a0e0(0);
        fn_710015a260(0);
        fn_710015a3e0(0);
        fn_710015a860(1);
        fn_710015aa20(1);
        fn_710015a760(0);
        fn_710015a8c0(4);
        fn_710015f770(0, nullptr, true);
        fn_710015f770(1, nullptr, true);
        fn_710015f770(2, nullptr, true);
        fn_71001606f0(0, 0, true);
        fn_71001606f0(1, 0, true);
        fn_71001606f0(2, 0, true);
        fn_710015ab00(3);
        fn_710015aa80(0);
        *flags = 0x101;
        self->field_0x317af7c = false;
        break;
    case 7:
        fn_71001cc608(self, fn_7100169be4(unk_710b372d68, g_unk_716e595bd0));
        fn_710015ab00(2);
        fn_710015aa80(0);
        fn_710015a0e0(1);
        fn_710015a260(1);
        fn_710015a3e0(0);
        fn_710015a860(1);
        fn_710015aa20(1);
        fn_710015a760(0);
        fn_710015a8c0(4);
        fn_710015f770(0, nullptr, true);
        fn_710015f770(1, nullptr, true);
        fn_710015f770(2, nullptr, true);
        fn_71001606f0(0, 0, true);
        fn_71001606f0(1, 0, true);
        fn_71001606f0(2, 0, true);
        *flags = 1;
        self->field_0x317af7c = false;
        break;
    case 8: {
        fn_71001cc608(self, fn_7100169be4(unk_710b372d68, g_unk_716e595be8));
        if (fn_71001aa0b4(self->field_0x317af70, 1, 0xe0, &location))
            fn_710015e1f0(self->field_0x317af68, 1, location, 1, a);
        if (fn_71001aa0b4(self->field_0x317af70, 1, 0xe1, &location))
            fn_710015e1f0(self->field_0x317af68, 1, location, 1, b);
        if (fn_71001aa0b4(self->field_0x317af70, 1, 0x59, &location))
            fn_710015e1f0(self->field_0x317af68, 1, location, 4, d);
        if (self->field_0x317ae20) {
            f32 values[4];
            values[0] = 0.0f;
            values[1] = 0.0f;
            __builtin_memcpy(&values[2], c, sizeof(f32));
            __builtin_memcpy(&values[3], c, sizeof(f32));
            fn_710015e1f0(self->field_0x317af68, 0, self->field_0x317ae20, 4, values);
        }
        fn_710015ab00(4);
        fn_710015aa80(0);
        fn_710015a0e0(1);
        fn_710015a260(0);
        fn_710015a3e0(0);
        fn_710015a860(0);
        fn_710015aa20(0);
        fn_710015a760(0);
        fn_710015f770(0, nullptr, true);
        fn_710015f770(1, nullptr, true);
        fn_710015f770(2, nullptr, true);
        fn_71001606f0(0, 1, true);
        fn_71001606f0(1, 0, true);
        fn_71001606f0(2, 0, true);
        *flags = 0;
        self->field_0x317af7c = false;
        // The camera's view, scaled vertically by the display's aspect correction (and flipped
        // when the device asks for it).
        UnkGfxCamera* camera = static_cast<UnkGfxCamera*>(self->field_0x317aa20);
        UnkMatrix44 view;
        for (u32 i = 0; i < 16; ++i)
            view.m[i] = 0.0f;
        view.m[0] = 1.0f;
        view.m[5] = 1.0f;
        view.m[10] = 1.0f;
        view.m[15] = 1.0f;
        f32 aspect = fn_7100132fb0();
        view = fn_71000127c8(fn_7100079138(1.0f, aspect, 1.0f), view);
        if (fn_710015c460() == 1)
            view.m[5] = -view.m[5];
        fn_710019c474(camera, &view);
        fn_7100161800(camera->field_0x68[camera->field_0x78]);
        fn_7100154420(camera->field_0x80);
        // +0x317ae18 is the last of the four words at +0x317ae00: a matrix uniform's buffer.
        if (self->field_0x317ae00[3]) {
            UnkMatrix44 projection;
            UnkCopyMatrix(&projection, camera->projection);
            fn_710015bf40(&projection, fn_7100132fa0());
            fn_710015e4b0(self->field_0x317af68, 0, self->field_0x317ae00[3], 4, 1, &projection, 1);
        }
        break;
    }
    case 9:
    case 10: {
        fn_71001cc608(self, fn_7100169be4(unk_710b372d68, g_unk_716e595bf8));
        fn_710015aa80(1);
        fn_710015a8c0(5);
        fn_710015a0e0(1);
        fn_710015a260(1);
        fn_710015ab00(mode == 9 ? 3 : 4);
        fn_710015a7d0(0);
        void* program = self->field_0x317af70;
        if (fn_71001aa0b4(program, 1, fn_7100167cc0(0), &location)) {
            f32 values[4];
            if (mode == 9) {
                values[0] = 1.0f;
                values[1] = 1.0f;
                values[2] = 1.0f;
            } else {
                values[0] = 0.5f;
                values[1] = 0.5f;
                values[2] = 0.5f;
            }
            values[3] = 1.0f;
            fn_710015e1f0(self->field_0x317af68, 1, location, 4, values);
        }
        *flags = 0;
        if (mode == 9)
            self->field_0x317af7c = false;
        break;
    }
    case 12: {
        fn_71001cc608(self, fn_7100169be4(unk_710b372d68, g_unk_716e595c10));
        fn_710015aa80(0);
        fn_710015a8c0(5);
        fn_710015a860(1);
        fn_710015aa20(0);
        fn_710015a0e0(1);
        fn_710015a260(1);
        fn_710015ab00(3);
        fn_710015a7d0(0);
        fn_710015a3e0(0);
        // The four lights' positions and colours (zero for the missing ones).
        UnkGfxLight** lights = reinterpret_cast<UnkGfxLight**>(self->unk_0xd8);
        for (u32 i = 0; i < 4; ++i) {
            f32 position[4] = {0.0f, 0.0f, 0.0f, 0.0f};
            f32 color[4] = {0.0f, 0.0f, 0.0f, 0.0f};
            if (self->field_0xf8 > i) {
                UnkGfxLight* light = lights[i];
                position[0] = light->position[0];
                position[1] = light->position[1];
                position[2] = light->position[2];
                color[0] = light->color[0];
                color[1] = light->color[1];
                color[2] = light->color[2];
                color[3] = light->color[3];
            }
            if (fn_71001aa0b4(self->field_0x317af70, 1, 0xff + i * 2, &location))
                fn_710015e1f0(self->field_0x317af68, 1, location, 4, position);
            if (fn_71001aa0b4(self->field_0x317af70, 1, 0x100 + i * 2, &location))
                fn_710015e1f0(self->field_0x317af68, 1, location, 4, color);
        }
        fn_71001cd3a0(self);
        // The named vectors, by name.
        UnkGfxNamedVectors& t = self->named_vectors;
        for (UnkGfxNamedVector* v = t.values; v != t.values + t.count; ++v) {
            if (v->name.data != &g_unk_7101d47108->empty) {
                u64 named;
                fn_71001aa0cc(self->field_0x317af70, 1, &v->name, &named);
                if (named)
                    fn_710015e1f0(self->field_0x317af68, 1, named, 4, &v->value.x);
            }
        }
        f32 value = self->field_0x3c;
        if (fn_71001aa0b4(self->field_0x317af70, 1, 0x74, &location))
            fn_710015e1f0(self->field_0x317af68, 1, location, 1, &value);
        *flags = 0;
        self->field_0x317af7c = false;
        break;
    }
    default:
        if (!self->field_0x317af7c && self->field_0x317af70)
            ResetOverlayProgram(self);
        fn_710015a0e0(1);
        fn_710015a260(1);
        fn_710015ab00(3);
        fn_710015a7d0(0);
        fn_710015aa80(0);
        *flags = 0x101;
        self->field_0x317af7c = false;
        break;
    }
    fn_710015b160();
}
