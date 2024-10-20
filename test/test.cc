#include <snake.hpp>
#include <iostream>
#include <iomanip>

Direction RaylibGetDirection(Direction & OldDirection)
{
  Direction NewDirection {OldDirection};
  if (IsKeyDown(KEY_RIGHT)) NewDirection = Right;
  if (IsKeyDown(KEY_LEFT))  NewDirection = Left;
  if (IsKeyDown(KEY_DOWN))  NewDirection = Down;
  if (IsKeyDown(KEY_UP))    NewDirection = Up;
  return NewDirection;
}


int main (int argc, char ** argv)
{
  // std::vector<Coord> Body;
  // Coord f (1, 1);
  // Coord g (2, 1);
  // Coord h (3, 1);
  // Coord I (4, 1);

  // Body.push_back(f);
  // Body.push_back(g);
  // Body.push_back(h);
  // Body.push_back(I);



  // for (auto const & ele : Body) std::cout << ele << "\n";
  // std::cout << std::endl;

  Snake AS;
  
  Map map;
  std::cout << AS << std::endl;

  Direction RR {Down};
  InitWindow(1620, 1080, "Title");
  SetTargetFPS(10);

  while (!WindowShouldClose()) 
  {
    BeginDrawing();
    ClearBackground(WHITE);

    RaylibDraw(map, AS);
    
    Bool F {false};

    RR = RaylibGetDirection(RR);
    RaylibDraw(map);


    if (GAME == NotOver) {
      AS.move(RR, F, map);
    } else {
      DrawRectangle(0, 0, 1920, 1080, {0, 0, 0, 128});
      DrawText("Game Over!", 560-100, 500, 40, BLACK);
    }

    

    EndDrawing();
  }
  CloseWindow();



  return 0;
}