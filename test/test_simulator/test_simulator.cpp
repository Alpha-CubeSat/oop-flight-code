#include <unity.h>
#include <RockblockSimulator.hpp>
#include <constants.hpp>

void test_simulator_initialize() {
    // check signal field starts at 5
    RockblockSimulator sim;
    sim.begin(constants::rockblock::baud);
    TEST_ASSERT_EQUAL(5, sim.get_signal());

    // no data queued
    TEST_ASSERT_EQUAL(0, sim.available());
    TEST_ASSERT_EQUAL(0, sim.read());

    // no data downlinked
    std::string s;
    TEST_ASSERT(s == sim.latest_downlink());
    std::deque<std::string> d;
    TEST_ASSERT(d == sim.all_downlinks());
}

void test_signal_change() {
    RockblockSimulator sim;
    // signal starts at 5
    TEST_ASSERT_EQUAL(5, sim.get_signal());

    // signal set to 0
    sim.set_signal(0);
    TEST_ASSERT_EQUAL(0, sim.get_signal());

    // signal set to 1
    sim.set_signal(1);
    TEST_ASSERT_EQUAL(1, sim.get_signal());

    // signal set to 2
    sim.set_signal(2);
    TEST_ASSERT_EQUAL(2, sim.get_signal());

    // signal set to 3
    sim.set_signal(3);
    TEST_ASSERT_EQUAL(3, sim.get_signal());

    // signal set to 4
    sim.set_signal(4);
    TEST_ASSERT_EQUAL(4, sim.get_signal());

    // signal set to 5
    sim.set_signal(5);
    TEST_ASSERT_EQUAL(5, sim.get_signal());

    // signal set to out of bounds value
    sim.set_signal(6);
    TEST_ASSERT_EQUAL(5, sim.get_signal());
}

void test_at() {
    // initialize sim
    RockblockSimulator sim;
    sim.begin(constants::rockblock::baud);

    // send AT command
    sim.print("AT\r");

    // grab entire response
    std::string s;
    while(sim.available()) {
        s += (char) sim.read();
    }

    // confirm correct response
    std::string r = "AT\r\r\nOK\r\n";
    TEST_ASSERT(s == r);
}

void test_signal_check() {
    // initialize sim
    RockblockSimulator sim;
    sim.begin(constants::rockblock::baud);

    // loop over all valid signals
    for(size_t signal = 0; signal <= 5; signal++) {
        // set signal
        sim.set_signal(signal);

        // send signal check command
        sim.print("AT+CSQ\r");

        // grab entire response
        std::string s;
        while(sim.available()) {
            s += (char) sim.read();
        }

        // confirm correct response
        std::string r = "AT+CSQ\r\r\n+CSQ:";
        r += '0' + signal;
        r += "\r\n\r\nOK\r\n";
        TEST_ASSERT(s == r);
    }
}

void test_flow_control() {
    // initialize sim
    RockblockSimulator sim;
    sim.begin(constants::rockblock::baud);

    // send flow control command
    sim.print("AT&K0\r");

    // grab entire response
    std::string s;
    while(sim.available()) {
        s += (char) sim.read();
    }

    // confirm correct response
    std::string r = "AT&K0\r\r\nOK\r\n";
    TEST_ASSERT(s == r);
}

void test_downlink_correct_checksum() {
    // initialize sim
    RockblockSimulator sim;
    sim.begin(constants::rockblock::baud);
    
    // send binary message length command
    sim.print("AT+SBDWB=70\r");

    // grab entire response
    std::string s1;
    while(sim.available()) {
        s1 += (char) sim.read();
    }

    // confirm correct response
    std::string r1 = "AT+SBDWB=70\r\r\nREADY\r\n";
    TEST_ASSERT(s1 == r1);

    // write 70 zeros
    for(size_t i = 0; i < 70; i++) {
        sim.write(0);
    }
    // write CORRECT checksum
    sim.write(0);
    sim.write(0);
    sim.write('\r');

     // grab entire response
    std::string s2;
    while(sim.available()) {
        s2 += (char) sim.read();
    }

    // confirm correct response
    std::string r2 = "\r\n0\r\n\r\nOK\r\n";
    TEST_ASSERT(s2 == r2);
}

void test_downlink_incorrect_checksum() {
    // initialize sim
    RockblockSimulator sim;
    sim.begin(constants::rockblock::baud);

    // send binary message length command
    sim.print("AT+SBDWB=70\r");

     // grab entire response
    std::string s1;
    while(sim.available()) {
        s1 += (char) sim.read();
    }

    // confirm correct response
    std::string r1 = "AT+SBDWB=70\r\r\nREADY\r\n";
    TEST_ASSERT(s1 == r1);

    // write 70 zeros
    for(size_t i = 0; i < 70; i++) {
        sim.write(0);
    }

    // write INCORRECT checksum
    sim.write(0);
    sim.write(1);
    sim.write('\r');

    // grab entire response
    std::string s2;
    while(sim.available()) {
        s2 += (char) sim.read();
    }

    // confirm correct response
    std::string r2 = "\r\n2\r\n\r\nOK\r\n";
    TEST_ASSERT(s2 == r2);
}

void test_mailbox_empty() {
    // initialize sim
    RockblockSimulator sim;
    sim.begin(constants::rockblock::baud);

    // send binary message length command
    sim.print("AT+SBDWB=70\r");

    // grab entire response
    std::string s1;
    while(sim.available()) {
        s1 += (char) sim.read();
    }

    // confirm correct response
    std::string r1 = "AT+SBDWB=70\r\r\nREADY\r\n";
    TEST_ASSERT(s1 == r1);

    // write 70 zeros
    for(size_t i = 0; i < 70; i++) {
        sim.write(0);
    }
    // write CORRECT checksum
    sim.write(0);
    sim.write(0);
    sim.write('\r');

     // grab entire response
    std::string s2;
    while(sim.available()) {
        s2 += (char) sim.read();
    }

    // confirm correct response
    std::string r2 = "\r\n0\r\n\r\nOK\r\n";
    TEST_ASSERT(s2 == r2);

    // check mailbox
    sim.print("AT+SBDIX\r");

    // grab entire response
    std::string s3;
    while(sim.available()) {
        s3 += (char) sim.read();
    }

    // confirm correct response
    std::string r3 = "AT+SBDIX\r\r\n+SBDIX: 0, 1, 0, 1, 0, 0\r\n\r\nOK\r\n";
    TEST_ASSERT(s3 == r3);
}

void test_receive_uplink() {
    // initialize sim
    RockblockSimulator sim;
    sim.begin(constants::rockblock::baud);

    // insert command for downlink
    sim.insert("0A0B0C0D0E0F01020304");

    // send binary message length command
    sim.print("AT+SBDWB=70\r");

    // grab entire response
    std::string s1;
    while(sim.available()) {
        s1 += (char) sim.read();
    }

    // confirm correct response
    std::string r1 = "AT+SBDWB=70\r\r\nREADY\r\n";
    TEST_ASSERT(s1 == r1);

    // write 70 zeros
    for(size_t i = 0; i < 70; i++) {
        sim.write(0);
    }
    // write CORRECT checksum
    sim.write(0);
    sim.write(0);
    sim.write('\r');

     // grab entire response
    std::string s2;
    while(sim.available()) {
        s2 += (char) sim.read();
    }

    // confirm correct response
    std::string r2 = "\r\n0\r\n\r\nOK\r\n";
    TEST_ASSERT(s2 == r2);

    // check mailbox
    sim.print("AT+SBDIX\r");

    // grab entire response
    std::string s3;
    while(sim.available()) {
        s3 += (char) sim.read();
    }

    // confirm correct response
    std::string r3 = "AT+SBDIX\r\r\n+SBDIX: 0, 1, 1, 1, 10, 0\r\n\r\nOK\r\n";
    TEST_ASSERT(s3 == r3);

    // read binary data
    sim.print("AT+SBDRB\r");

    // grab entire response
    std::string s4;
    while(sim.available()) {
        s4 += (char) sim.read();
    }

    // confirm correct response
    std::string r4;
    r4 += "AT+SBDRB\r";
    r4 += (char) 0;
    r4 += (char) 10;
    r4 += (char) 10;
    r4 += (char) 11;
    r4 += (char) 12;
    r4 += (char) 13;
    r4 += (char) 14;
    r4 += (char) 15;
    r4 += (char) 1;
    r4 += (char) 2;
    r4 += (char) 3;
    r4 += (char) 4;
    r4 += (char) 0;
    r4 += (char) 85;
    r4 += "\r\nOK\r\n";
    TEST_ASSERT(s4 == r4);
}

void test_flush() {
    // initialize sim
    RockblockSimulator sim;
    sim.begin(constants::rockblock::baud);

    // insert commands for downlink
    sim.insert("0A0B0C0D0E0F01020304");
    sim.insert("0A0B0C0D0E0F01020304");
    sim.insert("0A0B0C0D0E0F01020304");
    sim.insert("0A0B0C0D0E0F01020304");

    for(size_t i = 0; i < 4; i++) {
        if(i != 2) {
            // send binary message length command
            sim.print("AT+SBDWB=70\r");

            // grab entire response
            std::string s1;
            while(sim.available()) {
                s1 += (char) sim.read();
            }

            // confirm correct response
            std::string r1 = "AT+SBDWB=70\r\r\nREADY\r\n";
            TEST_ASSERT(s1 == r1);

            // write 70 zeros
            for(size_t i = 0; i < 70; i++) {
                sim.write(0);
            }

            // write CORRECT checksum
            sim.write(0);
            sim.write(0);
            sim.write('\r');

            // grab entire response
            std::string s2;
            while(sim.available()) {
                s2 += (char) sim.read();
            }

            // confirm correct response
            std::string r2 = "\r\n0\r\n\r\nOK\r\n";
            TEST_ASSERT(s2 == r2);
        } else {
            sim.print("AT+SBDWT=FLUSH_MT\r");

             // grab entire response
            std::string s2;
            while(sim.available()) {
                s2 += (char) sim.read();
            }

            // confirm correct response
            std::string r2 = "AT+SBDWT=FLUSH_MT\r\r\nOK\r\n";
            TEST_ASSERT(s2 == r2);
        }

        // check mailbox
        sim.print("AT+SBDIX\r");

        // grab entire response
        std::string s3;
        while(sim.available()) {
            s3 += (char) sim.read();
        }

        // confirm correct response
        std::string r3;
        r3 += "AT+SBDIX\r\r\n+SBDIX: ";
        switch(i) {
            case 0:
                r3 += "0, 1, 1, 1, 10, 3";
                break;
            case 1:
                r3 += "0, 2, 1, 2, 10, 2";
                break;
            case 2:
                r3 += "0, 3, 1, 3, 10, 1";
                break;
            case 3:
                r3 += "0, 4, 1, 4, 8, 0";
                break;
        }
        r3 += "\r\n\r\nOK\r\n";
        TEST_ASSERT(s3 == r3);

        // read binary data
        sim.print("AT+SBDRB\r");

        // grab entire response
        std::string s4;
        while(sim.available()) {
            s4 += (char) sim.read();
        }

        // confirm correct response
        std::string r4;
        r4 += "AT+SBDRB\r";
        if(i != 3) {
            r4 += (char) 0;
            r4 += (char) 10;
            r4 += (char) 10;
            r4 += (char) 11;
            r4 += (char) 12;
            r4 += (char) 13;
            r4 += (char) 14;
            r4 += (char) 15;
            r4 += (char) 1;
            r4 += (char) 2;
            r4 += (char) 3;
            r4 += (char) 4;
            r4 += (char) 0;
            r4 += (char) 85;
        } else {
            r4 += (char) 0;
            r4 += (char) 8;
            r4 += "FLUSH_MT";
            r4 += (char) 2;
            r4 += (char) 130;
        }
        r4 += "\r\nOK\r\n";
        TEST_ASSERT(s4 == r4);
    }
}

int test_simulator() {
    UNITY_BEGIN();
    RUN_TEST(test_simulator_initialize);
    RUN_TEST(test_signal_change);
    RUN_TEST(test_at);
    RUN_TEST(test_signal_check);
    RUN_TEST(test_flow_control);
    RUN_TEST(test_downlink_correct_checksum);
    RUN_TEST(test_downlink_incorrect_checksum);
    RUN_TEST(test_mailbox_empty);
    RUN_TEST(test_receive_uplink);
    RUN_TEST(test_flush);
    return UNITY_END();
}

#ifdef DESKTOP
int main() {
    return test_simulator();
}
#else
#include <Arduino.h>
void setup() {
    delay(2000);
    Serial.begin(9600);
    test_simulator();
}

void loop() {}
#endif