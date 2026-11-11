// Copy constructors of two small polymorphic holders in the CCutscene::CTunableCutscene unit: the
// vtable, one word, then a 27-byte payload copied as raw bytes. Names and layouts are placeholders.
#include "game/UnkTU2661.h"
#include "types.h"

struct UnkCutsceneHolder {
    char* vtable;
    u64 field_0x8;
    u8 payload[27];
};

static inline UnkCutsceneHolder* CopyHolder(UnkCutsceneHolder* self, const UnkCutsceneHolder* other, void** vt) {
    self->vtable = reinterpret_cast<char*>(vt + 2);
    self->field_0x8 = other->field_0x8;
    __builtin_memcpy(self->payload, other->payload, sizeof(self->payload));
    return self;
}

UnkCutsceneHolder* fn_71010710e4(UnkCutsceneHolder* self, const UnkCutsceneHolder* other) {
    return CopyHolder(self, other, g_unk_7101cac988);
}

UnkCutsceneHolder* fn_7101071118(UnkCutsceneHolder* self, const UnkCutsceneHolder* other) {
    return CopyHolder(self, other, g_unk_7101cac988);
}

// Builds a holder in storage from fn_7100dddc28, then copies the word and the payload.
UnkCutsceneHolder* fn_7100dddc28(void* alloc, u32 size);

static inline UnkCutsceneHolder* CreateHolder(void* alloc, const u32* size, const UnkCutsceneHolder* other) {
    UnkCutsceneHolder* self = fn_7100dddc28(alloc, *size);
    self->field_0x8 = other->field_0x8;
    __builtin_memcpy(self->payload, other->payload, sizeof(self->payload));
    return self;
}

UnkCutsceneHolder* fn_710106f0a0(void* alloc, const u32* size, const UnkCutsceneHolder* other) {
    return CreateHolder(alloc, size, other);
}

UnkCutsceneHolder* fn_710106f0e4(void* alloc, const u32* size, const UnkCutsceneHolder* other) {
    return CreateHolder(alloc, size, other);
}

// Assignment: the word and the payload, unless assigning to itself.
static inline UnkCutsceneHolder* AssignHolder(UnkCutsceneHolder* self, const UnkCutsceneHolder* other) {
    if (self != other) {
        self->field_0x8 = other->field_0x8;
        __builtin_memcpy(self->payload, other->payload, sizeof(self->payload));
    }
    return self;
}

UnkCutsceneHolder* fn_7101071158(UnkCutsceneHolder* self, const UnkCutsceneHolder* other) {
    return AssignHolder(self, other);
}

UnkCutsceneHolder* fn_7101071184(UnkCutsceneHolder* self, const UnkCutsceneHolder* other) {
    return AssignHolder(self, other);
}
