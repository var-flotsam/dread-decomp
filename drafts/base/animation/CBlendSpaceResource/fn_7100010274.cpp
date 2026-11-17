// CBlendSpaceResource unit: reads a value of a reflected class through the class's own binary
// deserializer: the function the class's "sBinaryDeserialize" attribute names is called with the
// file's unread bytes and their size, and the file moves on by the byte count it returns.
// Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkHashStream.h"
#include "base/animation/CBlendSpaceResource.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection.h"
#include "base/reflection/CClass.h"
#include "base/reflection/CType.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::global::CRntFile;
using base::global::CRntVector;
using base::global::CStrId;
using base::reflection::CClass;
using base::reflection::CType;
using base::reflection::UnkReflObject;
using base::reflection::UnkReflValue;
using base::reflection::UnkTypedRef;

const CType* fn_7100010d7c();  // base::reflection::refl_GetType<void*>

// An argument of a reflected call: a value referring to the caller's variable.
struct UnkReflArg_l010274 {
    UnkReflValue value;

    UnkReflArg_l010274(const CType* type, void* ptr) {
        value.kind = 2;
        value.field_0x0 = nullptr;
        value.type = nullptr;
        fn_71000a18d4(&value, type, ptr, true);
    }
    UnkReflArg_l010274(UnkReflArg_l010274&& other) { fn_71000a179c(&value, &other.value); }
    ~UnkReflArg_l010274() { fn_71000a1c60(&value); }
};

struct UnkReflArgs_l010274 {
    UnkReflArg_l010274* data;
    u64 count;
};

// A reflected function: vf_0x10 calls it on `target` with the arguments.
struct UnkReflFunction_l010274 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual UnkReflValue vf_0x10(UnkTypedRef* target, const UnkReflArgs_l010274* args, u32 flags);
};

// Whether the value holds one of the type `Get` returns.
template <const CType* (*Get)()>
static inline bool Holds(const UnkReflValue& v) {
    return v.type && fn_710009d50c(reinterpret_cast<const CType*>(v.type), Get());
}

bool fn_7100010274(CRntFile* file, UnkTypedRef* value) {
    if (file->position >= file->size)
        return false;
    const CClass* cls = nullptr;
    if (const CType* type = value->type) {
        if (fn_710009d50c(reinterpret_cast<const UnkReflObject*>(type)->vf_0x0(), fn_7100096100()))
            cls = static_cast<const CClass*>(type);
    }
    UnkReflValue attr = fn_710009ddd4(reinterpret_cast<UnkVersionEntry*>(const_cast<CType*>(value->type)),
                                      fn_7100011040());
    bool ok = false;
    if (cls && attr.type) {
        CStrId name(base::global::UnkStrIdInline{}, nullptr, false);
        if (Holds<fn_7100009658>(attr)) {
            name = *base::reflection::refl_ArgValue<CStrId>(&attr);
            auto* func = static_cast<UnkReflFunction_l010274*>(
                fn_71000978e4(reinterpret_cast<char*>(const_cast<CClass*>(cls)), &name));
            if (func) {
                u64 size = fn_710007c1a4(file) - file->position;
                u8* data = file->data + file->position;
                CRntVector<UnkReflArg_l010274> args(0xf);
                args.EmplaceBack(UnkReflArg_l010274(fn_7100010d7c(), &data));
                args.EmplaceBack(UnkReflArg_l010274(fn_7100010e58(), &size));
                UnkReflArgs_l010274 span = {args.data, args.count};
                {
                    UnkReflValue result = func->vf_0x10(value, &span, 0);
                    if (result.type) {
                        u64 read;  // unset in the original when the result is not an 8-byte value
                        if (fn_710009d50c(reinterpret_cast<const CType*>(result.type), fn_7100010e58()))
                            read = *base::reflection::refl_ArgValue<u64>(&result);
                        if (read != 0) {
                            fn_710007c00c(file, read, 1);
                            ok = true;
                        }
                    }
                    fn_71000a1c60(&result);
                }
                args.Free();
            }
        }
    }
    fn_71000a1c60(&attr);
    return ok;
}
