print("[lua] Hello World!")

print(type(tsuki.window))

local window = tsuki.window({})
print("created window")

local label = tsuki.lib.label.new()
print("created label")
tsuki.lib.window.set_child(window, label)

tsuki.lib.window.present(window)
