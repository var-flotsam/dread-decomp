// Unit 94 (base::serialization::CTypedRefWriter's unit): for the innermost context, finds the
// enclosing context it belongs to and, when that context's field names an element factory for
// this depth, builds the reflected call that creates the element. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/global/UnkStringPool.h"
#include "base/reflection.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::global::CRntString;
using base::global::CRntVector;
using base::global::CStrId;
using base::global::UnkStrIdBase;
using base::global::UnkStringData;
using base::global::UnkStringPool;
using base::reflection::CClass;
using base::reflection::CType;
using base::reflection::UnkReflAttrKey;
using base::reflection::UnkReflField;
using base::reflection::CFunction;
using base::reflection::UnkReflObject;
using base::reflection::UnkReflObjectB;
using base::reflection::UnkReflValue;

// The functions of a held type (only the destroy slot is named).
struct UnkHeldType_f29a0 {
    u8 pad_0x0[0x40];
    void (*destroy)(void* storage);
};

struct UnkTypedRefEntry_f29a0 {
    void* context;
    u8 storage[0xc8];
    UnkHeldType_f29a0* storage_type;  // 0xd0
};
static_assert(sizeof(UnkTypedRefEntry_f29a0) == 0xd8, "entry layout");

struct UnkTypedRefWriter {
    u32 field_0x0;
    u32 count;
    alignas(8) u8 storage[sizeof(UnkTypedRefEntry_f29a0)];

    UnkTypedRefEntry_f29a0* entries() { return reinterpret_cast<UnkTypedRefEntry_f29a0*>(storage); }
};

// One context of the stack: the value being read, the context it belongs to, the field it fills
// and the element factory found for it.
struct UnkReaderContext_l0f17c8 {
    u8 pad_0x0[0x8];
    UnkReflValue value;               // 0x8
    UnkReaderContext_l0f17c8* parent;  // 0x20
    u8 pad_0x28[0x58];
    UnkReflField* field;        // 0x80
    CFunction* function;  // 0x88
    UnkReflObjectB* call;       // 0x90
    bool field_0x98;
};
static_assert(__builtin_offsetof(UnkReaderContext_l0f17c8, field_0x98) == 0x98, "context layout");

// Sets the number of contexts: the ones dropped have their held storage destroyed, the ones added
// start empty.
static inline void SetCount(UnkTypedRefWriter* self, u32 count) {
    if (count < self->count) {
        for (u64 i = count; i < self->count; ++i) {
            UnkTypedRefEntry_f29a0& entry = self->entries()[i];
            if (entry.storage_type) {
                entry.storage_type->destroy(entry.storage);
                entry.storage_type = nullptr;
            }
        }
    } else {
        for (u64 i = self->count; i < count; ++i) {
            self->entries()[i].context = nullptr;
            self->entries()[i].storage_type = nullptr;
        }
    }
    self->count = count;
}

// The context at `index`, growing the stack to reach it.
static inline UnkReaderContext_l0f17c8* ContextAt(UnkTypedRefWriter* self, u32 index) {
    if (self->count < index + 1)
        SetCount(self, index + 1);
    return static_cast<UnkReaderContext_l0f17c8*>(self->entries()[index].context);
}

// A reflected value that owns what it holds.
struct UnkOwnedValue_l0f17c8 : UnkReflValue {
    UnkOwnedValue_l0f17c8() {
        field_0x0 = nullptr;
        type = nullptr;
        kind = 2;
        field_0x14 = 1;
    }
    // A copy of `other`'s value, or a reference to it.
    UnkOwnedValue_l0f17c8(const UnkReflValue& other, u32 by_ref) {
        field_0x0 = nullptr;
        type = nullptr;
        kind = other.kind;
        fn_71000a18d4(this, Type(other), other.type ? Address(other) : nullptr, by_ref);
    }
    // A reference to `value`, typed by its reflected type.
    template <class T>
    explicit UnkOwnedValue_l0f17c8(T& value) {
        field_0x0 = nullptr;
        type = nullptr;
        kind = 2;
        fn_71000a18d4(this, base::reflection::UnkReflTypeOf<T>::Get(), &value, true);
    }
    UnkOwnedValue_l0f17c8(UnkOwnedValue_l0f17c8&& other) { fn_71000a179c(this, &other); }
    ~UnkOwnedValue_l0f17c8() { fn_71000a1c60(this); }

    static const CType* Type(const UnkReflValue& v) { return reinterpret_cast<const CType*>(v.type); }
    // Where the value's data is: inline when it fits in 8 bytes, else through field_0x0.
    static const void* Address(const UnkReflValue& v) {
        return (v.field_0x14 == 1 || Type(v)->m_size > 8) ? v.field_0x0 : &v;
    }
};

// A field's attribute set: the values and their keys (the key names' string data), `count` of each.
struct UnkAttrSet_l0f17c8 {
    UnkReflValue* values;
    const void** keys;
    u32 count;

    const UnkReflValue* Find(const UnkReflAttrKey& key) const {
        for (u32 i = 0; i < count; ++i)
            if (keys[i] == key.name.data)
                return &values[i];
        return values + count;
    }
    const UnkReflValue* End() const { return values + count; }
};

// A string id made from a string's text out of line, released like CStrId.
struct UnkTextStrId_l0f17c8 {
    UnkStringData* data;

    explicit UnkTextStrId_l0f17c8(const CRntString& text) {
        CStrId_ConstructFromRntString(reinterpret_cast<UnkStrIdBase*>(this), text, true);
    }
    ~UnkTextStrId_l0f17c8() {
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
    const CStrId* Id() const { return reinterpret_cast<const CStrId*>(this); }
};

// Appends `value` to the call's arguments, growing the array by its step when it is full: the
// old elements are moved over and the old array freed.
template <class T>
inline void AppendArg_l0f17c8(CRntVector<T>& args, T&& value) {
    u32 n = args.count;
    T* data = args.data;
    if (n >= args.capacity) {
        u32 capacity = args.GrownCapacity(n);
        T* grown = static_cast<T*>(args.allocator->vf_0x40((u64)capacity * sizeof(T), 8, args.flags));
        if (data) {
            for (u32 i = 0; i < n; ++i) {
                new (&grown[i]) T(static_cast<T&&>(data[i]));
                data[i].~T();
            }
            args.allocator->vf_0x48(data);
        }
        args.data = data = grown;
        args.capacity = capacity;
    }
    new (&data[n]) T(static_cast<T&&>(value));
    args.count = n + 1;
}

struct UnkReflFunctionEx_7100095bdc;
void fn_7100095684(UnkReflFunctionEx_7100095bdc* self, void* owner, const UnkReflValue* value, void* x3);
void* fn_71000978e4(char* cls, const CStrId* name);
CFunction* fn_71000979a0(CClass* self, UnkReflField* field, const UnkReflAttrKey& key);

bool fn_71000f17c8(UnkTypedRefWriter* self) {
    auto* top = static_cast<UnkReaderContext_l0f17c8*>(self->entries()[self->count - 1].context);
    UnkReaderContext_l0f17c8* parent = top->parent;
    if (!parent)
        return true;

    // How many contexts below the top one the parent is.
    u32 depth;
    u32 below = 0;
    do {
        depth = below++;
    } while (ContextAt(self, self->count - below - 1) != parent);

    const CType* type = reinterpret_cast<const CType*>(parent->value.type);
    CClass* cls = nullptr;
    if (type && fn_710009d50c(reinterpret_cast<const UnkReflObject*>(type)->vf_0x0(), fn_7100096100()))
        cls = static_cast<CClass*>(const_cast<CType*>(type));

    top->function = fn_71000979a0(cls, parent->field, fn_710008fd60());

    const auto* attrs = reinterpret_cast<const UnkAttrSet_l0f17c8*>(&parent->field->attrs_0x70);
    const UnkReflValue* found = attrs->Find(fn_710008ff80());
    UnkOwnedValue_l0f17c8 names_value =
        found != attrs->End() ? UnkOwnedValue_l0f17c8(*found, false) : UnkOwnedValue_l0f17c8();
    if (names_value.type) {
        const auto* names = static_cast<const CRntVector<CRntString>*>(UnkOwnedValue_l0f17c8::Address(names_value));
        if (names->count > depth &&
            StringBuffer_Differs(reinterpret_cast<char*>(const_cast<CRntString*>(&names->data[depth])), g_unk_710162e2ed)) {
            UnkTextStrId_l0f17c8 name(names->data[depth]);
            void* owner = fn_71000978e4(reinterpret_cast<char*>(cls), name.Id());

            CRntVector<UnkOwnedValue_l0f17c8> args(0x59);
            AppendArg_l0f17c8(args, UnkOwnedValue_l0f17c8());
            AppendArg_l0f17c8(args, UnkOwnedValue_l0f17c8(top->field_0x98));

            {
                UnkOwnedValue_l0f17c8 target(parent->value, true);
                auto* call = static_cast<UnkReflFunctionEx_7100095bdc*>(operator new(0x128));
                fn_7100095684(call, owner, &target, &args);
                UnkReflObjectB* old = top->call;
                top->call = reinterpret_cast<UnkReflObjectB*>(call);
                if (old)
                    delete old;
            }
            args.Free();
        }
    }
    return true;
}
