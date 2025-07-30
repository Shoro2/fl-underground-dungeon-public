#include "UndergroundInstance.h"
#include "ScriptMgr.h"
#include "ScriptedGossip.h"
#include "Player.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Chat.h"
#include <string>
#include <algorithm>
#include "UndergroundState.h"
#include "InstanceSaveMgr.h"
#include "Spell.h"

// Starter NPC: entry = 86000 (used for dungeon difficulty selection)

class npc_dungeon_starter : public CreatureScript
{
public:
    npc_dungeon_starter() : CreatureScript("npc_dungeon_starter") { }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        ClearGossipMenuFor(player);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Begin Challenge", GOSSIP_SENDER_MAIN, 42, "Enter a difficulty level (1–100):", 0, true);
        SendGossipMenuFor(player, 90099, creature->GetGUID());
        return true;
    }

    bool OnGossipSelectCode(Player* player, Creature* /*creature*/, uint32 /*sender*/, uint32 /*action*/, const char* code) override
    {
        CloseGossipMenuFor(player);

        if (player->GetGroup())
        {
            ChatHandler(player->GetSession()).SendSysMessage("You must not be in a group to enter the solo dungeon.");
            return false;
        }

        if (!code || !isNumeric(code))
        {
            ChatHandler(player->GetSession()).SendSysMessage("Invalid input. Please enter a number between 1 and 100.");
            return false;
        }

        int difficulty = atoi(code);
        if (difficulty < 1 || difficulty > 100)
        {
            ChatHandler(player->GetSession()).SendSysMessage("Difficulty must be between 1 and 100.");
            return false;
        }

        ChatHandler(player->GetSession()).PSendSysMessage("Difficulty set to: %u", difficulty);

        UndergroundState::SetDifficulty(player, difficulty);
        UndergroundState::SetStartTime(player, getMSTime());

        //player->SetPhaseMask(1, true);
        player->TeleportTo(33, -241.953995f, 2160.459961f, 78.504204f, 2.407250f);
        LOG_DEBUG("module", "Starting solo dungeon for player: '{}'", player->GetName());
        return true;
    }


private:
    static bool isNumeric(const char* str)
    {
        for (int i = 0; str[i] != '\0'; ++i)
        {
            if (!isdigit(str[i]))
                return false;
        }
        return true;
    }
};

class fl_underground_player_script : public PlayerScript
{
public:
    fl_underground_player_script() : PlayerScript("fl_underground_player_script") { }

    std::unordered_map<ObjectGuid, bool> wasGrouped;

    void OnMapChanged(Player* player) override
    {
        uint32 previousMap = UndergroundState::GetLastMap(player);

        // Ignore map change if run was completed
        if (UndergroundState::WasRunCompleted(player))
        {
            CompleteDungeonRun(player);
            return;
        }

        if (previousMap == 33 && player->GetMapId() != 33)
        {
            ChatHandler(player->GetSession()).SendSysMessage("You left the dungeon. Run aborted.");
            AbortDungeonRun(player);
        }

        UndergroundState::SetLastMap(player);
    }

    //void OnSpellCast(Player* player, Spell* spell, bool /*skipCheck*/) override
    //{
    //    // Only apply effect inside the solo dungeon
    //    if (player->GetMapId() != 33)
    //        return;
    //    const uint32 slowAuraId = 100028;
    //    if (spell->GetSpellInfo()->Id == 642) // Divine Shield
    //    {
    //        player->AddAura(slowAuraId, player); // Add slow aura
    //        ChatHandler(player->GetSession()).SendSysMessage("[Dungeon] You feel a strange weight slowing your movements...");
    //    }
    //    else if (spell->GetSpellInfo()->Id == 66) // Invisibility
    //    {
    //        player->AddAura(slowAuraId, player); // Add slow aura
    //        player->AddAura(100029, player); // Add -damage aura
    //        ChatHandler(player->GetSession()).SendSysMessage("[Dungeon] You feel a strange weight slowing your movements...");
    //    }
    //    else if (spell->GetSpellInfo()->Id == 58984) // Shadowmeld
    //    {
    //        player->AddAura(100029, player); // Add -damage aura
    //        ChatHandler(player->GetSession()).SendSysMessage("You feel strange...");
    //    }
    //    else if (spell->GetSpellInfo()->Id == 5384) // Feign Death
    //    {
    //        player->AddAura(100029, player); // Add -damage aura
    //        ChatHandler(player->GetSession()).SendSysMessage("You feel strange...");
    //    }
    //    else if (spell->GetSpellInfo()->Id == 2379) // Speed Pot
    //    {
    //        player->AddAura(100029, player); // Add -damage aura
    //        ChatHandler(player->GetSession()).SendSysMessage("You feel strange...");
    //    }
    //    else if (spell->GetSpellInfo()->Id == 3448) // Lesser Invisibility Potion
    //    {
    //        player->AddAura(slowAuraId, player); // Add slow aura
    //        ChatHandler(player->GetSession()).SendSysMessage("[Dungeon] You feel a strange weight slowing your movements...");
    //    }
    //    else if (spell->GetSpellInfo()->Id == 11464) // Invisibility Potion
    //    {
    //        player->AddAura(slowAuraId, player); // Add slow aura
    //        ChatHandler(player->GetSession()).SendSysMessage("[Dungeon] You feel a strange weight slowing your movements...");
    //    }
    //} rogue sprint, nitro boosters


    // todo: add counter and aura for creeps alive and unlock door when 50% of all creeps are dead
    //void OnCreatureKill(Player* killer, Creature* killed) override
    //{
    //    CalculateXPGain(killer, killed);
    //
    //}
    //
    //
    //void OnCreatureKilledByPet(Player* killer, Creature* killed) override
    //{
    //    CalculateXPGain(killer, killed);
    //}

    void OnPlayerJustDied(Player* player) override
    {
        HandlePlayerDeathInDungeon(player);
    }

    void HandlePlayerDeathInDungeon(Player* player)
    {
        if (player->GetMapId() != 33)
            return;
        ChatHandler(player->GetSession()).SendSysMessage("[Dungeon] You died. The run is over and you are being teleported out.");
        LOG_DEBUG("module", "Player death");
        

        // Spieler wiederbeleben (vor dem Teleport, damit keine Leiche im Dungeon bleibt)
        if (player->isDead())
        {
            player->ResurrectPlayer(1.0f, false);
            player->SetHealth(player->GetMaxHealth());
            player->SpawnCorpseBones();
            AbortDungeonRun(player);
        }
        
    }
    

    void OnUpdate(Player* player, uint32 /*diff*/) override
    {
        
        if (player->GetMapId() != 33)
        {
            wasGrouped[player->GetGUID()] = player->GetGroup() != nullptr;
            return;
        }

        bool currentlyGrouped = player->GetGroup() != nullptr;

        if (currentlyGrouped && !wasGrouped[player->GetGUID()])
        {
            ChatHandler(player->GetSession()).SendSysMessage("[Dungeon] You joined a group. Run aborted.");
            AbortDungeonRun(player);
        }

        wasGrouped[player->GetGUID()] = currentlyGrouped;
        
    }

    void OnLogout(Player* player) override
    {
        if (player->GetMapId() == 33)
        {
            ChatHandler(player->GetSession()).SendSysMessage("[Dungeon] You logged out inside the dungeon. Run aborted.");
            AbortDungeonRun(player);
        }

        wasGrouped.erase(player->GetGUID());
    }

private:
    void AbortDungeonRun(Player* player)
    {
        InstanceMap* instanceMap = player->GetMap()->ToInstanceMap();

        // Teleport player out
        player->TeleportTo(727, 13414.074219f, 12073.212891f, -46.700172f, 5.403475f);

        // Reset instance if possible
        if (instanceMap)
        {
            if (InstanceScript* instance = instanceMap->GetInstanceScript())
            {
                if (instance_underground* underground = dynamic_cast<instance_underground*>(instance))
                {
                    underground->ResetInstance(player);
                }
            }
        }

        // Clear player state
        UndergroundState::ClearDifficulty(player);
        UndergroundState::MarkRunAborted(player);
        UndergroundState::ClearLastMap(player);

        // Unbind from instance
        sInstanceSaveMgr->PlayerUnbindInstance(player->GetGUID(), 33, DUNGEON_DIFFICULTY_NORMAL, true, player);
    }

    void CompleteDungeonRun(Player* player)
    {
        // Teleport player out
        player->TeleportTo(727, 13414.074219f, 12073.212891f, -46.700172f, 5.403475f);

        // Reset instance if possible
        if (InstanceMap* instanceMap = player->GetMap()->ToInstanceMap())
        {
            if (InstanceScript* instance = instanceMap->GetInstanceScript())
            {
                if (instance_underground* underground = dynamic_cast<instance_underground*>(instance))
                {
                    underground->ResetInstance(player);
                }
            }
        }

        // Clear player state
        UndergroundState::ClearDifficulty(player);
        UndergroundState::MarkRunCompleted(player);
        UndergroundState::ClearLastMap(player);

        // Unbind from instance
        sInstanceSaveMgr->PlayerUnbindInstance(player->GetGUID(), 33, DUNGEON_DIFFICULTY_NORMAL, true, player);

        // Erfolgsmeldung
        ChatHandler(player->GetSession()).SendSysMessage("[Dungeon] Congratulations! You have completed the run.");
    }
};


void AddMyPlayerScripts()
{
    new npc_dungeon_starter();
    new fl_underground_player_script();
}
