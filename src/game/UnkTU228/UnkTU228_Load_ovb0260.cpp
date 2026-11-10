// Unit 228: loads the decal resources a manager needs (the error decal first, kept in a global) the first
// time they are asked for. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "types.h"

using base::global::CFilePathStrId;

struct UnkDecalResource;
struct UnkDecalResourceMgr;
UnkDecalResource* fn_710019ffe8(UnkDecalResourceMgr* mgr, const CFilePathStrId* path, bool flag);

extern UnkDecalResource* g_unk_716e352738;
extern CFilePathStrId g_unk_716e352760 __attribute__((visibility("hidden")));
extern CFilePathStrId g_unk_716e352768 __attribute__((visibility("hidden")));
extern CFilePathStrId g_unk_716e352770 __attribute__((visibility("hidden")));
extern CFilePathStrId g_unk_716e352778 __attribute__((visibility("hidden")));
extern CFilePathStrId g_unk_716e352780 __attribute__((visibility("hidden")));

struct UnkDecalMgr_b260 {
    u8 unk_0x0[8];
    bool loaded;                 // 0x8
    u8 unk_0x9[7];
    UnkDecalResource* field_0x10;
    UnkDecalResource* field_0x18;
    UnkDecalResource* field_0x20;
    UnkDecalResource* field_0x28;
    UnkDecalResource* field_0x30;
};

void fn_710019fe7c(UnkDecalMgr_b260* self) {
    UnkDecalResourceMgr* mgr = reinterpret_cast<UnkDecalResourceMgr*>(self);
    if (!g_unk_716e352738) {
        CFilePathStrId path("error", true);
        g_unk_716e352738 = fn_710019ffe8(mgr, &path, false);
    }
    if (!self->field_0x10)
        self->field_0x10 = fn_710019ffe8(mgr, &g_unk_716e352760, false);
    if (!self->field_0x30)
        self->field_0x30 = fn_710019ffe8(mgr, &g_unk_716e352768, false);
    if (!self->field_0x18)
        self->field_0x18 = fn_710019ffe8(mgr, &g_unk_716e352770, false);
    if (!self->field_0x20)
        self->field_0x20 = fn_710019ffe8(mgr, &g_unk_716e352778, false);
    if (!self->field_0x28)
        self->field_0x28 = fn_710019ffe8(mgr, &g_unk_716e352780, false);
    self->loaded = true;
}
