#include <Fault.hpp>
#include <Faults.hpp>
#include <sfr.hpp>
#include <unity.h>

void test_basic_fault_states()
{
  // setup
  Fault* f = fault_groups::hardware_faults::burn_wire;
  TEST_ASSERT_FALSE(f->get_base());
  TEST_ASSERT_FALSE(f->get_signaled());
  TEST_ASSERT_FALSE(f->get_supressed());

  f->signal();
  TEST_ASSERT_TRUE(f->get_base());
  TEST_ASSERT_TRUE(f->get_signaled());
  TEST_ASSERT_FALSE(f->get_supressed());

  f->suppress();
  TEST_ASSERT_FALSE(f->get_base());
  TEST_ASSERT_TRUE(f->get_signaled());
  TEST_ASSERT_TRUE(f->get_supressed());

  f->release();
  TEST_ASSERT_FALSE(f->get_base());
  TEST_ASSERT_FALSE(f->get_signaled());
  TEST_ASSERT_TRUE(f->get_supressed());

  f->unsuppress();
  TEST_ASSERT_FALSE(f->get_base());
  TEST_ASSERT_FALSE(f->get_signaled());
  TEST_ASSERT_FALSE(f->get_supressed());

}

void test_sensor_reading_is_valid(){
  SensorReading* s = sfr::current::solar_current_average;
  Fault* f = fault_groups::power_faults::solar_current;
  TEST_ASSERT_TRUE(s->is_valid());
  f->signal();
  TEST_ASSERT_FALSE(s->is_valid());
  f->suppress();
  TEST_ASSERT_TRUE(s->is_valid());
  f->release();
  TEST_ASSERT_TRUE(s->is_valid());
  f->unsuppress();
  TEST_ASSERT_TRUE(s->is_valid());
}

void test_low_power_transission(){
  SensorReading* s = sfr::battery::voltage_average;
  Fault* f = fault_groups::power_faults::voltage;
  TEST_ASSERT_TRUE(s->is_valid());
  s->set_value(6);
  TEST_ASSERT_FALSE(s->is_valid());
  s->set_value(3);
  TEST_ASSERT_TRUE(s->is_valid());
}


int test_faults()
{
  UNITY_BEGIN();
  RUN_TEST(test_basic_fault_states);
  RUN_TEST(test_sensor_reading_is_valid);
  RUN_TEST(test_low_power_transission);
  return UNITY_END();
}

#ifdef DESKTOP
int main()
{
  return test_faults();
}
#else
#include <Arduino.h>
void setup()
{
  delay(2000);
  Serial.begin(9600);
  Serial.println("Faults Test Started");
  test_faults();
}

void loop() {}
#endif