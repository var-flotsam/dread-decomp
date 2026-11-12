// Unit 1601 (CThermalDeviceComponent): the copy assignment of the slot that holds a callback set inline or behind a
// pointer, the callback-set array's push of a copy and its copy and move "set at index" slots, and the component's
// destructor. Names and layouts are placeholders.
#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "base/UnkVector.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/CChangeStateDoorsLogicAction.h"
#include "game/CCharClassWaterPoolComponent.h"
#include "game/CUsableComponent.h"
#include "game/UnkTU1601.h"
#include "game/UnkTU341.h"
#include "types.h"



struct UnkRefCounted {
    virtual void vf0();
    virtual void vf8();
    virtual void Delete();
    union {
        s32 ref_count;
    };
};

struct UnkCallbackBase {
    virtual void vf0();
    UnkOwnedErasedVector callbacks;
    u32 field_0x30;

    UnkCallbackBase() : field_0x30(0) {}
};

struct UnkCallbackSet : UnkCallbackBase {
    virtual void vf8();
    UnkRefCounted* owner;

    UnkCallbackSet(const UnkCallbackSet& other) : owner(nullptr) {
        UnkRefCounted* p = other.owner;
        if (p != owner) {
            if (p)
                ++p->ref_count;
            if (UnkRefCounted* old = owner) {
                if (--old->ref_count == 0)
                    old->Delete();
            }
            owner = p;
        }
        field_0x30 = other.field_0x30;
        fn_710023b29c(&callbacks, &other.callbacks);
    }

    UnkCallbackSet(UnkCallbackSet&& other);  // fn_71007b1928

    __attribute__((always_inline)) ~UnkCallbackSet() {
        if (UnkRefCounted* p = owner) {
            if (--p->ref_count == 0)
                p->Delete();
        }
    }
};

struct UnkCallbackSetElem {
    UnkCallbackSet set;
    u32 field_0x40;
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

    // Reserve is this array type's out-of-line copy of Reserve.
    template <void (*Reserve)(UnkCallbackSetVector*, u32)>
    __attribute__((always_inline)) void PushBackCopy(const UnkCallbackSetElem& value) {
        u32 n = count;
        u32 new_capacity = capacity;
        if (new_capacity <= n && step != 0)
            new_capacity = (n / step + 1) * step;
        if ((s32)n >= (s32)new_capacity)  // hangs, as UnkVector::PushBack does
            for (;;) {
            }
        Reserve(this, new_capacity);
        new (&data[count]) UnkCallbackSetElem(value);
        ++count;
    }

    __attribute__((always_inline)) void Truncate(u32 size) {
        for (u64 i = size; i < count; ++i)
            data[i].~UnkCallbackSetElem();
        count = size;
    }
};

// A value that sits inline in the slot or, with the kind 2 at +0x50, behind a pointer.
struct UnkThermalValue_b0237 {
    u8 pad_0x0[8];
    u8 list[0x28];  // 0x8, an UnkErasedVector
    u32 field_0x30;
    u8 pad_0x34[4];
    base::UnkRefHandle handle;  // 0x38
    u32 field_0x40;
};
static_assert(__builtin_offsetof(UnkThermalValue_b0237, field_0x40) == 0x40, "UnkThermalValue_b0237 layout");

struct UnkThermalSlot_b0237 {
    u8 storage[0x50];
    u32 kind;  // 0x50
};

static inline u32* ThermalFirst(UnkThermalSlot_b0237* slot) {
    if (slot->kind == 2)
        return *reinterpret_cast<u32**>(slot->storage);
    return reinterpret_cast<u32*>(slot->storage);
}

static inline UnkThermalValue_b0237* ThermalSecond(UnkThermalSlot_b0237* slot) {
    UnkThermalValue_b0237* inline_value = reinterpret_cast<UnkThermalValue_b0237*>(slot->storage + 8);
    if (slot->kind == 2)
        return *reinterpret_cast<UnkThermalValue_b0237**>(inline_value);
    return inline_value;
}

UnkThermalSlot_b0237* fn_7100a94ac4(UnkThermalSlot_b0237* self, UnkThermalSlot_b0237* other) {
    if (self == other)
        return self;
    u32 value = *ThermalFirst(other);
    *ThermalFirst(self) = value;
    UnkThermalValue_b0237* src = ThermalSecond(other);
    UnkThermalValue_b0237* dst = ThermalSecond(self);
    const base::UnkRefHandle* src_handle = &src->handle;
    fn_710031d208(&dst->handle, src_handle);
    dst->field_0x30 = src->field_0x30;
    fn_710023b29c(reinterpret_cast<UnkErasedVector*>(dst->list), reinterpret_cast<UnkErasedVector*>(src->list));
    dst->field_0x40 = src->field_0x40;
    return self;
}


void fn_7100a95868(UnkCallbackSetVector* v, const UnkCallbackSetElem* value) {
    v->PushBackCopy<fn_7100a956b8>(*value);
}

struct UnkSetElem_b0237 {
    void* vtable;
    UnkErasedValue_0x10* list_data;  // 0x8, an UnkErasedVector written as flat members
    u32 list_count;
    u32 list_capacity;
    u32 list_step;
    bool list_grow;
    u32 list_flags;
    base::UnkAllocator* list_allocator;
    u32 field_0x30;
    base::UnkRefHandle owner;  // 0x38
    u32 field_0x40;

    UnkErasedVector* List() { return reinterpret_cast<UnkErasedVector*>(&list_data); }
    const UnkErasedVector* List() const { return reinterpret_cast<const UnkErasedVector*>(&list_data); }

    __attribute__((always_inline)) void Construct() {
        vtable = reinterpret_cast<char*>(g_unk_7101a20998) + 0x10;
        list_data = nullptr;
        list_count = 0;
        list_capacity = 0;
        list_step = 10;
        list_grow = true;
        list_flags = 0xf;
        list_allocator = fn_7100083990();
        field_0x30 = 0;
        vtable = reinterpret_cast<char*>(g_unk_7101a33fa0) + 0x10;
        owner.ptr = nullptr;
    }
};
static_assert(sizeof(UnkSetElem_b0237) == 0x48, "layout");


struct UnkSetVector_b0237 {
    UnkSetElem_b0237* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool growable;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) void GrowTo(u32 index) {
        if (growable && count <= index) {
            u32 n = capacity;
            if (n <= index && step != 0)
                n = (index / step + 1) * step;
            fn_7100a956b8(reinterpret_cast<UnkCallbackSetVector*>(this), n);
            fn_7100a956b8(reinterpret_cast<UnkCallbackSetVector*>(this), index + 1);
            for (u32 i = count; i < index + 1; ++i)
            {
                UnkSetElem_b0237* e = &data[i];
                __builtin_memset(e, 0, sizeof(UnkSetElem_b0237));
                e->Construct();
            }
            count = index + 1;
        }
    }
};

void fn_7100a9514c(UnkSetVector_b0237* self, const u32* index, const UnkSetElem_b0237* value) {
    u32 i = *index;
    self->GrowTo(i);
    const base::UnkRefHandle* src = &value->owner;
    UnkSetElem_b0237* e = &self->data[i];
    fn_710031d208(&e->owner, src);
    e->field_0x30 = value->field_0x30;
    fn_710023b29c(e->List(), value->List());
    e->field_0x40 = value->field_0x40;
}

// The move slot: the given entry's list is emptied (fn_7100ad3588) and its owner released after the assignment.
void fn_7100a952b8(UnkSetVector_b0237* self, const u32* index, UnkSetElem_b0237* value) {
    u32 i = *index;
    self->GrowTo(i);
    UnkSetElem_b0237* data = self->data;
    const base::UnkRefHandle* src = &value->owner;
    UnkSetElem_b0237* e = &data[i];
    fn_710031d208(&e->owner, src);
    UnkErasedVector* list = value->List();
    e->field_0x30 = value->field_0x30;
    fn_710023b29c(e->List(), list);
    value->field_0x30 = 0;
    fn_7100ad3588(list);
    if (base::UnkRefCounted* p = value->owner.ptr) {
        if (--p->ref_count == 0)
            delete p;
        value->owner.ptr = nullptr;
    }
    data[i].field_0x40 = value->field_0x40;
}

#include "base/global/CStrId.h"

using base::global::CStrId;

struct UnkThermalComp_b0237 {
    void** vtable;
    u8 pad_0x8[0x540];
    union {
        UnkCallbackSetVector devices;  // 0x548
        struct {
            UnkCallbackSetElem* devices_data;  // the same array written as flat members
            u32 devices_count;
            u32 devices_capacity;
            u32 devices_step;
            bool devices_grow;
            u32 devices_flags;
            base::UnkAllocator* devices_allocator;
        };
    };
    CStrId id_0x570;
    CStrId id_0x578;
    CStrId id_0x580;
    CStrId id_0x588;
    CStrId id_0x590;
    u32 field_0x598;
    u8 pad_0x59c[1];
    u8 field_0x59d;
};
static_assert(__builtin_offsetof(UnkThermalComp_b0237, id_0x570) == 0x570, "UnkThermalComp_b0237 layout");

void fn_7100a91f00(UnkThermalComp_b0237* self) {
    self->vtable = g_unk_7101a92ed0 + 2;
    self->id_0x590.Reset();
    self->id_0x588.Reset();
    self->id_0x580.Reset();
    self->id_0x578.Reset();
    self->id_0x570.Reset();
    if (self->devices.data) {
        fn_7100a959a0(&self->devices, 0);
        self->devices.allocator->vf_0x48(self->devices.data);
        self->devices.capacity = 0;
    }
    fn_7100a9e6bc(reinterpret_cast<char*>(self));
}
