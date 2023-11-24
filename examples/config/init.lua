local tsuki = require("tsuki")

local label1 = tsuki.widget.label({
	signals = {
		[1000] = function (self)
			self.text = os.date("%H:%M:%S")
		end
	},
	text = "Hello World!",
})

label1.text = "Erm..."

--[[tsukilib.signal.connect(label1, 5000, function ()
	label1.text = "It should've changed"
end)]]

local window1 = tsukilib.window.new()
tsukilib.window.set_child(window1, label1)
tsukilib.window.present(window1)
