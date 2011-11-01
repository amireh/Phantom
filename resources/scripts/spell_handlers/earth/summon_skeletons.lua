local Skeleton = { Soldier = nil, Mage = nil, Acolyte = nil, Warrior = nil}

local create = function(inCaster, inTarget, inSpell, inName)
	Pixy.Log("I'm summoning a Skeleton: " .. inName)-- .. " - SPELL COST: " .. inSpell:getCostWP() .. ":" .. inSpell:getCostHP() .. ":" .. inSpell:getCostChannels())
  --tolua.cast(inCaster, "Pixy::Puppet")

	-- create the unit and broadcast it to the players
  local unit = Instance:create_unit("Skeleton " .. inName, inCaster)

  if (not unit) then return false end

	return true
end

local Skeleton_Soldier = {}
function Skeleton_Soldier:cast()
  return create(self.Caster, self.Target, self.Spell, "Soldier")
end
local Skeleton_Ravager = {}
function Skeleton_Ravager:cast()
  return create(self.Caster, self.Target, self.Spell, "Ravager")
end
local Skeleton_Acolyte = {}
function Skeleton_Acolyte:cast()
  return create(self.Caster, self.Target, self.Spell, "Acolyte")
end

local Skeleton_Warlord = {}
function Skeleton_Warlord:cast()
  return create(self.Caster, self.Target, self.Spell, "Warlord")
end

subscribe_spell("Summon: Skeleton Soldier", Skeleton_Soldier)
subscribe_spell("Summon: Skeleton Ravager", Skeleton_Ravager)
subscribe_spell("Summon: Skeleton Acolyte", Skeleton_Acolyte)
subscribe_spell("Summon: Skeleton Warlord", Skeleton_Warlord)
