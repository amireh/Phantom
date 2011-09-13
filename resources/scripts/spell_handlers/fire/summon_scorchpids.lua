local Scorchpid = {
  Drone = nil,
  Ashcrawler = nil,
  Lavawalker = nil,
  Magmaburster = nil
}

local create = function(inCaster, inTarget, inSpell, inName)
	Pixy.Log("I'm summoning a Scorchpid: " .. inName .. " - SPELL COST: " .. inSpell:getCostWP() .. ":" .. inSpell:getCostHP() .. ":" .. inSpell:getCostChannels())
  tolua.cast(inCaster, "Pixy::Puppet")

	-- create the unit and broadcast it to the players
  local unit = Instance:create_unit("Scorchpid " .. inName, inCaster)

  if (not unit) then return false end

	return true
end

Scorchpid.Drone = function(inCaster, inTarget, inSpell)
  return create(inCaster, inTarget, inSpell, "Drone")
end
Scorchpid.Ashcrawler = function(inCaster, inTarget, inSpell)
  return create(inCaster, inTarget, inSpell, "Ashcrawler")
end
Scorchpid.Lavawalker = function(inCaster, inTarget, inSpell)
  return create(inCaster, inTarget, inSpell, "Lavawalker")
end
Scorchpid.Magmaburster = function(inCaster, inTarget, inSpell)
  return create(inCaster, inTarget, inSpell, "Magmaburster")
end

subscribe_spell("Summon: Scorchpid Drone", Scorchpid.Drone)
subscribe_spell("Summon: Scorchpid Ashcrawler", Scorchpid.Ashcrawler)
subscribe_spell("Summon: Scorchpid Lavawalker", Scorchpid.Lavawalker)
subscribe_spell("Summon: Scorchpid Magmaburster", Scorchpid.Magmaburster)
