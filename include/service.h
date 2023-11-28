#pragma once

#include <gtk/gtk.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

G_BEGIN_DECLS

#define SERVICE_TYPE (service_get_type())
G_DECLARE_FINAL_TYPE(Service, service, TSUKI, SERVICE, GObject)

Service *service_new(void);

void service_update_data(Service *self, const gchar *new_data);

G_END_DECLS

void services_init(lua_State *L);
