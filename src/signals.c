#include <gtk/gtk.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

typedef struct {
	lua_State *L;
	int widget_ud_ref;
	int fn_ref;
	guint signal_id;
} TsukiCallbackData;

bool tsuki_signal_time(gpointer P) {
	TsukiCallbackData *data = (TsukiCallbackData *)P;
	lua_State *L = data->L;
	lua_rawgeti(L, LUA_REGISTRYINDEX, data->fn_ref);
	lua_rawgeti(L, LUA_REGISTRYINDEX, data->widget_ud_ref);
	lua_call(L, 1, 0);

	return true;
}


int l_tsuki_signal_connect(lua_State *L) {
	if (lua_isnumber(L, 2) == 1 && lua_isfunction(L, 3)) {
		TsukiCallbackData *data = (TsukiCallbackData *)malloc(sizeof(TsukiCallbackData));
		if (!data) {
			return luaL_error(L, "Failed to allocate memory");
		}

		data->L = L; // FIXME: Investigate lua state lifetime
		data->fn_ref = luaL_ref(L, LUA_REGISTRYINDEX);
		lua_pushvalue(L, 1);
		data->widget_ud_ref = luaL_ref(L, LUA_REGISTRYINDEX);
		data->signal_id = g_timeout_add(lua_tonumber(L, 2), G_SOURCE_FUNC(tsuki_signal_time), data);
	}

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
