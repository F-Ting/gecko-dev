#include <cstddef>
#include <utility>

#define MOZ_MUST_RETURN_FROM_CALLER __attribute__((annotate("moz_must_return_from_caller")))
#define MOZ_MAY_CALL_AFTER_MUST_RETURN __attribute__((annotate("moz_may_call_after_must_return")))

void MOZ_MUST_RETURN_FROM_CALLER Throw() {}
void DoAnythingElse();
int MakeAnInt();
int MOZ_MAY_CALL_AFTER_MUST_RETURN SafeMakeInt();
bool Condition();

// It might be nicer to #include "mozilla/ScopeExit.h" and use that here -- but
// doing so also will #define the two attribute-macros defined above, running a
// risk of redefinition errors.  Just stick to the normal clang-static-analyzer test
// style and use as little external code as possible.

template<typename Func>
class ScopeExit {
  Func exitFunction;
  bool callOnDestruction;
public:
  explicit ScopeExit(Func&& func)
    : exitFunction(std::move(func))
    , callOnDestruction(true)
  {}

  ~ScopeExit() {
    if (callOnDestruction) {
      exitFunction();
    }
  }

  void release() { callOnDestruction = false; }
};

template<typename ExitFunction>
ScopeExit<ExitFunction>
MakeScopeExit(ExitFunction&& func)
{
  return ScopeExit<ExitFunction>(std::move(func));
}

class Foo {
public:
  __attribute__((annotate("moz_implicit"))) Foo(std::nullptr_t);
  Foo();
};

void a1() {
  Throw();
}

int a2() {
  Throw(); // expected-error {{You must immediately return after calling this function}}
  return MakeAnInt();
}

int a3() {
  // RAII operations happening after a must-immediately-return are fine.
  auto atExit = MakeScopeExit([] { DoAnythingElse(); });
  Throw();
  return 5;
}

int a4() {
  Throw(); // expected-error {{You must immediately return after calling this function}}
  return Condition() ? MakeAnInt() : MakeAnInt();
}

void a5() {
  Throw(); // expected-error {{You must immediately return after calling this function}}
  DoAnythingElse();
}

int a6() {
  Throw(); // expected-error {{You must immediately return after calling this function}}
  DoAnythingElse();
  return MakeAnInt();
}

int a7() {
  Throw(); // expected-error {{You must immediately return after calling this function}}
  DoAnythingElse();
  return Condition() ? MakeAnInt() : MakeAnInt();
}

int a8() {
  Throw();
  return SafeMakeInt();
}

int a9() {
  if (Condition()) {
    Throw();
  }
  return SafeMakeInt();
}

int a10() {
  auto atExit = MakeScopeExit([] { DoAnythingElse(); });

  if (Condition()) {
    Throw();
    return SafeMakeInt();
  }

  atExit.release();
  DoAnythingElse();
  return 5;
}

void b1() {
  if (Condition()) {
    Throw();
  }
}

int b2() {
  if (Condition()) {
    Throw(); // expected-error {{You must immediately return after calling this function}}
  }
  return MakeAnInt();
}

int b3() {
  if (Condition()) {
    Throw();
  }
  return 5;
}

int b4() {
  if (Condition()) {
    Throw(); // expected-error {{You must immediately return after calling this function}}
  }
  return Condition() ? MakeAnInt() : MakeAnInt();
}

void b5() {
  if (Condition()) {
    Throw(); // expected-error {{You must immediately return after calling this function}}
  }
  DoAnythingElse();
}

void b6() {
  if (Condition()) {
    Throw(); // expected-error {{You must immediately return after calling this function}}
    DoAnythingElse();
  }
}

void b7() {
  if (Condition()) {
    Throw();
    return;
  }
  DoAnythingElse();
}

void b8() {
  if (Condition()) {
    Throw(); // expected-error {{You must immediately return after calling this function}}
    DoAnythingElse();
    return;
  }
  DoAnythingElse();
}

void b9() {
  while (Condition()) {
    Throw(); // expected-error {{You must immediately return after calling this function}}
  }
}

void b10() {
  while (Condition()) {
    Throw();
    return;
  }
}

void b11() {
  Throw(); // expected-error {{You must immediately return after calling this function}}
  if (Condition()) {
    return;
  } else {
    return;
  }
}

void b12() {
  switch (MakeAnInt()) {
  case 1:
    break;
  default:
    Throw();
    return;
  }
}

void b13() {
  if (Condition()) {
    Throw();
  }
  return;
}

Foo b14() {
  if (Condition()) {
    Throw();
    return nullptr;
  }
  return nullptr;
}

Foo b15() {
  if (Condition()) {
    Throw();
  }
  return nullptr;
}

Foo b16() {
  if (Condition()) {
    Throw();
  }
  return Foo();
}
