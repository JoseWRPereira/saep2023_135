#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_mac.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_http_server.h"

#include "saidas.h"

#define CONFIG_ESP_WIFI_SSID            "SAEP2023_CFP135"
#define CONFIG_ESP_WIFI_PASSWORD        "saep@2023"
#define CONFIG_ESP_WIFI_CHANNEL         1
#define CONFIG_ESP_MAX_STA_CONN         5



#define EXAMPLE_ESP_WIFI_SSID      CONFIG_ESP_WIFI_SSID
#define EXAMPLE_ESP_WIFI_PASS      CONFIG_ESP_WIFI_PASSWORD
#define EXAMPLE_ESP_WIFI_CHANNEL   CONFIG_ESP_WIFI_CHANNEL
#define EXAMPLE_MAX_STA_CONN       CONFIG_ESP_MAX_STA_CONN

static const char *TAG = "wifi softAP";

static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                                    int32_t event_id, void* event_data)
{
    if (event_id == WIFI_EVENT_AP_STACONNECTED) {
        wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
        ESP_LOGI(TAG, "station "MACSTR" join, AID=%d",
                 MAC2STR(event->mac), event->aid);
    } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
        wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
        ESP_LOGI(TAG, "station "MACSTR" leave, AID=%d",
                 MAC2STR(event->mac), event->aid);
    }
}

void wifi_init_softap(void)
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        NULL));

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .ssid_len = strlen(EXAMPLE_ESP_WIFI_SSID),
            .channel = EXAMPLE_ESP_WIFI_CHANNEL,
            .password = EXAMPLE_ESP_WIFI_PASS,
            .max_connection = EXAMPLE_MAX_STA_CONN,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK,
            .pmf_cfg = {
                    .required = false,
            },
        },
    };
    if (strlen(EXAMPLE_ESP_WIFI_PASS) == 0) {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

	esp_wifi_connect();
    ESP_LOGI(TAG, "wifi_init_softap finished. SSID:%s password:%s channel:%d",
             EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS, EXAMPLE_ESP_WIFI_CHANNEL);
}

void nvs_init(void)
{
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "ESP_WIFI_MODE_AP");
}


esp_err_t get_handler_entradas(httpd_req_t *req)
{
    char resp[] = "<!DOCTYPE HTML>\
<html><head><title>SAEP2023</title>\
<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\
</head><body><center><h1>SAEP 2023</h1><hr><br>Saidas\
<h3>\
<form action=\"/saidas\">\
<input type=\"checkbox\" name=\"K1\" value=\"1\" checke >\
<label for=\"K1\">Tomada1</label><br>\
<input type=\"checkbox\" name=\"K2\" value=\"1\" checke >\
<label for=\"K2\">Tomada2</label><br>\
<input type=\"checkbox\" name=\"K3\" value=\"1\" checke >\
<label for=\"K3\">Tomada3</label><br>\
<input type=\"checkbox\" name=\"K4\" value=\"1\" checke >\
<label for=\"K4\">Tomada4</label>\
</h3>\
<input type=\"submit\" value=\"Atualizar\">\
</form>\
</center>\
</body>\
</html>";

    resp[247] = saida_get(T1E) ? 'd':' ';
    resp[333] = saida_get(T2E) ? 'd':' ';
    resp[419] = saida_get(T3E) ? 'd':' ';
    resp[505] = saida_get(T4E) ? 'd':' ';
    // ESP_LOGI(TAG, "The checkeD %c %c %c", resp[417], resp[418], resp[419]);
    // ESP_LOGI(TAG, "The checkeD %c %c %c", resp[503], resp[504], resp[505]);


    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}


esp_err_t get_handler_saidas(httpd_req_t *req)
{
    // Read the URI line and get the host
    char *buf;
    size_t buf_len;
    buf_len = httpd_req_get_hdr_value_len(req, "Host") + 1;
    if (buf_len > 1)
    {
        buf = malloc(buf_len);
        if (httpd_req_get_hdr_value_str(req, "Host", buf, buf_len) == ESP_OK)
        {
            ESP_LOGI(TAG, "Host: %s", buf);
        }
        free(buf);
    }

    // Read the URI line and get the parameters
    buf_len = httpd_req_get_url_query_len(req) + 1;
    if (buf_len > 1) {
        buf = malloc(buf_len);
        if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
            ESP_LOGI(TAG, "Found URL query: %s", buf);
            char param[32];

            if (httpd_query_key_value(buf, "K1", param, sizeof(param)) == ESP_OK) 
            {
                ESP_LOGI(TAG, "The K1 value = %s", param);
                saida_set(T1E,1);
            }
            else
            {
                saida_set(T1E,0);
            }

            if (httpd_query_key_value(buf, "K2", param, sizeof(param)) == ESP_OK) 
            {
                ESP_LOGI(TAG, "The K2 value = %s", param);
                saida_set(T2E,1);
            }
            else
            {
                saida_set(T2E,0);
            }

            if (httpd_query_key_value(buf, "K3", param, sizeof(param)) == ESP_OK) 
            {
                ESP_LOGI(TAG, "The K3 value = %s", param);
                saida_set(T3E,1);
            }
            else
            {
                saida_set(T3E,0);
            }

            if (httpd_query_key_value(buf, "K4", param, sizeof(param)) == ESP_OK) 
            {
                ESP_LOGI(TAG, "The K4 value = %s", param);
                saida_set(T4E,1);
            }
            else
            {
                saida_set(T4E,0);
            }

        }
        free(buf);
    }
    else
    {
        saida_set(T1E,0);
        saida_set(T2E,0);
        saida_set(T3E,0);
        saida_set(T4E,0);
    }

    get_handler_entradas(req);
    return ESP_OK;
}








/* URI handler structure for GET /uri */
httpd_uri_t uri_get = {
    .uri = "/",
    .method = HTTP_GET,
    .handler = get_handler_saidas,
    .user_ctx = NULL};

httpd_uri_t uri_get_saidas= {
    .uri = "/saidas",
    .method = HTTP_GET,
    .handler = get_handler_saidas,
    .user_ctx = NULL};

httpd_uri_t uri_get_entradas = {
    .uri = "/entradas",
    .method = HTTP_GET,
    .handler = get_handler_entradas,
    .user_ctx = NULL};




httpd_handle_t start_webserver(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;
    if (httpd_start(&server, &config) == ESP_OK)
    {
        httpd_register_uri_handler(server, &uri_get);
        httpd_register_uri_handler(server, &uri_get_saidas);
        httpd_register_uri_handler(server, &uri_get_entradas);
    }
    return server;
}

void stop_webserver(httpd_handle_t server)
{
    if (server)
    {
        httpd_stop(server);
    }
}




void wifi_init( void )
{
    nvs_init();
    wifi_init_softap();
    start_webserver();
}