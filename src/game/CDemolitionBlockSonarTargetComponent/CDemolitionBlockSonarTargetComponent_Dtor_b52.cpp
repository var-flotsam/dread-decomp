// CDemolitionBlockSonarTargetComponent unit (tools/tu_map.py unit 1520): a deleting destructor. The
// vtable, the array of 0x20-byte polymorphic entries at +0x48 (each destroyed through its vtable slot
// at +0x30, then the array freed), the counted reference at +0x40 dropped, then the base destructor
// fn_7100a1082c and operator delete. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/CDemolitionBlockSonarTargetComponent.h"
#include "game/CSonarTargetComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkSonarTargetEntry_b52 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual ~UnkSonarTargetEntry_b52();  // 0x30
    u8 pad_0x8[0x18];
};

struct UnkSonarTarget_b52 {
    void** vtable;
    u8 pad_0x8[0x38];
    UnkRefObject* ref;                                   // 0x40
    base::UnkVector<UnkSonarTargetEntry_b52> entries;  // 0x48
};

// By reference: the object is known to exist, so the delete has no null check.
void fn_7100a0f888(UnkSonarTarget_b52& self) {
    self.vtable = reinterpret_cast<void**>(g_unk_7101a8b8f0 + 0x10);
    self.entries.Free();
    if (UnkRefObject* p = self.ref) {
        if (--p->ref_count == 0)
            p->vf_0x10();
    }
    fn_7100a1082c(reinterpret_cast<char*>(&self));
    operator delete(&self);
}
