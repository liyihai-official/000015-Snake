#pragma once
#include <vector>
#include <memory>
#include <raylib.h>
#include <random>
#include <type_traits>
#include <algorithm>

#include <iostream>
#include <iomanip>


using Float   = float;
using World   = int16_t;
using Integer = int32_t;
using String  = std::string;
using Bool    = bool;


enum Direction : World { Up, U, Down, D, Right, R, Left, L };

enum GameStatus : Bool { IsOver, NotOver };

GameStatus GAME {NotOver};


template<typename T>
class RandomGenerator {
  static_assert(std::is_arithmetic<T>::value, "T must be a numeric type.");

  public:
  RandomGenerator(const T min_val, const T max_val)
: distribution(min_val, max_val) 
  {
std::random_device rd;  
generator.seed(rd());   
  }

  T getRandom() { return distribution(generator); }

  private:
  std::mt19937 generator;   
  std::conditional_t< std::is_integral<T>::value,
                      std::uniform_int_distribution<T>,
                      std::uniform_real_distribution<T>
                    > distribution;
};



struct Map;

/// @brief 
class Coord 
{
  public:
  Integer x, y;

  public:
  Coord() = delete;
  Coord(Integer x, Integer y) noexcept : x {x}, y {y} {}

  Coord(const Coord &)  noexcept;
  Coord(Coord &&)       noexcept;

  Coord& operator=(const Coord &) noexcept;
  Coord& operator=(Coord &&)      noexcept;

  Coord& operator+=(const Coord &);
  Coord& operator+=(Coord &&);

  Coord operator+(const Coord &);
  Coord operator+(Coord &&);

  Bool operator<(const Coord &);
  Bool operator<(Coord &&);

  Bool operator<=(const Coord &);
  Bool operator<=(Coord &&);

  Bool operator>=(const Coord &);
  Bool operator>=(Coord &&);

  Bool operator==(const Coord &);
  Bool operator==(Coord &&);

  Bool operator!=(const Coord &);
  Bool operator!=(Coord &&);

  friend std::ostream& operator<< (std::ostream &, const Coord &);
  friend void raylibDraw(const Map &, const Coord &);

}; // end of struct Coord


struct Map 
{
  static const Integer X_SIZE     {1080};
  static const Integer Y_SIZE     {1080};
  static const Integer CELL_SIZE  {60};
  static const Integer X_NUM      {X_SIZE / CELL_SIZE};
  static const Integer Y_NUM      {Y_SIZE / CELL_SIZE};

  Coord Food;
  Coord Original {0, 0}, Maximum {Y_NUM, X_NUM};

  RandomGenerator<Integer> RNG_X, RNG_Y; 

  Map() : Food {Coord {10, 0}}, RNG_X (0, X_NUM-1), RNG_Y (0, Y_NUM-1)  {};


  void NewFood()
  {
    Food = Coord{RNG_Y.getRandom(), RNG_X.getRandom()};
  }

  friend class Coord;
  friend void RaylibDraw(const Map &);
}; // other end of struct Map





/// @brief 
struct Snake
{
  std::vector<Coord> Body;
  unsigned long int Length;

  Direction direct;

  Snake() noexcept : Body {{Coord(0, 0), Coord(1, 0)}}, direct {Right}, Length {Body.size()} {}

  void move(Direction &, Bool &, Map &);

  friend std::ostream& operator<<(std::ostream &, const Snake &);
  friend void RaylibDraw(const Map &, const Snake &, const Map &);

}; // end of struct Snake



inline 
Coord::Coord(const Coord & other) 
noexcept
: x {other.x}, y {other.y}
{ std::cout << "Calling Copy Constructor. " << std::endl; }

inline 
Coord::Coord(Coord && other) 
noexcept
: x { std::move(other.x) }, y { std::move(other.y) }
{ std::cout << "Calling Move Constructor. " << std::endl; }


///
/// Operators 
///

inline
Coord& Coord::operator=(const Coord & other) 
noexcept
{
  if (this != &other) x = other.x; y = other.y;
  return *this;
}

inline 
Coord& Coord::operator=(Coord && other)
noexcept
{
  if (this != &other)
  {
x = std::move(other.x);
y = std::move(other.y);
  }
  return *this;
}

inline 
Coord 
Coord::operator+(const Coord & other)
{
  Coord res(x+other.x, y+other.y);
  return res;
}

inline 
Coord
Coord::operator+(Coord && other)
{
  Coord res(x+other.x, y+other.y);
  return res;
}

inline
Coord &
Coord::operator+=(const Coord & other)
{
  x += other.x; y += other.y;
  return *this;
}

inline
Coord &
Coord::operator+=(Coord && other)
{
  x += other.x; y += other.y;
  return *this;
}

inline 
Bool Coord::operator<(const Coord & other)
{
  Bool res {false};
  if (x < other.x && y < other.y) res = true;
  return res;
}

inline 
Bool Coord::operator<(Coord && other)
{
  Bool res {false};
  if (x < other.x && y < other.y) res = true;
  return res;
}


inline 
Bool Coord::operator<=(const Coord & other)
{
  Bool res {false};
  if (x <= other.x && y <= other.y) res = true;
  return res;
}

inline 
Bool Coord::operator<=(Coord && other)
{
  Bool res {false};
  if (x <= other.x && y <= other.y) res = true;
  return res;
}

inline 
Bool Coord::operator>=(const Coord & other)
{
  Bool res {false};
  if (!(x < other.x) && !(y < other.y)) res = true;
  return res;
}

inline 
Bool Coord::operator>=(Coord && other)
{
  Bool res {false};
  if (!(x < other.x) && !(y < other.y)) res = true;
  return res;
}

inline 
Bool Coord::operator==(const Coord & other)
{
  Bool res {false};
  if (other.x == x && other.y == y) res = true;
  return res;
}

inline 
Bool Coord::operator==(Coord && other) 
{
  Bool res {false};
  if (other.x == x && other.y == y) res = true;
  return res;
}


inline 
Bool Coord::operator!=(const Coord & other) 
{
  Bool res {!(*this == other)};
  return res;
}

inline 
Bool Coord::operator!=(Coord && other)
{
  Bool res {!(*this == other)};
  return res;
}

///
/// Modifiers
///
inline 
void 
Snake::move(Direction & direction, Bool & HasFood, Map & map)
{
  direct = direction;
  Coord New {Body.front()};
  switch (direct)
  {
case Up:    case U: New += Coord(-1, 0);  break;
case Down:  case D: New += Coord(1, 0);   break;
case Right: case R: New += Coord(0, 1);   break;
case Left:  case L: New += Coord(0, -1);  break;
default:                                  break;
  }
  if (New < map.Maximum && New >= map.Original) 
  {
    if (Length > 4)
    {
      auto it = std::find(Body.begin(), Body.end(), New);
      if (it != Body.end()) GAME = IsOver;
    }

    Body.insert(Body.begin(), New);
    Length = Body.size();
    if (New != map.Food) {Body.pop_back();}
    else { map.NewFood(); }
  }
  else GAME = IsOver;
}


/// 
///
/// Friend Functions          /// 
///   Operator << std::iostream
///   Raylib Friends
///
///

std::ostream & operator<<(std::ostream & os, const Coord & in)
{
  os << "[" << in.x << ", " << in.y << "]";
  return os;
}

std::ostream & operator<<(std::ostream & os, const Snake & in)
{
  for (auto const & ele : in.Body) os << ele << " \n ";
  return os;
}


void RaylibDraw(const Map & map, const Coord & in)
{
  DrawRectangle(
    in.y*map.CELL_SIZE, 
    in.x*map.CELL_SIZE,
    map.CELL_SIZE, map.CELL_SIZE, 
    GRAY
  );
}

void RaylibDraw(const Map & map, const Snake & in)
{
  for (auto const & ele : in.Body) 
  { RaylibDraw(map, ele); }
}


void RaylibDraw(const Map & map)
{
  const Integer X_NUM {map.X_SIZE / map.CELL_SIZE};
  const Integer Y_NUM {map.Y_SIZE / map.CELL_SIZE};

  for (Integer i = 0; i < X_NUM; ++i) 
  for (Integer j = 0; j < Y_NUM; ++j)
      DrawRectangleLines(
        i*map.CELL_SIZE, 
        j*map.CELL_SIZE, 
        map.CELL_SIZE, 
        map.CELL_SIZE, GRAY);

  RaylibDraw(map, map.Food);
}