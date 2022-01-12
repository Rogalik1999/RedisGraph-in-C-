//---------------------------------------------------------------------------

#pragma hdrstop

#include "StringBuffer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


StringBuffer::StringBuffer(const char* data, int len): size_buffer(len+1), current_position(0), actual_data("")
{
    ///Przypadki gdy nie wiadomo czy string ma znak konczacy czy nie
    buffer = new char[size_buffer];
    strncpy(buffer,data,size_buffer);
    buffer[size_buffer-1] = '\0';

    ///Gdy uzytkownik poda dlugosc z znakiem \0 to wtedy bedziemy mieli na koncu dwa null'e i trzeba odjac o dwa
    /// jesli uzytkownik poda dlugosc bez znaku to wtedy odejmujemy jeden bo jest tylko jeden null
    if(buffer[size_buffer-2] == 0) lenBuffer = size_buffer-2;
    else lenBuffer = size_buffer-1;

    current_pointer_position = buffer;

}

StringBuffer::~StringBuffer()
{
    delete [] buffer;
    std::cout<<"czyszcze sie\n";
}

int StringBuffer::readDataByChr(const char* str, bool getIfNool){

    if(current_position == lenBuffer){
        return STRING_BUFFER_NO_DATA;
    }

    char* pch;
    int found_position;
    pch = strstr(current_pointer_position,str);

    if(pch == NULL){

        if(getIfNool){
            actual_data.append(current_pointer_position,lenBuffer-current_position);
            current_position += lenBuffer-current_position;
            current_pointer_position = buffer + current_position + 1;
            return STRING_BUFFER_READ_REMAIN;
        }
        return STRING_BUFFER_NO_READ;
    }
    else{
        found_position = pch - current_pointer_position ;
        current_position += found_position + strlen(str);
        actual_data.append(current_pointer_position,found_position + strlen(str));
        current_pointer_position = pch + strlen(str);

        return STRING_BUFFER_READ;

    }

}

int StringBuffer::readDataByNum(int len, int& readChar){

    if(current_position == lenBuffer){
        readChar = 0;
        return STRING_BUFFER_NO_DATA;
    }

    if(current_position + len > lenBuffer){
        readChar = lenBuffer-current_position;
        actual_data.append(current_pointer_position,readChar);
        current_position += readChar;
        current_pointer_position = buffer + current_position + 1;

        return STRING_BUFFER_READ_REMAIN;

    }
    else{
        actual_data.append(current_pointer_position,len);
        current_position += len;
        current_pointer_position += len ;
        readChar = len;
        return STRING_BUFFER_READ;
    }

}

int StringBuffer::checkDataByNum(int len){

    if(current_position == lenBuffer) return STRING_BUFFER_NO_DATA;
    if(current_position + len > lenBuffer) return STRING_BUFFER_READ_REMAIN;
    else{
        actual_data.append(current_pointer_position,len);
        return STRING_BUFFER_READ;
    }

}

///*current_position == size_buffer - 1 jest aby nie uwzgledniac juz znaku ktory konczy napis czyli ten null terminated(\0)
/// przy strncpy nalezy przy kopiowaniu znakow nalezy przy dlugosci dodac jeden (len+1) aby mogl objemowac jeszcze znak konczacy stringa
void StringBuffer::write(const char* data, int lenght){


    //std::cout<<"ZNAKKKK "<<(int)(*(data+lenght-1))<<std::endl;
    int len;
    //int len = strlen(data);
    if(data[lenght-1] == 0) {len = lenght - 1;}
    else len = lenght;

   // if(lenBuffer - current_position + len > size_buffer - 1){
        char* pch = new char[lenBuffer - current_position + len + 1];
        size_buffer = lenBuffer - current_position + len + 1 ;
        memcpy(pch,current_pointer_position,lenBuffer - current_position);
        delete [] buffer;
        strncpy(pch + lenBuffer - current_position,data,len);
        buffer = pch;
        lenBuffer = lenBuffer - current_position + len;
        buffer[size_buffer-1] = '\0';
        current_pointer_position = buffer;
        current_position = 0;
//    }else{
//        ///for overlapping memory blocks, memmove is a safer approach than memcpy ->http://www.cplusplus.com/reference/cstring/memcpy/
//        memmove(buffer,current_pointer_position,lenBuffer-current_position);
//        strncpy(buffer + lenBuffer - current_position, data, len);
//        lenBuffer = lenBuffer - current_position + len;
//        current_pointer_position = buffer;
//        current_position = 0;
//    }

}

