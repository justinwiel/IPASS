#include "MPU.hpp"
#include "hwlib.hpp"



void MPU6050::setup(uint8_t range_setting){
    switch(range_setting){
        case 0:
            fs_range = 250;
            break;
        case 1:
            fs_range = 500;
            break;
        case 2:
            fs_range = 1000;
            break;
        case 3: 
            fs_range = 2000;
            break;

    }
    auto to_write =  (fs_range << 4) ; // first three bytes are ignored as such the value needs to be shifted 3 before being written
    writeRegister(I2C_MST_STATUS,0b10000000);
    writeRegister(PWR_MGMT_1,0x80);
    hwlib::wait_ms(100);
    writeRegister(PWR_MGMT_1,0b0001);
    writeRegister(PWR_MGMT_2,0x00);
    hwlib::wait_ms(200);
    writeRegister(INT_ENABLE,0x00);
    writeRegister(FIFO_EN,0x00);
    writeRegister(PWR_MGMT_1,0x00);
    writeRegister(I2C_MST_CTRL,0x00);
    writeRegister(USER_CTRL,0x00);
    writeRegister(USER_CTRL,0x0c);
    hwlib::wait_ms(15);
    writeRegister(CONFIG,0b00000001);
    writeRegister(SMPLRT_DIV, 0);
    writeRegister(GYRO_CONFIG,to_write);
    writeRegister(ACCEL_CONFIG, to_write);
    writeRegister(USER_CTRL, 0x40);
    writeRegister(SIGNAL_PATH_RESET,0b00000111);
    writeRegister(SIGNAL_PATH_RESET,0b00000000);
    writeRegister(GYRO_CONFIG,0b00011000);
    writeRegister(ACCEL_CONFIG, 0b00011000);
}

void MPU6050::writeRegister(uint8_t sub_adrr, uint8_t  data){
    auto wbus = hwlib::i2c_write_transaction (I2C_bus, address);
    wbus.write(sub_adrr) ;
    wbus.write(data);
    wbus.~i2c_write_transaction();
}

uint8_t* MPU6050::readRegister(uint8_t sub_addr, uint8_t* data, uint8_t size){
    auto wbus = hwlib::i2c_write_transaction (I2C_bus, address);
    wbus.write(sub_addr) ;
    wbus.~i2c_write_transaction();
    auto rbus = hwlib::i2c_read_transaction (I2C_bus, 0x68);
    rbus.read(data,size);
    rbus.~i2c_read_transaction();
    return data;
}

xyz xyz::operator-(xyz & rhs){
    return xyz(x - rhs.x , y - rhs.y, z - rhs.z);
}

xyz xyz::operator-=(xyz & rhs){
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
}
xyz xyz::operator+(xyz & rhs){
    return xyz(x + rhs.x,y + rhs.y,  z + rhs.z);
}
xyz xyz::operator+=(xyz & rhs){
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}
xyz xyz::operator*(xyz & rhs){
    return xyz(x * rhs.x,y * rhs.y,  z * rhs.z);

}
xyz xyz::operator*=(xyz & rhs){
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;
    return *this;
}
xyz xyz::operator*(int16_t & rhs){
    return xyz(x * rhs,y * rhs,  z * rhs);
}
xyz xyz::operator*=(int16_t & rhs){
    x *= rhs;
    y *= rhs;
    z *= rhs;
    return *this;
}


xyz MPU6050::getAccdata(int desired_range){
    uint8_t data[6];
        readRegister(ACCEL_XOUT_H,data,6);
    int16_t x = (int16_t)(data[0] << 8) | data[1];
    int16_t y = (int16_t)(data[2] << 8) | data[3];
    int16_t z = (int16_t)(data[4] << 8) | data[5];
    return xyz((x/(fs_range/desired_range))%(desired_range+1),(y/(fs_range/desired_range))%(desired_range+1),(z/(fs_range/desired_range))%(desired_range+1)); 
}
xyz MPU6050::getGyrodata(int desired_range){
    uint8_t data[6];
        readRegister(GYRO_XOUT_H,data,6);
    int16_t x = (int16_t)(data[0] << 8) | data[1];
    int16_t y = (int16_t)(data[2] << 8) | data[3];
    int16_t z = (int16_t)(data[4] << 8) | data[5];
    return xyz((x/(fs_range/desired_range))%(desired_range+1),(y/(fs_range/desired_range))%(desired_range+1),(z/(fs_range/desired_range))%(desired_range+1)); 
}
int16_t MPU6050::getTempdata(){
    uint8_t data[2];
        readRegister(TEMP_OUT_H,data,2);
    int16_t temp = (data[0] << 8) | data[1];

    return temp/340+36.53;
}

all_values MPU6050::getAlldata(int desired_range){
    return all_values( getGyrodata(desired_range),getAccdata(desired_range),getTempdata());
}

xyz MPU6050::getAccdata_raw(){
    uint8_t data[6];
        readRegister(ACCEL_XOUT_H,data,6);
    int16_t x = (int16_t)(data[0] << 8) | data[1];
    int16_t y = (int16_t)(data[2] << 8) | data[3];
    int16_t z = (int16_t)(data[4] << 8) | data[5];
    return xyz(x,y,z); 
}
xyz MPU6050::getGyrodata_raw(){
    uint8_t data[6];
        readRegister(GYRO_XOUT_H,data,6);
    int16_t x = (int16_t)(data[0] << 8) | data[1];
    int16_t y = (int16_t)(data[2] << 8) | data[3];
    int16_t z = (int16_t)(data[4] << 8) | data[5];
    return xyz(x,y,z); 
}
int16_t MPU6050::getTempdata_raw(){
    uint8_t data[2];
        readRegister(TEMP_OUT_H,data,2);
    int16_t temp = (data[0] << 8) | data[1];

    return temp;
}

all_values MPU6050::getAlldata_raw(){
    return all_values( getGyrodata_raw(),getAccdata_raw(),getTempdata_raw());
}

void MPU6050::test(hwlib::pin_in & switch_button ){
    writeRegister(0x6b,0);
    auto oled = hwlib::glcd_oled( I2C_bus, 0x3c );  
    auto f1 = hwlib::font_default_8x8();
    auto d1 = hwlib::terminal_from(oled,f1);
    setup(3);
    writeRegister(PWR_MGMT_1, 0x00);
    all_values all_data = getAlldata(10);
    bool raw = false;
    for(;;){
        if(switch_button.read()){
            switch(raw){
                case true:
                    raw  = false;
                    break;
                case false:
                    raw = true;
                    break;
            }
            hwlib::wait_ms(1);
        }
        switch (raw){
            case true:
                all_data = getAlldata_raw();
                break;
            case false:
                all_data = getAlldata(10);
                break;


        }
        d1 <<  '\f' << "acc_x: " << all_data.acc.x  << "\nacc_y: " << all_data.acc.y
        << "\nacc_z: " << all_data.acc.z << "\ntemp: " << all_data.temp << "\ngyro_x: " << all_data.gyr.x << 
        "\ngyro_y: " << all_data.gyr.y << "\ngyro_z: " << all_data.gyr.z <<hwlib::flush;
        hwlib::wait_ms(10);
    }
 	
}
