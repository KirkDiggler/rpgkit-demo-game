// demo_game — minimal playable combat demonstrating Chain<T> damage resolution
//
// Build & run (from the repo root):
//   make run
//   (or: ./build/demo_game after make build)
//
// What this shows:
//   1. Chain<int> models attacks with staged modifiers
//   2. Base damage -> effects -> final adjustments
//   3. Execute returns breakdown showing each modifier's contribution
//   4. Real gameplay loop with turn-based combat

#include <cstdlib>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <vector>

#include "rpg/core/chain.hpp"
#include "rpg/core/status.hpp"

namespace {

// ============================================================================
// Tuning constants
// ============================================================================
constexpr int kHeroMaxHp = 20;
constexpr int kGoblinMaxHp = 12;
constexpr int kHeroStrikeDamage = 6;
constexpr int kGoblinToughness = 2;
constexpr int kGoblinBiteDamage = 4;

// ============================================================================
// Game types
// ============================================================================
struct Fighter {
  std::string name;
  int hp;
};

// ============================================================================
// Error handling - example-grade: stop loudly on any failure
// ============================================================================
void mustBeOk(const rpg::core::Status& status) {
  if (!status.isOk()) {
    std::cerr << "error: " << status.message() << '\n';
    std::exit(EXIT_FAILURE);
  }
}

// ============================================================================
// Input validation
// ============================================================================
int readChoice() {
  std::cout << "\nYour turn! (1) Attack: ";
  int choice;
  while (true) {
    if (std::cin >> choice) {
      if (choice == 1) {
        return choice;
      }
      std::cout << "Invalid choice. Enter 1 to attack: ";
    } else {
      // Clear error state and ignore bad input
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "Invalid input. Enter 1 to attack: ";
    }
  }
}

// ============================================================================
// Combat resolution
// ============================================================================
void resolveDamage(Fighter& attacker, Fighter& defender,
                   rpg::core::Chain<int>& attackChain) {
  std::cout << "\n" << attacker.name << " attacks!\n";
  
  // Execute the chain starting from 0 (modifiers add damage)
  const rpg::core::Chain<int>::Result result = attackChain.execute(0);
  
  // Show the breakdown - every modifier's contribution
  for (const rpg::core::Chain<int>::Step& step : result.breakdown) {
    std::cout << "  " << step.id << " (" << step.stage << "): " 
              << step.before << " -> " << step.after << '\n';
  }
  
  std::cout << "Total damage: " << result.value << '\n';
  
  // Apply damage
  defender.hp -= result.value;
  if (defender.hp < 0) {
    defender.hp = 0;
  }
  
  std::cout << defender.name << " HP: " << defender.hp << '\n';
}

// ============================================================================
// Attack chains
// ============================================================================
rpg::core::Chain<int> buildHeroAttack() {
  // Hero strike: base damage, reduced by goblin toughness
  rpg::core::Chain<int> chain(std::vector<std::string>{"base", "effects", "final"});
  
  // Strike: base weapon damage
  mustBeOk(chain.add("base", "strike", [](int dmg) { 
    return dmg + kHeroStrikeDamage; 
  }));
  
  // Goblin's natural armor reduces damage
  mustBeOk(chain.add("final", "goblin_tough", [](int dmg) { 
    return dmg - kGoblinToughness; 
  }));
  
  return chain;
}

rpg::core::Chain<int> buildGoblinAttack() {
  // Goblin bite: base damage with random variance
  rpg::core::Chain<int> chain(std::vector<std::string>{"base", "effects", "final"});
  
  // Bite: base attack damage
  mustBeOk(chain.add("base", "bite", [](int dmg) { 
    return dmg + kGoblinBiteDamage; 
  }));
  
  // Erratic bite: +/- 1 variance
  static std::mt19937 rng{std::random_device{}()};
  mustBeOk(chain.add("effects", "variance", [](int dmg) {
    std::uniform_int_distribution<int> variance{-1, 1};
    return dmg + variance(rng);
  }));
  
  return chain;
}

}  // namespace

// ============================================================================
// Main game loop
// ============================================================================
int main() {
  std::cout << "=== RPGKit Demo: Terminal Combat ===\n";
  std::cout << "Hero vs Goblin - fight to the finish!\n\n";
  
  // Initialize fighters
  Fighter hero{"Hero", kHeroMaxHp};
  Fighter goblin{"Goblin", kGoblinMaxHp};
  
  std::cout << hero.name << " HP: " << hero.hp << '\n';
  std::cout << goblin.name << " HP: " << goblin.hp << '\n';
  
  // Build attack chains once - they contain functions, not values
  rpg::core::Chain<int> heroAttack = buildHeroAttack();
  rpg::core::Chain<int> goblinAttack = buildGoblinAttack();
  
  // Combat loop
  while (hero.hp > 0 && goblin.hp > 0) {
    // Player turn
    readChoice();  // Currently only "1" for attack
    resolveDamage(hero, goblin, heroAttack);
    
    if (goblin.hp <= 0) {
      break;
    }
    
    // Goblin turn
    std::cout << "\n--- Goblin's turn ---\n";
    resolveDamage(goblin, hero, goblinAttack);
  }
  
  // Victory message
  std::cout << "\n=== COMBAT END ===\n";
  if (hero.hp > 0) {
    std::cout << "Victory! " << hero.name << " survives with " << hero.hp << " HP.\n";
  } else {
    std::cout << "Defeat! " << goblin.name << " wins!\n";
  }
  
  return 0;
}
