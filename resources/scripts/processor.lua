function setupEvtMap()
  EventMap = {}
	tracking = false
end

-- registers a handler to an event
function subscribeToEvt(inUID, inHandler)

	-- init handler table for event if it doesnt exist
	if (not EventMap[inUID]) then
		EventMap[inUID] = {}
	end

	table.insert(EventMap[inUID], inHandler)

	Pixy.Log("Subscribed handler to " .. inUID)
end
bind = subscribeToEvt -- an alias

function unsubscribeFromEvt(inUID, inHandler)
  if not EventMap[inUID] then return false end

  removeByValue(EventMap[inUID], inHandler)

  return true
end
unbind = unsubscribeFromEvt

function clearBindings()
  EventMap = {}
end

-- calls handlers and returns status
function processEvt(inEvt)
  --inEvt = tolua.cast(inEvt, "Pixy::Event")
  --tolua.cast(inEvt.UID, "Pixy::EventUID")
  if (inEvt.UID == Pixy.EventUID.Connected) then Pixy.Log("There's a connected evt!") end

	if (not EventMap[inEvt.UID]) then return true end

  Pixy.Log("Handling an event " .. inEvt.UID)
  for handler in list_iter(EventMap[inEvt.UID]) do
    handler(inEvt)
  end

	return true
end

setupEvtMap()
