#include <stdio.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <gtk/gtk.h>

#include "widgets/widgets.c"


void activate(GtkApplication *app, gpointer user_data) {
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	lua_pushlightuserdata(L, app);
	lua_setfield(L, LUA_REGISTRYINDEX, "GtkApplication");

	lua_newtable(L);
	lua_setglobal(L, "tsuki");

	tsuki_widget_register_fns(L);

	luaL_dofile(L, "/home/thomas/Projects/tsuki/examples/init.lua");
}

int main(int argc, char *argv[]) {
	GtkApplication *app;
	int status;

	app = gtk_application_new("com.github.thomascft.tsuki", G_APPLICATION_DEFAULT_FLAGS);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);

	return status;
}
