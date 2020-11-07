function on_load()
	debug_print("loaded test_sub_menu.json")
end
function back()
	play_sound("click", false)
	load_menu("test_menu.json")
end