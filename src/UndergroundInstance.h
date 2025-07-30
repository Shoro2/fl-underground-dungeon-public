#pragma once

#include "InstanceScript.h"
#include "Map.h"
#include "Player.h"

class instance_underground : public InstanceScript
{
public:
    instance_underground(InstanceMap* map);

    void OnPlayerEnter(Player* player) override;
    void ResetInstance(Player* player);
    uint8 GetPlayerDifficulty(ObjectGuid guid);

    InstanceMap* instance = nullptr; // zuerst: Zeiger auf Instanz
    bool mobsSpawned = false;        // dann: Status-Flag
    std::vector<ObjectGuid> spawnedCreatures; // zuletzt: Liste

    uint32 lastTimerUpdate = 0;

};
