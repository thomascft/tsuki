#ifndef SIGNAL_H_
#define SIGNAL_H_

#include <lua.h>
#include <gtk/gtk.h>


typedef struct {
	lua_State *L;
	int widget_ud_ref;
	int fn_ref;
	guint signal_id;
} CallbackUserData;

bool signal_time(gpointer P);

int l_signal_connect(lua_State *L);

void signal_fns_register(lua_State *L);

#endif 
