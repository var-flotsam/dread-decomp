// CColliderTriggerComponent's unit (tools/tu_map.py unit 1564): reads the component, then the name of the
// actor whose logic shape it uses, and points the shape reference (+0x428) at that actor when the game
// manager knows it. Names and layouts are placeholders.
#include "base/global/CRntFile.h"
#include "base/global/CStrId.h"
#include "game/CTriggerComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU56.h"
#include "types.h"

using base::global::CStrId;

struct UnkColliderTrigger_b0058;
struct UnkWeakVariant_b62a1;
struct UnkWeakTarget;
void fn_710023df08(UnkWeakVariant_b62a1* self, UnkWeakTarget* target);

// A string id's pooled data: the id's hash at +0x30.
struct UnkTriggerKey_la4c6a0 {
    u8 pad_0x0[0x30];
    u64 hash;  // 0x30
};

struct UnkColliderTrigger_la4c6a0 {
    u8 pad_0x0[0x428];
    u8 shape_ref[0x40];  // 0x428, the actor's weak reference (fn_710023df08)
};

void CColliderTriggerComponent_ReadShapeLink(UnkColliderTrigger_la4c6a0* self, base::global::CRntFile* file) {
    u64 key;
    alignas(8) u32 kind;
    const char* name;
    CTriggerComponent_LoadFromFile(reinterpret_cast<UnkColliderTrigger_b0058*>(self), file);
    CRntFile_ReadCString(file, &name);
    void* actor;
    {
        UnkGameManager* manager = g_unk_7101d47310;
        CStrId id(name, true);
        u32 kind_value = g_unk_71727bebb8;
        u64 hash = reinterpret_cast<UnkTriggerKey_la4c6a0*>(id.data)->hash;
        key = hash;
        kind = kind_value;
        actor = fn_7100deba8c(manager, &key, &kind);
    }
    if (actor)
        fn_710023df08(reinterpret_cast<UnkWeakVariant_b62a1*>(self->shape_ref), static_cast<UnkWeakTarget*>(actor));
}
