/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

// From SC
void AddSC_underground_player();
void AddSC_underground_creature();
void AddSC_underground_boss();
void AddSC_instance_underground();

// Add all
// cf. the naming convention https://github.com/azerothcore/azerothcore-wotlk/blob/master/doc/changelog/master.md#how-to-upgrade-4
// additionally replace all '-' in the module folder name with '_' here
void Addfl_underground_dungeonScripts()
{
    AddSC_underground_player();
    AddSC_underground_creature();
    AddSC_underground_boss();
    AddSC_instance_underground();
}

