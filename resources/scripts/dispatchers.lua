Dispatchers = {}
Spells = {}
-- Spell Handlers
local Definitions = {}
local SpellHandlerInstances = {}
-- There are two kinds of unit handler hooks:
--  1) Generic state hooks: these are a list of handlers that can be subscribed
--      to a _state_ and will be called irrelevant of the Unit model
--
--  2) Specific unit hooks: these are per-unit-per-state hooks
local UnitHandlers = {
  Generic = {
    Alive = {},
    Dead = {}
  }
}

local EventHandlers = {
  DrawSpell = {},
  DropSpell = {}
}

local function unit_subscribed(name, state)
  return UnitHandlers[name] and UnitHandlers[name][state]
end

Spells.onDrawSpell = function(puppet, spell)
  for handler in list_iter(EventHandlers.DrawSpell) do
    handler(puppet, spell)
  end
end

Spells.onDropSpell = function(puppet, spell)
  for handler in list_iter(EventHandlers.DropSpell) do
    handler(puppet, spell)
  end
end

Dispatchers.onEntityAlive = function(e)
  local entity = e.Any

  for handler in list_iter(UnitHandlers.Generic.Alive) do
    handler(e)
  end

  if unit_subscribed(entity:getName(), "Alive") then
    return UnitHandlers[entity:getName()]["Alive"](entity)
  end

  return true
end

Dispatchers.onEntityDied = function(e)
  local entity = e.Any

  for handler in list_iter(UnitHandlers.Generic.Dead) do
    handler(e)
  end

  if unit_subscribed(entity:getName(), "Dead") then
    return UnitHandlers[entity:getName()]["Dead"](entity)
  end

  return true
end

-- spell subscriptions
function subscribe_spell(inSpellName, inDef)
	Pixy.Log("subscribing to Spell: " .. inSpellName)
	Definitions[inSpellName] = inDef
end

-- unit subscriptions
function subscribe_unit(inUnitName, inState, inMethod)
	Pixy.Log("subscribing to Unit: " .. inUnitName .. " on " .. inState)
  if not UnitHandlers[inUnitName] then UnitHandlers[inUnitName] = {} end
	UnitHandlers[inUnitName][inState] = inMethod
end

function subscribe_generic_unit_handler(inState, inMethod)
  if not UnitHandlers.Generic[inState] then
    UnitHandlers.Generic[inState] = {}
  end

  table.insert(UnitHandlers.Generic[inState], inMethod)
end

function unsubscribe_generic_unit_handler(inState, inMethod)
  removeByValue(UnitHandlers.Generic[inState], inMethod)
end

function subscribe_event_handler(inEvent, inMethod)
  assert(EventHandlers[inEvent])
  table.insert(EventHandlers[inEvent], inMethod)
end

function unsubscribe_event_handler(inEvent, inMethod)
  assert(EventHandlers[inEvent])
  removeByValue(EventHandlers[inEvent], inMethod)
end
--[[
function process_spell(inCaster, inTarget, inSpell, inEvt)
  --tolua.cast(inCaster, "Pixy::Puppet")
  --tolua.cast(inSpell, "Pixy::Spell")
	--~ Pixy.Log("I have a spell named '" .. inSpell:getName() .. "' to handle!")

	local spellHandler = SpellHandlers[inSpell:getName()]

	if not spellHandler then return false else return spellHandler(inCaster, inTarget, inSpell) end
end
]]
-- this is called by every buff handler when the spell effect is over
Spells.__destroyInstance = function(inUID)
  local handler = SpellHandlerInstances[inUID]
  --~ handler:destroy()

  SpellHandlerInstances[inUID] = nil
  return true
end

Spells.onProcessBuff = function(inSpell)
  local instance = SpellHandlerInstances[inSpell:getUID()]
  if not instance then
    return error("Could not find the spell handler instance for " .. inSpell:getName() .. "#" .. inSpell:getUID())
  end

  instance:process()

  if instance:isDone() then instance:destroy() end
end

Spells.onCastSpell = function(inCaster, inTarget, inSpell)
  -- 1. if there is no registered handler for this spell, error out
  local definition = Definitions[inSpell:getName()]
  if not definition then
    return Pixy.Log("Could not find a spell definition for " .. inSpell:getName())
  end

  -- 2. create a new instance of the SpellHandler for this type of spell
  local instance = SpellHandler:new(inSpell, inCaster, inTarget, definition)
  instance:bootstrap()

  -- 3. track it
  assert(not SpellHandlerInstances[inSpell:getUID()])
  SpellHandlerInstances[inSpell:getUID()] = instance

  -- 4. call it
  local result = instance:cast()

  -- if the cast failed, remove the handler instance
  if not result then
    Spells.__destroyInstance(uid)
  else
    -- 5. if it's not a buff, destroy it
    if instance:isDone() then instance:destroy() end
  end

  return result
end
