#pragma once

#include "service.h"

typedef struct {
	Service *service;
	GDataInputStream *stream;

} CallbackData;

void register_hyprland_signals(ServiceClass *klass);
gboolean read_hyprland_socket(CallbackData *data);
void init_hyprland_service(Service *service);
