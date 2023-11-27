#include "service.h"
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

struct _Service {
	GObject parent_instance;
};
struct _ServiceClass {
	GObjectClass parent_class;
};

G_DEFINE_TYPE(Service, service, G_TYPE_OBJECT)

enum {
	DATA_UPDATED,
	NUM_SIGNALS
};

static gint service_signals[NUM_SIGNALS] = {0};

static void service_class_init(ServiceClass *klass) {
	service_signals[DATA_UPDATED] = g_signal_new(
		"data-updated",
		G_TYPE_FROM_CLASS(klass),
		G_SIGNAL_RUN_FIRST,
		0,
		NULL, NULL, NULL,
		G_TYPE_NONE,
		1,
		G_TYPE_STRING
	);
}

static void service_init(Service *self) {

}

void service_update_data(Service *self, const gchar *new_data) {
	g_signal_emit(self, service_signals[DATA_UPDATED], 0, new_data);
}

Service *service_new() {
	return g_object_new(SERVICE_TYPE, NULL);
}

int l_service_update_data(lua_State *L) {
	const gchar *user_data = lua_tostring(L, 1);
	lua_getfield(L, LUA_REGISTRYINDEX, "ServiceInstance");
	Service *service = lua_touserdata(L, -1);


	service_update_data(service, user_data);

	return 0;
}


static const luaL_Reg l_service_fns[] = {
	{"update_data", l_service_update_data},
	{NULL, NULL}
};

void service_fns_register(lua_State *L) {
	Service *serviceinstance = service_new();
	lua_pushlightuserdata(L, serviceinstance);
	lua_setfield(L, LUA_REGISTRYINDEX, "ServiceInstance");

	lua_getglobal(L, "tsukilib");
	luaL_newlib(L, l_service_fns);
	lua_setfield(L, -2, "service");
	lua_pop(L, 1);
}
