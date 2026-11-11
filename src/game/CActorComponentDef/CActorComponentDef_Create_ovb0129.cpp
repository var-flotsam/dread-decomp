// CActorComponentDef's unit (tools/tu_map.py unit 343): fn_7100253988 creates the definition object registered
// for a component type (fn_7100253834 finds its class, which must derive from CActorComponentDef's type) and
// gives it its name and type ids; fn_710025435c reads a definition's reflected object from a length-prefixed
// block of a file (a reader copy parses it with this unit's callbacks, g_unk_71726a0058) and moves the file
// past the block. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkHashStream.h"
#include "base/animation/CBlendSpaceResource.h"
#include "base/global/CRntFile.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflObjectRead.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkFileReader {
    base::global::CRntFile file;
    u32 field_0x38;
    u32 field_0x3c;
};

void fn_710007beac(base::global::CRntFile* self, const base::global::CRntFile* other, u32 flags,
                   base::UnkAllocator* allocator);
void fn_710000f750(UnkParseContext* self, void* root);

// This unit's parser callbacks.
extern UnkParseCallbacks g_unk_71726a0058 __attribute__((visibility("hidden")));

static inline s32 Parse(UnkFileReader* reader, UnkReadTarget* target) {
    UnkParseContext ctx(g_unk_71726a0058, reader, target);
    UnkReadEntry* end = target->entries + target->count;
    return reinterpret_cast<s32 (*)(UnkParseContext*, void*)>(&fn_710000f750)(
        &ctx, target->count ? end[-1].field_0x0->field_0x10 : nullptr);
}

// Reads the object from a length-prefixed block of `file` (a 32-bit length, then the block, which a
// reader copy parses from the current position); the file is moved past the block. True when the
// block is empty.
bool fn_710025435c(UnkReflObject* self, base::global::CRntFile* file) {
    s32 length = 0;
    UnkFileReader reader;
    fn_710007c714(file, &length, 4, 1);
    if (length < 1)
        return true;
    u64 position = file->position;
    fn_710007beac(&reader.file, file, 0x16, nullptr);
    reader.field_0x3c = 2;
    fn_710007c00c(&reader.file, position, 0);
    fn_710000ccd8(&reader);
    UnkReadTarget target;
    {
        const base::reflection::CClass* type = self->vf_0x0();
        base::reflection::UnkReflValue value;
        value.field_0x0 = nullptr;
        value.type = nullptr;
        value.kind = 2;
        fn_71000a18d4(&value, type, self, true);
        fn_71000f0568(&target, &value, 5);
        fn_71000a1c60(&value);
    }
    bool ok = false;
    if (reader.file.position < reader.file.size && reader.field_0x38 == 0)
        ok = Parse(&reader, &target) != 0;
    fn_710007c00c(file, length, 1);
    for (u64 i = 0; i < target.count; ++i) {
        UnkReadEntry& e = target.entries[i];
        if (e.ops) {
            e.ops->destroy(e.storage);
            e.ops = nullptr;
        }
    }
    fn_710007c0e4(&reader.file);
    return ok;
}

struct UnkHashData_b0129 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkKeyId_b0129 {
    UnkHashData_b0129* data;
};

// The definition class registered for a class (by its type id), not decompiled here.
struct UnkRegClass_b0129;
UnkRegClass_b0129* fn_7100253834(const UnkKeyId_b0129* id);

// The registered definition class's type (fn_7100254534 inlined: the class getter, then its type).
extern char g_unk_71726a00e8[] __attribute__((visibility("hidden")));
static inline const base::reflection::CType* ComponentDefType() {
    fn_71002525c0();
    return reinterpret_cast<const base::reflection::CType*>(g_unk_71726a00e8);
}

// A registered class as a factory: the instance size at +0x18 and its constructor at +0x28.
struct UnkDefFactory_b0129 {
    u8 pad_0x0[0x18];
    u64 size;
    u8 pad_0x20[0x8];
    void (*construct)(void* object);
};

struct UnkComponentDef_b0129 {
    void* vtable;
    base::UnkStrIdMember name;
    base::UnkStrIdMember type;
};

// Creates the definition registered for a component class (fn_7100253834), named `name`.
UnkComponentDef_b0129* fn_7100253988(const base::UnkStrIdMember& name, const UnkKeyId_b0129& type) {
    auto* cls = reinterpret_cast<UnkDefFactory_b0129*>(fn_7100253834(&type));
    if (!cls || !fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(cls), ComponentDefType()) ||
        !cls->construct)
        return nullptr;
    auto* def = static_cast<UnkComponentDef_b0129*>(fn_7100083990()->vf_0x40(cls->size, 8, 5));
    if (!def)
        return nullptr;
    cls->construct(def);
    def->name = name;
    def->type = reinterpret_cast<const base::UnkStrIdMember&>(type);
    return def;
}
