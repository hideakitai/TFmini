#pragma once

#ifndef TFMINI_H
#define TFMINI_H

class TFmini
{
    struct Packet
    {
        union {
            uint8_t b[2];
            uint16_t i;
        } distance;
        union {
            uint8_t b[2];
            uint16_t i;
        } strength;
        uint8_t int_time;
        uint8_t sum;

        void clear() { distance.i = strength.i = int_time = sum = 0; }
    };

    enum class State
    {
        HEAD_L, HEAD_H, DIST_L, DIST_H, STRENGTH_L, STRENGTH_H, INT_TIME, RESERVED, CHECKSUM
    };

    static const uint8_t FRAME_HEADER = 0x59;

public:

    static const uint32_t BAUDRATE = 115200;

    TFmini() {}

    void attach(Stream& s)
    {
        stream_ = &s;
        setMode();
    }

    bool available()
    {
        if (!stream_) return false;

        update();
        if (b_available)
        {
            b_available = false;
            return true;
        }
        else
            return false;
    }

    uint16_t getDistance() const { return packet.distance.i; }
    uint16_t getStrength() const { return packet.strength.i; }
    uint8_t getIntegrationTime() const { return packet.int_time; }

private:

    void setMode()
    {
        delay(100);
        // standard mode
        stream_->write((uint8_t)0x42);
        stream_->write((uint8_t)0x57);
        stream_->write((uint8_t)0x02);
        stream_->write((uint8_t)0x00);
        stream_->write((uint8_t)0x00);
        stream_->write((uint8_t)0x00);
        stream_->write((uint8_t)0x01);
        stream_->write((uint8_t)0x06);
        delay(100);
    }

    void update()
    {
        while(stream_->available())
        {
            uint8_t data = (uint8_t)stream_->read();

            if (state != State::CHECKSUM) buffer.sum += data;

            switch(state)
            {
                case State::HEAD_L:
                {
                    reset();
                    buffer.sum = data;
                    if (data == FRAME_HEADER) state = State::HEAD_H;
                    break;
                }
                case State::HEAD_H:
                {
                    if (data == FRAME_HEADER) state = State::DIST_L;
                    else                      state = State::HEAD_L;
                    break;
                }
                case State::DIST_L:
                {
                    buffer.distance.b[0] = data;
                    state = State::DIST_H;
                    break;
                }
                case State::DIST_H:
                {
                    buffer.distance.b[1] = data;
                    state = State::STRENGTH_L;
                    break;
                }
                case State::STRENGTH_L:
                {
                    buffer.strength.b[0] = data;
                    state = State::STRENGTH_H;
                    break;
                }
                case State::STRENGTH_H:
                {
                    buffer.strength.b[1] = data;
                    state = State::INT_TIME;
                    break;
                }
                case State::INT_TIME:
                {
                    buffer.int_time = data;
                    state = State::RESERVED;
                    break;
                }
                case State::RESERVED:
                {
                    state = State::CHECKSUM;
                    break;
                }
                case State::CHECKSUM:
                {
                    if (buffer.sum == data)
                    {
                        packet = buffer;
                        b_available = true;
                    }
                    else
                    {
                        b_available = false;
                    }
                    reset();
                    break;
                }
                default:
                {
                    reset();
                    break;
                }
            }
        }
    }

    void reset()
    {
        buffer.clear();
        state = State::HEAD_L;
    }

    Packet packet;
    Packet buffer;
    State state;

    bool b_available;
    Stream* stream_;
};

#endif // TFMINI_H