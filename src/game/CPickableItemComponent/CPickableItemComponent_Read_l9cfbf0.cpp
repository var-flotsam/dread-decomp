// Unit 1493 (CPickableItemComponent): reads an item description from a file: two values, six strings
// and a flag for fn_71009cfe98, then, while the file has data left, two flags, a handle by name and a
// value. Names and layouts are placeholders.
#include "base/global/CRntFile.h"
#include "base/global/CStrId.h"
#include "game/UnkTU56.h"
#include "types.h"

using base::global::CRntFile;
using base::global::CStrId;

struct UnkStrIdHandle {
    CStrId id;
    void* field_0x8;
};
void fn_7100ae986c(UnkStrIdHandle* self, const CStrId& id);

// A handle built in place from an id by fn_7100ae986c, released when it goes out of scope.
class UnkStrIdHandleTemp_l9cfbf0 {
public:
    explicit UnkStrIdHandleTemp_l9cfbf0(const CStrId& id) { fn_7100ae986c(get(), id); }
    ~UnkStrIdHandleTemp_l9cfbf0() { get()->id.~CStrId(); }
    UnkStrIdHandle* get() { return reinterpret_cast<UnkStrIdHandle*>(storage); }
    const UnkStrIdHandle& value() { return *get(); }

private:
    alignas(UnkStrIdHandle) char storage[sizeof(UnkStrIdHandle)];
};

struct UnkItemDesc_l9cfe98;
void fn_71009cfe98(UnkItemDesc_l9cfe98* self, f32 value_0xa8, f32 value_0x50, const char* id_0x98,
                   const char* kind, const char* id_0xa0, const char* id_0xb8, const char* string_0xc0,
                   const char* string_0xf0, bool flag);

struct UnkPickableItem_l9cfbf0 {
    u8 pad_0x0[0xac];
    u32 field_0xac;
    u8 pad_0xb0[0xf0];
    UnkStrIdHandle field_0x1a0;
    u8 field_0x1b0;
    u8 field_0x1b1;
    bool field_0x1b2;
};
static_assert(__builtin_offsetof(UnkPickableItem_l9cfbf0, field_0x1a0) == 0x1a0, "layout");

static inline bool HasData(const CRntFile* file) {
    return file->position < file->size;
}

bool fn_71009cfbf0(UnkPickableItem_l9cfbf0* self, CRntFile* file) {
    const char* id_0x98;
    f32 value_0xa8 = 0.0f;
    f32 value_0x50 = 0.0f;
    const char* kind;
    const char* id_0xa0;
    const char* id_0xb8;
    const char* string_0xc0;
    const char* string_0xf0;
    bool flag;
    fn_710007c00c(file, 0, 0);
    fn_710007cee4(file, &id_0x98);
    fn_710007c714(file, &value_0xa8, 4, 1);
    fn_710007c714(file, &value_0x50, 4, 1);
    fn_710007cee4(file, &kind);
    fn_710007cee4(file, &id_0xa0);
    fn_710007cee4(file, &id_0xb8);
    fn_710007cee4(file, &string_0xc0);
    fn_710007cee4(file, &string_0xf0);
    fn_710007c714(file, &flag, 1, 1);
    fn_71009cfe98(reinterpret_cast<UnkItemDesc_l9cfe98*>(self), value_0xa8, value_0x50, id_0x98, kind,
                  id_0xa0, id_0xb8, string_0xc0, string_0xf0, flag);

    if (HasData(file)) {
        u8 value;  // unset in the original: the read fills it
        fn_710007c714(file, &value, 1, 1);
        self->field_0x1b1 = value;
    }
    if (HasData(file)) {
        u8 value = 0;
        fn_710007c714(file, &value, 1, 1);
        self->field_0x1b0 = value;
    }
    if (HasData(file)) {
        const char* name;
        fn_710007cee4(file, &name);
        self->field_0x1a0 = UnkStrIdHandleTemp_l9cfbf0(CStrId(name, true)).value();
    }
    if (HasData(file))
        fn_710007c714(file, &self->field_0xac, 4, 1);
    self->field_0x1b2 = false;
    return true;
}
