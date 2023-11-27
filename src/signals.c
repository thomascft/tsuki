#include <gtk/gtk.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "service.h"

typedef struct {
	lua_State *L;
	int widget_ud_ref;
	int fn_ref;
	guint signal_id;
} CallbackData;

bool tsuki_signal_time(gpointer P) {
	CallbackData *data = (CallbackData *)P;
	lua_State *L = data->L;
	lua_rawgeti(L, LUA_REGISTRYINDEX, data->fn_ref);
	lua_rawgeti(L, LUA_REGISTRYINDEX, data->widget_ud_ref);
	lua_call(L, 1, 0);

	return true;
}

bool data_updated(Service *serviceinstance, gchar *new_data, gpointer udp) {
	CallbackData *user_data = (CallbackData *)udp;
	lua_State *L = user_data->L;

	lua_rawgeti(L, LUA_REGISTRYINDEX, user_data->fn_ref);
	lua_rawgeti(L, LUA_REGISTRYINDEX, user_data->widget_ud_ref);
	lua_pushstring(L, new_data);
	lua_call(L, 2, 0);

	return true;
}


int l_tsuki_signal_connect(lua_State *L) {
	CallbackData *data = (CallbackData *)malloc(sizeof(CallbackData));
	data->L = L; // FIXME: Investigate lua state lifetime
	data->fn_ref = luaL_ref(L, LUA_REGISTRYINDEX);
	lua_pushvalue(L, 1);
	data->widget_ud_ref = luaL_ref(L, LUA_REGISTRYINDEX);
	if (!data) {
		return luaL_error(L, "Failed to allocate memory");
	}
	if (lua_isnumber(L, 2) == 1) {

		data->signal_id = g_timeout_add(lua_tonumber(L, 2), G_SOURCE_FUNC(tsuki_signal_time), data);

	} 
	else if (lua_isstring(L, 2) == 1) {
		lua_getfield(L, LUA_REGISTRYINDEX, "ServiceInstance");
		Service *serviceinstance = lua_touserdata(L, -1);

		data->signal_id = g_signal_connect(serviceinstance, lua_tostring(L, 2), G_CALLBACK(data_updated), data);

	}
	else return luaL_error(L, "Failed to connect to signal");

	return 0;
}

luaL_Reg l_tsuki_signal_fns[] = {
	{"connect", l_tsuki_signal_connect},
	{NULL, NULL}
};

void signal_fns_register(lua_State *L) {
	lua_getglobal(L, "tsukilib");

	luaL_newlib(L, l_tsuki_signal_fns);
	lua_setfield(L, 1, "signal");

	lua_pop(L, 1);
}
