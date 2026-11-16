// CBlackboard's unit: reads the blackboard's reflected object from a file (a reader copy of it,
// from the start, parsed with this unit's callbacks); whether it parsed. Names and layouts are
// placeholders.
#include "base/UnkFileReader.h"
#include "base/UnkHashStream.h"
#include "base/animation/CBlendSpaceResource.h"
#include "base/global/CRntFile.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflObjectRead.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CBlackboard.h"
#include "game/UnkTU56.h"
#include "types.h"

using base::reflection::UnkReflObject;

static inline bool Parse(UnkFileReader* reader, UnkReadTarget* target) {
    UnkParseContext ctx(g_unk_71729fd068, reader, target);
    UnkReadEntry* end = target->entries + target->count;
    return fn_710000f750(&ctx, target->count ? end[-1].field_0x0->field_0x10 : nullptr) != 0;
}

bool fn_71010477f0(UnkReflObject* self, const base::global::CRntFile* file) {
    UnkFileReader reader;
    fn_710007beac(&reader.file, file, 0x16, nullptr);
    reader.field_0x3c = 2;
    fn_710007c00c(&reader.file, 0, 0);
    fn_710000ccd8(&reader);
    bool ok = false;
    if (reader.field_0x38 == 0) {
        UnkReadTarget target;
        {
            const base::reflection::CClass* type = self->vf_0x0();
            base::reflection::UnkReflValue value;
            value.field_0x0 = nullptr;
            value.type = nullptr;
            value.kind = 2;
            fn_71000a18d4(&value, type, self, true);
            fn_71000f0568(&target, &value, 0x15);
            fn_71000a1c60(&value);
        }
        ok = false;
        if (reader.file.position < reader.file.size && reader.field_0x38 == 0)
            ok = Parse(&reader, &target);
        for (u64 i = 0; i < target.count; ++i) {
            UnkReadEntry& e = target.entries[i];
            if (e.ops) {
                e.ops->destroy(e.storage);
                e.ops = nullptr;
            }
        }
    }
    fn_710007c0e4(&reader.file);
    return ok;
}
