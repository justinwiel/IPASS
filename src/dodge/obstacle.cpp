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

#include "obstacle.hpp"
#include "../../lib/MPU.hpp"

using namespace dodge;

void obstacle::draw()
{
    for (int x = start.x; x <= end.x; x++)
    {
        for (int y = start.y; y <= end.y; y++)
        {
            w.write(xy(x, y)); //write a pixel everwhere you need 
        }
    }
}
void obstacle::reset()
{
    start = org_start; //set the item back to it's original values
    end = org_end;
}
void obstacle::update() //move obstacle with the assigned speed
{
    start.x -= speed.x;
    end.x -= speed.x;
    start.y -= speed.y;
    end.y -= speed.y;
}

void obstacle::interact(sprite &other)
{
} //empty function to keep the compiler happy

void Player::draw()
{
    for (int x = start.x; x <= end.x; x++)
    {
        for (int y = start.y; y <= end.y; y++)
        {
            if ((y < eye_start.y) || (y > eye_end.y) || (x < eye_start.x) || (x > eye_end.x))
            {
                w.write(xy(x, y));
            }
        }
    }
}

void Player::change_pos_x(int8_t val)//change x  postion
{
    start_prev.x = start.x;
    end_prev.x = end.x;
    start.x += val;
    end.x += val;
    eye_start.x += val;
    eye_end.x += val;
}

void Player::change_pos_y(int8_t val) //change y position
{
    start_prev.y = start.y;
    end_prev.y = end.y;
    start.y += val;
    end.y += val;
    eye_start.y += val;
    eye_end.y += val;
}

void Player::update()
{
    chip.setup(3);
    auto data = chip.getAccdata_scale(10); //update current postion with accelerometer orientation
    change_pos_y(data.z);
    change_pos_x(-data.y);
    check_next_pos();
}

void Player::check_next_pos()
{
    while (end.y > 58) // if y would be out of bounds make sure it's not 
    {
        start.y -= 1;
        end.y -= 1;
        eye_start.y -= 1;
        eye_end.y -= 1;
    }
    while (start.y < 5)
    {
        start.y += 1;
        end.y += 1;
        eye_start.y += 1;
        eye_end.y += 1;
    }
    while (end.x > 126) // if x would be out of bounds make sure it's not 
    {
        start.x -= 1;
        end.x -= 1;
        eye_start.x -= 1;
        eye_end.x -= 1;
    }
    while (start.x < 5)
    {
        start.x += 1;
        end.x += 1;
        eye_start.x += 1;
        eye_end.x += 1;
    }
}

void Player::interact(sprite &other)
{
    if (this != &other)
    {
        if (overlaps(other))
        {
            hit = true;
        }
    }
}

bool obstacle::within(int x, int a, int b)
{
    return (x >= a) && (x <= b);
}

bool Player::overlaps(sprite &other)
{// standard collision checking
    bool x_overlap_start = within(
                               start.x,
                               other.start.x,
                               other.end.x) ||
                           within(
                               other.start.x,
                               start.x,
                               end.x);

    bool y_overlap_start = within(
                               start.y,
                               other.start.y,
                               other.end.y) ||
                           within(
                               other.start.y,
                               start.y,
                               end.y);
    bool x_overlap_end = within(
                             end.x,
                             other.start.x,
                             other.end.x) ||
                         within(
                             other.end.x,
                             start.x,
                             end.x);

    bool y_overlap_end = within(
                             end.y,
                             other.start.y,
                             other.end.y) ||
                         within(
                             other.end.y,
                             start.y,
                             end.y);

    return  (x_overlap_start && y_overlap_start) || (x_overlap_end && y_overlap_end);
}
void Player::gameOver()//print game over
{
    auto f = hwlib::font_default_8x8();
    auto terminal = hwlib::terminal_from(w, f);
    w.clear();
    terminal << '\f' << "\n\n\n"
             << "    Game over" << hwlib::flush;
}

void Player::reset()//reset the player
{
    start = org_start;
    end = org_end;
    eye_start = org_start_eye;
    eye_end = org_end_eye;
    hit = false;
}

void Player::gameWon()//print a win message 
{
    auto f = hwlib::font_default_8x8();
    auto terminal = hwlib::terminal_from(w, f);
    w.clear();
    terminal << '\f' << "\n\n\n"
             << "     You Win!" << hwlib::flush;
}
