# Functions

How this snapshot (`c911685`, 2026-07-11) works. The private module has replaced most of it since
(data-driven AIs, run manager, config, AIO window).

## Run flow

1. **Start** (`npc_dungeon_starter`, NPC 86000): gossip "Begin Challenge" with a code box; the typed value must
   be a number 1-100 and the player must not be in a group. `UndergroundState::SetDifficulty` and
   `SetStartTime` store it on the player, then `TeleportTo(741, …)`.
2. **Spawn** (`instance_underground::OnPlayerEnter`): once per instance, every spawn point gets random entries
   from a melee list and a caster list, then one boss is picked at random from 84288-84290. Nothing spawns when
   the player carries no difficulty.
3. **Scaling** (`UndergroundUtils.h`): `ApplyHealthScaling` multiplies health by `1 + difficulty * 0.05`;
   `CastScaledSpell` rescales effect 0 of a spell by the difficulty factor; creatures read the difficulty from a
   nearby player (`SetCreatureDifficultyFromNearbyPlayer`).
4. **Boss kill** (`UndergroundBoss.cpp`): Paragon XP `difficulty * 8` through mod-paragon's
   `IncreaseParagonXP`, Explorer Badges 251100 (`difficulty / 10 + 1`, clamped 2-11), chance drops by difficulty
   band (for example Expert Emblem 251000 from difficulty 80); items go by mail when the bags are full
   (`MailItemOrGive`). The run is marked completed and inserted into `dungeon_runs`; a `TeleportOutEvent` on the
   player's event queue moves them to map 727 after 10 s, and that map change finishes the run (unbind, message).
5. **Abort** (`fl_underground_player_script`): death, logout, joining a group or leaving map 741 ends the run,
   teleports to map 727 and unbinds the instance.

## Hooks

| Script | Hooks |
|---|---|
| `npc_dungeon_starter` (CreatureScript) | `OnGossipHello`, `OnGossipSelectCode` |
| `fl_underground_player_script` (PlayerScript) | `OnPlayerMapChanged`, `OnPlayerJustDied`, `OnPlayerUpdate` (group check), `OnPlayerLogout` |
| `instance_underground` (InstanceMapScript, map 741) | `OnPlayerEnter` (spawning) |
| `boss_dralak`, `boss_vampir_lord`, `boss_crypt_lord` | boss AIs; aura 100027 on the boss while its adds live |
| trash AIs (`npc_underground_*` and others) | timed abilities from 800600-800671 cast through `CastScaledSpell` |

## State

`UndergroundState.h` keeps the difficulty in `UNIT_FIELD_BYTES_2` byte 2 on the player and in
`UNIT_FIELD_BYTES_1` byte 2 on creatures. The private module has since moved the player's run state into a run
manager; creatures still carry the difficulty byte there. No config keys are read; spawn points and entry lists are
constants in `UndergroundInstance.cpp`.
