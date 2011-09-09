package.path = ";;./?.lua;../resources/scripts/?.lua;"

Pixy.Server = {}
local Handlers = {}
pixy = {}

require "turns"

function list_iter(t)
  local i = 0
  local n = table.getn(t)
  return function ()
    i = i + 1
    if i <= n then return t[i] end
  end
end

function subscribe_spell(inSpellName, inMethod)
	Pixy.Log("subscribing to " .. inSpellName)
	Handlers[inSpellName] = inMethod
end

function process_spell(inCaster, inTarget, inSpell, inEvt)
  --tolua.cast(inCaster, "Pixy::Puppet")
  --tolua.cast(inSpell, "Pixy::Spell")
	--~ Pixy.Log("I have a spell named '" .. inSpell:getName() .. "' to handle!")

	local spellHandler = Handlers[inSpell:getName()]

	if not spellHandler then return false else return spellHandler(inCaster, inTarget, inSpell) end

end

function register_instance(inInstance)

	Instance = inInstance

	function Pixy.Log(msg) Instance:Log(msg) end
	Pixy.Log( "*************************************************")
	Pixy.Log( "* Launching Pixy Server Lua module!             *")

	--EvtMgr = Pixy.EventManager:getSingletonPtr()
  --UnitFactory = Pixy.UnitFactory:getSingletonPtr()
  --PuppetFactory = Pixy.PuppetFactory:getSingletonPtr()

	Pixy.Log( "* Launched successfully!                        *")
	Pixy.Log( "*************************************************")

  require("d_lister")
  --~ require("spell_processor")
  path_to_handlers = "../resources/scripts/spell_handlers"
  races = {"earth", "air", "fire", "water"}
  package.path = package.path .. ";" .. path_to_handlers .. "/?.lua"
  for race in list_iter(races) do
    for filename in dirtree(path_to_handlers .. "/" .. race) do
      stripped_name = string.gsub(filename, path_to_handlers .. "/", "")
      stripped_name = string.gsub(stripped_name, ".lua", "")

      require(stripped_name)
    end
  end
end

function arbitrary(func, ...)
  pixy[func](unpack(arg))
end
