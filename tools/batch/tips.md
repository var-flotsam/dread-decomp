# Matching tips

Source forms that reproduce what Clang 9 does, gathered by the agent batches of 2026-11-09 to 2026-11-11 and
kept where they are still allowed. Every tip here is ordinary C++ a developer could have written. Tips that
steered the compiler with forms the rules now rule out (barriers, inlining attributes, unions around a member,
untyped stores to force a reload, byte-offset casts, macros standing in for a helper) were taken out on
2026-11-14; git history has the old list. When the only way to match is one of those, the function is a
registers-only or other non-match and goes by `CLAUDE.md`, "Matching standard".

## Order of compares and branches
- Choosing among values: Clang 9 emits compares in reverse source case order; reorder the switch cases.
- Float compares with b.hi / b.ls: write the condition negated, e.g. `!(x <= 0)`.
- fcmp register order: `node_x < a` and `a > node_x` give different code.
- `fmin` against `fminnm`: `!(v >= 1.0f) ? v : 1.0f` gives fmin; `v < 1.0f ? v : 1.0f` gives fminnm.
  Likewise `!(t < 0.0f) ? t : 0.0f` gives fmax and `t > 0 ? t : 0` gives fmaxnm.
- `(a == empty) ^ (b == empty)` reproduces cset/eor/tbz with the original's compare order.
- cmp/ccmp with a hoisted load: a non-short-circuit `(p == nullptr) | (x == 1)` inside a helper that takes the
  block by reference (`T&`, so the pointer is known dereferenceable).
- Bool to float: `return flag;` gives the original fcsel order; `flag ? 1.0f : 0.0f` swaps it.
- Inlined lookups building an iterator in place: put the out-of-range case first.
- Hash compares turned into a switch: put the id compare between them; read the hash through an inline helper.
- One call with two arguments on two paths: write a separate call on each branch instead of a `?:` argument.

## Order of loads and stores
- 3-component zero constructors keep the original store order; brace-init reverses it.
- Two stores emitted in reverse of source order: write them reversed; load vector components into locals
  before filling a struct.
- Constructor store order: build members with constructors that use member-initializer lists. A flag set in
  the constructor body and one set in the init list pair their stores differently.
- Load order before an indirect call: copy the arguments into locals first.
- Load order around a temporary string id: compare through an inline helper taking both ids by `const&`.
- fadd operand order: `x = a; x += b;`.
- Identity matrix: zero it, then set the diagonal in reverse (`m[15]`, `m[10]`, `m[5]`, `m[0]`).
- Reloads after a store come from type-based alias analysis: the field's real type decides them. Byte arrays
  whose count is reloaded after an element store have `u8` elements, not `bool`; out-flags written as `u8*`
  rather than `bool*` reload after the store. Look for the field's real type before anything else.
- One 64-bit load of two u32 fields: the two fields are one `u64` member with accessors.
- Static helpers get argument promotion (loads move above stores); a non-static inline helper keeps the
  original order.

## Calls, temporaries and the stack
- Allocas: the first-declared local gets the highest address; write nested string concatenations directly in
  the body to reproduce the temporaries' order.
- Scope a local so its lifetime ends early to reproduce stack slot sharing. A local declared in a switch body
  before a later case label gets no lifetime markers (its own stack slot).
- Missing 8-byte stack slot: often the 8-byte type-id argument struct, or a 16-byte struct whose last word
  is not set.
- `alignas(8)` on a u32 temporary passed by address reproduces an 8-byte stack slot.
- Temporaries passed by `&&` built in the call (`f(T())`) are destroyed straight after the call.
- An out-of-line callee typed with the element's own pointer type (not `void*`) lets Clang reuse the element
  pointer after the call.
- Passing a picked pointer by reference (`*p`) into a helper deletes the original's null path: use pointer
  parameters.
- Caller and callee in one file can make Clang drop unread arguments or keep values in registers across the
  call: the original's units decide which functions share a file (`tools/tu_map.py`).
- Clang devirtualizes calls on a polymorphic member object: model it as its own struct and call through a
  pointer.
- 12-byte result returned in x8: `UnkVec3f` as the return type (its user copy constructor makes it
  non-trivial).
- Initializer-list constructor arguments arrive as (x1 pointer, x2 size).
- A RAII mutex-lock struct fixed register allocation in the sound play functions.

## Shapes the game uses
- Class checks: the original calls the object's `vf_0x0` before the `refl_GetClass` getter; a helper
  templated on the getter keeps that order.
- `CStrId` temporaries whose destructor must fold away: build them with `UnkStrId` and move-assign. Two-step
  string-id release in destructors: `CStrId::Reset()`.
- Linear searches in some units only match as libc++ `std::find_if` / `find_if_not` over
  `src/game_s7/UnkIter.h`. Lookup by name: `std::find_if` with the lambda capturing the key by value (`[key]`).
- Linear search against end without folding: `Find()` returns `&values[count]` on a miss; compare with
  `&table->values[table->count]` through the table pointer.
- "Add one, return last" (GrowAt-style): `T* end = &data[n]; return n ? end[-1] : *end;`.
- Element-copy loops: walking dst/src pointers and a u32 counter; a zeroed key gets a constructor and
  `data[i] = T()`. Element-move loops: `Entry* old = src++; old->~Entry();`.
- Index erase over a polymorphic vector: as `fn_71004bf588` writes it.
- Arrays of unique_ptr: `UnkOwnedPtr` from `game_s2/UnkOwnedList.h`.
- Unit static initialisers (`_GLOBAL__sub_I`-style functions): see `game/UnkTUInit.h`. unique_ptr-with-
  std::function tables match with plain libc++ types.
- Guarded statics reached through the GOT: a function-local static inside a non-static `inline` function,
  named for the original's symbol through its declaration (`tools/dread/asm_labels.py`).
- Direction between two actors: `Pos(a) - Pos(b)` with an inline `Pos()` returning a 3-float struct and
  `operator-` taking const references.
- Deleting destructors drop their last dead store only with `__builtin_operator_delete`.
