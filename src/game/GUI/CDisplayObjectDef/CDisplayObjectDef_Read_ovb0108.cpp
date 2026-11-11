// GUI::CDisplayObjectDef's unit (tools/tu_map.py): this unit's copy of the blackboard reader's
// object read (fn_710104b540, src/game/CBlackboard/CBlackboard_Sections_ovb0026.cpp): reads the
// object's name id, and, for a pointer, the id of the type it points to (the registered type
// replaces the object's own when there is one), then reads the object as that type
// (fn_7100b09820). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU83.h"
#include "types.h"

struct UnkReadArg_b0026;
struct UnkVersionRegistry;
struct UnkVersionEntry;
extern UnkVersionRegistry* g_unk_7101d4c8d0;
void fn_71000ee9fc(void* reader, u32 flags, UnkReadArg_b0026* arg, const base::global::CStrId* name);
const base::reflection::CType* fn_710009c640(base::reflection::UnkReflObject* obj);

// A typed pointer to a value the reader fills (a CName id here); UnkReadArg_b0026's layout.
struct UnkDisplayReadArg_ovb0108 {
    const base::reflection::CType* type;
    u64* value;
};

struct UnkDisplayReader_ovb0108 {
    u8 pad_0x0[0x30];
    void* reader;
};
s32 fn_7100b09820(UnkDisplayReader_ovb0108* self, base::reflection::UnkReflObject* obj,
                  const base::reflection::CType* type);

static inline void ReadId(void* reader, u32 flags, UnkDisplayReadArg_ovb0108* arg, u64* id) {
    arg->type = fn_710000fd7c();
    arg->value = id;
    fn_71000ee9fc(reader, flags, reinterpret_cast<UnkReadArg_b0026*>(arg), &g_unk_7101d470e0);
}

s32 fn_7100b09720(UnkDisplayReader_ovb0108* self, base::reflection::UnkReflObject* obj) {
    UnkDisplayReadArg_ovb0108 arg;
    u64 name = (u64)-1;
    ReadId(self->reader, 0, &arg, &name);
    const base::reflection::CType* type = fn_710009c640(obj);
    if (fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(obj->vf_0x0()),
                      reinterpret_cast<const base::reflection::CType*>(fn_710009bedc()))) {
        u64 id = (u64)-1;
        ReadId(self->reader, 1, &arg, &id);
        if (id != (u64)-1)
            type = reinterpret_cast<const base::reflection::CType*>(fn_71000a5118(g_unk_7101d4c8d0, &id));
    }
    return fn_7100b09820(self, obj, type);
}
