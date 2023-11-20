local tsuki = require("tsuki")

local label2 = tsukisys.lib.label.new()
local label1 = tsuki.widget.label({
	text = "Hello World!",
})

label1.text = "Erm..."

local window1 = tsukisys.lib.window.new()
tsukisys.lib.window.set_child(window1, label1)
tsukisys.lib.window.present(window1)
