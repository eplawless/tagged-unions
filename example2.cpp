#include <iostream>
#include <string>
#include <sstream>
#include "tagged-unions.h"

TAGGED_STRUCT(CharacterStats,
    (Rogue, {
        int Level;
        int BackstabCount;
    }),
    (Warrior, {
        int Level;
        std::string CoolNickname;
    })
)

int GetLevel(const FCharacterStats& Character) {
    auto GenericGetLevel = [](const auto& Stats) { return Stats.Level; };
    return Match<int>(Character, GenericGetLevel, GenericGetLevel);
}

void PrintGreeting(const FCharacterStats& Character) {
    std::cout << Match<std::string>(Character,
        +[](const FCharacterStats_Rogue& Rogue) -> std::string {
            return "Good job being a rogue!";
        },
        [](const FCharacterStats_Warrior& Warrior) -> std::string {
            std::stringstream ss;
            ss << "Good job being a warrior, " << Warrior.CoolNickname << "!\n";
            return ss.str();
        }) << "\n";
}

int main(int argc, char **argv) {
    FCharacterStats Character1 = FCharacterStats_Rogue {
        .Level = 10,
        .BackstabCount = 9999
    };
    FCharacterStats Character2 = FCharacterStats_Warrior {
        .Level = 20,
        .CoolNickname = "Dave"
    };
    
    std::cout << "You are levels " << GetLevel(Character1)
        << " and " << GetLevel(Character2) << "\n";
    PrintGreeting(Character1);
    PrintGreeting(Character2);
    return 0;
}
