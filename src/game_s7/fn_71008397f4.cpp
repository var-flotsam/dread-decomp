// Copies of fn_71002fd814 (src/game_s2/fn_71002fd814.cpp) for other types, with their own callees: fn_71008397f4, fn_7100723cf0, fn_7100723e20, fn_710072b9e0, fn_71007ba354, fn_710115a9e8.
// Gives the holder a weak reference when it has none or the one it has lost its target: the holder's
// handle is made to refer to the weak handle of the object fn_7100303054 returns (created on first
// use), or to nothing when there is no such object.
#include "game_s2/UnkWeakHandle.h"

struct UnkHolder {
    UnkWeakHandle* field_0x0;
};

UnkWeakTarget* fn_7100303054();

UnkWeakTarget* fn_7100302014();
UnkWeakTarget* fn_7100726598();
UnkWeakTarget* fn_7100286ad4();
UnkWeakTarget* fn_710072bca8();
UnkWeakTarget* fn_71007bb354();
UnkWeakTarget* fn_71006f4a30();
void fn_71008397f4(void* unused, UnkHolder* holder) {
    if (!holder->field_0x0 || !holder->field_0x0->target) {
        UnkWeakTarget* target = fn_7100302014();
        UnkWeakHandle* handle = target ? GetWeakHandle(target) : nullptr;
        AssignRef(&holder->field_0x0, handle);
    }
}

void fn_7100723cf0(void* unused, UnkHolder* holder) {
    if (!holder->field_0x0 || !holder->field_0x0->target) {
        UnkWeakTarget* target = fn_7100726598();
        UnkWeakHandle* handle = target ? GetWeakHandle(target) : nullptr;
        AssignRef(&holder->field_0x0, handle);
    }
}

void fn_7100723e20(void* unused, UnkHolder* holder) {
    if (!holder->field_0x0 || !holder->field_0x0->target) {
        UnkWeakTarget* target = fn_7100286ad4();
        UnkWeakHandle* handle = target ? GetWeakHandle(target) : nullptr;
        AssignRef(&holder->field_0x0, handle);
    }
}

void fn_710072b9e0(void* unused, UnkHolder* holder) {
    if (!holder->field_0x0 || !holder->field_0x0->target) {
        UnkWeakTarget* target = fn_710072bca8();
        UnkWeakHandle* handle = target ? GetWeakHandle(target) : nullptr;
        AssignRef(&holder->field_0x0, handle);
    }
}

void fn_71007ba354(void* unused, UnkHolder* holder) {
    if (!holder->field_0x0 || !holder->field_0x0->target) {
        UnkWeakTarget* target = fn_71007bb354();
        UnkWeakHandle* handle = target ? GetWeakHandle(target) : nullptr;
        AssignRef(&holder->field_0x0, handle);
    }
}

void fn_710115a9e8(void* unused, UnkHolder* holder) {
    if (!holder->field_0x0 || !holder->field_0x0->target) {
        UnkWeakTarget* target = fn_71006f4a30();
        UnkWeakHandle* handle = target ? GetWeakHandle(target) : nullptr;
        AssignRef(&holder->field_0x0, handle);
    }
}

