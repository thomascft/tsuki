local tsuki = require("tsuki")

local label2 = tsukilib.label.new()
local label1 = tsuki.widget.label({
	text = "Hello World!",
})

label1.text = "Erm..."

tsukilib.signal.connect(label1, 5000, function ()
	label1.text = "It should've changed"
end)

local window1 = tsukilib.window.new()
tsukilib.window.set_child(window1, label1)
tsukilib.window.present(window1)
