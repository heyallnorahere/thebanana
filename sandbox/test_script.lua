function on_load()
	debug_print("loaded test_menu.json")
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
function next()
	play_sound("click", false)
	load_menu("test_sub_menu.json")
end