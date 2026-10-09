# fl-underground-dungeon-public

Public copy of Forgotten Land's solo dungeon module "Azealia Under" (map **741**): difficulty 1-100 chosen
at a starter NPC, random trash, one of three bosses, scaled rewards, a run log. The maintained module is
the private `Shoro2/fl-underground-dungeon`; this copy shares its history up to `c911685` (2026-07-11) and
has received nothing since its last push on 2026-07-12. What the evidence says it was for: a public
mirror of the module that carried the same commits as the private repo until then (no stars, forks or
watchers on GitHub). Description of this snapshot: [README.md](README.md).

## Ids and tables (this snapshot)

| What | Id / name |
|---|---|
| Instance map | 741, `instance_template.script = 'instance_underground'` (`data/sql/db-world/base/underground_base.sql`); exit to map 727 |
| Starter NPC | 86000 with the C++ gossip `npc_dungeon_starter` ("Begin Challenge", difficulty typed into the gossip box, menu 90099) |
| C++ scripts | `instance_underground`, `fl_underground_player_script`, bosses `boss_dralak`, `boss_vampir_lord`, `boss_crypt_lord`, the trash AIs in `UndergroundCreature.cpp` |
| Creatures | trash 84263-84287, bosses 84288-84290 (rows come from the FL world database, not from this repo) |
| Spells, auras | trash/boss spells 800600-800671, boss aura 100027 |
| Table | `dungeon_runs` (characters; the schema is only in the README, no SQL file) |
| Dependency | mod-paragon (`ParagonUtils.h`, `IncreaseParagonXP`), a hard include here |
| Config | none read: `conf/fl-underground-dungeon.conf.dist` is the module template's `MyModule.Enable` |

## Status and progress

- **Where it runs**: nowhere. Forgotten Land's servers build the private module; their checkouts carry
  commits made after this copy stopped.
- **Evidence**: this state is the one the in-game module test of 2026-07-13 passed (T2: NPC 86000, map 741,
  exit to 727; vault `forgotten-land/06-fl-modules.md`). Nothing has been tested here since.
- **Done in this copy**: the port to the current AzerothCore API, real 3.3.5 spells for the trash, unique
  loader entry points, boss spell / exit teleport / completed-flag fixes (2026-07-09..11), an English README.
- **Missing against the private module** (2026-10-09): data-driven AIs with a run manager and the
  `Underground.*` config, the AIO window (`/ud`) with history and leaderboard, the characters base SQL
  (`dungeon_run_state`, `dungeon_runs`), the optional mod-paragon include, the solo-run flag on the Player, the
  armour box for the 21 CoA classes, the Dungeon Guide opening the window, the per-repo docs.

## Next steps

1. Decide the repo's future (the owner's call): (a) **sync** it to the private `master` and keep it in step, (b)
   **archive** it on GitHub after a README banner saying it is frozen at 2026-07-11, or (c) **delete** it.
   Recommendation (suggestion): (b). Nothing consumes it; a sync would add a standing chore and would publish the
   private docs' internal workbench and host paths unless they are scrubbed first; archiving keeps the public
   history without implying it is current.
2. Whatever is chosen: the vault's `forgotten-land/11-linux-host-deployment.md` step 2 still says to clone this
   repo as `fl-underground-dungeon`; point it at the private repo, which its own trap table already names.
3. If the repo stays visible: fix the README (it says map 33 while its code uses 741, and AGPL v3 while
   `LICENSE` is the template's MIT text).

Open points in full: [todo.md](todo.md).

## Working here

- Public repository: anything committed here is world-readable. Keep host addresses, credentials and internal
  paths out.
- No development here. Module work happens in the private `Shoro2/fl-underground-dungeon` (branch
  `claude/<topic>-<sessionId>`, merged into its `master` and pushed, no pull requests); a sync would copy from there.
- Build check, if ever needed: clone into an AzerothCore `modules/` folder named `fl-underground-dungeon` (the
  generated `Addfl_underground_dungeonScripts()` must match the folder name) next to mod-paragon, then
  re-run cmake.
- Vault (share-public `docs/World of Warcraft/`): `forgotten-land/06-fl-modules.md`,
  `forgotten-land/11-linux-host-deployment.md`; the private module's own docs.
- Doc set: INDEX.md, CLAUDE.md, data_structure.md, functions.md, log.md (newest first), todo.md.
