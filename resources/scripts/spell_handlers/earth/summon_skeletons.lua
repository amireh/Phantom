local Skeleton = { Soldier = nil, Mage = nil, Acolyte = nil, Warrior = nil}

local create = function(inCaster, inTarget, inSpell, inName)
	Pixy.Log("I'm summoning a Skeleton: " .. inName)-- .. " - SPELL COST: " .. inSpell:getCostWP() .. ":" .. inSpell:getCostHP() .. ":" .. inSpell:getCostChannels())
  tolua.cast(inCaster, "Pixy::Puppet")

	-- create the unit and broadcast it to the players
  local unit = Instance:create_unit("Skeleton " .. inName, inCaster)

  if (not unit) then return false end

	return true
end


Skeleton.Soldier = function(inCaster, inTarget, inSpell)
  return create(inCaster, inTarget, inSpell, "Soldier")
end
Skeleton.Ravager = function(inCaster, inTarget, inSpell)
  return create(inCaster, inTarget, inSpell, "Ravager")
end
Skeleton.Acolyte = function(inCaster, inTarget, inSpell)
  return create(inCaster, inTarget, inSpell, "Acolyte")
end
Skeleton.Warlord = function(inCaster, inTarget, inSpell)
  return create(inCaster, inTarget, inSpell, "Warlord")
end

subscribe_spell("Summon: Skeleton Soldier", Skeleton.Soldier)
subscribe_spell("Summon: Skeleton Ravager", Skeleton.Ravager)
subscribe_spell("Summon: Skeleton Acolyte", Skeleton.Acolyte)
subscribe_spell("Summon: Skeleton Warlord", Skeleton.Warlord)
