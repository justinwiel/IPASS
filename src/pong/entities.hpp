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

#ifndef BAL_HPP
#define BAL_HPP
#include "hwlib.hpp"
using namespace hwlib;



class sprite {
protected:
    window & w;

public:
    xy start;
    xy end;
    xy bounce;
    xy org_end = end;
    xy org_start = start;
    sprite(window & w,xy  start, xy  end, xy bounce):
        w(w),
        start(start),
        end(end),
        bounce(bounce)
        {}
    virtual void draw() = 0;
    virtual void update() = 0;
    virtual void interact(const sprite & other);
    virtual void reset() = 0;

};

class ball :public sprite{
protected:
public:
    xy speed;
    xy org_speed = speed;
    ball(window & w,xy  start, xy  end, xy  speed, xy bounce= xy(0,0)):
        sprite(w,  start,  end, bounce),
        speed(speed)
        {}
    void draw() ;
    void update() ;
    void check_next_pos( const sprite & ai);
    bool overlaps(const sprite & other);
    void interact(const sprite & other) ;
    void reset();

    
    
};

class border :public sprite{
public:
    border(window & w,xy  start, xy  end, xy bounce):
        sprite(w,  start,  end, bounce)
        {}
    virtual void update() override;
    virtual void interact(const sprite & other)override ;
    virtual void draw() ;

    bool overlaps(const sprite & other);
    virtual void reset();

};

class deathwall :public border{
private:
    int player;
public:
    bool clear = true;
    deathwall(window & w, xy start, xy end, int player):
        border(w,start,end,xy(0,0)),
        player(player)
        {}
    void interact(const sprite & other) ;
    void gameOver() ;
    void gameWon();
    void reset() override;
    
};
#endif