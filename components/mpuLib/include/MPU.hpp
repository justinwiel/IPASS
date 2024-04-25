// Boost Software License - Version 1.0 - August 17th, 2003

// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:

// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
//-------------------------------
// author: Justin van der Wiel 2021
//--------------------------------

/// @file
/// \brief 
/// c++ library for basic interaction with the mpu6050
/// \mainpage
/// \author Justin van der Wiel
/// \version 1.0 (last edited 29-6-2021)
/// \copyright boost license
///
/// ------------------------------------
///  This library is meant to be used with the mpu6050 chip, it does not include the i2c master capabilities or the auxilary bus
///  to use the library it's important you read the documentation and understand the way it outputs data
/// -------------------------------------

#include <cstdint>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_system.h"
#ifndef MPU_HPP
#define MPU_HPP

        constexpr int  SELF_TEST_X_ACCEL =   0x0D; //adaptation of the constexpr int list from: https://github.com/simondlevy/MPU/blob/master/src/MPU.h
        constexpr int  SELF_TEST_Y_ACCEL =  0x0E;   
        constexpr int  SELF_TEST_Z_ACCEL =  0x0F;
        constexpr int  SELF_TEST_A     =    0x10;
        constexpr int  XG_OFFSET_H      =   0x13;
        constexpr int  XG_OFFSET_L      =   0x14;
        constexpr int  YG_OFFSET_H      =   0x15;
        constexpr int  YG_OFFSET_L      =   0x16;
        constexpr int  ZG_OFFSET_H      =   0x17;
        constexpr int  ZG_OFFSET_L      =   0x18;
        constexpr int  SMPLRT_DIV       =   0x19;
        constexpr int  I2C_MST_EN       =   0x20;
        constexpr int  ACCEL_CONFIG2    =   0x1D;
        constexpr int  LP_ACCEL_ODR     =   0x1E;
        constexpr int  MOT_THR          =   0x1F;   
        constexpr int  MOT_DUR          =   0x20;  
        constexpr int  CONFIG           =   0x1A;
        constexpr int  GYRO_CONFIG      =   0x1B;
        constexpr int  ACCEL_CONFIG     =   0x1C;
        constexpr int  ZMOT_THR         =   0x21;  
        constexpr int  ZRMOT_DUR        =   0x22;  
        constexpr int  FIFO_EN          =   0x23;
        constexpr int  I2C_MST_CTRL     =   0x24;
        constexpr int  I2C_SLV0_ADDR    =   0x25;
        constexpr int  I2C_SLV0_REG     =   0x26;
        constexpr int  I2C_SLV0_CTRL    =   0x27;
        constexpr int  I2C_SLV1_ADDR    =   0x28;
        constexpr int  I2C_SLV1_REG     =   0x29;
        constexpr int  I2C_SLV1_CTRL    =   0x2A;
        constexpr int  I2C_SLV2_ADDR    =   0x2B;
        constexpr int  I2C_SLV2_REG     =   0x2C;
        constexpr int  I2C_SLV2_CTRL    =   0x2D;
        constexpr int  I2C_SLV3_ADDR    =   0x2E;
        constexpr int  I2C_SLV3_REG     =   0x2F;
        constexpr int  I2C_SLV3_CTRL    =   0x30;
        constexpr int  I2C_SLV4_ADDR    =   0x31;
        constexpr int  I2C_SLV4_REG     =   0x32;
        constexpr int  I2C_SLV4_DO      =   0x33;
        constexpr int  I2C_SLV4_CTRL    =   0x34;
        constexpr int  I2C_SLV4_DI      =   0x35;
        constexpr int  I2C_MST_STATUS   =   0x36;
        constexpr int  INT_PIN_CFG      =   0x37;
        constexpr int  INT_ENABLE        =  0x38;
        constexpr int  DMP_INT_STATUS    =  0x39;  // Check DMP interrupt
        constexpr int  INT_STATUS        =  0x3A;
        constexpr int  ACCEL_XOUT_H      =  0x3B;
        constexpr int  ACCEL_XOUT_L      =  0x3C;
        constexpr int  ACCEL_YOUT_H      =  0x3D;
        constexpr int  ACCEL_YOUT_L      =  0x3E;
        constexpr int  ACCEL_ZOUT_H      =  0x3F;
        constexpr int  ACCEL_ZOUT_L      =  0x40;
        constexpr int  TEMP_OUT_H        =  0x41;
        constexpr int  TEMP_OUT_L        =  0x42;
        constexpr int  GYRO_XOUT_H       =  0x43;
        constexpr int  GYRO_XOUT_L       =  0x44;
        constexpr int  GYRO_YOUT_H       =  0x45;
        constexpr int  GYRO_YOUT_L       =  0x46;
        constexpr int  GYRO_ZOUT_H       =  0x47;
        constexpr int  GYRO_ZOUT_L       =  0x48;
        constexpr int  EXT_SENS_DATA_00  =  0x49;
        constexpr int  EXT_SENS_DATA_01  =  0x4A;
        constexpr int  EXT_SENS_DATA_02  =  0x4B;
        constexpr int  EXT_SENS_DATA_03  =  0x4C;
        constexpr int  EXT_SENS_DATA_04  =  0x4D;
        constexpr int  EXT_SENS_DATA_05  =  0x4E;
        constexpr int  EXT_SENS_DATA_06  =  0x4F;
        constexpr int  EXT_SENS_DATA_07  =  0x50;
        constexpr int  EXT_SENS_DATA_08  =  0x51;
        constexpr int  EXT_SENS_DATA_09  =  0x52;
        constexpr int  EXT_SENS_DATA_10  =  0x53;
        constexpr int  EXT_SENS_DATA_11  =  0x54;
        constexpr int  EXT_SENS_DATA_12  =  0x55;
        constexpr int  EXT_SENS_DATA_13  =  0x56;
        constexpr int  EXT_SENS_DATA_14  =  0x57;
        constexpr int  EXT_SENS_DATA_15  =  0x58;
        constexpr int  EXT_SENS_DATA_16  =  0x59;
        constexpr int  EXT_SENS_DATA_17  =  0x5A;
        constexpr int  EXT_SENS_DATA_18  =  0x5B;
        constexpr int  EXT_SENS_DATA_19  =  0x5C;
        constexpr int  EXT_SENS_DATA_20  =  0x5D;
        constexpr int  EXT_SENS_DATA_21  =  0x5E;
        constexpr int  EXT_SENS_DATA_22  =  0x5F;
        constexpr int  EXT_SENS_DATA_23  =  0x60;
        constexpr int  MOT_DETECT_STATUS =  0x61;
        constexpr int  I2C_SLV0_DO       =  0x63;
        constexpr int  I2C_SLV1_DO      =   0x64;
        constexpr int  I2C_SLV2_DO      =   0x65;
        constexpr int  I2C_SLV3_DO      =   0x66;
        constexpr int  I2C_MST_DELAY_CTRL =  0x67;
        constexpr int  SIGNAL_PATH_RESET=   0x68;
        constexpr int  MOT_DETECT_CTRL  =   0x69;
        constexpr int  USER_CTRL        =   0x6A;  
        constexpr int  PWR_MGMT_1       =   0x6B; 
        constexpr int  PWR_MGMT_2       =   0x6C;
        constexpr int  DMP_BANK         =   0x6D; 
        constexpr int  DMP_RW_PNT       =   0x6E;
        constexpr int  DMP_REG          =   0x6F;
        constexpr int  DMP_REG_1        =   0x70;
        constexpr int  DMP_REG_2        =   0x71;
        constexpr int  FIFO_COUNTH      =   0x72;
        constexpr int  FIFO_COUNTL      =   0x73;
        constexpr int  FIFO_R_W         =   0x74;
        constexpr int  WHO_AM_I         =   0x75; 
        constexpr int  XA_OFFSET_H      =   0x77;
        constexpr int  XA_OFFSET_L      =   0x78;
        constexpr int  YA_OFFSET_H      =   0x7A;
        constexpr int  YA_OFFSET_L      =   0x7B;
        constexpr int  ZA_OFFSET_H      =   0x7D;
        constexpr int  ZA_OFFSET_L      =   0x7E;
        constexpr int  I2C_SLV0_EN      =   0x80;
        constexpr int  I2C_READ_FLAG    =   0x80;


/// \brief
/// custom datatype for storing 3-dimentional values
/// \details
// xyz contains 3 int16_t variables which can be used to store the (processed or raw) output of the sensor

class xyz{
public:
    int16_t x,y,z;
    /// \brief
    /// constructor for the xyz data type
    /// \details
    /// constructs an xyz from the given x,y and z coördinates
    xyz(int16_t x, int16_t y, int16_t z):
    x(x),
    y(y),
    z(z)
    {}
    /// \brief
    /// constructor for the xyz data type
    /// \details
    /// constructs an xyz containing all zeros, can be used to create dummy xyz variables
    xyz():
    x(0),
    y(0),
    z(0)
    {}
    /// \brief
    /// a operator- for the xyz datatype 
    /// \details
    /// subtracts one xyz from another
    /// subtracting is done by adding together the individual integers
    xyz operator-(xyz & rhs);
    /// \brief
    /// a operator-= for the xyz datatype 
    /// \details
    /// subtracts one xyz from another and assigns it to the left operand
    /// subtracting is done by adding together the individual integers
    xyz operator-=(xyz & rhs);
    /// \brief
    /// a operator+ for the xyz datatype
    /// \details
    /// adds one xyz to another 
    /// subtracting is done by adding together the individual integers
    xyz operator+(xyz & rhs);
    /// \brief
    ///  a operator+= for the xyz datatype
    /// \details
    /// adds one xyz to another and assigns it to the left operand
    /// adding is done by adding together the individual integers
    xyz operator+=(xyz & rhs);    
    /// \brief
    ///  a operator* for the xyz datatype
    /// \details
    /// multiplies one xyz with another 
    /// multlying is done by multplying the individual integers with thieir counterpart from the second xyz
    xyz operator*(xyz & rhs);
    /// \brief
    ///  a operator*= for the xyz datatype
    /// \details
    /// multiplies one xyz with another  and assigns it to the left operand
    /// multlying is done by multplying the individual integers with thieir counterpart from the second xyz
    xyz operator*=(xyz & rhs);
    /// \brief
    ///  a operator* for the xyz datatype
    /// \details
    /// multpiplies a xyz with an integer 
    /// multlying is done by multplying the individual integers with the given integer
    xyz operator*(int16_t & rhs);
    /// \brief
    ///  a operator*= for the xyz datatype
    /// \details
    /// multpiplies a xyz with an integer and assigns it to the left operand
    /// multlying is done by multplying the individual integers with the given integer
    xyz operator*=(int16_t & rhs);

};
/// \brief 
/// a container for the full range output of the chip
/// \details
/// a class for storing and transfering the output of a mpu6050, this is NOT a data type, there are no operators
/// it is only meant as a means of quickly moving around variables
class all_values  {
public:
    xyz acc;
    xyz gyr;
    int16_t temp;
    /// \brief 
    /// this function takes the desired values and stores them int the class
    /// \details
    /// function takes two xyz variables and an integer to store and transport.
    all_values(xyz acc, xyz gyr, int16_t temp):
    acc(acc),
    gyr(gyr),
    temp(temp)
    {}

};

/// \brief
/// A class to control the mpu6050 chip 
/// \details 
/// Contains a number of functions to activate differerent and use different functions of the mpu6050 gyroscope/accelerometer
class MPU6050 {
private: 
    double  gyrosensitivity  = 0;  //has decimal values 
    int16_t  accelsensitivity = 0; // only has whole values
    uint8_t   address;
    bool  A0;
    int16_t fs_range = 0;
    all_values fifo_read_test();
public:
    /// \brief
    /// Constructor for the mpu6050 class, takes a bus and a boolian
    /// \details 
    /// This constructor for the mpu6050 class takes a bus to write to and a boolian used to determine the chip address. 
    /// if the A0 pin of the chip is either not connected or connected to ground, set this to 0.
    /// If the A0 pin of the chip is connected to VCC set this to 1

    MPU6050(bool  A0=0 ):
    A0(A0)
    { 
        address = 0x68 + A0;
    } 
    /// \brief
    /// Writes a byte to a register
    /// \details
    /// Takes the sub-address of the register you want to write and the data you want to write to it, if you write more than one byte, the chip auto-increments the address(except the for fifo register)
    void writeRegister(uint8_t sub_addr, uint8_t  data);
    /// \brief 
    /// Reads data from a register
    /// \details 
    /// Takes the sub-address of the register you want to read, a buffer and the amount of data you want to read, if you read more than one byte, the chip auto-increments the address(except the for fifo register)
    /// \n The data gets written into the buffer directly, optionally you can also choose to have the data returned into a second or the same buffer
    uint8_t* readRegister(uint8_t sub_addr, uint8_t* data, uint8_t size);
    /// \brief
    /// Intializes the chip and sets the fs_range and afs_range
    /// \details
    /// Takes the desired range setting, this is value from 0 to 3
    /// \n 0 = 250 lowest range highest sensitivity,
    /// \n 1 = 500,
    /// \n 2= 1000,
    /// \n 3 = 2000 highest range lowest sensitivity;
    /// \n Sets up everything that needs to be setup for normal operation, this function should always be the first called function in an application using this library
    void setup(int8_t range_setting);
    /// \brief
    /// Reads accelerometer data and returns an xyz
    /// \details
    /// This function reads all accelerometer data registers and turns them into 3 usable integers on a scale that can be specified by the user, the greater the scale the more unstable the data
    /// \n NOTE: The chosen scale should not go above the range chosen in the setup function 
    xyz getAccdata_scale(int desired_range);
    void getAccdata_scale(const int& desired_range,xyz*acc);
    /// \brief
    /// Reads gyroscope data and returns an xyz
    /// \details
    /// This function reads all gyroscope data registers and turns them into 3 usable integers on a scale that can be specified by the user, the greater the scale the more unstable the data
    /// \n NOTE: The chosen scale should not go above the range chosen in the setup function 
    xyz getGyrodata_scale(int desired_range);
    void getGyrodata_scale(const int& desired_range,xyz*gyr);
    /// \brief
    /// Reads temperature data and returns an integer
    /// \details
    /// This function reads all temperature data registers and processes the output with the calculation specified in the datasheet
    int16_t getTempdata();
    /// \brief
    /// Reads sensor data and returns an all_values
    /// \details
    /// This function reads all sensor data registers and turns them into 2 xyz variables, on a scale that can be specified by the user, and an integer combined into an all_values container
    /// \n NOTE: The chosen scale should not go above the range chosen in the setup function 
    all_values getAlldata_scale(int desired_range);
    /// \brief
    /// Reads accelerometer data and returns an xyz
    /// \details
    /// Reads accelerometer data registers and processes the data in the way specified in the datasheet and returns it as an xyz
    void getAccdata(xyz* acc);
    xyz getAccdata();
    /// \brief
    /// Reads gyroscope data and returns an xyz
    /// \details
    /// Reads gyroscope data registers and processes the data in the way specified in the datasheet and returns it as an xyz
    void getGyrodata(xyz* gyr);
    xyz getGyrodata();
    /// \brief
    /// Reads sensor data and returns an all_values
    /// \details
    /// This function reads all sensor data registers, processes them in the way specified in the datasheet and returns them as 2 xyz variables and an integer combined into an all_values container
    all_values getAlldata();
    /// \brief
    /// Reads accelerometer data and returns an xyz
    /// \details
    /// Reads the accelerometer data registers and returns the raw xyz values
    xyz getAccdata_raw();
    /// \brief
    /// Reads gyroscope data and returns an xyz
    /// \details
    /// Reads the gyroscope data registers and returns the raw xyz values
    xyz getGyrodata_raw();
    /// \brief
    /// Reads temperature data and returns an integer
    /// \details
    /// Reads the temperature data registers and returns the raw integer value
    int16_t getTempdata_raw();
    /// \brief
    /// Reads all data and returns an all_values
    /// \details
    /// Reads the all data registers and returns the raw xyz values and a raw integer combined into an all_values
    all_values getAlldata_raw();
    /// \brief
    /// Enables interrupts
    /// \details 
    /// Writes to the interrupt enable register and sets the flags for data ready and fifo overflow interrupt to 1
    void interrupt_enable();
    /// \brief
    /// Disables interrupts
    /// \details 
    /// Writes to the interrupt enable register and sets the flags for data ready and fifo overflow interrupt to 0
    void interrupt_disable();
    /// \brief
    /// Reads interrupt register
    /// \details
    /// Reads the interrupt status register and writes the output into a buffer 
    void read_interrupt(uint8_t data[1]);
    /// \brief
    /// Enables fifo for accelerometer and the gyroscope
    /// \details
    /// Writes to the fifo enable register and sets the enable flags for the gyroscope and accelerometer to 1
    /// \n NOTE: The fifo_read function triggers this function on it's own this is preferable over manual activation and deactivation, the buffer can get overflowed if not read enough while left open
    void fifo_enable();
    /// \brief
    /// Reads the fifo register
    /// \details
    /// This function enables the fifo register, reads the register and then disables the register 
    all_values fifo_read();
    /// \brief
    /// Disables fifo for accelerometer and the gyroscope
    /// \details
    /// Writes to the fifo enable register and sets the enable flags for the gyroscope and accelerometer to 0
    /// \n NOTE: The fifo_read function triggers this function on it's own this is preferable over manual activation and deactivation, the buffer can get overflowed if not read enough while left open
    void fifo_disable();
    /// \brief
    /// Resets the fifo buffer
    /// \details 
    /// Sets the fifo reset flag in the user control register to 1, this flag returns to 0 after triggering the reset
    void fifo_reset();

    esp_err_t InitI2C();

    int16_t getfs_range();
};


#endif