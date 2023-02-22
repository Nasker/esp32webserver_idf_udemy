# See the build system documentation in IDF programming guide
# for more information about component CMakeLists.txt files.

idf_component_register(SRCS main.c rgb_led.c wifi_app.c http_server.c ds18b20_sensor.c ultrasonic_sensor.c app_nvs.c wifi_reset_button.c sntp_time_sync.c mqtt_demo_mutual_auth.c
                        INCLUDE_DIRS "."
                        EMBED_FILES webpage/app.css webpage/app.js webpage/favicon.ico webpage/index.html webpage/jquery-3.3.1.min.js
                        ) 
target_add_binary_data(${COMPONENT_TARGET} "certs/aws_root_ca.pem" TEXT)
target_add_binary_data(${COMPONENT_TARGET} "certs/certificate_pem.crt" TEXT)
target_add_binary_data(${COMPONENT_TARGET} "certs/private_pem.key" TEXT)