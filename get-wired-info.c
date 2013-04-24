// File: get-wired-info.c
// Last Updated: Galen                23 Apr 2013
//
// Commentary:
//
/*
 * Compile with:
 *  gcc -Wall `pkg-config --libs --cflags glib-2.0 dbus-glib-1 libnm-glib libnm-util` get-wired-info.c -o get-wired-info
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glib.h>
#include <nm-client.h>
#include <nm-utils.h>
#include <nm-device-ethernet.h>

const gchar *
device_state_to_localized_string (NMDevice *device)
{
        NMDeviceType type;
        NMDeviceState state;

        type = nm_device_get_device_type (device);
        state = nm_device_get_state (device);

        const gchar *value = NULL;
        switch (state) {
        case NM_DEVICE_STATE_UNKNOWN:
                /* TRANSLATORS: device status */
                value = "Status unknown";
                break;
        case NM_DEVICE_STATE_UNMANAGED:
                /* TRANSLATORS: device status */
                value = "Unmanaged";
                break;
        case NM_DEVICE_STATE_UNAVAILABLE:
                /* TRANSLATORS: device status */
                if (nm_device_get_firmware_missing (device))
                        value = "Firmware missing";
                else if (type == NM_DEVICE_TYPE_ETHERNET &&
                         !nm_device_ethernet_get_carrier (NM_DEVICE_ETHERNET (device)))
                        value = "Cable unplugged";
                else
                        value = "Unavailable";
                break;
        case NM_DEVICE_STATE_DISCONNECTED:
                /* TRANSLATORS: device status */
                value = "Disconnected";
                break;
        case NM_DEVICE_STATE_PREPARE:
        case NM_DEVICE_STATE_CONFIG:
        case NM_DEVICE_STATE_IP_CONFIG:
        case NM_DEVICE_STATE_IP_CHECK:
                /* TRANSLATORS: device status */
                value = "Connecting";
                break;
        case NM_DEVICE_STATE_NEED_AUTH:
                /* TRANSLATORS: device status */
                value = "Authentication required";
                break;
        case NM_DEVICE_STATE_ACTIVATED:
                /* TRANSLATORS: device status */
                value = "Connected";
                break;
        case NM_DEVICE_STATE_DEACTIVATING:
                /* TRANSLATORS: device status */
                value = "Disconnecting";
                break;
        case NM_DEVICE_STATE_FAILED:
                /* TRANSLATORS: device status */
                value = "Connection failed";
                break;
        default:
                /* TRANSLATORS: device status */
                value = "Status unknown (missing)";
                break;
        }
        return value;
}

const gchar *
device_state_reason_to_localized_string (NMDevice *device)
{
        const gchar *value = NULL;
        NMDeviceStateReason state_reason;

        /* we only want the StateReason's we care about */
        nm_device_get_state_reason (device, &state_reason);
        switch (state_reason) {
        case NM_DEVICE_STATE_REASON_CONFIG_FAILED:
                /* TRANSLATORS: device status reason */
                value = "Configuration failed";
                break;
        case NM_DEVICE_STATE_REASON_IP_CONFIG_UNAVAILABLE:
                /* TRANSLATORS: device status reason */
                value = "IP configuration failed";
                break;
        case NM_DEVICE_STATE_REASON_IP_CONFIG_EXPIRED:
                /* TRANSLATORS: device status reason */
                value = "IP configuration expired";
                break;
        case NM_DEVICE_STATE_REASON_NO_SECRETS:
                /* TRANSLATORS: device status reason */
                value = "Secrets were required, but not provided";
                break;
        case NM_DEVICE_STATE_REASON_SUPPLICANT_DISCONNECT:
                /* TRANSLATORS: device status reason */
                value = "802.1x supplicant disconnected";
                break;
        case NM_DEVICE_STATE_REASON_SUPPLICANT_CONFIG_FAILED:
                /* TRANSLATORS: device status reason */
                value = "802.1x supplicant configuration failed";
                break;
        case NM_DEVICE_STATE_REASON_SUPPLICANT_FAILED:
                /* TRANSLATORS: device status reason */
                value = "802.1x supplicant failed";
                break;
        case NM_DEVICE_STATE_REASON_SUPPLICANT_TIMEOUT:
                /* TRANSLATORS: device status reason */
                value = "802.1x supplicant took too long to authenticate";
                break;
        case NM_DEVICE_STATE_REASON_PPP_START_FAILED:
                /* TRANSLATORS: device status reason */
                value = "PPP service failed to start";
                break;
        case NM_DEVICE_STATE_REASON_PPP_DISCONNECT:
                /* TRANSLATORS: device status reason */
                value = "PPP service disconnected";
                break;
        case NM_DEVICE_STATE_REASON_PPP_FAILED:
                /* TRANSLATORS: device status reason */
                value = "PPP failed";
                break;
        case NM_DEVICE_STATE_REASON_DHCP_START_FAILED:
                /* TRANSLATORS: device status reason */
                value = "DHCP client failed to start";
                break;
        case NM_DEVICE_STATE_REASON_DHCP_ERROR:
                /* TRANSLATORS: device status reason */
                value = "DHCP client error";
                break;
        case NM_DEVICE_STATE_REASON_DHCP_FAILED:
                /* TRANSLATORS: device status reason */
                value = "DHCP client failed";
                break;
        case NM_DEVICE_STATE_REASON_SHARED_START_FAILED:
                /* TRANSLATORS: device status reason */
                value = "Shared connection service failed to start";
                break;
        case NM_DEVICE_STATE_REASON_SHARED_FAILED:
                /* TRANSLATORS: device status reason */
                value = "Shared connection service failed";
                break;
        case NM_DEVICE_STATE_REASON_AUTOIP_START_FAILED:
                /* TRANSLATORS: device status reason */
                value = "AutoIP service failed to start";
                break;
        case NM_DEVICE_STATE_REASON_AUTOIP_ERROR:
                /* TRANSLATORS: device status reason */
                value = "AutoIP service error";
                break;
        case NM_DEVICE_STATE_REASON_AUTOIP_FAILED:
                /* TRANSLATORS: device status reason */
                value = "AutoIP service failed";
                break;
        case NM_DEVICE_STATE_REASON_MODEM_BUSY:
                /* TRANSLATORS: device status reason */
                value = "Line busy";
                break;
        case NM_DEVICE_STATE_REASON_MODEM_NO_DIAL_TONE:
                /* TRANSLATORS: device status reason */
                value = "No dial tone";
                break;
        case NM_DEVICE_STATE_REASON_MODEM_NO_CARRIER:
                /* TRANSLATORS: device status reason */
                value = "No carrier could be established";
                break;
        case NM_DEVICE_STATE_REASON_MODEM_DIAL_TIMEOUT:
                /* TRANSLATORS: device status reason */
                value = "Dialing request timed out";
                break;
        case NM_DEVICE_STATE_REASON_MODEM_DIAL_FAILED:
                /* TRANSLATORS: device status reason */
                value = "Dialing attempt failed";
                break;
        case NM_DEVICE_STATE_REASON_MODEM_INIT_FAILED:
                /* TRANSLATORS: device status reason */
                value = "Modem initialization failed";
                break;
        case NM_DEVICE_STATE_REASON_GSM_APN_FAILED:
                /* TRANSLATORS: device status reason */
                value = "Failed to select the specified APN";
                break;
        case NM_DEVICE_STATE_REASON_GSM_REGISTRATION_NOT_SEARCHING:
                /* TRANSLATORS: device status reason */
                value = "Not searching for networks";
                break;
        case NM_DEVICE_STATE_REASON_GSM_REGISTRATION_DENIED:
                /* TRANSLATORS: device status reason */
                value = "Network registration denied";
                break;
        case NM_DEVICE_STATE_REASON_GSM_REGISTRATION_TIMEOUT:
                /* TRANSLATORS: device status reason */
                value = "Network registration timed out";
                break;
        case NM_DEVICE_STATE_REASON_GSM_REGISTRATION_FAILED:
                /* TRANSLATORS: device status reason */
                value = "Failed to register with the requested network";
                break;
        case NM_DEVICE_STATE_REASON_GSM_PIN_CHECK_FAILED:
                /* TRANSLATORS: device status reason */
                value = "PIN check failed";
                break;
        case NM_DEVICE_STATE_REASON_FIRMWARE_MISSING:
                /* TRANSLATORS: device status reason */
                value = "Firmware for the device may be missing";
                break;
        case NM_DEVICE_STATE_REASON_CONNECTION_REMOVED:
                /* TRANSLATORS: device status reason */
                value = "Connection disappeared";
                break;
        case NM_DEVICE_STATE_REASON_CARRIER:
                /* TRANSLATORS: device status reason */
                value = "Carrier/link changed";
                break;
        case NM_DEVICE_STATE_REASON_CONNECTION_ASSUMED:
                /* TRANSLATORS: device status reason */
                value = "Existing connection was assumed";
                break;
        case NM_DEVICE_STATE_REASON_MODEM_NOT_FOUND:
                /* TRANSLATORS: device status reason */
                value = "Modem not found";
                break;
        case NM_DEVICE_STATE_REASON_BT_FAILED:
                /* TRANSLATORS: device status reason */
                value = "Bluetooth connection failed";
                break;
        case NM_DEVICE_STATE_REASON_GSM_SIM_NOT_INSERTED:
                /* TRANSLATORS: device status reason */
                value = "SIM Card not inserted";
                break;
        case NM_DEVICE_STATE_REASON_GSM_SIM_PIN_REQUIRED:
                /* TRANSLATORS: device status reason */
                value = "SIM Pin required";
                break;
        case NM_DEVICE_STATE_REASON_GSM_SIM_PUK_REQUIRED:
                /* TRANSLATORS: device status reason */
                value = "SIM Puk required";
                break;
        case NM_DEVICE_STATE_REASON_GSM_SIM_WRONG:
                /* TRANSLATORS: device status reason */
                value = "SIM wrong";
                break;
        case NM_DEVICE_STATE_REASON_INFINIBAND_MODE:
                /* TRANSLATORS: device status reason */
                value = "InfiniBand device does not support connected mode";
                break;
        case NM_DEVICE_STATE_REASON_DEPENDENCY_FAILED:
                /* TRANSLATORS: device status reason */
                value = "Connection dependency failed";
                break;
        default:
                /* no StateReason to show */
                break;
        }
        return value;
}

static gchar *
get_ipv4_config_address_as_string (NMIP4Config *ip4_config, const char *what)
{
        const GSList *list;
        struct in_addr addr;
        gchar *str = NULL;
        gchar tmp[INET_ADDRSTRLEN];
        NMIP4Address *address;

        /* get address */
        list = nm_ip4_config_get_addresses (ip4_config);
        if (list == NULL)
                goto out;

        /* we only care about one address */
        address = list->data;
        if (!strcmp (what, "address"))
                addr.s_addr = nm_ip4_address_get_address (address);
        else if (!strcmp (what, "gateway"))
                addr.s_addr = nm_ip4_address_get_gateway (address);
        else if (!strcmp (what, "netmask"))
                addr.s_addr = nm_utils_ip4_prefix_to_netmask (nm_ip4_address_get_prefix (address));
        else
                goto out;

        if (!inet_ntop (AF_INET, &addr, tmp, sizeof(tmp)))
                goto out;
        if (g_strcmp0 (tmp, "0.0.0.0") == 0)
                goto out;
        str = g_strdup (tmp);
out:
        return str;
}

static gchar *
get_ipv4_config_name_servers_as_string (NMIP4Config *ip4_config)
{
        const GArray *array;
        GString *dns;
        struct in_addr addr;
        gchar tmp[INET_ADDRSTRLEN];
        int i;
        gchar *str = NULL;

        array = nm_ip4_config_get_nameservers (ip4_config);
        if (array == NULL || array->len == 0)
                goto out;

        dns = g_string_new (NULL);
        for (i = 0; i < array->len; i++) {
                addr.s_addr = g_array_index (array, guint32, i);
                if (inet_ntop (AF_INET, &addr, tmp, sizeof(tmp)))
                        g_string_append_printf (dns, "%s;", tmp);
        }
        str = g_string_free (dns, FALSE);
out:
        return str;
}

static void
get_wired_info (NMDevice *nm_device)
{
        const char *str;
        GString *status;
        guint speed = 0;
        NMDeviceState state;
        const gchar *status_reason;
        gboolean device_visible;
        NMIP4Config *ip4_config = NULL;
        gchar *str_tmp;

        state = nm_device_get_state (nm_device);
        device_visible = (state != NM_DEVICE_STATE_UNAVAILABLE && state != NM_DEVICE_STATE_UNMANAGED);
        if (!device_visible) {
                g_print("device is unavailable or unmanaged");
                return;
        }

        /* set device state, with status and optionally speed */
        status = g_string_new (device_state_to_localized_string (nm_device));
        if (state != NM_DEVICE_STATE_UNAVAILABLE)
                speed = nm_device_ethernet_get_speed (NM_DEVICE_ETHERNET (nm_device));
        if (speed  > 0) {
                g_string_append (status, " - ");
                /* Translators: network device speed */
                g_string_append_printf (status, "%d Mb/s", speed);
        }
        g_print("Status: %s\n", status->str);

        // gtk_label_set_label (GTK_LABEL (widget), status->str);
        g_string_free (status, TRUE);

        status_reason = device_state_reason_to_localized_string (nm_device);
        if (status_reason)
                g_print("status_reason = %s\n", status_reason);

        /* device MAC */
        str = nm_device_ethernet_get_hw_address (NM_DEVICE_ETHERNET (nm_device));
        g_print("Hardware Address: %s\n", str);

        /* get IPv4 parameters */
        ip4_config = nm_device_get_ip4_config (nm_device);
        if (ip4_config != NULL) {

                /* IPv4 address */
                str_tmp = get_ipv4_config_address_as_string (ip4_config, "address");
                g_print("IP Address: %s\n", str_tmp);
                g_free (str_tmp);

                /* IPv4 address */
                str_tmp = get_ipv4_config_address_as_string (ip4_config, "netmask");
                g_print("Netmask: %s\n", str_tmp);
                g_free (str_tmp);

                /* IPv4 route */
                str_tmp = get_ipv4_config_address_as_string (ip4_config, "gateway");
                g_print("Default Route: %s\n", str_tmp);
                g_free (str_tmp);

                /* IPv4 DNS */
                str_tmp = get_ipv4_config_name_servers_as_string (ip4_config);
                g_print("DNS: %s\n", str_tmp);
                g_free (str_tmp);
        }
}

int main(int argc, char *argv[])
{
        NMClient *nm_client;
        NMDevice *device;
        const char *iface = "eth0";

        /* Initialize GType system */
        g_type_init ();

        /* Get NMClient object */
        nm_client = nm_client_new ();
        if (!nm_client) {
                g_warning ("Error: Could not create NMClient.");
                return EXIT_FAILURE;
        }

        device = nm_client_get_device_by_iface (nm_client, iface);
        if (!device || NM_IS_DEVICE_ETHERNET (device) == FALSE) {
                g_warning("The ethernet device(%s) could not be found.", iface);
                goto out;
        }

        get_wired_info (device);

out:
        g_object_unref (nm_client);

        return 0;
}

// Change Log:
// 23 Apr 2013        Galen        Created
