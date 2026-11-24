// CBlendSpaceResource unit: reads one reflected value from an in-memory file: a CName as its hash, a
// byte buffer as its size then its bytes, a string in place, or any other type by its assign
// function straight from the file's data. Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkHashStream.h"
#include "base/animation/CBlendSpaceResource.h"
#include "base/global/CName.h"
#include "base/global/CStrId.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU56.h"
#include "types.h"

using base::global::CName;
using base::global::CRntFile;
using base::global::UnkNameHash;
using base::reflection::CType;
using base::reflection::UnkTypedRef;

// A byte buffer that may own its storage.
struct UnkByteBuffer_00fbb0 {
    u32 size;
    u8* data;  // 0x8
    bool owned;  // 0x10

    void Reset() {
        if (owned && data) {
            delete[] data;
            data = nullptr;
        }
        data = nullptr;
        size = 0;
        owned = true;
    }
};

// Assigns `src` to `dst` through dst's type.
static inline bool Assign(const CType* type, UnkTypedRef* dst, UnkTypedRef src) {
    return fn_710009d7a8(type, dst, &src);
}

// The kind and the name the callers pass (as to fn_71000ee9fc) are not read here.
bool fn_710000fbb0(CRntFile* file, u32, UnkTypedRef* value, const base::global::CStrId*) {
    if (value->type == fn_710000fd7c()) {
        CName name(UnkNameHash(), -1);
        CRntFile_Read(file, &name, sizeof(name), 1);
        Assign(value->type, value, {fn_710000fd7c(), &name});
        return true;
    }
    if (fn_710009d50c(value->type, static_cast<const CType*>(fn_710000ffd0()))) {
        if (UnkByteBuffer_00fbb0* buffer = static_cast<UnkByteBuffer_00fbb0*>(value->ptr)) {
            CRntFile_Read(file, buffer, sizeof(buffer->size), 1);
            u32 size = buffer->size;
            buffer->Reset();
            buffer->data = static_cast<u8*>(fn_7100083ba0(size, 0xf));
            buffer->size = size;
            return fn_710007c70c(file, buffer->data, size) == buffer->size;
        }
    }
    if (fn_710009d52c(value->type, fn_71000081e8())) {
        const char* text = nullptr;
        CRntFile_ReadCString(file, &text);
        Assign(value->type, value, {fn_71000081e8(), &text});
        return true;
    }
    if (file->position >= file->size)
        return false;
    const CType* type = value->type;
    void* dst = value->ptr;
    if (fn_710007c1a4(file) - file->position < type->m_size)
        return false;
    type->m_copyAssign(dst, file->data + file->position);
    CRntFile_Seek(file, type->m_size, 1);
    return true;
}
