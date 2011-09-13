local Bargor = {
  Stomper = nil,
  Pulverizer = nil,
  Chieftain = nil,
  Maimer = nil
}

local create = function(inCaster, inTarget, inSpell, inName)
	Pixy.Log("I'm summoning a Bargor: " .. inName .. " - SPELL COST: " .. inSpell:getCostWP() .. ":" .. inSpell:getCostHP() .. ":" .. inSpell:getCostChannels())
  tolua.cast(inCaster, "Pixy::Puppet")

	-- create the unit and broadcast it to the players
  local unit = Instance:create_unit("Bargor " .. inName, inCaster)

  if (not unit) then return false end

	return true
end

Bargor.Stomper = function(inCaster, inTarget, inSpell)
  return create(inCaster, inTarget, inSpell, "Stomper")
end
Bargor.Pulverizer = function(inCaster, inTarget, inSpell)
  return create(inCaster, inTarget, inSpell, "Pulverizer")
end
Bargor.Chieftain = function(inCaster, inTarget, inSpell)
  return create(inCaster, inTarget, inSpell, "Chieftain")
end
Bargor.Maimer = function(inCaster, inTarget, inSpell)
  return create(inCaster, inTarget, inSpell, "Maimer")
end

subscribe_spell("Summon: Bargor Stomper", Bargor.Stomper)
subscribe_spell("Summon: Bargor Pulverizer", Bargor.Pulverizer)
subscribe_spell("Summon: Bargor Chieftain", Bargor.Chieftain)
subscribe_spell("Summon: Bargor Maimer", Bargor.Maimer)
