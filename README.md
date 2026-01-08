# fl-underground-dungeon

An AzerothCore module that delivers a **solo dungeon experience** with scalable difficulty (1–100), randomized trash spawns, and rotating bosses. Players start the run via an NPC, fight through dynamically spawned enemies, and earn scaled rewards—plus a run-time log in the character database.

## Table of Contents
- [Features](#features)
- [Requirements](#requirements)
- [Installation](#installation)
- [Configuration](#configuration)
- [Gameplay Flow](#gameplay-flow)
- [Rewards](#rewards)
- [Database](#database)
- [Project Structure](#project-structure)
- [Development & Extension](#development--extension)
- [License](#license)

## Features
- **Solo dungeon with free difficulty (1–100)** via NPC gossip.
- **Dynamic enemy selection**: random melee, casters, and a random boss per run.
- **Health & damage scaling** based on the chosen difficulty.
- **Run tracking**: start time, abort handling, completion log in the DB.
- **Reward logic** with scaling items and rare drops.
- **Failsafes** for death, logout, or group join (run is aborted).

## Requirements
- **AzerothCore (WotLK)** with C++ module workflow enabled.
- Write access to the character database if run logging is used.

## Installation
1. Place the module in your AzerothCore modules directory:
   ```bash
   cd /path/to/azerothcore
   git clone <repo-url> modules/fl-underground-dungeon
   ```
2. Re-generate and build AzerothCore:
   ```bash
   mkdir -p build && cd build
   cmake ..
   make -j$(nproc)
   ```
3. Start the worldserver.

## Configuration
The module ships with a sample configuration file:
- `conf/fl-underground-dungeon.conf.dist`

You can copy it as a starting point:
```bash
cp conf/fl-underground-dungeon.conf.dist /path/to/your/worldserver.conf.d/fl-underground-dungeon.conf
```

## Gameplay Flow
1. Player interacts with the **Dungeon Starter NPC (Entry: 86000)**.
2. Difficulty is set via a gossip dialog (1–100).
3. Player is teleported to the instance **Map 33**.
4. Enemies are spawned randomly and difficulty is stored on creatures.
5. Boss kill ends the run; loot & Paragon XP are awarded; run is logged.
6. Death, logout, grouping, or leaving the instance aborts the run.

## Rewards
- **Trash mobs**: badge and material drops based on difficulty.
- **Boss**: Paragon XP, Explorer Badges, rare emblems, boxes, materials, mount chances.
- Loot is mailed if bags are full.

## Database
The boss run is stored in `dungeon_runs`. Create the table in the **Character DB**:

```sql
CREATE TABLE IF NOT EXISTS dungeon_runs (
  guid INT UNSIGNED NOT NULL,
  name VARCHAR(32) NOT NULL,
  difficulty TINYINT UNSIGNED NOT NULL,
  duration_ms INT UNSIGNED NOT NULL,
  created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
```

> Note: The insert query currently uses the columns `guid`, `name`, `difficulty`, `duration_ms`.

## Project Structure
- `src/UndergroundInstance.*` – Instance logic & spawn management.
- `src/UndergroundCreature.cpp` – Trash/add AI including scaled spells.
- `src/UndergroundBoss.cpp` – Boss mechanics, loot, and run logging.
- `src/UndergroundPlayer.cpp` – Starter NPC, run management, abort handling.
- `src/UndergroundState.h` – Difficulty tracking & run state.
- `src/UndergroundUtils.h` – Scaling & loot helper functions.
- `src/MP_loader.cpp` – AzerothCore module loader.

## Development & Extension
- Add new bosses or trash mobs via additional **CreatureScripts**.
- Add new spells in `UndergroundCreature.cpp` and `UndergroundBoss.cpp`.
- Spawn points and entry lists live in `UndergroundInstance.cpp`.

## License
This project uses the **GNU AGPL v3** license, like AzerothCore.
