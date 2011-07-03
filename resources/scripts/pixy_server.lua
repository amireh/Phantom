package.path = ";;./?.lua;../resources/scripts/?.lua;"

Pixy.Server = {}

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
  require("spell_processor")
  path_to_handlers = "../resources/scripts/spell_handlers"
  package.path = package.path .. ";" .. path_to_handlers .. "/?.lua"
  for filename in dirtree(path_to_handlers) do
	  stripped_name = string.gsub(filename, path_to_handlers .. "/", "")
	  stripped_name = string.gsub(stripped_name, ".lua", "")

	  require(stripped_name)
  end
end
