//---------------------------------------------------------------------------

#ifndef StringBufferH
#define StringBufferH

#define STRING_BUFFER_READ 2 /* odpowiedz ktora informuje ze udalo sie zwrocic */
#define STRING_BUFFER_READ_REMAIN 1 /* odpowiedz ktora informuje ze nie znalazlo i zwrocilo reszte*/
#define STRING_BUFFER_NO_DATA -1 /* odpowiedz ktora informuje ze nie ma nic w buferze*/
#define STRING_BUFFER_NO_READ 0

#include <iostream>
#include <string>
#include <string.h>



class StringBuffer
{
    char* buffer;
    int size_buffer;
    int current_position;
    int lenBuffer;
    int found_char;
    char* current_pointer_position;
    std::string actual_data;

public:
    StringBuffer(const char* data, int len);
    ~StringBuffer();

    ///Wyszukuje i zwraca fragment z bufera do pierwszego pojawienia sie podanych w argumencie substringa
    /// bool getifNool - warunek pytajacy czy jesli nie znajdzie danego fragmentu to ma odczytywac reszte czy nic nie robic
    int readDataByChr(const char* str, bool getIfNool);

    ///zczytuje i zwraca z bufera tyle znakow ile podano w parametrze
    ///int& readChar - zwraca ilosc znakow odczytanych
    int readDataByNum(int len, int& readChar);

    ///pobranie z bufera bajtow bez zmieniania pozycji w buforze
    int checkDataByNum(int len);

    ///Zapisuje nowe dane do bufera i poprzednie sie zeruja
    void write(const char* data, int lenght);

    ///Pobiera aktualne dane i zeruje wartosc w tej klasie
    std::string getData(){ return std::move(actual_data); }

    ///Zwraca rozmiar aktualnych danych
    int getSizeData() { return actual_data.length(); }

    ///Czysci aktualne dane
    void flush(){ actual_data = ""; }
//std::cout<<actual_data<<std::endl;
private:
    ///Funkcja typu strstr ktora oprucz wyszukiwania w danej tablicy znakow zmiennej x stringa znakow i zwracania wskaznika
    ///jeszcze ma taka zaleznosc ze w przypadku konca buffera i czesc tych znakow jest juz znaleziona to do zmiennej foundChar ustawia
    ///liczbe znakow ktora moze byc mozliwa przy kolejnym wywolaniu.


};
#endif
