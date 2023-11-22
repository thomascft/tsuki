local tsuki = require("tsuki")

local label2 = tsukilib.label.new()
local label1 = tsuki.widget.label({
	text = "Hello World!",
})

label1.text = "Erm..."

local window1 = tsukilib.window.new()
tsukilib.window.set_child(window1, label1)
tsukilib.window.present(window1)
