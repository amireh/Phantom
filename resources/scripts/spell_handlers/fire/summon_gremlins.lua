local Gremlin = {
  Mechanical = nil,
  Brawler = nil,
  Engineer = nil,
  Master = nil
}

local create = function(inCaster, inTarget, inSpell, inName)
	Pixy.Log("I'm summoning a Gremlin: " .. inName .. " - SPELL COST: " .. inSpell:getCostWP() .. ":" .. inSpell:getCostHP() .. ":" .. inSpell:getCostChannels())
  tolua.cast(inCaster, "Pixy::Puppet")

	-- create the unit and broadcast it to the players
  local unit = Instance:create_unit(inName, inCaster)

  if (not unit) then return false end

	return true
end

Gremlin.Mechanical = function(inCaster, inTarget, inSpell)
  return create(inCaster, inTarget, inSpell, "Mechanical Gremlin")
end
Gremlin.Brawler = function(inCaster, inTarget, inSpell)
  return create(inCaster, inTarget, inSpell, "Gremlin Brawler")
end
Gremlin.Engineer = function(inCaster, inTarget, inSpell)
  return create(inCaster, inTarget, inSpell, "Gremlin Engineer")
end
Gremlin.Master = function(inCaster, inTarget, inSpell)
  return create(inCaster, inTarget, inSpell, "Master Gremlin")
end

subscribe_spell("Summon: Mechanical Gremlin", Gremlin.Mechanical)
subscribe_spell("Summon: Gremlin Brawler", Gremlin.Brawler)
subscribe_spell("Summon: Gremlin Engineer", Gremlin.Engineer)
subscribe_spell("Summon: Master Gremlin", Gremlin.Master)
