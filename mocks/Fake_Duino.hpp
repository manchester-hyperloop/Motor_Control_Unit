//
//  Fake_Duino.hpp
//  Hyperloop Door_Unit
//
//  Created by Harry O'Brien on 17/11/2020.
//

#ifndef Mocks_Fake_Duino_h
#define Mocks_Fake_Duino_h

#ifdef UNIT_TEST

#include <stdint.h>
#include <map>
#include <utility>
#include <iostream>

#define HIGH 0x1
#define LOW 0x0

#define OUTPUT 0x0
#define INPUT 0x1
#define INPUT_PULLUP 0x2

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define abs(x) ((x) > 0 ? (x) : -(x))
#define constrain(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))
#define round(x) ((x) >= 0 ? (long)((x) + 0.5) : (long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x) * (x))

enum IO_State
{
    IO_UNDEFINED = -1, // (undefined behaviour)
    IO_HIGH = 0,
    IO_LOW = 1
};

enum PinMode
{
    PIN_UNDEFINED = -1,
    PIN_OUTPUT,
    PIN_INPUT,
    PIN_INPUT_PULLUP
};

class IO_Mock
{
    IO_State state = IO_UNDEFINED;
    PinMode mode = PIN_UNDEFINED;

    uint8_t pin = -1;

public:
    IO_Mock() {}

    IO_Mock(PinMode pinMode)
        : mode(pinMode)
    {
    }

    IO_Mock(uint8_t _pin, PinMode pinMode)
        : pin(_pin), mode(pinMode)
    {
    }

    void setHigh()
    {
        state = IO_HIGH;
    }

    void setLow()
    {
        state = IO_LOW;
    }

    IO_State getState() const
    {
        return state;
    }
};

static std::map<uint8_t, IO_Mock> initialisedPins;

static void digitalWrite(uint8_t pin, uint8_t state)
{
    try
    {
        IO_Mock &mock = initialisedPins[pin];
        if (state == 0x01)
        {
            mock.setHigh();
        }
        else
        {
            mock.setLow();
        }
    }
    catch (...)
    {
        std::cerr << "Pin " << pin << " not initialised yet!" << std::endl;
    }
}

static bool digitalRead(uint8_t pin)
{
    try
    {
        IO_Mock mock = initialisedPins[pin];
        return (mock.getState() == IO_State::IO_HIGH ? 1 : 0);
    }
    catch (...)
    {
        std::cerr << "Pin " << pin << " not initialised yet!" << std::endl;
    }

    return 0;
}

static void pinMode(uint8_t pin, uint8_t mode)
{
    IO_Mock mock(pin, (PinMode)mode);

    if ((PinMode)mode == PinMode::PIN_INPUT_PULLUP)
    {
        mock.setHigh();
    }

    initialisedPins.insert(std::pair<uint8_t, IO_Mock>(pin, mock));
}

#endif /* UNIT_TEST */
#endif /* Fake_Duino_h */
