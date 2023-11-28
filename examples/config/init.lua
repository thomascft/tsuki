local tsuki = require("tsuki")

local label1 = tsuki.widget.label({
	signals = {
		--[[[1000] = function (self)
			self.text = os.date("%H:%M:%S")
		end]]
		["hyprland-changed"] = function (self, data)
			self.text = data
		end
	},

	text = "Hello World!",
})

label1.text = "Erm..."

local window1 = tsukilib.window.new()
tsukilib.window.set_child(window1, label1)
tsukilib.window.present(window1)

