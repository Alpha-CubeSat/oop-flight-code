"""
    This file plots IMU data!

    First, insert the following lines into main before the mcl.execute() call.

    int start = 1;
    if (start) {
        sfr::imu::powered = true; // Assuming initialization went correctly
        sfr::mission::current_mode = sfr::mission::bootCamera;
    }

    Insert the following statements at the point of the main control loop (or within an execution task in the MCL)
    where you want to collect IMU data.

    Serial.print("Gyro_X: ");
    Serial.print((gyro_x));
    Serial.print(" Gyro_Y: ");
    Serial.print((gyro_y));
    Serial.print(" Gyro_Z: ");
    Serial.print((gyro_z));
    Serial.print(" Time: ");
    Serial.println(millis());

    Run the MCL and log the Serial Monitor using PuTTY.
    Instructions are here: https://www.eye4software.com/hydromagic/documentation/articles-and-howtos/serial-port-logging/

    Update this file to include the correct file path of the putty log

    Finally, run this file (type [python plot_serial_imu_data.py] into terminal) to see the plots
"""
import matplotlib.pyplot as plt

if __name__ == "__main__":
    with open(r"C:\Users\Pawn1st\oop-flight-code\tools\putty.log", 'r', encoding='utf-8') as file:
        data = file.readlines()

    file.close()

    gyro_x_values = []
    gyro_y_values = []
    gyro_z_values = []
    time_values = []

    for line in data:
        if "Gyro_X:" in line:
            line_array = line.split()
            gyro_x_values.append(float(line_array[1]))
            gyro_y_values.append(float(line_array[3]))
            gyro_z_values.append(float(line_array[5]))
            time_values.append(float(line_array[7]))

    plt.plot(time_values, gyro_x_values, label = "Gyro X")
    plt.plot(time_values, gyro_y_values, label = "Gyro Y")
    plt.plot(time_values, gyro_z_values, label = "Gyro Z")
    plt.title('IMU Gyroscope Values vs Time')
    plt.xlabel('Time (ms)')
    plt.ylabel('Gyroscope Values')
    plt.legend()
    plt.show()