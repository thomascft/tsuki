#include <stdio.h>
#include <string.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <gtk/gtk.h>

int l_tsuki_widget_get_classes(lua_State *L) {
	GtkWidget **widget_ud = lua_touserdata(L, 1);
	GtkWidget *widget = *widget_ud;
	char **classes = gtk_widget_get_css_classes(widget);

	lua_newtable(L); // idx 2
	
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

	return 1;
}
static const luaL_Reg l_tsuki_widget_fns[] = {
	{"get_classes", l_tsuki_widget_get_classes},
	{"set_classes", l_tsuki_widget_set_classes},
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

void tsuki_widget_register_fns(lua_State *L) {
	lua_getglobal(L, "tsuki");
	lua_newtable(L); // The lib table
	
	luaL_newlib(L, l_tsuki_widget_fns);
	lua_setfield(L, 2, "widget");

	luaL_newlib(L, l_tsuki_window_fns);
	lua_setfield(L, 2, "window");

	luaL_newlib(L, l_tsuki_label_fns);
	lua_setfield(L, 2, "label");
	
	lua_setfield(L, 1, "lib");

	luaL_dofile(L, "/home/thomas/Projects/tsuki/src/widgets/widgets.lua");
}
