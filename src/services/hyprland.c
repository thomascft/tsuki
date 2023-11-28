#include  <stdio.h>
#include <stdlib.h>

#include "gio/gio.h"
#include "services/hyprland.h"



enum {
	HYPRLAND_CHANGED,
	HYPRLAND_LAST_SIGNAL
};

static gint hyprland_signals[HYPRLAND_LAST_SIGNAL] = {0};

void register_hyprland_signals(ServiceClass *klass) {
	hyprland_signals[HYPRLAND_CHANGED] = g_signal_new(
		"hyprland-changed",
		G_TYPE_FROM_CLASS(klass),
		G_SIGNAL_RUN_FIRST,
		0,
		NULL, NULL, NULL,
		G_TYPE_NONE,
		1,
		G_TYPE_STRING
	);
}
gboolean read_hyprland_socket(CallbackData *data) {
	while (TRUE) {
		GError *error = NULL;	
		gchar *event = g_data_input_stream_read_line(data->stream, NULL, NULL, &error);

		if (error != NULL) {
			g_print("Failed to read line from hyprland socket: %s\n", error->message);
			break;
		}

		g_print("Output is: %s\n", event);
		g_signal_emit(data->service, hyprland_signals[HYPRLAND_CHANGED], 0, event);
	}

	free(data);

	return false;
}
void init_hyprland_service(Service *service) {
	char *instance_signature = getenv("HYPRLAND_INSTANCE_SIGNATURE");

	if (instance_signature == NULL) {
		g_print("HYPRLAND_INSTANCE_SIGNATURE is not set");
		return;
	}

	GString *path = g_string_new("/tmp/hypr/");
	g_string_append(path, instance_signature);
	g_string_append(path, "/.socket2.sock");

	GSocketAddress *address = g_unix_socket_address_new(path->str); 

	GError *error = NULL;
	GSocketClient *client = g_socket_client_new();
	GSocketConnection *connection = g_socket_client_connect(client, G_SOCKET_CONNECTABLE(address), NULL, &error);

	if (error != NULL) {
		g_print("Failed to connect to hyprland ipc: %s\n", error->message);
		g_error_free(error);
		return;
	}

	GInputStream *input_stream = g_io_stream_get_input_stream(G_IO_STREAM(connection));

	GDataInputStream *data_stream = g_data_input_stream_new(input_stream);

	CallbackData *data = (CallbackData *)malloc(sizeof(CallbackData));
	data->service = service;
	data->stream = data_stream;
	GThread *thread = g_thread_new("ReadHyprlandSocket", (GThreadFunc)read_hyprland_socket, data);

	if (thread == NULL) {
		g_print("Failed to create socket reader thread");
	}
}

