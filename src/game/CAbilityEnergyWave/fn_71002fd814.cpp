// Gives the holder a weak reference when it has none or the one it has lost its target: the holder's
// handle is made to refer to the weak handle of the object fn_7100303054 returns (created on first
// use), or to nothing when there is no such object.
#include "game_s2/UnkWeakHandle.h"

struct UnkHolder {
    UnkWeakHandle* field_0x0;
};

UnkWeakTarget* fn_7100303054();

void fn_71002fd814(void* unused, UnkHolder* holder) {
    if (!holder->field_0x0 || !holder->field_0x0->target) {
        UnkWeakTarget* target = fn_7100303054();
        UnkWeakHandle* handle = target ? GetWeakHandle(target) : nullptr;
        AssignRef(&holder->field_0x0, handle);
    }
}
