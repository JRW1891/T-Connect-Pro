/*
 * @Description: 出厂测试程序
 * @Author: LILYGO_L
 * @Date: 2025-02-05 13:48:33
 * @LastEditTime: 2025-06-04 17:02:56
 * @License: GPL 3.0
 */

#include <Arduino.h>
#include "Arduino_GFX_Library.h"
#include "pin_config.h"
#include "Wire.h"
#include "WiFi.h"
#include <HTTPClient.h>
#include "Arduino_DriveBus_Library.h"
#include "Material_16Bit_222x480px.h"
#include <SPI.h>
#include <SD.h>
#include <FS.h>
#include <Ethernet.h>
#include "utility/w5100.h"
#include "driver/twai.h"

#define SOFTWARE_NAME "Original_Test"

#define SOFTWARE_LASTEDITTIME "202506041434"
#define BOARD_VERSION "V1.0"

#define WIFI_SSID "xinyuandianzi"
#define WIFI_PASSWORD "AA15994823428"
// #define WIFI_SSID "LilyGo-AABB"
// #define WIFI_PASSWORD "xinyuandianzi"

#define WIFI_CONNECT_WAIT_MAX (10000)

#define NTP_SERVER1 "pool.ntp.org"
#define NTP_SERVER2 "time.nist.gov"
#define NTP_SERVER3 "asia.pool.ntp.org"
#define GMT_OFFSET_SEC 8 * 3600 // Time zone setting function, written as 8 * 3600 in East Eighth Zone (UTC/GMT+8:00)
#define DAY_LIGHT_OFFSET_SEC 0  // Fill in 3600 for daylight saving time, otherwise fill in 0

// Intervall:
#define POLLING_RATE_MS 100
#define TWAI_TRANSMIT_OVERTIME_MS 1000

// 文件下载链接
// const char *fileDownloadUrl = "https://code.visualstudio.com/docs/?dv=win64user";//vscode
// const char *fileDownloadUrl = "https://dldir1.qq.com/qqfile/qq/PCQQ9.7.17/QQ9.7.17.29225.exe"; // 腾讯CDN加速
// const char *fileDownloadUrl = "https://cd001.www.duba.net/duba/install/packages/ever/kinsthomeui_150_15.exe"; // 金山毒霸
const char *fileDownloadUrl = "https://freetyst.nf.migu.cn/public/product9th/product45/2022/05/0716/2018%E5%B9%B409%E6%9C%8812%E6%97%A510%E7%82%B943%E5%88%86%E7%B4%A7%E6%80%A5%E5%86%85%E5%AE%B9%E5%87%86%E5%85%A5%E5%8D%8E%E7%BA%B3179%E9%A6%96/%E6%A0%87%E6%B8%85%E9%AB%98%E6%B8%85/MP3_128_16_Stero/6005751EPFG164228.mp3?channelid=02&msisdn=d43a7dcc-8498-461b-ba22-3205e9b6aa82&Tim=1728484238063&Key=0442fa065dacda7c";
// const char *fileDownloadUrl = "https://github.com/espressif/arduino-esp32/releases/download/3.0.1/esp32-3.0.1.zip";

const uint64_t Local_MAC = ESP.getEfuseMac();

bool Wifi_Connection_Flag = true;
size_t CycleTime = 0;
size_t CycleTime_2 = 0;
size_t CycleTime_3 = 0;
size_t CycleTime_4 = 0;
size_t CycleTime_5 = 0;
size_t CycleTime_6 = 0;
size_t CycleTime_7 = 0;
uint8_t Image_Flag = 0;

bool Skip_Current_Test = false;

uint8_t Current_Rotation = 1;

struct RS485_Operator
{
    using state = enum {
        UNCONNECTED, // 未连接
        CONNECTED,   // 已连接
        CONNECTING,  // 正在连接
        PAUSE,       // 传输数据错误暂停连接
    };

    struct
    {
        struct
        {
            String code = "null";
            uint8_t count = 0;
        } error;

        uint32_t send_data = 0;
        uint32_t receive_data = 0;
        uint8_t connection_status = state::UNCONNECTED;
        bool send_flag = false;
    } device_1;

    struct
    {
        struct
        {
            String code = "null";
            uint8_t count = 0;
        } error;

        uint32_t send_data = 0;
        uint32_t receive_data = 0;
        uint8_t connection_status = state::UNCONNECTED;
        bool send_flag = false;

    } device_2;

    uint8_t send_package_1[105] =
        {
            0x0A, // 设备标头识别

            // 动态数据计数
            0B00000000, // 数据高2位
            0B00000000, // 数据高1位
            0B00000000, // 数据低2位
            0B00000000, // 数据低1位

            // 100个数据包
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
    };

    uint8_t send_package_2[105] =
        {
            0x0A, // 设备标头识别

            // 动态数据计数
            0B00000000, // 数据高2位
            0B00000000, // 数据高1位
            0B00000000, // 数据低2位
            0B00000000, // 数据低1位

            // 100个数据包
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
            0xAA,
    };
};

struct CAN_Operator
{
    using state = enum {
        UNCONNECTED, // 未连接
        CONNECTED,   // 已连接
        CONNECTING,  // 正在连接
        PAUSE,       // 传输数据错误暂停连接
    };

    struct
    {
        struct
        {
            String code = "null";
            uint8_t count = 0;
        } error;

        uint32_t send_data = 0;
        uint32_t receive_data = 0;
        uint8_t connection_status = state::UNCONNECTED;
        bool send_flag = false;
    } device_1;
};

struct Ethernet_Relay_Operator
{
    struct
    {
        String code = "null";
        bool flag = false;
    } initialization;

    // Web relay trigger flag
    bool html_relay1_flag = false;
};

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network.
// gateway and subnet are optional:
// The MAC address must be an even number first !
uint8_t mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 1, 177);
IPAddress myDns(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

RS485_Operator RS485_OP;
CAN_Operator CAN_OP;
Ethernet_Relay_Operator Ethernet_Relay_OP;

Arduino_DataBus *bus = new Arduino_HWSPI(
    SCREEN_DC /* DC */, SCREEN_CS /* CS */, SCREEN_SCLK /* SCK */,
    SCREEN_MOSI /* MOSI */, SCREEN_MISO /* MISO */);

Arduino_GFX *gfx = new Arduino_ST7796(
    bus, SCREEN_RST /* RST */, Current_Rotation /* rotation */, true /* IPS */,
    SCREEN_WIDTH /* width */, SCREEN_HEIGHT /* height */,
    49 /* col offset 1 */, 0 /* row offset 1 */, 49 /* col_offset2 */, 0 /* row_offset2 */);

std::shared_ptr<Arduino_IIC_DriveBus> IIC_Bus =
    std::make_shared<Arduino_HWIIC>(IIC_SDA, IIC_SCL, &Wire1);

std::shared_ptr<Arduino_IIC_DriveBus> IIC_Bus_2 =
    std::make_shared<Arduino_HWIIC>(IIC_SDA_2, IIC_SCL_2, &Wire);

void Arduino_IIC_Touch_Interrupt(void);
void Arduino_IIC_RTC_Interrupt(void);

std::unique_ptr<Arduino_IIC> CST226SE(new Arduino_CST2xxSE(IIC_Bus, CST226SE_DEVICE_ADDRESS,
                                                           TOUCH_RST, TOUCH_INT, Arduino_IIC_Touch_Interrupt));

std::unique_ptr<Arduino_IIC> PCF8563(new Arduino_PCF8563(IIC_Bus_2, PCF8563_DEVICE_ADDRESS,
                                                         DRIVEBUS_DEFAULT_VALUE, PCF8563_INT, Arduino_IIC_RTC_Interrupt));

void Arduino_IIC_RTC_Interrupt(void)
{
    PCF8563->IIC_Interrupt_Flag = true;
}
void Arduino_IIC_Touch_Interrupt(void)
{
    CST226SE->IIC_Interrupt_Flag = true;
}

void Lora_Operation_Interrupt(void)
{
#ifdef T_Connect_Pro_V1_0_SX1262
    // we sent or received a packet, set the flag
    Lora_Op.operation_flag = true;
#endif
}

bool Touch_Rotation_Convert(int32_t *x, int32_t *y)
{
    if ((*x == -1) && (*y == -1))
    {
        return false;
    }

    int32_t x_buffer = *x;
    int32_t y_buffer = *y;

    switch (Current_Rotation)
    {
    case 0:
        break;
    case 1:
        *x = y_buffer;
        *y = SCREEN_WIDTH - x_buffer;
        break;
    case 2:
        *x = SCREEN_WIDTH - x_buffer;
        *y = SCREEN_HEIGHT - y_buffer;
        break;
    case 3:
        *x = SCREEN_HEIGHT - y_buffer;
        *y = x_buffer;
        break;

    default:
        break;
    }
    return true;
}

void Skip_Test_Loop()
{
    uint8_t fingers_number = CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH_FINGER_NUMBER);

    if (fingers_number > 0)
    {
        int32_t touch_x = CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH_COORDINATE_X);
        int32_t touch_y = CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH_COORDINATE_Y);

        Touch_Rotation_Convert(&touch_x, &touch_y);

        switch (Current_Rotation)
        {
        case 0:
        case 2:
            if (touch_x > 40 && touch_x < 140 &&
                touch_y > SCREEN_HEIGHT - (SCREEN_HEIGHT / 3) && touch_y < SCREEN_HEIGHT - (SCREEN_HEIGHT / 3) + 40)
            {
                Skip_Current_Test = true;
            }
            break;
        case 1:
        case 3:
            if (touch_x > (SCREEN_HEIGHT / 3 + 10) && touch_x < (SCREEN_HEIGHT / 3 + 10) + 140 &&
                touch_y > 155 && touch_y < 195)
            {
                Skip_Current_Test = true;
            }
            break;

        default:
            break;
        }
    }
}

bool Uart_Check_Dynamic_Data(uint8_t *uart_data, uint32_t check_data)
{
    uint32_t temp = (uint32_t)uart_data[1] << 24 | (uint32_t)uart_data[2] << 16 |
                    (uint32_t)uart_data[3] << 8 | (uint32_t)uart_data[4];

    if (temp == 0)
    {
        if (temp == check_data)
        {
            return true;
        }
    }
    else
    {
        if (temp == check_data + 1)
        {
            return true;
        }
    }

    return false;
}

bool Uart_Check_Static_Data(uint8_t *uart_data, uint8_t *check_data)
{
    if (memcmp(&uart_data[5], &check_data[5], 100) == 0)
    {
        return true;
    }
    return false;
}

void GFX_Print_Ethernet_Relay_Button()
{
    switch (Current_Rotation)
    {
    case 0:
    case 2:
        gfx->fillRect(SCREEN_WIDTH / 4, SCREEN_HEIGHT - (SCREEN_HEIGHT / 3) - 30, SCREEN_WIDTH / 2, 50, PALERED);
        gfx->drawRect(SCREEN_WIDTH / 4, SCREEN_HEIGHT - (SCREEN_HEIGHT / 3) - 30, SCREEN_WIDTH / 2, 50, RED);
        gfx->setTextSize(1);
        gfx->setTextColor(WHITE);

        gfx->setCursor(SCREEN_WIDTH / 4 + 25, SCREEN_HEIGHT - (SCREEN_HEIGHT / 3) - 40 + 30);
        gfx->printf("Relay Test");
        break;
    case 1:
    case 3:
        gfx->fillRect(SCREEN_HEIGHT - (SCREEN_HEIGHT / 4) + 20, 140, SCREEN_WIDTH / 2 - 30, 50, PALERED);
        gfx->drawRect(SCREEN_HEIGHT - (SCREEN_HEIGHT / 4) + 20, 140, SCREEN_WIDTH / 2 - 30, 50, RED);
        gfx->setTextSize(1);
        gfx->setTextColor(WHITE);

        gfx->setCursor(SCREEN_HEIGHT - (SCREEN_HEIGHT / 4) + 30, 140 + 20);
        gfx->printf("Relay Test");
        break;

    default:
        break;
    }
}

bool GFX_Print_Ethernet_Relay_Button_Trigger(int32_t x, int32_t y)
{
    if ((x == -1) && (y == -1))
    {
        return false;
    }

    switch (Current_Rotation)
    {
    case 0:
    case 2:
        if (x > SCREEN_WIDTH / 4 && x < SCREEN_WIDTH / 4 + SCREEN_WIDTH / 2 &&
            y > SCREEN_HEIGHT - (SCREEN_HEIGHT / 3) - 30 && y < SCREEN_HEIGHT - (SCREEN_HEIGHT / 3) - 30 + 50)
        {
            return true;
        }
        break;
    case 1:
    case 3:
        if (x > (SCREEN_HEIGHT - (SCREEN_HEIGHT / 4) + 20) && x < ((SCREEN_HEIGHT - (SCREEN_HEIGHT / 4) + 20) + (SCREEN_WIDTH / 2 - 30)) &&
            y > 140 && y < 190)
        {
            return true;
        }
        break;

    default:
        break;
    }

    return false;
}

void GFX_Print_RS485_CAN_Connect_Button()
{
    switch (Current_Rotation)
    {
    case 0:
    case 2:
        gfx->fillRect(SCREEN_WIDTH / 4, SCREEN_HEIGHT - (SCREEN_HEIGHT / 3) - 30, SCREEN_WIDTH / 2, 50, PALERED);
        gfx->drawRect(SCREEN_WIDTH / 4, SCREEN_HEIGHT - (SCREEN_HEIGHT / 3) - 30, SCREEN_WIDTH / 2, 50, RED);
        gfx->setTextSize(1);
        gfx->setTextColor(WHITE);

        gfx->setCursor(SCREEN_WIDTH / 4 + 30, SCREEN_HEIGHT - (SCREEN_HEIGHT / 3) - 40 + 30);
        gfx->printf("Reconnect");
        break;
    case 1:
    case 3:
        gfx->fillRect(SCREEN_HEIGHT - (SCREEN_HEIGHT / 4) + 20, 140, SCREEN_WIDTH / 2 - 30, 50, PALERED);
        gfx->drawRect(SCREEN_HEIGHT - (SCREEN_HEIGHT / 4) + 20, 140, SCREEN_WIDTH / 2 - 30, 50, RED);
        gfx->setTextSize(1);
        gfx->setTextColor(WHITE);

        gfx->setCursor(SCREEN_HEIGHT - (SCREEN_HEIGHT / 4) + 35, 140 + 20);
        gfx->printf("Reconnect");
        break;

    default:
        break;
    }
}

bool GFX_Print_RS485_CAN_Connect_Button_Trigger(int32_t x, int32_t y)
{
    if ((x == -1) && (y == -1))
    {
        return false;
    }

    switch (Current_Rotation)
    {
    case 0:
    case 2:
        if (x > SCREEN_WIDTH / 4 && x < SCREEN_WIDTH / 4 + SCREEN_WIDTH / 2 &&
            y > SCREEN_HEIGHT - (SCREEN_HEIGHT / 3) - 30 && y < SCREEN_HEIGHT - (SCREEN_HEIGHT / 3) - 30 + 50)
        {
            return true;
        }
        break;
    case 1:
    case 3:
        if (x > (SCREEN_HEIGHT - (SCREEN_HEIGHT / 4) + 20) && x < ((SCREEN_HEIGHT - (SCREEN_HEIGHT / 4) + 20) + (SCREEN_WIDTH / 2 - 30)) &&
            y > 140 && y < 190)
        {
            return true;
        }
        break;

    default:
        break;
    }

    return false;
}

void Ethernet_Reset(const uint8_t resetPin)
{
    pinMode(resetPin, OUTPUT);
    digitalWrite(resetPin, HIGH);
    delay(250);
    digitalWrite(resetPin, LOW);
    delay(50);
    digitalWrite(resetPin, HIGH);
    delay(350);
}

void Twai_Send_Message()
{
    // Configure message to transmit
    twai_message_t message;
    message.identifier = 0xF1;
    // message.data_length_code = 1;
    // message.data[0] = CAN_Count++;
    message.data_length_code = 8;
    message.data[0] = 0x0A;
    message.data[1] = 0x0A;
    message.data[2] = 0x0A;
    message.data[3] = 0x0A;
    message.data[4] = (uint8_t)(CAN_OP.device_1.send_data >> 24);
    message.data[5] = (uint8_t)(CAN_OP.device_1.send_data >> 16);
    message.data[6] = (uint8_t)(CAN_OP.device_1.send_data >> 8);
    message.data[7] = (uint8_t)CAN_OP.device_1.send_data;

    // Queue message for transmission
    if (twai_transmit(&message, pdMS_TO_TICKS(TWAI_TRANSMIT_OVERTIME_MS)) == ESP_OK)
    {
        printf("Message queued for transmission\n");
    }
    else
    {
        printf("Failed to queue message for transmission\n");
    }
}

void Twai_Receive_Message(twai_message_t &message)
{
    // Process received message
    if (message.extd)
    {
        Serial.println("Message is in Extended Format");
    }
    else
    {
        Serial.println("Message is in Standard Format");
    }
    Serial.printf("ID: 0x%X\n", message.identifier);
    if (!(message.rtr))
    {
        for (int i = 0; i < message.data_length_code; i++)
        {
            Serial.printf("Data [%d] = %d\n", i, message.data[i]);
        }
        Serial.println("");

        if ((message.data[0] == 0x0A) &&
            (message.data[1] == 0x0A) &&
            (message.data[2] == 0x0A) &&
            (message.data[3] == 0x0A))
        {
            CAN_OP.device_1.receive_data =
                ((uint32_t)message.data[4] << 24) |
                ((uint32_t)message.data[5] << 16) |
                ((uint32_t)message.data[6] << 8) |
                (uint32_t)message.data[7];

            CAN_OP.device_1.send_data = CAN_OP.device_1.receive_data + 1;
            CAN_OP.device_1.connection_status = CAN_OP.state::CONNECTED;
            // 清除错误计数看门狗
            CAN_OP.device_1.error.count = 0;
            CAN_OP.device_1.send_flag = true;
            CycleTime_7 = millis() + 500;
        }
    }
}

bool Ethernet_Initialization_Assertion(String *assertion)
{
    switch (Ethernet.hardwareStatus())
    {
    case EthernetNoHardware:
        // no point in carrying on, so do nothing forevermore:

        Serial.println("Ethernet No Hardware");
        *assertion = "Ethernet No Hardware";
        return false;

        break;
    case EthernetW5100:
        Serial.println("Ethernet W5100 Discovery !");
        break;
    case EthernetW5200:
        Serial.println("Ethernet W5200 Discovery !");
        break;
    case EthernetW5500:
        Serial.println("Ethernet W5500 Discovery !");
        break;
    }

    switch (Ethernet.linkStatus())
    {
    case Unknown:
        // no point in carrying on, so do nothing forevermore:

        Serial.print("Link status: ");
        Serial.println("Unknown");
        Serial.println("Hardware error !");

        *assertion = "Hardware error";
        return false;

        break;
    case LinkON:
        Serial.print("Link status: ");
        Serial.println("ON");
        break;
    case LinkOFF:
        Serial.print("Link status: ");
        Serial.println("OFF");
        Serial.println("The network cable is not connected !");

        *assertion = "Please insert the network cable";
        return false;

        break;
    }

    // start the Ethernet connection:
    Serial.println("Trying to get an IP address using DHCP...");
    if (Ethernet.begin(mac, 10000, 10000) == 0)
    {
        // initialize the Ethernet device not using DHCP:
        // Ethernet.begin(mac, ip, myDns, gateway, subnet);

        Serial.println("-------------------------");
        Serial.println("[INFO] Configuring random DHCP failed !");
        Serial.println("");
        // Serial.println("[INFO] Configuring static IP...");
        // Serial.print("[Static] IP Address: ");
        // Serial.println(Ethernet.localIP());
        // Serial.print("[Static] Subnet Mask: ");
        // Serial.println(Ethernet.subnetMask());
        // Serial.print("[Static] Gateway: ");
        // Serial.println(Ethernet.gatewayIP());
        // Serial.print("[Static] DNS: ");
        // Serial.println(Ethernet.dnsServerIP());
        // Serial.println("-------------------------");
        // Serial.println("");

        *assertion = "DHCP configuration failed";
        return false;
    }
    else
    {
        Serial.println("-------------------------");
        Serial.println("[INFO] Configuring random DHCP successfully !");
        Serial.println("");
        Serial.print("[DHCP] IP Address: ");
        Serial.println(Ethernet.localIP());
        Serial.print("[DHCP] Subnet Mask: ");
        Serial.println(Ethernet.subnetMask());
        Serial.print("[DHCP] Gateway: ");
        Serial.println(Ethernet.gatewayIP());
        Serial.print("[DHCP] DNS: ");
        Serial.println(Ethernet.dnsServerIP());
        Serial.println("-------------------------");
        Serial.println("");
    }
    return true;
}

void RS485_Initialization(void)
{
    while (Serial1.available() > 0) // 清空缓存
    {
        Serial1.read();
    }
    while (Serial2.available() > 0) // 清空缓存
    {
        Serial2.read();
    }
}

void CAN_Drive_Initialization(void)
{
    // Initialize configuration structures using macro initializers
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT((gpio_num_t)CAN_TX, (gpio_num_t)CAN_RX, TWAI_MODE_NORMAL);
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_1MBITS();
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

    // Install TWAI driver
    if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK)
    {
        Serial.println("Driver installed");
    }
    else
    {
        Serial.println("Failed to install driver");
    }

    // Start TWAI driver
    if (twai_start() == ESP_OK)
    {
        Serial.println("Driver started");
    }
    else
    {
        Serial.println("Failed to start driver");
    }

    // 配置
    uint32_t alerts_to_enable = TWAI_ALERT_TX_IDLE | TWAI_ALERT_TX_SUCCESS |
                                TWAI_ALERT_TX_FAILED | TWAI_ALERT_ERR_PASS |
                                TWAI_ALERT_BUS_ERROR | TWAI_ALERT_RX_DATA |
                                TWAI_ALERT_RX_QUEUE_FULL;

    if (twai_reconfigure_alerts(alerts_to_enable, NULL) == ESP_OK)
    {
        Serial.println("CAN Alerts reconfigured");
    }
    else
    {
        Serial.println("Failed to reconfigure alerts");
    }
}

void Ethernet_Initialization(void)
{
    SPI.begin(W5500_SCLK, W5500_MISO, W5500_MOSI, W5500_CS);
    Ethernet_Reset(W5500_RST);
    Ethernet.init(W5500_CS);

    W5100.init();
}

void Wifi_STA_Test(void)
{
    String text;
    int wifi_num = 0;

    gfx->fillScreen(WHITE);
    gfx->setCursor(0, 0);
    gfx->setTextSize(1);
    gfx->setTextColor(BLACK);

    Serial.println("\nScanning wifi");
    gfx->printf("\nScanning wifi\n");
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    wifi_num = WiFi.scanNetworks();
    if (wifi_num == 0)
    {
        text = "\nWiFi scan complete !\nNo wifi discovered.\n";
    }
    else
    {
        text = "\nWiFi scan complete !\n";
        text += wifi_num;
        text += " wifi discovered.\n\n";

        for (int i = 0; i < wifi_num; i++)
        {
            text += (i + 1);
            text += ": ";
            text += WiFi.SSID(i);
            text += " (";
            text += WiFi.RSSI(i);
            text += ")";
            text += (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " \n" : "*\n";
            delay(10);
        }
    }

    Serial.println(text);
    gfx->println(text);

    delay(3000);
    text.clear();
    gfx->fillScreen(WHITE);
    gfx->setCursor(0, 0);

    Serial.print("Connecting to ");
    gfx->printf("\nConnecting to\n");

    Serial.print(WIFI_SSID);
    gfx->printf("%s", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    uint32_t last_tick = millis();

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        gfx->printf(".");
        delay(100);

        if (millis() - last_tick > WIFI_CONNECT_WAIT_MAX)
        {
            Wifi_Connection_Flag = false;
            break;
        }
    }

    if (Wifi_Connection_Flag == true)
    {
        Serial.print("\nThe connection was successful ! \nTakes ");
        gfx->printf("\nThe connection was successful ! \nTakes ");
        Serial.print(millis() - last_tick);
        gfx->print(millis() - last_tick);
        Serial.println(" ms\n");
        gfx->printf(" ms\n");

        gfx->setTextColor(PURPLE);
        gfx->printf("\nWifi test passed!");
    }
    else
    {
        gfx->setTextColor(RED);
        gfx->printf("\nWifi test error!\n");
    }
}

void WIFI_HTTP_Download_File(void)
{
    // 初始化HTTP客户端
    HTTPClient http;
    http.begin(fileDownloadUrl);
    // 获取重定向的URL
    const char *headerKeys[] = {"Location"};
    http.collectHeaders(headerKeys, 1);

    // 记录下载开始时间
    size_t startTime = millis();
    // 无用时间
    size_t uselessTime = 0;

    // 发起GET请求
    int httpCode = http.GET();

    while (httpCode == HTTP_CODE_MOVED_PERMANENTLY || httpCode == HTTP_CODE_FOUND)
    {
        String newUrl = http.header("Location");
        Serial.printf("Redirecting to: %s\n", newUrl.c_str());
        http.end(); // 关闭旧的HTTP连接

        // 使用新的URL重新发起GET请求
        http.begin(newUrl);
        httpCode = http.GET();
    }

    if (httpCode == HTTP_CODE_OK)
    {
        // 获取文件大小
        size_t fileSize = http.getSize();
        Serial.printf("Starting file download...\n");
        Serial.printf("file size: %f MB\n", fileSize / 1024.0 / 1024.0);
        gfx->printf("\nStarting file download...\n");
        gfx->printf("file size: %f MB\n", fileSize / 1024.0 / 1024.0);

        // 读取HTTP响应
        WiFiClient *stream = http.getStreamPtr();

        size_t temp_count_s = 0;
        size_t temp_fileSize = fileSize;
        // uint8_t *buf_1 = (uint8_t *)heap_caps_malloc(64 * 1024, MALLOC_CAP_SPIRAM);
        uint8_t buf_1[4096] = {0};
        CycleTime = millis() + 3000; // 开始计时
        while (http.connected() && (temp_fileSize > 0 || temp_fileSize == -1))
        {
            // 获取可用数据的大小
            size_t availableSize = stream->available();
            if (availableSize)
            {
                // temp_fileSize -= stream->read(buf_1, min(availableSize, (size_t)(64 * 1024)));
                temp_fileSize -= stream->read(buf_1, min(availableSize, (size_t)(4096)));

                if (millis() > CycleTime)
                {
                    size_t temp_time_1 = millis();
                    temp_count_s += 3;
                    Serial.printf("Download speed: %f KB/s\n", ((fileSize - temp_fileSize) / 1024.0) / temp_count_s);
                    Serial.printf("Remaining file size: %f MB\n\n", temp_fileSize / 1024.0 / 1024.0);

                    gfx->fillRect(0, 30, 200, 68, WHITE);
                    gfx->setCursor(0, 30);
                    gfx->printf("Speed: %f KB/s\n", ((fileSize - temp_fileSize) / 1024.0) / temp_count_s);
                    gfx->printf("Size: %f MB\n\n", temp_fileSize / 1024.0 / 1024.0);

                    CycleTime = millis() + 3000;
                    size_t temp_time_2 = millis();

                    uselessTime = uselessTime + (temp_time_2 - temp_time_1);
                }
            }
            if (temp_count_s > 30)
            {
                break;
            }
        }
        // 关闭HTTP客户端
        http.end();

        // 记录下载结束时间并计算总花费时间
        size_t endTime = millis();

        if (temp_count_s < 30)
        {
            Serial.printf("Download completed!\n");
            Serial.printf("Total download time: %f s\n", (endTime - startTime - uselessTime) / 1000.0);
            Serial.printf("Average download speed: %f KB/s\n", (fileSize / 1024.0) / ((endTime - startTime - uselessTime) / 1000.0));

            gfx->printf("Completed!\n");
            gfx->printf("Time: %f s\n", (endTime - startTime - uselessTime) / 1000.0);
            gfx->printf("Speed: %f KB/s\n", (fileSize / 1024.0) / ((endTime - startTime - uselessTime) / 1000.0));
        }
        else
        {
            Serial.printf("Download incomplete!\n");
            Serial.printf("Total download time: %f s\n", (endTime - startTime - uselessTime) / 1000.0);
            Serial.printf("Average download speed: %f KB/s\n", ((fileSize - temp_fileSize) / 1024.0) / ((endTime - startTime - uselessTime) / 1000.0));

            gfx->printf("Completed!\n");
            gfx->printf("Time: %f s\n", (endTime - startTime - uselessTime) / 1000.0);
            gfx->printf("Speed: %f KB/s\n", ((fileSize - temp_fileSize) / 1024.0) / ((endTime - startTime - uselessTime) / 1000.0));
        }
    }
    else
    {
        Serial.printf("Failed to download\n");
        Serial.printf("Error httpCode: %d \n", httpCode);

        gfx->printf("Failed to download\n");
        gfx->printf("Error httpCode: %d \n\n", httpCode);
    }
}

void PrintLocalTime(void)
{
    struct tm timeinfo;
    uint32_t temp_buf = 0;
    bool temp_buf_2 = true;

    while (getLocalTime(&timeinfo) == false)
    {
        Serial.printf("\n.");
        gfx->printf("\n.");
        configTime(GMT_OFFSET_SEC, DAY_LIGHT_OFFSET_SEC, NTP_SERVER1, NTP_SERVER2, NTP_SERVER3);
        temp_buf++;
        if (temp_buf > 5)
        {
            temp_buf_2 = false;
            break;
        }
    }
    if (temp_buf_2 == false)
    {
        Serial.println("Failed to obtain time");
        gfx->setCursor(80, 200);
        gfx->setTextColor(RED);
        gfx->print("Failed to obtain time!");
        return;
    }

    Serial.println("Get time success");
    Serial.println(&timeinfo, "%A,%B %d %Y %H:%M:%S"); // Format Output
    gfx->setCursor(80, 200);
    gfx->setTextColor(PURPLE);
    gfx->print(&timeinfo, " %Y");
    gfx->setCursor(80, 215);
    gfx->print(&timeinfo, "%B %d");
    gfx->setCursor(80, 230);
    gfx->print(&timeinfo, "%H:%M:%S");
}

void GFX_Print_Touch_Info_Loop()
{
    int32_t touch_x_1 = CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH1_COORDINATE_X);
    int32_t touch_y_1 = CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH1_COORDINATE_Y);
    int32_t touch_x_2 = CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH2_COORDINATE_X);
    int32_t touch_y_2 = CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH2_COORDINATE_Y);
    int32_t touch_x_3 = CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH3_COORDINATE_X);
    int32_t touch_y_3 = CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH3_COORDINATE_Y);
    int32_t touch_x_4 = CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH4_COORDINATE_X);
    int32_t touch_y_4 = CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH4_COORDINATE_Y);
    int32_t touch_x_5 = CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH5_COORDINATE_X);
    int32_t touch_y_5 = CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH5_COORDINATE_Y);
    uint8_t fingers_number = CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH_FINGER_NUMBER);

    Touch_Rotation_Convert(&touch_x_1, &touch_y_1);
    Touch_Rotation_Convert(&touch_x_2, &touch_y_2);
    Touch_Rotation_Convert(&touch_x_3, &touch_y_3);
    Touch_Rotation_Convert(&touch_x_4, &touch_y_4);
    Touch_Rotation_Convert(&touch_x_5, &touch_y_5);

    gfx->fillRect(0, 40, SCREEN_WIDTH, SCREEN_HEIGHT / 2, WHITE);
    gfx->setTextSize(1);
    gfx->setTextColor(BLACK);

    gfx->setCursor(20, 50);
    gfx->printf("ID: %#X ", (int32_t)CST226SE->IIC_Device_ID());

    gfx->setCursor(20, 65);
    gfx->printf("Fingers Number:%d ", fingers_number);

    gfx->setCursor(20, 80);
    gfx->printf("Touch X1:%d Y1:%d ", touch_x_1, touch_y_1);
    gfx->setCursor(20, 95);
    gfx->printf("Touch X2:%d Y2:%d ", touch_x_2, touch_y_2);
    gfx->setCursor(20, 110);
    gfx->printf("Touch X3:%d Y3:%d ", touch_x_3, touch_y_3);
    gfx->setCursor(20, 125);
    gfx->printf("Touch X4:%d Y4:%d ", touch_x_4, touch_y_4);
    gfx->setCursor(20, 140);
    gfx->printf("Touch X5:%d Y5:%d ", touch_x_5, touch_y_5);
}

void GFX_Print_Time_Info_Loop()
{
    gfx->fillRoundRect(35, 35, 152, 95, 10, WHITE);
    gfx->setTextSize(1);

    if (Wifi_Connection_Flag == true)
    {
        struct tm timeinfo;
        if (!getLocalTime(&timeinfo, 1000))
        {
            Serial.println("Failed to obtain time");
            gfx->setCursor(50, 45);
            gfx->setTextColor(RED);
            gfx->print("Time error");
            return;
        }
        Serial.println("Get time success");
        Serial.println(&timeinfo, "%A,%B %d %Y %H:%M:%S"); // Format Output
        gfx->setCursor(50, 45);
        gfx->setTextColor(ORANGE);
        gfx->print(&timeinfo, " %Y");
        gfx->setCursor(50, 60);
        gfx->print(&timeinfo, "%B %d");
        gfx->setCursor(50, 75);
        gfx->print(&timeinfo, "%H:%M:%S");
    }
    else
    {
        gfx->setCursor(50, 45);
        gfx->setTextColor(RED);
        gfx->print("Network error");
    }

    gfx->setCursor(50, 90);
    gfx->printf("SYS Time:%d", (uint32_t)millis() / 1000);
}

void GFX_Print_1()
{
    switch (Current_Rotation)
    {
    case 0:
    case 2:
        gfx->fillRect(20, SCREEN_HEIGHT - (SCREEN_HEIGHT / 4), 80, 40, ORANGE);
        gfx->drawRect(20, SCREEN_HEIGHT - (SCREEN_HEIGHT / 4), 80, 40, PURPLE);
        gfx->fillRect(120, SCREEN_HEIGHT - (SCREEN_HEIGHT / 4), 80, 40, PURPLE);
        gfx->drawRect(120, SCREEN_HEIGHT - (SCREEN_HEIGHT / 4), 80, 40, ORANGE);
        gfx->setTextSize(1);
        gfx->setTextColor(WHITE);
        gfx->setCursor(35, SCREEN_HEIGHT - (SCREEN_HEIGHT / 4) + 15);
        gfx->printf("Try Again");
        gfx->setCursor(135, SCREEN_HEIGHT - (SCREEN_HEIGHT / 4) + 15);
        gfx->printf("Next Test");
        break;

    case 1:
    case 3:
        gfx->fillRect(SCREEN_HEIGHT - (SCREEN_HEIGHT / 4) + 20, 20, 80, 40, ORANGE);
        gfx->drawRect(SCREEN_HEIGHT - (SCREEN_HEIGHT / 4) + 20, 20, 80, 40, PURPLE);
        gfx->fillRect(SCREEN_HEIGHT - (SCREEN_HEIGHT / 4) + 20, 80, 80, 40, PURPLE);
        gfx->drawRect(SCREEN_HEIGHT - (SCREEN_HEIGHT / 4) + 20, 80, 80, 40, ORANGE);
        gfx->setTextSize(1);
        gfx->setTextColor(WHITE);
        gfx->setCursor(SCREEN_HEIGHT - (SCREEN_HEIGHT / 4) + 35, 35);
        gfx->printf("Try Again");
        gfx->setCursor(SCREEN_HEIGHT - (SCREEN_HEIGHT / 4) + 35, 95);
        gfx->printf("Next Test");
        break;

    default:
        break;
    }
}

int8_t GFX_Print_1_Trigger(int32_t x, int32_t y)
{
    if ((x == -1) && (y == -1))
    {
        return -1;
    }

    switch (Current_Rotation)
    {
    case 0:
    case 2:
        if (x > 20 && x < 100 &&
            y > (SCREEN_HEIGHT - (SCREEN_HEIGHT / 4)) && y < (SCREEN_HEIGHT - (SCREEN_HEIGHT / 4) + 40))
        {
            return 1; // 触发按键1
        }
        if (x > 120 && x < 200 &&
            y > (SCREEN_HEIGHT - (SCREEN_HEIGHT / 4)) && y < (SCREEN_HEIGHT - (SCREEN_HEIGHT / 4) + 40))
        {
            return 2; // 触发按键2
        }
        break;

    case 1:
    case 3:
        if (x > (SCREEN_HEIGHT - (SCREEN_HEIGHT / 4) + 20) && x < (SCREEN_HEIGHT - (SCREEN_HEIGHT / 4) + 20) + 80 &&
            y > 20 && y < 60)
        {
            return 1; // 触发按键1
        }
        if (x > (SCREEN_HEIGHT - (SCREEN_HEIGHT / 4) + 20) && x < (SCREEN_HEIGHT - (SCREEN_HEIGHT / 4) + 20) + 80 &&
            y > 80 && y < 120)
        {
            return 2; // 触发按键2
        }
        break;

    default:
        break;
    }

    return -1;
}

void GFX_Print_2()
{
    switch (Current_Rotation)
    {
    case 0:
    case 2:
        gfx->fillRect(40, SCREEN_HEIGHT - (SCREEN_HEIGHT / 3), 140, 40, RED);
        gfx->drawRect(40, SCREEN_HEIGHT - (SCREEN_HEIGHT / 3), 140, 40, CYAN);

        gfx->setTextSize(1);
        gfx->setTextColor(WHITE);
        gfx->setCursor(60, SCREEN_HEIGHT - (SCREEN_HEIGHT / 3) + 15);
        gfx->printf("Skip Current Test");
        break;
    case 1:
    case 3:
        gfx->fillRect(SCREEN_HEIGHT / 3 + 10, 155, 140, 40, RED);
        gfx->drawRect(SCREEN_HEIGHT / 3 + 10, 155, 140, 40, CYAN);

        gfx->setTextSize(1);
        gfx->setTextColor(WHITE);
        gfx->setCursor(SCREEN_HEIGHT / 3 + 25, 170);
        gfx->printf("Skip Current Test");
        break;

    default:
        break;
    }
}

void GFX_Print_TEST(String s)
{
    Skip_Current_Test = false;

    switch (Current_Rotation)
    {
    case 0:
    case 2:
        gfx->fillScreen(WHITE);
        gfx->setCursor(SCREEN_WIDTH / 4 + 20, SCREEN_HEIGHT / 4);
        gfx->setTextSize(3);
        gfx->setTextColor(PALERED);
        gfx->printf("TEST");

        gfx->setCursor(20, SCREEN_HEIGHT / 4 + 40);
        gfx->setTextSize(2);
        gfx->setTextColor(BLACK);
        gfx->print(s);

        GFX_Print_2();

        gfx->setCursor(SCREEN_WIDTH / 2 - 17, SCREEN_HEIGHT / 2);
        gfx->setTextSize(4);
        gfx->setTextColor(RED);
        gfx->printf("3");
        delay(300);
        gfx->fillRect(SCREEN_WIDTH / 2 - 17, SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2 + 20, 20, WHITE);
        gfx->setCursor(SCREEN_WIDTH / 2 - 17, SCREEN_HEIGHT / 2);
        gfx->printf("2");
        for (int i = 0; i < 100; i++)
        {
            Skip_Test_Loop();
            delay(10);

            if (Skip_Current_Test == true)
            {
                break;
            }
        }
        gfx->fillRect(SCREEN_WIDTH / 2 - 17, SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2 + 20, 20, WHITE);
        gfx->setCursor(SCREEN_WIDTH / 2 - 17, SCREEN_HEIGHT / 2);
        gfx->printf("1");
        for (int i = 0; i < 100; i++)
        {
            Skip_Test_Loop();
            delay(10);

            if (Skip_Current_Test == true)
            {
                break;
            }
        }
        break;
    case 1:
    case 3:
        gfx->fillScreen(WHITE);
        gfx->setCursor(SCREEN_HEIGHT / 2 - 50, SCREEN_WIDTH / 8);
        gfx->setTextSize(3);
        gfx->setTextColor(PALERED);
        gfx->printf("TEST");

        gfx->setCursor(20, SCREEN_WIDTH / 8 + 40);
        gfx->setTextSize(2);
        gfx->setTextColor(BLACK);
        gfx->print(s);

        GFX_Print_2();

        gfx->setCursor(SCREEN_HEIGHT / 2 - 17, SCREEN_WIDTH / 3 + 30);
        gfx->setTextSize(4);
        gfx->setTextColor(RED);
        gfx->printf("3");
        delay(300);
        gfx->fillRect(SCREEN_HEIGHT / 2 - 17, SCREEN_WIDTH / 3 + 30, SCREEN_HEIGHT / 3 + 20, 20, WHITE);
        gfx->setCursor(SCREEN_HEIGHT / 2 - 17, SCREEN_WIDTH / 3 + 30);
        gfx->printf("2");
        for (int i = 0; i < 100; i++)
        {
            Skip_Test_Loop();
            delay(10);

            if (Skip_Current_Test == true)
            {
                break;
            }
        }
        gfx->fillRect(SCREEN_HEIGHT / 2 - 17, SCREEN_WIDTH / 3 + 30, SCREEN_HEIGHT / 3 + 20, 20, WHITE);
        gfx->setCursor(SCREEN_HEIGHT / 2 - 17, SCREEN_WIDTH / 3 + 30);
        gfx->printf("1");
        for (int i = 0; i < 100; i++)
        {
            Skip_Test_Loop();
            delay(10);

            if (Skip_Current_Test == true)
            {
                break;
            }
        }
        break;

    default:
        break;
    }
}

void GFX_Print_FINISH()
{
    gfx->setCursor(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4 - 30);
    gfx->setTextSize(3);
    gfx->setTextColor(ORANGE);
    gfx->printf("FINISH");
}

void GFX_Print_START()
{
    switch (Current_Rotation)
    {
    case 0:
    case 2:
        gfx->setCursor(SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 2 - 50);
        break;
    case 1:
    case 3:
        gfx->setCursor(SCREEN_HEIGHT / 2 - 50, SCREEN_WIDTH / 2 - 20);
        break;

    default:
        break;
    }

    gfx->setTextSize(4);
    gfx->setTextColor(RED);
    gfx->printf("START");
}

void GFX_Print_RS485CAN_Info(void)
{
    gfx->fillScreen(WHITE);
    gfx->setTextSize(2);
    gfx->setTextColor(PURPLE);
    gfx->setCursor(15, 10);
    gfx->printf("RS485CAN Info");

    gfx->setTextColor(BLACK);
    gfx->setTextSize(1);
    gfx->setCursor(20, 40);
    gfx->printf("[RS485]:115200 bps/s");
    gfx->setCursor(20, 50);
    gfx->printf("[CAN]:1 mbit/s");

    gfx->setTextColor(ORANGE);
    gfx->setCursor(17, 70);
    gfx->printf("<----------UART Info----------> ");
    gfx->setTextColor(BLACK);
    gfx->setCursor(20, 80);
    gfx->printf("[RS485_1]:");
    gfx->setCursor(20, 120);
    gfx->printf("[RS485_2]:");

    gfx->setTextColor(ORANGE);
    gfx->setCursor(10, 160);
    gfx->printf("<------------CAN Info------------> ");
    gfx->setTextColor(BLACK);
    gfx->setCursor(20, 170);
    gfx->printf("[CAN]:");
}

void GFX_Print_Ethernet_Info(void)
{
    gfx->fillScreen(WHITE);
    gfx->setTextSize(2);
    gfx->setTextColor(PURPLE);
    gfx->setCursor(30, 10);
    gfx->printf("Eth Relay Info");
}

void GFX_Print_RS485_Info_Loop(void)
{
    if (millis() > CycleTime)
    {
        gfx->fillRect(0, 90, SCREEN_WIDTH, 20, WHITE);
        gfx->setTextSize(1);
        if (RS485_OP.device_1.connection_status == RS485_OP.state::UNCONNECTED)
        {
            gfx->setTextColor(BLACK);
            gfx->setCursor(20, 90);
            gfx->printf("   [Connect]: Unconnected");
        }
        else if (RS485_OP.device_1.connection_status == RS485_OP.state::CONNECTED)
        {
            gfx->setTextColor(DARKGREEN);
            gfx->setCursor(20, 90);
            gfx->printf("   [Connect]: Connected");
            gfx->setTextColor(BLACK);
            gfx->setCursor(20, 100);
            gfx->printf("   [Receive Data]: %u", RS485_OP.device_1.receive_data);
        }
        else if (RS485_OP.device_1.connection_status == RS485_OP.state::CONNECTING)
        {
            gfx->setTextColor(BLUE);
            gfx->setCursor(20, 90);
            gfx->printf("   [Connect]: Connecting");
            gfx->setTextColor(BLACK);
            gfx->setCursor(20, 100);
            gfx->printf("   [Send Data]: %u", RS485_OP.device_1.send_data);
        }
        else if (RS485_OP.device_1.connection_status == RS485_OP.state::PAUSE)
        {
            gfx->setTextColor(RED);
            gfx->setCursor(20, 90);
            gfx->printf("   [Connect]: Pause");
            gfx->setCursor(20, 100);
            gfx->printf("   [Error]: %s", RS485_OP.device_1.error.code.c_str());
        }

        gfx->fillRect(0, 130, SCREEN_WIDTH, 20, WHITE);
        gfx->setTextSize(1);
        if (RS485_OP.device_2.connection_status == RS485_OP.state::UNCONNECTED)
        {
            gfx->setTextColor(BLACK);
            gfx->setCursor(20, 130);
            gfx->printf("   [Connect]: Unconnected");
        }
        else if (RS485_OP.device_2.connection_status == RS485_OP.state::CONNECTED)
        {
            gfx->setTextColor(DARKGREEN);
            gfx->setCursor(20, 130);
            gfx->printf("   [Connect]: Connected");
            gfx->setTextColor(BLACK);
            gfx->setCursor(20, 140);
            gfx->printf("   [Receive Data]: %u", RS485_OP.device_2.receive_data);
        }
        else if (RS485_OP.device_2.connection_status == RS485_OP.state::CONNECTING)
        {
            gfx->setTextColor(BLUE);
            gfx->setCursor(20, 130);
            gfx->printf("   [Connect]: Connecting");
            gfx->setTextColor(BLACK);
            gfx->setCursor(20, 140);
            gfx->printf("   [Send Data]: %u", RS485_OP.device_2.send_data);
        }
        else if (RS485_OP.device_2.connection_status == RS485_OP.state::PAUSE)
        {
            gfx->setTextColor(RED);
            gfx->setCursor(20, 130);
            gfx->printf("   [Connect]: Pause");
            gfx->setCursor(20, 140);
            gfx->printf("   [Error]: %s", RS485_OP.device_2.error.code.c_str());
        }

        CycleTime = millis() + 500;
    }

    if (RS485_OP.device_1.send_flag == true)
    {
        if (millis() > CycleTime_2)
        {
            RS485_OP.device_1.send_flag = false;

            RS485_OP.send_package_1[1] = (uint8_t)(RS485_OP.device_1.send_data >> 24);
            RS485_OP.send_package_1[2] = (uint8_t)(RS485_OP.device_1.send_data >> 16);
            RS485_OP.send_package_1[3] = (uint8_t)(RS485_OP.device_1.send_data >> 8);
            RS485_OP.send_package_1[4] = (uint8_t)RS485_OP.device_1.send_data;

            // send another one
            Serial.println("[RS485_0] Sending another packet ... ");

            Serial1.write(RS485_OP.send_package_1, 105);
        }
    }

    if (RS485_OP.device_2.send_flag == true)
    {
        if (millis() > CycleTime_4)
        {
            RS485_OP.device_2.send_flag = false;

            RS485_OP.send_package_2[1] = (uint8_t)(RS485_OP.device_2.send_data >> 24);
            RS485_OP.send_package_2[2] = (uint8_t)(RS485_OP.device_2.send_data >> 16);
            RS485_OP.send_package_2[3] = (uint8_t)(RS485_OP.device_2.send_data >> 8);
            RS485_OP.send_package_2[4] = (uint8_t)RS485_OP.device_2.send_data;

            // send another one
            Serial.println("[RS485] Sending another packet ... ");

            Serial2.write(RS485_OP.send_package_2, 105);
        }
    }

    if (RS485_OP.device_1.connection_status != RS485_OP.state::PAUSE)
    {
        while (Serial1.available() > 0)
        {
            // delay(1000); // 接收等待
            uint8_t uart_receive_package[105] = {0};
            Serial1.read(uart_receive_package, sizeof(uart_receive_package));

            if (uart_receive_package[0] == 0x0A)
            {
                if (Uart_Check_Dynamic_Data(uart_receive_package, RS485_OP.device_1.send_data) == false) // 动态数据校验
                {
                    // while (1)
                    // {
                    Serial.printf("[RS485_0] Check Dynamic Data Failed\n");
                    Serial.printf("[RS485_0] Check Data: %d\n", RS485_OP.device_1.send_data);
                    Serial.printf("[RS485_0] Received Data: %d\n", (uint32_t)uart_receive_package[1] << 24 | (uint32_t)uart_receive_package[2] << 16 |
                                                                       (uint32_t)uart_receive_package[3] << 8 | (uint32_t)uart_receive_package[4]);
                    Serial.printf("[RS485_0] Received Buf[1]: %#X\n", uart_receive_package[1]);
                    Serial.printf("[RS485_0] Received Buf[2]: %#X\n", uart_receive_package[2]);
                    Serial.printf("[RS485_0] Received Buf[3]: %#X\n", uart_receive_package[3]);
                    Serial.printf("[RS485_0] Received Buf[4]: %#X\n", uart_receive_package[4]);
                    //     delay(1000);
                    // }
                    RS485_OP.device_1.error.code = "Dynamic data error";
                    RS485_OP.device_1.connection_status = RS485_OP.state::PAUSE;
                }
                else if (Uart_Check_Static_Data(uart_receive_package, RS485_OP.send_package_1) == false) // 静态数据校验
                {
                    Serial.printf("[RS485_0] Check Static Data Failed\n");
                    for (int i = 0; i < 100; i++)
                    {
                        Serial.printf("[RS485_0] Received Buf[%d]: %#X\n", i + 5, uart_receive_package[i + 5]);
                    }
                    RS485_OP.device_1.error.code = "Static data error";
                    RS485_OP.device_1.connection_status = RS485_OP.state::PAUSE;
                    // delay(1000);
                }
                else
                {
                    // delay(500);

                    Serial.printf("[RS485_0] Check Data Successful\n");
                    Serial.printf("[RS485_0] Check Data: %d\n", RS485_OP.device_1.send_data);
                    Serial.printf("[RS485_0] Received Data: %d\n", (uint32_t)uart_receive_package[1] << 24 | (uint32_t)uart_receive_package[2] << 16 |
                                                                       (uint32_t)uart_receive_package[3] << 8 | (uint32_t)uart_receive_package[4]);
                    Serial.printf("[RS485_0] Received Buf[1]: %#X\n", uart_receive_package[1]);
                    Serial.printf("[RS485_0] Received Buf[2]: %#X\n", uart_receive_package[2]);
                    Serial.printf("[RS485_0] Received Buf[3]: %#X\n", uart_receive_package[3]);
                    Serial.printf("[RS485_0] Received Buf[4]: %#X\n", uart_receive_package[4]);

                    Serial.printf("[RS485_0] Received Buf[105]: %#X\n", uart_receive_package[104]);

                    RS485_OP.device_1.receive_data =
                        ((uint32_t)uart_receive_package[1] << 24) |
                        ((uint32_t)uart_receive_package[2] << 16) |
                        ((uint32_t)uart_receive_package[3] << 8) |
                        (uint32_t)uart_receive_package[4];

                    RS485_OP.device_1.send_data = RS485_OP.device_1.receive_data + 1;

                    RS485_OP.device_1.connection_status = RS485_OP.state::CONNECTED;
                    // 清除错误计数看门狗
                    RS485_OP.device_1.error.count = 0;

                    RS485_OP.device_1.send_flag = true;
                    CycleTime_2 = millis() + 500;
                }
            }
            else
            {
                Serial.printf("[RS485_0] Check Header Failed\n");
                Serial.printf("[RS485_0] Received Header: %#X\n", uart_receive_package[0]);
                Serial.printf("[RS485_0] Received Data: %d\n", (uint32_t)uart_receive_package[1] << 24 | (uint32_t)uart_receive_package[2] << 16 |
                                                                   (uint32_t)uart_receive_package[3] << 8 | (uint32_t)uart_receive_package[4]);
                Serial.printf("[RS485_0] Received Buf[1]: %#X\n", uart_receive_package[1]);
                Serial.printf("[RS485_0] Received Buf[2]: %#X\n", uart_receive_package[2]);
                Serial.printf("[RS485_0] Received Buf[3]: %#X\n", uart_receive_package[3]);
                Serial.printf("[RS485_0] Received Buf[4]: %#X\n", uart_receive_package[4]);

                RS485_OP.device_1.error.code = "Header error";
                RS485_OP.device_1.connection_status = RS485_OP.state::PAUSE;
            }
        }
    }
    else
    {
        if (Serial1.available() > 0)
        {
            RS485_OP.device_1.connection_status = RS485_OP.state::CONNECTING;
            RS485_OP.device_1.send_data = 0;
            // 清除错误计数看门狗
            RS485_OP.device_1.error.count = 0;
        }
    }

    if (RS485_OP.device_2.connection_status != RS485_OP.state::PAUSE)
    {
        while (Serial2.available() > 0)
        {
            // delay(1000); // 接收等待
            uint8_t uart_receive_package[105] = {0};
            Serial2.read(uart_receive_package, sizeof(uart_receive_package));

            if (uart_receive_package[0] == 0x0A)
            {
                if (Uart_Check_Dynamic_Data(uart_receive_package, RS485_OP.device_2.send_data) == false) // 动态数据校验
                {
                    // while (1)
                    // {
                    Serial.printf("[RS485] Check Dynamic Data Failed\n");
                    Serial.printf("[RS485] Check Data: %d\n", RS485_OP.device_2.send_data);
                    Serial.printf("[RS485] Received Data: %d\n", (uint32_t)uart_receive_package[1] << 24 | (uint32_t)uart_receive_package[2] << 16 |
                                                                     (uint32_t)uart_receive_package[3] << 8 | (uint32_t)uart_receive_package[4]);
                    Serial.printf("[RS485] Received Buf[1]: %#X\n", uart_receive_package[1]);
                    Serial.printf("[RS485] Received Buf[2]: %#X\n", uart_receive_package[2]);
                    Serial.printf("[RS485] Received Buf[3]: %#X\n", uart_receive_package[3]);
                    Serial.printf("[RS485] Received Buf[4]: %#X\n", uart_receive_package[4]);
                    //     delay(1000);
                    // }
                    RS485_OP.device_2.error.code = "Dynamic data error";
                    RS485_OP.device_2.connection_status = RS485_OP.state::PAUSE;
                }
                else if (Uart_Check_Static_Data(uart_receive_package, RS485_OP.send_package_2) == false) // 静态数据校验
                {
                    Serial.printf("[RS485] Check Static Data Failed\n");
                    for (int i = 0; i < 100; i++)
                    {
                        Serial.printf("[RS485] Received Buf[%d]: %#X\n", i + 5, uart_receive_package[i + 5]);
                    }
                    RS485_OP.device_2.error.code = "Static data error";
                    RS485_OP.device_2.connection_status = RS485_OP.state::PAUSE;
                    // delay(1000);
                }
                else
                {
                    // delay(500);

                    Serial.printf("[RS485] Check Data Successful\n");
                    Serial.printf("[RS485] Check Data: %d\n", RS485_OP.device_2.send_data);
                    Serial.printf("[RS485] Received Data: %d\n", (uint32_t)uart_receive_package[1] << 24 | (uint32_t)uart_receive_package[2] << 16 |
                                                                     (uint32_t)uart_receive_package[3] << 8 | (uint32_t)uart_receive_package[4]);
                    Serial.printf("[RS485] Received Buf[1]: %#X\n", uart_receive_package[1]);
                    Serial.printf("[RS485] Received Buf[2]: %#X\n", uart_receive_package[2]);
                    Serial.printf("[RS485] Received Buf[3]: %#X\n", uart_receive_package[3]);
                    Serial.printf("[RS485] Received Buf[4]: %#X\n", uart_receive_package[4]);

                    Serial.printf("[RS485] Received Buf[105]: %#X\n", uart_receive_package[104]);

                    RS485_OP.device_2.receive_data =
                        ((uint32_t)uart_receive_package[1] << 24) |
                        ((uint32_t)uart_receive_package[2] << 16) |
                        ((uint32_t)uart_receive_package[3] << 8) |
                        (uint32_t)uart_receive_package[4];

                    RS485_OP.device_2.send_data = RS485_OP.device_2.receive_data + 1;

                    RS485_OP.device_2.connection_status = RS485_OP.state::CONNECTED;
                    // 清除错误计数看门狗
                    RS485_OP.device_2.error.count = 0;

                    RS485_OP.device_2.send_flag = true;
                    CycleTime_4 = millis() + 500;
                }
            }
            else
            {
                Serial.printf("[RS485] Check Header Failed\n");
                Serial.printf("[RS485] Received Header: %#X\n", uart_receive_package[0]);
                Serial.printf("[RS485] Received Data: %d\n", (uint32_t)uart_receive_package[1] << 24 | (uint32_t)uart_receive_package[2] << 16 |
                                                                 (uint32_t)uart_receive_package[3] << 8 | (uint32_t)uart_receive_package[4]);
                Serial.printf("[RS485] Received Buf[1]: %#X\n", uart_receive_package[1]);
                Serial.printf("[RS485] Received Buf[2]: %#X\n", uart_receive_package[2]);
                Serial.printf("[RS485] Received Buf[3]: %#X\n", uart_receive_package[3]);
                Serial.printf("[RS485] Received Buf[4]: %#X\n", uart_receive_package[4]);

                RS485_OP.device_2.error.code = "Header error";
                RS485_OP.device_2.connection_status = RS485_OP.state::PAUSE;
            }
        }
    }
    else
    {
        if (Serial2.available() > 0)
        {
            RS485_OP.device_2.connection_status = RS485_OP.state::CONNECTING;
            RS485_OP.device_2.send_data = 0;
            // 清除错误计数看门狗
            RS485_OP.device_2.error.count = 0;
        }
    }

    if ((RS485_OP.device_1.connection_status != RS485_OP.state::PAUSE) &&
        RS485_OP.device_2.connection_status != RS485_OP.state::PAUSE)
    {
        if (millis() > CycleTime_3)
        {
            RS485_OP.device_1.error.count++;
            RS485_OP.device_2.error.count++;
            if (RS485_OP.device_1.error.count > 5)
            {
                RS485_OP.device_1.error.count = 6;
                RS485_OP.device_1.send_data = 0;
                RS485_OP.device_1.connection_status = RS485_OP.state::UNCONNECTED;
            }
            if (RS485_OP.device_2.error.count > 5)
            {
                RS485_OP.device_2.error.count = 6;
                RS485_OP.device_2.send_data = 0;
                RS485_OP.device_2.connection_status = RS485_OP.state::UNCONNECTED;
            }

            CycleTime_3 = millis() + 1000;
        }
    }
}

void GFX_Print_CAN_Info_Loop(void)
{
    if (millis() > CycleTime_5)
    {
        gfx->fillRect(0, 180, SCREEN_WIDTH, 20, WHITE);
        gfx->setTextSize(1);

        if (CAN_OP.device_1.connection_status == CAN_OP.state::UNCONNECTED)
        {
            gfx->setTextColor(BLACK);
            gfx->setCursor(20, 180);
            gfx->printf("   [Connect]: Unconnected");
        }
        else if (CAN_OP.device_1.connection_status == CAN_OP.state::CONNECTED)
        {
            gfx->setTextColor(DARKGREEN);
            gfx->setCursor(20, 180);
            gfx->printf("   [Connect]: Connected");
            gfx->setTextColor(BLACK);
            gfx->setCursor(20, 190);
            gfx->printf("   [Receive Data]: %u", CAN_OP.device_1.receive_data);
        }
        else if (CAN_OP.device_1.connection_status == CAN_OP.state::CONNECTING)
        {
            gfx->setTextColor(BLUE);
            gfx->setCursor(20, 180);
            gfx->printf("   [Connect]: Connecting");
            gfx->setTextColor(BLACK);
            gfx->setCursor(20, 190);
            gfx->printf("   [Send Data]: %u", CAN_OP.device_1.send_data);
        }
        else if (CAN_OP.device_1.connection_status == CAN_OP.state::PAUSE)
        {
            gfx->setTextColor(RED);
            gfx->setCursor(20, 180);
            gfx->printf("   [Connect]: Pause");
            gfx->setCursor(20, 190);
            gfx->printf("   [Error]: %s", CAN_OP.device_1.error.code.c_str());
        }

        CycleTime_5 = millis() + 500;
    }

    if (CAN_OP.device_1.send_flag == true)
    {
        if (millis() > CycleTime_7)
        {
            CAN_OP.device_1.send_flag = false;

            // send another one
            Serial.println("[CAN] Sending another packet ... ");

            Twai_Send_Message();
        }
    }

    if (CAN_OP.device_1.connection_status != CAN_OP.state::PAUSE)
    {
        // 通信报警检测
        uint32_t alerts_triggered;
        twai_read_alerts(&alerts_triggered, pdMS_TO_TICKS(POLLING_RATE_MS));
        // 总线状态信息
        twai_status_info_t twai_status_info;
        twai_get_status_info(&twai_status_info);

        // switch (alerts_triggered)
        // {
        // case TWAI_ALERT_ERR_PASS:
        //     Serial.println("\nAlert: TWAI controller has become error passive.");
        //     CAN_OP.device_1.error.code = "TWAI_ERR_PASS";
        //     CAN_OP.device_1.connection_status = CAN_OP.state::PAUSE;
        //     // delay(1000);
        //     break;
        // case TWAI_ALERT_BUS_ERROR:
        //     Serial.println("\nAlert: A (Bit, Stuff, CRC, Form, ACK) error has occurred on the bus.");
        //     Serial.printf("Bus error count: %d\n", twai_status_info.bus_error_count);
        //     // delay(1000);
        //     break;
        // case TWAI_ALERT_TX_FAILED:
        //     Serial.println("\nAlert: The Transmission failed.");
        //     Serial.printf("TX buffered: %d\t", twai_status_info.msgs_to_tx);
        //     Serial.printf("TX error: %d\t", twai_status_info.tx_error_counter);
        //     Serial.printf("TX failed: %d\n", twai_status_info.tx_failed_count);
        //     // delay(1000);
        //     break;
        // case TWAI_ALERT_TX_SUCCESS:
        //     Serial.println("\nAlert: The Transmission was successful.");
        //     Serial.printf("TX buffered: %d\t", twai_status_info.msgs_to_tx);
        //     break;
        // case TWAI_ALERT_RX_QUEUE_FULL:
        //     Serial.println("\nAlert: The RX queue is full causing a received frame to be lost.");
        //     Serial.printf("RX buffered: %d\t", twai_status_info.msgs_to_rx);
        //     Serial.printf("RX missed: %d\t", twai_status_info.rx_missed_count);
        //     Serial.printf("RX overrun %d\n", twai_status_info.rx_overrun_count);
        //     // delay(1000);
        //     break;

        // default:
        //     break;
        // }

        // switch (twai_status_info.state)
        // {
        // case TWAI_STATE_RUNNING:
        //     Serial.println("\nTWAI_STATE_RUNNING");
        //     break;
        // case TWAI_STATE_BUS_OFF:
        //     Serial.println("\nTWAI_STATE_BUS_OFF");
        //     twai_initiate_recovery();
        //     // delay(1000);
        //     CAN_OP.device_1.error.code = "BUS_OFF";
        //     CAN_OP.device_1.connection_status = CAN_OP.state::PAUSE;
        //     break;
        // case TWAI_STATE_STOPPED:
        //     Serial.println("\nTWAI_STATE_STOPPED");
        //     twai_start();
        //     // delay(1000);
        //     CAN_OP.device_1.error.code = "BUS_STOPPED";
        //     CAN_OP.device_1.connection_status = CAN_OP.state::PAUSE;
        //     break;
        // case TWAI_STATE_RECOVERING:
        //     Serial.println("\nTWAI_STATE_RECOVERING");
        //     // delay(1000);
        //     break;

        // default:
        //     break;
        // }

        // 如果TWAI有信息接收到
        if (alerts_triggered & TWAI_ALERT_RX_DATA)
        {
            twai_message_t rx_buf;
            while (twai_receive(&rx_buf, 0) == ESP_OK)
            {
                Twai_Receive_Message(rx_buf);
            }
        }
    }

    if (CAN_OP.device_1.connection_status != CAN_OP.state::PAUSE)
    {
        if (millis() > CycleTime_6)
        {
            CAN_OP.device_1.error.count++;
            if (CAN_OP.device_1.error.count > 5)
            {
                CAN_OP.device_1.error.count = 6;
                CAN_OP.device_1.send_data = 0;
                CAN_OP.device_1.connection_status = CAN_OP.state::UNCONNECTED;
            }
            CycleTime_6 = millis() + 1000;
        }
    }
}

void GFX_Print_RS485CAN_Info_Loop(void)
{
    GFX_Print_RS485_Info_Loop();
    GFX_Print_CAN_Info_Loop();
}

void GFX_Print_Ethernet_Info_Loop(void)
{
    if (millis() > CycleTime)
    {
        gfx->fillRect(0, 40, SCREEN_WIDTH, 100, WHITE);
        gfx->setTextSize(1);

        gfx->setTextColor(BLACK);
        gfx->setTextSize(1);

        if (Ethernet_Relay_OP.initialization.flag == false)
        {
            gfx->setCursor(20, 40);
            gfx->printf("[State]:");
            gfx->setTextColor(RED);
            gfx->setCursor(20, 50);
            gfx->printf("  Initialization failed");
            gfx->setTextColor(BLACK);
            gfx->setCursor(20, 60);
            gfx->printf("[Assertion]:");
            gfx->setTextColor(RED);
            gfx->setCursor(20, 70);
            gfx->printf("  %s", Ethernet_Relay_OP.initialization.code.c_str());
        }
        else
        {
            gfx->setCursor(20, 40);
            gfx->printf("[State]:");
            gfx->setTextColor(DARKGREEN);
            gfx->setCursor(20, 50);
            gfx->printf("  Initialization successful");
            gfx->setTextColor(BLACK);
            gfx->setCursor(20, 60);
            gfx->print("[IP address]: ");
            gfx->println(Ethernet.localIP());
            gfx->setCursor(20, 70);
            gfx->print("[Subnet mask]: ");
            gfx->println(Ethernet.subnetMask());
            gfx->setCursor(20, 80);
            gfx->print("[Gateway]: ");
            gfx->println(Ethernet.gatewayIP());
            gfx->setCursor(20, 90);
            gfx->print("[DNS]: ");
            gfx->println(Ethernet.dnsServerIP());
        }

        if (Ethernet_Relay_OP.initialization.flag == false)
        {
            if (Ethernet_Initialization_Assertion(&Ethernet_Relay_OP.initialization.code) == false)
            {
                Ethernet_Relay_OP.initialization.flag = false;
            }
            else
            {
                // start listening for clients
                server.begin();

                Ethernet_Relay_OP.initialization.flag = true;
            }
        }

        CycleTime = millis() + 500;
    }

    if (millis() > CycleTime_2)
    {
        if (Ethernet.linkStatus() != LinkON)
        {
            Ethernet_Relay_OP.initialization.flag = false;
        }
        CycleTime_2 = millis() + 3000;
    }

    if (Ethernet_Relay_OP.initialization.flag == true)
    {
        EthernetClient client = server.available();
        if (client)
        {                                  // if you get a client,
            Serial.println("New Client."); // print a message out the serial port
            String currentLine = "";       // make a String to hold incoming data from the client
            while (client.connected())
            { // loop while the client's connected
                if (client.available())
                {                           // if there's bytes to read from the client,
                    char c = client.read(); // read a byte, then
                    Serial.write(c);        // print it out the serial monitor
                    if (c == '\n')
                    { // if the byte is a newline character

                        // if the current line is blank, you got two newline characters in a row.
                        // that's the end of the client HTTP request, so send a response:
                        if (currentLine.length() == 0)
                        {
                            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                            // and a content-type so the client knows what's coming, then a blank line:
                            client.println("HTTP/1.1 200 OK");
                            client.println("Content-type:text/html");
                            client.println();

                            // the content of the HTTP response follows the header:
                            client.println("</head><body><div align=\"center\"><h2>Relay Ethernet Server Test</h2><br />");
                            client.print("<h3>Click <a href=\"/RELAY_1\">here</a> to change the Relay 1 state.<br></h3>");
                            // The HTTP response ends with another blank line:
                            client.println();
                            // break out of the while loop:
                            break;
                        }
                        else
                        { // if you got a newline, then clear currentLine:
                            currentLine = "";
                        }
                    }
                    else if (c != '\r')
                    {                     // if you got anything else but a carriage return character,
                        currentLine += c; // add it to the end of the currentLine
                    }

                    // Check to see if the client request was "GET /H" or "GET /L":
                    if (currentLine.endsWith("GET /RELAY_1"))
                    {
                        Ethernet_Relay_OP.html_relay1_flag = !Ethernet_Relay_OP.html_relay1_flag;

                        if (Ethernet_Relay_OP.html_relay1_flag == 0)
                        {
                            digitalWrite(RELAY_1, HIGH);
                        }
                        else
                        {
                            digitalWrite(RELAY_1, LOW);
                        }
                    }
                }
            }
            // close the connection:
            client.stop();
            Serial.println("Client Disconnected.");
            // give the web browser time to receive the data
            // delay(500);
        }
    }
}

void Original_Test_1()
{
    gfx->fillScreen(WHITE);

    gfx->setTextSize(2);
    gfx->setTextColor(PURPLE);
    gfx->setCursor(50, 20);
    gfx->printf("Touch Info");

    GFX_Print_Touch_Info_Loop();

    GFX_Print_1();
}

void Original_Test_3()
{
    gfx->fillScreen(WHITE);

    GFX_Print_START();

    for (int i = 255; i >= 0; i--)
    {
        ledcWrite(1, i);
        delay(3);
    }
    delay(3000);
    for (int i = 0; i <= 255; i++)
    {
        ledcWrite(1, i);
        delay(3);
    }
    delay(1000);
}

void Original_Test_3_Loop()
{
    gfx->fillScreen(WHITE);

    GFX_Print_FINISH();

    GFX_Print_1();
}

void Original_Test_4()
{
    gfx->fillScreen(RED);
    delay(3000);
    gfx->fillScreen(GREEN);
    delay(3000);
    gfx->fillScreen(BLUE);
    delay(3000);
    gfx->fillScreen(WHITE);
    delay(3000);

    switch (Current_Rotation)
    {
    case 0:
    case 2:
        gfx->draw16bitRGBBitmap(0, 0, (uint16_t *)gImage_1, SCREEN_WIDTH, SCREEN_HEIGHT);
        delay(3000);

        gfx->draw16bitRGBBitmap(0, 0, (uint16_t *)gImage_2, SCREEN_WIDTH, SCREEN_HEIGHT);
        delay(3000);

        gfx->fillScreen(BLACK);
        delay(3000);
        break;
    case 1:
    case 3:
        gfx->draw16bitRGBBitmap(0, 0, (uint16_t *)gImage_3, SCREEN_HEIGHT, SCREEN_WIDTH);
        delay(3000);

        gfx->draw16bitRGBBitmap(0, 0, (uint16_t *)gImage_4, SCREEN_HEIGHT, SCREEN_WIDTH);
        delay(3000);

        gfx->fillScreen(BLACK);
        delay(3000);

        break;

    default:
        break;
    }
}

void Original_Test_4_Loop()
{
    // switch (Current_Rotation)
    // {
    // case 0:
    // case 2:
    //     gfx->draw16bitRGBBitmap(0, 0, (uint16_t *)gImage_2, SCREEN_WIDTH, SCREEN_HEIGHT);
    //     break;
    // case 1:
    // case 3:
    //     gfx->draw16bitRGBBitmap(0, 0, (uint16_t *)gImage_4, SCREEN_HEIGHT, SCREEN_WIDTH);
    //     break;

    // default:
    //     break;
    // }

    gfx->fillScreen(BLACK);

    GFX_Print_FINISH();

    GFX_Print_1();
}

void Original_Test_5()
{
    RS485_Initialization();
    // CAN_Drive_Initialization();

    // RS485_OP.device_1.send_flag = true;
    RS485_OP.device_1.send_data = 0;
    RS485_OP.device_1.connection_status = RS485_OP.state::UNCONNECTED;
    // 清除错误计数看门狗
    RS485_OP.device_1.error.count = 0;

    // RS485_OP.device_2.send_flag = true;
    RS485_OP.device_2.send_data = 0;
    RS485_OP.device_2.connection_status = RS485_OP.state::UNCONNECTED;
    // 清除错误计数看门狗
    RS485_OP.device_2.error.count = 0;

    // CAN_OP.device_1.send_flag = true;
    CAN_OP.device_1.connection_status = CAN_OP.state::UNCONNECTED;
    // 清除错误
    CAN_OP.device_1.error.code = "null";
    // 清除错误计数看门狗
    CAN_OP.device_1.error.count = 0;
    CycleTime_7 = millis() + 1000;
}

void Original_Test_5_Loop()
{
    gfx->fillScreen(WHITE);

    GFX_Print_RS485CAN_Info();
    GFX_Print_RS485_CAN_Connect_Button();

    GFX_Print_1();
}

void Original_Test_6()
{
    digitalWrite(W5500_CS, HIGH);
    digitalWrite(SCREEN_CS, HIGH);
    digitalWrite(SD_CS, HIGH);

    Ethernet_Initialization();
}

void Original_Test_6_Loop()
{
    gfx->fillScreen(WHITE);

    GFX_Print_Ethernet_Info();

    GFX_Print_Ethernet_Relay_Button();

    GFX_Print_1();
}

void Original_Test_8()
{
    Wifi_STA_Test();

    delay(2000);

    gfx->setTextColor(BLACK);
    gfx->setCursor(0, 0);

    if (Wifi_Connection_Flag == true)
    {
        gfx->fillScreen(WHITE);

        // Obtain and set the time from the network time server
        // After successful acquisition, the chip will use the RTC clock to update the holding time
        configTime(GMT_OFFSET_SEC, DAY_LIGHT_OFFSET_SEC, NTP_SERVER1, NTP_SERVER2, NTP_SERVER3);
        // PrintLocalTime();

        WIFI_HTTP_Download_File();
    }
    else
    {
        // gfx->setCursor(80, 200);
        // gfx->setTextColor(RED);
        // gfx->print("Not connected to the network");

        // gfx->fillScreen(WHITE);
        // GFX_Print_FINISH();
    }
    // delay(3000);

    GFX_Print_FINISH();
    GFX_Print_1();
}

void Original_Test_8_Loop()
{
    gfx->fillScreen(WHITE);
    GFX_Print_FINISH();
    GFX_Print_1();
}

void Rotation_Trigger(void (*func)())
{
    if (digitalRead(0) == LOW)
    {
        delay(300);
        Current_Rotation++;
        if (Current_Rotation > 3)
        {
            Current_Rotation = 0;
        }

        gfx->setRotation(Current_Rotation);
        func();
    }
}

void Original_Test_2()
{
    gfx->fillScreen(WHITE);

    gfx->setTextSize(2);
    gfx->setTextColor(PURPLE);
    gfx->setCursor(60, 30);
    gfx->printf("SD Info");

    gfx->setCursor(0, 100);
    gfx->setTextSize(1);
    gfx->setTextColor(BLACK);

    SPI.setFrequency(16000000);
    SPI.begin(SD_SCLK, SD_MISO, SD_MOSI);

    // gfx->fillScreen(WHITE);
    // GFX_Print_FINISH();
    GFX_Print_1();
}

void SD_Test_Loop(void)
{
    Serial.println("Detecting SD card");

    if (SD.begin(SD_CS, SPI) == false) // SD boots
    {
        gfx->fillRect(0, 50, SCREEN_WIDTH - 60, 100, WHITE);
        gfx->setTextSize(1);
        gfx->setTextColor(BLACK);
        gfx->setCursor(0, 60);

        Serial.println("SD bus initialization failed !");
        gfx->printf("SD bus init failed!\n");
    }
    else
    {
        gfx->fillRect(0, 50, SCREEN_WIDTH - 60, 100, WHITE);
        gfx->setTextSize(1);
        gfx->setTextColor(BLACK);
        gfx->setCursor(0, 60);

        Serial.println("SD bus initialization successful !");
        gfx->printf("SD bus init successful!\n");

        uint8_t card_type = SD.cardType();
        uint64_t card_size = SD.cardSize() / (1024 * 1024);
        // uint8_t num_sectors = SD.numSectors();
        switch (card_type)
        {
        case CARD_NONE:
            Serial.println("No SD card attached");
            gfx->printf("No SD card attached\n");
            gfx->printf("SD card test failed\n");

            break;
        case CARD_MMC:
            Serial.print("SD Card Type: ");
            Serial.println("MMC");
            Serial.printf("SD Card Size: %lluMB\n", card_size);
            gfx->printf("SD Card Type:MMC\nSD Card Size:%lluMB\n", card_size);
            gfx->printf("SD card test successful\n");

            break;
        case CARD_SD:
            Serial.print("SD Card Type: ");
            Serial.println("SDSC");
            Serial.printf("SD Card Size: %lluMB\n", card_size);
            gfx->printf("SD Card Type:SDSC\nSD Card Size:%lluMB\n", card_size);
            gfx->printf("SD card tested successful\n");

            break;
        case CARD_SDHC:
            Serial.print("SD Card Type: ");
            Serial.println("SDHC");
            Serial.printf("SD Card Size: %lluMB\n", card_size);
            gfx->printf("SD Card Type:SDHC\nSD Card Size:%lluMB\n", card_size);
            gfx->printf("SD card tested successful\n");

            break;
        default:
            Serial.println("UNKNOWN");
            gfx->printf("UNKNOWN");
            gfx->printf("SD card test failed\n");

            break;
        }
    }
    SD.end();
}

void GFX_Print_RTC_Switch_Info()
{
    switch (Current_Rotation)
    {
    case 0:
    case 2:
        gfx->fillRect(SCREEN_WIDTH / 4, SCREEN_HEIGHT - (SCREEN_HEIGHT / 3) - 30, SCREEN_WIDTH / 2, 50, PALERED);
        gfx->drawRect(SCREEN_WIDTH / 4, SCREEN_HEIGHT - (SCREEN_HEIGHT / 3) - 30, SCREEN_WIDTH / 2, 50, RED);
        gfx->setTextSize(1);
        gfx->setTextColor(WHITE);

        gfx->setCursor(SCREEN_WIDTH / 4 + 30, SCREEN_HEIGHT - (SCREEN_HEIGHT / 3) - 40 + 30);
        gfx->printf("RTC Reset");
        break;
    case 1:
    case 3:
        gfx->fillRect(SCREEN_HEIGHT - (SCREEN_HEIGHT / 4) + 20, 140, SCREEN_WIDTH / 2 - 30, 50, PALERED);
        gfx->drawRect(SCREEN_HEIGHT - (SCREEN_HEIGHT / 4) + 20, 140, SCREEN_WIDTH / 2 - 30, 50, RED);
        gfx->setTextSize(1);
        gfx->setTextColor(WHITE);

        gfx->setCursor(SCREEN_HEIGHT - (SCREEN_HEIGHT / 4) + 35, 140 + 20);
        gfx->printf("RTC Reset");
        break;

    default:
        break;
    }
}

void Original_Test_7()
{
    gfx->fillScreen(WHITE);

    gfx->setTextSize(2);
    gfx->setTextColor(PURPLE);
    gfx->setCursor(60, 30);
    gfx->printf("RTC Info");

    GFX_Print_RTC_Switch_Info();

    GFX_Print_1();
}

void GFX_Print_RTC_Info_Loop()
{
    gfx->fillRect(0, 50, SCREEN_WIDTH, SCREEN_HEIGHT / 2, WHITE);
    gfx->setTextSize(1);
    gfx->setTextColor(BLACK);

    gfx->setCursor(20, 65);
    gfx->printf("PCF8563  Weekday: %s\n",
                PCF8563->IIC_Read_Device_State(PCF8563->Arduino_IIC_RTC::Status_Information::RTC_WEEKDAYS_DATA).c_str());
    gfx->setCursor(20, 80);
    gfx->printf("PCF8563  Year: %d\n",
                (int32_t)PCF8563->IIC_Read_Device_Value(PCF8563->Arduino_IIC_RTC::Value_Information::RTC_YEARS_DATA));
    gfx->setCursor(20, 95);
    gfx->printf("PCF8563 Date: %d.%d\n",
                (int32_t)PCF8563->IIC_Read_Device_Value(PCF8563->Arduino_IIC_RTC::Value_Information::RTC_MONTHS_DATA),
                (int32_t)PCF8563->IIC_Read_Device_Value(PCF8563->Arduino_IIC_RTC::Value_Information::RTC_DAYS_DATA));
    gfx->setCursor(20, 110);
    gfx->printf("PCF8563 Time: %d:%d:%d\n",
                (int32_t)PCF8563->IIC_Read_Device_Value(PCF8563->Arduino_IIC_RTC::Value_Information::RTC_HOURS_DATA),
                (int32_t)PCF8563->IIC_Read_Device_Value(PCF8563->Arduino_IIC_RTC::Value_Information::RTC_MINUTES_DATA),
                (int32_t)PCF8563->IIC_Read_Device_Value(PCF8563->Arduino_IIC_RTC::Value_Information::RTC_SECONDS_DATA));
}

bool GFX_Print_RTC_Switch_Button_Trigger(int32_t x, int32_t y)
{
    if ((x == -1) && (y == -1))
    {
        return false;
    }

    switch (Current_Rotation)
    {
    case 0:
    case 2:
        if (x > SCREEN_WIDTH / 4 && x < SCREEN_WIDTH / 4 + SCREEN_WIDTH / 2 &&
            y > SCREEN_HEIGHT - (SCREEN_HEIGHT / 3) - 30 && y < SCREEN_HEIGHT - (SCREEN_HEIGHT / 3) - 30 + 50)
        {
            return true;
        }
        break;
    case 1:
    case 3:
        if (x > (SCREEN_HEIGHT - (SCREEN_HEIGHT / 4) + 20) && x < ((SCREEN_HEIGHT - (SCREEN_HEIGHT / 4) + 20) + (SCREEN_WIDTH / 2 - 30)) &&
            y > 140 && y < 190)
        {
            return true;
        }
        break;

    default:
        break;
    }

    return false;
}

void Original_Test_Loop()
{
    GFX_Print_TEST("Touch Test");
    if (Skip_Current_Test == false)
    {
        Original_Test_1();

        while (1)
        {
            bool temp = false;

            Rotation_Trigger(Original_Test_1);

            // if (CST226SE->IIC_Interrupt_Flag == true)
            // {
            //     CST226SE->IIC_Interrupt_Flag = false;

            int32_t touch_x = CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH_COORDINATE_X);

            if (CST226SE->IIC_Interrupt_Flag == true)
            {
                if (touch_x > SCREEN_WIDTH)
                {
                    gfx->fillRect(0, 40, SCREEN_WIDTH, SCREEN_HEIGHT / 2, WHITE);
                    gfx->setTextSize(1);
                    gfx->setTextColor(BLACK);

                    gfx->setCursor(20, 50);
                    gfx->printf("ID: %#X ", (int32_t)CST226SE->IIC_Device_ID());

                    gfx->setCursor(20, 65);
                    gfx->setTextColor(RED);
                    gfx->printf("The Home button is triggered");
                }

                CST226SE->IIC_Interrupt_Flag = false;
            }

            uint8_t fingers_number = CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH_FINGER_NUMBER);

            if (fingers_number > 0)
            {
                int32_t touch_x = CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH_COORDINATE_X);
                int32_t touch_y = CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH_COORDINATE_Y);

                GFX_Print_Touch_Info_Loop();

                Touch_Rotation_Convert(&touch_x, &touch_y);

                switch (GFX_Print_1_Trigger(touch_x, touch_y))
                {
                case 1:
                    GFX_Print_TEST("Touch Test");
                    Original_Test_1();
                    if (Skip_Current_Test == true)
                    {
                        temp = true;
                    }
                    break;
                case 2:
                    temp = true;
                    break;

                default:
                    break;
                }
            }
            // }

            if (temp == true)
            {
                break;
            }
        }
    }

    GFX_Print_TEST("LCD Backlight Test");
    if (Skip_Current_Test == false)
    {
        Original_Test_3();
        Original_Test_3_Loop();

        while (1)
        {
            bool temp = false;

            Rotation_Trigger(Original_Test_3_Loop);

            // if (CST226SE->IIC_Interrupt_Flag == true)
            // {
            //     CST226SE->IIC_Interrupt_Flag = false;

            uint8_t fingers_number = CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH_FINGER_NUMBER);

            if (fingers_number > 0)
            {
                int32_t touch_x = CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH_COORDINATE_X);
                int32_t touch_y = CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH_COORDINATE_Y);

                Touch_Rotation_Convert(&touch_x, &touch_y);

                switch (GFX_Print_1_Trigger(touch_x, touch_y))
                {
                case 1:
                    GFX_Print_TEST("LCD Backlight Test");
                    Original_Test_3();
                    Original_Test_3_Loop();
                    if (Skip_Current_Test == true)
                    {
                        temp = true;
                    }
                    break;
                case 2:
                    temp = true;
                    break;

                default:
                    break;
                }
            }
            // }

            if (temp == true)
            {
                break;
            }
        }
    }

    GFX_Print_TEST("LCD Color Test");
    if (Skip_Current_Test == false)
    {
        Original_Test_4();
        Original_Test_4_Loop();
        while (1)
        {
            bool temp = false;

            Rotation_Trigger(Original_Test_4_Loop);

            // if (CST226SE->IIC_Interrupt_Flag == true)
            // {
            //     CST226SE->IIC_Interrupt_Flag = false;
            uint8_t fingers_number = CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH_FINGER_NUMBER);

            if (fingers_number > 0)
            {
                int32_t touch_x = CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH_COORDINATE_X);
                int32_t touch_y = CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH_COORDINATE_Y);

                Touch_Rotation_Convert(&touch_x, &touch_y);

                switch (GFX_Print_1_Trigger(touch_x, touch_y))
                {
                case 1:
                    GFX_Print_TEST("LCD Color Test");
                    Original_Test_4();
                    Original_Test_4_Loop();
                    if (Skip_Current_Test == true)
                    {
                        temp = true;
                    }
                    break;
                case 2:
                    temp = true;
                    break;

                default:
                    break;
                }
            }
            // }

            if (temp == true)
            {
                break;
            }
        }
    }

    GFX_Print_TEST("SD Test");
    if (Skip_Current_Test == false)
    {
        Original_Test_2();

        while (1)
        {
            bool temp = false;

            Rotation_Trigger(Original_Test_2);

            if (millis() > CycleTime)
            {
                SD_Test_Loop();
                CycleTime = millis() + 1000;
            }

            // if (CST226SE->IIC_Interrupt_Flag == true)
            // {
            //     CST226SE->IIC_Interrupt_Flag = false;

            uint8_t fingers_number = CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH_FINGER_NUMBER);

            if (fingers_number > 0)
            {
                int32_t touch_x = CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH_COORDINATE_X);
                int32_t touch_y = CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH_COORDINATE_Y);

                Touch_Rotation_Convert(&touch_x, &touch_y);

                switch (GFX_Print_1_Trigger(touch_x, touch_y))
                {
                case 1:
                    GFX_Print_TEST("SD Test");
                    Original_Test_2();
                    if (Skip_Current_Test == true)
                    {
                        temp = true;
                    }
                    break;
                case 2:
                    temp = true;
                    break;

                default:
                    break;
                }
            }
            // }

            if (temp == true)
            {
                break;
            }
        }
    }

    GFX_Print_TEST("RTC Test");
    if (Skip_Current_Test == false)
    {
        Original_Test_7();

        while (1)
        {
            bool temp = false;

            Rotation_Trigger(Original_Test_7);

            if (millis() > CycleTime)
            {
                GFX_Print_RTC_Info_Loop();
                CycleTime = millis() + 1000;
            }

            // if (CST226SE->IIC_Interrupt_Flag == true)
            // {
            //     CST226SE->IIC_Interrupt_Flag = false;

            uint8_t fingers_number = CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH_FINGER_NUMBER);

            if (fingers_number > 0)
            {
                int32_t touch_x = CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH_COORDINATE_X);
                int32_t touch_y = CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH_COORDINATE_Y);

                Touch_Rotation_Convert(&touch_x, &touch_y);

                switch (GFX_Print_1_Trigger(touch_x, touch_y))
                {
                case 1:
                    GFX_Print_TEST("RTC Test");
                    Original_Test_7();
                    if (Skip_Current_Test == true)
                    {
                        temp = true;
                    }
                    break;
                case 2:
                    temp = true;
                    break;

                default:
                    break;
                }

                if (GFX_Print_RTC_Switch_Button_Trigger(touch_x, touch_y) == true)
                {
                    // 重置时间
                    //  关闭RTC
                    PCF8563->IIC_Write_Device_State(PCF8563->Arduino_IIC_RTC::Device::RTC_CLOCK_RTC,
                                                    PCF8563->Arduino_IIC_RTC::Device_State::RTC_DEVICE_OFF);
                    // 时钟传感器设置秒
                    PCF8563->IIC_Write_Device_Value(PCF8563->Arduino_IIC_RTC::Device_Value::RTC_SET_SECOND_DATA,
                                                    58);
                    // 时钟传感器设置分
                    PCF8563->IIC_Write_Device_Value(PCF8563->Arduino_IIC_RTC::Device_Value::RTC_SET_MINUTE_DATA,
                                                    59);
                    // 时钟传感器设置时
                    PCF8563->IIC_Write_Device_Value(PCF8563->Arduino_IIC_RTC::Device_Value::RTC_SET_HOUR_DATA,
                                                    23);
                    // 时钟传感器设置天
                    PCF8563->IIC_Write_Device_Value(PCF8563->Arduino_IIC_RTC::Device_Value::RTC_SET_DAY_DATA,
                                                    31);
                    // 时钟传感器设置月
                    PCF8563->IIC_Write_Device_Value(PCF8563->Arduino_IIC_RTC::Device_Value::RTC_SET_MONTH_DATA,
                                                    12);
                    // 时钟传感器设置
                    PCF8563->IIC_Write_Device_Value(PCF8563->Arduino_IIC_RTC::Device_Value::RTC_SET_YEAR_DATA,
                                                    99);
                    // 开启RTC
                    PCF8563->IIC_Write_Device_State(PCF8563->Arduino_IIC_RTC::Device::RTC_CLOCK_RTC,
                                                    PCF8563->Arduino_IIC_RTC::Device_State::RTC_DEVICE_ON);

                    CycleTime_2 = millis() + 1000;
                }
            }
            // }

            if (temp == true)
            {
                break;
            }
        }
    }

    GFX_Print_TEST("WIFI STA Test");
    if (Skip_Current_Test == false)
    {
        Original_Test_8();
        while (1)
        {
            bool temp = false;

            Rotation_Trigger(Original_Test_8_Loop);

            // if (CST226SE->IIC_Interrupt_Flag == true)
            // {
            //     CST226SE->IIC_Interrupt_Flag = false;

            uint8_t fingers_number = CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH_FINGER_NUMBER);

            if (fingers_number > 0)
            {
                int32_t touch_x = CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH_COORDINATE_X);
                int32_t touch_y = CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH_COORDINATE_Y);

                Touch_Rotation_Convert(&touch_x, &touch_y);

                switch (GFX_Print_1_Trigger(touch_x, touch_y))
                {
                case 1:
                    GFX_Print_TEST("WIFI STA Test");
                    Original_Test_8();
                    if (Skip_Current_Test == true)
                    {
                        temp = true;
                    }
                    break;
                case 2:
                    temp = true;
                    break;

                default:
                    break;
                }
            }
            // }

            if (temp == true)
            {
                break;
            }
        }
    }

    GFX_Print_TEST("RS485CAN Test");
    if (Skip_Current_Test == false)
    {
        Original_Test_5();
        Original_Test_5_Loop();
        while (1)
        {
            bool temp = false;

            Rotation_Trigger(Original_Test_5_Loop);

            GFX_Print_RS485CAN_Info_Loop();

            // if (CST226SE->IIC_Interrupt_Flag == true)
            // {
            //     CST226SE->IIC_Interrupt_Flag = false;

            uint8_t fingers_number = CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH_FINGER_NUMBER);

            if (fingers_number > 0)
            {
                int32_t touch_x = CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH_COORDINATE_X);
                int32_t touch_y = CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH_COORDINATE_Y);

                Touch_Rotation_Convert(&touch_x, &touch_y);

                switch (GFX_Print_1_Trigger(touch_x, touch_y))
                {
                case 1:
                    GFX_Print_TEST("RS485CAN Test");
                    Original_Test_5();
                    Original_Test_5_Loop();
                    if (Skip_Current_Test == true)
                    {
                        temp = true;
                    }
                    break;
                case 2:
                    temp = true;
                    break;

                default:
                    break;
                }

                if (GFX_Print_RS485_CAN_Connect_Button_Trigger(touch_x, touch_y) == true)
                {
                    RS485_OP.device_1.send_flag = true;
                    RS485_OP.device_1.send_data = 0;
                    RS485_OP.device_1.connection_status = RS485_OP.state::CONNECTING;
                    // 清除错误计数看门狗
                    RS485_OP.device_1.error.count = 0;

                    RS485_OP.device_2.send_flag = true;
                    RS485_OP.device_2.send_data = 0;
                    RS485_OP.device_2.connection_status = RS485_OP.state::CONNECTING;
                    // 清除错误计数看门狗
                    RS485_OP.device_2.error.count = 0;

                    CAN_OP.device_1.send_flag = true;
                    CAN_OP.device_1.connection_status = CAN_OP.state::CONNECTING;
                    // 清除错误
                    CAN_OP.device_1.error.code = "null";
                    // 清除错误计数看门狗
                    CAN_OP.device_1.error.count = 0;

                    CycleTime_2 = millis() + 1000;
                    CycleTime_4 = millis() + 1000;
                    CycleTime_7 = millis() + 1000;
                }
            }
            // }

            if (temp == true)
            {
                break;
            }
        }
    }

    GFX_Print_TEST("Ethernet Relay Test");
    if (Skip_Current_Test == false)
    {
        Original_Test_6();
        Original_Test_6_Loop();
        while (1)
        {
            bool temp = false;

            Rotation_Trigger(Original_Test_6_Loop);

            GFX_Print_Ethernet_Info_Loop();

            // if (CST226SE->IIC_Interrupt_Flag == true)
            // {
            //     CST226SE->IIC_Interrupt_Flag = false;
            //     delay(100);

            uint8_t fingers_number = CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH_FINGER_NUMBER);

            if (fingers_number > 0)
            {
                int32_t touch_x = CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH_COORDINATE_X);
                int32_t touch_y = CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH_COORDINATE_Y);

                Touch_Rotation_Convert(&touch_x, &touch_y);

                switch (GFX_Print_1_Trigger(touch_x, touch_y))
                {
                case 1:
                    GFX_Print_TEST("Ethernet Relay Test");
                    Original_Test_6();
                    Original_Test_6_Loop();
                    if (Skip_Current_Test == true)
                    {
                        temp = true;
                    }
                    break;
                case 2:
                    temp = true;
                    break;

                default:
                    break;
                }

                if (GFX_Print_Ethernet_Relay_Button_Trigger(touch_x, touch_y) == true)
                {
                    delay(300);
                    if (Ethernet_Relay_OP.html_relay1_flag == 0)
                    {
                        digitalWrite(RELAY_1, LOW);
                        Ethernet_Relay_OP.html_relay1_flag = 1;
                    }
                    else
                    {
                        digitalWrite(RELAY_1, HIGH);
                        Ethernet_Relay_OP.html_relay1_flag = 0;
                    }
                }
            }
            // }

            if (temp == true)
            {
                digitalWrite(W5500_CS, HIGH);
                digitalWrite(SCREEN_CS, HIGH);
                digitalWrite(SD_CS, HIGH);

                break;
            }
        }
    }
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Ciallo");
    Serial.println("[T-Connect-Pro_" + (String)BOARD_VERSION + "_No_Lora_Add_Sd_Rtc][" + (String)SOFTWARE_NAME +
                   "]_firmware_" + (String)SOFTWARE_LASTEDITTIME);

    pinMode(W5500_CS, OUTPUT);
    pinMode(SCREEN_CS, OUTPUT);
    pinMode(SD_CS, OUTPUT);
    digitalWrite(W5500_CS, HIGH);
    digitalWrite(SCREEN_CS, HIGH);
    digitalWrite(SD_CS, HIGH);

    pinMode(RELAY_1, OUTPUT);
    digitalWrite(RELAY_1, HIGH);

    ledcAttachPin(SCREEN_BL, 1);
    ledcSetup(1, 2000, 8);
    ledcWrite(1, 255);

    // 初始化串口，并重新定义引脚
    // 参数包括串行通信的波特率、串行模式、使用的 RX 引脚和 TX 引脚。
    Serial1.begin(115200, SERIAL_8N1, RS485_RX_1, RS485_TX_1);
    Serial2.begin(115200, SERIAL_8N1, RS485_RX_2, RS485_TX_2);

    CAN_Drive_Initialization();

    if (CST226SE->begin() == false)
    {
        Serial.println("CST226SE initialization fail");
        delay(2000);
    }
    else
    {
        Serial.println("CST226SE initialization successfully");
    }

    if (PCF8563->begin() == false)
    {
        Serial.println("PCF8563 initialization fail");
        delay(2000);
    }
    else
    {
        Serial.println("PCF8563 initialization successfully");
    }

    gfx->begin();
    gfx->fillScreen(WHITE);

    Original_Test_Loop();

    gfx->setTextSize(1);
    gfx->fillScreen(PALERED);
}

void loop()
{
    if (millis() > CycleTime)
    {
        GFX_Print_Time_Info_Loop();
        CycleTime = millis() + 1000;
    }

    if (digitalRead(0) == LOW)
    {
        delay(300);
        Current_Rotation++;
        if (Current_Rotation > 3)
        {
            Current_Rotation = 0;
        }

        gfx->setRotation(Current_Rotation);

        switch (Image_Flag)
        {
        case 0:

            switch (Current_Rotation)
            {
            case 0:
            case 2:
                gfx->draw16bitRGBBitmap(0, 0, (uint16_t *)gImage_1, SCREEN_WIDTH, SCREEN_HEIGHT); // RGB
                break;
            case 1:
            case 3:
                gfx->draw16bitRGBBitmap(0, 0, (uint16_t *)gImage_3, SCREEN_HEIGHT, SCREEN_WIDTH); // RGB
                break;

            default:
                break;
            }

            break;
        case 1:
            switch (Current_Rotation)
            {
            case 0:
            case 2:
                gfx->draw16bitRGBBitmap(0, 0, (uint16_t *)gImage_2, SCREEN_WIDTH, SCREEN_HEIGHT); // RGB
                break;
            case 1:
            case 3:
                gfx->draw16bitRGBBitmap(0, 0, (uint16_t *)gImage_4, SCREEN_HEIGHT, SCREEN_WIDTH); // RGB
                break;

            default:
                break;
            }
            break;

        default:
            break;
        }
    }

    if (CST226SE->IIC_Interrupt_Flag == true)
    {
        CST226SE->IIC_Interrupt_Flag = false;
        delay(100);

        uint8_t fingers_number = CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH_FINGER_NUMBER);

        if (fingers_number > 0)
        {
            int32_t touch_x = CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH_COORDINATE_X);
            int32_t touch_y = CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH_COORDINATE_Y);

            Touch_Rotation_Convert(&touch_x, &touch_y);

            switch (Image_Flag)
            {
            case 0:

                switch (Current_Rotation)
                {
                case 0:
                case 2:
                    gfx->draw16bitRGBBitmap(0, 0, (uint16_t *)gImage_1, SCREEN_WIDTH, SCREEN_HEIGHT); // RGB
                    break;
                case 1:
                case 3:
                    gfx->draw16bitRGBBitmap(0, 0, (uint16_t *)gImage_3, SCREEN_HEIGHT, SCREEN_WIDTH); // RGB
                    break;

                default:
                    break;
                }

                break;
            case 1:
                switch (Current_Rotation)
                {
                case 0:
                case 2:
                    gfx->draw16bitRGBBitmap(0, 0, (uint16_t *)gImage_2, SCREEN_WIDTH, SCREEN_HEIGHT); // RGB
                    break;
                case 1:
                case 3:
                    gfx->draw16bitRGBBitmap(0, 0, (uint16_t *)gImage_4, SCREEN_HEIGHT, SCREEN_WIDTH); // RGB
                    break;

                default:
                    break;
                }
                break;

            default:
                break;
            }

            Image_Flag++;

            if (Image_Flag > 1)
            {
                Image_Flag = 0;
            }

            Serial.printf("[1] point x: %d  point y: %d \r\n", touch_x, touch_y);

            gfx->setTextSize(1);
            gfx->setCursor(touch_x, touch_y);
            gfx->fillCircle(touch_x, touch_y, 3, RED);
            gfx->setCursor(touch_x, touch_y);
            gfx->setTextColor(RED);
            gfx->printf(" Fingers Number:%d", fingers_number);
            gfx->setCursor(touch_x, touch_y + 10);
            gfx->printf(" Touch X:%d Y:%d", touch_x, touch_y);
        }
    }
}