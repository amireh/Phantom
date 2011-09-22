local turn_number = 0
local channels_step = 12
-- every _cmod_ turns, a hero gains _cpt_ channels, note that 1 round is 2 turns
-- cmod: Channels Modifier
local cmod = 4
-- every _channels_step_ turns, _cpt_ increases by 1
-- cpt: Channels Per Turn
local cpt = 1

pixy.tick_turn = function()
  turn_number = turn_number + 1
  Pixy.Log("+++ New turn started: " .. turn_number .. "+++")
  if (turn_number % channels_step == 0) then cpt = cpt + 1 end
end

pixy.tick_resources = function(puppet)
  local channels = 0
  if (turn_number % cmod == 0 or turn_number % cmod == 1) then
    channels = cpt
  end

  puppet:setChannels(puppet:getChannels() + channels)
  puppet:setWP(puppet:getChannels())

  local evt = Pixy.Event:new(Pixy.EventUID.UpdatePuppet, Pixy.EventFeedback.Ok)
  evt:setProperty("UID", puppet:getUID())
  evt:setProperty("Channels", puppet:getChannels())
  evt:setProperty("WP", puppet:getWP())
  Instance:broadcast(evt)

  Pixy.Log(puppet:getName() .. "#" .. puppet:getUID() .. " now has " .. puppet:getChannels() .. " channels")

end
