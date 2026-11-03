// CAudioComponent unit (tools/tu_map.py). fn_710071a7dc plays a sound request through the sound
// manager (g_unk_7101d4d820, fn_71001080b0) when the component can play (vtable slot 47) and the
// owner's (+0x30) component at +0x28, when of the class fn_71011abbd4 names, hasn't reached its
// limit (+0x15c) of sounds of that kind (fn_7100108254); a playing sound whose object asks for it
// (+0x124) gets the component's position (+0x40, fn_7100106024). Returns the sound's handle, or the
// empty one. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkSoundMgr;
extern UnkSoundMgr* g_unk_7101d4d820;

struct UnkSoundObject_b50 {
    u8 pad_0x0[0x124];
    bool field_0x124;
};

struct UnkSoundRef {
    UnkSoundObject_b50* object;
    s64 serial;
};
struct UnkSoundHandle {
    UnkSoundRef* ref;
    s64 serial;
};
extern const UnkSoundHandle g_unk_7108b6c460;  // the empty handle

// The object a handle names while it is still the one the handle was made for.
inline UnkSoundObject_b50* GetSoundObject_b50(const UnkSoundHandle* handle) {
    if (handle->ref && handle->serial != -1 && handle->ref->serial == handle->serial)
        return handle->ref->object;
    return nullptr;
}

struct UnkSoundRequest_b50 {
    u8 pad_0x0[0x20];
    u8 field_0x20[0x8];
};

u32 fn_7100108254(UnkSoundMgr* mgr, const void* kind);
UnkSoundHandle fn_71001080b0(UnkSoundMgr* mgr, const UnkSoundRequest_b50* request);
void fn_7100106024(UnkSoundObject_b50* object, const void* position);

struct UnkSoundLimiter_b50 : UnkReflObject {
    u8 pad_0x8[0x154];
    s32 limit;  // 0x15c
};

struct UnkAudioOwner_b50 {
    u8 pad_0x0[0x28];
    UnkReflObject* field_0x28;
};

struct UnkAudio_b50 {
    virtual void vf_0();
    virtual void vf_1();
    virtual void vf_2();
    virtual void vf_3();
    virtual void vf_4();
    virtual void vf_5();
    virtual void vf_6();
    virtual void vf_7();
    virtual void vf_8();
    virtual void vf_9();
    virtual void vf_10();
    virtual void vf_11();
    virtual void vf_12();
    virtual void vf_13();
    virtual void vf_14();
    virtual void vf_15();
    virtual void vf_16();
    virtual void vf_17();
    virtual void vf_18();
    virtual void vf_19();
    virtual void vf_20();
    virtual void vf_21();
    virtual void vf_22();
    virtual void vf_23();
    virtual void vf_24();
    virtual void vf_25();
    virtual void vf_26();
    virtual void vf_27();
    virtual void vf_28();
    virtual void vf_29();
    virtual void vf_30();
    virtual void vf_31();
    virtual void vf_32();
    virtual void vf_33();
    virtual void vf_34();
    virtual void vf_35();
    virtual void vf_36();
    virtual void vf_37();
    virtual void vf_38();
    virtual void vf_39();
    virtual void vf_40();
    virtual void vf_41();
    virtual void vf_42();
    virtual void vf_43();
    virtual void vf_44();
    virtual void vf_45();
    virtual void vf_46();
    virtual bool vf_47();

    u8 pad_0x8[0x28];
    UnkAudioOwner_b50* owner;  // 0x30
    u8 pad_0x38[0x8];
    u8 field_0x40[0x10];
};

UnkSoundHandle fn_710071a7dc(UnkAudio_b50* self, const UnkSoundRequest_b50* request) {
    if (!self->vf_47())
        return g_unk_7108b6c460;
    UnkReflObject* component = self->owner->field_0x28;
    if (component && fn_710009d50c(component->vf_0x0(), fn_71011abbd4())) {
        s32 limit = static_cast<UnkSoundLimiter_b50*>(component)->limit;
        if (limit >= 0 && fn_7100108254(g_unk_7101d4d820, request->field_0x20) >= (u32)limit)
            return g_unk_7108b6c460;
    }
    UnkSoundHandle handle = fn_71001080b0(g_unk_7101d4d820, request);
    if (UnkSoundObject_b50* object = GetSoundObject_b50(&handle)) {
        if (object->field_0x124)
            fn_7100106024(object, self->field_0x40);
    }
    return handle;
}

// fn_71007193d4: after the base step (fn_71010c4228), sets up the emitter at +0x70 (fn_7100719444,
// flag 0x1000) when the owner (+0x30) has its component at +0x200, then resets the volumes at +0xec
// to 1 and the values after them to 0.
struct UnkParent;
void fn_71010c4228(UnkParent*);

struct UnkAudioOwner2_b50 {
    u8 pad_0x0[0x200];
    void* field_0x200;
};

struct UnkAudioInit_b50 {
    u8 pad_0x0[0x30];
    UnkAudioOwner2_b50* owner;
    u8 pad_0x38[0x38];
    u8 field_0x70[0x7c];
    f32 volumes[11];  // 0xec
    u32 values[16];   // 0x118
};

void fn_7100719444(void* emitter, u32 flags);

void fn_71007193d4(UnkAudioInit_b50* self) {
    fn_71010c4228(reinterpret_cast<UnkParent*>(self));
    if (self->owner->field_0x200)
        fn_7100719444(self->field_0x70, 0x1000);
    for (int i = 0; i < 11; ++i)
        self->volumes[i] = 1.0f;
    for (int i = 0; i < 16; ++i)
        self->values[i] = 0;
}
