#include <iostream>
#include <string>
#include <string_view>
#include <array>
#include <regex>
#include <cstring>
#include <math.h>

#define HONEY_TREE_MAX (21)

using namespace std;

bool sanitizeYesNoInput(const string &output) {
    string yesNoAnswer;
    regex yesNoRegex("^[nyNY]$");

    while ((cout << output) && (!(cin >> yesNoAnswer) || !regex_match(yesNoAnswer, yesNoRegex))) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    return toupper(yesNoAnswer[0]) == 'Y';
}

template <typename T>
void sanitizeInput(const string &output, T &index, T lowLimit, T highLimit) {
    while ((cout << output) && (!(cin >> index) || (index < lowLimit || index > highLimit))) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

const uint32_t getIDsInput(const bool sidFlag) {
    uint16_t tid, sid = 0;
    sanitizeInput<uint16_t>("Insert your TID: ", tid, 0, 65535);

    if (sidFlag) {
        sanitizeInput<uint16_t>("Insert your SID: ", sid, 0, 65535);
    }

    return (sid << 16) + tid;
}

void printMunchlaxTreeLocations() {
    if (!sanitizeYesNoInput("Would you like to show your Munchlax tree locations? (y/n) ")) {
        return;
    }

    static constexpr array<string_view, HONEY_TREE_MAX> honeyTreeLocationNames{ "Route 205 (South)", "Route 205 (North)", "Route 206", "Route 207",
                                                                                "Route 208", "Route 209", "Route 210 (South)", "Route 210 (North)",
                                                                                "Route 211", "Route 212 (North)", "Route 212 (South)", "Route 213",
                                                                                "Route 214", "Route 215", "Route 218", "Route 221", "Route 222",
                                                                                "Valley Windworks", "Eterna Forest", "Fuego Ironworks", "Floaroma Meadow" };

    const bool knownSID = sanitizeYesNoInput("Do you know your SID? (y/n) ");
    const short treeLimitNumber = knownSID ? 4 : 2;
    const uint32_t ids = getIDsInput(knownSID);
    static array<uint8_t, 4> treeLocationIndexes{};

    for (short i = treeLimitNumber, j = 0; i >= 0; i--, j += 8) {
        treeLocationIndexes[i] = ((ids >> j) & 0xFF) % HONEY_TREE_MAX;
    }

    cout << "\n\nMunchlax tree locations:\n\n";

    for (short i = 1; i < treeLimitNumber; i++) {
        for (short j = 0; j < i; j++) {
            if (treeLocationIndexes[j] == treeLocationIndexes[i]) {
                treeLocationIndexes[i]++;

                if (treeLocationIndexes[i] >= HONEY_TREE_MAX) {
                    treeLocationIndexes[i] = 0;
                }
            }
        }

        cout << honeyTreeLocationNames[treeLocationIndexes[i - 1]] << "\n";
    }

    cout << honeyTreeLocationNames[treeLocationIndexes[treeLimitNumber - 1]] << "\n\n\n";
}

bool sanitizeGameVersionInput() {
    string gameVersion;
    regex gameVersionRegex("^(dp|DP|Dp|dP|pt|Pt|PT)$");

    while ((cout << "Game version? (dp/pt) ") && (!(cin >> gameVersion) || !regex_match(gameVersion, gameVersionRegex))) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    transform(gameVersion.begin(), gameVersion.end(), gameVersion.begin(), ::toupper);

    return gameVersion == "PT";
}

void printTableSlotIndexes(bool munchlaxFlag) {
    const bool ptFlag = sanitizeGameVersionInput();
    static constexpr array<string_view, 6> pokemonDPNames0{ "Wurmple (40%)", "Silcoon (D) / Cascoon (P) (20%)", "Combee (20%)", "Burmy (10%)", "Cherubi (5%)", "Aipom (5%)" };
    static constexpr array<string_view, 6> pokemonDPNames1{ "Combee (40%)", "Burmy (20%)", "Cherubi (20%)", "Aipom (10%)", "Heracross (5%)", "Wurmple (5%)" };
    static constexpr array<string_view, 6> pokemonPtNames0{ "Combee (40%)", "Wurmple (20%)", "Burmy (20%)", "Cherubi (10%)", "Aipom (5%)", "Aipom (5%)" };
    static constexpr array<string_view, 6> pokemonPtNames1{ "Burmy (40%)", "Cherubi (20%)", "Combee (20%)", "Aipom (10%)", "Aipom (5%)", "Heracross (5%)" };

    static constexpr array<const string_view*, 4> pokemonNameTables{ pokemonDPNames0.data() , pokemonDPNames1.data(), pokemonPtNames0.data() , pokemonPtNames1.data() };

    cout << "\n\nTable Slots\n\n";

    for (short i = 0; i < 2; i++) {
        cout << "Table " << i + 1 << (i == 0 ? " (70%)" : " (20%)") << ":\n";

        for (size_t j = 0; j < 6; j++) {
            cout << j + 1 << " " << pokemonNameTables[(munchlaxFlag ? 1 - i : i) + (2 * ptFlag)][j] << "\n";
        }

        cout << "\n";
    }

    if (munchlaxFlag) {
        cout << "Table 3 (1%): \nMunchlax (100%)\n\n";
    }

    cout << "\n";
}

void getTreeTableSlotInput(short &tableIndex, short &slotIndex, const bool munchlaxTreeFlag) {
    printTableSlotIndexes(munchlaxTreeFlag);
    sanitizeInput<short>("Insert the wanted table index: ", tableIndex, 1, munchlaxTreeFlag ? 3 : 2);

    if (tableIndex != 3) {
        sanitizeInput<short>("Insert the wanted encounter slot: ", slotIndex, 1, 6);
    }
}

void sanitizeHexInput(uint32_t &seed) {
    string stringSeed;
    regex hexRegex("^[0-9a-fA-F]{1,8}$");

    while ((cout << "Insert the initial seed: ") && (!(cin >> stringSeed) || !regex_match(stringSeed, hexRegex))) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    seed = stoul(stringSeed, nullptr, 16);
}

void getRNGInput(uint32_t &seed, unsigned long &advances, bool &researchFlag) {
    researchFlag = sanitizeYesNoInput("Do you know the initial seed? (y/n) ");

    if (researchFlag) {
        sanitizeHexInput(seed);
        sanitizeInput<unsigned long>("Insert the current advances: ", advances, 0, ULONG_MAX);
    }
}

uint32_t LCRNG(uint32_t seed) {
    return 0x41C64E6D * seed + 0x6073;
}

void advanceRNG(uint32_t &seed, unsigned long n = 1) {
    for (unsigned long i = 0; i < n; i++) {
        seed = LCRNG(seed);
    }
}

uint16_t getHighSeed(uint32_t seed) {
    return seed >> 16;
}

bool isWantedTableCheck(uint32_t seed, short index, const bool munchlax) {
    static constexpr array<short, 5> tableThresholds{ 0, 1, 10, 30, 100 };
    short tableRangeIndex = index == 3 ? 1 : 5 - index;

    return trunc(getHighSeed(seed) / 656) >= tableThresholds[tableRangeIndex - 1] && trunc(getHighSeed(seed) / 656) < tableThresholds[tableRangeIndex];
}

bool isWantedEncounterSlotCheck(uint32_t seed, short table, short index) {
    static constexpr array<short, 7> encounterSlotThresholds{ 100, 60, 40, 20, 10, 5, 0 };

    return table == 3 || (trunc(getHighSeed(seed) / 656) >= encounterSlotThresholds[index] && trunc(getHighSeed(seed) / 656) < encounterSlotThresholds[index - 1]);
}

void findTreePokemon(short tableIndex, short slotIndex, const bool munchlaxTreeFlag, uint32_t seed, unsigned long advances) {
    for (;; advances++) {
        uint32_t tempSeed = LCRNG(seed);

        if (!isWantedTableCheck(tempSeed, tableIndex, munchlaxTreeFlag)) {
            advanceRNG(seed);
            continue;
        }

        tempSeed = LCRNG(tempSeed);

        if (isWantedEncounterSlotCheck(tempSeed, tableIndex, slotIndex)) {
            printf("\n\nTarget seed: %08X | Target advances: %lu\n\n------------------------------------------------\n\n", seed, advances);
            return;
        }

        advanceRNG(seed);
    }
}

void findTreeSeed(short tableIndex, short slotIndex, const bool munchlaxTreeFlag) {
    const short hour = 24, maxDelay = 10000;
    short minDelay;
    unsigned long maxAdvances;

    sanitizeInput<short>("Insert the min delay: ", minDelay, 600, 9999);
    sanitizeInput<unsigned long>("Insert the max advances: ", maxAdvances, 1, ULONG_MAX);

    for (short ab = 0; ab < 256; ab++) {
        for (short cd = 0; cd < hour; cd++) {
            for (short efgh = minDelay; efgh < maxDelay; efgh++) {
                uint32_t seed = ((ab << 24) | (cd << 16)) + efgh;
                uint32_t tempSeed = seed;

                for (unsigned long advances = 0; advances < maxAdvances; advances++) {
                    uint32_t tempSeed2 = LCRNG(tempSeed);

                    if (!isWantedTableCheck(tempSeed2, tableIndex, munchlaxTreeFlag)) {
                        advanceRNG(tempSeed);
                        continue;
                    }

                    tempSeed2 = LCRNG(tempSeed2);

                    if (isWantedEncounterSlotCheck(tempSeed2, tableIndex, slotIndex)) {
                        printf("\n\nTarget seed: %08X | Target advances: %lu\n\n------------------------------------------------\n\n", seed, advances);
                        return;
                    }

                    advanceRNG(tempSeed);
                }
            }
        }
    }
}

int main() {
    uint32_t initialSeed;
    unsigned long currentAdvances;
    short tableIndex, slotIndex;
    bool seedResearchFlag;

    while (true) {
        printMunchlaxTreeLocations();

        bool isMunchlaxTreeFlag = sanitizeYesNoInput("Is it a Munchlax Tree? (y/n) ");
        getTreeTableSlotInput(tableIndex, slotIndex, isMunchlaxTreeFlag);
        getRNGInput(initialSeed, currentAdvances, seedResearchFlag);

        if (seedResearchFlag) {
            advanceRNG(initialSeed, currentAdvances);
            findTreePokemon(tableIndex, slotIndex, isMunchlaxTreeFlag, initialSeed, currentAdvances);
            continue;
        }

        findTreeSeed(tableIndex, slotIndex, isMunchlaxTreeFlag);
    }
}