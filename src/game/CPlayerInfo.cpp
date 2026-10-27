// CPlayerInfo's unit (the class name from reflection; the object is 0x400 bytes): its constructor.
// Names and layouts are placeholders inferred from the constructor.
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// The base class's constructor (stores its vtable and clears the word at +8).
void fn_7101043904(void* self);
void fn_710002f4e4(void* self);  // a timer's constructor
void fn_7100f10c4c(void* self);
void fn_7100f10e18(void* self);
void fn_7100f10f20(void* self);
void fn_7100f0f4d0();
extern char g_unk_7101caa450[];  // CPlayerInfo's vtable

// A string id's data with a plain count: these copies increment it with int alias info (the
// original reuses the pools it loaded once across the increments).
struct UnkPlainStringData {
    u8 unk_0x0[0x8];
    s32 ref_count;
};

// A string id copied from a global id, its count incremented unless it is the pool's empty string.
struct UnkPlayerInfoId {
    UnkStringData* data;

    UnkPlayerInfoId(const base::global::UnkStrIdBase& other, UnkStringPool* const& pool) : data(other.data) {
        if (&pool->empty != data)
            ++reinterpret_cast<UnkPlainStringData*>(data)->ref_count;
    }
    UnkPlayerInfoId(const base::global::UnkFilePathStrIdBase& other, UnkStringPool* const& pool) : data(other.data) {
        if (&pool->empty != data)
            ++reinterpret_cast<UnkPlainStringData*>(data)->ref_count;
    }
};

struct UnkPlayerInfoRecord {
    UnkPlayerInfoId name;
    UnkPlayerInfoId path;
    u64 field_0x10;

    UnkPlayerInfoRecord()
        : name(g_unk_7101d470e0, g_unk_7101d47108), path(g_unk_7101d470e8, g_unk_7101d47200), field_0x10(0) {}
};

// A callback bound to its owner.
struct UnkPlayerInfoCallback {
    void* field_0x0;
    void* owner;
    void (*function)();
};

struct UnkPlayerInfo {
    void* vtable;
    u64 field_0x8;
    u32 field_0x10;
    u8 pad_0x14[0x70];
    u32 field_0x84;
    u32 field_0x88;
    bool field_0x8c;
    bool field_0x8d;
    bool field_0x8e;
    u32 field_0x90;
    u8 pad_0x94[0x40];
    u32 field_0xd4;
    u32 field_0xd8;
    u32 field_0xdc;
    u32 field_0xe0;
    u32 field_0xe4;
    u32 field_0xe8;
    bool field_0xec;
    bool field_0xed;
    u32 field_0xf0;
    bool field_0xf4;
    f32 field_0xf8;
    f32 field_0xfc;
    u32 field_0x100;
    bool field_0x104;
    u8 pad_0x105[0x7];
    f32 field_0x10c[3];
    u8 pad_0x118[0x10];
    u64 field_0x128;
    u32 field_0x130;
    u64 field_0x138;
    bool field_0x140;
    f32 field_0x144[2];
    u8 pad_0x14c[0x4];
    u8 field_0x150[0x30];
    u8 field_0x180[0x38];
    u8 field_0x1b8[0x30];
    UnkPlayerInfoCallback field_0x1e8;
    UnkPlayerInfoRecord records[4];  // 0x200
    u64 field_0x260;
    s64 field_0x268;
    u8 timer_0x270[0xb0];
    u64* field_0x320;  // two parallel arrays with a shared count (base/UnkPairArrays.h's layout)
    u64* field_0x328;
    u32 field_0x330;
    u32 field_0x334;
    u32 field_0x338;
    u32 field_0x33c;
    base::UnkAllocator* field_0x340;
    u8 timer_0x348[0xb0];
    bool field_0x3f8;
};
static_assert(__builtin_offsetof(UnkPlayerInfo, field_0x10c) == 0x10c, "");
static_assert(__builtin_offsetof(UnkPlayerInfo, field_0x1e8) == 0x1e8, "");
static_assert(__builtin_offsetof(UnkPlayerInfo, field_0x3f8) == 0x3f8, "");
static_assert(sizeof(UnkPlayerInfo) == 0x400, "");

void fn_7100f086c8(UnkPlayerInfo* self) {
    fn_7101043904(self);
    self->vtable = g_unk_7101caa450 + 0x10;
    self->field_0x10 = 0;
    self->field_0x84 = 0;
    self->field_0x88 = 0;
    self->field_0x8c = false;
    self->field_0x8d = false;
    self->field_0x8e = false;
    self->field_0x90 = 0;
    self->field_0xd4 = 0;
    self->field_0xd8 = 0;
    self->field_0xdc = 0;
    self->field_0xe0 = 0;
    self->field_0xe4 = 0;
    self->field_0xe8 = 0;
    self->field_0xec = true;
    self->field_0xed = false;
    self->field_0xf0 = 0;
    self->field_0xf4 = false;
    self->field_0xf8 = 0.0f;
    self->field_0xfc = 400.0f;
    self->field_0x100 = 0;
    self->field_0x104 = false;
    self->field_0x10c[0] = 0.0f;
    self->field_0x10c[1] = 0.0f;
    self->field_0x10c[2] = 0.0f;
    self->field_0x128 = 0;
    self->field_0x130 = 0;
    self->field_0x138 = 0;
    self->field_0x140 = false;
    self->field_0x144[0] = 0.0f;
    self->field_0x144[1] = 0.0f;
    fn_7100f10c4c(self->field_0x150);
    fn_7100f10e18(self->field_0x180);
    fn_7100f10f20(self->field_0x1b8);
    self->field_0x1e8.field_0x0 = nullptr;
    self->field_0x1e8.owner = self;
    self->field_0x1e8.function = fn_7100f0f4d0;
    new (&self->records[0]) UnkPlayerInfoRecord();
    new (&self->records[1]) UnkPlayerInfoRecord();
    new (&self->records[2]) UnkPlayerInfoRecord();
    new (&self->records[3]) UnkPlayerInfoRecord();
    self->field_0x260 = 0;
    self->field_0x268 = -1;
    fn_710002f4e4(self->timer_0x270);
    self->field_0x320 = nullptr;
    self->field_0x328 = nullptr;
    self->field_0x330 = 0;
    self->field_0x334 = 0;
    self->field_0x338 = 10;
    self->field_0x33c = 5;
    self->field_0x340 = fn_7100083990();
    fn_710002f4e4(self->timer_0x348);
    self->field_0x3f8 = false;
}
