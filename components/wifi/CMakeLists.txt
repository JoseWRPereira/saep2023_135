idf_component_register(SRCS "wifi.c"
                    REQUIRES driver
                    REQUIRES esp_wifi
                    REQUIRES nvs_flash
                    REQUIRES esp_netif
                    REQUIRES esp_http_server
                    REQUIRES saidas
                    INCLUDE_DIRS "include")
