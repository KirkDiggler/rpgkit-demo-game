// Tutorial 02 — your first terminal game: a hero, a goblin, and a fight.

// This is the playable result of docs/tutorials/02-your-first-terminal-game.md,
// and the base every later tutorial builds on (cards, energy, block...).

// Build & run (from the repo root):
// make build
// ./build/rpgkit_demo_game

// The one big idea: damage is never just subtracted. Every hit travels
// through a Chain — a little pipeline of rules — and comes out the other
// side with a receipt showing what each rule did. Right now the goblin has
// one rule (tough skin). Later tutorials add more rules WITHOUT touching
// the combat math that's already here.

#include <cstdlib>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include "rpg/core/chain.hpp"
#include "rpg/core/status.hpp"

namespace {

// The tuning table: every game number lives here, named, in one place.
constexpr int kHeroHp      = 20;
constexpr int kGoblinHp    = 18;
constexpr int kStrikeDamage = 6;
constexpr int kClawsDamage = 4;

struct Fighter {
  std::string name;
  int hp = 0;
};

void mustBeOk(const rpg::core::Status& status) {
  if (!status.isOk()) {
    std::cerr << "error: " << status.message() << '\n';
    std::exit(EXIT_FAILURE);
  }
}

int readChoice() {
  int choice = 0;
  while (!(std::cin >> choice)) {
    if (std::cin.eof()) return 0;
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

int heroStrikes() {
  rpg::core::Chain<int> damage(std::vector<std::string>{"base", "final"});
  mustBeOk(damage.add("final", "tough-skin", [](int dmg) { return dmg - 1; }));
  return resolveDamage("Strike", kStrikeDamage, damage);
}

int goblinClaws() {
  const rpg::core::Chain<int> damage(std::vector<std::string>{"base", "final"});
  return resolveDamage("Claws", kClawsDamage, damage);
}

} // namespace

int main() {
  Fighter hero{.name = "Hero", .hp = kHeroHp};
  Fighter goblin{.name = "Goblin", .hp = kGoblinHp};

  std::cout << "A goblin blocks your path!\n";

  while (true) {
    std::cout << '\n'
              << hero.name << ": " << hero.hp << " HP "
              << goblin.name << ": " << goblin.hp << " HP\n";
    std::cout << "[1] Strike [0] Run away\n> ";

    const int choice = readChoice();

    if (choice == 0) {
      std::cout << "\nYou live to fight another day. The goblin cackles.\n";
      return 0;
    }
    if (choice != 1) {
      std::cout << " no such move.\n";
      continue;
    }

    goblin.hp -= heroStrikes();
    if (goblin.hp <= 0) {
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
