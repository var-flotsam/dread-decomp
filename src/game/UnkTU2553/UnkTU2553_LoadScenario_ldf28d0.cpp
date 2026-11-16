// Unit 2553 (near CGameManager::CTunableGameManager): requests a scenario load. Fills a load record
// with the scenario's strings, ids and values, copies it into the manager's pending record, marks it
// pending and tells the GUI scripts (guicallbacks.OnLoadScenarioRequest). Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CDropComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2553.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

struct UnkRecord_df25a0;
struct UnkReflRecord_ovb0106;
struct UnkTunable;

UnkRecord_df25a0* fn_7100df25a0(UnkRecord_df25a0* self, const UnkRecord_df25a0* other);
void fn_7100ad8550(UnkReflRecord_ovb0106* self);
void fn_7100ad8864(UnkReflRecord_ovb0106* self);

// The load record's members this function sets.
struct UnkLoadRecordFields_df28d0 {
    u8 pad_0x0[0x18];
    CRntString field_0x18;
    CRntString field_0x48;
    CRntString field_0x78;
    CStrId field_0xa8;
    u8 pad_0xb0[0x24];
    u32 field_0xd4;
    u8 field_0xd8;
    u8 field_0xd9;
    u8 pad_0xda[6];
    CRntString field_0xe0;
    u32 field_0x110;
    u32 pad_0x114;
    void* field_0x118;
    f32 field_0x120;
};
static_assert(__builtin_offsetof(UnkLoadRecordFields_df28d0, field_0xe0) == 0xe0, "layout");
static_assert(__builtin_offsetof(UnkLoadRecordFields_df28d0, field_0x120) == 0x120, "layout");

// A load record on the stack, built and destroyed by its out-of-line constructor and destructor.
struct UnkLoadRecord_df28d0 {
    alignas(8) u8 raw[0x128];

    UnkLoadRecord_df28d0() { fn_7100ad8550(reinterpret_cast<UnkReflRecord_ovb0106*>(this)); }
    ~UnkLoadRecord_df28d0() { fn_7100ad8864(reinterpret_cast<UnkReflRecord_ovb0106*>(this)); }
    UnkLoadRecordFields_df28d0& Fields() { return *reinterpret_cast<UnkLoadRecordFields_df28d0*>(raw); }
};

// The manager's pending load record (+0x4a0) and the flag set once a load was requested.
struct UnkGameManagerLoad_df28d0 {
    u8 pad_0x0[0x4a0];
    bool pending;  // 0x4a0
    u8 pad_0x4a1[0xf];
    bool field_0x4b0;
    u8 pad_0x4b1[7];
    CRntString scenario;  // 0x4b8
    CRntString field_0x4e8;
    u8 pad_0x518[0x60];
    u8 field_0x578;
    u8 field_0x579;
    u8 pad_0x57a[0x4d6];
    bool load_requested;  // 0xa50
};
static_assert(__builtin_offsetof(UnkGameManagerLoad_df28d0, field_0x578) == 0x578, "layout");
static_assert(__builtin_offsetof(UnkGameManagerLoad_df28d0, load_requested) == 0xa50, "layout");

struct UnkDropTunable_df28d0 {
    u8 pad_0x0[0x2a];
    bool field_0x2a;
};

static inline const char* Text(const CRntString& s) { return s.data ? s.data : g_unk_710162e2ed; }

void fn_7100df28d0(UnkGameManager* manager, const CRntString& a, const CRntString& b, const CRntString& c,
                   const CStrId& id, u32 flag, const CRntString& d, const CStrId& e, u32 f, void* g, f32 h) {
    UnkGameManagerLoad_df28d0* self = reinterpret_cast<UnkGameManagerLoad_df28d0*>(manager);
    UnkLoadRecord_df28d0 record;
    UnkLoadRecordFields_df28d0& r = record.Fields();
    fn_710007eaa0(&r.field_0x18, &a);
    fn_710007eaa0(&r.field_0x48, &b);
    fn_710007eaa0(&r.field_0x78, &c);
    r.field_0xa8 = e;
    r.field_0xd4 = flag;
    fn_710007eaa0(&r.field_0xe0, &d);
    r.field_0x110 = f;
    r.field_0x118 = g;
    r.field_0x120 = h;
    r.field_0xd8 = self->field_0x578;
    r.field_0xd9 = self->field_0x579;
    fn_7100df25a0(reinterpret_cast<UnkRecord_df25a0*>(&self->pending),
                  reinterpret_cast<const UnkRecord_df25a0*>(&record));
    self->pending = true;
    self->field_0x4b0 = false;
    fn_7100000790("guicallbacks.OnLoadScenarioRequest", "sssb", Text(self->scenario), Text(self->field_0x4e8),
                  "Samus", self->field_0x578);
    self->load_requested = true;
    reinterpret_cast<UnkDropTunable_df28d0*>(&g_unk_717271b668)->field_0x2a = false;
}
