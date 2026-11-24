// Unit 94 (base::serialization::CTypedRefWriter's unit): has the innermost context's reader (its own,
// or the one its class names under "AfterElementDeserialization") read a copy of the context's value.
// Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/global/UnkStringPool.h"
#include "base/reflection.h"
#include "base/reflection/CType.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStrIdBase;
using base::reflection::CClass;
using base::reflection::CType;
using base::reflection::UnkReflArray;
using base::reflection::UnkReflObject;
using base::reflection::UnkReflTypedPtr;
using base::reflection::UnkReflValue;

struct UnkTypedRefWriter;

// What a reader is handed: a copy of the value, owned by the caller.
struct UnkReaderKey_f07fc {
    UnkReflValue* value;
    u64 owned;
};

// What a reader returns: a reflected value it owns.
struct UnkReadResult_f07fc : UnkReflValue {
    ~UnkReadResult_f07fc() { fn_71000a1c60(this); }
};

// Reads a context's value (only the slot this calls is named).
struct UnkReader_f07fc {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual UnkReadResult_f07fc vf_0x10(const UnkReflTypedPtr& source, const UnkReaderKey_f07fc& key, u32 flags);
};

// The readers a class keeps by name (CClass::m_array_0x110), 0xb8 bytes each.
struct UnkClassReader_f07fc : UnkReader_f07fc {
    u8 pad_0x8[0xb0];
};
static_assert(sizeof(UnkClassReader_f07fc) == 0xb8, "reader layout");

struct UnkWriterContext_f07fc {
    virtual void vf_0x0();
    UnkReflValue value;  // 0x8
    u8 pad_0x20[0x58];
    UnkReader_f07fc* reader;  // 0x78
};
static_assert(__builtin_offsetof(UnkWriterContext_f07fc, reader) == 0x78, "context layout");

struct UnkWriterEntry_f07fc {
    UnkWriterContext_f07fc* context;
    u8 storage[0xc8];
    const CType* storage_type;
};
static_assert(sizeof(UnkWriterEntry_f07fc) == 0xd8, "entry layout");

// The writer's context stack: a count and storage for 100 entries.
struct UnkContextStack_f07fc {
    u32 count;
    u8 storage[100 * sizeof(UnkWriterEntry_f07fc)];

    UnkWriterEntry_f07fc* entries() { return reinterpret_cast<UnkWriterEntry_f07fc*>(storage); }
    UnkWriterContext_f07fc* Top() { return entries()[count - 1].context; }
};

// The writer with its whole context stack.
struct UnkTypedRefWriterFull_f07fc {
    u32 field_0x0;
    UnkContextStack_f07fc stack;  // 0x4
    bool field_0x5468;            // readers are run
};
static_assert(__builtin_offsetof(UnkTypedRefWriterFull_f07fc, field_0x5468) == 0x5468, "writer layout");

// The name of a reader, as the class attribute holds it.
struct UnkReaderName_f07fc {
    const CRntString* string;
    u32 length;
};

// A string id built from a string out of line (fn_7100000850), released like CStrId.
struct UnkReaderId_f07fc {
    UnkStringData* data;

    explicit UnkReaderId_f07fc(const CRntString& str) {
        CStrId_ConstructFromRntString(reinterpret_cast<UnkStrIdBase*>(this), str, true);
    }
    ~UnkReaderId_f07fc() { reinterpret_cast<CStrId*>(this)->~CStrId(); }
};

// Where a reflected value's data is: inline when it fits in 8 bytes, else through field_0x0.
static inline void* ValueAddress(UnkReflValue& value) {
    if (!value.type)
        return nullptr;
    if (value.field_0x14 == 1 || reinterpret_cast<const CType*>(value.type)->m_size > 8)
        return value.field_0x0;
    return &value;
}

// The value's type and address.
static inline UnkReflTypedPtr TypedAddress(UnkReflValue& value) {
    UnkReflTypedPtr r;
    r.type = const_cast<char*>(value.type);
    r.ptr = ValueAddress(value);
    return r;
}

// The reader the class names under "AfterElementDeserialization", if any.
static inline UnkReader_f07fc* FindClassReader(const CType* type) {
    UnkReader_f07fc* reader = nullptr;
    UnkReflValue attr =
        fn_710009ddd4(reinterpret_cast<UnkVersionEntry*>(const_cast<CType*>(type)), base::reflection::attr::AfterElementDeserialization());
    if (attr.type) {
        auto* name = static_cast<UnkReaderName_f07fc*>(ValueAddress(attr));
        if (name->length &&
            StringBuffer_Differs(reinterpret_cast<char*>(const_cast<CRntString*>(name->string)), g_unk_710162e2ed)) {
            UnkReaderId_f07fc id(*name->string);
            const CClass* cls =
                type && fn_710009d50c(reinterpret_cast<const UnkReflObject*>(type)->vf_0x0(), fn_7100096100())
                    ? static_cast<const CClass*>(type)
                    : nullptr;
            const UnkReflArray& readers = cls->m_array_0x110;
            const CStrId* keys = static_cast<const CStrId*>(readers.keys);
            u64 i = 0;
            for (; i < readers.count; ++i) {
                if (keys[i].data == id.data)
                    break;
            }
            reader = &static_cast<UnkClassReader_f07fc*>(readers.data)[i];
        }
    }
    fn_71000a1c60(&attr);
    return reader;
}

bool fn_71000f07fc(UnkTypedRefWriter* self) {
    auto* writer = reinterpret_cast<UnkTypedRefWriterFull_f07fc*>(self);
    UnkWriterContext_f07fc* context = writer->stack.Top();
    UnkReader_f07fc* reader = context->reader;
    if (!reader && writer->field_0x5468)
        reader = FindClassReader(reinterpret_cast<const CType*>(context->value.type));
    if (reader && writer->field_0x5468) {
        base::UnkAllocator* allocator = GetDefaultAllocator();
        UnkReflValue* copy;
        {
            UnkReflValue ref;
            ref.field_0x0 = nullptr;
            ref.type = nullptr;
            ref.kind = context->value.kind;
            fn_71000a18d4(&ref, reinterpret_cast<const CType*>(context->value.type), ValueAddress(context->value),
                          1);
            copy = static_cast<UnkReflValue*>(allocator->vf_0x40(0xf0, 8, 0x59));
            fn_71000a179c(copy, &ref);
            fn_71000a1c60(&ref);
        }
        UnkReaderKey_f07fc key = {copy, 1};
        reader->vf_0x10(TypedAddress(context->value), key, 1);
        if (copy) {
            fn_71000a1c60(copy);
            allocator->vf_0x48(copy);
        }
    }
    return true;
}
