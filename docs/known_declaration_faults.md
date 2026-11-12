# Known faults in declarations

Declarations that are known or suspected to be wrong, and the matches that work around them. The agent
runs of 2026-11-09 to 2026-11-11 collected this list as they hit each one (their `header_fixes.txt`, which
existed only in one worktree's `build/` folder); the batch ids say which batch reported it.

It matters twice. A wrong declaration blocks other matches or forces a workaround, and on x86-64 it is a
crash, not a comment: a result declared too small or an argument in the wrong class corrupts the caller in
the port (journal, 2026-11-08 and 2026-11-09). The entries are the workers' reports and have not all been
checked against the original. Remove an entry when its declaration is corrected, in the same change.

## Wrong return type

| Symbol | Declared | The original | Reported by |
|---|---|---|---|
| `fn_71011d9e90`, `fn_71010e655c`, `fn_7100362324`, `fn_710000f750` | `void` | returns a value | b0005, b0001, b0026, b0032 |
| `fn_710007b518` | `void` (in a header) | returns a pointer the caller uses | b0099 |
| `fn_71011a32d8`, `fn_71011a3574` | `void` | return values that are used; reached as `bool` through `asm`-label aliases in `UnkTU2577_SubAreaMgr_ovb0189.cpp` | b0189 |
| `fn_7100ef0cdc`, `fn_7100ef11f4` | `void()` in `function_inits_02.cpp` | not stated | b0189 |

## Wrong parameters

| Symbol | Declared | The original | Reported by |
|---|---|---|---|
| `fn_7100004578` | a count parameter that is not `u64` (`include/base/UnkResType.h`) | matches when it is `u64` | b0033 |
| `fn_71007616e8` | as `src/clones/clones_02.cpp` declares it | defined with another signature (matters for the port's link) | b0028 |
| `fn_71011a1d00` | no `self` parameter (`src/game/CScenario.cpp`) | takes `self` | b0032 |
| `fn_710076438c` | lacks a `bool` argument | takes it | b0001, b0008 |
| `fn_710014580c` | one argument (`src/game/CFXComponent/fn_7101152b90.cpp`) | two; b0052 casts at the call | b0052 |
| `fn_71010d4234` | `char*(char*)` in 4 files | a caller passes a float | b0048 |
| `fn_7101105e14` | with an `f32` (`CQuetzoaAIComponent_Dodge_A3_b76.cpp`) | the call passes none; blocks `fn_710113430c` | b0050 |
| `fn_7100caacd0` | `void()` (`CBTObserverComponent.cpp`) | `(self, const u32*)`; matched in scratch, blocked | b0055 |
| `fn_7100ca7a64` | `(void*, u16)` (`game_s2/fn_71002b3828.cpp`) | takes `u32`; matched in scratch, blocked | b0055 |
| `fn_7100f0f4d0` | `void()` (`src/game/CPlayerInfo.cpp`) | `(self, message)` | b0071 |
| `fn_71007b70d8` | `void(void*)` (`CDoorEmmyFXComponent.cpp`) | two parameters | b0074 |
| `fn_71011b7ec0` | 2 parameters (`include/game/UnkGuiFrames.h`) | a caller passes 3 | b0080 |
| `fn_71009ea3e4` | with an `f32` | the original leaves it unset | b0103 |
| `fn_7100df31b8` | third argument `bool` | the caller passes 2; its matching draft is held back in `drafts/` | b0103 |
| `fn_7100219558` | third argument `bool` | `u32`; called through a function-pointer cast in `UnkTU2666_lua_ovb0157`/`ovb0159` | b0157 |
| `fn_7100eb33c8`, `fn_7100eb2eac` | one argument | need a second | later batch |
| `fn_7100eaf72c` | no parameters | takes some | later batch |
| `fn_7100331c7c` | 2 arguments | 1; `UnkTU437_Aim_ovb0198.cpp` declares it through an `asm` label | b0198 |
| `fn_7100b43b30` | not stated | `UnkTU1689_MapMenu_ovb0203.cpp` declares it through an alias | b0203 |
| `fn_71001cde84` | takes `bool` | `u32` first argument; `UnkRenderCalls_b0253` reaches it as a static member | b0253 |
| `fn_71000e88c0` | a `CClass*` getter in the generated `include/base/reflection/UnkClassGetters.h` | a `void` descriptor initialiser: the generator names it wrongly | b0136 |

## Matches to look at again

| Where | What |
|---|---|
| `fn_71011990e8` (`UnkTU2730_Lists_ovb0188.cpp`) | passes an uninitialised first argument to `fn_7101198e88` to match; the intended argument is unknown |
| `fn_7100b532a0` (`UnkTU1703_Title_ovb0215.cpp`) | the switch's state-to-callee order is a guess: jump tables are not compared by `check.py` |
| `fn_71007121e0` | defined `__attribute__((weak))` so its caller in the same file keeps the original's reload |
| `fn_710002ec40`, `fn_71005fd088`, `fn_71007b5c48` and others with function-local guarded statics | the static is a compiler-named symbol, not a `g_unk_` one, so its address is not checked (as in the older `fn_7100adcf10.cpp`) |
| `fn_71007a6b58` (`NON_MATCHING` minor) | the element-move loop may match written as `Entry* old = src++; old->~Entry();` |
| `UnkTU811_Emmy_ovb0193.cpp`, `CPerceptionComponent_Ctor_ovb0266.cpp`, the weak destructor pair of `fn_710077afa4`/`fn_710077b1e8` | `trymatch.py` reports a function "not mapped to an address" |

Resolved since the runs: the two string literals of `fn_7100775358` (`UnkTU1176_Central_ovb0230.cpp`) that
were taken from a truncated listing are the original's (`check.py` compares string contents since #355);
`fn_7100ac82c0`'s `void` declaration was corrected to `s32` in the lean run.

## Structs defined in several files

The same struct written out in more than one `.cpp`, each a separate definition in the port's one program:
`UnkResolvedValue` (b0106), `UnkRef_c3` (b0065), `UnkFxObject_ovb0041` (b0041, b0046), `UnkShotWeakRef` (b0006),
`UnkSamusSettings` (b0004). These are the ones the workers noticed; `tools/decl_check.py` does not look at
struct definitions, and 1,020 struct names are defined in more than one file.

One of the largest was settled on 2026-11-12: `UnkGameManagerIdTable`, the game's table of string ids, was
defined in 60 files, each with the one or two slots it reads and its own copy of the labelled constructor and
destructor (`fn_7100031894`, `fn_71000407f0`). It is `include/game/UnkGameManagerIdTable.h` now, with every slot
those files name as a `CStrId`; a file that reads a slot's data as a record of its own casts at the read. 16
more files that saw the same table under a struct name of their own (`UnkIdTable_b55sh`, ...), with the
constructor named in a comment and not by a label, were folded in the same day. Two views are left:
`UnkReflSingleton` in `base/reflection/UnkReflStatics.h`, which the generated sources use, and the one in
`src/game_s7/UnkPropS7.h`, which reads the table as bytes.

## Shared functions the mechanical header migration could not move

`tools/decl_plan.py` and `tools/hub_migrate.py` moved 1,353 shared functions' declarations into unit headers on
2026-11-11. 43 of those tried are still left with every local copy in place, because moving them changed a
file or stopped it compiling. Each needs a person to settle the type. (`tools/decl_plan.py --exclude` takes
this list; remove a symbol here when it has moved.)

**31: the header's forward declaration clashes with a type of another kind that a file defines under the same name (a typedef, a template alias, a type in a namespace).**
`fn_710007b444` `fn_710009fb38` `fn_710009ffd4` `fn_71000fbce0` `fn_71002295ac` `fn_7100229688` `fn_7100231b40` `fn_710026d2e4` `fn_710027fc88` `fn_7100287598` `fn_71002af070` `fn_71002af6fc` `fn_71002b3114` `fn_71002b3328` `fn_71002b52a4` `fn_71002f239c` `fn_7100353be8` `fn_7100407170` `fn_7100d46804` `fn_7100d46c60` `fn_7100d476e4` `fn_7100ee34f0` `fn_7100ee62bc` `fn_7100ee6884` `fn_7100ef1e1c` `fn_7100f84868` `fn_7100f85f00` `fn_7100f86020` `fn_7100f86254` `fn_7100faa3d8` `fn_710119a110`

**7: the file's compiled output changed.**
`fn_71005c1008` `fn_7100895498` `fn_710098ae04` `fn_71009a60fc` `fn_71009a64cc` `fn_710111b230` `fn_71011201e4`

**5: a file uses a type the header does not bring.**
`fn_71007142e8` `fn_710099d6f0` `fn_71009a1f40` `fn_71009b0800` `fn_71010d1598`

A third pass on 2026-11-12 moved 437 more symbols, none left over: the planner forward-declares a type the
declaring file takes from an include when every file of the tree agrees on it (`tools/decl_plan.py`,
`type_index`).

A fourth pass on 2026-11-12, after the conflicting signatures were settled, moved 91 more symbols, 16 functions and 11 globals of the lists here among them (the lists and their counts are what is left).

A fifth pass on 2026-11-12 moved 423 more symbols (16 functions and 4 globals of the lists here among them). The planner learned two things for it: a type that exactly one header under
`include/` defines (`CStrId`, `CRntString`, `UnkVec3f`) is used through that header, with its full name in
the declaration; and a name that some file reaches through a namespace or defines as an alias is never
forward-declared. What a declaration needs in its header now travels with it in the plan, so
`tools/hub_migrate.py` writes it with the declaration and takes it back with it.

## Shared globals the mechanical header migration could not move

`tools/decl_plan.py` and `tools/hub_migrate.py` moved 674 shared symbols' declarations (672 globals and two
functions) into unit headers on 2026-11-11, each global into the header of the unit whose data span holds it
(`tools/data_map.py`). 22 of those tried are still left with every local copy in place. (They are in the
`--exclude` list with the functions above; remove a symbol here when it has moved.)

**18: the header's forward declaration of the global's type clashes with a type of the same name in a namespace that a file opens (the compiler: "reference is ambiguous").**
`g_unk_7101a45f90` `g_unk_7101a45fe8` `g_unk_7108b6d8f0` `g_unk_71726dfca0` `g_unk_71727115d0` `g_unk_7172731200` `g_unk_71729f0188` `g_unk_71729f0190` `g_unk_71729f0198` `g_unk_71729f01a0` `g_unk_71729f01a8` `g_unk_71729f01c8` `g_unk_71729f0228` `g_unk_71729f0238` `g_unk_71729f0240` `g_unk_71729f0248` `g_unk_71729f0298` `g_unk_71729f02a8`

**1: the global's unit header also declares a function that one of the files declares with another return type (the functions of the list above), so that file cannot include the header.**
`g_unk_7101d4bbc0`

**3: a file's `using` declaration or typedef clashes with the header's forward declaration.**
`g_unk_7172730858` `g_unk_7172735f58` `g_unk_7172795e08`

Not tried in this pass: 328 globals whose type comes from a shared header (`UnkReflType`, `UnkConverter`,
`CStrId`, `UnkProperty`, `UnkTUHeader`), 95 that lie between two units' data spans, 92 with conflicting
signatures, and 2 whose copies differ in a visibility attribute. (The conflicting ones are down to 10 since 2026-11-12, see below.)

## Symbols declared with more than one signature

Two files that name the same struct differently declare the same function, or the same global, with two
signatures: the same call, and two symbols for the port's linker. 135 symbols were like that. On 2026-11-12
each file that differed from the usual declaration got that declaration, and says at the use what it reads the
object as (`fn(reinterpret_cast<UnkWorld*>(model), hash)`, `reinterpret_cast<UnkManager*>(g_unk_71726bc570)`):
164 files, each kept only with unchanged output. The usual declaration is a header's, else the most common;
for a global a typed declaration wins over a `char[]` or `void*` placeholder, and a file that only reads a
typed pointer as `void*` needs no cast.

20 are left (`python tools/decl_check.py --list` shows them first):
- `fn_7100010e58`, `fn_71002ee1dc`, `fn_7100ad61f8`: class getters that a static-initialiser file declares as
  returning its own `UnkEnumType*`; the shared declaration returns `const CType*`, and the file's templates take
  the getter as a template argument.
- `fn_71002aeb9c`, `fn_710031da18`, `fn_71007ed25c`: passed as a template argument whose parameter has the
  file's own struct name.
- `fn_7100745e24`: returns a three-float struct by value that two groups of files define under two names.
- `fn_7100862088`: one side returns a reference, the other a pointer to pointer; the output changes.
- `fn_71011428bc`: a `const UnkQualifiedId&` against a `const CStrId*` built in place at the call.
- `fn_7101177044`: the flag class it takes by value has two names (`UnkFlags_7101176690`, `..._7101176ea8`).
- `g_unk_7101aa2b80`, `g_unk_7101aa3030`, `g_unk_7101aa3440`, `g_unk_7101aa7d00`: vtables passed to a
  template that wants the file's `char[]`.
- `g_unk_7101d47360`, `g_unk_71726bc560`: one file passes the singleton to a template by reference to `void*`.
- `g_unk_71726bc570`: `src/game_s7/UnkGuiS7.h` declares it in a namespace with its own type.
- `g_unk_7101d4cfb0`, `g_unk_7172794d40`: a `const CType` that one file writes through as `UnkReflType`.
- `g_unk_7172759178`: a template type the other file does not have.

## Members bound to their function by a comment only

A constructor, destructor or method declared in a struct and written `Foo();  // fn_<addr>` tells a reader
which function it is and tells the port's linker nothing. The owner's rule (2026-11-12): a labelled member is
the declaration, `Foo() asm("fn_<addr>");`. 26 members got their label on 2026-11-12, each checked first: the
original's calls of that member go to the labelled address in every matching function that calls it. 112
member declarations over 62 symbols still have the comment only:
- 3 that no matching function calls, so the comment could not be checked (`fn_7100133be4`, `fn_7100b262f4`,
  `fn_71000c2f58`);
- 9 whose function a hand-written file also defines as a free function (`fn_7100082c38` in
  `src/base/UnkDefaultAllocator.cpp` is `new (self) UnkDefaultAllocator();`): there the labelled constructor
  would be the function that calls it, and the port's compiler does not come back from that file;
- the ones whose function is also declared as a free function somewhere (`CRntString`'s constructors against
  `fn_710007e704` in `base/UnkNamedRegistry.h`, and 30 more): the label would be a second signature of the
  symbol, so the free declaration has to go first, and its callers have to construct the object instead;
- the ones whose struct is defined in several files (the 25 other views of the id table, the callback sets):
  the label would be a copy in each file, so the struct needs one definition first.

## Definitions that disagree with their declaration

`python tools/decl_check.py --definitions` lists every hand-written definition whose result or parameters
are not what a declaration of the same function in another file says. On AArch64 such a pair still matches,
because the registers hold the right values anyway; in the PC port the call is wrong.
`tools/decl_check.py --base` fails when a pull request adds one. To settle one, read what the original does
with the result and argument registers on both sides of the call, give the header the true signature, and
make the defining file include that header.

There were 312 on 2026-11-11, 168 of them with `void` on one side and a value on the other. 100 of those
are settled: in 96 the definition returns a value (it matches, and the original sets the result register
or a caller reads it) and the `void` declarations took its result type; in 4 the definition is `void`, no
caller in the original reads a result, and the declarations became `void`. Every touched file builds to
the same output. Six more were settled by reading the original (#372): `fn_7100362324` returns the entry
`fn_71003646b0` returns, and `fn_7100017ba0`, `fn_710063c670`, `fn_710081b510` and `fn_7100d8e0a0` return nothing,
so the functions that end by calling them return nothing either.

85 more were settled on 2026-11-12 by trying both directions and letting the byte match decide. First the
declarations were rewritten to the definition's shape: where every file that uses them still built to the
same output, the callers were already passing exactly that (48). Then the definition was reshaped to what
its callers use (a parameter's type, parameters the callers pass and it ignores, the order of a float
parameter, the result's type), kept where the definition still matched (37). Two reshaped definitions were
put back: the original passes the extra register on to another call (`tools/forwarded_args.py` lists
`fn_7100232c90` and `fn_7100d7babc`), so the parameter has to be forwarded, not just declared.

A third step the same day settled 29 more. In 26 the declaration was `void fn()` or the like only because a
file stores the function's address in a generic slot, or passes it to a registration call
(`InitConverter(..., fn)`, `link->callback = fn`): the declaration now has the true signature and the place
that erases the type says so, `reinterpret_cast<void (*)()>(&fn)`. And the check stopped counting a 64-bit
integer against a pointer, which is one full register on every 64-bit target (15 pairs).

The last 82 were read one at a time against the original, on both sides of the call (2026-11-12), and 81
are settled, every touched file building to the same output. What they turned out to be:
- *A register the caller leaves as it came* (12). The source called `fn(self)` and matched because the
  original never touches x1, x2 or s0 before the call: the argument is the caller's own parameter, or a value
  it had just computed, passed on. Written out (`fn_710020ca64(member, from)`, `fn_71006badf4(self, v)`,
  `fn_7100979854(self, xform, changed)`, the three path-text builders), the code is the same and
  `tools/forwarded_args.py` has four hits fewer. One empty barrier that kept such a register alive is gone.
- *A function handed to a template or a registration call* (31). The template's parameter, or the slot, said
  `void (*)(...)` and the function returns a value (the erase functions of the keyed maps, the reflection
  invokers, the HUD's bound calls). The template parameter now has the real type; a slot that erases the type
  says so with a cast.
- *A float in another place, flags as words* (11). Integer and float arguments travel in separate registers, so
  `(p, 1, 0, 0.0f, 1.0f)` and `(p, t, 0.0f, 1.0f, true, false)` are the same call; the definitions' order and
  `bool` are what the original reads (`and w1, w1, #1`).
- *A class passed by value against a pointer* (6). A class with a copy constructor travels as the address of
  the caller's copy. Where the caller makes that copy (`fn_71000f9d20`, `fn_7101190890`), the parameter is the
  class by value; where the callee reads it again after each call it makes (`fn_7100eea4c8`), it is a const
  reference.
- *A result nobody could see* (14). A caller that ends in a tail call matches under any return type, its own
  included: `fn_71005028e4` and `fn_710076438c` return nothing, `fn_71007038f4` and its three callers return
  nothing, `fn_7100202e7c` forwards a word (as `bool` it gains an `and w0, w0, #1`). Two callers that were
  `NON_MATCHING(minor)` match now that their callees have the right type (`fn_71003f1b00`, `fn_71003ad8e4`).
- *A parameter the callers set and nothing reads* (2): `fn_7100232c90` and `fn_710110bb4c` now declare it,
  unnamed, with the reason above them.
- `fn_7100fc9d78`, declared `void` in 20 files, returns a sound handle; it and the handle
  (`game/sound/UnkSoundHandle.h`, 24 local copies of the struct) are declared once.

One is left: `fn_71003f4bc0`. Its caller negates the result as a `bool` (`mvn w8, w0; and w1, w8, #1`), and its
own code only matches when it returns a word (as `bool` its three returns come out as `mov w8, ...; and w0, w8,
#1`). Seven ways of writing its ending were tried.

`fn_71000e88c0` is a different fault with the same look: the generated `UnkClassGetters.h` lists it as a class
getter and it is an initialiser that returns nothing; the generator's list is wrong for it.

## Names that point to more than one address

`python tools/name_check.py --version 2.1.0` compares, for every name `tools/check.py` cannot resolve to an
address (a named constructor, a vtable by class name, a local static), where the original points at each
reference to it. A 4-byte function that only branches on counts as its target, and functions with the same
code count as one meaning (the original has several instantiations of `base::refl_IterSync` over lists of one
layout, and two copies of `std::__throw_bad_function_call`).

On 2026-11-12 it passes with `--strict`: 109,340 references under 11,776 names, none pointing to more than one
address. What it found on the way: a string destructor's name on a call the original makes to `fn_710026ead0`
itself (#369), and three placeholder property classes of `src/game_s7/UnkAnyPropsS7.h` that stood for five
sets of classes, one per record; each including file now names its set (`UNK_ANY_PROPS_SET`).
