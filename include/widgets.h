#ifndef WIDGETS_H_
#define WIDGETS_H_

#include <lua.h>

int l_window_new(lua_State *L);
int l_window_get_child(lua_State *L); //TODO: Needs impl tested

int l_window_set_child(lua_State *L);

int l_window_present(lua_State *L);

int l_widget_get_classes(lua_State *L);
int l_widget_get_valign(lua_State *L);
int l_widget_get_halign(lua_State *L);
int l_widget_get_vexpand(lua_State *L);
int l_widget_get_hexpand(lua_State *L);
int l_widget_get_height(lua_State *L);
int l_widget_get_width(lua_State *L);
int l_widget_get_tooltip(lua_State *L);
int l_widget_get_visible(lua_State *L);

int l_widget_set_classes(lua_State *L);
int l_widget_set_valign(lua_State *L);
int l_widget_set_halign(lua_State *L);
int l_widget_set_vexpand(lua_State *L);
int l_widget_set_hexpand(lua_State *L);
int l_widget_set_size(lua_State *L);
int l_widget_set_tooltip(lua_State *L);
int l_widget_set_visible(lua_State *L);

int l_label_new(lua_State *L);

int l_label_get_text(lua_State *L);

int l_label_set_text(lua_State *L);

void widget_fns_register(lua_State *L);

#endif
