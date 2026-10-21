// Handlers taking reflected values and returning a status record {0, 0, 2, 1}: calls with a
// value and an optional pointer, a bounds reset over point lists, a string-id array copy and
// erasing an element of an array of 0x58-byte polymorphic records.
#include <new>
#include <utility>

#include "base/UnkDestructors.h"
#include "types.h"

using base::UnkStrIdMember;

struct UnkValueType {
    u8 unk_0x0[0x18];
    u64 size;
};

// The value is stored in place when its type fits in 8 bytes, unless kind is 1 (by reference).
// The values a handler gets are consecutive.
struct UnkValueArg {
    void* storage;
    UnkValueType* type;
    u8 unk_0x10[0x4];
    s32 kind;

    template <class T>
    const T& Get() const {
        if (kind == 1 || type->size >= 9)
            return *static_cast<const T*>(storage);
        return *reinterpret_cast<const T*>(this);
    }
    // No value without a type.
    template <class T>
    const T* GetPtr() const {
        if (!type)
            return nullptr;
        if (kind == 1 || type->size >= 9)
            return static_cast<const T*>(storage);
        return reinterpret_cast<const T*>(this);
    }
};

struct UnkValueHolder {
    u8 unk_0x0[0x8];
    char* owner;
};

struct UnkSetStatus {
    u64 field_0x0;
    u64 field_0x8;
    u32 field_0x10;
    u32 field_0x14;
};

static inline UnkSetStatus Done() {
    UnkSetStatus status;
    status.field_0x0 = 0;
    status.field_0x8 = 0;
    status.field_0x10 = 2;
    status.field_0x14 = 1;
    return status;
}

struct UnkArgObj;

void fn_7100d7c828(char* owner, const UnkArgObj* obj, bool flag);

UnkSetStatus fn_7100d82864(void* self, UnkValueHolder* holder, UnkValueArg* const* arg) {
    const UnkValueArg* args = *arg;
    fn_7100d7c828(holder->owner, args[0].GetPtr<UnkArgObj>(), args[1].Get<bool>());
    return Done();
}

struct UnkObjSet;
void* fn_7100a2c13c(UnkObjSet* set, const UnkArgObj* obj);
void fn_7100a2b7b8(UnkObjSet* set, const UnkArgObj* obj);

struct UnkSetOwner {
    u8 unk_0x0[0x2e8];
    UnkObjSet* set;
};

UnkSetStatus fn_7100d82918(void* self, UnkValueHolder* holder, UnkValueArg* const* arg) {
    const UnkValueArg* args = *arg;
    UnkSetOwner* owner = reinterpret_cast<UnkSetOwner*>(holder->owner);
    const UnkArgObj* obj = args[0].GetPtr<UnkArgObj>();
    if (owner->set && fn_7100a2c13c(owner->set, obj))
        fn_7100a2b7b8(owner->set, obj);
    return Done();
}

struct UnkCopyEntry {
    UnkStrIdMember id;
    u8 unk_0x8[0x10];
};

struct UnkCopyOwner {
    u8 unk_0x0[0xd0];
    UnkCopyEntry* dst;
    UnkStrIdMember* src;
    u32 count;
};

// NON_MATCHING(minor): the source iterator and the pool pointer swap registers.
UnkSetStatus fn_7100e6e850(void* self, UnkValueHolder* holder, UnkValueArg* const* arg) {
    UnkCopyOwner* owner = reinterpret_cast<UnkCopyOwner*>(holder->owner);
    UnkCopyEntry* d = owner->dst;
    for (UnkStrIdMember* s = owner->src; s != owner->src + owner->count; ++s) {
        d->id = *s;
        ++d;
    }
    return Done();
}

// Resets the bounds to empty, then grows them by each point of each list (projected by
// fn_710007b430).
struct UnkVec2 {
    f32 x, y;
};
struct UnkVec3 {
    f32 x, y, z;
};
void fn_710007b430(UnkVec2* out, const UnkVec3* p);

struct UnkPointList {
    UnkVec3* points;
    u32 count;
    u8 unk_0xc[0x54];
};

struct UnkBoundsOwner {
    u8 unk_0x0[0x6028];
    UnkPointList* lists;
    u8 unk_0x6030[0x8];
    u32 count;
    u8 unk_0x603c[0x30c];
    UnkVec2 min, max;
};

static inline UnkVec2 Min(UnkVec2 a, UnkVec2 b) {
    UnkVec2 r;
    r.x = a.x < b.x ? a.x : b.x;
    r.y = a.y < b.y ? a.y : b.y;
    return r;
}
static inline UnkVec2 Max(UnkVec2 a, UnkVec2 b) {
    UnkVec2 r;
    r.x = a.x > b.x ? a.x : b.x;
    r.y = a.y > b.y ? a.y : b.y;
    return r;
}

// NON_MATCHING(minor): the comparisons of each point are scheduled differently.
UnkSetStatus fn_7100e51c54(void* self, UnkValueHolder* holder, UnkValueArg* const* arg) {
    UnkBoundsOwner* owner = reinterpret_cast<UnkBoundsOwner*>(holder->owner);
    owner->min.x = 3.4028235e38f;
    owner->min.y = 3.4028235e38f;
    owner->max.x = -3.4028235e38f;
    owner->max.y = -3.4028235e38f;
    for (UnkPointList* l = owner->lists, *e = l + owner->count; l != e; ++l) {
        for (UnkVec3 *p = l->points, *pe = p + l->count; p != pe; ++p) {
            UnkVec2 v;
            fn_710007b430(&v, p);
            owner->min = Min(owner->min, v);
            owner->max = Max(owner->max, v);
        }
    }
    return Done();
}

// An array of 0x58-byte records (destroyed through the vtable; moved inline, the member at
// 0x18 out of line). Erasing at the index given destroys it and moves the rest down.
struct UnkMovable {
    u8 unk_0x0[0x40];
    UnkMovable(UnkMovable&& other);  // fn_71002458a4
};

struct UnkRecord58 {
    virtual void vf_0x0();
    virtual ~UnkRecord58();
    u64 field_0x8;
    u32 field_0x10;
    UnkMovable field_0x18;

    UnkRecord58(UnkRecord58&& other)
        : field_0x8(other.field_0x8), field_0x10(other.field_0x10), field_0x18(std::move(other.field_0x18)) {}
};

struct UnkRecordArray {
    u8 unk_0x0[0x8];
    UnkRecord58* data;
    u32 count;
};

__attribute__((always_inline)) static inline UnkSetStatus EraseAt(UnkValueHolder* holder, UnkValueArg* const* arg) {
    UnkRecordArray* a = reinterpret_cast<UnkRecordArray*>(holder->owner);
    u32 index = (*arg)->Get<u32>();
    if (a->count > index) {
        a->data[index].~UnkRecord58();
        for (u64 i = index; i < a->count - 1; ++i) {
            new (&a->data[i]) UnkRecord58(std::move(a->data[i + 1]));
            a->data[i + 1].~UnkRecord58();
        }
        --a->count;
    }
    return Done();
}

// NON_MATCHING(minor): the original addresses the moved records from the next one (and loads
// the vtable later); one instruction shorter here.
UnkSetStatus fn_7100c07f58(void* self, UnkValueHolder* holder, UnkValueArg* const* arg) {
    return EraseAt(holder, arg);
}
// NON_MATCHING(minor): as fn_7100c07f58.
UnkSetStatus fn_7100c08060(void* self, UnkValueHolder* holder, UnkValueArg* const* arg) {
    return EraseAt(holder, arg);
}

struct UnkInsertTarget;
void fn_7100c0c10c(UnkInsertTarget* target, const UnkArgObj* obj, u32 value);

struct UnkInsertOwner {
    u8 unk_0x0[0x8];
    UnkInsertTarget* target;  // the object itself is at 8
    u8 unk_0x10[0x20];
    bool dirty;
};

UnkSetStatus fn_7100c07eb0(void* self, UnkValueHolder* holder, UnkValueArg* const* arg) {
    UnkInsertOwner* owner = reinterpret_cast<UnkInsertOwner*>(holder->owner);
    const UnkValueArg* args = *arg;
    u32 value = args[0].Get<u32>();
    const UnkArgObj* obj = args[1].GetPtr<UnkArgObj>();
    fn_7100c0c10c(reinterpret_cast<UnkInsertTarget*>(&owner->target), obj, value);
    owner->dirty = true;
    return Done();
}
