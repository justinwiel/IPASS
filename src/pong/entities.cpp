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


#include "entities.hpp"
#include "../../lib/MPU.hpp"




bool within( int x, int a, int b ){
   return ( x >= a ) && ( x <= b );
}

bool ball::overlaps( const sprite & other ){//if the ball is going to hit a wall on the next move it returns true
    bool x_overlap_start = within( 
        start.x ,
        other.start.x,
        other.end.x )
    || within( 
        other.start.x , 
        start.x, 
        end.x
   );
     
   bool y_overlap_start = within( 
        start.y,
        other.start.y , 
        other.end.y
    ) || within( 
        other.start.y , 
        start.y, 
        end.y
    ); 
    bool x_overlap_end = within( 
        end.x ,
        other.start.x,
        other.end.x )
    || within( 
        other.end.x , 
        start.x, 
        end.x
   );
     
   bool y_overlap_end = within( 
        end.y,
        other.start.y , 
        other.end.y
    ) || within( 
        other.end.y , 
        start.y, 
        end.y
    ); 
    bool x_overlap_start_speed = within( 
        start.x -speed.x,
        other.start.x,
        other.end.x )
    || within( 
        other.start.x +speed.x , 
        start.x, 
        end.x
   );
     
    bool y_overlap_start_speed = within( 
        start.y -speed.y ,
        other.start.y , 
        other.end.y
    ) || within( 
      other.start.y +speed.y , 
      start.y, 
      end.y
   );
   bool x_overlap_end_speed = within( 
      end.x +speed.x,  
      other.start.x,
      other.end.x )
    || within( 
      other.end.x - speed.x, 
      start.x, 
      end.x
   );
     
   bool y_overlap_end_speed = within( 
      end.y +speed.y , 
      other.start.y, 
      other.end.y
   ) || within( 
      other.end.y - speed.y  , 
      start.y, 
      end.y
   );
    return (x_overlap_start_speed && y_overlap_start_speed) || (x_overlap_end_speed && y_overlap_end_speed)|| (x_overlap_start && y_overlap_start) || (x_overlap_end && y_overlap_end);
}

void ball::check_next_pos(const sprite & ai){
    if(overlaps(ai)){
        start.x -= 1;
        end.x -= 1;
        start.y -= 1;
        end.y -= 1;
    }
}

void ball::draw() {//draw the ball
    for(int x  = start.x; x <= end.x;x++){
        for(int y = start.y; y <= end.y;y++){
            w.write(xy(x,y));
        }
    }
}

void ball::update() {//move the ball
    start = start + speed; 
    end = end + speed; 
}

void ball::interact(const sprite & other){ //interact to bounce the ball
    if(this != & other){
        if(overlaps(other)){
            speed.x = speed.x * other.bounce.x;
            speed.y = speed.y * other.bounce.y;
        }
    }


}

void ball::reset(){//reset the ball
    start = org_start;
    end = org_end;
    speed = org_speed;
}

void border::draw(){
    for(int x  = start.x; x <= end.x;x++){
        for(int y = start.y; y <= end.y;y++){
            w.write(xy(x,y));
        }
    }
}

void border::update(){

}//empty function to keep the compiler happy

void border::interact(const sprite & other){
    
}//empty function to keep the compiler happy


bool border::overlaps( const sprite & other ){
   //fairly standard bounce function, starts are transposed by 4 to detect collisions before they happen
   bool x_overlap = within( 
      start.x - 4,
      other.start.x,
      other.end.x )
    || within( 
      other.start.x -4, 
      start.x, 
      end.x
   );
     
   bool y_overlap = within( 
      start.y ,
      other.start.y, 
      other.end.y
   ) || within( 
      other.start.y, 
      start.y, 
      end.y
   );
   bool x_overlap_end = within( 
      end.x ,  
      other.start.x - 4,
      other.end.x  )
    || within( 
      other.end.x -4, 
      start.x, 
      end.x
   );
     
   bool y_overlap_end = within( 
      end.y , 
      other.start.y, 
      other.end.y
   ) || within( 
      other.end.y , 
      start.y, 
      end.y
   );
   return (x_overlap && y_overlap) || (x_overlap_end && y_overlap_end);
}

void border::reset(){//reset the item
    start = org_start;
    end = org_end;
}

void deathwall::reset(){//reset the item
    border::reset();
    clear = true;
}
void deathwall::gameOver(){
    auto f = hwlib::font_default_8x8();
    auto terminal = hwlib::terminal_from(w,f);
    w.clear();
    terminal << '\f' << "\n\n\n" << "    Game over" << hwlib::flush;
    clear = false;

}

void deathwall::gameWon(){
    auto f = hwlib::font_default_8x8();
    auto terminal = hwlib::terminal_from(w,f);
    w.clear();
    terminal << '\f' << "\n\n\n" << "     You Win!" << hwlib::flush;
    clear = false;   
}
void deathwall::interact(const sprite & other){//if players wall was hit game over, if ai wall is hit game won
    if(this != & other){
        if(overlaps(other)&&player == 1){
            gameOver();

        }else if(overlaps(other)&&player ==2){
            gameWon();
        }
    }


}