// Destructor of the class copied by fn_71007abba4: releases the sub-object's refcounted handle,
// destroys and frees its array of 16-byte type-erased values, releases the string id and
// frees the byte buffer.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "types.h"

struct UnkValue_71007a8eec;

// Function table of a type-erased value; only the destroy slot is named.
struct UnkValueOps_71007a8eec {
    void* field_0x0[8];
    void (*destroy)(UnkValue_71007a8eec* self);
};

struct UnkValue_71007a8eec {
    void* storage;
    union {
        UnkValueOps_71007a8eec* ops;
    };

    ~UnkValue_71007a8eec() {
        if (ops) {
            ops->destroy(this);
            ops = nullptr;
        }
    }
};

class UnkSubBase_71007a8eec {
public:
    virtual void vf_0x0();
    ~UnkSubBase_71007a8eec() { values.Free(); }

    base::UnkVector<UnkValue_71007a8eec> values;
    u32 field_0x68;
};

class UnkSub_71007a8eec : public UnkSubBase_71007a8eec {
public:
    void vf_0x0() override;

    base::UnkRefPtrMember handle;
};

class UnkStruct_71007a8eec {
public:
    virtual void vf_0x0();
    __attribute__((always_inline)) ~UnkStruct_71007a8eec() {}

    struct Buffer {
        base::UnkVector<u8> v;
        ~Buffer() { v.Free(); }
    } buffer;
    base::UnkStrIdMember id;
    UnkSub_71007a8eec sub;
};

void fn_71007a8eec(UnkStruct_71007a8eec* self) {
    self->~UnkStruct_71007a8eec();
}
