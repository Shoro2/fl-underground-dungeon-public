#include "UndergroundInstance.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellMgr.h"
#include "SpellInfo.h"
#include "Creature.h"
#include "CreatureAIImpl.h"
#include "Player.h"
#include "UndergroundUtils.h"
#include "UndergroundState.h"
#include "ParagonUtils.h"

class TeleportOutEvent : public BasicEvent
{
public:
    TeleportOutEvent(Player* player) : _player(player) {}

    bool Execute(uint64 /*time*/, uint32 /*diff*/) override
    {
        if (_player && _player->IsInWorld())
        {
            _player->TeleportTo(727, 13414.074219f, 12073.212891f, -46.700172f, 5.403475f);
            ChatHandler(_player->GetSession()).SendSysMessage("[Dungeon] You have been teleported out.");
        }
        return true;
    }

private:
    Player* _player;
};



enum BossSpells
{
    SPELL_HOLY_SMITE = 800657,
    SPELL_CONSECRATION = 800656,
    SPELL_DEVOTION_AURA = 800658,
    SPELL_AVENGING_WRATH = 800659,
    SPELL_HAMMER_OF_WRATH = 800660,
    SPELL_WINGS_VISUAL = 38162,

    SPELL_CARRION_SWARM = 800661,
    SPELL_VAMPIRIC_BOLT = 800662,
    SPELL_JUDGEMENT_OF_DARKNESS = 800664, //triggers 800665
    SPELL_AURA_OF_DARKNESS = 800663, 
    SPELL_BLOOD_TAP = 800666,

    SPELL_CRYPT_SCARAB_SWARM = 800667,
    SPELL_IMPALE = 800668,
    SPELL_BONE_SPIKES = 800670,
    SPELL_CHITINOUS_SKIN = 800671
};

enum Events
{
    EVENT_SUMMON_SCARABS = 1,
    EVENT_HOLY_SMITE = 2,
    EVENT_CONSECRATION = 3,
    EVENT_DEVOTION_AURA = 4,
    EVENT_AVENGING_WRATH = 5,
    EVENT_HAMMER_OF_WRATH = 6,
    EVENT_WINGS_VISUAL = 7,

    EVENT_CARRION_SWARM = 8,
    EVENT_VAMPIRIC_BOLT = 9,
    EVENT_JUDGEMENT_OF_DARKNESS = 10, //triggers 800665
    EVENT_AURA_OF_DARKNESS = 11,
    EVENT_BLOOD_TAP = 12,

    EVENT_CRYPT_SCARAB_SWARM = 13,
    EVENT_IMPALE = 14,
    EVENT_BONE_SPIKES = 15,
    EVENT_CHITINOUS_SKIN = 16,

    EVENT_UPDATE_TRASH_AURA = 17,
};

void GiveLoot(Creature* me, Unit* killer)
{
    if (killer)
    {
        LOG_DEBUG("module", "GiveLoot called for killer: '{}'",  killer->GetGUID().ToString());
        if (killer->GetOwner()) {
            LOG_DEBUG("module", "GiveLoot: GetOwner()");
            killer = killer->GetOwner();
        }
            
        Player* player;
        
        if(!killer->ToPlayer()){
            LOG_DEBUG("module", "GiveLoot: Killer is not a player, aborting loot.");
            player = me->SelectNearestPlayer(200.0f);
            if (!player) return;
        }
        else {
            player = killer->ToPlayer();
        }
        
        uint8 difficulty = UndergroundState::ReadDifficultyFromCreature(me);

        // Reward: Paragon XP
        uint32 paragonXP = difficulty*8;
        IncreaseParagonXP(player, paragonXP);
        ChatHandler(player->GetSession()).PSendSysMessage("[Abyssal Mastery] You earned %u XP!", paragonXP);


        // Reward: Explorer Badges (2–11)
        uint32 badges = std::clamp<uint32>(difficulty / 10 + 1, 2, 11);
        MailItemOrGive(player, 251100, badges, "Dungeon Reward: Explorer Badges", me);

        // Reward: Expert Emblem only at difficulty ≥ 80
        if (difficulty >= 80)
        {
            if (urand(1, 100) <= 25)
                MailItemOrGive(player, 251000, 1, "Dungeon Reward: Expert Emblem", me);
            
        }

        if(difficulty >= 100)
        {
            if (urand(1, 100) <= 5)
                MailItemOrGive(player, 80004, 1, "Rare Drop: Exobeast Energy Plate", me);
        }

        // Class-based armor box
        if (difficulty >= 26 && urand(1, 100) <= 50)
        {
            uint32 chest = 0;
            switch (player->getClass())
            {
            case CLASS_MAGE:
            case CLASS_WARLOCK:
            case CLASS_PRIEST:        chest = 251001; break;
            case CLASS_ROGUE:
            case CLASS_DRUID:         chest = 251002; break;
            case CLASS_SHAMAN:
            case CLASS_HUNTER:        chest = 251003; break;
            case CLASS_PALADIN:
            case CLASS_WARRIOR:
            case CLASS_DEATH_KNIGHT:  chest = 251004; break;
            }

            if (chest != 0)
            {
                MailItemOrGive(player, chest, 1, "Mystery Armor Box", me);
                ChatHandler(player->GetSession()).SendSysMessage("[Loot] You found a mysterious armor box!");
            }
        }

        // Jewelry box
        if (difficulty >= 26 && urand(1, 100) <= 25)
        {
            MailItemOrGive(player, 251005, 1, "Mystery Jewelry Box", me);
            ChatHandler(player->GetSession()).SendSysMessage("[Loot] You found a mysterious jewelry box!");
        }

        // Materials
        if (urand(1, 100) <= 75)
            MailItemOrGive(player, 80002, urand(1, 3), "Loot: Relic Fragment", me);

        if (urand(1, 100) <= 50)
            MailItemOrGive(player, 80006, urand(1, 3), "Loot: Exonium Ore", me);

        if (difficulty >= 50 && urand(1, 100) <= 20)
            MailItemOrGive(player, 80051, 1, "Loot: Exonium Magical Crystal", me);

        // Mounts
        if (urand(1, 10000) == 10000) {
            LOG_DEBUG("module", "MOUNT drop for killer: '{}'", killer->GetGUID().ToString());
            MailItemOrGive(player, 80095, 1, "EPIC Loot: Lava Mammoth", me);
        }
        if (urand(1, 10000) == 10000) {
            LOG_DEBUG("module", "MOUNT drop for killer: '{}'", killer->GetGUID().ToString());
            MailItemOrGive(player, 80088, 1, "EPIC Loot: Maldraxxus Fly", me);
        }
            
        if (urand(1, 10000) == 10000) {
            LOG_DEBUG("module", "MOUNT drop for killer: '{}'", killer->GetGUID().ToString());
            MailItemOrGive(player, 80096, 1, "EPIC Loot: Primal Dragonfly", me);
        }
        if (urand(1, 10000) == 10000) {
            LOG_DEBUG("module", "MOUNT drop for killer: '{}'", killer->GetGUID().ToString());
            MailItemOrGive(player, 80100, 1, "EPIC Loot: Thunder Lizard", me);
        }
        if (urand(1, 10000) == 10000) {
            LOG_DEBUG("module", "MOUNT drop for killer: '{}'", killer->GetGUID().ToString());
            MailItemOrGive(player, 80093, 1, "EPIC Loot: Lava Slug", me);
        }
        LOG_DEBUG("module", "Finished Loot for player: '{}'", killer->GetGUID().ToString());

        // Log run duration
        uint32 guid = player->GetGUID().GetCounter();
        std::string name = player->GetName();

        uint32 startTime = UndergroundState::GetStartTime(player);

        uint32 duration = getMSTime() - startTime;

        UndergroundState::MarkRunCompleted(player);

        CharacterDatabase.Query(
            "INSERT INTO dungeon_runs (guid, name, difficulty, duration_ms) VALUES ('{}', '{}', '{}', '{}')",
            guid, name.c_str(), difficulty, duration
        );

        //ChatHandler(player->GetSession()).PSendSysMessage(
        //    "[Dungeon] Run logged: GUID=%u, Name=%s, Difficulty=%u, Duration=%u ms",
        //    guid, name.c_str(), difficulty, duration
        //);

        // Teleport player out after 10 seconds with animation and message
        if (player && player->IsInWorld())
        {
            ChatHandler(player->GetSession()).SendSysMessage("[Dungeon] You will be teleported out in 10 seconds.");
            // 10 Sekunden später teleportieren
            me->m_Events.AddEvent(new TeleportOutEvent(player), me->m_Events.CalculateTime(10000));
        }


    }
}

class boss_dralak : public CreatureScript
{
public:
    boss_dralak() : CreatureScript("boss_dralak") { }

    struct boss_dralakAI : public ScriptedAI
    {
        boss_dralakAI(Creature* creature) : ScriptedAI(creature)
        {
            baseHealth = creature->GetMaxHealth();
        }

        
        bool phaseTwo = false;
        float scale = 1.0f;
        uint32 baseHealth = 0;
        bool scaledHP = false;

        void UpdateTrashAura()
        {
            if (!me->GetInstanceScript())
                return;

            if (instance_underground* instance = dynamic_cast<instance_underground*>(me->GetInstanceScript()))
            {
                uint32 aliveCount = 0;
                for (ObjectGuid guid : instance->spawnedCreatures)
                {
                    if (Creature* creature = me->GetMap()->GetCreature(guid))
                    {
                        if (creature->IsAlive() && creature != me)
                            ++aliveCount;
                    }
                }
                if (aliveCount == 0)
                {
                    me->RemoveAurasDueToSpell(100027); // remove aura if no alive creatures
                    return;
                }
                me->AddAura(100027, me); // ensure aura is applied
                if (Aura* aura = me->GetAura(100027))
                    aura->SetStackAmount(aliveCount);
            }
        }


        
        void Init()
        {
            SetCreatureDifficultyFromNearbyPlayer(me);
            ApplyHealthScaling(me, baseHealth, scaledHP);
            uint8 difficulty = UndergroundState::ReadDifficultyFromCreature(me);
            events.ScheduleEvent(EVENT_HOLY_SMITE, 8000);
            if (difficulty >= 50) events.ScheduleEvent(EVENT_CONSECRATION, 10s);
            if (difficulty >= 75) events.ScheduleEvent(EVENT_DEVOTION_AURA, 10s);
            //events.ScheduleEvent(EVENT_UPDATE_TRASH_AURA, 1s);
        }


        void Reset() override
        {
            events.Reset();
            phaseTwo = false;
            scaledHP = false;

            Init();

        }

        void JustDied(Unit* killer) override
        {
            LOG_DEBUG("module", "Paladin kill for killer: '{}'", killer->GetGUID().ToString());
            me->Yell("The light... fades...", LANG_UNIVERSAL);
            GiveLoot(me, killer);
            
        }


        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
            {
                return;
            }

            events.Update(diff);

            if (!phaseTwo && me->HealthBelowPct(50))
            {
                phaseTwo = true;
                me->Yell("Witness my true form!", LANG_UNIVERSAL);
                DoCast(me, SPELL_WINGS_VISUAL, true);
                DoCast(me, SPELL_AVENGING_WRATH);
                events.ScheduleEvent(EVENT_HAMMER_OF_WRATH, 10s);
            }

            float scale = 1.0f + (UndergroundState::ReadDifficultyFromCreature(me) * 0.03f);
                switch (events.ExecuteEvent())
                {
                case EVENT_HOLY_SMITE:
                    CastScaledSpell(me, me->GetVictim(), SPELL_HOLY_SMITE, scale);
                    events.ScheduleEvent(EVENT_HOLY_SMITE, 9s);
                    break;
                case EVENT_CONSECRATION:
                    CastScaledSpell(me, me->GetVictim(), SPELL_CONSECRATION, scale);
                    events.ScheduleEvent(EVENT_CONSECRATION, 20s);
                    break;
                case EVENT_DEVOTION_AURA:
                    DoCastVictim(SPELL_DEVOTION_AURA);
                    events.ScheduleEvent(EVENT_DEVOTION_AURA, 30s);
                    break;
                case EVENT_HAMMER_OF_WRATH:
                    CastScaledSpell(me, me->GetVictim(), SPELL_HAMMER_OF_WRATH, scale);
                    events.ScheduleEvent(EVENT_HAMMER_OF_WRATH, 15s);
                    break;
                //case EVENT_UPDATE_TRASH_AURA:
                //    UpdateTrashAura();
                //    ApplyHealthScaling(me, baseHealth, scaledHP);
                //    events.ScheduleEvent(EVENT_UPDATE_TRASH_AURA, 5s);
                //    break;
                }


            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new boss_dralakAI(creature);
    }
};

class boss_vampir_lord : public CreatureScript
{
public:
    boss_vampir_lord() : CreatureScript("boss_vampir_lord") {}

    struct boss_vampir_lordAI : public ScriptedAI
    {
        boss_vampir_lordAI(Creature* creature) : ScriptedAI(creature) {
            baseHealth = creature->GetMaxHealth();
        }

        bool phaseTwo = false;
        float scale = 1.0f;
        uint32 baseHealth = 0;
        bool scaledHP = false;

        void UpdateTrashAura()
        {
            if (!me->GetInstanceScript())
                return;

            if (instance_underground* instance = dynamic_cast<instance_underground*>(me->GetInstanceScript()))
            {
                uint32 aliveCount = 0;
                for (ObjectGuid guid : instance->spawnedCreatures)
                {
                    if (Creature* creature = me->GetMap()->GetCreature(guid))
                    {
                        if (creature->IsAlive() && creature != me)
                            ++aliveCount;
                    }
                }
                if (aliveCount == 0)
                {
                    me->RemoveAurasDueToSpell(100027); // remove aura if no alive creatures
                    return;
                }
                me->AddAura(100027, me); // ensure aura is applied
                if (Aura* aura = me->GetAura(100027))
                    aura->SetStackAmount(aliveCount);
            }
        }


        void Init()
        {
            SetCreatureDifficultyFromNearbyPlayer(me);
            ApplyHealthScaling(me, baseHealth, scaledHP);
            uint8 difficulty = UndergroundState::ReadDifficultyFromCreature(me);
            events.ScheduleEvent(EVENT_CARRION_SWARM, 8000);
            if (difficulty >= 50) events.ScheduleEvent(EVENT_VAMPIRIC_BOLT, 10s);
            if (difficulty >= 75) events.ScheduleEvent(EVENT_JUDGEMENT_OF_DARKNESS, 20s);
            //events.ScheduleEvent(EVENT_UPDATE_TRASH_AURA, 1s);
        }

        void Reset() override
        {
            events.Reset();
            phaseTwo = false;
            scaledHP = false;
            Init();
        }

        void JustDied(Unit* killer) override
        {
            LOG_DEBUG("module", "vamp kill for killer: '{}'", killer->GetGUID().ToString());
            events.Reset();
            me->Yell("The night... calls me back...", LANG_UNIVERSAL);
            GiveLoot(me, killer);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim()) return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
            {
                return;
            }

            events.Update(diff);
            float scale = 1.0f + (UndergroundState::ReadDifficultyFromCreature(me) * 0.03f);
            if (!phaseTwo && me->HealthBelowPct(50))
            {
                phaseTwo = true;
                me->Yell("Witness my true thirst!", LANG_UNIVERSAL);
                DoCast(me, SPELL_AURA_OF_DARKNESS);
                events.ScheduleEvent(EVENT_BLOOD_TAP, 15s);
            }
            
            switch (events.ExecuteEvent())
            {
                case EVENT_CARRION_SWARM:
                    CastScaledSpell(me, me->GetVictim(), SPELL_CARRION_SWARM, scale);
                    events.ScheduleEvent(EVENT_CARRION_SWARM, 12s);
                    break;
                case EVENT_VAMPIRIC_BOLT:
                    CastScaledSpell(me, me->GetVictim(), SPELL_VAMPIRIC_BOLT, scale);
                    events.ScheduleEvent(EVENT_VAMPIRIC_BOLT, 15s);
                    break;
                case EVENT_JUDGEMENT_OF_DARKNESS:
                    DoCastVictim(SPELL_JUDGEMENT_OF_DARKNESS);
                    events.ScheduleEvent(EVENT_JUDGEMENT_OF_DARKNESS, 20s);
                    break;
                case EVENT_BLOOD_TAP:
                    CastScaledSpell(me, me->GetVictim(), SPELL_BLOOD_TAP, scale);
                    events.ScheduleEvent(EVENT_BLOOD_TAP, 10s);
                    break;
                //case EVENT_UPDATE_TRASH_AURA:
                //{
                //    UpdateTrashAura();
                //    ApplyHealthScaling(me, baseHealth, scaledHP);
                //    events.ScheduleEvent(EVENT_UPDATE_TRASH_AURA, 5s);
                //    break;
                //}
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override { return new boss_vampir_lordAI(creature); }
};

class boss_crypt_lord : public CreatureScript
{
public:
    boss_crypt_lord() : CreatureScript("boss_crypt_lord") {}

    struct boss_crypt_lordAI : public ScriptedAI
    {
        boss_crypt_lordAI(Creature* creature) : ScriptedAI(creature) {
            baseHealth = creature->GetMaxHealth();
        }

        EventMap events;
        bool phaseTwo = false;
        float scale = 1.0f;
        uint32 baseHealth = 0;
        bool scaledHP = false;

        void UpdateTrashAura()
        {
            if (!me->GetInstanceScript())
                return;

            if (instance_underground* instance = dynamic_cast<instance_underground*>(me->GetInstanceScript()))
            {
                uint32 aliveCount = 0;
                for (ObjectGuid guid : instance->spawnedCreatures)
                {
                    if (Creature* creature = me->GetMap()->GetCreature(guid))
                    {
                        if (creature->IsAlive() && creature != me)
                            ++aliveCount;
                    }
                }
                if(aliveCount ==0)
                {
                    me->RemoveAurasDueToSpell(100027); // remove aura if no alive creatures
                    return;
                }
                me->AddAura(100027, me); // ensure aura is applied
                if (Aura* aura = me->GetAura(100027))
                    aura->SetStackAmount(aliveCount);
            }
        }


        void Init()
        {
            SetCreatureDifficultyFromNearbyPlayer(me);
            ApplyHealthScaling(me, baseHealth, scaledHP);
            uint8 difficulty = UndergroundState::ReadDifficultyFromCreature(me);
            events.ScheduleEvent(EVENT_CRYPT_SCARAB_SWARM, 8000);
            if (difficulty >= 50) events.ScheduleEvent(EVENT_IMPALE, 10s);
            if (difficulty >= 75) events.ScheduleEvent(EVENT_BONE_SPIKES, 20s);
            //events.ScheduleEvent(EVENT_UPDATE_TRASH_AURA, 1s);
        }

        void Reset() override
        {
            events.Reset();
            phaseTwo = false;
            scaledHP = false;
            Init();
        }

        void JustDied(Unit* killer) override
        {
            LOG_DEBUG("module", "bug kill for killer: '{}'", killer->GetGUID().ToString());
            events.Reset();
            me->Yell("The swarm... retreats...", LANG_UNIVERSAL);
            GiveLoot(me, killer);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim()) return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
            {
                return;
            }

            events.Update(diff);
            float scale = 1.0f + (UndergroundState::ReadDifficultyFromCreature(me) * 0.03f);
            if (!phaseTwo && me->HealthBelowPct(50))
            {
                phaseTwo = true;
                me->Yell("Arise, my children!", LANG_UNIVERSAL);
                DoCast(me, SPELL_CHITINOUS_SKIN);
                events.ScheduleEvent(EVENT_SUMMON_SCARABS, 12s);
            }


                switch (events.ExecuteEvent())
                {
                    case EVENT_CRYPT_SCARAB_SWARM:
                        CastScaledSpell(me, me->GetVictim(), SPELL_CRYPT_SCARAB_SWARM, scale);
                        events.ScheduleEvent(EVENT_CRYPT_SCARAB_SWARM,12s);
                        break;
                    case EVENT_IMPALE:
                        CastScaledSpell(me, me->GetVictim(), SPELL_IMPALE, scale);
                        events.ScheduleEvent(EVENT_IMPALE, 150s);
                        break;
                    case EVENT_BONE_SPIKES:
                        CastScaledSpell(me, me->GetVictim(), SPELL_BONE_SPIKES, scale);
                        events.ScheduleEvent(EVENT_BONE_SPIKES, 20s);
                        break;
                    case EVENT_SUMMON_SCARABS:
                    {
                        for (size_t i = 0; i < 4; i++)
                        {
                            Creature* TempSumm = me->SummonCreature(84285, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0.0f, TEMPSUMMON_TIMED_DESPAWN, 40000);
                            SetCreatureDifficultyFromNearbyPlayer(TempSumm);
                            uint32 baseHealth = 0;
                            scaledHP = false;
                            baseHealth = TempSumm->GetMaxHealth();
                            ApplyHealthScaling(TempSumm, baseHealth, scaledHP);
                        }
                        events.ScheduleEvent(EVENT_SUMMON_SCARABS, 30s);
                    }
                    //case EVENT_UPDATE_TRASH_AURA:
                    //    UpdateTrashAura();
                    //    ApplyHealthScaling(me, baseHealth, scaledHP);
                    //    events.ScheduleEvent(EVENT_UPDATE_TRASH_AURA, 5s);
                    //    break;   

                }
                
            

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override { return new boss_crypt_lordAI(creature); }
};



void AddMyBossScripts()
{
    new boss_vampir_lord();
    new boss_crypt_lord();
    new boss_dralak();
}
