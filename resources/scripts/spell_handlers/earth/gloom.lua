local turns_left = 0
local enemy = nil
-- Gloom increases the cost of all non-summoning spells by 2
local _cost = 2
local increase_cost = function(puppet, spell)
  -- gloom only affects non-summoning spells
  if spell:getName():find("Summon") then return true end

  Pixy.Log("checking whether spell " .. spell:getName() .. " owned by " .. spell:getCaster():getName() .. " needs to be WP cost increased")
  if spell:getCaster():getUID() == enemy:getUID() then
    spell:setCostWP(spell:getCostWP() + _cost)
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
  if spell:getCaster():getUID() == enemy:getUID() then
    spell:setCostWP(spell:getCostWP() - _cost)
    Pixy.Log("restoring cost of spell "
      .. spell:getName() .. "#" .. spell:getUID()
      .. " for caster " .. spell:getCaster():getName()
      .. ", now WP=" .. spell:getCostWP())
  end

  return true
end

local apply_buff = function(inCaster, inTarget, inSpell)
  if inCaster:getRank() ~= Pixy.PUPPET then return false end

  -- get the opponent puppet
  Pixy.Log("Applying " .. inSpell:getName() .. " on " .. inTarget:getName() .. "#" .. inTarget:getUID() .. " by " .. inCaster:getName() .. "!")

  enemy = inTarget
  -- attach the buff to the puppet
  inTarget:attachBuff(inSpell)
  turns_left = inSpell:getDuration()
  inSpell:setExpired(false)

  -- increase the cost of all the puppet's current spells
  local exporter = Pixy.SpellListExporter()
  exporter:export(Instance, inTarget:getHand(), "Pixy::Spell", "Temp")
  for spell in list_iter(Temp) do
    increase_cost(nil, spell)
  end
  Temp = nil

  -- increase the cost of all spells to be drawn by the puppet
  subscribe_event_handler("DrawSpell", increase_cost)

  return true
end

local process_buff = function(inCaster, inTarget, inSpell)

  turns_left = turns_left - 1
  -- remove spell when the duration expires
  if (turns_left == 0) then
    inSpell:setExpired(true)

    -- revert the cost of all the puppet's current spells
    local exporter = Pixy.SpellListExporter()
    exporter:export(Instance, inTarget:getHand(), "Pixy::Spell", "Temp")
    for spell in list_iter(Temp) do
      revert_cost(nil, spell)
    end
    Temp = nil
    unsubscribe_event_handler("DrawSpell", increase_cost)
  end

  return true
end

local process = function(inCaster, inTarget, inSpell)
  --tolua.cast(inCaster, "Pixy::Puppet")

  inTarget = Instance:get_enemy(inCaster:getUID())
  if (inTarget:hasBuff(inSpell:getUID())) then
    return process_buff(inCaster, inTarget, inSpell)
  else
    return apply_buff(inCaster, inTarget, inSpell)
  end

end

subscribe_spell("Gloom", process)
