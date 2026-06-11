# Game Status

_Last updated: 2026-06-11 by Claude (setup)_

## The game right now

A playable terminal fight: hero vs goblin, one Strike button, all damage
through rpgkit chains with the receipt printed every hit. (The tutorial-02
base game, rebuilt standalone against rpgkit v0.1.0.)

## Implemented

- Turn loop: player strikes, goblin claws back, first to 0 HP loses
- Damage receipts via `rpg::core::Chain<int>` (goblin's tough-skin rule)
- Safe input reading (bad input re-prompts, EOF quits cleanly)
- Standalone build: FetchContent pins rpgkit v0.1.0; `make run` plays

## In progress / next

- Follow rpgkit tutorial 03 (cards & energy): Card struct, dealt hand of 4,
  3-energy budget, Salve heal chain with max-hp cap

## Known issues

- None recorded.

## Round log

- 2026-06-11: round 2 — rebuilt the game standalone from the
  start-your-own-game how-to (FetchContent v0.1.0). No interventions needed.
- 2026-06-10: round 1 — first game built inside the rpgkit clone; finding:
  no doc taught the standalone-project pattern (fixed by rpgkit#9).
