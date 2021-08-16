#include <mcp_can.h>
#include <SPI.h>

#define FG1 0 // else FG2

static long unsigned int rxId;
static unsigned char len = 0;
static unsigned char rxBuf[8];

#define CAN0_INT 9 // Set INT to pin 9
MCP_CAN CAN0(10);  // Set CS to pin 10

typedef struct rec_tag {
    long unsigned int rxId;
    unsigned char len;
    unsigned char rxBuf[8];
} rec;

#define CANSPEED CAN_500KBPS
#define CRYSTAL  MCP_8MHZ

static unsigned long tm3 = 0;

void setup() {
    Serial.begin(115200);
    while (!Serial) {}
    Serial.println("Starting...");

    // Initialize MCP2515 running at 8MHz with a baudrate of 500kb/s and the masks and filters disabled.
    if(CAN0.begin(MCP_ANY, CANSPEED, CRYSTAL) == CAN_OK)
        Serial.println("Success");
    else
        Serial.println("Error");
  
    CAN0.setMode(MCP_NORMAL); // Set operation mode to normal so the MCP2515 sends acks to received data.

    pinMode(CAN0_INT, INPUT); // Configuring pin for /INT input

    Serial.println("");

    tm3 = millis ();
}

//                                         VV    V => bits 1111 1111 1100 = Speed
rec send_wp1 = { 0x207, 8, { 0x00, 0xA1, 0x00, 0x00, 0x00, 0x00, 0x3B, 0x7F } }; // Speed 0x01=3 x50=207 x60=248 (384=248 == 1.5483)
rec send_wp2 = { 0x207, 8, { 0x00, 0x5F, 0x00, 0x00, 0x00, 0x00, 0x3B, 0x7F } }; // Speed 0
//                                   ^^ = 5F 5E 4B
//                                   ^^ - Water-Temp -60 (0x3D) 0xA1=Half (100c) [FG2]

rec send_wp3 = { 0x15F, 8, { 0x00, 0x00, 0xB0, 0x00, 0xD2, 0x00, 0xED, 0xAC } }; // xE00=MAX (7000), xE00=3584 ~ RPM/2, xDAC=7000
rec send_wp4 = { 0x15F, 8, { 0x00, 0x00, 0xB0, 0x00, 0xD2, 0x00, 0xE0, 0x00 } };
//

#if FG1 == 0

  rec send_wp5 = { 0x120, 8, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0x80 } }; // Boost (MAP)
//rec send_wp5 = { 0x120, 8, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x27, 0x00 } }; // Boost (MAP) for 1-Bar MAX range test
//rec send_wp5 = { 0x120, 8, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0xE0 } }; // Boost (MAP) for 2-Bar range test
  rec send_wp6 = { 0x120, 8, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };
//                                                                   ^^----^ 0x26E0=max

#else // IS A FG1:-

rec send_wp5 = { 0x425, 8, { 0x00, 0x00, 0x00, 0x13, 0x80, 0x00, 0x00, 0x00 } }; // Boost
rec send_wp6 = { 0x425, 8, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };
//                                               ^^----^ 0x26 0xC0 --> 0x27 0x00

rec send_wp7 = { 0x427, 8, { 0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }; // FG1 Water Temp
rec send_wp8 = { 0x427, 8, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };
//                             ^^-50=0, 51=1 79 = Half? A3=FULL
#endif

rec send_wp9 = { 0x44D, 8, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8C, 0x00 } }; // Oil Temp
rec send_wpA = { 0x44D, 8, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };
//                                                                 ^^-0x58= 50c 0x5C=51c
//                                                                 ^^-0xBC=149c 0xC0=150c
//           22*4=50     35*4 [140](0x8c)=100     48*4=150

//{ 0x12D, 8, { 0x77, 0x00, 0x00, 0x00, 0x12, 0x72, 0x00, 0x00 } }
//{ 0x143, 8, { 0x60, 0x00, 0x18, 0x00, 0x80, 0x12, 0x7F, 0x55 } }

//{ 0x15F, 8, { 0x00, 0x00, 0xB0, 0x00, 0xD1, 0xF4, 0xE2, 0x4E } }

//{ 0x207, 8, { 0x00, 0x5E, 0x00, 0x00, 0x00, 0x00, 0x3B, 0x7F } } // *******
//{ 0x274, 8, { 0x00, 0x2C, 0x00, 0x00, 0xA0, 0x00, 0x99, 0xEC } }
//{ 0x35C, 8, { 0x44, 0x14, 0x00, 0x00, 0x16, 0x77, 0x01, 0x72 } }
//{ 0x425, 8, { 0x00, 0x00, 0x00, 0x00, 0x68, 0x1F, 0x7F, 0xFB } }
//{ 0x44D, 8, { 0x02, 0x5F, 0x3E, 0x60, 0x2E, 0xD0, 0x47, 0xBA } }
//{ 0x4B0, 8, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }
//{ 0x0E7, 8, { 0x00, 0x00, 0x60, 0x00, 0x0F, 0xFF, 0x00, 0x00 } }
//{ 0x108, 8, { 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00 } }
//{ 0x211, 8, { 0x60, 0xFA, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00 } }
//{ 0x232, 8, { 0x05, 0x32, 0x60, 0x00, 0xBE, 0x8A, 0xFF, 0xFF } }
//{ 0x33B, 8, { 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x8A } }
//{ 0x0C9, 8, { 0x01, 0xFF, 0xA4, 0x07, 0x04, 0xB8, 0x00, 0x00 } }
//{ 0x3E9, 8, { 0x00, 0x00, 0x42, 0x82, 0x03, 0x00, 0x00, 0x63 } }
//{ 0x209, 8, { 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }
//{ 0x367, 8, { 0x00, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }
//{ 0x427, 8, { 0x00, 0x80, 0x26, 0x26, 0x03, 0x00, 0x08, 0x02 } }
//{ 0x4B0, 8, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }
//{ 0x023, 8, { 0x05, 0x00, 0x03, 0x00, 0x61, 0x00, 0x00, 0x00 } }
//{ 0x085, 8, { 0x00, 0x00, 0x00, 0x00, 0x03, 0x1C, 0x83, 0x1B } }
//{ 0x097, 8, { 0x00, 0x4E, 0x00, 0x49, 0x00, 0x41, 0x00, 0x69 } }
//{ 0x0A5, 8, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00 } }
//{ 0x0FC, 8, { 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }
//{ 0x120, 8, { 0x00, 0x41, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00 } }

//{ 0x12D, 8, { 0x6F, 0x00, 0x00, 0x00, 0x14, 0x7D, 0x00, 0x00 } }
//{ 0x12D, 8, { 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }

static int what = 0;
static int state = 0;

static void setSpeed (unsigned int s) {
    #if FG1 == 0
        s = (unsigned int)((double)s * 1.565);
        s <<= 6;
        send_wp1.rxBuf [2] = (s >>  8) & 0xFF;
        send_wp1.rxBuf [3] &= 0x3F;
        send_wp1.rxBuf [3] |= s & 0xC0;
    #else
        s *= 128;
        send_wp1.rxBuf [4] = (s >>  8) & 0xFF;
        send_wp1.rxBuf [5] = s & 0xFF;
    #endif
}

static void setRPM (unsigned int r) {
    #if FG1 == 0
        r >>= 1; // /2
        send_wp3.rxBuf [6] &= 0xF0;
        send_wp3.rxBuf [6] |= (r >> 8) & 0x0F;
        send_wp3.rxBuf [7] = r & 0xFF;
    #else
        r *= 4;
        send_wp1.rxBuf [0] |= (r >> 8) & 0xFF;
        send_wp1.rxBuf [1] = r & 0xFF;
    #endif
}

#define MSGWAIT 20

static int jj = 1;
static int ii = 0x1380;

void loop() {
    char a;
    unsigned long now = millis ();

    if ((state) && (now - tm3 > 100 /*2000*/)) {
        tm3 = now;
        switch (what++) {
        case 0:
            if (state) {
                setSpeed (100);
              //setSpeed (140);
              //setSpeed (220);
              //setSpeed (259);
              //setSpeed (260);
              //setSpeed (261);
              //setSpeed (262);
              //setSpeed (263);
                CAN0.sendMsgBuf(send_wp1.rxId, 0, send_wp1.len, &(send_wp1.rxBuf [0]));
                delay (MSGWAIT);

                #if FG1 == 0
                    //send_wp5.rxBuf [6] = ii >> 8;
                    //send_wp5.rxBuf [7] = ii & 0xFF;
                    if (jj == 0) {
                        ii += 48;
                        if (ii >= 0x2700)
                            jj = 1;
                    } else {
                        ii -= 48;
                        if (ii <= 0) {
                            ii = 0;
                            jj = 0;
                        }
                    }
                #endif
                CAN0.sendMsgBuf(send_wp5.rxId, 0, send_wp5.len, &(send_wp5.rxBuf [0]));
                delay (MSGWAIT);

                #if FG1 == 1
                    CAN0.sendMsgBuf(send_wp7.rxId, 0, send_wp7.len, &(send_wp7.rxBuf [0]));
                    delay (MSGWAIT);
                #endif
                CAN0.sendMsgBuf(send_wp9.rxId, 0, send_wp9.len, &(send_wp9.rxBuf [0]));
            }
            break;
        case 1:
            if (state) {
                setRPM (3500);
              //setRPM (4000);
              //setRPM (4500);
              //setRPM (5000);
              //setRPM (5500);
              //setRPM (5750);
              //setRPM (5998);
              //setRPM (6000); // Normal FPV Alarm Point
              //setRPM (7000);
              //setRPM (8000);
                #if FG1 == 0
                    CAN0.sendMsgBuf(send_wp3.rxId, 0, send_wp3.len, &(send_wp3.rxBuf [0]));
                #endif
            }
            what = 0;
            break;
        default:
            break;
        }
    }
    while (Serial.available ()) {
        a = Serial.read (); // Flush

        if (a == 'r') { // Run
            state = 1;
        }
        if (a == 's') { // Stop
            state = 0;
            CAN0.sendMsgBuf(send_wp2.rxId, 0, send_wp2.len, &(send_wp2.rxBuf [0]));
            delay (MSGWAIT);
            #if FG1 == 0
                CAN0.sendMsgBuf(send_wp4.rxId, 0, send_wp4.len, &(send_wp4.rxBuf [0]));
              //setRPM (5299);
              //setRPM (5300);
              //CAN0.sendMsgBuf(send_wp3.rxId, 0, send_wp3.len, &(send_wp3.rxBuf [0]));
                delay (MSGWAIT);
            #endif
            CAN0.sendMsgBuf(send_wp6.rxId, 0, send_wp6.len, &(send_wp6.rxBuf [0]));
            delay (MSGWAIT);
            #if FG1 == 1
                CAN0.sendMsgBuf(send_wp8.rxId, 0, send_wp8.len, &(send_wp8.rxBuf [0]));
                delay (MSGWAIT);
            #endif
            CAN0.sendMsgBuf(send_wpA.rxId, 0, send_wpA.len, &(send_wpA.rxBuf [0]));
        }
    }
    while (!digitalRead(CAN0_INT)) { // Flush
        CAN0.readMsgBuf(&rxId, &len, rxBuf);
    }
}
