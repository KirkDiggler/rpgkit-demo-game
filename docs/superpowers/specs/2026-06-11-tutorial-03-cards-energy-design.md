# Tutorial 03 — Cards & Energy Design

## Goal

Upgrade `src/main.cpp` from its tutorial 02 one-button state to a tutorial 03 deck-builder skeleton: card struct, random dealt hand, energy budget, and healing through chains.

## Scope

### In scope
- `Card` struct (name, cost, damage, heal fields)
- `cardPool()` returning Strike (1e/6dmg), Bash (2e/11dmg), Salve (1e/4heal)
- `dealHand()` — 4 random picks from pool per turn
- Energy budget: 3 energy/turn, inner loop plays cards until exhausted or player opts out
- `printHand()` — shows hand with costs and `[played]` markers
- `resolveHeal()` — healing chain with max-HP cap rule, printed receipt
- `PhaseResult` enum (kTurnOver, kGoblinDown, kQuit) to signal player phase outcome
- Hero `Fighter` gains `maxHp` field; tuning table adds `kEnergyPerTurn`, `kHandSize`
- EOF/kQuit input handling (piped input exits cleanly)

### Out of scope
- Bonus challenge: Smite card (3e/16dmg)
- Bonus challenge: Vampiric Strike (damage + heal)
- Bonus challenge: Energy carryover

## Architecture

Single-file (`src/main.cpp`), no new headers. Same file layout as tutorial 02: tuning table → structs → helpers → main().

### What changes
- `Fighter` struct gains `maxHp` field
- Tuning table grows from 4 to 5 constants (adds `kEnergyPerTurn`, `kHandSize`; updates HP/dmg numbers to match tutorial 03)
- Inline `heroStrikes()` replaced by generalized `cardDamage(const Card&)` — same chain/rule, card source differs
- Single-choice main loop replaced by `playerPhase()` inner loop that drains energy

### What stays the same (untouched)
- `resolveDamage()` — signature, logic, receipt format
- `goblinClaws()` — no changes
- Goblin's turn in `main()` — structurally identical
- `readChoice()` — gains kQuit sentinel only
- `mustBeOk()` helper

## Constraints

- All damage/healing through `rpg::core::Chain<int>` with receipt printed
- Every fallible rpgkit call returns `Status` — check via `mustBeOk`
- One chain per resolution
- File stays under ~400 lines
- No edits under `build/`

## Implementation approach

Dispatch a single subagent with tutorial 03 MD content + current code + AGENTS.md rules. Subagent reads the tutorial's code snippets and writes implementation from scratch (no access to the full example `main.cpp`). Verifies with `make build` and `make run`.
