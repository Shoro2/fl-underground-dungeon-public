# Data structure

State of `master` (`c911685`, 2026-07-11).

| Path | What |
|---|---|
| `src/MP_loader.cpp` | `Addfl_underground_dungeonScripts()`, calls the four `AddSC_*` registrars |
| `src/UndergroundPlayer.cpp` | starter NPC gossip `npc_dungeon_starter`, `fl_underground_player_script` (abort and completion) |
| `src/UndergroundInstance.cpp`, `.h` | `instance_underground` (map 741): spawn points, trash lists, the random boss |
| `src/UndergroundCreature.cpp` | the trash AIs, their scaled spells and drops |
| `src/UndergroundBoss.cpp` | the three boss AIs, boss loot, Paragon XP, the `dungeon_runs` insert |
| `src/UndergroundState.h` | difficulty and run state kept in unit byte fields |
| `src/UndergroundUtils.h` | scaling, scaled spell casts, bag-space check, mail-or-give |
| `data/sql/db-world/base/underground_base.sql` | binds `instance_underground` to map 741 in `instance_template` |
| `conf/fl-underground-dungeon.conf.dist` | the AzerothCore module template's `MyModule.Enable`; not read by the code |
| `README.md` | feature description; also the only `dungeon_runs` schema |
| `LICENSE` | MIT text from the AzerothCore module template (the README says AGPL v3) |
| `.github/` | the module template's issue forms, READMEs and CI workflows (`core-build.yml`, `core_codestyle.yml`) |
| `apps/ci/ci-codestyle.sh`, `include.sh` (empty), `pull_request_template.md`, `.editorconfig`, `.gitattributes`, `.gitignore` | module template files |

## Database

| Table | DB | Written by |
|---|---|---|
| `dungeon_runs` (`guid`, `name`, `difficulty`, `duration_ms`, `created_at`) | characters | `UndergroundBoss.cpp` at a boss kill; create it from the README |
| `instance_template` row 741 | world | `underground_base.sql` |

Creature templates 84263-84290, the spells 800600-800671, aura 100027 and gossip menu 90099 are expected in
the Forgotten Land world database; this repo ships none of them.
