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

#include "hwlib.hpp"
#include <array>
#include "src/pong/enemy.hpp"
#include "src/pong/entities.hpp"
#include "lib/MPU.hpp"
#include "src/pong/player.hpp"
#include "src/dodge/obstacle.hpp"

void play_pong(hwlib::glcd_oled &oled, MPU6050 &chip, hwlib::pin_in &button)
{
  auto top = border(oled, hwlib::xy(0, 0), hwlib::xy(128, 0), hwlib::xy(1, -1));
  auto bottom = border(oled, hwlib::xy(0, oled.size.y - 1), hwlib::xy(128, oled.size.y - 1), hwlib::xy(1, -1));
  auto goal1 = deathwall(oled, hwlib::xy(1, 1), hwlib::xy(1, 62), 1);
  auto goal2 = deathwall(oled, hwlib::xy(127, 1), hwlib::xy(127, 62), 2);
  auto ai = enemy(oled, hwlib::xy(117, 18), hwlib::xy(122, 36), hwlib::xy(-1, 1), 4);
  auto me = Player(oled, hwlib::xy(6, 18), hwlib::xy(10, 36), hwlib::xy(-1, 1));
  ball pong(oled, hwlib::xy(60, 28), hwlib::xy(64, 32), hwlib::xy(4, 5));
  std::array<sprite *, 7> objects = {&pong, &top, &bottom, &goal1, &goal2, &ai, &me};

  for (;;)
  {
    while (goal1.clear && goal2.clear)
    {
      if (button.read())
      {
        hwlib::wait_ms(100);
        return;
      }

      oled.clear();
      for (auto &p : objects)
      {
        p->draw();
      }
      oled.flush();
      for (auto &p : objects)
      {
        p->update();
      }
      ai.get_target(pong);
      ai.check_next_pos(top, bottom);
      me.get_movement();
      me.check_next_pos(top, bottom);
      pong.check_next_pos(ai);
      for (auto &p : objects)
      {
        for (auto &other : objects)
        {
          if (p != other)
          {
            p->interact(*other);
          }
        }
      }
      if (!(goal1.clear) || !(goal2.clear))
      {
        for (auto &p : objects)
        {
          p->reset();
        }
        hwlib::wait_ms(1000);
      }
    }
  }
}

void play_dodge(MPU6050 &chip, hwlib::glcd_oled &oled, hwlib::pin_in &button)
{
  auto player = dodge::Player(oled, hwlib::xy(60, 28), hwlib::xy(68, 36), chip, hwlib::xy(65, 30), hwlib::xy(67, 32));

  auto target = dodge::obstacle(oled, hwlib::xy(80, 0), hwlib::xy(90, 28), hwlib::xy(1, 0));
  auto target2 = dodge::obstacle(oled, hwlib::xy(130, 24), hwlib::xy(140, 64), hwlib::xy(1, 0));
  auto target3 = dodge::obstacle(oled, hwlib::xy(180, 20), hwlib::xy(190, 45), hwlib::xy(1, 0));
  auto target4 = dodge::obstacle(oled, hwlib::xy(40, 60), hwlib::xy(60, 80), hwlib::xy(0, 1));
  auto target5 = dodge::obstacle(oled, hwlib::xy(80, 60), hwlib::xy(120, 80), hwlib::xy(0, 1));
  std::array<dodge::sprite *, 6> objects = {&player, &target, &target2, &target3, &target4, &target5};
  for (;;)
  {
    if (button.read())
    {
      return;
    }
    oled.clear();
    for (auto &p : objects)
    {
      p->draw();
    }
    oled.flush();
    for (auto &p : objects)
    {
      p->update();
    }
    for (auto &p : objects)
    {
      for (auto &other : objects)
      {
        if (p != other)
        {
          p->interact(*other);
        }
      }
    }
    if (player.hit)
    {
      player.gameOver();
      hwlib::wait_ms(2000);
      for (auto &p : objects)
      {
        p->reset();
      }
    }
    if (target3.end.x < 0)
    {
      player.gameWon();
      hwlib::wait_ms(2000);
      for (auto &p : objects)
      {
        p->reset();
      }
    }
  }
}

int main()
{
  auto scl = hwlib::target::pin_oc(hwlib::target::pins::scl);
  auto sda = hwlib::target::pin_oc(hwlib::target::pins::sda);
  auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda(scl, sda);
  auto green_button = hwlib::target::pin_in(hwlib::target::pins::d7);
  auto red_button = hwlib::target::pin_in(hwlib::target::pins::d11);
  auto yellow_button = hwlib::target::pin_in(hwlib::target::pins::d12);
  auto black_button = hwlib::target::pin_in(hwlib::target::pins::d10);
  auto green_led = hwlib::target::pin_out(hwlib::target::pins::d9);
  auto yellow_led = hwlib::target::pin_out(hwlib::target::pins::d8);
  auto oled = hwlib::glcd_oled(i2c_bus, 0x3c);
  auto chip = MPU6050(i2c_bus, 0);
  oled.clear();
  auto f1 = hwlib::font_default_8x8();
  auto d1 = hwlib::terminal_from(oled, f1);
  for (;;)
  {
    d1 << '\f' << "select a game, \n\ngreen pong, \n\nred  dodge, \n\npress yellow \nfor testing" << hwlib::flush;
    if (green_button.read())
    {
      play_pong(oled, chip, black_button);
    }
    if (red_button.read())
    {
      play_dodge(chip, oled, black_button);
    }
    if (yellow_button.read())
    {
      chip.test(black_button, oled,green_led,yellow_led);
    }
  }
}