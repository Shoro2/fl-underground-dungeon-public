#pragma once

#include "Player.h"
#include "Creature.h"
#include <map>
#include <mutex>

namespace UndergroundState
{
    // Map-based storage (fallback or legacy)
    static std::map<ObjectGuid, uint8> difficultyMap;
    static std::mutex stateMutex;

    inline void SetDifficulty(Player* player, uint8 diff)
    {
        std::lock_guard<std::mutex> lock(stateMutex);
        difficultyMap[player->GetGUID()] = diff;

        // Zusätzlich: direkt auf Spielerobjekt speichern (z.B. UNIT_FIELD_BYTES_2, byte 2)
        player->SetByteValue(UNIT_FIELD_BYTES_2, 2, diff);
    }


    inline uint8 GetDifficulty(Player* player)
    {
        std::lock_guard<std::mutex> lock(stateMutex);
        auto it = difficultyMap.find(player->GetGUID());
        if (it != difficultyMap.end())
            return it->second;

        // Fallback: vom Spielerobjekt lesen
        return player->GetByteValue(UNIT_FIELD_BYTES_2, 2);
    }


    inline void ClearDifficulty(Player* player)
    {
        std::lock_guard<std::mutex> lock(stateMutex);
        difficultyMap.erase(player->GetGUID());
    }

    // New: store difficulty directly on creature (reliable, avoids Player GUID issues)
    inline void StoreDifficultyInCreature(Creature* creature, uint8 diff)
    {
        creature->SetByteValue(UNIT_FIELD_BYTES_1, 2, diff);
    }

    inline uint8 ReadDifficultyFromCreature(Creature* creature)
    {
        /*
        if (uint8 diff = creature->GetByteValue(UNIT_FIELD_BYTES_1, 2))
            return diff;
        else
            return 0;
            */
        return creature->GetByteValue(UNIT_FIELD_BYTES_1, 2);
    }

    inline void MarkRunAborted(Player* player)
    {
        player->SetByteValue(UNIT_FIELD_BYTES_2, 1, 1);
    }

    inline bool WasRunAborted(Player* player)
    {
        return player->GetByteValue(UNIT_FIELD_BYTES_2, 1) == 1;
    }

    inline void ClearRunAborted(Player* player)
    {
        player->SetByteValue(UNIT_FIELD_BYTES_2, 1, 0);
    }

    static std::map<ObjectGuid, uint32> lastKnownMap;

    inline void SetLastMap(Player* player)
    {
        std::lock_guard<std::mutex> lock(stateMutex);
        lastKnownMap[player->GetGUID()] = player->GetMapId();
    }

    inline uint32 GetLastMap(Player* player)
    {
        std::lock_guard<std::mutex> lock(stateMutex);
        auto it = lastKnownMap.find(player->GetGUID());
        return (it != lastKnownMap.end()) ? it->second : 0;
    }

    inline void ClearLastMap(Player* player)
    {
        std::lock_guard<std::mutex> lock(stateMutex);
        lastKnownMap.erase(player->GetGUID());
    }

    inline void MarkRunCompleted(Player* player)
    {
        player->SetByteValue(UNIT_FIELD_BYTES_2, 1, 2);
    }

    inline bool WasRunCompleted(Player* player)
    {
        return player->GetByteValue(UNIT_FIELD_BYTES_2, 1) == 2;
    }

    inline void ClearRunCompleted(Player* player)
    {
        if (WasRunCompleted(player))
            player->SetByteValue(UNIT_FIELD_BYTES_2, 1, 0);
    }

    static std::map<ObjectGuid, uint32> startTimeMap;

    inline void SetStartTime(Player* player, uint32 time)
    {
        std::lock_guard<std::mutex> lock(stateMutex);
        startTimeMap[player->GetGUID()] = time;
    }

    inline uint32 GetStartTime(Player* player)
    {
        std::lock_guard<std::mutex> lock(stateMutex);
        auto it = startTimeMap.find(player->GetGUID());
        return (it != startTimeMap.end()) ? it->second : 0;
    }

    inline void ClearStartTime(Player* player)
    {
        std::lock_guard<std::mutex> lock(stateMutex);
        startTimeMap.erase(player->GetGUID());
    }

}
