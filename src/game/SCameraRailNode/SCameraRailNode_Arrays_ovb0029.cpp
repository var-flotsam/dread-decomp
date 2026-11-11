// SCameraRailNode's unit (tools/tu_map.py, the name from reflection): the reflected function set-up
// of a camera-rail node, and the array helpers around its records: element moves and erasure,
// iterator Begin/End (their cached value re-pointed by fn_7100c0621c / fn_7100c051e8), the cached
// (index, element) value's move constructor and move assignment, and an array of word pairs (copy
// constructor, insertion, grow-to-index). Names and layouts are placeholders.
#include <new>

#include "base/UnkCloneHelpers.h"
#include "base/UnkCollectionIter.h"
#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "base/UnkVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "game/SCameraRailNode.h"
#include "game/UnkTU341.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflFunction;
using base::reflection::UnkReflValue;
using base::UnkAllocator;

extern char g_unk_717277cb68[] __attribute__((visibility("hidden")));

UnkReflValue fn_7100c07c58(void*, char* obj, UnkReflValue* const* args);
void fn_710011ddc0(UnkReflFunction* fn);

// Sets up a reflected function taking a camera-rail node: its name, the node class as its return
// type, its invoker fn_7100c07c58 (src/game/SCameraRailNode/SCameraRailNode_b40.cpp), then its
// parameters (fn_710011ddc0).
void fn_7100bfff5c(UnkReflFunction* fn, const char* name) {
    fn->name = base::global::UnkStrId(name, true);
    fn_7100bfe80c();
    fn->return_type = reinterpret_cast<const CType*>(g_unk_717277cb68);
    fn->field_0xb2 = 0x101;
    fn->field_0x40 = nullptr;
    fn->invoker = reinterpret_cast<void*>(&fn_7100c07c58);
    fn_710011ddc0(fn);
}

// Pushes two parameter types on a reflected function: unsigned and the camera-rail node class.
void fn_7100c07d10(UnkReflFunction* fn) {
    fn->params.PushBack(fn_7100005178());
    fn_7100bfe80c();
    fn->params.PushBack(reinterpret_cast<const CType*>(g_unk_717277cb68));
}

// An array moved by taking over its storage (the source keeps its step and allocator).
struct UnkMovedArray_b29 {
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    void* allocator;

    UnkMovedArray_b29(UnkMovedArray_b29&& other)
        : data(other.data), count(other.count), capacity(other.capacity), step(other.step),
          field_0x14(other.field_0x14), flags(other.flags), allocator(other.allocator) {
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
};

struct UnkElem50_b29;

struct UnkElem50Vtbl_b29 {
    void* vf_0x0;
    void (*Destroy)(UnkElem50_b29* self);
};

struct UnkElem50_b29 {
    const UnkElem50Vtbl_b29* vtable;
    UnkMovedArray_b29 field_0x8;
    bool field_0x30;
    u32 field_0x34[6];

    UnkElem50_b29(UnkElem50_b29&& other)
        : vtable(reinterpret_cast<const UnkElem50Vtbl_b29*>(&g_unk_7101aa28d8[2])),
          field_0x8(static_cast<UnkMovedArray_b29&&>(other.field_0x8)), field_0x30(other.field_0x30),
          field_0x34() {}
    void Destroy() { vtable->Destroy(this); }
};
static_assert(sizeof(UnkElem50_b29) == 0x50, "UnkElem50_b29 layout");

void fn_7100c07b00(UnkElem50_b29* self, UnkElem50_b29* other) {
    new (self) UnkElem50_b29(static_cast<UnkElem50_b29&&>(*other));
}

struct UnkElem50Array_b29 {
    UnkElem50_b29* data;
    u32 count;

    void Erase(u32 index) {
        if (index >= count)
            return;
        data[index].Destroy();
        for (u64 i = index; i < count - 1; ++i) {
            UnkElem50_b29* e = &data[i];
            new (e) UnkElem50_b29(static_cast<UnkElem50_b29&&>(e[1]));
            (&data[i])[1].Destroy();
        }
        --count;
    }
};

void fn_7100c0a0a4(UnkElem50Array_b29* self, const u32* index) { self->Erase(*index); }

// An (index, element) value: empty (0), owned (1) or referring to another's (2).
struct UnkElem50Value_b29 {
    struct Value {
        u32 index;
        UnkElem50_b29 element;
    };
    struct Ref {
        u32* index;
        UnkElem50_b29* element;
    };
    alignas(8) u8 storage[sizeof(Value)];
    u32 state;
};

void fn_7100c09a44(UnkElem50Value_b29* self, UnkElem50Value_b29* other) {
    self->state = 0;
    UnkElem50Value_b29::Value* value = reinterpret_cast<UnkElem50Value_b29::Value*>(self->storage);
    if (other->state == 1) {
        self->state = 1;
        UnkElem50Value_b29::Value* from = reinterpret_cast<UnkElem50Value_b29::Value*>(other->storage);
        value->index = from->index;
        new (&value->element) UnkElem50_b29(static_cast<UnkElem50_b29&&>(from->element));
    } else if (other->state == 2) {
        self->state = 1;
        const UnkElem50Value_b29::Ref* ref = reinterpret_cast<const UnkElem50Value_b29::Ref*>(other->storage);
        UnkElem50_b29* element = ref->element;
        value->index = *ref->index;
        new (&value->element) UnkElem50_b29(static_cast<UnkElem50_b29&&>(*element));
    }
}

struct UnkRailNodeB_b29;

struct UnkRailNodeBVtbl_b29 {
    void* vf_0x0;
    void (*Destroy)(UnkRailNodeB_b29* self);
};

struct UnkRailNodeB_b29 {
    const UnkRailNodeBVtbl_b29* vtable;
    u8 unk_0x8[0x50];
    void Destroy() { vtable->Destroy(this); }
};

struct UnkNodeArray_b29 {
    UnkRailNodeB_b29* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    UnkAllocator* allocator;

    ~UnkNodeArray_b29() {
        if (data) {
            for (u64 i = 0; i < count; ++i)
                data[i].Destroy();
            UnkRailNodeB_b29* p = data;
            count = 0;
            allocator->vf_0x48(p);
            capacity = 0;
        }
    }
};

struct UnkElem50D_b29 {
    void* vtable;
    UnkNodeArray_b29 nodes;
    bool field_0x30;
    u32 field_0x34[6];

    ~UnkElem50D_b29() { vtable = &g_unk_7101aa28d8[2]; }
};
static_assert(sizeof(UnkElem50D_b29) == 0x50, "UnkElem50D_b29 layout");

struct UnkElem50DValue_b29 {
    struct Value {
        u32 index;
        UnkElem50D_b29 element;
    };
    alignas(8) u8 storage[sizeof(Value)];
    u32 state;

    UnkElem50DValue_b29() : state(0) {}
    ~UnkElem50DValue_b29() {
        if (state == 2) {
        } else if (state == 1) {
            reinterpret_cast<Value*>(storage)->~Value();
        } else {
            return;
        }
        state = 0;
    }
};

void fn_7100c0621c(char* self);

struct UnkElem50Iter_b29 {
    u32 index;
    UnkElem50D_b29* element;
    UnkElem50DValue_b29 current;

    UnkElem50Iter_b29(u32 index_, UnkElem50D_b29* element_) : index(index_), element(element_) {
        fn_7100c0621c(reinterpret_cast<char*>(this));
    }
    UnkElem50Iter_b29& operator=(const UnkElem50Iter_b29& other) {
        if (this != &other) {
            index = other.index;
            element = other.element;
            fn_7100c0621c(reinterpret_cast<char*>(this));
        }
        return *this;
    }
};

struct UnkElem50DArray_b29 {
    UnkElem50D_b29* data;
    u32 count;
};

void fn_7100c0ab28(const UnkElem50DArray_b29* v, UnkElem50Iter_b29* out) { *out = UnkElem50Iter_b29(0, v->data); }

void fn_7100c0ac28(const UnkElem50DArray_b29* v, UnkElem50Iter_b29* out) { *out = {v->count, v->data + v->count}; }

struct UnkRailRecord_b29 {
    virtual void vf_0x0();
    virtual ~UnkRailRecord_b29();
    u8 unk_0x8[0x50];
};

struct UnkRailElem_b29 {
    void* vtable;
    base::UnkVector<UnkRailRecord_b29> records;
    bool field_0x30;
    u32 field_0x34[6];

    UnkRailElem_b29& operator=(UnkRailElem_b29&& other) {
        records = static_cast<base::UnkVector<UnkRailRecord_b29>&&>(other.records);
        field_0x30 = other.field_0x30;
        return *this;
    }
};
static_assert(sizeof(UnkRailElem_b29) == 0x50, "UnkRailElem_b29 layout");

using UnkRailElemValue_b29 = base::UnkIterValue<UnkRailElem_b29, u32>;

// The element slot holds either the element or (state 2) a pointer to it.
static inline UnkRailElem_b29* ElementOf(UnkRailElemValue_b29* v) {
    UnkRailElem_b29* slot = &v->value.element;
    return v->state == 2 ? *reinterpret_cast<UnkRailElem_b29**>(slot) : slot;
}

// Move-assigns the (index, element) pair the other value holds or refers to.
UnkRailElemValue_b29* fn_7100c09c34(UnkRailElemValue_b29* self, UnkRailElemValue_b29* other) {
    if (other->state == 1) {
        self->Index() = other->value.index;
        *ElementOf(self) = static_cast<UnkRailElem_b29&&>(other->value.element);
    } else if (other->state == 2) {
        self->Index() = *other->ref.index;
        *ElementOf(self) = static_cast<UnkRailElem_b29&&>(*other->ref.element);
    }
    return self;
}

struct UnkCameraRailCallbacks_b29;
template <>
struct base::UnkCallbackTraits<UnkCameraRailCallbacks_b29> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a20950); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};


// A camera-rail node: three words and a callback list.
struct UnkRailNodeC_b29 {
    char* vtable;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    base::UnkCallbackListShort<UnkCameraRailCallbacks_b29> set;

    ~UnkRailNodeC_b29() { vtable = g_unk_7101aa2888 + 0x10; }
};
static_assert(sizeof(UnkRailNodeC_b29) == 0x58, "UnkRailNodeC_b29 layout");

void fn_7100c051e8(void* self);

struct UnkRailNodeIter_b29 {
    u32 index;
    UnkRailNodeC_b29* element;
    base::UnkIterValue<UnkRailNodeC_b29, u32> current;

    UnkRailNodeIter_b29(u32 index, UnkRailNodeC_b29* element) : index(index), element(element) {
        fn_7100c051e8(this);
    }
    UnkRailNodeIter_b29& operator=(const UnkRailNodeIter_b29& other) {
        if (this != &other) {
            index = other.index;
            element = other.element;
            fn_7100c051e8(this);
        }
        return *this;
    }
};

void fn_7100c09774(const base::UnkVector<UnkRailNodeC_b29>* v, UnkRailNodeIter_b29* out) {
    *out = UnkRailNodeIter_b29(0, v->data);
}

// The braces evaluate the count before the data, as the original does.
void fn_7100c098c0(const base::UnkVector<UnkRailNodeC_b29>* v, UnkRailNodeIter_b29* out) {
    *out = UnkRailNodeIter_b29{v->count, v->data + v->count};
}

// A camera-rail node: three words and a callback set (base vtable, list, a word, an owner).
struct UnkRailNodeM_b29 {
    void* vtable;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    void* set_vtable;           // 0x18
    UnkErasedVector callbacks;  // 0x20
    u32 field_0x48;
    base::UnkRefHandle owner;  // 0x50

    UnkRailNodeM_b29& operator=(UnkRailNodeM_b29&& other) {
        field_0x8 = other.field_0x8;
        field_0xc = other.field_0xc;
        field_0x10 = other.field_0x10;
        fn_7100234f78(&owner, &other.owner);
        UnkErasedVector* list = &other.callbacks;
        field_0x48 = other.field_0x48;
        fn_710023b29c(&callbacks, list);
        other.field_0x48 = 0;
        fn_7100ad3588(list);
        if (base::UnkRefCounted* p = other.owner.ptr) {
            if (--p->ref_count == 0)
                delete p;
            other.owner.ptr = nullptr;
        }
        return *this;
    }
};
static_assert(sizeof(UnkRailNodeM_b29) == 0x58, "UnkRailNodeM_b29 layout");

using UnkRailNodeValue_b29 = base::UnkIterValue<UnkRailNodeM_b29, u32>;

// The element slot holds either the element or (state 2) a pointer to it.
static inline UnkRailNodeM_b29* ElementOf(UnkRailNodeValue_b29* v) {
    UnkRailNodeM_b29* slot = &v->value.element;
    return v->state == 2 ? *reinterpret_cast<UnkRailNodeM_b29**>(slot) : slot;
}

// Move-assigns the (index, node) pair the other value holds or refers to.
UnkRailNodeValue_b29* fn_7100c08480(UnkRailNodeValue_b29* self, UnkRailNodeValue_b29* other) {
    if (other->state == 1) {
        self->Index() = other->value.index;
        *ElementOf(self) = static_cast<UnkRailNodeM_b29&&>(other->value.element);
    } else if (other->state == 2) {
        self->Index() = *other->ref.index;
        *ElementOf(self) = static_cast<UnkRailNodeM_b29&&>(*other->ref.element);
    }
    return self;
}

// A pair of words, copied and cleared word by word through untyped copies (the original reloads
// the array's fields after each store).
struct UnkWordPair_b29 {
    u32 words[2];

    UnkWordPair_b29() {
        __builtin_memset(&words[0], 0, sizeof(u32));
        __builtin_memset(&words[1], 0, sizeof(u32));
    }
    UnkWordPair_b29(const UnkWordPair_b29& other) {
        __builtin_memcpy(&words[0], &other.words[0], sizeof(u32));
        __builtin_memcpy(&words[1], &other.words[1], sizeof(u32));
    }
};

// An engine array (base::UnkVector's layout) of word pairs.
struct UnkWordPairVector_b29 {
    UnkWordPair_b29* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool growable;
    u32 flags;
    UnkAllocator* allocator;

    __attribute__((always_inline)) void Reserve(u32 n) {
        if (capacity >= n)
            return;
        UnkWordPair_b29* p =
            static_cast<UnkWordPair_b29*>(allocator->vf_0x40((u64)n * sizeof(UnkWordPair_b29), 8, flags));
        if (data) {
            UnkWordPair_b29* src = data;
            UnkWordPair_b29* dst = p;
            for (u32 i = 0; i < count; ++i, ++dst, ++src)
                new (dst) UnkWordPair_b29(*src);
            allocator->vf_0x48(data);
        }
        data = p;
        capacity = n;
    }

    __attribute__((always_inline)) u32 GrownCapacity() const {
        u32 n = capacity;
        if (n <= count && step != 0)
            n = (count / step + 1) * step;
        return n;
    }

    __attribute__((always_inline)) void Insert(const UnkWordPair_b29& value, u32 index) {
        if (index == count) {
            u32 n = count;
            u32 new_capacity = capacity;
            if (new_capacity <= n && step != 0)
                new_capacity = (n / step + 1) * step;
            if ((s32)n >= (s32)new_capacity)  // hangs, as UnkVector::PushBack does
                for (;;) {
                }
            Reserve(new_capacity);
            new (&data[count]) UnkWordPair_b29(value);
        } else {
            Reserve(GrownCapacity());
            for (u32 i = count - 1; i >= index; --i) {
                UnkWordPair_b29* e = &data[i];
                new (&e[1]) UnkWordPair_b29(*e);
                if (i == 0)
                    break;
            }
            new (&data[index]) UnkWordPair_b29(value);
        }
        ++count;
    }

    __attribute__((always_inline)) void PushBack(const UnkWordPair_b29& value) {
        u32 n = capacity;
        if (n <= count && step != 0)
            n = (count / step + 1) * step;
        if ((s32)count >= (s32)n)  // hangs, as UnkVector::PushBack does
            for (;;) {
            }
        Reserve(n);
        new (&data[count]) UnkWordPair_b29(value);
        ++count;
    }
};

// The copy constructor: an empty array with the other's step, flags and allocator, reserved to
// the other's capacity, then a copy of each pair appended.
void fn_7100c0b5bc(UnkWordPairVector_b29* self, const UnkWordPairVector_b29* other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->growable = other->growable;
    self->flags = other->flags;
    self->allocator = other->allocator;
    self->Reserve(other->capacity);
    for (const UnkWordPair_b29 *it = other->data, *end = other->data + other->count; it != end; ++it)
        self->PushBack(*it);
}

// The pair at `index`, a growable array first grown to hold it (new pairs cleared).
UnkWordPair_b29* fn_7100c0bdbc(UnkWordPairVector_b29* self, u32 index) {
    if (self->growable && self->count <= index) {
        u32 n = self->capacity;
        if (n <= index && self->step != 0)
            n = (index / self->step + 1) * self->step;
        self->Reserve(n);
        self->Reserve(index + 1);
        for (u32 i = self->count; i < index + 1; ++i)
            new (&self->data[i]) UnkWordPair_b29();
        self->count = index + 1;
    }
    return &self->data[index];
}

void fn_7100c0b98c(UnkWordPairVector_b29* self, const UnkWordPair_b29* value, u32 index) { self->Insert(*value, index); }

// The same insertion of a pair given to be moved (a pair moves as it copies).
void fn_7100c0bba4(UnkWordPairVector_b29* self, UnkWordPair_b29* value, u32 index) { self->Insert(*value, index); }
