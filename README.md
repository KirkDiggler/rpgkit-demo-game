# rpgkit-demo-game

A terminal combat game built on [rpgkit](https://github.com/KirkDiggler/rpgkit),
and the live artifact of an experiment: **the game-layer code here is written
by a local ~30B model (qwen)**, working only from rpgkit's tutorials, examples,
and how-to docs. Where the model succeeds, the docs work; where it stumbles,
we file a doc/API issue against rpgkit and re-run. Interventions are the data.

Experiment design: [rpg-project/ideas/rpgkit-demo-game](https://github.com/KirkDiggler/rpg-project/tree/main/ideas/rpgkit-demo-game)

## Play it

```sh
make run
```

First build downloads rpgkit (pinned to a release tag) via CMake
FetchContent — no install step. Requires: cmake ≥ 3.25, a C++20 compiler,
make ([toolchain setup](https://github.com/KirkDiggler/rpgkit/blob/main/docs/tutorials/01-getting-started.md)).

## Layout

```
src/main.cpp     the game (model-written, human-reviewed)
CMakeLists.txt   the whole dependency story: FetchContent + one target
Makefile         make build / run / clean
AGENTS.md        rules for the resident model
```
## Screenshots
<img width="912" height="729" alt="image" src="https://github.com/user-attachments/assets/cfb486ca-6164-4872-b4bc-432027ad1d2c" />

## License

MIT, same as rpgkit.
