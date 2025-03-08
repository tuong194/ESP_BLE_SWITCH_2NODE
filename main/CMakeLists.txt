set(srcs "main.c"
        "RD_BLE/rd_ble_mesh.c"
        "RD_BLE/rd_message_control.c" 
        "RD_Control/rd_control_led_rl.c"
        "RD_Peripheral/rd_gpio.c"
        
        # "./MY_LIB/my_gpio.c" 
        # "./MY_LIB/my_mqtt.c" 
        # "./MY_LIB/wifi_STA.c"
        )

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS  ".")
