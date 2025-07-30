#ifndef UNDERGROUND_UTILS_H
#define UNDERGROUND_UTILS_H

#include "Creature.h"
#include "Player.h"
#include "SpellMgr.h"
#include "SpellInfo.h"
#include "Chat.h"
#include "UndergroundState.h"
#include "Mail.h"
#include "Item.h"
#include "DatabaseEnv.h"

// Assigns difficulty to creature from internal storage or nearest player
inline void SetCreatureDifficultyFromNearbyPlayer(Creature* creature)
{
    uint8 diff = UndergroundState::ReadDifficultyFromCreature(creature);

    // If no difficulty set yet, try getting it from a nearby player
    if (diff == 0)
    {
        if (Unit* unit = creature->SelectNearestPlayer(200.0f))
        {
            Player* player = unit->ToPlayer();
            if (player) //  && player->IsAlive()
            {
                if (!player->IsAlive())
                    return;
                diff = UndergroundState::GetDifficulty(player);
                UndergroundState::StoreDifficultyInCreature(creature, diff);
            }
               
        }
    }


}

// Casts a spell with scaled damage based on difficulty multiplier
inline void CastScaledSpell(Creature* caster, Unit* target, uint32 spellId, float scaleFactor)
{
    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
    /*
    if (target->ToPlayer()) {
        ChatHandler(target->ToPlayer()->GetSession()).PSendSysMessage("[DEBUG] CastScaledSpell called with ID: %u (%s)", spellId, spellInfo ? spellInfo->SpellName[0] : "INVALID");
    }
    */
    if (!spellInfo || !caster || !target)
        return;
    if (!target->IsAlive())
        return;
    if (target == caster || int32(spellInfo->Effects[EFFECT_0].CalcValue()) == 0) {
        caster->CastSpell(target, spellId, false);
        if (caster->GetVictim())
            if (Player* player = caster->GetVictim()->ToPlayer())
                //ChatHandler(player->GetSession()).PSendSysMessage("Non scaled spell cast");
                return;
    }
    
    int32 damage = int32(spellInfo->Effects[EFFECT_0].CalcValue() * scaleFactor);
    if (damage == 0)
        return;
    caster->CastCustomSpell(target, spellId, &damage, nullptr, nullptr, false);

    // Optional: notify player about scaling
    /*
    if (caster->GetVictim())
        if (Player* player = caster->GetVictim()->ToPlayer())
            ChatHandler(player->GetSession()).PSendSysMessage("Scaling to difficulty: %u– Spell : %s", uint32((scaleFactor - 1.0f) * 100), spellInfo->SpellName[0]);
            */
}

// Scales creature health once based on stored difficulty
inline void ApplyHealthScaling(Creature* creature, uint32 baseHealth, bool& scaledFlag)
{
    
    if (scaledFlag) return;
        

    //LOG_DEBUG("module", "Apply Health Start");

    uint8 difficulty = UndergroundState::ReadDifficultyFromCreature(creature);

    //LOG_DEBUG("module", "Apply Health diff: '{}'", difficulty);

    if (difficulty == 0)
    {
        if (Unit* unit = creature->SelectNearestPlayer(500.0f))
        {
            
            if (Player* player = unit->ToPlayer()) //   && player->IsAlive()
            {
                //LOG_DEBUG("module", "Apply Health get diff from player");
                difficulty = UndergroundState::GetDifficulty(player);
                //LOG_DEBUG("module", "Apply Health store diff in creature: '{}'", difficulty);
                UndergroundState::StoreDifficultyInCreature(creature, difficulty);
                //LOG_DEBUG("module", "Apply Health done store diff in creature");
            }
        }
    }
    if (difficulty == 0)
    {
        //LOG_DEBUG("module", "Apply Health diff = 0 return");
        return;
    }
    float scale = 1.0f + (difficulty * 0.05f);
    creature->SetMaxHealth(uint32(baseHealth * scale));
    creature->SetHealth(creature->GetMaxHealth());
    scaledFlag = true;


    //LOG_DEBUG("module", "Apply Health done");

}

inline bool HasFreeBagSlots(Player* player, uint32 needed)
{
    uint32 freeSlots = 0;
    for (uint8 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; ++i)
    {
        if (Bag* bag = player->GetBagByPos(i))
            freeSlots += bag->GetFreeSlots();
        else
            ++freeSlots;
    }
    return freeSlots >= needed;
}

inline void MailItemOrGive(Player* player, uint32 itemId, uint32 count, std::string const& subject, Creature* sender)
{
    if (!player || !itemId)
        return;
    if (HasFreeBagSlots(player, 3))
    {
        player->AddItem(itemId, count);
    }
    else
    {
        MailDraft draft(subject, "");
        if (Item* item = Item::CreateItem(itemId, count, player))
        {
            draft.AddItem(item);
        }

        CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();
        draft.SendMailTo(
            trans,
            MailReceiver(player),
            MailSender(MAIL_CREATURE, sender->GetEntry()),
            MAIL_CHECK_MASK_COPIED
        );
        CharacterDatabase.CommitTransaction(trans);

        ChatHandler(player->GetSession()).PSendSysMessage("[Loot] Your bag is full. Item %u was mailed.", itemId);
    }
}


#endif // UNDERGROUND_UTILS_H
