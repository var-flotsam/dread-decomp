// A copy of fn_7101114f80 (src/game_s7/fn_7101114f80.cpp) with its own sync function: Begin of a
// reflected array of callback sets without the value after each set (fn_71002475b0). Its sets are
// UnkCallbackSet_7101a20950 (vtable 0x7101a20950, as in include/game/UnkCallbackSetVector.h). The
// two other copies that were here use other set classes: fn_7100d88e14 is in
// src/game_aca57c/fn_7100d88e14.cpp, fn_7101132350 in src/game_s7/fn_7101132350.cpp.
#include "base/UnkErasedVector.h"
#include "base/UnkVector.h"
#include "game/CChangeStateDoorsLogicAction.h"
#include "game/CCharClassWaterPoolComponent.h"
#include "game/UnkCallbackSet_7101a20950.h"
#include "game/UnkTU1601.h"
#include "game/UnkTU341.h"

struct UnkCallbackSetElem {
    UnkCallbackSet_7101a20950 set;
};

// The cached (index, element) value: empty (0), owned (1) or referring to the iterator's (2).
struct UnkIterValue {
    struct Value {
        u32 index;
        UnkCallbackSetElem element;
    };
    alignas(8) u8 storage[sizeof(Value)];
    u32 state;

    struct Ref {
        u32* index;
        UnkCallbackSetElem* element;
    };

    UnkIterValue() : state(0) {}
    // An owned value either way: a reference's element is moved into a value of its own.
    __attribute__((always_inline)) void MoveFrom(UnkIterValue& other) {
        state = 0;
        Value* value = reinterpret_cast<Value*>(storage);
        if (other.state == 1) {
            state = 1;
            Value* from = reinterpret_cast<Value*>(other.storage);
            value->index = from->index;
            new (&value->element) UnkCallbackSetElem(static_cast<UnkCallbackSetElem&&>(from->element));
        } else if (other.state == 2) {
            state = 1;
            const Ref* ref = reinterpret_cast<const Ref*>(other.storage);
            UnkCallbackSetElem* element = ref->element;
            value->index = *ref->index;
            new (&value->element) UnkCallbackSetElem(static_cast<UnkCallbackSetElem&&>(*element));
        }
    }
    ~UnkIterValue() {
        if (state == 2) {
        } else if (state == 1) {
            reinterpret_cast<Value*>(storage)->~Value();
        } else {
            return;
        }
        state = 0;
    }
};

// Sync is the iterator's sync function: re-points `current` at the iterator's index and
// element, or resets it (one per copy of the collection functions).
template <void (*Sync)(void*)>
struct UnkIter {
    u32 index;
    UnkCallbackSetElem* element;
    UnkIterValue current;

    UnkIter(u32 index, UnkCallbackSetElem* element);
    __attribute__((always_inline)) UnkIter& operator=(const UnkIter& other) {
        if (this != &other) {
            index = other.index;
            element = other.element;
            Sync(this);
        }
        return *this;
    }
};

template <void (*Sync)(void*)>
inline UnkIter<Sync>::UnkIter(u32 index, UnkCallbackSetElem* element) : index(index), element(element) {
    Sync(this);
}

// Reserve (one copy per array type).
struct UnkCallbackSetVector;
// PushBack of a copy (one per array type)
struct UnkCallbackSetVector {
    UnkCallbackSetElem* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    template <void (*Reserve)(UnkCallbackSetVector*, u32),
              void (*PushBack)(UnkCallbackSetVector*, const UnkCallbackSetElem*)>
    __attribute__((always_inline)) void CopyConstruct(const UnkCallbackSetVector& other) {
        data = nullptr;
        count = 0;
        capacity = 0;
        step = other.step;
        field_0x14 = other.field_0x14;
        flags = other.flags;
        allocator = other.allocator;
        Reserve(this, other.capacity);
        for (const UnkCallbackSetElem *p = other.data, *end = p + other.count; p != end; ++p)
            PushBack(this, p);
    }

    __attribute__((always_inline)) void Reserve(u32 n) {
        if (capacity >= n)
            return;
        UnkCallbackSetElem* p =
            static_cast<UnkCallbackSetElem*>(allocator->vf_0x40((u64)n * sizeof(UnkCallbackSetElem), 8, flags));
        if (data) {
            UnkCallbackSetElem* src = data;
            UnkCallbackSetElem* dst = p;
            for (u32 i = 0; i < count; ++i, ++dst, ++src) {
                new (dst) UnkCallbackSetElem(static_cast<UnkCallbackSetElem&&>(*src));
                src->~UnkCallbackSetElem();
            }
            allocator->vf_0x48(data);
        }
        data = p;
        capacity = n;
    }

    __attribute__((always_inline)) void Erase(u32 index) {
        if (index >= count)
            return;
        data[index].~UnkCallbackSetElem();
        for (u64 i = index; i < count - 1; ++i) {
            UnkCallbackSetElem* e = &data[i];
            new (e) UnkCallbackSetElem(static_cast<UnkCallbackSetElem&&>(e[1]));
            (&data[i])[1].~UnkCallbackSetElem();  // not data[i + 1], which indexes differently
        }
        --count;
    }

    __attribute__((always_inline)) u32 GrownCapacity() const {
        u32 n = capacity;
        if (n <= count && step != 0)
            n = (count / step + 1) * step;
        return n;
    }

    // Reserve is this array type's out-of-line copy of Reserve.
    template <void (*Reserve)(UnkCallbackSetVector*, u32)>
    __attribute__((always_inline)) void Insert(UnkCallbackSetElem&& value, u32 index) {
        if (index == count) {
            u32 n = count;
            u32 new_capacity = capacity;
            if (new_capacity <= n && step != 0)
                new_capacity = (n / step + 1) * step;
            if ((s32)n >= (s32)new_capacity)  // hangs, as UnkVector::PushBack does
                for (;;) {
                }
            Reserve(this, new_capacity);
            new (&data[count]) UnkCallbackSetElem(static_cast<UnkCallbackSetElem&&>(value));
        } else {
            Reserve(this, GrownCapacity());
            for (u32 i = count - 1; i >= index; --i) {
                UnkCallbackSetElem* e = &data[i];
                new (&e[1]) UnkCallbackSetElem(static_cast<UnkCallbackSetElem&&>(*e));
                data[i].~UnkCallbackSetElem();
                if (i == 0)
                    break;
            }
            new (&data[index]) UnkCallbackSetElem(static_cast<UnkCallbackSetElem&&>(value));
        }
        ++count;
    }

    __attribute__((always_inline)) void Truncate(u32 size) {
        for (u64 i = size; i < count; ++i)
            data[i].~UnkCallbackSetElem();
        count = size;
    }
};

template <void (*Sync)(void*)>
static inline __attribute__((always_inline)) void Begin(const UnkCallbackSetVector* v, UnkIter<Sync>* out) {
    *out = UnkIter<Sync>(0, v->data);
}

void fn_71002475b0(const UnkCallbackSetVector* v, UnkIter<fn_710023ac4c>* out) {
    Begin(v, out);
}
