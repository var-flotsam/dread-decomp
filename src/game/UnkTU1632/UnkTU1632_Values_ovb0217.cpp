// Unit 1632 of tools/tu_map.py: the engine's array of type-erased values (base/UnkErasedVector.h)
// and the text form of such a list of values: a copy of the array, the array built from a text of
// pieces split at a separator (each piece an unsigned number in brackets or a string id), the
// pieces joined back into one text, and a comparison of two lists of typed values. Names and
// layouts are placeholders.
#include <new>
#include <stdlib.h>

#include "../../game_aca57c/UnkBufferString.h"
#include "base/UnkAtExit.h"
#include "base/UnkCloneHelpers.h"
#include "base/UnkErasedVector.h"
#include "base/global/CRntString.h"
#include "base/global/CRntStringAssign.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU57.h"
#include "game/UnkTUInit.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::reflection::CType;
using base::reflection::UnkTypedRef;

using UnkString100 = UnkBufferString<0x100, g_unk_7101a194b8>;

bool fn_710009d5d4(const char* type, base::reflection::UnkTypedRef* dst, const base::reflection::UnkTypedRef* src);
struct UnkFormatSource;
UnkString100 fn_7100adc570(UnkFormatSource* self);
void fn_710007e7d8(CRntString* self, const char* str, bool borrowed);
void fn_7100ad386c(UnkErasedVector* self, UnkErasedValue_0x10* value);
extern const char g_unk_710164b8a8[];
extern const char g_unk_710164b8a9[];

// The object these helpers are members of; none of them reads it.
struct UnkPathList_b0217;

// ---- comparing two lists of typed values ----

struct UnkPathElem_b0217 {
    void* field_0x0;
    const CType* type;  // 0x8
};

struct UnkPathArgs_b0217 {
    UnkPathElem_b0217* data;
    u32 count;  // 0x8
};

bool fn_7100ad2d90(UnkPathArgs_b0217* a, UnkPathArgs_b0217* b) {
    if (a->count != b->count)
        return false;
    if (a->count) {
        UnkPathElem_b0217* ea = a->data;
        UnkPathElem_b0217* eb = b->data;
        do {
            UnkTypedRef ref_a = {ea->type, ea};
            UnkTypedRef ref_b = {eb->type, eb};
            if (!fn_710009d5d4(reinterpret_cast<const char*>(ea->type), &ref_a, &ref_b))
                return false;
            ++ea;
            ++eb;
        } while (ea != a->data + a->count);
    }
    return true;
}

// ---- the string id value kept for the list ----

struct UnkReflPathArg_ovb0106;

inline const CStrId& IdAt_b0217(u32 offset) {
    static UnkGameManagerIdTable s_table asm("g_unk_7101d552d0");
    return *reinterpret_cast<const CStrId*>(s_table.pad_0x0 + offset);
}

// A string-id value, destroyed at exit by fn_710010c424, after 16 bytes of other data.
struct UnkIdValueHolder_b0217 {
    u64 field_0x0[2];
    UnkValue value;

    __attribute__((always_inline)) UnkIdValueHolder_b0217()
        : value(CStrId(IdAt_b0217(0x1e08)), refl_TypeOf<fn_7100009658>) {
        fn_7100000250(fn_710010c424, &value, g_unk_7101a19000);
    }
};

UnkReflPathArg_ovb0106* fn_7100ad2e20() {
    static UnkIdValueHolder_b0217 holder;
    return reinterpret_cast<UnkReflPathArg_ovb0106*>(&holder.value);
}

// ---- the array of type-erased values ----

// Copy constructor.
void fn_7100ad2fb0(UnkErasedVector* self, const UnkErasedVector* other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;

    u32 n = other->capacity;
    if (self->capacity < n) {
        auto* p = static_cast<UnkErasedValue_0x10*>(
            self->allocator->vf_0x40((u64)n * sizeof(UnkErasedValue_0x10), 8, self->flags));
        if (self->data) {
            UnkErasedValue_0x10* src = self->data;
            UnkErasedValue_0x10* dst = p;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                new (dst) UnkErasedValue_0x10(static_cast<UnkErasedValue_0x10&&>(*src));
                src->~UnkErasedValue_0x10();
            }
            self->allocator->vf_0x48(self->data);
        }
        self->data = p;
        self->capacity = n;
    }

    for (const UnkErasedValue_0x10* it = other->data, *end = it + other->count; it != end; ++it)
        fn_710023b424(self, it);
}

// ---- values of a text ----

// The value as it is returned: in memory (the copy is never made); destroyed through its type.
struct UnkReturnedValue_b0217 : UnkValue {
    template <class T>
    __attribute__((always_inline)) UnkReturnedValue_b0217(const T& v, UnkReflType* (*get_type)())
        : UnkValue(v, get_type) {}
    UnkReturnedValue_b0217(const UnkReturnedValue_b0217&);
    ~UnkReturnedValue_b0217() {
        if (type) {
            type->destroy(this);
            type = nullptr;
        }
    }
};

// A string id built in place by the out-of-line constructor and released at the end.
union UnkStrIdSlot_b0217 {
    CStrId id;
    UnkStrIdSlot_b0217() {}
    ~UnkStrIdSlot_b0217() { id.~CStrId(); }
};

// The value a piece of text stands for: "[number]" is an unsigned, anything else a string id.
__attribute__((noinline)) UnkReturnedValue_b0217 fn_7100ad32d0(UnkPathList_b0217* list, const CRntString* text) {
    const char* data = text->data;
    if (data[0] == '[' && data[(s32)(text->length - 1)] == ']') {
        u32 number = strtol(data + 1, nullptr, 10);
        return UnkReturnedValue_b0217(number, refl_TypeOf<fn_7100005178>);
    }
    UnkStrIdSlot_b0217 slot;
    fn_7100000850(&slot.id, *text, true);
    return UnkReturnedValue_b0217(slot.id, refl_TypeOf<fn_7100009658>);
}

// A counted part of a string, on 0x100 bytes of inline storage.
struct UnkPieceText_b0217 : UnkString100 {
    __attribute__((always_inline)) UnkPieceText_b0217(const char* str, u32 length) {
        fn_710007e7f8(Str(), str, length);
    }
};

static inline UnkReturnedValue_b0217 Parse(const UnkPieceText_b0217& piece) {
    UnkPathList_b0217* unused;
    return fn_7100ad32d0(unused, piece.Str());
}
static inline void Push(UnkErasedVector* self, UnkReturnedValue_b0217&& value) {
    fn_7100ad386c(self, reinterpret_cast<UnkErasedValue_0x10*>(&value));
}

// Parses one piece of the text into a value and appends it.
#define ADD_PIECE(self, text, start, length) \
    Push(self, Parse(UnkPieceText_b0217((text)->data + (start), (length))))

// Builds the array from a text of pieces split at the separator.
void fn_7100ad3118(UnkErasedVector* self, const CRntString* text) {
    self->step = 10;
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->field_0x14 = true;
    self->flags = 0xf;
    self->allocator = fn_7100083990();
    u32 length = text->length;
    for (u32 pos = 0; length && pos < length;) {
        s32 found = fn_710007fb54(text, g_unk_710164b8a9, pos);
        if (found < 0) {
            u32 rest = text->length - pos;
            ADD_PIECE(self, text, pos, rest);
            return;
        }
        if ((u32)found > pos)
            ADD_PIECE(self, text, pos, found - pos);
        pos = found + 1;
    }
}

// ---- the text of a list of values ----

struct UnkPathValue_b0217 {
    void* field_0x0;
    const CType* type;  // 0x8
};

struct UnkPathValues_b0217 {
    UnkPathValue_b0217* data;
    u32 count;  // 0x8
};

// A string built by the callee in the caller's result slot (never constructed here).
struct UnkStringOut_b0217 {
    union {
        CRntString str;
    };
    UnkStringOut_b0217() {}
    UnkStringOut_b0217(const UnkStringOut_b0217&);  // never called: the result is built in place
    ~UnkStringOut_b0217() { str.~CRntString(); }
    operator const CRntString&() const { return str; }
};

// A value as text; an unsigned one in brackets.
__attribute__((noinline)) UnkStringOut_b0217 fn_7100ad370c(UnkPathList_b0217* list, UnkPathValue_b0217* value) {
    UnkStringOut_b0217 out;
    CRntString& result = out.str;
    if (value->type == fn_7100005178()) {
        fn_710007e7d8(&result, g_unk_710164b8a8, true);
        {
            UnkString100 text = fn_7100adc570(reinterpret_cast<UnkFormatSource*>(value));
            fn_710007f3b4(&result, text.Str());
        }
        fn_710007f5ac(&result, ']');
    } else {
        UnkString100 text = fn_7100adc570(reinterpret_cast<UnkFormatSource*>(value));
        fn_710007eab4(&result, text.Str());
    }
    return out;
}

static inline CRntString Concat(const CRntString& a, const CRntString& b) {
    return fn_710007eee0(&a, &b);
}
static inline void Append(CRntString& self, const CRntString& other) {
    fn_710007f3b4(&self, &other);
}

// The values' texts joined with the separator between them.
UnkStringOut_b0217 fn_7100ad3614(void* source) {
    UnkPathValues_b0217* values = static_cast<UnkPathValues_b0217*>(source);
    UnkStringOut_b0217 out;
    if (!values->count) {
        fn_710007e6c4(&out.str);
        return out;
    }
    UnkPathValue_b0217* it = values->data;
    // fn_7100ad370c ignores its first argument; the original leaves that register as it was.
    UnkPathList_b0217* unused;
    UnkStringOut_b0217 joined_text = fn_7100ad370c(unused, it);
    for (++it; it != values->data + values->count; ++it) {
        Append(joined_text.str, Concat(CRntString(g_unk_710164b8a9), fn_7100ad370c(unused, it)));
    }
    new (&out.str) CRntString(static_cast<CRntString&&>(joined_text.str));
    return out;
}
