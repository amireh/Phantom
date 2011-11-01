local create = function(inCaster, inTarget, inSpell, inName)
	Pixy.Log("I'm summoning a Gremlin: " .. inName .. " - SPELL COST: " .. inSpell:getCostWP() .. ":" .. inSpell:getCostHP() .. ":" .. inSpell:getCostChannels())
  --tolua.cast(inCaster, "Pixy::Puppet")

	-- create the unit and broadcast it to the players
  assert(inCaster)
  print("Caster's name : " .. inCaster:getName())
  local unit = Instance:create_unit(inName, inCaster)

  if (not unit) then return false end

	return true
end

local Gremlin_Mechanical = {}
function Gremlin_Mechanical:cast()
  return create(self.Caster, self.Target, self.Spell, "Mechanical Gremlin")
end
local Gremlin_Brawler = {}
function Gremlin_Brawler:cast()
  return create(self.Caster, self.Target, self.Spell, "Gremlin Brawler")
end
local Gremlin_Engineer = {}
function Gremlin_Engineer:cast()
  return create(self.Caster, self.Target, self.Spell, "Gremlin Engineer")
end
local Gremlin_Master = {}
function Gremlin_Master:cast()
  return create(self.Caster, self.Target, self.Spell, "Master Gremlin")
end


subscribe_spell("Summon: Mechanical Gremlin", Gremlin_Mechanical)
subscribe_spell("Summon: Gremlin Brawler", Gremlin_Brawler)
subscribe_spell("Summon: Gremlin Engineer", Gremlin_Engineer)
subscribe_spell("Summon: Master Gremlin", Gremlin_Master)
