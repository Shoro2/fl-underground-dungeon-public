# fl-underground-dungeon

Ein AzerothCore-Modul für ein **Solo-Dungeon-Erlebnis** mit skalierbarer Schwierigkeit (1–100), zufälligen Trash-Spawns und wechselnden Bossen. Spieler starten den Run über einen NPC, kämpfen sich durch dynamisch gespawnte Gegner und erhalten skalierte Belohnungen – inklusive Log der Laufzeit in der Charakterdatenbank.

## Inhaltsverzeichnis
- [Features](#features)
- [Voraussetzungen](#voraussetzungen)
- [Installation](#installation)
- [Konfiguration](#konfiguration)
- [Gameplay-Flow](#gameplay-flow)
- [Belohnungen](#belohnungen)
- [Datenbank](#datenbank)
- [Projektstruktur](#projektstruktur)
- [Entwicklung & Erweiterung](#entwicklung--erweiterung)
- [Lizenz](#lizenz)

## Features
- **Solo-Dungeon mit freier Schwierigkeit (1–100)** über NPC-Gossip.
- **Dynamische Gegnerauswahl**: zufällige Nahkämpfer, Caster und ein zufälliger Boss pro Run.
- **Skalierung von Lebenspunkten & Schaden** anhand der gewählten Schwierigkeit.
- **Run-Tracking**: Startzeit, Abbruch-Handling, Abschluss-Log in der DB.
- **Belohnungs-Logik** mit skalierenden Items und seltenen Drops.
- **Failsafes** bei Tod, Logout oder Gruppenbeitritt (Run wird abgebrochen).

## Voraussetzungen
- **AzerothCore (WotLK)** mit aktiviertem C++-Modul-Workflow.
- Schreibzugriff auf die Charakterdatenbank, falls das Run-Logging genutzt werden soll.

## Installation
1. Modul in das AzerothCore-Modulverzeichnis legen:
   ```bash
   cd /path/to/azerothcore
   git clone <repo-url> modules/fl-underground-dungeon
   ```
2. AzerothCore neu generieren und bauen:
   ```bash
   mkdir -p build && cd build
   cmake ..
   make -j$(nproc)
   ```
3. Worldserver starten.

## Konfiguration
Das Modul bringt eine Beispiel-Konfiguration mit:
- `conf/fl-underground-dungeon.conf.dist`

Aktuell ist dort nur ein Beispielschalter enthalten. Du kannst sie als Vorlage nutzen:
```bash
cp conf/fl-underground-dungeon.conf.dist /path/to/your/worldserver.conf.d/fl-underground-dungeon.conf
```

## Gameplay-Flow
1. Spieler interagiert mit dem **Dungeon-Starter-NPC (Entry: 86000)**.
2. Schwierigkeit wird über einen Gossip-Dialog (1–100) gesetzt.
3. Spieler wird in die Instanz **Map 33** teleportiert.
4. Gegner werden zufällig gespawnt, Difficulty wird auf Kreaturen gespeichert.
5. Boss-Kill beendet den Run; Loot & Paragon-XP werden vergeben; Run wird geloggt.
6. Bei Tod, Logout, Gruppenbeitritt oder Verlassen der Instanz wird der Run abgebrochen.

## Belohnungen
- **Trash-Mobs**: Badge- und Materialdrops abhängig von Schwierigkeit.
- **Boss**: Paragon-XP, Explorer-Badges, seltene Emblems, Boxen, Materialien, Mount-Chancen.
- Loot wird bei Bedarf per **Ingame-Mail** zugestellt, wenn keine Taschenplätze frei sind.

## Datenbank
Der Boss-Run wird in `dungeon_runs` gespeichert. Lege die Tabelle in der **Character-DB** an:

```sql
CREATE TABLE IF NOT EXISTS dungeon_runs (
  guid INT UNSIGNED NOT NULL,
  name VARCHAR(32) NOT NULL,
  difficulty TINYINT UNSIGNED NOT NULL,
  duration_ms INT UNSIGNED NOT NULL,
  created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
```

> Hinweis: Die Insert-Query nutzt aktuell genau die Spalten `guid`, `name`, `difficulty`, `duration_ms`.

## Projektstruktur
- `src/UndergroundInstance.*` – Instanz-Logik & Spawn-Verwaltung.
- `src/UndergroundCreature.cpp` – Trash- und Add-AI inkl. skalierter Spells.
- `src/UndergroundBoss.cpp` – Boss-Mechaniken, Loot und Run-Logging.
- `src/UndergroundPlayer.cpp` – Start-NPC, Run-Management, Abbruch-Handling.
- `src/UndergroundState.h` – Difficulty-Tracking & Run-State.
- `src/UndergroundUtils.h` – Skalierungs- & Loot-Helferfunktionen.
- `src/MP_loader.cpp` – Modullader für AzerothCore.

## Entwicklung & Erweiterung
- Neue Boss- oder Trash-Mobs können über zusätzliche **CreatureScripts** ergänzt werden.
- Zusätzliche Spells lassen sich in `UndergroundCreature.cpp` und `UndergroundBoss.cpp` definieren.
- Die Spawnpunkte und Entry-Listen findest du in `UndergroundInstance.cpp`.

## Lizenz
Dieses Projekt nutzt die **GNU AGPL v3** Lizenz wie AzerothCore.
