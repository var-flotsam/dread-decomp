// CShockWaveComponent's destructors (complete and deleting), the same shape as fn_71010cf1e8 and
// fn_71010cf260 (src/game/CFakePhysicsMovement/fn_71010cf260.cpp): restore the vtables, free the
// node list at 0x250, destroy the member at 0x40 (fn_71003cc474) and run the component's base
// destructor (fn_71010c4194). Names and the layout are placeholders.
#include "base/global/CRntVector.h"
#include "game/CAutsharpAIComponent.h"
#include "game/CShockWaveComponent.h"
#include "game/UnkComponent.h"
#include "game/UnkTU604.h"
#include "types.h"
#include "base/UnkVtableList.h"

struct UnkShockWaveMember;

struct UnkShockWaveMember {
    u8 unk_0x0[0x210];
    ~UnkShockWaveMember() { fn_71003cc474(this); }
};

struct UnkShockWaveNode {
    u64 field_0x0;
    UnkShockWaveNode* next;
};

struct UnkShockWaveList : base::UnkVtableList<UnkShockWaveNode> {
    ~UnkShockWaveList() {
        vtable = &g_unk_7101a32680[2];
        Clear();
    }
};

struct UnkShockWaveBase : UnkBaseComponent {
    ~UnkShockWaveBase() { fn_71010c4194(this); }
};

struct CShockWaveComponent : UnkShockWaveBase {
    UnkShockWaveMember field_0x40;
    UnkShockWaveList field_0x250;

    ~CShockWaveComponent() { vtable = &g_unk_7101a8a820[2]; }
};

void fn_71009f3a60(CShockWaveComponent& self) {
    self.~CShockWaveComponent();
}

void fn_71009f3ae0(CShockWaveComponent& self) {
    delete &self;
}
