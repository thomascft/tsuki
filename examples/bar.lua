-- This is an example of how a simple bar may be setup once the api is done
local tsuki = require("tsuki")

local create_workspaces = function()
	local workspaces = {}
	for i=1,10 do
		workspaces[i] = tsuki.widget.button({
			text = i
		})
	end
	return workspaces
end

local workspaces = tsuki.widget.box({
	children = create_workspaces(),
	signals = {
		{"hyprland", function (widget, hyprland)
			for i,v in ipairs(widget.children) do
				if hyprland.workspaces[i].active then
					v.classes = {"active"}
				else
					v.classes = {"inactive"}
				end
			end
		end},
	}
})

local now_playing = tsuki.widget.label({
	signals = {
		{"mpris", function (widget, data)
			widget.text = data.currently_playing
		end}
	}
})

local left = tsuki.widget.box({
	children = {
		workspaces
	}
})
local center = tsuki.widget.box({
	children = {
		now_playing
	}
})
local right = tsuki.widget.box({
	children = {
		--battery,
		--clock
	}
})

local bar_widgets = tsuki.widget.box({
	children = {
		left,
		center,
		right
	}
})


local bar = tsuki.window({
	align = {"top", "center"},
	width = "100%",
	height = "10px",
	layer = "top",
	exclusive = true,
	child = bar_widgets
})

bar:present()
