#include <iostream>
#include <locale>
#include <map>
#include <iterator>
#include <cctype>
#include <algorithm>
#include <cstdlib>
#include <set>
#include <cmath>
#include <cstdint>

#include "Advent.hpp"
#include "Intcode.hpp"

#include <boost/range/adaptor/map.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/iterator/transform_iterator.hpp>

Intcode::Memory getProgram()
{
  return { 3,8,1005,8,311,1106,0,11,0,0,0,104,1,104,0,3,8,102,-1,8,10,101,1,10,10,4,10,1008,8,1,10,4,10,1001,8,0,29,1006,0,98,2,1005,8,10,1,1107,11,10,3,8,102,-1,8,10,1001,10,1,10,4,10,1008,8,0,10,4,10,101,0,8,62,1006,0,27,2,1002,12,10,3,8,1002,8,-1,10,1001,10,1,10,4,10,108,0,8,10,4,10,1002,8,1,90,1,1006,1,10,2,1,20,10,3,8,102,-1,8,10,1001,10,1,10,4,10,1008,8,1,10,4,10,102,1,8,121,1,1003,5,10,1,1003,12,10,3,8,102,-1,8,10,101,1,10,10,4,10,1008,8,1,10,4,10,1002,8,1,151,1006,0,17,3,8,102,-1,8,10,1001,10,1,10,4,10,108,0,8,10,4,10,1002,8,1,175,3,8,102,-1,8,10,1001,10,1,10,4,10,108,1,8,10,4,10,101,0,8,197,2,6,14,10,1006,0,92,1006,0,4,3,8,1002,8,-1,10,101,1,10,10,4,10,108,0,8,10,4,10,1001,8,0,229,1006,0,21,2,102,17,10,3,8,1002,8,-1,10,101,1,10,10,4,10,1008,8,1,10,4,10,1001,8,0,259,3,8,102,-1,8,10,1001,10,1,10,4,10,108,0,8,10,4,10,102,1,8,280,1006,0,58,1006,0,21,2,6,11,10,101,1,9,9,1007,9,948,10,1005,10,15,99,109,633,104,0,104,1,21101,937150919572,0,1,21102,328,1,0,1105,1,432,21101,0,387394675496,1,21102,1,339,0,1106,0,432,3,10,104,0,104,1,3,10,104,0,104,0,3,10,104,0,104,1,3,10,104,0,104,1,3,10,104,0,104,0,3,10,104,0,104,1,21102,46325083283,1,1,21102,1,386,0,1106,0,432,21101,0,179519401051,1,21102,397,1,0,1106,0,432,3,10,104,0,104,0,3,10,104,0,104,0,21102,1,868410348308,1,21102,1,420,0,1105,1,432,21102,718086501140,1,1,21102,1,431,0,1105,1,432,99,109,2,22101,0,-1,1,21101,40,0,2,21101,0,463,3,21101,453,0,0,1106,0,496,109,-2,2105,1,0,0,1,0,0,1,109,2,3,10,204,-1,1001,458,459,474,4,0,1001,458,1,458,108,4,458,10,1006,10,490,1101,0,0,458,109,-2,2105,1,0,0,109,4,2102,1,-1,495,1207,-3,0,10,1006,10,513,21102,0,1,-3,22102,1,-3,1,22102,1,-2,2,21102,1,1,3,21102,1,532,0,1105,1,537,109,-4,2105,1,0,109,5,1207,-3,1,10,1006,10,560,2207,-4,-2,10,1006,10,560,22101,0,-4,-4,1105,1,628,22102,1,-4,1,21201,-3,-1,2,21202,-2,2,3,21102,1,579,0,1105,1,537,22101,0,1,-4,21102,1,1,-1,2207,-4,-2,10,1006,10,598,21102,1,0,-1,22202,-2,-1,-2,2107,0,-3,10,1006,10,620,22102,1,-1,1,21102,1,620,0,105,1,495,21202,-2,-1,-2,22201,-4,-2,-4,109,-5,2106,0,0 };
}

enum Orientation
  {
   LoopToWest,
   North,
   East,
   South,
   West,
   LoopToNorth
  };

enum Turn
  {
   Left = -1,
   Right = 1
  };

enum class Color
  {
   Black,
   White
  };

using Point = std::pair<int, int>;
using Field = std::map<Point, Color>;

struct Robot : public Intcode::Queue
{
  Robot(Field& field)
    : field(field)
  {}

  enum class State
    {
     Idle,
     WaitingForColor,
     WaitingForTurn,
    };

  std::int64_t pop() override
  {
    std::int64_t result = -1;
    switch(state)
      {
      case State::Idle:
	result = static_cast<std::int64_t>(field[Point{x, y}]);
	state = State::WaitingForColor;
	break;
      default:
	throw std::runtime_error("Unexpected operation during pop, wrong state");
	break;
      }
    return result;
  }
  
  void push(std::int64_t value) override
  {
    switch(state)
      {
      case State::WaitingForColor:
	paint(static_cast<Color>(value));
	state = State::WaitingForTurn;
	break;
      case State::WaitingForTurn:
	{
	  auto t = ((value != 0) ? Turn::Right : Turn::Left);
	  turn(t);
	  state = State::Idle;
	}
	break;
      default:
	throw std::runtime_error("Push occurred in idle state");
	break;
      }
  }

  void turn(Turn t)
  {
    d = static_cast<Orientation>(static_cast<int>(d) + static_cast<int>(t));

    if(d == Orientation::LoopToNorth)
      {
	d = Orientation::North;
      }
    else if (d == Orientation::LoopToWest)
      {
	d = Orientation::West;
      }
    
    // after turn, move.
    move();
  }

  void move()
  {
    auto d_x = 0, d_y = 0;
    switch(d)
      {
      case Orientation::North:
	d_y = -1;
	break;
      case Orientation::East:
	d_x = 1;
	break;
      case Orientation::South:
	d_y = 1;
	break;
      case Orientation::West:
	d_x = -1;
	break;
      default:
	std::cout << static_cast<int>(d) << std::endl;
	throw std::runtime_error("Invalid orientation during move");
      }
    x += d_x;
    y += d_y;
  }

  void paint(Color c)
  {
    auto key = std::make_pair(x, y);
    field[key] = c;
  }
  
  Field& field;

  State state = State::Idle;

  Orientation d = Orientation::North;
  int x = 0;
  int y = 0;
};

int main()
{
  auto& input = std::cin;
  input.imbue(std::locale(input.getloc(), new Advent::ctype));

  Field field;
  field[Point{0,0}] = Color::White;

  Robot robot(field);

  Intcode::ConsoleQueue queue;
  Intcode::Context context { getProgram (), robot, robot };

  Intcode::run(context);

  Advent::printMap(field.begin(), field.end(),
  		   [](auto c)
  		   {
  		     return c == Color::White ? '@' : ' ';
  		   });

  return 0;
}
