local widget = {}



local widgetMetatable = {
	__index = function (w, k)
		local field = w.__indexMap[k]
		if field ~= nil then
			field(w)
		end
	end,
	__indexMap = {
		["class"] = tsuki.lib.widget.set_class(),
		["valign"] = tsuki.lib.widget.set_valign(),
		["halign"] = tsuki.lib.widget.set_halign(),
		["vexpand"] = tsuki.lib.widget.set_vexpand(),
		["hexpand"] = tsuki.lib.widget.set_hexpand(),
		["width"] = tsuki.lib.widget.set_width(),
		["height"] = tsuki.lib.widget.set_height(),
		["active"] = tsuki.lib.widget.set_active(),
		["tooltip"] = tsuki.lib.widget.set_tooltip(),
		["visible"] = tsuki.lib.widget.set_visible(),
		["style"] = tsuki.lib.widget.set_style(),
	},
	__newindex = function (w, k, o)
		local field = w.__indexMap[k]
		if field ~= nil then
			field(w, o)
		end
	end,
	__newIndexMap = {
		["class"] = tsuki.lib.widget.get_class(),
		["valign"] = tsuki.lib.widget.get_valign(),
		["halign"] = tsuki.lib.widget.get_halign(),
		["vexpand"] = tsuki.lib.widget.get_vexpand(),
		["hexpand"] = tsuki.lib.widget.get_hexpand(),
		["width"] = tsuki.lib.widget.get_width(),
		["height"] = tsuki.lib.widget.get_height(),
		["active"] = tsuki.lib.widget.get_active(),
		["tooltip"] = tsuki.lib.widget.get_tooltip(),
		["visible"] = tsuki.lib.widget.get_visible(),
	},
	__call = function ()
		
	end
}

tsuki.window = function (opts)
	local window = tsuki.lib.window.new()

	return window
end

tsuki.widget = {}

tsuki.widget.label = function (opts)
	local widget = tsuki.lib.label.new()
	local labelMetatable = widgetMetatable

	labelMetatable.__indexMap["text"] = tsuki.lib.label.set_text()
	labelMetatable.__newIndexMap["text"] = tsuki.lib.label.get_text()

	setmetatable(widget, labelMetatable)

	for k,v in ipairs(opts) do
		widget[k] = v
	end

	return widget
end

