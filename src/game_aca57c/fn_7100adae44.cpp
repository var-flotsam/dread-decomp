// Assignments of a pair of values held either in place or by reference (state 2), from another
// such pair: read and written through the state-dependent accessors.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

template <class X, class Y>
struct UnkValuePairRef {
    union {
        struct {
            X x;
            Y y;
        } values;
        struct {
            X* x;
            Y* y;
        } refs;
    };
    s32 state;

    X& GetX() { return state == 2 ? *refs.x : values.x; }
    Y& GetY() { return state == 2 ? *refs.y : values.y; }

    // Destroys the in-place values (state 1); either way the pair is left empty.
    void Reset() {
        switch (state) {
        case 1:
            values.y.~Y();
            values.x.~X();
            state = 0;
            break;
        case 2:
            state = 0;
            break;
        }
    }

    // Refers to `x` and `y`.
    void Set(X* x, Y* y) {
        Reset();
        state = 2;
        refs.x = x;
        refs.y = y;
    }
};

// Assigns both values whatever the other's state.
template <class X, class Y>
static inline UnkValuePairRef<X, Y>* Assign(UnkValuePairRef<X, Y>* self, UnkValuePairRef<X, Y>* other) {
    if (self != other) {
        self->GetX() = other->GetX();
        self->GetY() = other->GetY();
    }
    return self;
}

// Assigns only from an in-place (state 1) or by-reference (state 2) pair.
template <class X, class Y>
static inline UnkValuePairRef<X, Y>* AssignSet(UnkValuePairRef<X, Y>* self, UnkValuePairRef<X, Y>* other) {
    if (self != other) {
        switch (other->state) {
        case 1:
            self->GetX() = other->values.x;
            self->GetY() = other->values.y;
            break;
        case 2:
            self->GetX() = *other->refs.x;
            self->GetY() = *other->refs.y;
            break;
        }
    }
    return self;
}

// A value and a flag, and two values, assigned member by member.
struct UnkValueFlag {
    u32 value;
    bool flag;

    // Copies the five data bytes (not the padding).
    UnkValueFlag& operator=(const UnkValueFlag& other) {
        __builtin_memcpy(this, &other, 5);
        return *this;
    }
};

struct UnkValues2Memberwise {
    u32 a;
    u32 b;

    UnkValues2Memberwise& operator=(const UnkValues2Memberwise& other) {
        a = other.a;
        b = other.b;
        return *this;
    }
};

// Two values.
struct UnkValues2 {
    u32 a;
    u32 b;
};

using UnkU32Pair = UnkValuePairRef<u32, u32>;
using UnkU32FlagPair = UnkValuePairRef<u32, UnkValueFlag>;
using UnkU32ValuesPair = UnkValuePairRef<u32, UnkValues2>;
using UnkU32Values2Pair = UnkValuePairRef<u32, UnkValues2Memberwise>;

UnkU32Pair* fn_7100adae44(UnkU32Pair* self, UnkU32Pair* other) { return Assign(self, other); }
UnkU32Pair* fn_7100dd0298(UnkU32Pair* self, UnkU32Pair* other) { return Assign(self, other); }
UnkU32Pair* fn_7100eb1f10(UnkU32Pair* self, UnkU32Pair* other) { return Assign(self, other); }
UnkU32Pair* fn_7100adaee4(UnkU32Pair* self, UnkU32Pair* other) { return AssignSet(self, other); }
UnkU32Pair* fn_7100dd0338(UnkU32Pair* self, UnkU32Pair* other) { return AssignSet(self, other); }
UnkU32Pair* fn_7100eb1fb0(UnkU32Pair* self, UnkU32Pair* other) { return AssignSet(self, other); }

UnkU32FlagPair* fn_7100b19250(UnkU32FlagPair* self, UnkU32FlagPair* other) { return Assign(self, other); }
UnkU32FlagPair* fn_7100b192f4(UnkU32FlagPair* self, UnkU32FlagPair* other) { return AssignSet(self, other); }
UnkU32ValuesPair* fn_7100b17a60(UnkU32ValuesPair* self, UnkU32ValuesPair* other) { return Assign(self, other); }
UnkU32ValuesPair* fn_7100b17afc(UnkU32ValuesPair* self, UnkU32ValuesPair* other) { return AssignSet(self, other); }
UnkU32Values2Pair* fn_7100c0aeb0(UnkU32Values2Pair* self, UnkU32Values2Pair* other) { return Assign(self, other); }
UnkU32Values2Pair* fn_7100c0af54(UnkU32Values2Pair* self, UnkU32Values2Pair* other) { return AssignSet(self, other); }

// An id and two values.
struct UnkIdValues2 {
    CStrId id;
    u32 a;
    u32 b;

    UnkIdValues2& operator=(const UnkIdValues2& other) {
        id = other.id;
        a = other.a;
        b = other.b;
        return *this;
    }
};

using UnkU32IdValuesPair = UnkValuePairRef<u32, UnkIdValues2>;

UnkU32IdValuesPair* fn_7100eaee5c(UnkU32IdValuesPair* self, UnkU32IdValuesPair* other) { return Assign(self, other); }

// Six values, an id and a half-word.
struct UnkValuesIdHalf {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    CStrId id;
    u16 field_0x20;

    UnkValuesIdHalf& operator=(const UnkValuesIdHalf& other) {
        field_0x0 = other.field_0x0;
        field_0x4 = other.field_0x4;
        field_0x8 = other.field_0x8;
        field_0xc = other.field_0xc;
        field_0x10 = other.field_0x10;
        field_0x14 = other.field_0x14;
        id = other.id;
        field_0x20 = other.field_0x20;
        return *this;
    }
};

using UnkIdValuesIdPair = UnkValuePairRef<CStrId, UnkValuesIdHalf>;

// As Assign, comparing the other with self.
template <class X, class Y>
static inline UnkValuePairRef<X, Y>* AssignFrom(UnkValuePairRef<X, Y>* self, UnkValuePairRef<X, Y>* other) {
    if (other != self) {
        self->GetX() = other->GetX();
        self->GetY() = other->GetY();
    }
    return self;
}

UnkIdValuesIdPair* fn_7100e5acb4(UnkIdValuesIdPair* self, UnkIdValuesIdPair* other) { return AssignFrom(self, other); }

// An id and four values; two ids and a value.
struct UnkIdValues4 {
    CStrId id;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;

    UnkIdValues4& operator=(const UnkIdValues4& other) {
        id = other.id;
        field_0x8 = other.field_0x8;
        field_0xc = other.field_0xc;
        field_0x10 = other.field_0x10;
        field_0x14 = other.field_0x14;
        return *this;
    }
};

struct UnkIdsValue {
    CStrId first;
    CStrId second;
    u32 field_0x10;

    __attribute__((always_inline)) UnkIdsValue& operator=(const UnkIdsValue& other) {
        first = other.first;
        second = other.second;
        field_0x10 = other.field_0x10;
        return *this;
    }
};

using UnkIdIdValues4Pair = UnkValuePairRef<CStrId, UnkIdValues4>;
using UnkIdIdsValuePair = UnkValuePairRef<CStrId, UnkIdsValue>;

UnkIdIdValues4Pair* fn_7100e70ab4(UnkIdIdValues4Pair* self, UnkIdIdValues4Pair* other) { return AssignFrom(self, other); }
UnkIdIdsValuePair* fn_7100c8fcd8(UnkIdIdsValuePair* self, UnkIdIdsValuePair* other) { return Assign(self, other); }

// A value and a string (assigned by fn_710007eaa0).
struct UnkAssignedString {
    u64 unk_0x0[6];

    UnkAssignedString& operator=(const UnkAssignedString& other);
};
void fn_710007eaa0(UnkAssignedString* self, const UnkAssignedString* other);
__attribute__((always_inline)) inline UnkAssignedString& UnkAssignedString::operator=(const UnkAssignedString& other) {
    fn_710007eaa0(this, &other);
    return *this;
}

struct UnkValueString {
    u32 value;
    UnkAssignedString string;

    UnkValueString& operator=(const UnkValueString& other) {
        value = other.value;
        string = other.string;
        return *this;
    }
};

using UnkU32ValueStringPair = UnkValuePairRef<u32, UnkValueString>;

UnkU32ValueStringPair* fn_7100b1ac58(UnkU32ValueStringPair* self, UnkU32ValueStringPair* other) { return Assign(self, other); }
UnkU32ValueStringPair* fn_7100b1acf8(UnkU32ValueStringPair* self, UnkU32ValueStringPair* other) { return AssignSet(self, other); }

// A value and an optional external array, with a pair referring to both: assignment copies the
// value and the array pointer and rebinds the pair. The in-place array frees its storage.
template <class T>
struct UnkFreedVector : base::UnkVector<T> {
    ~UnkFreedVector() { this->Free(); }
};

struct UnkBoundValueArray {
    u32 value;
    UnkFreedVector<u32>* array;
    UnkValuePairRef<u32, UnkFreedVector<u32>> pair;
};

static inline UnkBoundValueArray* Assign(UnkBoundValueArray* self, const UnkBoundValueArray* other) {
    if (self != other) {
        self->value = other->value;
        self->array = other->array;
        if (self->array)
            self->pair.Set(&self->value, self->array);
        else
            self->pair.Reset();
    }
    return self;
}

UnkBoundValueArray* fn_7100d3f08c(UnkBoundValueArray* self, const UnkBoundValueArray* other) { return Assign(self, other); }
UnkBoundValueArray* fn_7100d3f14c(UnkBoundValueArray* self, const UnkBoundValueArray* other) { return Assign(self, other); }
UnkBoundValueArray* fn_7100d3f514(UnkBoundValueArray* self, const UnkBoundValueArray* other) { return Assign(self, other); }
UnkBoundValueArray* fn_7100d3f5d4(UnkBoundValueArray* self, const UnkBoundValueArray* other) { return Assign(self, other); }

// As UnkBoundValueArray, with a string (destroyed by fn_710007ecd4) placed 0x10 into the pair.
struct alignas(16) UnkFreedString {
    u64 unk_0x0[6];

    ~UnkFreedString();
};
void fn_710007ecd4(UnkFreedString* self);
inline UnkFreedString::~UnkFreedString() { fn_710007ecd4(this); }

struct UnkBoundValueString {
    u32 value;
    UnkFreedString* string;
    UnkValuePairRef<u32, UnkFreedString> pair;
};

static inline UnkBoundValueString* Assign(UnkBoundValueString* self, const UnkBoundValueString* other) {
    if (self != other) {
        self->value = other->value;
        self->string = other->string;
        if (self->string)
            self->pair.Set(&self->value, self->string);
        else
            self->pair.Reset();
    }
    return self;
}

UnkBoundValueString* fn_7100b15ac4(UnkBoundValueString* self, const UnkBoundValueString* other) { return Assign(self, other); }
UnkBoundValueString* fn_7100b15b54(UnkBoundValueString* self, const UnkBoundValueString* other) { return Assign(self, other); }
UnkBoundValueString* fn_7100b15ebc(UnkBoundValueString* self, const UnkBoundValueString* other) { return Assign(self, other); }
UnkBoundValueString* fn_7100b15f4c(UnkBoundValueString* self, const UnkBoundValueString* other) { return Assign(self, other); }

// A pair of an id and two arrays, set to refer to another pair's referents.
struct UnkPairReleasedStrId {
    base::global::UnkStringData* data;

    ~UnkPairReleasedStrId() {
        if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
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

struct UnkVectorPair {
    UnkFreedVector<u32> first;
    UnkFreedVector<u32> second;
    u64 unk_0x50[2];
};

using UnkIdVectorsPair = UnkValuePairRef<UnkPairReleasedStrId, UnkVectorPair>;

static inline UnkIdVectorsPair* SetRefs(UnkIdVectorsPair* self, const UnkIdVectorsPair* other) {
    if (self != other)
        self->Set(other->refs.x, other->refs.y);
    return self;
}

UnkIdVectorsPair* fn_7100e48460(UnkIdVectorsPair* self, const UnkIdVectorsPair* other) { return SetRefs(self, other); }
UnkIdVectorsPair* fn_7100e4892c(UnkIdVectorsPair* self, const UnkIdVectorsPair* other) { return SetRefs(self, other); }

// Four values, an id, a record assigned by fn_7100e4f444 and a value.
struct UnkArraysRecord {
    u64 unk_0x0[0xc];
};
UnkArraysRecord* fn_7100e4f444(UnkArraysRecord* self, const UnkArraysRecord* other);  // operator=

struct UnkValuesIdArrays {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    CStrId id;
    UnkArraysRecord arrays;
    u32 field_0x78;

    UnkValuesIdArrays& operator=(const UnkValuesIdArrays& other) {
        field_0x0 = other.field_0x0;
        field_0x4 = other.field_0x4;
        field_0x8 = other.field_0x8;
        field_0xc = other.field_0xc;
        id = other.id;
        fn_7100e4f444(&arrays, &other.arrays);
        field_0x78 = other.field_0x78;
        return *this;
    }
};

using UnkIdValuesArraysPair = UnkValuePairRef<CStrId, UnkValuesIdArrays>;

UnkIdValuesArraysPair* fn_7100e60320(UnkIdValuesArraysPair* self, UnkIdValuesArraysPair* other) {
    return AssignFrom(self, other);
}

// A record whose leading part is assigned by fn_710011dac0 (after two values and a flag set
// on assignment), then a value and the arrays record.
struct UnkPartsRecord {
    u64 unk_0x0[5];
    u32 field_0x28;
    bool field_0x2c;
    bool field_0x2d;
    u64 unk_0x30[2];
    u32 field_0x40;
    UnkArraysRecord arrays;

    UnkPartsRecord& operator=(const UnkPartsRecord& other);
};
void fn_710011dac0(UnkPartsRecord* self, const UnkPartsRecord* other);
inline UnkPartsRecord& UnkPartsRecord::operator=(const UnkPartsRecord& other) {
    field_0x28 = other.field_0x28;
    field_0x2c = other.field_0x2c;
    field_0x2d = true;
    fn_710011dac0(this, &other);
    field_0x40 = other.field_0x40;
    fn_7100e4f444(&arrays, &other.arrays);
    return *this;
}

using UnkIdPartsPair = UnkValuePairRef<CStrId, UnkPartsRecord>;

UnkIdPartsPair* fn_7100e5e5ec(UnkIdPartsPair* self, UnkIdPartsPair* other) { return AssignFrom(self, other); }

// An id, a value, an array of polymorphic records (assigned by fn_7100e4e898), flags, values
// and two arrays records.
struct UnkPolyVector {
    u64 unk_0x0[5];

    UnkPolyVector& operator=(const UnkPolyVector& other);
};
UnkPolyVector* fn_7100e4e898(UnkPolyVector* self, const UnkPolyVector* other);
__attribute__((always_inline)) inline UnkPolyVector& UnkPolyVector::operator=(const UnkPolyVector& other) {
    fn_7100e4e898(this, &other);
    return *this;
}

struct UnkIdPolyRecord {
    CStrId id;
    u32 field_0x8;
    u64 unk_0x10;
    UnkPolyVector records;
    bool field_0x40;
    u8 unk_0x41[0x1f];
    bool field_0x60;
    u32 field_0x64;
    u32 field_0x68;
    u32 field_0x6c;
    u32 field_0x70;
    UnkArraysRecord first;
    UnkArraysRecord second;

    __attribute__((always_inline)) UnkIdPolyRecord& operator=(const UnkIdPolyRecord& other) {
        id = other.id;
        field_0x8 = other.field_0x8;
        records = other.records;
        field_0x40 = other.field_0x40;
        field_0x60 = other.field_0x60;
        field_0x64 = other.field_0x64;
        field_0x68 = other.field_0x68;
        field_0x6c = other.field_0x6c;
        field_0x70 = other.field_0x70;
        fn_7100e4f444(&first, &other.first);
        fn_7100e4f444(&second, &other.second);
        return *this;
    }
};
static_assert(sizeof(UnkIdPolyRecord) == 0x138, "");

using UnkU32PolyRecordPair = UnkValuePairRef<u32, UnkIdPolyRecord>;

UnkU32PolyRecordPair* fn_7100e61b0c(UnkU32PolyRecordPair* self, UnkU32PolyRecordPair* other) { return Assign(self, other); }

// A callback set (as in src/game_s4/fn_71008947b8.cpp): a vtable, an array assigned by
// fn_710023b29c, a value and an owner reference assigned by fn_710031d208.
struct UnkCallbackArray {
    u64 unk_0x0[5];
};
UnkCallbackArray* fn_710023b29c(UnkCallbackArray* self, const UnkCallbackArray* other);

struct UnkCallbackOwner;
void fn_710031d208(UnkCallbackOwner** self, UnkCallbackOwner* const* other);

struct UnkCallbackSet {
    void* vtable;
    UnkCallbackArray callbacks;
    u32 field_0x30;
    UnkCallbackOwner* owner;

    __attribute__((always_inline)) UnkCallbackSet& operator=(const UnkCallbackSet& other) {
        UnkCallbackOwner* const* from = &other.owner;
        fn_710031d208(&owner, from);
        field_0x30 = other.field_0x30;
        fn_710023b29c(&callbacks, &other.callbacks);
        return *this;
    }
};

using UnkU32CallbacksPair = UnkValuePairRef<u32, UnkCallbackSet>;

UnkU32CallbacksPair* fn_7100d87614(UnkU32CallbacksPair* self, UnkU32CallbacksPair* other) { return Assign(self, other); }

// Three values and a callback set whose owner is a handle assigned by fn_7100234f78.
struct UnkCallbackHandle;
void fn_7100234f78(UnkCallbackHandle** self, UnkCallbackHandle* const* other);

struct UnkHandleCallbackSet {
    void* vtable;
    UnkCallbackArray callbacks;
    u32 field_0x30;
    UnkCallbackHandle* owner;

    __attribute__((always_inline)) UnkHandleCallbackSet& operator=(const UnkHandleCallbackSet& other) {
        UnkCallbackHandle* const* from = &other.owner;
        fn_7100234f78(&owner, from);
        field_0x30 = other.field_0x30;
        fn_710023b29c(&callbacks, &other.callbacks);
        return *this;
    }
};

struct UnkValuesCallbacks {
    u64 unk_0x0;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    UnkHandleCallbackSet set;

    __attribute__((always_inline)) UnkValuesCallbacks& operator=(const UnkValuesCallbacks& other) {
        field_0x8 = other.field_0x8;
        field_0xc = other.field_0xc;
        field_0x10 = other.field_0x10;
        set = other.set;
        return *this;
    }
};

using UnkU32ValuesCallbacksPair = UnkValuePairRef<u32, UnkValuesCallbacks>;

UnkU32ValuesCallbacksPair* fn_7100c0837c(UnkU32ValuesCallbacksPair* self, UnkU32ValuesCallbacksPair* other) {
    return Assign(self, other);
}
