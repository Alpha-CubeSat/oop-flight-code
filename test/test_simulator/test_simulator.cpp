#include <unity.h>
#include <RockblockSimulator.hpp>
#include <constants.hpp>

void test_simulator_initialize() {
    // check signal field starts at 0
    RockblockSimulator sim;
    TEST_ASSERT_EQUAL(0, sim.get_signal());

    // check that begin sets signal to 5
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
    // signal starts at 0
    TEST_ASSERT_EQUAL(0, sim.get_signal());

    // signal set to 3
    sim.set_signal(3);
    TEST_ASSERT_EQUAL(3, sim.get_signal());

    // signal set to out of bounds value
    sim.set_signal(6);
    TEST_ASSERT_EQUAL(3, sim.get_signal());

    // signal set to max val
    sim.set_signal(5);
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

}

void test_receive_uplink() {

}

void test_flush() {

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
    // RUN_TEST(test_mailbox_empty);
    // RUN_TEST(test_flush);
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