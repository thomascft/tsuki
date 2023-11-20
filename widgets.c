#include <stdio.h>
#include <string.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <gtk/gtk.h>

char *h_string_from_gtk_align(GtkAlign g) {
	if (g == GTK_ALIGN_FILL) {
		return "fill";
	}
	else if (g == GTK_ALIGN_START) {
		return "start";
	}
	else if (g == GTK_ALIGN_END) {
		return "end";
	}
	else if (g == GTK_ALIGN_CENTER) {
		return "center";
	}
	else if (g == GTK_ALIGN_BASELINE_FILL) {
		return "baseline-fill";
	}
	else if (g == GTK_ALIGN_BASELINE_CENTER) {
		return "baseline-center";
	}
	else {
		return ""; // TODO: Is this safe 
	}
}
GtkAlign h_gtk_align_from_string(const char *s) {
	if (strcmp(s, "fill") == 0) {
		return GTK_ALIGN_FILL;
	}
	else if (strcmp(s, "start") == 0) {
		return GTK_ALIGN_START;
	}
	else if (strcmp(s, "end") == 0) {
		return GTK_ALIGN_END;
	}
	else if (strcmp(s, "center") == 0) {
		return GTK_ALIGN_CENTER;
	}
	else if (strcmp(s, "baseline-fill") == 0) {
		return GTK_ALIGN_BASELINE_FILL;
	}
	else if (strcmp(s, "baseline-center") == 0) {
		return GTK_ALIGN_BASELINE_CENTER;
	}
	else {
		return GTK_ALIGN_START; // FIXME: This should probably return something different 
	}
}

int l_tsuki_widget_get_classes(lua_State *L) {
	GtkWidget **widget_ud = lua_touserdata(L, 1);
	GtkWidget *widget = *widget_ud;
	char **classes = gtk_widget_get_css_classes(widget);

	lua_newtable(L);
	
	for (int i = 0; classes[i] != NULL; i++ ) {
		lua_pushstring(L, classes[i]);
		lua_seti(L, 2, i+1);
	}

	return 1;
	g_strfreev(classes);
}
int l_tsuki_widget_set_classes(lua_State *L) {
	GtkWidget **widget_ud = lua_touserdata(L, 1);
	GtkWidget *widget = *widget_ud;
	const char *classes[] = {lua_tostring(L, 2), NULL}; //TODO: Fix it so it actually takes an array

	gtk_widget_set_css_classes(widget, classes);

	return 0;
}
int l_tsuki_widget_get_valign(lua_State *L) {
	GtkWidget **widget_ud = lua_touserdata(L, 1);
	GtkWidget *widget = *widget_ud;
	GtkAlign g = gtk_widget_get_valign(widget);
	const char *s = h_string_from_gtk_align(g);

	lua_pushstring(L, s);

	return 1;
}
int l_tsuki_widget_set_valign(lua_State *L) {
	GtkWidget **widget_ud = lua_touserdata(L, 1);
	GtkWidget *widget = *widget_ud;
	const char *s = lua_tostring(L, 2);
	GtkAlign g = h_gtk_align_from_string(s);

	gtk_widget_set_valign(widget, g);

	return 0;
}
int l_tsuki_widget_get_halign(lua_State *L) {
	GtkWidget **widget_ud = lua_touserdata(L, 1);
	GtkWidget *widget = *widget_ud;
	GtkAlign g = gtk_widget_get_halign(widget);
	const char *s = h_string_from_gtk_align(g);

	lua_pushstring(L, s);

	return 1;
}
int l_tsuki_widget_set_halign(lua_State *L) {
	GtkWidget **widget_ud = lua_touserdata(L, 1);
	GtkWidget *widget = *widget_ud;
	const char *s = lua_tostring(L, 2);
	GtkAlign g = h_gtk_align_from_string(s);

	gtk_widget_set_halign(widget, g);

	return 0;
}
int l_tsuki_widget_get_vexpand(lua_State *L) {
	GtkWidget **widget_ud = lua_touserdata(L, 1);
	GtkWidget *widget = *widget_ud;

	lua_pushboolean(L, gtk_widget_get_vexpand(widget));

	return 1;
}
int l_tsuki_widget_set_vexpand(lua_State *L) {
	GtkWidget **widget_ud = lua_touserdata(L, 1);
	GtkWidget *widget = *widget_ud;
	bool vexpand_true = lua_toboolean(L, 2);

	gtk_widget_set_vexpand(widget, vexpand_true);

	return 0;
}
int l_tsuki_widget_get_hexpand(lua_State *L) {
	GtkWidget **widget_ud = lua_touserdata(L, 1);
	GtkWidget *widget = *widget_ud;

	lua_pushboolean(L, gtk_widget_get_hexpand(widget));

	return 1;
}
int l_tsuki_widget_set_hexpand(lua_State *L) {
	GtkWidget **widget_ud = lua_touserdata(L, 1);
	GtkWidget *widget = *widget_ud;
	bool hexpand_true = lua_toboolean(L, 2);

	gtk_widget_set_hexpand(widget, hexpand_true);

	return 0;
}

int l_tsuki_widget_get_width(lua_State *L) {
	GtkWidget **widget_ud = lua_touserdata(L, 1);
	GtkWidget *widget = *widget_ud;
	int width = gtk_widget_get_width(widget);

	lua_pushinteger(L, width);

	return 1;
}
int l_tsuki_widget_get_height(lua_State *L) {
	GtkWidget **widget_ud = lua_touserdata(L, 1);
	GtkWidget *widget = *widget_ud;
	int height = gtk_widget_get_height(widget);

	lua_pushinteger(L, height);

	return 1;
}
int l_tsuki_widget_set_size(lua_State *L) {
	GtkWidget **widget_ud = lua_touserdata(L, 1);
	GtkWidget *widget = *widget_ud;
	int width = lua_getfield(L, 2, "width");
	int height = lua_getfield(L, 2, "height");

	gtk_widget_set_size_request(widget, width, height);

	return 0;
}

int l_tsuki_widget_get_tooltip(lua_State *L) {
	GtkWidget **widget_ud = lua_touserdata(L, 1);
	GtkWidget *widget = *widget_ud;
	const char *tooltip = gtk_widget_get_tooltip_text(widget);

	lua_pushstring(L, tooltip);

	return 1;
}
int l_tsuki_widget_set_tooltip(lua_State *L) {
	GtkWidget **widget_ud = lua_touserdata(L, 1);
	GtkWidget *widget = *widget_ud;
	const char *tooltip = lua_tostring(L, 2);

	gtk_widget_set_tooltip_text(widget, tooltip);

	return 0;
}
int l_tsuki_widget_get_visible(lua_State *L) {
	GtkWidget **widget_ud = lua_touserdata(L, 1);
	GtkWidget *widget = *widget_ud;
	bool is_visible = gtk_widget_get_visible(widget);

	lua_pushboolean(L, is_visible);

	return 1;
}
int l_tsuki_widget_set_visible(lua_State *L) {
	GtkWidget **widget_ud = lua_touserdata(L, 1);
	GtkWidget *widget = *widget_ud;
	bool is_visible = lua_toboolean(L, 2);

	gtk_widget_set_visible(widget, is_visible);

	return 1;
}
static const luaL_Reg l_tsuki_widget_fns[] = {
	{"get_classes", l_tsuki_widget_get_classes},
	{"set_classes", l_tsuki_widget_set_classes},
	{"get_valign", l_tsuki_widget_get_valign},
	{"set_valign", l_tsuki_widget_set_valign},
	{"get_halign", l_tsuki_widget_get_halign},
	{"set_halign", l_tsuki_widget_set_halign},
	{"get_vexpand", l_tsuki_widget_get_vexpand},
	{"set_vexpand", l_tsuki_widget_set_vexpand},
	{"get_hexpand", l_tsuki_widget_get_hexpand},
	{"set_hexpand", l_tsuki_widget_set_hexpand},
	{"get_tooltip", l_tsuki_widget_get_tooltip},
	{"set_tooltip", l_tsuki_widget_set_tooltip},
	{"get_visible", l_tsuki_widget_get_visible},
	{"set_visible", l_tsuki_widget_set_visible},
	{"get_width", l_tsuki_widget_get_width},
	{"get_height", l_tsuki_widget_get_height},
	{"set_size", l_tsuki_widget_set_size},
	{NULL, NULL}
};

int l_tsuki_window_new(lua_State *L) {
	GtkApplication *app;
	lua_getfield(L, LUA_REGISTRYINDEX, "GtkApplication");
	app = lua_touserdata(L, -1);

	GtkWidget *window = gtk_application_window_new(app);
	GtkWidget **window_ud = lua_newuserdata(L, sizeof(window));

	*window_ud = window;

	return 1;
}
int l_tsuki_window_set_child(lua_State *L) {
	GtkWidget **window_ud = lua_touserdata(L, 1);
	GtkWidget *window = *window_ud;
	GtkWidget **widget_ud = lua_touserdata(L, 2);
	GtkWidget *widget = *widget_ud;

	gtk_window_set_child(GTK_WINDOW(window), widget);

	return 0;
}
int l_tsuki_window_present(lua_State *L) {
	GtkWidget **window_ud = lua_touserdata(L, 1);
	GtkWidget *window = *window_ud;
	gtk_window_present(GTK_WINDOW(window));

	return 0;
}
static const luaL_Reg l_tsuki_window_fns[] = {
	{"new", l_tsuki_window_new},
	{"set_child", l_tsuki_window_set_child},
	{"present", l_tsuki_window_present},
	{NULL, NULL}
};

int l_tsuki_label_new(lua_State *L) {
	GtkWidget *widget = gtk_label_new("");
	GtkWidget **widget_ud = lua_newuserdata(L, sizeof(widget));

	*widget_ud = widget;

	return 1;
}
int l_tsuki_label_get_text(lua_State *L) {
	GtkWidget **widget_ud = lua_touserdata(L, 1);
	GtkWidget *widget = *widget_ud;
	
	lua_pushstring(L, gtk_label_get_text(GTK_LABEL(widget)));

	return 1;
}
int l_tsuki_label_set_text(lua_State *L) {
	GtkWidget **widget_ud = lua_touserdata(L, 1);
	GtkWidget *widget = *widget_ud;
	const char *text = lua_tostring(L, 2);
	
	gtk_label_set_text(GTK_LABEL(widget), text);

	return 0;
}
static const luaL_Reg l_tsuki_label_fns[] = {
	{"new", l_tsuki_label_new},
	{"get_text", l_tsuki_label_get_text},
	{"set_text", l_tsuki_label_set_text},
	{NULL, NULL}
};

int l_tsuki_helper_set_metatable(lua_State *L) {
	// FIXME: I wasn't able to set them metatable within the lua state, so this is my fix
	lua_setmetatable(L, 1);

	return 0;
} 

static const luaL_Reg l_tsuki_helper_fns[] = {
	{"set_metatable", l_tsuki_helper_set_metatable},
	{NULL, NULL}
};

void tsuki_widget_register_fns(lua_State *L) {
	lua_getglobal(L, "tsukisys");
	lua_newtable(L); // The lib table
	
	luaL_newlib(L, l_tsuki_widget_fns);
	lua_setfield(L, 2, "widget");

	luaL_newlib(L, l_tsuki_window_fns);
	lua_setfield(L, 2, "window");

	luaL_newlib(L, l_tsuki_label_fns);
	lua_setfield(L, 2, "label");
	
	luaL_newlib(L, l_tsuki_helper_fns);
	lua_setfield(L, 2, "helper");
	
	lua_setfield(L, 1, "lib");
}
