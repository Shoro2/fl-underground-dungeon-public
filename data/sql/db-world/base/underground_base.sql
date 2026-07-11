-- Bind the Underground instance script to map 741 (Azealia Under).
-- The map row itself is created by the Forgotten Land world migration;
-- this only attaches the module's InstanceMapScript so ScriptName-based
-- boss/creature bindings inside the instance resolve their instance data.
INSERT INTO `instance_template` (`map`, `parent`, `script`, `allowMount`) VALUES (741, 0, 'instance_underground', 1)
ON DUPLICATE KEY UPDATE `script` = 'instance_underground';
