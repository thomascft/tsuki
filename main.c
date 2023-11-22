#include <stdio.h>
#include <stdlib.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <gtk/gtk.h>
#include <string.h>

#include "widgets.c"

void add_to_lua_path(lua_State *L) {
	char new_path[1000] = "";
	char tsuki_lib_path[1000] = "";

	strcpy(tsuki_lib_path, getenv("TSUKI_LIB_PATH"));
	if (!*tsuki_lib_path) {
		strcpy(tsuki_lib_path, "/usr/share/tsuki/lib"); // TODO: Update this once it's ready for release
	}

	lua_getglobal(L, "package");
	lua_getfield(L, -1, "path");
	const char *current_path = lua_tostring(L, -1);
	strcat(new_path, current_path);
	strcat(new_path, ";");
	strcat(new_path, tsuki_lib_path);
	strcat(new_path, "/?/init.lua");
	lua_pop(L, 1);
	lua_pushstring(L, new_path);
	lua_setfield(L, -2, "path");
}

void activate(GtkApplication *app, gpointer user_data) {
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	lua_pushlightuserdata(L, app);
	lua_setfield(L, LUA_REGISTRYINDEX, "GtkApplication");

	lua_newtable(L);
	lua_setglobal(L, "tsukilib");

	l_tsuki_widget_fns_register(L);
	add_to_lua_path(L);

	if (LUA_OK != luaL_dofile(L, "/home/thomas/Projects/tsuki/examples/init.lua")){
		printf("%s\n", lua_tostring(L, -1));
		lua_pop(L, 1);
	}
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
