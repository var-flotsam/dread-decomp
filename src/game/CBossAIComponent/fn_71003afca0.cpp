// Takes the string id at +0x518 of the object at +0x14908, makes a handle of it
// (fn_7100ae986c) and, unless the id is empty, passes the handle to three services of the
// global registry (fn_71011437bc, fn_7100301860, fn_7100b30454).
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"

using base::global::CStrId;

struct UnkStrIdHandle {
    CStrId id;
    void* field_0x8;
    UnkStrIdHandle(const CStrId& id) asm("fn_7100ae986c");  // fn_7100ae986c
};

void* fn_7100df55e4(UnkGameManager* manager);
void* fn_7100df5440(UnkGameManager* manager);
void fn_71011437bc(void* service, const UnkStrIdHandle& handle);
void fn_7100301860(void* service, const UnkStrIdHandle& handle);
void fn_7100b30454(UnkIngameMenu* menu, const UnkStrIdHandle& handle);

struct UnkSubject {
    u8 pad_0x0[0x518];
    CStrId field_0x518;
};

struct UnkSelf {
    u8 pad_0x0[0x14908];
    UnkSubject* field_0x14908;
};

void fn_71003afca0(UnkSelf* self) {
    UnkStrIdHandle handle(self->field_0x14908->field_0x518);
    if (handle.id.data != &g_unk_7101d47108->empty) {
        fn_71011437bc(fn_7100df55e4(g_unk_7101d47310), handle);
        fn_7100301860(fn_7100df5440(g_unk_7101d47310), handle);
        fn_7100b30454(fn_7100deac84(g_unk_7101d47310), handle);
    }
}
