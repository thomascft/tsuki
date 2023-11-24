local _M = {}

local function subscribe_signals(widget, signals)
	for signal,callback in pairs(signals) do
		tsukilib.signal.connect(widget, signal, callback)
	end
end

local widgetMetatable = {
	__indexmap = {
		["class"] = tsukilib.widget.get_classes,
		["valign"] = tsukilib.widget.get_valign,
		["halign"] = tsukilib.widget.get_halign,
		["vexpand"] = tsukilib.widget.get_vexpand,
		["hexpand"] = tsukilib.widget.get_hexpand,
		["width"] = tsukilib.widget.get_width,
		["height"] = tsukilib.widget.get_height,
		["tooltip"] = tsukilib.widget.get_tooltip,
		["visible"] = tsukilib.widget.get_visible,
	},
	__newindexmap = {
		["class"] = tsukilib.widget.set_classes,
		["valign"] = tsukilib.widget.set_valign,
		["halign"] = tsukilib.widget.set_halign,
		["vexpand"] = tsukilib.widget.set_vexpand,
		["hexpand"] = tsukilib.widget.set_hexpand,
		["size"] = tsukilib.widget.set_size,
		["tooltip"] = tsukilib.widget.set_tooltip,
		["visible"] = tsukilib.widget.set_visible,
	},
	__index = function (widget, field)
		local metatable = getmetatable(widget)
		local fieldGetter = metatable.__indexmap[field]

		if fieldGetter then
			return fieldGetter(widget)
		else
			warn("Invalid field: " .. field)
		end
	end,
	__newindex = function (widget, field, value)
		local metatable = getmetatable(widget)
		local fieldSetter = metatable.__newindexmap[field]

		if fieldSetter then
			return fieldSetter(widget, value)
		else
			warn("Invalid field: " .. field)
		end
	end
}

_M.label = function (opts)

	opts = opts or {}

	local widget = tsukilib.label.new()
	local labelMetatable = widgetMetatable

	labelMetatable.__indexmap["text"] = tsukilib.label.get_text
	labelMetatable.__newindexmap["text"] = tsukilib.label.set_text

	tsukilib.helper.set_metatable(widget, labelMetatable)

	local signals = opts.signals

	opts.signals = nil

	for k,v in pairs(opts) do
		widget[k] = v
	end

	subscribe_signals(widget, signals)

	return widget
end

return _M
