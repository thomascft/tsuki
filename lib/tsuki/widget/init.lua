local _M = {}

local widgetMetatable = {
	__indexmap = {
		["class"] = tsukisys.lib.widget.get_classes,
		["valign"] = tsukisys.lib.widget.get_valign,
		["halign"] = tsukisys.lib.widget.get_halign,
		["vexpand"] = tsukisys.lib.widget.get_vexpand,
		["hexpand"] = tsukisys.lib.widget.get_hexpand,
		["width"] = tsukisys.lib.widget.get_width,
		["height"] = tsukisys.lib.widget.get_height,
		["tooltip"] = tsukisys.lib.widget.get_tooltip,
		["visible"] = tsukisys.lib.widget.get_visible,
	},
	__newindexmap = {
		["class"] = tsukisys.lib.widget.set_classes,
		["valign"] = tsukisys.lib.widget.set_valign,
		["halign"] = tsukisys.lib.widget.set_halign,
		["vexpand"] = tsukisys.lib.widget.set_vexpand,
		["hexpand"] = tsukisys.lib.widget.set_hexpand,
		["size"] = tsukisys.lib.widget.set_size,
		["tooltip"] = tsukisys.lib.widget.set_tooltip,
		["visible"] = tsukisys.lib.widget.set_visible,
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

	local widget = tsukisys.lib.label.new()
	local labelMetatable = widgetMetatable

	labelMetatable.__indexmap["text"] = tsukisys.lib.label.get_text
	labelMetatable.__newindexmap["text"] = tsukisys.lib.label.set_text

	tsukisys.lib.helper.set_metatable(widget, labelMetatable)

	for k,v in pairs(opts) do
		widget[k] = v
	end

	return widget
end

return _M
