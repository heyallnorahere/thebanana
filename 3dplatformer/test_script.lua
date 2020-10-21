function on_load()
	debug_print("loaded test_menu.json")
end
function on_unload()
	debug_print("unloaded test_menu.json")
end
function ellipse_button()
	play_sound("click", false)
	debug_print("clicked ellipse")
end
function rect_button()
	play_sound("click", false)
	debug_print("clicked rectangle")
end
function exit()
	play_sound("click", false)
	close_game()
end