#include "mbed.h"
#include "LSM6DSLSensor.h"


#define PI 3.141592654

PwmOut ledR(PE_3);  //RED
PwmOut ledG(LED1);  //GREEN
PwmOut ledB(LED2);  //BLUE

static DevI2C devI2c(PB_11, PB_10);
static LSM6DSLSensor acc_gyro(&devI2c, 0xD4, D4, D5);// high address

float computeAngle(int x, int y, int z){

  float res;  //added this on a seperate line
  res =(float)(atan((x)/(sqrt((y*y)+(z*z)))) * (180 / PI)); //equation 6 as given and converted to degrees.

  return res;
}

float computeroll(int x, int y, int z){

  float roll;
  roll = (float)(atan((y)/(sqrt((x*x)+(z*z)))) * (180 / PI));//equation 7 as given and converted to degrees.

  return roll;

}
/* Simple main function */
int main() {
    uint8_t id;
    int32_t axes[3];
    float res = 0;
    float roll = 0;
    float RED = 0;
    float BLUE = 0;
    float GREEN = 0;


    acc_gyro.init(NULL);

    acc_gyro.enable_x();
    acc_gyro.enable_g();

    printf("This is an accelerometer example running on Mbed OS %d.%d.%d.\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
    acc_gyro.read_id(&id);
    printf("LSM6DSL accelerometer & gyroscope = 0x%X\r\n", id);
    printf ("The anlgles for pitch and roll are in degrees\r\n", res );
    printf("TKELLY_Lab_3\r\n");

    while (1) {// while loop

        acc_gyro.get_x_axes(axes); //get data from gyro

        
        res = computeAngle(axes[0], axes[1], axes[2]); //calling pitch angle function
        roll = computeroll(axes[0], axes[1], axes[2]); //calling roll angle function

        // Print the x,y,z values and the pitch angle and roll angle.

        printf("LSM6DSL: %6d, %6d, %6d, Pitch is %3.2f, Roll is %3.2f\r\n", axes[0], axes[1], axes[2], res, roll);
        


            if(axes[2] != 0){

                //adjusting leds based on hz of time on/off
                //Blue Led PC9 just wouldn't dim???

                ledB.write(BLUE);
                BLUE = (((sqrt(axes[0]*axes[0]))/(1020)));  //ajusts Green led2 in place of blue led for x
                ledR.write(RED);
                RED = (((sqrt(axes[1]*axes[1]))/(1020)));  //ajusts RED led for y
                ledG.write(GREEN);
                GREEN = (((sqrt(axes[2]*axes[2]))/(1020)));  //ajusts green led1 for Z

            }
            

        thread_sleep_for(500);  //sleep 0.5 seconds and return to begining of loop
    }
}