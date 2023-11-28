#include "service.h"
#include "services/hyprland.h"
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

	register_hyprland_signals(klass);
}

static void service_init(Service *self) {

}

void service_update_data(Service *self, const gchar *new_data) {
	g_signal_emit(self, service_signals[DATA_UPDATED], 0, new_data);
}

Service *service_new() {
	return g_object_new(SERVICE_TYPE, NULL);
}
void services_init(lua_State *L) {
	Service *service = service_new();
	lua_pushlightuserdata(L, service);
	lua_setfield(L, LUA_REGISTRYINDEX, "ServiceInstance");

	init_hyprland_service(service);
}
