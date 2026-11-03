// CCentralUnitComponent unit (tools/tu_map.py unit 1176): fn_710077a630 is base::UnkVector's Reserve for
// 0x80-byte entries, out of line: each entry is moved as two 0x40-byte halves (fn_71002458a4) and the
// old one destroyed (fn_7100778228). fn_710077a51c and fn_710077a82c are the array's copy constructor
// and copy assignment: reserve the other's capacity, then append a copy of each entry (fn_710077a700).
// fn_71007788c4 is a constructor: the vtable, a copy of the empty string id at +0x8, the container at
// +0x10 (base vtable, empty array with step 10 and flags 0xf, final vtable, allocator), cleared fields
// and the range 0..1 at +0x50, then the member at +0x58 (fn_71002b22ac, the empty id and 0..1). Names
// and layouts are placeholders. fn_7100779ba4 is an assignment of a record whose word at +0x0 and two
// parts at +0x8 are inline, or behind pointers there when its kind (+0x88) is 2; each part copies its
// handle (+0x38), word (+0x30) and array (+0x8).
#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkHalf_cu {
    u8 data[0x40];
    UnkHalf_cu(UnkHalf_cu&& other) asm("fn_71002458a4");
};

struct UnkEntry_cu {
    UnkHalf_cu first;
    UnkHalf_cu second;
    UnkEntry_cu(UnkEntry_cu&& other)
        : first(static_cast<UnkHalf_cu&&>(other.first)), second(static_cast<UnkHalf_cu&&>(other.second)) {}
    ~UnkEntry_cu() asm("fn_7100778228");
};

void fn_710077a630(base::UnkVector<UnkEntry_cu>* self, u32 n) {
    self->Reserve(n);
}

void fn_710077a700(base::UnkVector<UnkEntry_cu>* self, const UnkEntry_cu* value);

static inline __attribute__((always_inline)) void AppendAll_cu(base::UnkVector<UnkEntry_cu>* self,
                                                               const base::UnkVector<UnkEntry_cu>* other) {
    for (const UnkEntry_cu *it = other->data, *end = other->data + other->count; it != end; ++it)
        fn_710077a700(self, it);
}

void fn_710077a51c(base::UnkVector<UnkEntry_cu>* self, const base::UnkVector<UnkEntry_cu>* other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    self->Reserve(other->capacity);
    AppendAll_cu(self, other);
}

base::UnkVector<UnkEntry_cu>* fn_710077a82c(base::UnkVector<UnkEntry_cu>* self,
                                           const base::UnkVector<UnkEntry_cu>* other) {
    if (self != other) {
        self->Clear();
        self->Reserve(other->capacity);
        AppendAll_cu(self, other);
    }
    return self;
}

extern void* g_unk_7101a6ec50[];
extern void* g_unk_7101a20998[];
extern void* g_unk_7101a20950[];
void fn_71002b22ac(void* member, const base::global::CStrId* name, f32 min, f32 max);

struct UnkRanged_cu {
    void** vtable;
    base::global::CStrId field_0x8;
    void** field_0x10;
    u64 field_0x18;
    u64 field_0x20;
    u32 field_0x28;
    bool field_0x2c;
    u32 field_0x30;
    base::UnkAllocator* field_0x38;
    u32 field_0x40;
    u64 field_0x48;
    f32 field_0x50;
    f32 field_0x54;
    u8 field_0x58[0x8];
};

void fn_71007788c4(UnkRanged_cu* self) {
    self->vtable = g_unk_7101a6ec50 + 2;
    new (&self->field_0x8) base::global::CStrId(g_unk_7101d470e0);
    self->field_0x10 = g_unk_7101a20998 + 2;
    self->field_0x18 = 0;
    self->field_0x20 = 0;
    self->field_0x28 = 10;
    self->field_0x2c = true;
    self->field_0x30 = 0xf;
    self->field_0x38 = fn_7100083990();
    self->field_0x40 = 0;
    self->field_0x10 = g_unk_7101a20950 + 2;
    self->field_0x48 = 0;
    self->field_0x50 = 0.0f;
    self->field_0x54 = 1.0f;
    fn_71002b22ac(self->field_0x58, &g_unk_7101d470e0, 0.0f, 1.0f);
}


struct UnkPart_cu {
    u8 pad_0x0[0x8];
    UnkErasedVector field_0x8;
    u8 pad_0x8[0x28 - sizeof(UnkErasedVector)];
    u32 field_0x30;
    base::UnkRefHandle field_0x38;

    void Assign(const UnkPart_cu& other) {
        const base::UnkRefHandle* handle = &other.field_0x38;
        fn_7100234f78(&field_0x38, handle);
        field_0x30 = other.field_0x30;
        fn_710023b29c(&field_0x8, &other.field_0x8);
    }
};

struct UnkParts_cu {
    UnkPart_cu first;
    UnkPart_cu second;
};

struct UnkKinded_cu {
    union {
        u32 word;
        u32* word_ptr;
    };
    union {
        UnkParts_cu parts;
        UnkParts_cu* parts_ptr;
    };
    u32 kind;  // 0x88

    u32& Word() { return kind == 2 ? *word_ptr : word; }
    const u32& Word() const { return kind == 2 ? *word_ptr : word; }
    UnkParts_cu& Parts() { return kind == 2 ? *parts_ptr : parts; }
    const UnkParts_cu& Parts() const { return kind == 2 ? *parts_ptr : parts; }
};

UnkKinded_cu* fn_7100779ba4(UnkKinded_cu* self, const UnkKinded_cu* other) {
    if (self != other) {
        self->Word() = other->Word();
        const UnkParts_cu& src = other->Parts();
        UnkParts_cu& dst = self->Parts();
        dst.first.Assign(src.first);
        dst.second.Assign(src.second);
    }
    return self;
}
