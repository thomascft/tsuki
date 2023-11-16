local label1 = tsuki.lib.label.new()
tsuki.lib.label.set_text(label1, "This is a work in progress...")

local window1 = tsuki.lib.window.new()
tsuki.lib.window.set_child(window1, label1)
tsuki.lib.window.present(window1)
