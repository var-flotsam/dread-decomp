// Unit 70: the digest of a reflected element's attributes: for each attribute the registry marks as
// digested, its key's hash and its value's hash go into a hash stream.
// Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkHashStream.h"
#include "base/animation/CBlendSpaceResource.h"
#include "base/global/CStrId.h"
#include "base/reflection.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CBlackboard.h"
#include "game/UnkTU70.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflValue;
using base::reflection::UnkReflValueRef;

struct UnkVersionRegistry;
struct UnkClone_fn_71000a53c0;
extern template void* base::refl_KeyedFind<UnkClone_fn_71000a53c0, 0xc0, 0x18>(char*, const base::UnkStrIdMember*);

struct UnkReflValueArg;
// The hash of a value given as a type and a typed reference.
u32 fn_710009db88(const base::reflection::CType* type, const UnkReflValueRef* value);

// A known attribute as the registry holds it; the flag at +0x15 puts it in the digest.
struct UnkReflAttrDigestInfo {
    u8 pad_0x0[0x15];
    bool digested;
};

// One attribute as an iterator over the attribute arrays yields it: the key and value pointed to
// (kind 2), or a key and value of its own (kind 1), or nothing (kind 0).
struct UnkReflAttrEntry {
    struct Owned {
        CStrId key;
        UnkReflValue value;
    };
    struct Ref {
        CStrId* key;
        UnkReflValue* value;
    };
    alignas(8) u8 storage[0x20];
    u32 kind;

    UnkReflAttrEntry(CStrId* key, UnkReflValue* value) : kind(2) {
        Refs().key = key;
        Refs().value = value;
    }
    ~UnkReflAttrEntry() {
        switch (kind) {
        case 1:
            fn_71000a1c60(&Own().value);
            Own().key.~CStrId();
            break;
        case 2:
            break;
        default:
            return;
        }
        kind = 0;
    }

    Owned& Own() { return *reinterpret_cast<Owned*>(storage); }
    Ref& Refs() { return *reinterpret_cast<Ref*>(storage); }
    CStrId* Key() { return kind == 2 ? Refs().key : &Own().key; }
    UnkReflValue* Value() { return kind == 2 ? Refs().value : &Own().value; }
    void operator++() {
        ++Refs().key;
        ++Refs().value;
    }
    bool operator!=(UnkReflAttrEntry& other) { return Refs().key != other.Refs().key; }
};

u32 fn_7100093bb8(void* ref) {
    auto* attrs = static_cast<UnkPropertyArrays*>(ref);
    UnkScopedHashStream stream(0x16, nullptr);
    u32 count = attrs->count;
    auto* values = static_cast<UnkReflValue*>(attrs->values);
    UnkReflAttrEntry end(&attrs->ids[count], &values[count]);
    for (UnkReflAttrEntry it(attrs->ids, values); it != end; ++it) {
        auto* info = static_cast<UnkReflAttrDigestInfo*>(base::refl_KeyedFind<UnkClone_fn_71000a53c0, 0xc0, 0x18>(
            reinterpret_cast<char*>(g_unk_7101d4c8d0), reinterpret_cast<const base::UnkStrIdMember*>(it.Key())));
        if (!info || !info->digested)
            continue;
        stream.Put(it.Key()->data->Key());
        UnkReflValueRef arg;
        fn_71000a1710(reinterpret_cast<UnkReflValueArg*>(&arg), it.Value());
        stream.Put(fn_710009db88(arg.type, &arg));
    }
    return stream.Finish();
}
