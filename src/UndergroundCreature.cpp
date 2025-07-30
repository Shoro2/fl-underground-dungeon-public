#include "UndergroundInstance.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "EventMap.h"
#include "SpellMgr.h"
#include "SpellInfo.h"
#include "UndergroundUtils.h"
#include "UndergroundState.h"

// Todo: Make heals/shields also scale with difficulty

enum Spells
{
    SPELL_UNDERGROUND_SOUL_FLAY = 800600,
    SPELL_UNDERGROUND_FROSTBOLT = 800601,
    SPELL_UNDERGROUND_FROST_NOVA = 800602,
    SPELL_UNDERGROUND_ICE_LANCE = 800603,
    SPELL_UNDERGROUND_BLIZZARD = 800604,
    SPELL_UNDERGROUND_CLEAVE = 800611, // 800605
    SPELL_UNDERGROUND_INFECTED_BITE = 800606,
    SPELL_UNDERGROUND_ENRAGE = 800607,
    SPELL_UNDERGROUND_STRIKE = 800608,
    SPELL_UNDERGROUND_ACID_SPIT = 800609,
    SPELL_UNDERGROUND_POISON_CLOUD = 800610,
    SPELL_UNDERGROUND_TRAMPLE = 800611,
    SPELL_UNDERGROUND_GORE = 800612,
    SPELL_UNDERGROUND_BONE_SHIELD = 800613,
    SPELL_UNDERGROUND_SUNDER_ARMOR = 800614,
    SPELL_UNDERGROUND_DEATH_COIL = 800615,
    SPELL_UNDERGROUND_SHADOW_BOLT = 800616,
    SPELL_UNDERGROUND_SHADOWFLAME = 800617,
    SPELL_UNDERGROUND_FEAR = 800618,
    SPELL_UNDERGROUND_CURSE_OF_DOOM = 800619,
    SPELL_UNDERGROUND_DRAIN_LIFE = 800620,
    SPELL_UNDERGROUND_CURSE_OF_AGONY = 800621,
    SPELL_UNDERGROUND_MIND_ROT = 800622,
    SPELL_UNDERGROUND_CURSE_OF_WEAKNESS = 800623,
    SPELL_UNDERGROUND_SHADOW_WORD_PAIN = 800624,
    SPELL_UNDERGROUND_SHADOWSTALKER_STAB = 800625,
    SPELL_UNDERGROUND_CURSE_OF_IDIOCY = 800626,
    SPELL_UNDERGROUND_CORRUPTION = 800627,
    SPELL_UNDERGROUND_SWARMING_SHADOWS = 800628,
    SPELL_UNDERGROUND_VOID_BOLT = 800629,
    SPELL_UNDERGROUND_SHADOW_CLAW = 800630,
    SPELL_UNDERGROUND_VOID_ZONE = 800631,
    SPELL_UNDERGROUND_BITE_WOUND = 800632,
    SPELL_UNDERGROUND_BLOOD_LEECH = 800633,
    SPELL_UNDERGROUND_EYE_BEAM = 800634,
    SPELL_UNDERGROUND_SHADOW_SHOCK = 800635,
    SPELL_UNDERGROUND_FLESH_REND = 800636,
    SPELL_UNDERGROUND_CARNAGE = 800637,
    SPELL_UNDERGROUND_SOUL_SPIKE = 800638,
    SPELL_UNDERGROUND_BANSHEE_WAIL = 800639,
    SPELL_UNDERGROUND_SAVAGE_POUNCE = 800640,
    SPELL_UNDERGROUND_FLESH_BITE = 800641,
    SPELL_UNDERGROUND_ACID_SPLASH = 800642,
    SPELL_UNDERGROUND_DIGESTIVE_ACID = 800643,
    SPELL_UNDERGROUND_WORM_BITE = 800644,
    SPELL_UNDERGROUND_NESTING_VOMIT = 800645,
    SPELL_UNDERGROUND_HOWLING_RAGE = 800646,
    SPELL_UNDERGROUND_BLOOD_ARMOR = 800647,
    SPELL_UNDERGROUND_BONE_SPLINTER = 800648,
    SPELL_UNDERGROUND_SPIRIT_BLAST = 800649,
    SPELL_UNDERGROUND_WAIL_OF_TORMENT = 800650,
    SPELL_UNDERGROUND_SHALESKIN = 800651,
    SPELL_UNDERGROUND_ABOMINABLE_SLAM = 800652,
    SPELL_UNDERGROUND_VILE_CLEAVE = 800611, //800653
    SPELL_UNDERGROUND_NIGHTMARE_BREATH = 800654,
    SPELL_UNDERGROUND_SHADOW_BITE = 800655

};

enum Events
{
    EVENT_UNDERGROUND_SOUL_FLAY = 1,
    EVENT_UNDERGROUND_FROSTBOLT = 2,
    EVENT_UNDERGROUND_FROST_NOVA = 3,
    EVENT_UNDERGROUND_ICE_LANCE = 4,
    EVENT_UNDERGROUND_BLIZZARD = 5,
    EVENT_UNDERGROUND_CLEAVE = 6,
    EVENT_UNDERGROUND_INFECTED_BITE = 7,
    EVENT_UNDERGROUND_ENRAGE = 8,
    EVENT_UNDERGROUND_STRIKE = 9,
    EVENT_UNDERGROUND_ACID_SPIT = 10,
    EVENT_UNDERGROUND_POISON_CLOUD = 11,
    EVENT_UNDERGROUND_TRAMPLE = 12,
    EVENT_UNDERGROUND_GORE = 13,
    EVENT_UNDERGROUND_BONE_SHIELD = 14,
    EVENT_UNDERGROUND_SUNDER_ARMOR = 15,
    EVENT_UNDERGROUND_DEATH_COIL = 16,
    EVENT_UNDERGROUND_SHADOW_BOLT = 17,
    EVENT_UNDERGROUND_SHADOWFLAME = 18,
    EVENT_UNDERGROUND_FEAR = 19,
    EVENT_UNDERGROUND_CURSE_OF_DOOM = 20,
    EVENT_UNDERGROUND_DRAIN_LIFE = 21,
    EVENT_UNDERGROUND_CURSE_OF_AGONY = 22,
    EVENT_UNDERGROUND_MIND_ROT = 23,
    EVENT_UNDERGROUND_CURSE_OF_WEAKNESS = 24,
    EVENT_UNDERGROUND_SHADOW_WORD_PAIN = 25,
    EVENT_UNDERGROUND_SHADOWSTALKER_STAB = 26,
    EVENT_UNDERGROUND_CURSE_OF_IDIOCY = 27,
    EVENT_UNDERGROUND_CORRUPTION = 28,
    EVENT_UNDERGROUND_SWARMING_SHADOWS = 29,
    EVENT_UNDERGROUND_VOID_BOLT = 30,
    EVENT_UNDERGROUND_SHADOW_CLAW = 31,
    EVENT_UNDERGROUND_VOID_ZONE = 32,
    EVENT_UNDERGROUND_BITE_WOUND = 33,
    EVENT_UNDERGROUND_BLOOD_LEECH = 34,
    EVENT_UNDERGROUND_EYE_BEAM = 35,
    EVENT_UNDERGROUND_SHADOW_SHOCK = 36,
    EVENT_UNDERGROUND_FLESH_REND = 37,
    EVENT_UNDERGROUND_CARNAGE = 38,
    EVENT_UNDERGROUND_SOUL_SPIKE = 39,
    EVENT_UNDERGROUND_BANSHEE_WAIL = 40,
    EVENT_UNDERGROUND_SAVAGE_POUNCE = 41,
    EVENT_UNDERGROUND_FLESH_BITE = 42,
    EVENT_UNDERGROUND_ACID_SPLASH = 43,
    EVENT_UNDERGROUND_DIGESTIVE_ACID = 44,
    EVENT_UNDERGROUND_WORM_BITE = 45,
    EVENT_UNDERGROUND_NESTING_VOMIT = 46,
    EVENT_UNDERGROUND_HOWLING_RAGE = 47,
    EVENT_UNDERGROUND_BLOOD_ARMOR = 48,
    EVENT_UNDERGROUND_BONE_SPLINTER = 49,
    EVENT_UNDERGROUND_SPIRIT_BLAST = 50,
    EVENT_UNDERGROUND_WAIL_OF_TORMENT = 51,
    EVENT_UNDERGROUND_SHALESKIN = 52,
    EVENT_UNDERGROUND_ABOMINABLE_SLAM = 53,
    EVENT_UNDERGROUND_VILE_CLEAVE = 54,
    EVENT_UNDERGROUND_NIGHTMARE_BREATH = 55,
    EVENT_UNDERGROUND_SHADOW_BITE = 56


};

void giveLoot(Unit* killer, Creature* me) {
    if (killer && me)
    {
        if (killer->GetOwner())
            killer = killer->GetOwner();

        Player* player;
        if (!killer->ToPlayer()) {
            player = me->SelectNearestPlayer(200.0f);
            if (!player) return;
        }
        else {
            player = killer->ToPlayer();
        }
        
        uint8 difficulty = UndergroundState::ReadDifficultyFromCreature(me);

        int32 badges = urand(1, std::clamp<uint32>(difficulty / 33, 1, 3));
        MailItemOrGive(player, 250065, badges, "Dungeon Reward: Quantum Gearing", me);

        if (urand(1, 100) <= 1 && difficulty < 75)
            MailItemOrGive(player, 95002, 1, "Dungeon Reward: Energy Core", me);

        else if (urand(1, 100) <= 2 && difficulty >= 75)
                MailItemOrGive(player, 95002, badges, "Dungeon Reward: Energy Core", me);
        
            
    }
}

#define ADD_EXTRA_SPELLS(diff, spell3, spell4) \
    if (diff >= 50) events.ScheduleEvent(spell3, 10s); \
    if (diff >= 75) events.ScheduleEvent(spell4, 11s);

// === Dread Maggot (caster/worm) NEW===
struct npc_dread_maggot : public CreatureScript
{
    npc_dread_maggot() : CreatureScript("npc_dread_maggot") {}

    struct npc_dread_maggotAI : public ScriptedAI
    {
        npc_dread_maggotAI(Creature* creature) : ScriptedAI(creature)
        {
            baseHealth = creature->GetMaxHealth();
        }

        uint32 baseHealth = 0;
        bool scaledHP = false;

        void Init() {
            uint8 difficulty = UndergroundState::ReadDifficultyFromCreature(me);
            events.ScheduleEvent(EVENT_UNDERGROUND_ACID_SPLASH, 6000);
            events.ScheduleEvent(EVENT_UNDERGROUND_WORM_BITE, 8000);
            if (difficulty >= 50) events.ScheduleEvent(EVENT_UNDERGROUND_DIGESTIVE_ACID, 9s);
            if (difficulty >= 75) events.ScheduleEvent(EVENT_UNDERGROUND_NESTING_VOMIT, 12s);
            SetCreatureDifficultyFromNearbyPlayer(me);
            ApplyHealthScaling(me, baseHealth, scaledHP);

        }

        void Reset() override
        {
            events.Reset();
            scaledHP = false;
            Init();
        }

        void JustDied(Unit* killer) override
        {
            giveLoot(killer, me);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim()) return;

            

            if (me->HasUnitState(UNIT_STATE_CASTING))
            {
                return;
            }
            events.Update(diff);
            DoMeleeAttackIfReady();

            float scale = 1.0f + (UndergroundState::ReadDifficultyFromCreature(me) * 0.02f);
            switch (events.ExecuteEvent())
            {
            case EVENT_UNDERGROUND_ACID_SPLASH:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_ACID_SPLASH, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_ACID_SPLASH, 15s);
                }
                break;
            case EVENT_UNDERGROUND_WORM_BITE:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_WORM_BITE, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_WORM_BITE, 10s);
                }
                break;
            case EVENT_UNDERGROUND_DIGESTIVE_ACID:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_DIGESTIVE_ACID, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_DIGESTIVE_ACID, 12s);
                }
                break;
            case EVENT_UNDERGROUND_NESTING_VOMIT:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_NESTING_VOMIT, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_NESTING_VOMIT, 60s);
                }
                break;
            default:
                break;
            }
        }
    };


    CreatureAI* GetAI(Creature* creature) const override { return new npc_dread_maggotAI(creature); }
};



// === Crypt Howler (melee group) ===
struct npc_crypt_howler : public CreatureScript
{
    npc_crypt_howler() : CreatureScript("npc_crypt_howler") {}

    struct npc_crypt_howlerAI : public ScriptedAI
    {
        npc_crypt_howlerAI(Creature* creature) : ScriptedAI(creature)
        {
            baseHealth = creature->GetMaxHealth();
        }

        uint32 baseHealth = 0;
        bool scaledHP = false;

        void Init() {
            SetCreatureDifficultyFromNearbyPlayer(me);
            ApplyHealthScaling(me, baseHealth, scaledHP);
            uint8 difficulty = UndergroundState::ReadDifficultyFromCreature(me);
            events.ScheduleEvent(EVENT_UNDERGROUND_SAVAGE_POUNCE, 5s);
            events.ScheduleEvent(EVENT_UNDERGROUND_CLEAVE, 11s);
            if (difficulty >= 50) events.ScheduleEvent(EVENT_UNDERGROUND_ENRAGE, 9s);
            if (difficulty >= 75) events.ScheduleEvent(EVENT_UNDERGROUND_BLOOD_LEECH, 13s);
            // Event scheduling was here before and remains untouched
        }

        void Reset() override
        {
            events.Reset();
            scaledHP = false;
            Init();
        }

        void JustDied(Unit* killer) override
        {
            giveLoot(killer, me);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim()) return;

            

            if (me->HasUnitState(UNIT_STATE_CASTING))
            {
                return;
            }
            events.Update(diff);
            DoMeleeAttackIfReady();

            float scale = 1.0f + (UndergroundState::ReadDifficultyFromCreature(me) * 0.02f);
            switch (events.ExecuteEvent())
            {
            case EVENT_UNDERGROUND_SAVAGE_POUNCE:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_SAVAGE_POUNCE, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_SAVAGE_POUNCE, 15s);
                }
                break;
            case EVENT_UNDERGROUND_CLEAVE:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_CLEAVE, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_CLEAVE, 10s);
                }
                break;
            case EVENT_UNDERGROUND_ENRAGE:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_ENRAGE, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_ENRAGE, 12s);
                }
                break;
            case EVENT_UNDERGROUND_BLOOD_LEECH:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_BLOOD_LEECH, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_BLOOD_LEECH, 60s);
                }
                break;
            default:
                break;
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override { return new npc_crypt_howlerAI(creature); }
};


// === Gloomfang (ranged/caster hybrid group) UPDATED ===
struct npc_gloomfang : public CreatureScript
{
    npc_gloomfang() : CreatureScript("npc_gloomfang") {}

    struct npc_gloomfangAI : public ScriptedAI
    {
        npc_gloomfangAI(Creature* creature) : ScriptedAI(creature)
        {
            baseHealth = creature->GetMaxHealth();
        }

        uint32 baseHealth = 0;
        bool scaledHP = false;

        void Init()
        {
            SetCreatureDifficultyFromNearbyPlayer(me);
            ApplyHealthScaling(me, baseHealth, scaledHP);
            uint8 difficulty = UndergroundState::ReadDifficultyFromCreature(me);
            events.ScheduleEvent(EVENT_UNDERGROUND_NIGHTMARE_BREATH, 13000);
            events.ScheduleEvent(EVENT_UNDERGROUND_SHADOW_BITE, 6000);
            if (difficulty >= 50) events.ScheduleEvent(EVENT_UNDERGROUND_DRAIN_LIFE, 10s);
            if (difficulty >= 75) events.ScheduleEvent(EVENT_UNDERGROUND_SOUL_FLAY, 60s);
        }

        void Reset() override
        {
            events.Reset();
            scaledHP = false;
            Init();
        }

        void JustDied(Unit* killer) override
        {
            giveLoot(killer, me);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim()) return;

            

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
            events.Update(diff);
            DoMeleeAttackIfReady();

            float scale = 1.0f + (UndergroundState::ReadDifficultyFromCreature(me) * 0.02f);
            switch (events.ExecuteEvent())
            {
            case EVENT_UNDERGROUND_NIGHTMARE_BREATH:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_NIGHTMARE_BREATH, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_NIGHTMARE_BREATH, 20s);
                }
                break;
            case EVENT_UNDERGROUND_SHADOW_BITE:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_SHADOW_BITE, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_SHADOW_BITE, 10s);
                }
                break;
            case EVENT_UNDERGROUND_DRAIN_LIFE:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_DRAIN_LIFE, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_DRAIN_LIFE, 12s);
                }
                break;
            case EVENT_UNDERGROUND_SOUL_FLAY:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_SOUL_FLAY, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_SOUL_FLAY, 60s);
                }
                break;
            default:
                break;
            }
        }

    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override { return new npc_gloomfangAI(creature); }
};



// === Twisted Abomination (melee group) UPDATED ===
struct npc_twisted_abomination : public CreatureScript
{
    npc_twisted_abomination() : CreatureScript("npc_twisted_abomination") {}

    struct npc_twisted_abominationAI : public ScriptedAI
    {
        npc_twisted_abominationAI(Creature* creature) : ScriptedAI(creature)
        {
            baseHealth = creature->GetMaxHealth();
        }

        uint32 baseHealth = 0;
        bool scaledHP = false;

        void Init()
        {
            SetCreatureDifficultyFromNearbyPlayer(me);
            ApplyHealthScaling(me, baseHealth, scaledHP);
            uint8 difficulty = UndergroundState::ReadDifficultyFromCreature(me);
            events.ScheduleEvent(EVENT_UNDERGROUND_ABOMINABLE_SLAM, 7000);
            events.ScheduleEvent(EVENT_UNDERGROUND_VILE_CLEAVE, 6000);
            if (difficulty >= 50) events.ScheduleEvent(EVENT_UNDERGROUND_ENRAGE, 10s);
            if (difficulty >= 75) events.ScheduleEvent(EVENT_UNDERGROUND_CURSE_OF_IDIOCY, 12s);
        }

        void Reset() override
        {
            events.Reset();
            scaledHP = false;
            Init();
        }

        void JustDied(Unit* killer) override
        {
            giveLoot(killer, me);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim()) return;

            

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
            events.Update(diff);
            DoMeleeAttackIfReady();

            float scale = 1.0f + (UndergroundState::ReadDifficultyFromCreature(me) * 0.02f);
            switch (events.ExecuteEvent())
            {
            case EVENT_UNDERGROUND_ABOMINABLE_SLAM:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_ABOMINABLE_SLAM, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_ABOMINABLE_SLAM, 10s);
                }
                break;
            case EVENT_UNDERGROUND_VILE_CLEAVE:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_VILE_CLEAVE, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_VILE_CLEAVE, 12s);
                }
                break;
            case EVENT_UNDERGROUND_ENRAGE:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_ENRAGE, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_ENRAGE, 15s);
                }
                break;
            case EVENT_UNDERGROUND_CURSE_OF_IDIOCY:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_CURSE_OF_IDIOCY, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_CURSE_OF_IDIOCY, 60s);
                }
                break;
            default:
                break;
            }
        }

    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override { return new npc_twisted_abominationAI(creature); }
};



// === Ashen Wailer (caster group) UPDATED ===
struct npc_ashen_wailer : public CreatureScript
{
    npc_ashen_wailer() : CreatureScript("npc_ashen_wailer") {}

    struct npc_ashen_wailerAI : public ScriptedAI
    {
        npc_ashen_wailerAI(Creature* creature) : ScriptedAI(creature)
        {
            baseHealth = creature->GetMaxHealth();
        }

        uint32 baseHealth = 0;
        bool scaledHP = false;

        void Init()
        {
            SetCreatureDifficultyFromNearbyPlayer(me);
            ApplyHealthScaling(me, baseHealth, scaledHP);
            uint8 difficulty = UndergroundState::ReadDifficultyFromCreature(me);
            events.ScheduleEvent(EVENT_UNDERGROUND_WAIL_OF_TORMENT, 60000);
            events.ScheduleEvent(EVENT_UNDERGROUND_SPIRIT_BLAST, 7000);
            if (difficulty >= 50) events.ScheduleEvent(EVENT_UNDERGROUND_SHALESKIN, 10s);
            if (difficulty >= 75) events.ScheduleEvent(EVENT_UNDERGROUND_SWARMING_SHADOWS, 12s);
        }

        void Reset() override
        {
            events.Reset();
            scaledHP = false;
            Init();
        }

        void JustDied(Unit* killer) override
        {
            giveLoot(killer, me);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim()) return;

            

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
            events.Update(diff);
            DoMeleeAttackIfReady();

            float scale = 1.0f + (UndergroundState::ReadDifficultyFromCreature(me) * 0.02f);
            switch (events.ExecuteEvent())
            {
            case EVENT_UNDERGROUND_WAIL_OF_TORMENT:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_WAIL_OF_TORMENT, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_WAIL_OF_TORMENT, 60s);
                }
                break;
            case EVENT_UNDERGROUND_SPIRIT_BLAST:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_SPIRIT_BLAST, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_SPIRIT_BLAST, 10s);
                }
                break;
            case EVENT_UNDERGROUND_SHALESKIN:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_SHALESKIN, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_SHALESKIN, 12s);
                }
                break;
            case EVENT_UNDERGROUND_SWARMING_SHADOWS:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_SWARMING_SHADOWS, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_SWARMING_SHADOWS, 15s);
                }
                break;
            default:
                break;
            }
        }

    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override { return new npc_ashen_wailerAI(creature); }
};



// === Warped Bonefiend (caster group) UPDATED ===
struct npc_warped_bonefiend : public CreatureScript
{
    npc_warped_bonefiend() : CreatureScript("npc_warped_bonefiend") {}

    struct npc_warped_bonefiendAI : public ScriptedAI
    {
        npc_warped_bonefiendAI(Creature* creature) : ScriptedAI(creature)
        {
            baseHealth = creature->GetMaxHealth();
        }

        uint32 baseHealth = 0;
        bool scaledHP = false;

        void Init()
        {
            SetCreatureDifficultyFromNearbyPlayer(me);
            ApplyHealthScaling(me, baseHealth, scaledHP);
            uint8 difficulty = UndergroundState::ReadDifficultyFromCreature(me);
            events.ScheduleEvent(EVENT_UNDERGROUND_BONE_SPLINTER, 8000);
            events.ScheduleEvent(EVENT_UNDERGROUND_SPIRIT_BLAST, 6000);
            if (difficulty >= 50) events.ScheduleEvent(EVENT_UNDERGROUND_SHADOW_WORD_PAIN, 10s);
            if (difficulty >= 75) events.ScheduleEvent(EVENT_UNDERGROUND_CURSE_OF_DOOM, 14s);
        }

        void Reset() override
        {
            events.Reset();
            scaledHP = false;
            Init();
        }

        void JustDied(Unit* killer) override
        {
            giveLoot(killer, me);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim()) return;

            

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
            events.Update(diff);
            DoMeleeAttackIfReady();

            float scale = 1.0f + (UndergroundState::ReadDifficultyFromCreature(me) * 0.02f);
            switch (events.ExecuteEvent())
            {
            case EVENT_UNDERGROUND_BONE_SPLINTER:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_BONE_SPLINTER, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_BONE_SPLINTER, 10s);
                }
                break;
            case EVENT_UNDERGROUND_SPIRIT_BLAST:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_SPIRIT_BLAST, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_SPIRIT_BLAST, 10s);
                }
                break;
            case EVENT_UNDERGROUND_SHADOW_WORD_PAIN:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_SHADOW_WORD_PAIN, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_SHADOW_WORD_PAIN, 12s);
                }
                break;
            case EVENT_UNDERGROUND_CURSE_OF_DOOM:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_CURSE_OF_DOOM, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_CURSE_OF_DOOM, 60s);
                }
                break;
            default:
                break;
            }
        }

    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override { return new npc_warped_bonefiendAI(creature); }
};


// === Bloodspike Beast (melee group) UPDATED ===
struct npc_bloodspike_beast : public CreatureScript
{
    npc_bloodspike_beast() : CreatureScript("npc_bloodspike_beast") {}

    struct npc_bloodspike_beastAI : public ScriptedAI
    {
        npc_bloodspike_beastAI(Creature* creature) : ScriptedAI(creature)
        {
            baseHealth = creature->GetMaxHealth();
        }

        uint32 baseHealth = 0;
        bool scaledHP = false;

        void Init()
        {
            SetCreatureDifficultyFromNearbyPlayer(me);
            ApplyHealthScaling(me, baseHealth, scaledHP);
            uint8 difficulty = UndergroundState::ReadDifficultyFromCreature(me);
            events.ScheduleEvent(EVENT_UNDERGROUND_HOWLING_RAGE, 10s);
            events.ScheduleEvent(EVENT_UNDERGROUND_CARNAGE, 7000);
            if (difficulty >= 50) events.ScheduleEvent(EVENT_UNDERGROUND_ENRAGE, 20s);
            if (difficulty >= 75) events.ScheduleEvent(EVENT_UNDERGROUND_BLOOD_ARMOR, 13s);
        }

        void Reset() override
        {
            events.Reset();
            scaledHP = false;
            Init();
        }

        void JustDied(Unit* killer) override
        {
            giveLoot(killer, me);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim()) return;

            

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
            events.Update(diff);
            DoMeleeAttackIfReady();

            float scale = 1.0f + (UndergroundState::ReadDifficultyFromCreature(me) * 0.02f);
            switch (events.ExecuteEvent())
            {
            case EVENT_UNDERGROUND_HOWLING_RAGE:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    DoCast(me->GetVictim(), SPELL_UNDERGROUND_HOWLING_RAGE);
                    events.ScheduleEvent(EVENT_UNDERGROUND_HOWLING_RAGE, 10s);
                }
                break;
            case EVENT_UNDERGROUND_CARNAGE:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_CARNAGE, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_CARNAGE, 10s);
                }
                break;
            case EVENT_UNDERGROUND_ENRAGE:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_ENRAGE, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_ENRAGE, 25s);
                }
                break;
            case EVENT_UNDERGROUND_BLOOD_ARMOR:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_BLOOD_ARMOR, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_BLOOD_ARMOR, 20s);
                }
                break;
            default:
                break;
            }
        }

    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override { return new npc_bloodspike_beastAI(creature); }
};



// === Hellpit Crawler (melee group) UPDATED ===
struct npc_hellpit_crawler : public CreatureScript
{
    npc_hellpit_crawler() : CreatureScript("npc_hellpit_crawler") {}

    struct npc_hellpit_crawlerAI : public ScriptedAI
    {
        npc_hellpit_crawlerAI(Creature* creature) : ScriptedAI(creature)
        {
            baseHealth = creature->GetMaxHealth();
        }

        uint32 baseHealth = 0;
        bool scaledHP = false;

        void Init()
        {
            SetCreatureDifficultyFromNearbyPlayer(me);
            ApplyHealthScaling(me, baseHealth, scaledHP);
            uint8 difficulty = UndergroundState::ReadDifficultyFromCreature(me);
            events.ScheduleEvent(EVENT_UNDERGROUND_WORM_BITE, 8000);
            events.ScheduleEvent(EVENT_UNDERGROUND_ACID_SPLASH, 12000);
            if (difficulty >= 50) events.ScheduleEvent(EVENT_UNDERGROUND_DIGESTIVE_ACID, 9s);
            if (difficulty >= 75) events.ScheduleEvent(EVENT_UNDERGROUND_NESTING_VOMIT, 13s);
        }

        void Reset() override
        {
            events.Reset();
            scaledHP = false;
            Init();
        }
        void JustDied(Unit* killer) override
        {
            giveLoot(killer, me);
        }


        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim()) return;

            

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
            events.Update(diff);
            DoMeleeAttackIfReady();

            float scale = 1.0f + (UndergroundState::ReadDifficultyFromCreature(me) * 0.02f);
            switch (events.ExecuteEvent())
            {
            case EVENT_UNDERGROUND_WORM_BITE:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_WORM_BITE, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_WORM_BITE, 10s);
                }
                break;
            case EVENT_UNDERGROUND_ACID_SPLASH:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_ACID_SPLASH, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_ACID_SPLASH, 12s);
                }
                break;
            case EVENT_UNDERGROUND_DIGESTIVE_ACID:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_DIGESTIVE_ACID, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_DIGESTIVE_ACID, 14s);
                }
                break;
            case EVENT_UNDERGROUND_NESTING_VOMIT:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_NESTING_VOMIT, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_NESTING_VOMIT, 60s);
                }
                break;
            default:
                break;
            }
        }

    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override { return new npc_hellpit_crawlerAI(creature); }
};



// === Rotting Hound (melee group) UPDATED ===
struct npc_rotting_hound : public CreatureScript
{
    npc_rotting_hound() : CreatureScript("npc_rotting_hound") {}

    struct npc_rotting_houndAI : public ScriptedAI
    {
        npc_rotting_houndAI(Creature* creature) : ScriptedAI(creature)
        {
            baseHealth = creature->GetMaxHealth();
        }

        uint32 baseHealth = 0;
        bool scaledHP = false;

        void Init()
        {
            SetCreatureDifficultyFromNearbyPlayer(me);
            ApplyHealthScaling(me, baseHealth, scaledHP);
            uint8 difficulty = UndergroundState::ReadDifficultyFromCreature(me);
            events.ScheduleEvent(EVENT_UNDERGROUND_SAVAGE_POUNCE, 12000);
            events.ScheduleEvent(EVENT_UNDERGROUND_FLESH_BITE, 7000);
            if (difficulty >= 50) events.ScheduleEvent(EVENT_UNDERGROUND_ENRAGE, 11s);
            if (difficulty >= 75) events.ScheduleEvent(EVENT_UNDERGROUND_BLOOD_LEECH, 14s);
        }

        void Reset() override
        {
            events.Reset();
            scaledHP = false;
            Init();
        }

        void JustDied(Unit* killer) override
        {
            giveLoot(killer, me);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim()) return;

            

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
            events.Update(diff);
            DoMeleeAttackIfReady();

            float scale = 1.0f + (UndergroundState::ReadDifficultyFromCreature(me) * 0.02f);
            switch (events.ExecuteEvent())
            {
            case EVENT_UNDERGROUND_SAVAGE_POUNCE:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_SAVAGE_POUNCE, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_SAVAGE_POUNCE, 15s);
                }
                break;
            case EVENT_UNDERGROUND_FLESH_BITE:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_FLESH_BITE, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_FLESH_BITE, 10s);
                }
                break;
            case EVENT_UNDERGROUND_ENRAGE:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_ENRAGE, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_ENRAGE, 12s);
                }
                break;
            case EVENT_UNDERGROUND_BLOOD_LEECH:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_BLOOD_LEECH, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_BLOOD_LEECH, 20s);
                }
                break;
            default:
                break;
            }
        }

    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override { return new npc_rotting_houndAI(creature); }
};


// === Soul-Leech Banshee (caster group) UPDATED ===
struct npc_soul_leech_banshee : public CreatureScript
{
    npc_soul_leech_banshee() : CreatureScript("npc_soul_leech_banshee") {}

    struct npc_soul_leech_bansheeAI : public ScriptedAI
    {
        npc_soul_leech_bansheeAI(Creature* creature) : ScriptedAI(creature)
        {
            baseHealth = creature->GetMaxHealth();
        }

        uint32 baseHealth = 0;
        bool scaledHP = false;

        void Init()
        {
            SetCreatureDifficultyFromNearbyPlayer(me);
            ApplyHealthScaling(me, baseHealth, scaledHP);
            uint8 difficulty = UndergroundState::ReadDifficultyFromCreature(me);
            events.ScheduleEvent(EVENT_UNDERGROUND_SOUL_SPIKE, 8000);
            events.ScheduleEvent(EVENT_UNDERGROUND_DRAIN_LIFE, 12000);
            if (difficulty >= 50) events.ScheduleEvent(EVENT_UNDERGROUND_BANSHEE_WAIL, 12s);
            if (difficulty >= 75) events.ScheduleEvent(EVENT_UNDERGROUND_SWARMING_SHADOWS, 14s);
        }

        void Reset() override
        {
            events.Reset();
            scaledHP = false;
            Init();
        }

        void JustDied(Unit* killer) override
        {
            giveLoot(killer, me);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim()) return;

            

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
            events.Update(diff);
            DoMeleeAttackIfReady();

            float scale = 1.0f + (UndergroundState::ReadDifficultyFromCreature(me) * 0.02f);
            switch (events.ExecuteEvent())
            {
            case EVENT_UNDERGROUND_SOUL_SPIKE:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_SOUL_SPIKE, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_SOUL_SPIKE, 12s);
                }
                break;
            case EVENT_UNDERGROUND_DRAIN_LIFE:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_DRAIN_LIFE, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_DRAIN_LIFE, 14s);
                }
                break;
            case EVENT_UNDERGROUND_BANSHEE_WAIL:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_BANSHEE_WAIL, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_BANSHEE_WAIL, 50s);
                }
                break;
            case EVENT_UNDERGROUND_SWARMING_SHADOWS:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_SWARMING_SHADOWS, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_SWARMING_SHADOWS, 60s);
                }
                break;
            default:
                break;
            }
        }

    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override { return new npc_soul_leech_bansheeAI(creature); }
};


// === Grotesque Brute (melee group) UPDATED ===
struct npc_grotesque_brute : public CreatureScript
{
    npc_grotesque_brute() : CreatureScript("npc_grotesque_brute") {}

    struct npc_grotesque_bruteAI : public ScriptedAI
    {
        npc_grotesque_bruteAI(Creature* creature) : ScriptedAI(creature)
        {
            baseHealth = creature->GetMaxHealth();
        }

        uint32 baseHealth = 0;
        bool scaledHP = false;

        void Init()
        {
            SetCreatureDifficultyFromNearbyPlayer(me);
            ApplyHealthScaling(me, baseHealth, scaledHP);
            uint8 difficulty = UndergroundState::ReadDifficultyFromCreature(me);
            events.ScheduleEvent(EVENT_UNDERGROUND_FLESH_REND, 8000);
            events.ScheduleEvent(EVENT_UNDERGROUND_CARNAGE, 7000);
            if (difficulty >= 50) events.ScheduleEvent(EVENT_UNDERGROUND_ENRAGE, 11s);
            if (difficulty >= 75) events.ScheduleEvent(EVENT_UNDERGROUND_CURSE_OF_WEAKNESS, 14s);
        }

        void Reset() override
        {
            events.Reset();
            scaledHP = false;
            Init();
        }

        void JustDied(Unit* killer) override
        {
            giveLoot(killer, me);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim()) return;

            

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
            events.Update(diff);
            DoMeleeAttackIfReady();

            float scale = 1.0f + (UndergroundState::ReadDifficultyFromCreature(me) * 0.02f);
            switch (events.ExecuteEvent())
            {
            case EVENT_UNDERGROUND_FLESH_REND:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_FLESH_REND, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_FLESH_REND, 12s);
                }
                break;
            case EVENT_UNDERGROUND_CARNAGE:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_CARNAGE, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_CARNAGE, 10s);
                }
                break;
            case EVENT_UNDERGROUND_ENRAGE:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_ENRAGE, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_ENRAGE, 14s);
                }
                break;
            case EVENT_UNDERGROUND_CURSE_OF_WEAKNESS:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_CURSE_OF_WEAKNESS, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_CURSE_OF_WEAKNESS, 60s);
                }
                break;
            default:
                break;
            }
        }

    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override { return new npc_grotesque_bruteAI(creature); }
};


// === Carrion Watcher (ranged group) UPDATED ===
struct npc_carrion_watcher : public CreatureScript
{
    npc_carrion_watcher() : CreatureScript("npc_carrion_watcher") {}

    struct npc_carrion_watcherAI : public ScriptedAI
    {
        npc_carrion_watcherAI(Creature* creature) : ScriptedAI(creature)
        {
            baseHealth = creature->GetMaxHealth();
        }

        uint32 baseHealth = 0;
        bool scaledHP = false;

        void Init()
        {
            SetCreatureDifficultyFromNearbyPlayer(me);
            ApplyHealthScaling(me, baseHealth, scaledHP);
            uint8 difficulty = UndergroundState::ReadDifficultyFromCreature(me);
            events.ScheduleEvent(EVENT_UNDERGROUND_EYE_BEAM, 12000);
            events.ScheduleEvent(EVENT_UNDERGROUND_SHADOW_SHOCK, 6000);
            if (difficulty >= 50) events.ScheduleEvent(EVENT_UNDERGROUND_DRAIN_LIFE, 10s);
            if (difficulty >= 75) events.ScheduleEvent(EVENT_UNDERGROUND_CURSE_OF_IDIOCY, 14s);
        }

        void Reset() override
        {
            events.Reset();
            scaledHP = false;
            Init();
        }

        void JustDied(Unit* killer) override
        {
            giveLoot(killer, me);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim()) return;

            

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
            events.Update(diff);
            DoMeleeAttackIfReady();

            float scale = 1.0f + (UndergroundState::ReadDifficultyFromCreature(me) * 0.02f);
            switch (events.ExecuteEvent())
            {
            case EVENT_UNDERGROUND_EYE_BEAM:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_EYE_BEAM, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_EYE_BEAM, 14s);
                }
                break;
            case EVENT_UNDERGROUND_SHADOW_SHOCK:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_SHADOW_SHOCK, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_SHADOW_SHOCK, 10s);
                }
                break;
            case EVENT_UNDERGROUND_DRAIN_LIFE:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_DRAIN_LIFE, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_DRAIN_LIFE, 12s);
                }
                break;
            case EVENT_UNDERGROUND_CURSE_OF_IDIOCY:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_CURSE_OF_IDIOCY, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_CURSE_OF_IDIOCY, 60s);
                }
                break;
            default:
                break;
            }
        }

    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override { return new npc_carrion_watcherAI(creature); }
};

// === Blightfang (melee group) UPDATED ===
struct npc_blightfang : public CreatureScript
{
    npc_blightfang() : CreatureScript("npc_blightfang") {}

    struct npc_blightfangAI : public ScriptedAI
    {
        npc_blightfangAI(Creature* creature) : ScriptedAI(creature)
        {
            baseHealth = creature->GetMaxHealth();
        }

        uint32 baseHealth = 0;
        bool scaledHP = false;

        void Init()
        {
            SetCreatureDifficultyFromNearbyPlayer(me);
            ApplyHealthScaling(me, baseHealth, scaledHP);
            uint8 difficulty = UndergroundState::ReadDifficultyFromCreature(me);
            events.ScheduleEvent(EVENT_UNDERGROUND_BITE_WOUND, 8000);
            events.ScheduleEvent(EVENT_UNDERGROUND_SHADOW_CLAW, 6000);
            if (difficulty >= 50) events.ScheduleEvent(EVENT_UNDERGROUND_BLOOD_LEECH, 10s);
            if (difficulty >= 75) events.ScheduleEvent(EVENT_UNDERGROUND_CURSE_OF_AGONY, 14s);
        }

        void Reset() override
        {
            events.Reset();
            scaledHP = false;
            Init();
        }

        void JustDied(Unit* killer) override
        {
            giveLoot(killer, me);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim()) return;

            

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
            events.Update(diff);
            DoMeleeAttackIfReady();

            float scale = 1.0f + (UndergroundState::ReadDifficultyFromCreature(me) * 0.02f);
            switch (events.ExecuteEvent())
            {
            case EVENT_UNDERGROUND_BITE_WOUND:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_BITE_WOUND, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_BITE_WOUND, 10s);
                }
                break;
            case EVENT_UNDERGROUND_SHADOW_CLAW:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_SHADOW_CLAW, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_SHADOW_CLAW, 9s);
                }
                break;
            case EVENT_UNDERGROUND_BLOOD_LEECH:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_BLOOD_LEECH, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_BLOOD_LEECH, 20s);
                }
                break;
            case EVENT_UNDERGROUND_CURSE_OF_AGONY:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_CURSE_OF_AGONY, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_CURSE_OF_AGONY, 60s);
                }
                break;
            default:
                break;
            }
        }

    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override { return new npc_blightfangAI(creature); }
};


// === Voidbound Revenant (caster group) UPDATED ===
struct npc_voidbound_revenant : public CreatureScript
{
    npc_voidbound_revenant() : CreatureScript("npc_voidbound_revenant") {}

    struct npc_voidbound_revenantAI : public ScriptedAI
    {
        npc_voidbound_revenantAI(Creature* creature) : ScriptedAI(creature)
        {
            baseHealth = creature->GetMaxHealth();
        }

        uint32 baseHealth = 0;
        bool scaledHP = false;

        void Init() {
            SetCreatureDifficultyFromNearbyPlayer(me);
            ApplyHealthScaling(me, baseHealth, scaledHP);
            uint8 difficulty = UndergroundState::ReadDifficultyFromCreature(me);
            events.ScheduleEvent(EVENT_UNDERGROUND_VOID_ZONE, 12000);
            events.ScheduleEvent(EVENT_UNDERGROUND_DRAIN_LIFE, 15000);
            if (difficulty >= 50) events.ScheduleEvent(EVENT_UNDERGROUND_SHADOW_WORD_PAIN, 9s);
            if (difficulty >= 75) events.ScheduleEvent(EVENT_UNDERGROUND_SOUL_FLAY, 14s);
        }

        void Reset() override
        {
            events.Reset();
            scaledHP = false;
            Init();
        }

        void JustDied(Unit* killer) override
        {
            giveLoot(killer, me);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim()) return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            
            events.Update(diff);
            DoMeleeAttackIfReady();

            float scale = 1.0f + (UndergroundState::ReadDifficultyFromCreature(me) * 0.02f);
            switch (events.ExecuteEvent())
            {
            case EVENT_UNDERGROUND_VOID_ZONE:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_VOID_ZONE, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_VOID_ZONE, 15s);
                }
                break;
            case EVENT_UNDERGROUND_DRAIN_LIFE:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_DRAIN_LIFE, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_DRAIN_LIFE, 10s);
                }
                break;
            case EVENT_UNDERGROUND_SHADOW_WORD_PAIN:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_SHADOW_WORD_PAIN, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_SHADOW_WORD_PAIN, 12s);
                }
                break;
            case EVENT_UNDERGROUND_SOUL_FLAY:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_SOUL_FLAY, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_SOUL_FLAY, 60s);
                }
                break;
            default:
                break;
            }
        }

    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override { return new npc_voidbound_revenantAI(creature); }
};



// === Fleshbound Horror (melee group) UPDATED ===
struct npc_fleshbound_horror : public CreatureScript
{
    npc_fleshbound_horror() : CreatureScript("npc_fleshbound_horror") {}

    struct npc_fleshbound_horrorAI : public ScriptedAI
    {
        npc_fleshbound_horrorAI(Creature* creature) : ScriptedAI(creature)
        {
            baseHealth = creature->GetMaxHealth();
        }

        uint32 baseHealth = 0;
        bool scaledHP = false;

        void Init()
        {
            SetCreatureDifficultyFromNearbyPlayer(me);
            ApplyHealthScaling(me, baseHealth, scaledHP);
            uint8 difficulty = UndergroundState::ReadDifficultyFromCreature(me);
            events.ScheduleEvent(EVENT_UNDERGROUND_SHADOWSTALKER_STAB, 13s);
            events.ScheduleEvent(EVENT_UNDERGROUND_CURSE_OF_IDIOCY, 10s);
            if (difficulty >= 50) events.ScheduleEvent(EVENT_UNDERGROUND_SHADOW_CLAW, 9s);
            if (difficulty >= 75) events.ScheduleEvent(EVENT_UNDERGROUND_MIND_ROT, 16s);
        }

        void Reset() override
        {
            events.Reset();
            scaledHP = false;
            Init();
        }

        void JustDied(Unit* killer) override
        {
            giveLoot(killer, me);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim()) return;

            

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
            events.Update(diff);
            DoMeleeAttackIfReady();

            float scale = 1.0f + (UndergroundState::ReadDifficultyFromCreature(me) * 0.02f);
            switch (events.ExecuteEvent())
            {
            case EVENT_UNDERGROUND_SHADOWSTALKER_STAB:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_SHADOWSTALKER_STAB, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_SHADOWSTALKER_STAB, 14s);
                }
                break;
            case EVENT_UNDERGROUND_CURSE_OF_IDIOCY:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_CURSE_OF_IDIOCY, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_CURSE_OF_IDIOCY, 60s);
                }
                break;
            case EVENT_UNDERGROUND_SHADOW_CLAW:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_SHADOW_CLAW, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_SHADOW_CLAW, 10s);
                }
                break;
            case EVENT_UNDERGROUND_MIND_ROT:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_MIND_ROT, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_MIND_ROT, 60s);
                }
                break;
            default:
                break;
            }
        }

    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override { return new npc_fleshbound_horrorAI(creature); }
};


// === Netherclaw Demon (caster group) UPDATED ===
struct npc_netherclaw_demon : public CreatureScript
{
    npc_netherclaw_demon() : CreatureScript("npc_netherclaw_demon") {}

    struct npc_netherclaw_demonAI : public ScriptedAI
    {
        npc_netherclaw_demonAI(Creature* creature) : ScriptedAI(creature)
        {
            baseHealth = creature->GetMaxHealth();
        }

        uint32 baseHealth = 0;
        bool scaledHP = false;

        void Init()
        {
            SetCreatureDifficultyFromNearbyPlayer(me);
            ApplyHealthScaling(me, baseHealth, scaledHP);
            uint8 difficulty = UndergroundState::ReadDifficultyFromCreature(me);
            events.ScheduleEvent(EVENT_UNDERGROUND_CURSE_OF_DOOM, 4s);
            events.ScheduleEvent(EVENT_UNDERGROUND_CURSE_OF_AGONY, 8s);
            if (difficulty >= 50) events.ScheduleEvent(EVENT_UNDERGROUND_DRAIN_LIFE, 9s);
            if (difficulty >= 75) events.ScheduleEvent(EVENT_UNDERGROUND_CURSE_OF_WEAKNESS, 14s);
        }

        void Reset() override
        {
            events.Reset();
            scaledHP = false;
            Init();
        }

        void JustDied(Unit* killer) override
        {
            giveLoot(killer, me);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim()) return;

            

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
            events.Update(diff);
            DoMeleeAttackIfReady();

            float scale = 1.0f + (UndergroundState::ReadDifficultyFromCreature(me) * 0.02f);
            switch (events.ExecuteEvent())
            {
            case EVENT_UNDERGROUND_CURSE_OF_DOOM:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_CURSE_OF_DOOM, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_CURSE_OF_DOOM, 6s);
                }
                break;
            case EVENT_UNDERGROUND_CURSE_OF_AGONY:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_CURSE_OF_AGONY, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_CURSE_OF_AGONY, 10s);
                }
                break;
            case EVENT_UNDERGROUND_DRAIN_LIFE:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_DRAIN_LIFE, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_DRAIN_LIFE, 12s);
                }
                break;
            case EVENT_UNDERGROUND_CURSE_OF_WEAKNESS:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_CURSE_OF_WEAKNESS, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_CURSE_OF_WEAKNESS, 18s);
                }
                break;
            default:
                break;
            }
        }

    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override { return new npc_netherclaw_demonAI(creature); }
};


// === Putrid Fleshbeast (melee group) UPDATED ===
struct npc_putrid_fleshbeast : public CreatureScript
{
    npc_putrid_fleshbeast() : CreatureScript("npc_putrid_fleshbeast") {}

    struct npc_putrid_fleshbeastAI : public ScriptedAI
    {
        npc_putrid_fleshbeastAI(Creature* creature) : ScriptedAI(creature)
        {
            baseHealth = creature->GetMaxHealth();
        }

        uint32 baseHealth = 0;
        bool scaledHP = false;

        void Init()
        {
            SetCreatureDifficultyFromNearbyPlayer(me);
            ApplyHealthScaling(me, baseHealth, scaledHP);
            uint8 difficulty = UndergroundState::ReadDifficultyFromCreature(me);
            events.ScheduleEvent(EVENT_UNDERGROUND_SHADOW_BOLT, 4000);
            events.ScheduleEvent(EVENT_UNDERGROUND_DRAIN_LIFE, 6000);
            if (difficulty >= 50) events.ScheduleEvent(EVENT_UNDERGROUND_MIND_ROT, 14s);
            if (difficulty >= 75) events.ScheduleEvent(EVENT_UNDERGROUND_SWARMING_SHADOWS, 11s);
        }

        void Reset() override
        {
            events.Reset();
            scaledHP = false;
            Init();
        }

        void JustDied(Unit* killer) override
        {
            giveLoot(killer, me);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim()) return;

            

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
            events.Update(diff);
            DoMeleeAttackIfReady();

            float scale = 1.0f + (UndergroundState::ReadDifficultyFromCreature(me) * 0.02f);
            switch (events.ExecuteEvent())
            {
            case EVENT_UNDERGROUND_SHADOW_BOLT:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_SHADOW_BOLT, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_SHADOW_BOLT, 10s);
                }
                break;
            case EVENT_UNDERGROUND_DRAIN_LIFE:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_DRAIN_LIFE, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_DRAIN_LIFE, 12s);
                }
                break;
            case EVENT_UNDERGROUND_MIND_ROT:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_MIND_ROT, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_MIND_ROT, 60s);
                }
                break;
            case EVENT_UNDERGROUND_SWARMING_SHADOWS:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_SWARMING_SHADOWS, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_SWARMING_SHADOWS, 60s);
                }
                break;
            default:
                break;
            }
        }

    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override { return new npc_putrid_fleshbeastAI(creature); }
};



// === Tormented Soul (caster group) UPDATED ===
struct npc_tormented_soul : public CreatureScript
{
    npc_tormented_soul() : CreatureScript("npc_tormented_soul") {}

    struct npc_tormented_soulAI : public ScriptedAI
    {
        npc_tormented_soulAI(Creature* creature) : ScriptedAI(creature)
        {
            baseHealth = creature->GetMaxHealth();
        }

        uint32 baseHealth = 0;
        bool scaledHP = false;

        void Init()
        {
            SetCreatureDifficultyFromNearbyPlayer(me);
            ApplyHealthScaling(me, baseHealth, scaledHP);
            uint8 difficulty = UndergroundState::ReadDifficultyFromCreature(me);
            events.ScheduleEvent(EVENT_UNDERGROUND_SOUL_FLAY, 8s);
            events.ScheduleEvent(EVENT_UNDERGROUND_FROST_NOVA, 1s);
            if (difficulty >= 50) events.ScheduleEvent(EVENT_UNDERGROUND_CURSE_OF_WEAKNESS, 14s);
            if (difficulty >= 75) events.ScheduleEvent(EVENT_UNDERGROUND_SHADOW_WORD_PAIN, 12s);
        }

        void Reset() override
        {
            events.Reset();
            scaledHP = false;
            Init();
        }

        void JustDied(Unit* killer) override
        {
            giveLoot(killer, me);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim()) return;

            

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            DoMeleeAttackIfReady();
            events.Update(diff);
            float scale = 1.0f + (UndergroundState::ReadDifficultyFromCreature(me) * 0.02f);
            switch (events.ExecuteEvent())
            {
            case EVENT_UNDERGROUND_SOUL_FLAY:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_SOUL_FLAY, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_SOUL_FLAY, 60s);
                }
                break;
            case EVENT_UNDERGROUND_CORRUPTION:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_FROST_NOVA, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_FROST_NOVA, 20s);
                }
                break;
            case EVENT_UNDERGROUND_CURSE_OF_WEAKNESS:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_CURSE_OF_WEAKNESS, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_CURSE_OF_WEAKNESS, 60s);
                }
                break;
            case EVENT_UNDERGROUND_SHADOW_WORD_PAIN:
                if (me->GetVictim() && me->GetVictim()->IsAlive())
                {
                    CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_SHADOW_WORD_PAIN, scale);
                    events.ScheduleEvent(EVENT_UNDERGROUND_SHADOW_WORD_PAIN, 16s);
                }
                break;
            default:
                break;
            }
        }

    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override { return new npc_tormented_soulAI(creature); }
};

// === Abyss Hound (melee group) UPDATED ===
struct npc_abyss_hound : public CreatureScript
{
    npc_abyss_hound() : CreatureScript("npc_abyss_hound") {}

    struct npc_abyss_houndAI : public ScriptedAI
    {
        npc_abyss_houndAI(Creature* creature) : ScriptedAI(creature)
        {
            baseHealth = creature->GetMaxHealth();
        }

        uint32 baseHealth = 0;
        bool scaledHP = false;

        void Init()
        {
            SetCreatureDifficultyFromNearbyPlayer(me);
            ApplyHealthScaling(me, baseHealth, scaledHP);
            uint8 difficulty = UndergroundState::ReadDifficultyFromCreature(me);
            events.ScheduleEvent(EVENT_UNDERGROUND_CURSE_OF_AGONY, 5s);
            events.ScheduleEvent(EVENT_UNDERGROUND_DRAIN_LIFE, 6s);
            if (difficulty >= 50) events.ScheduleEvent(EVENT_UNDERGROUND_SHADOW_BOLT, 11s);
            if (difficulty >= 75) events.ScheduleEvent(EVENT_UNDERGROUND_CURSE_OF_IDIOCY, 15s);
        }

        void Reset() override
        {
            events.Reset();
            scaledHP = false;
            Init();
        }

        void JustDied(Unit* killer) override
        {
            giveLoot(killer, me);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim()) return;

            

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
            events.Update(diff);
            DoMeleeAttackIfReady();

            float scale = 1.0f + (UndergroundState::ReadDifficultyFromCreature(me) * 0.02f);
            switch (events.ExecuteEvent())
            {
            case EVENT_UNDERGROUND_CURSE_OF_AGONY:
                CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_CURSE_OF_AGONY, scale);
                events.ScheduleEvent(EVENT_UNDERGROUND_CURSE_OF_AGONY, 60s);
                break;
            case EVENT_UNDERGROUND_DRAIN_LIFE:
                CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_DRAIN_LIFE, scale);
                events.ScheduleEvent(EVENT_UNDERGROUND_DRAIN_LIFE, 12s);
                break;
            case EVENT_UNDERGROUND_SHADOW_BOLT:
                CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_SHADOW_BOLT, scale);
                events.ScheduleEvent(EVENT_UNDERGROUND_SHADOW_BOLT, 8s);
                break;
            case EVENT_UNDERGROUND_CURSE_OF_IDIOCY:
                CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_CURSE_OF_IDIOCY, scale);
                events.ScheduleEvent(EVENT_UNDERGROUND_CURSE_OF_IDIOCY, 60s);
                break;
            default:
                break;
            }
        }

    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override { return new npc_abyss_houndAI(creature); }
};



// === Shadowbone Stalker (caster group) UPDATED ===
struct npc_shadowbone_stalker : public CreatureScript
{
    npc_shadowbone_stalker() : CreatureScript("npc_shadowbone_stalker") {}

    struct npc_shadowbone_stalkerAI : public ScriptedAI
    {
        npc_shadowbone_stalkerAI(Creature* creature) : ScriptedAI(creature)
        {
            baseHealth = creature->GetMaxHealth();
        }

        uint32 baseHealth = 0;
        bool scaledHP = false;

        void Init()
        {
            SetCreatureDifficultyFromNearbyPlayer(me);
            ApplyHealthScaling(me, baseHealth, scaledHP);
            uint8 difficulty = UndergroundState::ReadDifficultyFromCreature(me);
            events.ScheduleEvent(EVENT_UNDERGROUND_SHADOW_BOLT, 2s);
            events.ScheduleEvent(EVENT_UNDERGROUND_CURSE_OF_DOOM, 14s);
            if (difficulty >= 50) events.ScheduleEvent(EVENT_UNDERGROUND_DRAIN_LIFE, 10s);
            if (difficulty >= 75) events.ScheduleEvent(EVENT_UNDERGROUND_CURSE_OF_AGONY, 6s);
        }

        void Reset() override
        {
            events.Reset();
            scaledHP = false;
            Init();
        }

        void JustDied(Unit* killer) override
        {
            giveLoot(killer, me);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim()) return;

            

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
            events.Update(diff);
            DoMeleeAttackIfReady();

            float scale = 1.0f + (UndergroundState::ReadDifficultyFromCreature(me) * 0.02f);
            switch (events.ExecuteEvent())
            {
            case EVENT_UNDERGROUND_SHADOW_BOLT:
                CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_SHADOW_BOLT, scale);
                events.ScheduleEvent(EVENT_UNDERGROUND_SHADOW_BOLT, 9s);
                break;
            case EVENT_UNDERGROUND_CURSE_OF_DOOM:
                CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_CURSE_OF_DOOM, scale);
                events.ScheduleEvent(EVENT_UNDERGROUND_CURSE_OF_DOOM, 60s);
                break;
            case EVENT_UNDERGROUND_DRAIN_LIFE:
                CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_DRAIN_LIFE, scale);
                events.ScheduleEvent(EVENT_UNDERGROUND_DRAIN_LIFE, 12s);
                break;
            case EVENT_UNDERGROUND_CURSE_OF_AGONY:
                CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_CURSE_OF_AGONY, scale);
                events.ScheduleEvent(EVENT_UNDERGROUND_CURSE_OF_AGONY, 60s);
                break;
            default:
                break;
            }
        }

    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override { return new npc_shadowbone_stalkerAI(creature); }
};


// === Underground Spectrum (caster group) UPDATED ===
struct npc_underground_spectrum : public CreatureScript
{
    npc_underground_spectrum() : CreatureScript("npc_underground_spectrum") {}

    struct npc_underground_spectrumAI : public ScriptedAI
    {
        npc_underground_spectrumAI(Creature* creature) : ScriptedAI(creature)
        {
            baseHealth = creature->GetMaxHealth();
        }

        uint32 baseHealth = 0;
        bool scaledHP = false;

        void Init() {
            SetCreatureDifficultyFromNearbyPlayer(me);
            ApplyHealthScaling(me, baseHealth, scaledHP);
            uint8 difficulty = UndergroundState::ReadDifficultyFromCreature(me);
            events.ScheduleEvent(SPELL_UNDERGROUND_FROSTBOLT, 6000);
            events.ScheduleEvent(SPELL_UNDERGROUND_ICE_LANCE, 8000);
            if (difficulty >= 50) events.ScheduleEvent(SPELL_UNDERGROUND_FROST_NOVA, 1s);
            if (difficulty >= 75) events.ScheduleEvent(SPELL_UNDERGROUND_BLIZZARD, 12s);
        }

        void Reset() override
        {
            events.Reset();
            scaledHP = false;
            Init();
        }

        void JustDied(Unit* killer) override
        {
            giveLoot(killer, me);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim()) return;

            

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            events.Update(diff);

            DoMeleeAttackIfReady();

            uint8 difficulty = UndergroundState::ReadDifficultyFromCreature(me);
            float scale = 1.0f + (difficulty * 0.03f);

            switch (events.ExecuteEvent())
            {
            case EVENT_UNDERGROUND_FROSTBOLT:
                CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_FROSTBOLT, scale);
                events.ScheduleEvent(EVENT_UNDERGROUND_FROSTBOLT, 10s);
                break;
            case EVENT_UNDERGROUND_ICE_LANCE:
                CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_ICE_LANCE, scale);
                events.ScheduleEvent(EVENT_UNDERGROUND_ICE_LANCE, 11s);
                break;
            case EVENT_UNDERGROUND_FROST_NOVA:
                CastScaledSpell(me, me, SPELL_UNDERGROUND_FROST_NOVA, scale);
                events.ScheduleEvent(EVENT_UNDERGROUND_FROST_NOVA, 10s);
                break;
            case EVENT_UNDERGROUND_BLIZZARD:
                CastScaledSpell(me, me, SPELL_UNDERGROUND_BLIZZARD, scale);
                events.ScheduleEvent(EVENT_UNDERGROUND_BLIZZARD, 20s);
                break;
            default:
                break;
            }
        }

    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override { return new npc_underground_spectrumAI(creature); }
};


// === Underground Ghoul (melee group) UPDATED ===
struct npc_underground_ghoul : public CreatureScript
{
    npc_underground_ghoul() : CreatureScript("npc_underground_ghoul") {}

    struct npc_underground_ghoulAI : public ScriptedAI
    {
        npc_underground_ghoulAI(Creature* creature) : ScriptedAI(creature)
        {
            baseHealth = creature->GetMaxHealth();
        }

        uint32 baseHealth = 0;
        bool scaledHP = false;

        void Init() {
            SetCreatureDifficultyFromNearbyPlayer(me);
            ApplyHealthScaling(me, baseHealth, scaledHP);
            uint8 difficulty = UndergroundState::ReadDifficultyFromCreature(me);
            events.ScheduleEvent(EVENT_UNDERGROUND_ACID_SPLASH, 6000);
            events.ScheduleEvent(EVENT_UNDERGROUND_WORM_BITE, 8000);
            if (difficulty >= 50) events.ScheduleEvent(EVENT_UNDERGROUND_DIGESTIVE_ACID, 9s);
            if (difficulty >= 75) events.ScheduleEvent(EVENT_UNDERGROUND_NESTING_VOMIT, 12s);
        }

        void Reset() override
        {
            events.Reset();
            scaledHP = false;
            Init();
        }

        void JustDied(Unit* killer) override
        {
            giveLoot(killer, me);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim()) return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            events.Update(diff);

            DoMeleeAttackIfReady();

            float scale = 1.0f + (UndergroundState::ReadDifficultyFromCreature(me) * 0.02f);

            switch (events.ExecuteEvent())
            {
            case EVENT_UNDERGROUND_CLEAVE:
                CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_CLEAVE, scale);
                events.ScheduleEvent(EVENT_UNDERGROUND_CLEAVE, 8s);
                break;
            case EVENT_UNDERGROUND_INFECTED_BITE:
                CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_INFECTED_BITE, scale);
                events.ScheduleEvent(EVENT_UNDERGROUND_INFECTED_BITE, 10s);
                break;
            case EVENT_UNDERGROUND_ENRAGE:
                DoCast(me, SPELL_UNDERGROUND_ENRAGE);
                events.ScheduleEvent(EVENT_UNDERGROUND_ENRAGE, 15s);
                break;
            case EVENT_UNDERGROUND_STRIKE:
                CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_STRIKE, scale);
                events.ScheduleEvent(EVENT_UNDERGROUND_STRIKE, 11s);
                break;
            default:
                break;
            }
        }

    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override { return new npc_underground_ghoulAI(creature); }
};


// === Disgusting Larva (caster group) UPDATED ===
struct npc_disgusting_larva : public CreatureScript
{
    npc_disgusting_larva() : CreatureScript("npc_disgusting_larva") {}

    struct npc_disgusting_larvaAI : public ScriptedAI
    {
        npc_disgusting_larvaAI(Creature* creature) : ScriptedAI(creature)
        {
            baseHealth = creature->GetMaxHealth();
        }

        uint32 baseHealth = 0;
        bool scaledHP = false;

        void Init()
        {
            SetCreatureDifficultyFromNearbyPlayer(me);
            ApplyHealthScaling(me, baseHealth, scaledHP);
            uint8 difficulty = UndergroundState::ReadDifficultyFromCreature(me);
            events.ScheduleEvent(EVENT_UNDERGROUND_ACID_SPIT, 7s);
            events.ScheduleEvent(EVENT_UNDERGROUND_POISON_CLOUD, 8s);
            if (difficulty >= 50) events.ScheduleEvent(EVENT_UNDERGROUND_TRAMPLE, 10s);
            if (difficulty >= 75) events.ScheduleEvent(EVENT_UNDERGROUND_GORE, 12s);
        }

        void Reset() override
        {
            events.Reset();
            scaledHP = false;
            Init();
        }

        void JustDied(Unit* killer) override
        {
            giveLoot(killer, me);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim()) return;

            

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            events.Update(diff);

            DoMeleeAttackIfReady();

            float scale = 1.0f + (UndergroundState::ReadDifficultyFromCreature(me) * 0.02f);

            switch (events.ExecuteEvent())
            {
            case EVENT_UNDERGROUND_ACID_SPIT:
                CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_ACID_SPIT, scale);
                events.ScheduleEvent(EVENT_UNDERGROUND_ACID_SPIT, 10s);
                break;
            case EVENT_UNDERGROUND_POISON_CLOUD:
                CastScaledSpell(me, me, SPELL_UNDERGROUND_POISON_CLOUD, scale);
                events.ScheduleEvent(EVENT_UNDERGROUND_POISON_CLOUD, 12s);
                break;
            case EVENT_UNDERGROUND_TRAMPLE:
                CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_TRAMPLE, scale);
                events.ScheduleEvent(EVENT_UNDERGROUND_TRAMPLE, 14s);
                break;
            case EVENT_UNDERGROUND_GORE:
                DoCast(me, SPELL_UNDERGROUND_GORE);
                events.ScheduleEvent(EVENT_UNDERGROUND_GORE, 16s);
                break;
            default:
                break;
            }
        }

    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override { return new npc_disgusting_larvaAI(creature); }
};


// === Awakened Bones (melee group) UPDATED ===
struct npc_awakened_bones : public CreatureScript
{
    npc_awakened_bones() : CreatureScript("npc_awakened_bones") {}

    struct npc_awakened_bonesAI : public ScriptedAI
    {
        npc_awakened_bonesAI(Creature* creature) : ScriptedAI(creature)
        {
            baseHealth = creature->GetMaxHealth();
        }

        uint32 baseHealth = 0;
        bool scaledHP = false;

        void Init()
        {
            SetCreatureDifficultyFromNearbyPlayer(me);
            ApplyHealthScaling(me, baseHealth, scaledHP);
            uint8 difficulty = UndergroundState::ReadDifficultyFromCreature(me);
            events.ScheduleEvent(EVENT_UNDERGROUND_BONE_SHIELD, 3s);
            events.ScheduleEvent(EVENT_UNDERGROUND_SUNDER_ARMOR, 6s);
            if (difficulty >= 50) events.ScheduleEvent(EVENT_UNDERGROUND_DEATH_COIL, 2s);
            if (difficulty >= 75) events.ScheduleEvent(EVENT_UNDERGROUND_SHADOW_BOLT, 11s);
        }

        void Reset() override
        {
            events.Reset();
            scaledHP = false;
            Init();
        }

        void JustDied(Unit* killer) override
        {
            giveLoot(killer, me);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim()) return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
            events.Update(diff);
            DoMeleeAttackIfReady();

            float scale = 1.0f + (UndergroundState::ReadDifficultyFromCreature(me) * 0.02f);

            switch (events.ExecuteEvent())
            {
            case EVENT_UNDERGROUND_BONE_SHIELD:
                DoCast(me, SPELL_UNDERGROUND_BONE_SHIELD);
                events.ScheduleEvent(EVENT_UNDERGROUND_BONE_SHIELD, 20s);
                break;
            case EVENT_UNDERGROUND_SUNDER_ARMOR:
                CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_SUNDER_ARMOR, scale);
                events.ScheduleEvent(EVENT_UNDERGROUND_SUNDER_ARMOR, 8s);
                break;
            case EVENT_UNDERGROUND_DEATH_COIL:
                CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_DEATH_COIL, scale);
                events.ScheduleEvent(EVENT_UNDERGROUND_DEATH_COIL, 60s);
                break;
            case EVENT_UNDERGROUND_SHADOW_BOLT:
                CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_SHADOW_BOLT, scale);
                events.ScheduleEvent(EVENT_UNDERGROUND_SHADOW_BOLT, 9s);
                break;
            default:
                break;
            }
        }

    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override { return new npc_awakened_bonesAI(creature); }
};


// === Underground Occultist (caster group) UPDATED ===
struct npc_underground_occultist : public CreatureScript
{
    npc_underground_occultist() : CreatureScript("npc_underground_occultist") {}

    struct npc_underground_occultistAI : public ScriptedAI
    {
        npc_underground_occultistAI(Creature* creature) : ScriptedAI(creature)
        {
            baseHealth = creature->GetMaxHealth();
        }

        uint32 baseHealth = 0;
        bool scaledHP = false;

        void Init()
        {
            SetCreatureDifficultyFromNearbyPlayer(me);
            ApplyHealthScaling(me, baseHealth, scaledHP);
            uint8 difficulty = UndergroundState::ReadDifficultyFromCreature(me);
            events.ScheduleEvent(EVENT_UNDERGROUND_SHADOW_BOLT, 4s);
            events.ScheduleEvent(EVENT_UNDERGROUND_DEATH_COIL, 2s);
            if (difficulty >= 50) events.ScheduleEvent(EVENT_UNDERGROUND_SHADOWFLAME, 10s);
            if (difficulty >= 75) events.ScheduleEvent(EVENT_UNDERGROUND_FEAR, 14s);
        }

        void Reset() override
        {
            events.Reset();
            scaledHP = false;
            Init();
        }

        void JustDied(Unit* killer) override
        {
            giveLoot(killer, me);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim()) return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            events.Update(diff);

            DoMeleeAttackIfReady();

            float scale = 1.0f + (UndergroundState::ReadDifficultyFromCreature(me) * 0.02f);

            switch (events.ExecuteEvent())
            {
            case EVENT_UNDERGROUND_SHADOW_BOLT:
                CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_SHADOW_BOLT, scale);
                events.ScheduleEvent(EVENT_UNDERGROUND_SHADOW_BOLT, 8s);
                break;
            case EVENT_UNDERGROUND_DEATH_COIL:
                CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_DEATH_COIL, scale);
                events.ScheduleEvent(EVENT_UNDERGROUND_DEATH_COIL, 70s);
                break;
            case EVENT_UNDERGROUND_SHADOWFLAME:
                CastScaledSpell(me, me->GetVictim(), SPELL_UNDERGROUND_SHADOWFLAME, scale);
                events.ScheduleEvent(EVENT_UNDERGROUND_SHADOWFLAME, 12s);
                break;
            case EVENT_UNDERGROUND_FEAR:
                DoCast(me->GetVictim(), SPELL_UNDERGROUND_FEAR);
                events.ScheduleEvent(EVENT_UNDERGROUND_FEAR, 60s);
                break;
            default:
                break;
            }
        }

    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override { return new npc_underground_occultistAI(creature); }
};


void AddMyCreatureScripts()
{
    new npc_underground_spectrum();
    new npc_underground_ghoul();
    new npc_disgusting_larva();
    new npc_awakened_bones();
    new npc_underground_occultist();
    new npc_shadowbone_stalker();
    new npc_abyss_hound();
    new npc_tormented_soul();
    new npc_putrid_fleshbeast();
    new npc_netherclaw_demon();
    new npc_fleshbound_horror();
    new npc_voidbound_revenant();
    new npc_blightfang();
    new npc_carrion_watcher();
    new npc_grotesque_brute();
    new npc_soul_leech_banshee();
    new npc_rotting_hound();
    new npc_hellpit_crawler();
    new npc_bloodspike_beast();
    new npc_warped_bonefiend();
    new npc_ashen_wailer();
    new npc_twisted_abomination();
    new npc_gloomfang();
    new npc_crypt_howler();
    new npc_dread_maggot();

}
