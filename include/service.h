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

int l_service_new(lua_State *L);
int l_service_update_data(lua_State *L);
void service_fns_register(lua_State *L);
