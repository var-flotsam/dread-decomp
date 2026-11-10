// CSubAreaManager::CTunableSubAreaManager unit (tools/tu_map.py unit 2577): fn_7100eeaa48: announces a sub-area change: the script CurrentScenario.OnSubAreaChange gets the four ids and the flag, the central units are told (fn_7100ddb7bc) and the change is queued at +0xa60 (fn_7100ef1608). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

struct CSubAreaManager;

// The pooled data behind an id as its text accessor sees it.
struct UnkIdTextData {
    u8 pad_0x0[0x10];
    CRntString str;
    u8 pad_0x40[0x4];
    s32 reads;
};
static inline const char* Text(const CStrId& id) {
    typedef s32 __attribute__((may_alias)) AnyS32;
    ++*reinterpret_cast<AnyS32*>(&reinterpret_cast<UnkIdTextData*>(id.data)->reads);
    const char* s = reinterpret_cast<UnkIdTextData*>(id.data)->str.data;
    return s ? s : g_unk_710162e2ed;
}

// The four ids and the flag of the sub-area change, as the deferred call takes them.
struct UnkSubAreaChange_b190 {
    union {
        CStrId prev_a;
    };
    union {
        CStrId prev_b;
    };
    union {
        CStrId cur_a;
    };
    union {
        CStrId cur_b;
    };
    bool flag;
    UnkSubAreaChange_b190(const CStrId& a, const CStrId& b, const CStrId& c, const CStrId& d, bool f)
        : prev_a(a), prev_b(b), cur_a(c), cur_b(d), flag(f) {}
    ~UnkSubAreaChange_b190() {}
};
void fn_7100eeac18(UnkSubAreaChange_b190* self);

void fn_7100000790(const char* name, const char* fmt, ...);
struct UnkDoorRegistry_b61;
void fn_7100ddb6dc(UnkDoorRegistry_b61* self);
extern UnkDoorRegistry_b61* g_unk_71726bee10;
struct UnkUnit2547;
void fn_7100ddb7bc(UnkUnit2547* self, void* a, void* b, void* c, void* d, bool e);
void fn_7100ef1608(void* set, const UnkSubAreaChange_b190* change);

void fn_7100eeaa48(CSubAreaManager* self, const CStrId& prev_a, const CStrId& prev_b, const CStrId& cur_a,
                   const CStrId& cur_b, bool flag) {
    fn_7100000790("CurrentScenario.OnSubAreaChange", "ssssb", Text(prev_a), Text(prev_b), Text(cur_a), Text(cur_b), flag);
    UnkDoorRegistry_b61* reg = g_unk_71726bee10;
    if (!reg) {
        reg = static_cast<UnkDoorRegistry_b61*>(fn_71000839a0(4, 0x3f));
        fn_7100ddb6dc(reg);
        g_unk_71726bee10 = reg;
    }
    fn_7100ddb7bc(reinterpret_cast<UnkUnit2547*>(reg), const_cast<CStrId*>(&prev_a), const_cast<CStrId*>(&prev_b),
                  const_cast<CStrId*>(&cur_a), const_cast<CStrId*>(&cur_b), flag);
    UnkSubAreaChange_b190 change(prev_a, prev_b, cur_a, cur_b, flag);
    fn_7100ef1608(reinterpret_cast<u8*>(self) + 0xa60, &change);
    fn_7100eeac18(&change);
}
