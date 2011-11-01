local __Cost = 2
local Gloom = {} -- Gloom increases the cost of all non-summoning spells by 2

local increase_cost = function(puppet, spell)
  -- gloom only affects non-summoning spells
  if spell:getName():find("Summon") then return true end

  Pixy.Log("checking whether spell " .. spell:getName() .. " owned by " .. spell:getCaster():getName() .. " needs to be WP cost increased")

  if puppet:hasBuffWithName("Gloom") then
  --if spell:getCaster():getUID() == puppet:getUID() then
    spell:setCostWP(spell:getCostWP() + __Cost)
    Pixy.Log("increasing cost of spell "
      .. spell:getName() .. "#" .. spell:getUID()
      .. " for caster " .. spell:getCaster():getName()
      .. ", now WP=" .. spell:getCostWP())
  end

  return true
end

local revert_cost = function(puppet, spell)
  -- gloom only affects non-summoning spells
  if spell:getName():find("Summon") then return true end
  --~ if spell:getCaster():getUID() == enemy:getUID() then
  if puppet:hasBuffWithName("Gloom") then
    spell:setCostWP(spell:getCostWP() - __Cost)
    Pixy.Log("restoring cost of spell "
      .. spell:getName() .. "#" .. spell:getUID()
      .. " for caster " .. spell:getCaster():getName()
      .. ", now WP=" .. spell:getCostWP())
  end

  return true
end


function Gloom:cast()
  if self.Caster:getRank() ~= Pixy.PUPPET then return false end

  -- get the opponent puppet
  Pixy.Log("Applying " .. self.Spell:getName() .. " on " .. self.Target:getName() .. "#" .. self.Target:getUID() .. " by " .. self.Caster:getName() .. "!")

  --~ enemy = self.Target
  -- attach the buff to the puppet
  self.Target:attachBuff(self.Spell)

  -- increase the cost of all the puppet's current spells
  local exporter = Pixy.SpellListExporter()
  exporter:export(Instance, self.Target:getHand(), "Pixy::Spell", "Temp")
  for spell in list_iter(Temp) do
    increase_cost(self.Target, spell)
  end
  Temp = nil

  -- increase the cost of all spells to be drawn by the puppet
  subscribe_event_handler("DrawSpell", increase_cost)

  return true
end

function Gloom:process()
  self:__tick()
end

function Gloom:cleanup()
  -- revert the cost of all the puppet's current spells
  local exporter = Pixy.SpellListExporter()
  exporter:export(Instance, self.Target:getHand(), "Pixy::Spell", "__TempSpells")
  for spell in list_iter(__TempSpells) do
    revert_cost(self.Target, spell)
  end
  __TempSpells = nil
  unsubscribe_event_handler("DrawSpell", increase_cost)
end

subscribe_spell("Gloom", Gloom)
