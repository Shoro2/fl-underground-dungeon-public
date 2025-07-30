#include "UndergroundInstance.h"
#include "Chat.h"
#include "Player.h"
#include "UndergroundState.h"
#include "UndergroundUtils.h"
#include "ScriptMgr.h"

instance_underground::instance_underground(InstanceMap* map)
    : InstanceScript(map), instance(map), mobsSpawned(false) { }

void instance_underground::OnPlayerEnter(Player* player)
{
    //player->SetPhaseMask(1, true);

    uint8 difficulty = UndergroundState::GetDifficulty(player);
    if (difficulty == 0)
    {
        ChatHandler(player->GetSession()).SendSysMessage("[Dungeon] No difficulty set. No creatures will be spawned.");
        return;
    }

    if (!mobsSpawned)
    {
        mobsSpawned = true;

        // Liste von Entry-IDs (nur Beispiel, kann erweitert werden)
        std::vector<uint32> meleeEntries = {
            84284, // Underground Ghoul OK
            84286, // Disgusting Larva OK
            84287, // Awakened Bones OK
            84264, // Twisted Abomination ?!?
            84266, // Crypt Howler ?!?
            84270, // Grotesque Brute OK
            84265, // Rotting Hound OK
            84269, // Hellpit Crawler OK
            84268, // Bloodspike Beast OK
            84273, // Blightfang OK
            84275, // Fleshbound Horror OK
            84274, // Abyss Hound OK
            84276, // Putrid Fleshbeast OK
        };


        std::vector<uint32> casterEntries = {
            84285, // Underground Spectrum OK
            84263, // Underground Occultist OK
            84267, // Gloomfang
            84271, // Warped Bonefiend
            84272, // Ashen Wailer
            84277, // Dread Maggot 84266
            84278, // Voidbound Revenant
            84279, // Soul-Leech Banshee
            84280, // Tormented Soul
            84281, // Shadowbone Stalker
            84282, // Carrion Watcher
            84283, // Netherclaw Demon
        };

        std::vector<uint32> BossEntries = {
            84288,
            84289,
            84290,
        };


        struct SpawnPoint { Position pos; uint32 count; };
        std::vector<SpawnPoint> spawnPoints = {
            {{-241.30f, 2186.07f, 71.27f, 4.79f}, 4}, //start   6   2   6   2   2   4   6
            {{-243.04f, 2224.74f, 69.88f, 4.79f}, 2}, //2nd
            {{-284.79f, 2210.76f, 69.85f, 4.79f}, 4}, //3nd
            //{{-320.74f, 2211.75f, 71.28f, 4.79f}, 2}, //smoke
            {{-305.59f, 2246.41f, 75.82f, 4.79f}, 2}, //stairs
            {{-280.00f, 2264.66f, 78.57f, 4.79f}, 2}, //pre boss 1
            {{-259.03f, 2261.32f, 78.55f, 4.79f}, 4}, //pre boss 2


        };

        for (auto const& point : spawnPoints)
        {
            for (uint32 i = 0; i < point.count / 2; ++i)
            {
                uint32 meleeEntry = meleeEntries[urand(0, meleeEntries.size() - 1)];
                if (Creature* creature = player->SummonCreature(meleeEntry, point.pos, TEMPSUMMON_MANUAL_DESPAWN))
                {
                    creature->SetPhaseMask(1, true);
                    creature->SetDefaultMovementType(RANDOM_MOTION_TYPE);
                    creature->GetMotionMaster()->MoveRandom(5.0f);
                    creature->AI()->JustRespawned();
                    UndergroundState::StoreDifficultyInCreature(creature, difficulty);
                    spawnedCreatures.push_back(creature->GetGUID());
                    //ChatHandler(player->GetSession()).PSendSysMessage("[Debug] Spawned '%s' (Entry: %u) at [%.2f, %.2f, %.2f] with Difficulty %u",
                    //    creature->GetName().c_str(),
                    //    creature->GetEntry(),
                    //    creature->GetPositionX(),
                    //    creature->GetPositionY(),
                    //    creature->GetPositionZ(),
                    //    difficulty);
                    LOG_DEBUG("module", "Spawned '{}' (Entry: {}) at [{:.2f}, {:.2f}, {:.2f}], Difficulty: {}",
                        creature->GetName(),
                        creature->GetEntry(),
                        creature->GetPositionX(),
                        creature->GetPositionY(),
                        creature->GetPositionZ(),
                        difficulty);


                }
                
                uint32 casterEntry = casterEntries[urand(0, casterEntries.size() - 1)];
                if (Creature* creature = player->SummonCreature(casterEntry, point.pos, TEMPSUMMON_MANUAL_DESPAWN))
                {
                    creature->SetPhaseMask(1, true);
                    creature->SetDefaultMovementType(RANDOM_MOTION_TYPE);
                    creature->GetMotionMaster()->MoveRandom(5.0f);
                    creature->AI()->JustRespawned();
                    UndergroundState::StoreDifficultyInCreature(creature, difficulty);
                    spawnedCreatures.push_back(creature->GetGUID());
                    //ChatHandler(player->GetSession()).PSendSysMessage("[Debug] Spawned '%s' (Entry: %u) at [%.2f, %.2f, %.2f] with Difficulty %u",
                    //    creature->GetName().c_str(),
                    //    creature->GetEntry(),
                    //    creature->GetPositionX(),
                    //    creature->GetPositionY(),
                    //    creature->GetPositionZ(),
                    //    difficulty);
                    LOG_DEBUG("module", "Spawned '{}' (Entry: {}) at [{:.2f}, {:.2f}, {:.2f}], Difficulty: {}",
                        creature->GetName(),
                        creature->GetEntry(),
                        creature->GetPositionX(),
                        creature->GetPositionY(),
                        creature->GetPositionZ(),
                        difficulty);

                }
                

            }
            LOG_DEBUG("module", "Done Spawning Creatures");
        }
        
        uint32 BossEntry = BossEntries[urand(0, BossEntries.size() - 1)];
        // Boss
        if (Creature* boss = player->SummonCreature(BossEntry, { -262.19f, 2313.88f, 78.61f, 3.00f }, TEMPSUMMON_MANUAL_DESPAWN))
        {
            LOG_DEBUG("module", "Spawned '{}' (Entry: {}) at [{:.2f}, {:.2f}, {:.2f}], Difficulty: {}",
                boss->GetName(),
                boss->GetEntry(),
                boss->GetPositionX(),
                boss->GetPositionY(),
                boss->GetPositionZ(),
                difficulty);
            boss->SetPhaseMask(1, true);
            boss->AI()->JustRespawned();
            //boss->AddAura(100027, boss); // Add trash aura
            //UndergroundState::StoreDifficultyInCreature(boss, difficulty);
            spawnedCreatures.push_back(boss->GetGUID());
            LOG_DEBUG("module", "Done Spawning Boss");
        }
        
    }
}


uint8 instance_underground::GetPlayerDifficulty(ObjectGuid guid)
{
    if (Player* player = ObjectAccessor::FindConnectedPlayer(guid))
        return UndergroundState::GetDifficulty(player);
    return 0;
}

void instance_underground::ResetInstance(Player* player)
{
    mobsSpawned = false;

    for (ObjectGuid guid : spawnedCreatures)
    {
        if (Creature* creature = instance->GetCreature(guid))
        {
            creature->DespawnOrUnsummon();
        }
    }

    spawnedCreatures.clear();

    ChatHandler(player->GetSession()).SendSysMessage("Solo underground instance reset.");
}

class instance_underground_instance : public InstanceMapScript
{
public:
    instance_underground_instance() : InstanceMapScript("instance_underground", 33) { }

    InstanceScript* GetInstanceScript(InstanceMap* map) const override
    {
        return new instance_underground(map);
    }
};

void AddSC_instance_underground()
{
    new instance_underground_instance();
}
