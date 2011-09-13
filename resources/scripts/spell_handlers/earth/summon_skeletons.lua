local Skeleton = {
  Footsoldier = nil,
  Mage = nil,
  Acolyte = nil,
  Warrior = nil
}

local create = function(inCaster, inTarget, inSpell, inName)
	Pixy.Log("I'm summoning a Skeleton: " .. inName .. " - SPELL COST: " .. inSpell:getCostWP() .. ":" .. inSpell:getCostHP() .. ":" .. inSpell:getCostChannels())
  tolua.cast(inCaster, "Pixy::Puppet")

	-- create the unit and broadcast it to the players
  local unit = Instance:create_unit("Skeleton " .. inName, inCaster)

  if (not unit) then return false end

	return true
end

Skeleton.Footsoldier = function(inCaster, inTarget, inSpell)
  return create(inCaster, inTarget, inSpell, "Footsoldier")
end
Skeleton.Mage = function(inCaster, inTarget, inSpell)
  return create(inCaster, inTarget, inSpell, "Mage")
end
Skeleton.Acolyte = function(inCaster, inTarget, inSpell)
  return create(inCaster, inTarget, inSpell, "Acolyte")
end
Skeleton.Warrior = function(inCaster, inTarget, inSpell)
  return create(inCaster, inTarget, inSpell, "Warrior")
end

subscribe_spell("Summon: Skeleton Footsoldier", Skeleton.Footsoldier)
subscribe_spell("Summon: Skeleton Mage", Skeleton.Mage)
subscribe_spell("Summon: Skeleton Acolyte", Skeleton.Acolyte)
subscribe_spell("Summon: Skeleton Warrior", Skeleton.Warrior)
