// File: connect-wifi.c
// Last Updated: Galen                23 Apr 2013
//
//
// Commentary: using nm_device_wifi_request_scan_simple() to scan wifi networks
// Since: NetworkManager-0.9.8
/*
 * Compile with:
 *  gcc -Wall `pkg-config --libs --cflags glib-2.0 dbus-glib-1 libnm-glib libnm-util` wifi-scan.c -o wifi-scan
 */

#include <stdio.h>
#include <stdlib.h>

#include <glib.h>
#include <nm-client.h>
#include <nm-device-wifi.h>

/* Global variables */
GMainLoop *loop = NULL; /* Main loop variable - needed for waiting for signal */

static void
signal_handler (int signo)
{
        if (signo == SIGINT || signo == SIGTERM) {
                g_message ("Caught signal %d, shutting down...", signo);
                g_main_loop_quit (loop);
        }
}

static void
setup_signals (void)
{
        struct sigaction action;
        sigset_t mask;

        sigemptyset (&mask);
        action.sa_handler = signal_handler;
        action.sa_mask = mask;
        action.sa_flags = 0;
        sigaction (SIGTERM,  &action, NULL);
        sigaction (SIGINT,  &action, NULL);
}

void scan_done_cb(NMDeviceWifi *device, GError *error, gpointer user_data)
{
        if (error) {
                const char *text = "failed to scan";
                char *err_text = g_strdup_printf ("(%d) %s", error->code,
                                                  error->message ? error->message : "Unknown error");

                g_warning ("%s: %s", text, err_text);
                g_free (err_text);
        } else {
                g_print("scan done\n");
        }

        g_main_loop_quit (loop);
}

int main(int argc, char *argv[])
{
        NMClient *nm_client;
        NMDevice *device;
        const char *iface = "wlan0";

        /* Initialize GType system */
        g_type_init ();

        /* Get NMClient object */
        nm_client = nm_client_new ();
        if (!nm_client) {
                g_message ("Error: Could not create NMClient.");
                return EXIT_FAILURE;
        }

        device = nm_client_get_device_by_iface (nm_client, iface);
        if (!device || NM_IS_DEVICE_WIFI (device) == FALSE) {
                g_warning("The wifi device(wlan0) could not be found.");
                goto out;
        }

        nm_device_wifi_request_scan_simple(NM_DEVICE_WIFI (device), scan_done_cb, NULL);

        loop = g_main_loop_new (NULL, FALSE);  /* Create main loop */
        setup_signals ();                      /* Setup UNIX signals */
        g_main_loop_run (loop);                /* Run main loop until scan done! */
        g_main_loop_unref (loop);

out:
        g_object_unref (nm_client);

        return 0;
}

// Change Log:
// 23 Apr 2013        Galen        Created
