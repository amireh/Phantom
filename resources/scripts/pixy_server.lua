package.path = ";;./?.lua;../resources/scripts/?.lua;"

Pixy.Server = {}
pixy = {}

require "helpers"
require "processor"

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


  require "turns"
  require "dispatchers"
  require "bindings"

  require("d_lister")
  local races = {"earth", "air", "fire", "water"}

  -- register spell bindings
  path_to_handlers = "../resources/scripts/spell_handlers"
  package.path = package.path .. ";" .. path_to_handlers .. "/?.lua"
  for race in list_iter(races) do
    for filename in dirtree(path_to_handlers .. "/" .. race) do
      stripped_name = string.gsub(filename, path_to_handlers .. "/", "")
      stripped_name = string.gsub(stripped_name, ".lua", "")

      require(stripped_name)
    end
  end

  -- register unit bindings
  path_to_handlers = "../resources/scripts/unit_handlers"
  package.path = package.path .. ";" .. path_to_handlers .. "/?.lua"
  for race in list_iter(races) do
    local _path_ = path_to_handlers .. "/" .. race
    if lfs.attributes(_path_) then
    for filename in dirtree(_path_) do
      stripped_name = string.gsub(filename, path_to_handlers .. "/", "")
      stripped_name = string.gsub(stripped_name, ".lua", "")

      require(stripped_name)
    end
    end
  end

end

function arbitrary(func, ...)
  pixy[func](unpack(arg))
end
