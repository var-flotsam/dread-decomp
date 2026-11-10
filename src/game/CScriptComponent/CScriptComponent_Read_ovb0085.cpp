// CScriptComponent's unit (tools/tu_map.py): the destructor (fn_7101139cdc: when both names are set, the
// script manager is told to drop the script named at +0x98; then the two names and the array at +0x40 are
// released before the component's base destructor), the read (fn_710113a004: after the base read, two strings of
// up to 0x200 bytes while the file has data left, stored as the two names at +0x68 and +0x98; when both are
// set the script manager, created on first use, loads the script) and the property handler
// (fn_7101139e08: the id table's key at +0x16a0 writes or reads the name at +0x68 as a string; other keys
// go to the base handler). Names and layouts are placeholders.
#include "../../game_s7/UnkPropS7.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntString.h"
#include "game/UnkComponent.h"
#include "types.h"

using namespace unk_prop;
using base::global::CRntFile;
using base::global::CRntString;

// As src/game/CTargetComponent/CTargetComponent_c2_2.cpp declares it.
void fn_710007ca94(CRntFile* file, char* buf);
// As src/game/CFactionComponent/CFactionComponent_Read_b48a3.cpp declares it: the base read.
void fn_710024fc08();
// As src/game/UnkUnit2669/UnkUnit2669_LoadByName.cpp declares it.
struct UnkUnit2669Str;
u32 fn_710109dd6c(void* self, void* source, UnkUnit2669Str* name, bool a, bool b);

// The script manager (0x88 bytes), created on first use.
struct UnkScriptManager_ovb0085;
extern UnkScriptManager_ovb0085* g_unk_71727bed90;
void fn_710109d810(UnkScriptManager_ovb0085* self);
void fn_710109d890(UnkScriptManager_ovb0085* self, CRntString* name);

extern char g_unk_7101cb30b8[];  // the component's vtable

struct UnkScriptEntry_ovb0085;

struct UnkScriptComponent_ovb0085 {
    void* vtable;
    u8 pad_0x8[0x38];
    base::UnkVector<UnkScriptEntry_ovb0085*> entries;  // 0x40
    CRntString name_0x68;
    CRntString name_0x98;
};
static_assert(__builtin_offsetof(UnkScriptComponent_ovb0085, name_0x98) == 0x98, "layout");

static inline UnkScriptManager_ovb0085* ScriptManager() {
    UnkScriptManager_ovb0085* manager = g_unk_71727bed90;
    if (!manager) {
        manager = static_cast<UnkScriptManager_ovb0085*>(fn_71000839a0(0x88, 0x3f));
        fn_710109d810(manager);
        g_unk_71727bed90 = manager;
    }
    return manager;
}

static inline bool IsSet(const CRntString& s) { return s.data && s.data[0]; }

void fn_7101139cdc(UnkScriptComponent_ovb0085* self) {
    self->vtable = g_unk_7101cb30b8 + 0x10;
    // The first name's text is read through raw bytes: the original computes the name's address again
    // for its destructor.
    const char* first = *reinterpret_cast<char* const*>(reinterpret_cast<u8*>(self) + 0x68);
    if (first && first[0] && IsSet(self->name_0x98))
        fn_710109d890(ScriptManager(), &self->name_0x98);
    fn_710007ecd4(&self->name_0x98);
    fn_710007ecd4(&self->name_0x68);
    self->entries.Free();
    fn_71010c4194(reinterpret_cast<UnkBaseComponent*>(self));
}

void fn_710113a004(UnkScriptComponent_ovb0085* self, CRntFile* file) {
    fn_710024fc08();
    char first[0x200] = {};
    if (file->position < file->size)
        fn_710007ca94(file, first);
    char second[0x200] = {};
    if (file->position < file->size)
        fn_710007ca94(file, second);
    fn_710007e77c(&self->name_0x68, first);
    fn_710007e77c(&self->name_0x98, second);
    if (IsSet(self->name_0x68) && IsSet(self->name_0x98))
        fn_710109dd6c(ScriptManager(), &self->name_0x68, reinterpret_cast<UnkUnit2669Str*>(&self->name_0x98),
                      false, true);
}

static inline const char* Text(const CRntString& s) { return s.data ? s.data : g_unk_710162e2ed; }

u32 fn_7101139e08(UnkScriptComponent_ovb0085* self, UnkProp* p) {
    if (!p->set && Is(p, Key(0x16a0)))
        return fn_710007cb4c(p->stream, "s", Text(self->name_0x68)) != 0;
    if (p->set && Is(p, Key(0x16a0)))
        return fn_710007cf30(p->stream, "s", Text(self->name_0x68)) != 0;
    return fn_71010c4250(reinterpret_cast<UnkBaseComponent*>(self), reinterpret_cast<UnkComponentProp*>(p));
}
