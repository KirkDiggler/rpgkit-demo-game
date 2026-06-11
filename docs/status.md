# Game Status

_Last updated: 2026-06-11 by Claude (tutorial 03 bonus challenges)_

## The game right now

A deck-builder skeleton: hero gets 4 random cards each turn (Strike/Bash/Salve/
Smite/Vampiric Strike), 3 energy/turn with carryover capped at 5, healing caps
at max HP via chain rules. All damage & healing through rpgkit chains with
receipts. Goblin at 30 HP, claws for 6 damage.

## Implemented

- Tutorial 03 deck-builder: Card struct, dealt hand (4 cards), energy system (3/turn, carryover capped at 5)
- Five card types: Strike (1 cost, 6 dmg), Bash (2 cost, 11 dmg), Salve (1 cost, 4 heal), Smite (3 cost, 16 dmg), Vampiric Strike (2 cost, 5 dmg + 3 heal)
- Healing chain with max-HP cap rule (room = maxHp - hp)
- Player phase loop: play cards until out of energy or end turn voluntarily
- Three refusals: invalid card choice, already played, can't afford
- Turn loop: player phase → goblin claws → check deaths
- Damage receipts via `rpg::core::Chain<int>` (goblin's tough-skin rule)
- Safe input reading (bad input re-prompts, EOF quits cleanly)
- Standalone build: FetchContent pins rpgkit v0.1.0; `make run` plays

## In progress / next

- (awaiting next tutorial or feature request)

## Known issues

- None recorded.

## Round log

- 2026-06-11: round 3b — tutorial 03 bonus challenges: Smite card, Vampiric Strike, energy carryover capped at 5
- 2026-06-11: round 3 — implemented tutorial 03 (cards & energy): Card struct,
  dealHand() with RNG, 3-energy player phase loop, heal chain with max-hp-cap
  rule, three refusals (invalid/played/can't afford). All tests pass. No
  interventions needed.
- 2026-06-11: round 2 — rebuilt the game standalone from the
  start-your-own-game how-to (FetchContent v0.1.0). No interventions needed.
- 2026-06-10: round 1 — first game built inside the rpgkit clone; finding:
  no doc taught the standalone-project pattern (fixed by rpgkit#9).
