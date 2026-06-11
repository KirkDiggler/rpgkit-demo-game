// Tutorial 03 — Cards & Energy: deck-builder skeleton with healing chains

// This is the playable result of docs/tutorials/03-cards-and-energy.md.
// A dealt hand of cards, an energy budget, and healing that caps at max HP.

// Build & run (from the repo root):
// make build
// ./build/rpgkit_demo_game

// The big idea from tutorial 02 still holds: damage (and now healing) is
// never just subtracted or added. Every change travels through a Chain —
// a pipeline of rules — and comes out the other side with a receipt showing
// what each rule did.

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <vector>
#include "rpg/core/chain.hpp"
#include "rpg/core/status.hpp"

namespace {

// The tuning table: every game number lives here, named, in one place.
constexpr int kHeroMaxHp = 20;
constexpr int kGoblinHp = 30;
constexpr int kClawsDamage = 6;
constexpr int kEnergyPerTurn = 3;
constexpr int kMaxEnergy = 5;
constexpr int kHandSize = 4;
constexpr int kQuit = -1;

struct Fighter {
  std::string name;
  int hp = 0;
  int maxHp = 0;
};

struct Card {
  std::string name;
  int cost = 0;
  int damage = 0;
  int heal = 0;
};

enum class PhaseResult {
  kTurnOver,
  kGoblinDown,
  kQuit
};

std::vector<Card> cardPool() {
  return {
      {.name = "Strike", .cost = 1, .damage = 6, .heal = 0},
      {.name = "Bash", .cost = 2, .damage = 11, .heal = 0},
      {.name = "Salve", .cost = 1, .damage = 0, .heal = 4},
      {.name = "Smite", .cost = 3, .damage = 16, .heal = 0},
      {.name = "Vampiric Strike", .cost = 2, .damage = 5, .heal = 3},
  };
}

void mustBeOk(const rpg::core::Status& status) {
  if (!status.isOk()) {
    std::cerr << "error: " << status.message() << '\n';
    std::exit(EXIT_FAILURE);
  }
}

int readChoice() {
  int choice = 0;
  while (!(std::cin >> choice)) {
    if (std::cin.eof()) return kQuit;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << " numbers only — try again: ";
  }
  return choice;
}

int resolveDamage(const std::string& attackName, int baseDamage,
                  const rpg::core::Chain<int>& chain) {
  const rpg::core::Chain<int>::Result result = chain.execute(baseDamage);
  std::cout << " " << attackName << ": " << baseDamage << " damage\n";
  for (const rpg::core::Chain<int>::Step& step : result.breakdown) {
    std::cout << "  " << step.id << " (" << step.stage << "): "
              << step.before << " -> " << step.after << '\n';
  }
  return result.value;
}

int resolveHealing(const std::string& healName, int baseHeal,
                   const rpg::core::Chain<int>& chain) {
  const rpg::core::Chain<int>::Result result = chain.execute(baseHeal);
  std::cout << " " << healName << ": " << baseHeal << " healing\n";
  for (const rpg::core::Chain<int>::Step& step : result.breakdown) {
    std::cout << "  " << step.id << " (" << step.stage << "): "
              << step.before << " -> " << step.after << '\n';
  }
  return result.value;
}

std::vector<Card> dealHand(std::mt19937& rng) {
  const std::vector<Card> pool = cardPool();
  std::uniform_int_distribution<std::size_t> pick{0, pool.size() - 1};
  std::vector<Card> hand;
  hand.reserve(kHandSize);
  for (int i = 0; i < kHandSize; ++i) {
    hand.push_back(pool.at(pick(rng)));
  }
  return hand;
}

void showHand(const std::vector<Card>& hand, const std::vector<bool>& played) {
  for (std::size_t i = 0; i < hand.size(); ++i) {
    if (played[i]) continue;
    const Card& card = hand[i];
    std::cout << "[" << (i + 1) << "] " << card.name
              << " (cost:" << card.cost;
    if (card.damage > 0) std::cout << " dmg:" << card.damage;
    if (card.heal > 0) std::cout << " heal:" << card.heal;
    std::cout << ") ";
  }
  std::cout << "[0] End turn\n> ";
}

int playDamageCard(const Card& card, Fighter& target) {
  rpg::core::Chain<int> damage(std::vector<std::string>{"base", "final"});
  mustBeOk(damage.add("final", "tough-skin", [](int dmg) { return dmg - 1; }));
  return resolveDamage(card.name, card.damage, damage);
}

int playHealCard(const Card& card, Fighter& target) {
  rpg::core::Chain<int> heal(std::vector<std::string>{"base", "cap"});
  const int room = std::max(0, target.maxHp - target.hp);
  mustBeOk(heal.add("cap", "max-hp-cap", [room](int amount) {
    return amount > room ? room : amount;
  }));
  return resolveHealing(card.name, card.heal, heal);
}

PhaseResult heroPhase(Fighter& hero, Fighter& goblin, std::mt19937& rng, int& energy) {
  const std::vector<Card> hand = dealHand(rng);
  std::vector<bool> played(hand.size(), false);
  energy = std::min(energy + kEnergyPerTurn, kMaxEnergy);

  while (energy > 0) {
    std::cout << "\nEnergy: " << energy << " | ";
    showHand(hand, played);

    const int choice = readChoice();

    if (choice == kQuit) {
      return PhaseResult::kQuit;
    }

    if (choice == 0) {
      std::cout << "Turn over.\n";
      return PhaseResult::kTurnOver;
    }

    const std::size_t idx = static_cast<std::size_t>(choice - 1);
    if (idx >= hand.size()) {
      std::cout << " no such card.\n";
      continue;
    }

    if (played[idx]) {
      std::cout << " already played that card.\n";
      continue;
    }

    const Card& card = hand[idx];
    if (card.cost > energy) {
      std::cout << " not enough energy (need " << card.cost << ").\n";
      continue;
    }

    played[idx] = true;
    energy -= card.cost;

    if (card.damage > 0) {
      goblin.hp -= playDamageCard(card, goblin);
      if (goblin.hp <= 0) {
        return PhaseResult::kGoblinDown;
      }
    }

    if (card.heal > 0) {
      hero.hp += playHealCard(card, hero);
    }
  }

  std::cout << "Out of energy. Turn over.\n";
  return PhaseResult::kTurnOver;
}

int goblinClaws() {
  const rpg::core::Chain<int> damage(std::vector<std::string>{"base", "final"});
  return resolveDamage("Claws", kClawsDamage, damage);
}

} // namespace

int main() {
  std::random_device rd;
  std::mt19937 rng{rd()};

  Fighter hero{.name = "Hero", .hp = kHeroMaxHp, .maxHp = kHeroMaxHp};
  Fighter goblin{.name = "Goblin", .hp = kGoblinHp, .maxHp = kGoblinHp};
  int energy = 0; // starts at zero, gets topped up each turn

  std::cout << "A goblin blocks your path!\n";

  while (true) {
    std::cout << '\n'
              << hero.name << ": " << hero.hp << "/" << hero.maxHp << " HP  "
              << goblin.name << ": " << goblin.hp << " HP\n";

    const PhaseResult result = heroPhase(hero, goblin, rng, energy);

    if (result == PhaseResult::kQuit) {
      std::cout << "\nYou live to fight another day. The goblin cackles.\n";
      return 0;
    }

    if (result == PhaseResult::kGoblinDown) {
      std::cout << "\nThe goblin falls. Victory!\n";
      return 0;
    }

    hero.hp -= goblinClaws();
    if (hero.hp <= 0) {
      std::cout << "\nYou crumple. The goblin wins this time...\n";
      return 0;
    }
  }
}
