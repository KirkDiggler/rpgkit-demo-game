# Rules for agents working in this repository

You are building a terminal game on top of the rpgkit library. Game code
only — the engine is a dependency, not your code.

## Hard rules

1. **Never edit anything under `build/`** — it is generated, including the
   rpgkit sources FetchContent downloads into `build/_deps/`. If you think
   you need to change rpgkit itself, STOP and report it instead: that is a
   finding, not a task.
2. **All damage/healing numbers resolve through `rpg::core::Chain`** and the
   breakdown receipt is shown to the player. No bare `hp -= x` for anything
   a rule could ever modify.
3. **Every fallible rpgkit call returns a `rpg::core::Status` — check it.**
   The `mustBeOk` helper pattern in `src/main.cpp` is the way.
4. **One chain per resolution.** Build it, add the rules that apply right
   now, execute, let it go out of scope.

## Your reference material (read before writing)

- Tutorials: https://github.com/KirkDiggler/rpgkit/tree/main/docs/tutorials
- Recipes: https://github.com/KirkDiggler/rpgkit/tree/main/docs/how-to
- Worked examples: https://github.com/KirkDiggler/rpgkit/tree/main/examples

## Definition of done, every task

```sh
make build    # compiles clean
make run      # play the affected path yourself via piped input if needed
```

A task is not done with a failing build, and not done untested.

## Style

- Tuning constants (`constexpr int kThing`) at the top — every game number
  named, in one place.
- Comments explain WHY, matching the style already in `src/main.cpp`.
- Keep `main.cpp` under ~400 lines; propose splitting into files when a
  task would push past that, and wait for approval.
