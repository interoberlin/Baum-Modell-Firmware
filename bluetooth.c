
#include "bluetooth.h"

baumhaus_tree_service_t tree_service;
ble_gap_sec_params_t    ble_security_parameters;
uint16_t                ble_connection_handle = BLE_CONN_HANDLE_INVALID;

extern void on_ble_connected();
extern void on_ble_disconnected();
extern void on_characteristic_written(uint16_t uuid, uint8_t* data, uint8_t length);


void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name)
{
    //printf("Error #%d. Restarting.\n", error_code);

    // On assert, the system can only recover with a reset.
    NVIC_SystemReset();
}


void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(0xDEADBEEF, line_num, p_file_name);
}


/**
 * @brief Initialize SoftDevice
 */
void init_softdevice()
{
    // Initialize the SoftDevice's event handler
    SOFTDEVICE_HANDLER_INIT(NRF_CLOCK_LFCLKSRC_XTAL_20_PPM, false);
}


// defined below
void ble_event_handler(ble_evt_t*);


/**
 * @brief Initialize Bluetooth Low Energy stack
 */
void init_ble_stack()
{
    ble_enable_params_t ble_enable_params;
    memset(&ble_enable_params, 0, sizeof(ble_enable_params));
    //ble_enable_params.gatts_enable_params.service_changed = 1;
    sd_ble_enable(&ble_enable_params);

    // Subscribe to all BLE events
    softdevice_ble_evt_handler_set(ble_event_handler);
}


/**
 * @brief Initialize Generic Access Profile (GAP)
 */
void init_gap()
{
    ble_gap_conn_params_t   gap_conn_params;
    ble_gap_conn_sec_mode_t sec_mode;

    /*
     * Set security mode
     */
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

    sd_ble_gap_device_name_set(
        &sec_mode,
        (const uint8_t *) BLE_DEVICE_NAME,
        strlen(BLE_DEVICE_NAME)
        );

    /*
     * Set connection parameters
     */
    memset(&gap_conn_params, 0, sizeof(gap_conn_params));

    gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
    gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
    gap_conn_params.slave_latency     = SLAVE_LATENCY;
    gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

    sd_ble_gap_ppcp_set(&gap_conn_params);

    /*
     * Change MAC address, if necessary
     */
    ble_gap_addr_t gap_address;

    sd_ble_gap_address_get(&gap_address);
/*
    printf("Dis' your default MAC: %x:%x:%x:%x:%x:%x\n",
          gap_address.addr[5],
          gap_address.addr[4],
          gap_address.addr[3],
          gap_address.addr[2],
          gap_address.addr[1],
          gap_address.addr[0]
    );
*/
    gap_address.addr_type = BLE_GAP_ADDR_TYPE_PUBLIC;
    sd_ble_gap_address_set(BLE_GAP_ADDR_CYCLE_MODE_NONE, &gap_address);
//    gap_address.addr_type = BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_NON_RESOLVABLE;
//    sd_ble_gap_address_set(BLE_GAP_ADDR_CYCLE_MODE_AUTO, &gap_address);
/*
    printf("MAC address, I choose you: %x:%x:%x:%x:%x:%x\n",
          gap_address.addr[5],
          gap_address.addr[4],
          gap_address.addr[3],
          gap_address.addr[2],
          gap_address.addr[1],
          gap_address.addr[0]
    );
*/
}


/**
 * @brief Initialize GATT services
 */
void init_services()
{
    /*
    // add vendor-specific UUID to BLE stack's table
    ble_uuid128_t   base_uuid = {{0xFB, 0x34, 0x9B, 0x5F, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x0F, 0x18, 0x00, 0x00}};

    tree_service.uuid = BLE_UUID_BAUMHAUS_TREE_SERVICE;
    tree_service.uuid_type = BLE_UUID_TYPE_BLE;

    sd_ble_uuid_vs_add(&base_uuid, &tree_service.uuid_type);
    */

    // add service
    tree_service.uuid.uuid = BLE_UUID_BAUMHAUS_TREE_SERVICE;
    tree_service.uuid.type = BLE_UUID_TYPE_BLE;

    sd_ble_gatts_service_add(
        BLE_GATTS_SRVC_TYPE_PRIMARY,
        &tree_service.uuid,
        &tree_service.service_handle
        );
}


/**
 * @brief Initialize the set_led_color characteristic
 */
void init_characteristic_set_led_color()
{
    ble_gatts_char_md_t characteristic_metadata;
    ble_gatts_attr_md_t attribute_metadata;
    ble_gatts_attr_t    gatt_attribute;

    memset(&characteristic_metadata, 0, sizeof(characteristic_metadata));

    characteristic_metadata.char_props.write            = 1;
    characteristic_metadata.char_props.write_wo_resp    = 1;
    characteristic_metadata.p_char_user_desc            = NULL;
    characteristic_metadata.p_char_pf                   = NULL;
    characteristic_metadata.p_user_desc_md              = NULL;
    characteristic_metadata.p_cccd_md                   = NULL;
    characteristic_metadata.p_sccd_md                   = NULL;

    memset(&attribute_metadata, 0, sizeof(attribute_metadata));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attribute_metadata.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attribute_metadata.write_perm);

    attribute_metadata.vloc     = BLE_GATTS_VLOC_STACK;
    attribute_metadata.rd_auth  = 0;
    attribute_metadata.wr_auth  = 0;
    attribute_metadata.vlen     = 1;

    memset(&gatt_attribute, 0, sizeof(gatt_attribute));

    tree_service.characteristic_set_led_color_uuid.uuid = BLE_UUID_SET_LED_COLOR_CHARACTERISTIC;
    tree_service.characteristic_set_led_color_uuid.type = tree_service.uuid.type;

    gatt_attribute.p_uuid       = &tree_service.characteristic_set_led_color_uuid;
    gatt_attribute.p_attr_md    = &attribute_metadata;
    gatt_attribute.init_len     = 1;
    gatt_attribute.init_offs    = 0;
    gatt_attribute.max_len      = 20;

    sd_ble_gatts_characteristic_add(
        tree_service.service_handle,
        &characteristic_metadata,
        &gatt_attribute,
        &tree_service.characteristic_set_led_color_handles
        );
}


/**
 * @brief Initialize all characteristics
 */
void init_characteristics()
{
    init_characteristic_set_led_color();
}


/**
 * @brief Initialize BLE advertiser
 */
void init_advertising(void)
{
    ble_advdata_t advertising_data;
    ble_advdata_t scan_response_data;
    uint8_t       flags = BLE_GAP_ADV_FLAGS_LE_ONLY_LIMITED_DISC_MODE;

    ble_uuid_t adv_uuids[] =
        {
            {tree_service.uuid.uuid, tree_service.uuid.type}
        };

    memset(&advertising_data, 0, sizeof(advertising_data));
    advertising_data.name_type               = BLE_ADVDATA_FULL_NAME;
//    advdata.include_appearance      = false;
    advertising_data.include_appearance      = true;
    advertising_data.flags.size              = sizeof(flags);
    advertising_data.flags.p_data            = &flags;

    memset(&scan_response_data, 0, sizeof(scan_response_data));
    scan_response_data.uuids_complete.uuid_cnt = sizeof(adv_uuids) / sizeof(adv_uuids[0]);
    scan_response_data.uuids_complete.p_uuids  = adv_uuids;

    ble_advdata_set(&advertising_data, &scan_response_data);
}


/**
 * @brief Send value update notifications for all subscribed characteristics
 */
void send_notifications()
{
    ble_gatts_hvx_params_t hvx_params;

    memset(&hvx_params, 0, sizeof(hvx_params));

    hvx_params.handle = tree_service.characteristic_set_led_color_handles.value_handle;
    hvx_params.p_data = "123";
    hvx_params.p_len  = "\3";
    hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;

    //uint32_t err_code =
    sd_ble_gatts_hvx(ble_connection_handle, &hvx_params);
}


/**
 * @brief Initialize security parameters
 */
void init_security_parameters()
{
    ble_security_parameters.timeout      = SEC_PARAM_TIMEOUT;
    ble_security_parameters.bond         = SEC_PARAM_BOND;
    ble_security_parameters.mitm         = SEC_PARAM_MITM;
    ble_security_parameters.io_caps      = SEC_PARAM_IO_CAPABILITIES;
    ble_security_parameters.oob          = SEC_PARAM_OOB;
    ble_security_parameters.min_key_size = SEC_PARAM_MIN_KEY_SIZE;
    ble_security_parameters.max_key_size = SEC_PARAM_MAX_KEY_SIZE;
}


/**
 * @brief Handler for events of the Connection Parameters Module
 */
void connection_parameters_event_handler(ble_conn_params_evt_t * p_evt)
{
    if(p_evt->evt_type == BLE_CONN_PARAMS_EVT_FAILED)
    {
        sd_ble_gap_disconnect(ble_connection_handle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
    }
}


/**
 * @brief Error handler for the Connection Parameters module
 */
void connection_paramaters_error_handler(uint32_t nrf_error)
{
    // TODO
}


/**
 * @brief Initialize Connection Parameters module
 */
void init_connection_parameters()
{
    ble_conn_params_init_t cp_init;

    memset(&cp_init, 0, sizeof(cp_init));

    cp_init.p_conn_params                  = NULL;
    cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
    cp_init.next_conn_params_update_delay  = NEXT_CONN_PARAMS_UPDATE_DELAY;
    cp_init.max_conn_params_update_count   = MAX_CONN_PARAMS_UPDATE_COUNT;
    cp_init.start_on_notify_cccd_handle    = BLE_GATT_HANDLE_INVALID;
    cp_init.disconnect_on_fail             = false;
    cp_init.evt_handler                    = connection_parameters_event_handler;
    cp_init.error_handler                  = connection_paramaters_error_handler;

    ble_conn_params_init(&cp_init);
}


/**
 * @brief Begin to send advertising packets
 */
void start_advertising()
{
    ble_gap_adv_params_t adv_params;

    // Start advertising
    memset(&adv_params, 0, sizeof(adv_params));

    adv_params.type        = BLE_GAP_ADV_TYPE_ADV_IND;
    adv_params.p_peer_addr = NULL;
    adv_params.fp          = BLE_GAP_ADV_FP_ANY;
    adv_params.interval    = APP_ADV_INTERVAL;
    adv_params.timeout     = APP_ADV_TIMEOUT_IN_SECONDS;

    sd_ble_gap_adv_start(&adv_params);

    // active low
    nrf_gpio_pin_clear(PIN_LED_ADVERTISING);
}


/**
 * @brief Handler for all BLE events
 */
void ble_event_handler(ble_evt_t* p_ble_evt)
{
    static ble_gap_evt_auth_status_t m_auth_status;
    ble_gap_enc_info_t *             p_enc_info;
    ble_gatts_evt_write_t*           p_evt_write;

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            // active low
            nrf_gpio_pin_set(PIN_LED_ADVERTISING);
            // active high
            nrf_gpio_pin_set(PIN_LED_CONNECTED);

            ble_connection_handle = p_ble_evt->evt.gap_evt.conn_handle;

            on_ble_connected();

            break;

        case BLE_GAP_EVT_DISCONNECTED:
            // active low
            nrf_gpio_pin_clear(PIN_LED_ADVERTISING);
            // active high
            nrf_gpio_pin_clear(PIN_LED_CONNECTED);

            ble_connection_handle = BLE_CONN_HANDLE_INVALID;

            on_ble_disconnected();

            start_advertising();

            break;

        case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
            sd_ble_gap_sec_params_reply(
                ble_connection_handle,
                BLE_GAP_SEC_STATUS_SUCCESS,
                &ble_security_parameters
                );
            break;

        case BLE_GATTS_EVT_WRITE:
            p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;

            if (p_evt_write->handle == tree_service.characteristic_set_led_color_handles.value_handle)
            {
                on_characteristic_written(tree_service.characteristic_set_led_color_uuid.uuid, p_evt_write->data, p_evt_write->len);
            }
            break;

        case BLE_GATTS_EVT_SYS_ATTR_MISSING:
            sd_ble_gatts_sys_attr_set(ble_connection_handle, NULL, 0);
            break;

        case BLE_GAP_EVT_AUTH_STATUS:
            m_auth_status = p_ble_evt->evt.gap_evt.params.auth_status;
            break;

        case BLE_GAP_EVT_SEC_INFO_REQUEST:
            p_enc_info = &m_auth_status.periph_keys.enc_info;
            if (p_enc_info->div == p_ble_evt->evt.gap_evt.params.sec_info_request.div)
            {
                sd_ble_gap_sec_info_reply(ble_connection_handle, p_enc_info, NULL);
            }
            else
            {
                // No keys found for this device
                sd_ble_gap_sec_info_reply(ble_connection_handle, NULL, NULL);
            }
            break;

        case BLE_GAP_EVT_TIMEOUT:
            if (p_ble_evt->evt.gap_evt.params.timeout.src == BLE_GAP_TIMEOUT_SRC_ADVERTISEMENT)
            {
                nrf_gpio_pin_clear(PIN_LED_CONNECTED);

                // Go to system-off mode (this function will not return; wakeup will cause a reset)
                //err_code = sd_power_system_off();
                NVIC_SystemReset();
            }
            break;
/*
        case BLE_EVT_TX_COMPLETE:
            if (!ble_buffer_available)
                tx_complete = true;
            break;
*/
        default:
            //printf("unimplemented event");
            // No implementation needed.
            break;
    }
}


/**
 * @brief Initialize Bluetooth Low Energy and start advertising
 */
void ble_init()
{
    nrf_gpio_cfg_output(PIN_LED_ADVERTISING);
    nrf_gpio_cfg_output(PIN_LED_CONNECTED);

    init_softdevice();
    init_ble_stack();
    init_gap();
    init_services();
    init_characteristics();
    init_advertising();
    init_connection_parameters();
    init_security_parameters();

    start_advertising();
}
