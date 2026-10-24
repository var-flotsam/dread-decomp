// Sets a camera-control flag held in an object's tail state. Clearing it runs the pending
// step; if the flag changed, the object is notified (vf_0x830) and a boss camera control found
// through the id table is told the new state.
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkCameraTarget {
    u8 pad_0x0[0x513];
    u8 field_0x513;
};

struct UnkCameraState {
    u8 field_0x0;
    u8 pad_0x1[7];
    u8 field_0x8;
    u8 pad_0x9[0x27];
    UnkCameraTarget* field_0x30;
};

struct UnkCameraOwner {
    void** vtable;
    u8 pad_0x8[0x28];
    void* field_0x30;

    void vf_0x830() { reinterpret_cast<void (*)(UnkCameraOwner*)>(vtable[0x830 / 8])(this); }
    UnkCameraState* state() { return reinterpret_cast<UnkCameraState*>(reinterpret_cast<char*>(this) + 0x148d8); }
};

struct UnkCameraCtrl {
    void** vtable;
    const void* vf_0x0() { return reinterpret_cast<const void* (*)(UnkCameraCtrl*)>(vtable[0])(this); }
};

// A large object built once, with an id at +0x998.
struct UnkIdTable {
    UnkIdTable();  // fn_7100031894
    ~UnkIdTable();
    u8 pad_0x0[0x998];
    u8 field_0x998[8];
};

inline UnkIdTable& GetIdTable() {
    static UnkIdTable s_table;
    return s_table;
}

void* fn_7100defcec(UnkGameManager* manager);
UnkCameraCtrl* fn_71010e389c(void* holder, void* key);
namespace base { namespace reflection { class CClass; } }
const base::reflection::CClass* fn_710074eca8();  // CBossCameraCtrl::refl_GetClass
void fn_71003ad174(UnkCameraOwner* self, u32 arg);
void fn_7100750804(UnkCameraCtrl* ctrl, void* arg);
void fn_7100750948(UnkCameraCtrl* ctrl, void* arg);

void fn_71003ad014(UnkCameraOwner* self, bool flag) {
    UnkCameraState* st = self->state();
    u8 want = flag;
    if (!flag && st->field_0x8) {
        UnkCameraTarget* target = st->field_0x30;
        st->field_0x8 = 0;
        if (target->field_0x513)
            fn_71003ad174(self, 0);
    }
    if (st->field_0x0 != want) {
        st->field_0x0 = want;
        self->vf_0x830();
        void* holder = fn_7100defcec(g_unk_7101d47310);
        if (holder) {
            UnkCameraCtrl* ctrl = fn_71010e389c(holder, GetIdTable().field_0x998);
            if (ctrl && ctrl->vf_0x0() == fn_710074eca8()) {
                if (st->field_0x0)
                    fn_7100750804(ctrl, self->field_0x30);
                else
                    fn_7100750948(ctrl, self->field_0x30);
            }
        }
    }
}
