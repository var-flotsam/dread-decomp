// CActorDef's unit (tools/tu_map.py unit 345): reads the definition's component list from a file: a count,
// then for each component its name and type. A fresh definition creates each component (fn_7100253988) and
// appends it; one already loaded looks it up by name. Each reads its own data (fn_7100253af0); the list is
// then sorted (fn_7100256810). Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkComponentDef_b0129.h"
#include "game/UnkTU56.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;

struct UnkKeyId_b0129;
struct UnkComponentDefRead_b67a1;
UnkComponentDef_b0129* fn_7100253988(const base::UnkStrIdMember& name, const UnkKeyId_b0129& type);
bool fn_7100253af0(UnkComponentDefRead_b67a1* self, base::global::CRntFile* file);

// The order the component list is sorted in (the comparator of the sort instantiated in fn_7100256810).
struct UnkComponentOrder_l2563dc {};
void fn_7100256810(UnkComponentDef_b0129** first, UnkComponentDef_b0129** last, UnkComponentOrder_l2563dc& order);

struct UnkActorDef_l2563dc {
    u8 pad_0x0[0x50];
    bool loaded;  // 0x50
    u8 pad_0x51[0x4f];
    CRntVector<UnkComponentDef_b0129*> components;  // 0xa0
};
static_assert(__builtin_offsetof(UnkActorDef_l2563dc, components) == 0xa0, "layout");

bool CActorDef_ReadComponents(UnkActorDef_l2563dc* self, base::global::CRntFile* file) {
    s32 count;
    CRntFile_Read(file, &count, 4, 1);
    if (!self->loaded)
        self->components.Reserve(count);
    bool ok = true;
    for (s32 i = 0; i < count && ok; ++i) {
        const char* name;
        const char* type;
        CRntFile_ReadCString(file, &name);
        CRntFile_ReadCString(file, &type);
        UnkComponentDef_b0129* def;
        if (self->loaded) {
            CStrId key(name, true);
            def = nullptr;
            for (UnkComponentDef_b0129* component : self->components) {
                if (component->name.data == key.data) {
                    def = component;
                    break;
                }
            }
        } else {
            CStrId nameId(name, true);
            CStrId typeId(type, true);
            def = fn_7100253988(reinterpret_cast<const base::UnkStrIdMember&>(nameId),
                                reinterpret_cast<const UnkKeyId_b0129&>(typeId));
        }
        if (def)
            ok = fn_7100253af0(reinterpret_cast<UnkComponentDefRead_b67a1*>(def), file);
        else
            ok = false;
        if (!self->loaded)
            self->components.PushBack(def);
    }
    UnkComponentOrder_l2563dc order;
    fn_7100256810(self->components.begin(), self->components.end(), order);
    return ok;
}
