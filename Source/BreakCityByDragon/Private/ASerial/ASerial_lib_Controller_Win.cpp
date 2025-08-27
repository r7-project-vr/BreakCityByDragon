#include "ASerial/ASerial_lib_Controller_Win.h"
#include "ASerial/WindowsSerial.h"
#include "ASerial/ASerial_ErrorCodeList.h"

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <string>



ASerial_lib_Controller_Win::ASerial_lib_Controller_Win(int target_device_id, int device_ver)
    : ASerialPacket(target_device_id)
{
    m_device_ver_max = device_ver;
    m_device_ver_min = device_ver;
}

ASerial_lib_Controller_Win::ASerial_lib_Controller_Win(int target_device_id, int device_ver_min, int device_ver_max)
    : ASerialPacket(target_device_id)
{
    m_device_ver_max = device_ver_max;
    m_device_ver_min = device_ver_min;
}

int ASerial_lib_Controller_Win::ConnectDevice(int COM_num)
{
    if (GetConnectionState() == true) {
        return -1;
    }

    int st = m_inteface->OpenPort(COM_num);
    if (st != 0) {
        return -1;
    }

    m_inteface->clear(); //接続したときにバッファをクリア

    st = WriteData(RESERVED_COMMAND_GET_INFO);

    ASerialDataStruct::ASerialData data_buf;

    clock_t read_time = clock();

    while (1) {
        st = ReadDataProcess(&data_buf);

        if (st != 0 || clock() - read_time >= 50) {       
            break; 
        }
    }

    if (clock() - read_time >= 50 || st == -1 || data_buf.data[0] != GetID() ||
        (data_buf.data[1] < m_device_ver_min && data_buf.data[1] > m_device_ver_max)) {
        m_inteface->ClosePort();
        return -1;
    }

    SetConnectionState(true);

    return 0;
}

int ASerial_lib_Controller_Win::AutoConnectDevice(void) {
    if (GetConnectionState() == true) {
        return -1;
    }

    int ret = 0;

    for(int i = 1; i <= 255; ++i) {
        int st = ConnectDevice(i);

        if(st == 0) {
            ret = i;
            break;
        }
    }

    if(ret == 0) {
        return -1;
    }

    return ret;
}

int ASerial_lib_Controller_Win::DisConnectDevice(void) {
    if(m_inteface->GetState() == false) {
        return -1;
    }

    int st = m_inteface->ClosePort();

    SetConnectionState(false);
    
    return st;
}

void ASerial_lib_Controller_Win::SetInterfacePt(WindowsSerial* interface_pt) { m_inteface = interface_pt; }

int ASerial_lib_Controller_Win::ReadDataProcess(ASerialDataStruct::ASerialData* read_data_buf)
{
    if (m_inteface->GetState() == false) {
        return -1;
    }

    int st = 0;
    if (m_inteface->available() > 0) {
        int read_c = m_inteface->read();
        if(read_c != -1){
            st = ReadPacketData(read_c, read_data_buf);
        }
    }

    if (st == -1) {
        return -1;
    }

    return st;
}

int ASerial_lib_Controller_Win::ReadData(ASerialDataStruct::ASerialData *read_data_buf) {
    if (m_inteface->GetState() == false) {
        return -1;
    }

    constexpr clock_t time_out = 50;
    
    bool time_out_flag = false;
    bool error_flag = false;

    clock_t time_buf = clock();
    while(1) {
        int st = ReadDataProcess(read_data_buf);

        if(st == 1) {
            break;
        }
        else if(st == -1) {
            error_flag = true;
            break;
        }

        if(clock() - time_buf >= time_out) {
            time_out_flag = true;
            break;
        }
    }

    if(error_flag == true) {    //読み取りエラー
        return -1;
    }
    else if(time_out_flag == true) { //タイムアウトエラー
        return -2;
    }

    return 0;
}

int ASerial_lib_Controller_Win::WriteData(uint8_t command, uint8_t* data, uint8_t data_num)
{
    int BUF_SIZE = GetNeedPacketBufSize(data, data_num);

    uint8_t* packet_buf = new uint8_t[BUF_SIZE];

    int st = MakePacketData(data, data_num, command, packet_buf);

    if (st == -1) {
        delete[] packet_buf;
        return -1;
    }

    int write_size = m_inteface->write(packet_buf, BUF_SIZE);

    delete[] packet_buf;

    if (write_size != BUF_SIZE) {
        return -1;
    }

    return 0;
}

int ASerial_lib_Controller_Win::WriteData(uint8_t command)
{
    const int BUF_SIZE = 6;

    uint8_t packet_buf[BUF_SIZE] = {0};

    int st = MakePacketData(command, packet_buf);

    if (st == -1) {
        return -1;
    }

    int write_size = m_inteface->write(packet_buf, BUF_SIZE);

    if (write_size != BUF_SIZE) {
        return -1;
    }

    return 0;
}